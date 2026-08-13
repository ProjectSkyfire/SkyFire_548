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

/*######
## Quest 14154 - By the Skin of His Teeth
######*/

enum BySkinOfHisTeeth
{
    NPC_LORD_DARIUS_CROWLEY_C1     = 35077,
    NPC_SEAN_DEMPSEY               = 35081,
    NPC_WORGEN_ALPHA_C1            = 35170,
    NPC_WORGEN_ALPHA_C2            = 35167,
    NPC_WORGEN_RUNT_C1             = 35188,
    NPC_WORGEN_RUNT_C2             = 35456,

    QUEST_BY_THE_SKIN_OF_HIS_TEETH = 14154,

    SPELL_BSOHT_BUFF               = 66914,
    SPELL_BSOHT_ENRAGE             = 8599,
    SPELL_BSOHT_LEFT_HOOK          = 67825,
    SPELL_BSOHT_DEMORALIZING_SHOUT = 61044,
    SPELL_BSOHT_SNAP_KICK          = 67827,

    EVENT_BSOHT_DURATION           = 118500,
    EVENT_BSOHT_SPAWN_TTL          = 20000,
    EVENT_BSOHT_ENRAGE_CD          = 30000,
};

struct BsohtPoint
{
    float x, y, z;
};

float const BsohtPlatformZ = 52.29f;

BsohtPoint const BsohtNWWaypointLoc1[2] =
{
    { -1630.62f, 1480.55f, 70.40f },
    { -1636.01f, 1475.81f, 64.51f }
};

BsohtPoint const BsohtNWWaypointLoc2[2] =
{
    { -1637.26f, 1488.86f, 69.95f },
    { -1642.45f, 1482.23f, 64.30f }
};

BsohtPoint const BsohtSWWaypointLoc1[2] =
{
    { -1718.31f, 1526.62f, 55.91f },
    { -1717.86f, 1490.77f, 56.61f }
};

BsohtPoint const BsohtSWWaypointLoc2[2] =
{
    { -1718.31f, 1526.62f, 55.91f },
    { -1717.86f, 1487.00f, 57.07f }
};

BsohtPoint const BsohtNWaypointLoc[1] =
{
    { -1593.38f, 1408.02f, 72.64f }
};

uint32 BsohtRand(uint32 min, uint32 max)
{
    return min + (uint32(rand32()) % (max - min + 1));
}

int32 BsohtIRand(int32 min, int32 max)
{
    return min + int32(uint32(rand32()) % uint32(max - min + 1));
}

/*######
## npc_sean_dempsey - controls the worgen wave-spawning for the duration of the event
######*/

class npc_sean_dempsey : public CreatureScript
{
public:
    npc_sean_dempsey() : CreatureScript("npc_sean_dempsey") { }

    struct npc_sean_dempseyAI : public ScriptedAI
    {
        npc_sean_dempseyAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 _spawnTimer;
        uint32 _eventTimer;
        uint32 _waveInterval;
        uint64 _playerGUID;
        bool _eventActive;
        bool _firstWave;

        void Reset() OVERRIDE
        {
            _eventActive = false;
            _firstWave = true;
            _spawnTimer = 0;
            _eventTimer = 0;
            _playerGUID = 0;
            _waveInterval = BsohtRand(9000, 15000);
        }

        void StartEvent(uint64 playerGUID)
        {
            _eventActive = true;
            _eventTimer = EVENT_BSOHT_DURATION;
            _playerGUID = playerGUID;
        }

        void SummonNextWave()
        {
            if (_firstWave)
            {
                me->SummonCreature(NPC_WORGEN_ALPHA_C2, -1732.81f, 1526.34f, 55.39f, 0.01f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                me->SummonCreature(NPC_WORGEN_ALPHA_C1, -1618.86f, 1505.68f, 70.24f, 3.91f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                _firstWave = false;
                return;
            }

            switch (BsohtRand(1, 5))
            {
                case 1: // Alphas on the NW and SW roofs
                    me->SummonCreature(NPC_WORGEN_ALPHA_C2, -1732.81f, 1526.34f, 55.39f, 0.01f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                    me->SummonCreature(NPC_WORGEN_ALPHA_C1, -1618.86f, 1505.68f, 70.24f, 3.91f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                    break;
                case 2: // 8 runts on the NW roof
                    for (uint8 i = 0; i < 4; ++i)
                    {
                        me->SummonCreature(NPC_WORGEN_RUNT_C1, -1611.40f, 1498.49f, 69.82f, 3.79f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                        me->SummonCreature(NPC_WORGEN_RUNT_C1, -1618.86f, 1505.68f, 70.24f, 3.91f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                    }
                    break;
                case 3: // 8 runts on the SW roof
                    for (uint8 i = 0; i < 4; ++i)
                    {
                        me->SummonCreature(NPC_WORGEN_RUNT_C2, -1732.81f, 1526.34f, 55.39f, 0.01f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                        me->SummonCreature(NPC_WORGEN_RUNT_C2, -1737.49f, 1526.11f, 55.51f, 0.01f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                    }
                    break;
                case 4: // Alphas on the N and SW roofs
                    me->SummonCreature(NPC_WORGEN_ALPHA_C2, -1732.81f, 1526.34f, 55.39f, 0.01f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                    me->SummonCreature(NPC_WORGEN_ALPHA_C1, -1562.59f, 1409.35f, 71.66f, 3.16f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                    break;
                case 5: // 8 runts split across the NW and SW roofs
                    for (uint8 i = 0; i < 4; ++i)
                    {
                        me->SummonCreature(NPC_WORGEN_RUNT_C2, -1732.81f, 1526.34f, 55.39f, 0.01f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                        me->SummonCreature(NPC_WORGEN_RUNT_C1, -1618.86f, 1505.68f, 70.24f, 3.91f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, EVENT_BSOHT_SPAWN_TTL);
                    }
                    break;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!_eventActive)
                return;

            if (_eventTimer <= diff)
            {
                _eventActive = false;
                if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                    player->AreaExploredOrEventHappens(QUEST_BY_THE_SKIN_OF_HIS_TEETH);
                return;
            }
            _eventTimer -= diff;

            if (_spawnTimer <= diff)
            {
                SummonNextWave();
                _spawnTimer = _waveInterval;
            }
            else
                _spawnTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_sean_dempseyAI(creature);
    }
};

/*######
## npc_lord_darius_crowley_c1 - starts the event on quest accept, fights the worgen
######*/

class npc_lord_darius_crowley_c1 : public CreatureScript
{
public:
    npc_lord_darius_crowley_c1() : CreatureScript("npc_lord_darius_crowley_c1") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() != QUEST_BY_THE_SKIN_OF_HIS_TEETH)
            return true;

        creature->CastSpell(player, SPELL_BSOHT_BUFF, true);

        if (Creature* dempsey = creature->FindNearestCreature(NPC_SEAN_DEMPSEY, 100.0f))
            CAST_AI(npc_sean_dempsey::npc_sean_dempseyAI, dempsey->AI())->StartEvent(player->GetGUID());

        return true;
    }

    struct npc_lord_darius_crowley_c1AI : public ScriptedAI
    {
        npc_lord_darius_crowley_c1AI(Creature* creature) : ScriptedAI(creature) { }

        uint32 _attackTimer;

        void Reset() OVERRIDE
        {
            _attackTimer = BsohtRand(1700, 2400);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (_attackTimer <= diff)
            {
                if (me->IsWithinMeleeRange(me->GetVictim()))
                {
                    switch (BsohtRand(0, 2))
                    {
                        case 0:
                            DoCast(me->GetVictim(), SPELL_BSOHT_LEFT_HOOK, true);
                            break;
                        case 1:
                            DoCast(me->GetVictim(), SPELL_BSOHT_DEMORALIZING_SHOUT, true);
                            break;
                        case 2:
                            DoCast(me->GetVictim(), SPELL_BSOHT_SNAP_KICK, true);
                            break;
                    }
                    _attackTimer = BsohtRand(1700, 2400);
                }
                else
                    me->GetMotionMaster()->MoveChase(me->GetVictim());
            }
            else
            {
                _attackTimer -= diff;
                DoMeleeAttackIfReady();
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lord_darius_crowley_c1AI(creature);
    }
};

/*######
## npc_worgen_runt_c1/c2, npc_worgen_alpha_c1/c2 - run rooftop waypoints to a jump-off
## point, land on the platform below, then fight whoever is closest
######*/

struct BsohtRoofFighterAI : public ScriptedAI
{
    BsohtRoofFighterAI(Creature* creature,
        float spawnX1, float spawnY1, BsohtPoint const* waypoints1, uint8 waypointCount1, float jumpX1, float jumpY1,
        float spawnX2, float spawnY2, BsohtPoint const* waypoints2, uint8 waypointCount2, float jumpX2, float jumpY2,
        float jumpSpeedXY, float jumpSpeedZ, float chaseRange)
        : ScriptedAI(creature),
          _spawnX1(spawnX1), _spawnY1(spawnY1), _waypoints1(waypoints1), _waypointCount1(waypointCount1), _jumpX1(jumpX1), _jumpY1(jumpY1),
          _spawnX2(spawnX2), _spawnY2(spawnY2), _waypoints2(waypoints2), _waypointCount2(waypointCount2), _jumpX2(jumpX2), _jumpY2(jumpY2),
          _jumpSpeedXY(jumpSpeedXY), _jumpSpeedZ(jumpSpeedZ), _chaseRange(chaseRange) { }

    void Reset() OVERRIDE
    {
        _running = _jumping = _fighting = false;
        _atLoc1 = _atLoc2 = false;
        _waypointId = 0;
        _issuedWaypointId = uint8(-1);
        _enrageTimer = 0;
        _willEnrage = BsohtRand(0, 1) != 0;
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        if (_fighting)
            return;

        me->GetMotionMaster()->Clear();
        me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
        _running = false;
        _jumping = false;
        _fighting = true;
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!_running && !_jumping && !_fighting)
        {
            if (me->GetPositionX() == _spawnX1 && me->GetPositionY() == _spawnY1)
            {
                _atLoc1 = true;
                _running = true;
            }
            else if (_waypoints2 && me->GetPositionX() == _spawnX2 && me->GetPositionY() == _spawnY2)
            {
                _atLoc2 = true;
                _running = true;
            }
        }

        if (_running)
        {
            if (_waypointId == _issuedWaypointId)
                return;

            if (_atLoc1 && _waypointId < _waypointCount1)
                me->GetMotionMaster()->MovePoint(_waypointId, _waypoints1[_waypointId].x, _waypoints1[_waypointId].y, _waypoints1[_waypointId].z);
            else if (_atLoc2 && _waypointId < _waypointCount2)
                me->GetMotionMaster()->MovePoint(_waypointId, _waypoints2[_waypointId].x, _waypoints2[_waypointId].y, _waypoints2[_waypointId].z);
            _issuedWaypointId = _waypointId;
            return;
        }

        if (_jumping)
            return;

        if (_fighting)
        {
            if (!me->GetVictim())
            {
                if (Player* player = me->SelectNearestPlayer(_chaseRange))
                    AttackStart(player);
                else if (Creature* crowley = me->FindNearestCreature(NPC_LORD_DARIUS_CROWLEY_C1, _chaseRange))
                    AttackStart(crowley);
            }

            if (!UpdateVictim())
                return;

            if (_enrageTimer <= diff)
            {
                if (_willEnrage && me->GetHealthPct() <= 30.0f)
                {
                    DoCast(me, SPELL_BSOHT_ENRAGE);
                    _enrageTimer = EVENT_BSOHT_ENRAGE_CD;
                }
            }
            else
                _enrageTimer -= diff;

            DoMeleeAttackIfReady();
        }
    }

    void MovementInform(uint32 type, uint32 id) OVERRIDE
    {
        if (type == EFFECT_MOTION_TYPE && id == EVENT_JUMP)
        {
            me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
            _jumping = false;
            _fighting = true;
            return;
        }

        if (type != POINT_MOTION_TYPE)
            return;

        _waypointId = id + 1;

        uint8 count = _atLoc1 ? _waypointCount1 : _waypointCount2;
        if (_waypointId >= count)
        {
            float jumpX = _atLoc1 ? _jumpX1 : _jumpX2;
            float jumpY = _atLoc1 ? _jumpY1 : _jumpY2;
            me->GetMotionMaster()->MoveJump(jumpX + float(BsohtIRand(-3, 3)), jumpY + float(BsohtIRand(-3, 3)), BsohtPlatformZ, _jumpSpeedXY, _jumpSpeedZ);
            _running = false;
            _jumping = true;
        }
    }

private:
    float _spawnX1, _spawnY1;
    BsohtPoint const* _waypoints1;
    uint8 _waypointCount1;
    float _jumpX1, _jumpY1;
    float _spawnX2, _spawnY2;
    BsohtPoint const* _waypoints2;
    uint8 _waypointCount2;
    float _jumpX2, _jumpY2;
    float _jumpSpeedXY, _jumpSpeedZ, _chaseRange;

    bool _running, _jumping, _fighting;
    bool _atLoc1, _atLoc2;
    uint8 _waypointId;
    uint8 _issuedWaypointId;
    uint32 _enrageTimer;
    bool _willEnrage;
};

class npc_worgen_runt_c1 : public CreatureScript
{
public:
    npc_worgen_runt_c1() : CreatureScript("npc_worgen_runt_c1") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new BsohtRoofFighterAI(creature,
            -1611.40f, 1498.49f, BsohtNWWaypointLoc1, 2, -1668.52f, 1439.69f,
            -1618.86f, 1505.68f, BsohtNWWaypointLoc2, 2, -1678.04f, 1450.88f,
            20.0f, 22.0f, 40.0f);
    }
};

class npc_worgen_runt_c2 : public CreatureScript
{
public:
    npc_worgen_runt_c2() : CreatureScript("npc_worgen_runt_c2") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new BsohtRoofFighterAI(creature,
            -1732.81f, 1526.34f, BsohtSWWaypointLoc1, 2, -1685.521f, 1458.48f,
            -1737.49f, 1526.11f, BsohtSWWaypointLoc2, 2, -1681.81f, 1445.54f,
            20.0f, 22.0f, 50.0f);
    }
};

class npc_worgen_alpha_c1 : public CreatureScript
{
public:
    npc_worgen_alpha_c1() : CreatureScript("npc_worgen_alpha_c1") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new BsohtRoofFighterAI(creature,
            -1618.86f, 1505.68f, BsohtNWWaypointLoc1, 2, -1668.52f, 1439.69f,
            -1562.59f, 1409.35f, BsohtNWaypointLoc, 1, -1660.17f, 1429.55f,
            22.0f, 20.0f, 40.0f);
    }
};

class npc_worgen_alpha_c2 : public CreatureScript
{
public:
    npc_worgen_alpha_c2() : CreatureScript("npc_worgen_alpha_c2") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new BsohtRoofFighterAI(creature,
            -1732.81f, 1526.34f, BsohtSWWaypointLoc1, 2, -1685.52f, 1458.48f,
            0.0f, 0.0f, nullptr, 0, 0.0f, 0.0f,
            20.0f, 22.0f, 40.0f);
    }
};

/*######
## Quest 14159 - The Rebel Lord's Arsenal
######*/

enum RebelLordsArsenal
{
    NPC_JOSIAH_AVERY_P2                = 35369,
    NPC_JOSIAH_AVERY_P4                = 35370,
    NPC_JOSIAH_AVERY_TRIGGER           = 50415,
    NPC_LORNA_CROWLEY_P4                = 35378,

    QUEST_REBEL_LORDS_ARSENAL          = 14159,

    SPELL_ARSENAL_COSMETIC_ATTACK      = 69873,
    SPELL_ARSENAL_SHOOT                = 6660,
    SPELL_WORGEN_BITE                  = 72870,
    SPELL_SUMMON_JOSIAH_AVERY          = 67350,
    SPELL_GET_SHOT                     = 67349,
    SPELL_PHASE_QUEST_2                = 59073,

    PHASE_JOSIAH_AVERY_REVEAL          = 171,

    SAY_JOSIAH_AVERY_TRIGGER_BITTEN    = 0,
};

/*######
## npc_josiah_avery - quest 14159 turn-in; marks the player as bitten and
## summons his phase-4 double for the worgen-reveal cutscene
######*/

class npc_josiah_avery : public CreatureScript
{
public:
    npc_josiah_avery() : CreatureScript("npc_josiah_avery") { }

    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) OVERRIDE
    {
        if (quest->GetQuestId() != QUEST_REBEL_LORDS_ARSENAL)
            return true;

        creature->AddAura(SPELL_WORGEN_BITE, player);
        player->RemoveAura(SPELL_PHASE_QUEST_2);
        player->SetPhased(PHASE_JOSIAH_AVERY_REVEAL, true, true);

        creature->SetPhased(PHASE_JOSIAH_AVERY_REVEAL, true, true);
        creature->CastSpell(creature, SPELL_SUMMON_JOSIAH_AVERY, true);
        creature->SetPhased(PHASE_JOSIAH_AVERY_REVEAL, true, false);

        return true;
    }
};

/*######
## npc_josiah_avery_trigger - waits for a bitten player to approach both Lorna
## Crowley and the phase-4 Josiah Avery double, then runs the reveal: Avery
## "attacks" the player, gets knocked back, and is shot down by Lorna.
######*/

class npc_josiah_avery_trigger : public CreatureScript
{
public:
    npc_josiah_avery_trigger() : CreatureScript("npc_josiah_avery_trigger") { }

    struct npc_josiah_avery_triggerAI : public ScriptedAI
    {
        npc_josiah_avery_triggerAI(Creature* creature) : ScriptedAI(creature) { }

        uint8 _step;
        uint32 _stepTimer;
        uint64 _playerGUID;

        void Reset() OVERRIDE
        {
            _step = 0;
            _stepTimer = 0;
            _playerGUID = 0;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (_step == 0)
            {
                Creature* lorna = me->FindNearestCreature(NPC_LORNA_CROWLEY_P4, 60.0f);
                Creature* avery = me->FindNearestCreature(NPC_JOSIAH_AVERY_P4, 80.0f);
                if (!lorna || !avery)
                    return;

                Player* player = me->SelectNearestPlayer(50.0f);
                if (!player || !player->HasAura(SPELL_WORGEN_BITE))
                    return;

                _playerGUID = player->GetGUID();
                _stepTimer = 200;
                _step = 1;
                return;
            }

            if (_stepTimer > diff)
            {
                _stepTimer -= diff;
                return;
            }

            Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID);
            Creature* lorna = me->FindNearestCreature(NPC_LORNA_CROWLEY_P4, 60.0f);
            Creature* avery = me->FindNearestCreature(NPC_JOSIAH_AVERY_P4, 80.0f);
            if (!player || !lorna || !avery)
            {
                Reset();
                return;
            }

            switch (_step)
            {
                case 1:
                    Talk(SAY_JOSIAH_AVERY_TRIGGER_BITTEN, player);
                    _stepTimer = 1200;
                    break;
                case 2:
                    avery->SetOrientation(avery->GetAngle(player));
                    avery->CastSpell(player, SPELL_ARSENAL_COSMETIC_ATTACK, true);
                    player->GetMotionMaster()->MoveKnockbackFrom(avery->GetPositionX(), avery->GetPositionY(), 22.0f, 8.0f);
                    avery->getThreatManager().resetAllAggro();
                    _stepTimer = 600;
                    break;
                case 3:
                    avery->GetMotionMaster()->MoveJump(-1791.94f, 1427.29f, 12.4584f, 18.0f, 7.0f);
                    _stepTimer = 200;
                    break;
                case 4:
                    lorna->CastSpell(avery, SPELL_ARSENAL_SHOOT, true);
                    _stepTimer = 200;
                    break;
                case 5:
                    avery->CastSpell(avery, SPELL_GET_SHOT, true);
                    avery->setDeathState(DeathState::JUST_DIED);
                    player->SaveToDB();
                    avery->DespawnOrUnsummon(1000);
                    me->DespawnOrUnsummon(1000);
                    Reset();
                    return;
            }

            ++_step;
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_josiah_avery_triggerAI(creature);
    }
};

void AddSC_gilneas()
{
    new go_merchant_square_door();
    new npc_sean_dempsey();
    new npc_lord_darius_crowley_c1();
    new npc_worgen_runt_c1();
    new npc_worgen_runt_c2();
    new npc_worgen_alpha_c1();
    new npc_worgen_alpha_c2();
    new npc_josiah_avery();
    new npc_josiah_avery_trigger();
}
