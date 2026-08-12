/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GameObjectAI.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SharedDefines.h"
#include "Util.h"
#include <unordered_set>

enum GilneasMerchantSquare
{
    NPC_FRIGHTENED_CITIZEN     = 35836,
    NPC_RAMPAGING_WORGEN       = 34884,
    NPC_MARKET_HOMES_EVACUATED = 35830,

    EVENT_SUMMON_TTL           = 300000,
    EVENT_CITIZEN_TTL          = 6000,
    EVENT_WORGEN_DELAY         = 2500,
    EVENT_DOOR_TIMER           = 8000,
};

// Centroid of every Merchant Square Door spawn (gameobject entry 195327) - a
// real, open plaza point every citizen can reliably path to, regardless of
// which door they came out of.
float const MerchantSquareCenterX = -1507.74f;
float const MerchantSquareCenterY = 1381.43f;
float const MerchantSquareCenterZ = 36.58f;

char const* const FrightenedCitizenShouts[] =
{
    "Flee! They're everywhere!",
    "It's coming right for me!",
    "Protect me, please!",
    "This place isn't safe. Let's leave!",
    "Worgen! Worgen everywhere!",
};

/*######
## go_merchant_square_door
######*/

class go_merchant_square_door : public GameObjectScript
{
public:
    go_merchant_square_door() : GameObjectScript("go_merchant_square_door") { }

    struct go_merchant_square_doorAI : public GameObjectAI
    {
        go_merchant_square_doorAI(GameObject* gameObject) : GameObjectAI(gameObject),
            _doorCloseTimer(0), _worgenTimer(0), _eventPending(false), _chaseActive(false), _chaseRefreshTimer(0),
            _playerGUID(0), _citizenGUID(0), _worgenGUID(0), _worgenX(0), _worgenY(0), _worgenZ(0), _angle(0) { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (_doorCloseTimer)
            {
                if (_doorCloseTimer <= diff)
                {
                    if (go->GetGoState() == GOState::GO_STATE_ACTIVE)
                        go->SetGoState(GOState::GO_STATE_READY);
                    _doorCloseTimer = 0;
                }
                else
                    _doorCloseTimer -= diff;
            }

            if (_eventPending)
            {
                if (_worgenTimer <= diff)
                {
                    _eventPending = false;

                    if (Player* player = ObjectAccessor::GetPlayer(*go, _playerGUID))
                    {
                        player->KilledMonsterCredit(NPC_MARKET_HOMES_EVACUATED);

                        Creature* citizen = ObjectAccessor::GetCreature(*go, _citizenGUID);
                        if (!citizen)
                            SF_LOG_ERROR("scripts", "go_merchant_square_door: citizen (guid %u) not found when spawning worgen.", GUID_LOPART(_citizenGUID));

                        if (Creature* worgen = player->SummonCreature(NPC_RAMPAGING_WORGEN, _worgenX, _worgenY, _worgenZ, _angle, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, EVENT_SUMMON_TTL))
                        {
                            if (citizen)
                            {
                                worgen->GetMotionMaster()->MoveChase(citizen);
                                _worgenGUID = worgen->GetGUID();
                                _chaseActive = true;
                                _chaseRefreshTimer = 2000;
                            }
                        }
                        else
                            SF_LOG_ERROR("scripts", "go_merchant_square_door: SummonCreature(%u) for Rampaging Worgen failed at (%f, %f, %f).", uint32(NPC_RAMPAGING_WORGEN), _worgenX, _worgenY, _worgenZ);
                    }
                    else
                        SF_LOG_ERROR("scripts", "go_merchant_square_door: player (guid %u) not found when worgen timer expired.", GUID_LOPART(_playerGUID));
                }
                else
                    _worgenTimer -= diff;
            }

            // ChaseMovementGenerator can settle once the worgen reaches contact
            // range and then never re-path as the citizen keeps wandering off -
            // keep re-issuing the chase for the life of the encounter.
            if (_chaseActive)
            {
                if (_chaseRefreshTimer <= diff)
                {
                    Creature* citizen = ObjectAccessor::GetCreature(*go, _citizenGUID);
                    Creature* worgen = ObjectAccessor::GetCreature(*go, _worgenGUID);
                    if (citizen && worgen && worgen->IsAlive())
                    {
                        worgen->GetMotionMaster()->MoveChase(citizen);
                        _chaseRefreshTimer = 2000;
                    }
                    else
                        _chaseActive = false;
                }
                else
                    _chaseRefreshTimer -= diff;
            }
        }

        bool GossipHello(Player* player) OVERRIDE
        {
            // One evacuation credit per player per door - otherwise a player
            // could just farm the same door 3 times instead of visiting 3.
            if (_usedByPlayers.count(player->GetGUID()))
            {
                SF_LOG_ERROR("scripts", "go_merchant_square_door: player %u already used this door (guid low %u), ignoring.", GUID_LOPART(player->GetGUID()), go->GetGUIDLow());
                return false;
            }

            if (_eventPending)
            {
                SF_LOG_ERROR("scripts", "go_merchant_square_door: player %u used door (guid low %u) while an event was already pending.", GUID_LOPART(player->GetGUID()), go->GetGUIDLow());
                return false;
            }

            if (go->GetGoState() != GOState::GO_STATE_READY)
            {
                SF_LOG_ERROR("scripts", "go_merchant_square_door: player %u used door (guid low %u) while not GO_STATE_READY (state=%u).", GUID_LOPART(player->GetGUID()), go->GetGUIDLow(), uint32(go->GetGoState()));
                return false;
            }

            _usedByPlayers.insert(player->GetGUID());

            go->SetGoState(GOState::GO_STATE_ACTIVE);
            _doorCloseTimer = EVENT_DOOR_TIMER;

            float angle = go->GetOrientation();
            float x = go->GetPositionX() - cos(angle) * 2.0f;
            float y = go->GetPositionY() - sin(angle) * 2.0f;
            float z = go->GetPositionZ();

            // Spawned via the player (not the door) so the summons inherit the
            // player's phase - a GameObject summoner always resolves to the
            // default phase, which left prior citizen/worgen spawns invisible.
            if (Creature* citizen = player->SummonCreature(NPC_FRIGHTENED_CITIZEN, x, y, z, angle, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, EVENT_CITIZEN_TTL))
            {
                // Passive and immune to the worgen chasing it - the chase is for
                // show, the citizen should never fight back or take damage.
                citizen->SetReactState(REACT_PASSIVE);
                citizen->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

                // MoveRandom moves in a straight line with no pathfinding (it
                // can clip through walls), so flee toward the open square
                // instead - MovePoint pathfinds around obstacles by default.
                citizen->GetMotionMaster()->MovePoint(0, MerchantSquareCenterX, MerchantSquareCenterY, MerchantSquareCenterZ);
                citizen->MonsterYell(FrightenedCitizenShouts[uint32(rand32()) % 5], Language::LANG_UNIVERSAL, NULL);

                _citizenGUID = citizen->GetGUID();
                _playerGUID = player->GetGUID();
                _worgenX = x - cos(angle) * 2.0f;
                _worgenY = y - sin(angle) * 2.0f;
                _worgenZ = z;
                _angle = angle;
                _worgenTimer = EVENT_WORGEN_DELAY;
                _eventPending = true;
            }
            else
                SF_LOG_ERROR("scripts", "go_merchant_square_door: SummonCreature(%u) for Frightened Citizen failed at (%f, %f, %f).", uint32(NPC_FRIGHTENED_CITIZEN), x, y, z);

            return false;
        }

    private:
        uint32 _doorCloseTimer;
        uint32 _worgenTimer;
        bool _eventPending;
        bool _chaseActive;
        uint32 _chaseRefreshTimer;
        uint64 _playerGUID;
        uint64 _citizenGUID;
        uint64 _worgenGUID;
        float _worgenX, _worgenY, _worgenZ, _angle;
        std::unordered_set<uint64> _usedByPlayers;
    };

    GameObjectAI* GetAI(GameObject* go) const OVERRIDE
    {
        return new go_merchant_square_doorAI(go);
    }
};

void AddSC_gilneas()
{
    new go_merchant_square_door();
}
