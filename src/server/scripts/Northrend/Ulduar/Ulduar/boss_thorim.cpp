/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "InstanceScript.h"
#include "Map.h"
#include "MotionMaster.h"
#include "MoveSplineInit.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "ulduar.h"

enum Spells
{
    // Thorim
    SPELL_SHEATH_OF_LIGHTNING                   = 62276,
    SPELL_STORMHAMMER                           = 62042,
    SPELL_STORMHAMMER_SIF                       = 64767,
    SPELL_STORMHAMMER_BOOMERANG                 = 64909,
    SPELL_DEAFENING_THUNDER                     = 62470,
    SPELL_CHARGE_ORB                            = 62016,
    SPELL_SUMMON_LIGHTNING_ORB                  = 62391,
    SPELL_LIGHTNING_DESTRUCTION                 = 62393,
    SPELL_TOUCH_OF_DOMINION                     = 62507,
    SPELL_TOUCH_OF_DOMINION_TRIGGERED           = 62565,
    SPELL_CHAIN_LIGHTNING                       = 62131,
    SPELL_LIGHTNING_ORB_CHARGED                 = 62186, // wrong duration, triggered spell should handle lightning release
    SPELL_LIGHTNING_CHARGE                      = 62279,
    SPELL_LIGHTNING_RELEASE                     = 62466,
    SPELL_LIGHTNING_PILLAR_2                    = 62976, // caster high position, target low position
    SPELL_LIGHTNING_PILLAR_1                    = 63238, // caster high position, target low position
    SPELL_UNBALANCING_STRIKE                    = 62130,
    SPELL_BERSERK_PHASE_1                       = 62560,
    SPELL_BERSERK_PHASE_2                       = 62555,
    SPELL_ACTIVATE_LIGHTNING_ORB_PERIODIC       = 62184,
    SPELL_TELEPORT_KEEPER_VISUAL                = 62940,

    // Lightning Field
    SPELL_LIGHTNING_FIELD                       = 64972,
    SPELL_LIGHTNING_BEAM_CHANNEL                = 45537,

    // Sif
    SPELL_BLIZZARD                              = 62577,
    SPELL_BLINK                                 = 62578,
    SPELL_FROSTBOLT_VOLLEY                      = 62580,
    SPELL_FROSTBOLT                             = 62583,
    SPELL_FROSTNOVA                             = 62597,
    SPELL_SIF_TRANSFORM                         = 64778,

    // Runic Colossus
    SPELL_SMASH                                 = 62339,
    SPELL_RUNIC_BARRIER                         = 62338,
    SPELL_RUNIC_CHARGE                          = 62613,
    SPELL_RUNIC_SMASH                           = 62465,
    SPELL_RUNIC_SMASH_RIGHT                     = 62057,
    SPELL_RUNIC_SMASH_LEFT                      = 62058,

    // Ancient Rune Giant
    SPELL_RUNIC_FORTIFICATION                   = 62942,
    SPELL_RUNE_DETONATION                       = 62526,
    SPELL_STOMP                                 = 62411
};

// The achievements of this encounter are driven by "be the target of spell" criteria. These two spells
// (Credit: Siffed and Credit: Thorim) do not exist in 5.4.8, so the script updates the criteria directly.
enum CriteriaSpells
{
    CRITERIA_SPELL_SIFFED                       = 64980,
    CRITERIA_SPELL_KILL                         = 64985
};

enum Phases
{
    PHASE_NULL,
    PHASE_1,
    PHASE_2
};

enum Events
{
    // Thorim
    EVENT_SAY_AGGRO_2 = 1,
    EVENT_SAY_SIF_START,
    EVENT_START_SIF_CHANNEL,
    EVENT_STORMHAMMER,
    EVENT_CHARGE_ORB,
    EVENT_SUMMON_ADDS,
    EVENT_BERSERK,
    EVENT_JUMPDOWN,
    EVENT_UNBALANCING_STRIKE,
    EVENT_CHAIN_LIGHTNING,
    EVENT_START_PERIODIC_CHARGE,
    EVENT_LIGHTNING_CHARGE,
    EVENT_ACTIVATE_LIGHTNING_FIELD,
    EVENT_OUTRO_1,
    EVENT_OUTRO_2,
    EVENT_OUTRO_3,
    EVENT_OUTRO_TELEPORT,

    // Runic Colossus
    EVENT_RUNIC_BARRIER,
    EVENT_SMASH,
    EVENT_RUNIC_CHARGE,
    EVENT_RUNIC_SMASH,

    // Ancient Rune Giant
    EVENT_RUNIC_FORTIFICATION,
    EVENT_STOMP,
    EVENT_RUNE_DETONATION,

    // Arena NPC
    EVENT_PRIMARY_ABILITY,
    EVENT_SECONDARY_ABILITY,
    EVENT_THIRD_ABILITY,
    EVENT_ABILITY_CHARGE,

    // Sif
    EVENT_BLINK,
    EVENT_FROST_NOVA,
    EVENT_FROSTBOLT,
    EVENT_FROSTBOLT_VOLLEY,
    EVENT_BLIZZARD
};

enum Yells
{
    // Thorim
    SAY_AGGRO_1                                 = 0,
    SAY_AGGRO_2                                 = 1,
    SAY_SPECIAL                                 = 2,
    SAY_JUMPDOWN                                = 3,
    SAY_SLAY                                    = 4,
    SAY_BERSERK                                 = 5,
    SAY_WIPE                                    = 6,
    SAY_DEATH                                   = 7,
    SAY_END_NORMAL_1                            = 8,
    SAY_END_NORMAL_2                            = 9,
    SAY_END_NORMAL_3                            = 10,
    SAY_END_HARD_1                              = 11,
    SAY_END_HARD_2                              = 12,
    SAY_END_HARD_3                              = 13,

    // Runic Colossus
    EMOTE_RUNIC_BARRIER                         = 0,

    // Ancient Rune Giant
    EMOTE_RUNIC_MIGHT                           = 0,

    // Sif
    SAY_SIF_START                               = 0,
    SAY_SIF_DESPAWN                             = 1,
    SAY_SIF_EVENT                               = 2
};

enum PreAddSpells
{
    SPELL_ACID_BREATH               = 62315,
    SPELL_SWEEP                     = 62316,

    SPELL_DEVASTATE                 = 62317,
    SPELL_HEROIC_SWIPE              = 62444,
    SPELL_SUNDER_ARMOR              = 57807,

    SPELL_BARBED_SHOT               = 62318,
    SPELL_SHOOT                     = 16496,

    SPELL_RENEW                     = 62333,
    SPELL_GREATER_HEAL              = 62334, /// 61965
    SPELL_CIRCLE_OF_HEALING         = 61964,

    SPELL_HOLY_SMITE                = 62335,

    SPELL_LEAP                      = 61934,

    SPELL_CHARGE                    = 32323,
    SPELL_MORTAL_STRIKE             = 35054,
    SPELL_WHIRLWIND                 = 33500,

    SPELL_LOW_BLOW                  = 62326,
    SPELL_PUMMEL                    = 38313,

    SPELL_RUNIC_LIGHTNING           = 62327,
    SPELL_RUNIC_MENDING             = 62328,
    SPELL_RUNIC_SHIELD              = 62321,

    SPELL_RUNIC_STRIKE              = 62322,
    SPELL_AURA_OF_CELERITY          = 62320,
    SPELL_AURA_OF_CELERITY_VISUAL   = 62398,

    SPELL_IMPALE                    = 62331,
    SPELL_WHIRLING_TRIP             = 64151,

    SPELL_CLEAVE                    = 42724,
    SPELL_HAMSTRING                 = 48639,
    SPELL_SHIELD_SMASH              = 62332,
};

enum TrashTypes
{
    // Pre Phase Trash
    BEHEMOTH,
    MERCENARY_CAPTAIN,
    MERCENARY_SOLDIER,

    // Arena Phase Trash
    DARK_RUNE_CHAMPION,
    DARK_RUNE_WARBRINGER,
    DARK_RUNE_COMMONER,
    DARK_RUNE_EVOKER,

    // Hall Way Trash
    IRON_RING_GUARD,
    IRON_HONOR_GUARD,

    // Shared
    DARK_RUNE_ACOLYTE
};

struct ThorimTrashInfo
{
    uint32 Type;
    uint32 Entry;
    uint32 PrimaryAbility;
    uint32 SecondaryAbility;
    uint32 ThirdAbility;
};

uint8 const ThorimTrashCount = 13;
ThorimTrashInfo const StaticThorimTrashInfo[ThorimTrashCount] =
{
    // Pre Phase
    { BEHEMOTH,             NPC_JORMUNGAR_BEHEMOTH,    SPELL_ACID_BREATH,     SPELL_SWEEP,        0                       },
    { MERCENARY_CAPTAIN,    NPC_MERCENARY_CAPTAIN_A,   SPELL_DEVASTATE,       SPELL_HEROIC_SWIPE, SPELL_SUNDER_ARMOR      },
    { MERCENARY_SOLDIER,    NPC_MERCENARY_SOLDIER_A,   SPELL_BARBED_SHOT,     SPELL_SHOOT,        0                       },
    { DARK_RUNE_ACOLYTE,    NPC_DARK_RUNE_ACOLYTE_PRE, SPELL_RENEW,           SPELL_GREATER_HEAL, SPELL_CIRCLE_OF_HEALING },
    { MERCENARY_CAPTAIN,    NPC_MERCENARY_CAPTAIN_H,   SPELL_DEVASTATE,       SPELL_HEROIC_SWIPE, SPELL_SUNDER_ARMOR      },
    { MERCENARY_SOLDIER,    NPC_MERCENARY_SOLDIER_H,   SPELL_BARBED_SHOT,     SPELL_SHOOT,        0                       },

    // Arena Phase
    { DARK_RUNE_CHAMPION,   NPC_DARK_RUNE_CHAMPION,    SPELL_MORTAL_STRIKE,   SPELL_WHIRLWIND,    0                       },
    { DARK_RUNE_WARBRINGER, NPC_DARK_RUNE_WARBRINGER,  SPELL_RUNIC_STRIKE,    0,                  0                       },
    { DARK_RUNE_EVOKER,     NPC_DARK_RUNE_EVOKER,      SPELL_RUNIC_LIGHTNING, SPELL_RUNIC_SHIELD, SPELL_RUNIC_MENDING     },
    { DARK_RUNE_COMMONER,   NPC_DARK_RUNE_COMMONER,    SPELL_LOW_BLOW,        SPELL_PUMMEL,       0                       },

    // Hall Way
    { IRON_RING_GUARD,      NPC_IRON_RING_GUARD,       SPELL_WHIRLING_TRIP,   SPELL_IMPALE,       0                       },
    { IRON_HONOR_GUARD,     NPC_IRON_HONOR_GUARD,      SPELL_CLEAVE,          SPELL_SHIELD_SMASH, 0                       },
    { DARK_RUNE_ACOLYTE,    NPC_DARK_RUNE_ACOLYTE,     SPELL_RENEW,           SPELL_GREATER_HEAL, 0                       }
};

enum Actions
{
    ACTION_INCREASE_PREADDS_COUNT,
    ACTION_ACTIVATE_RUNIC_SMASH,
    ACTION_ACTIVATE_ADDS,
    ACTION_PILLAR_CHARGED,
    ACTION_START_HARD_MODE,
    ACTION_BERSERK,
    ACTION_ENABLE_BOUNDARY
};

struct SummonLocation
{
    Position pos;
    uint32 entry;
};

SummonLocation const PreAddLocations[] =
{
    { { 2149.68f, -263.477f, 419.679f, 3.120f }, NPC_JORMUNGAR_BEHEMOTH    },
    { { 2131.31f, -271.640f, 419.840f, 2.188f }, NPC_MERCENARY_CAPTAIN_A   },
    { { 2127.24f, -259.182f, 419.974f, 5.917f }, NPC_MERCENARY_SOLDIER_A   },
    { { 2123.32f, -254.770f, 419.840f, 6.170f }, NPC_MERCENARY_SOLDIER_A   },
    { { 2120.10f, -258.990f, 419.840f, 6.250f }, NPC_MERCENARY_SOLDIER_A   },
    { { 2129.09f, -277.142f, 419.756f, 1.222f }, NPC_DARK_RUNE_ACOLYTE_PRE }
};

SummonLocation const ColossusAddLocations[] =
{
    { { 2218.38f, -297.50f, 412.18f, 1.030f }, NPC_IRON_RING_GUARD   },
    { { 2235.07f, -297.98f, 412.18f, 1.613f }, NPC_IRON_RING_GUARD   },
    { { 2235.26f, -338.34f, 412.18f, 1.589f }, NPC_IRON_RING_GUARD   },
    { { 2217.69f, -337.39f, 412.18f, 1.241f }, NPC_IRON_RING_GUARD   },
    { { 2227.58f, -308.30f, 412.18f, 1.591f }, NPC_DARK_RUNE_ACOLYTE },
    { { 2227.47f, -345.37f, 412.18f, 1.566f }, NPC_DARK_RUNE_ACOLYTE }
};

SummonLocation const GiantAddLocations[] =
{
    { { 2198.05f, -428.77f, 419.95f, 6.056f }, NPC_IRON_HONOR_GUARD  },
    { { 2220.31f, -436.22f, 412.26f, 1.064f }, NPC_IRON_HONOR_GUARD  },
    { { 2158.88f, -441.73f, 438.25f, 0.127f }, NPC_IRON_HONOR_GUARD  },
    { { 2198.29f, -436.92f, 419.95f, 0.261f }, NPC_DARK_RUNE_ACOLYTE },
    { { 2230.93f, -434.27f, 412.26f, 1.931f }, NPC_DARK_RUNE_ACOLYTE }
};

Position const SifSpawnPosition = { 2148.301f, -297.8453f, 438.3308f, 2.687807f };

enum Data
{
    ACHIEVEMENT_DONT_STAND_IN_THE_LIGHTNING = 29712972,
    ACHIEVEMENT_SIFFED                      = 29772978,
    ACHIEVEMENT_LOSE_YOUR_ILLUSION          = 31763183,
    DATA_CHARGED_PILLAR                     = 1
};

enum Misc
{
    // Item entry of the mace Thorim wields while he holds the Stormhammer
    THORIM_WEAPON_ITEM_ID                   = 45900,

    // The event bunny the Leap spell must never pick (static spawn of the 10 player arena)
    LEAP_EXCLUDED_BUNNY_GUID                = 136515,

    PRE_PHASE_ADD_COUNT                     = 6
};

Position const LightningOrbPath[] =
{
    { 2134.889893f, -298.632996f, 438.247467f },
    { 2134.570068f, -440.317993f, 438.247467f },
    { 2167.820312f, -440.330261f, 438.247589f },
    { 2213.394287f, -433.318298f, 412.665863f },
    { 2227.766113f, -433.275818f, 412.177032f },
    { 2227.551270f, -263.081085f, 412.176880f },
    { 2202.208008f, -262.939270f, 412.168976f },
    { 2182.310059f, -263.233093f, 414.739410f }
};

// used for trash jump calculation
Position const ArenaCenter = { 2134.77f, -262.307f };

// used for lightning field calculation
Position const LightningFieldCenter = { 2135.178f, -321.122f };

float const ArenaFloorRadius = 45.4f;
float const BalconyRadius = 32.0f;

// Everything above this height is on Thorim's balcony
float const BalconyZ = 428.0f;

// The arena is the floor circle without the balcony circle on top of it.
bool IsInsideArena(Position const* pos)
{
    return ArenaCenter.GetExactDist2dSq(pos) <= ArenaFloorRadius * ArenaFloorRadius
        && LightningFieldCenter.GetExactDist2dSq(pos) > BalconyRadius * BalconyRadius;
}

// SkyFire has no per creature boundary, so each AI keeps its own copy of what TrinityCore's SetBoundary does.
struct ArenaBoundary
{
    ArenaBoundary() : Active(false), Negate(false) { }

    void Set(bool active, bool negate = false)
    {
        Active = active;
        Negate = negate;
    }

    bool IsInBoundary(Position const* pos) const
    {
        return !Active || (IsInsideArena(pos) != Negate);
    }

    bool Active;
    bool Negate;
};

class HeightPositionCheck
{
    public:
        HeightPositionCheck(bool ret) : _ret(ret) { }

        bool operator()(Position const* pos) const
        {
            return (pos->GetPositionZ() > BalconyZ) == _ret;
        }

    private:
        bool _ret;
};

struct OutOfArenaCheck
{
    bool operator()(Position const* who) const
    {
        return !IsInsideArena(who);
    }
};

// A player (not a pet) within the given range of the referer, used for charges
class PlayerInRangePredicate
{
    public:
        PlayerInRangePredicate(Unit const* referer, float minRange, float maxRange) : _referer(referer), _minRange(minRange), _maxRange(maxRange) { }

        bool operator()(Unit* unit) const
        {
            return unit->GetTypeId() == TypeID::TYPEID_PLAYER && unit->IsInRange(_referer, _minRange, _maxRange);
        }

    private:
        Unit const* _referer;
        float _minRange;
        float _maxRange;
};

void SetImmuneToPC(Creature* creature, bool apply)
{
    if (apply)
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    else
        creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
}

Creature* GetThorimCreature(InstanceScript* instance, uint32 data)
{
    return instance->instance->GetCreature(instance->GetData64(data));
}

GameObject* GetThorimObject(InstanceScript* instance, uint32 data)
{
    return instance->instance->GetGameObject(instance->GetData64(data));
}

class RunicSmashExplosionEvent : public BasicEvent
{
    public:
        RunicSmashExplosionEvent(Creature* owner) : _owner(owner) { }

        bool Execute(uint64 /*eventTime*/, uint32 /*updateTime*/) OVERRIDE
        {
            _owner->CastSpell((Unit*)NULL, SPELL_RUNIC_SMASH, false);
            return true;
        }

    private:
        Creature* _owner;
};

class TrashJumpEvent : public BasicEvent
{
    public:
        TrashJumpEvent(Creature* owner) : _owner(owner), _stage(0) { }

        bool Execute(uint64 eventTime, uint32 /*updateTime*/) OVERRIDE
        {
            switch (_stage)
            {
                case 0:
                    _owner->CastSpell((Unit*)NULL, SPELL_LEAP, false);
                    ++_stage;
                    _owner->m_Events.AddEvent(this, eventTime + 2000);
                    return false;
                case 1:
                    _owner->SetReactState(REACT_AGGRESSIVE);
                    _owner->AI()->DoZoneInCombat(_owner);
                    _owner->AI()->DoAction(ACTION_ENABLE_BOUNDARY);
                    return true;
                default:
                    break;
            }

            return true;
        }

    private:
        Creature* _owner;
        uint8 _stage;
};

class LightningFieldEvent : public BasicEvent
{
    public:
        LightningFieldEvent(Creature* owner) : _owner(owner) { }

        bool Execute(uint64 eventTime, uint32 /*updateTime*/) OVERRIDE
        {
            if (InstanceScript* instance = _owner->GetInstanceScript())
            {
                if (instance->GetBossState(BOSS_THORIM) == IN_PROGRESS)
                {
                    _owner->CastSpell((Unit*)NULL, SPELL_LIGHTNING_FIELD, false);
                    _owner->m_Events.AddEvent(this, eventTime + 1000);
                    return false;
                }
            }

            _owner->InterruptNonMeleeSpells(false);
            _owner->AI()->EnterEvadeMode();
            return true;
        }

    private:
        Creature* _owner;
};

class boss_thorim : public CreatureScript
{
    public:
        boss_thorim() : CreatureScript("boss_thorim") { }

        struct boss_thorimAI : public BossAI
        {
            boss_thorimAI(Creature* creature) : BossAI(creature, BOSS_THORIM)
            {
                _encounterFinished = false;
                _activePillarGUID = 0;
                Initialize();
            }

            void Initialize()
            {
                _killedCount = 0;
                _waveType = 0;
                _hardMode = true;
                _orbSummoned = false;
                _dontStandInTheLightning = true;
            }

            void Reset() OVERRIDE
            {
                if (_encounterFinished)
                    return;

                _boundary.Set(false);
                _Reset();
                Initialize();

                me->SetReactState(REACT_PASSIVE);
                me->SetDisableGravity(true);
                me->SetControlled(true, UNIT_STATE_ROOT);
                SetImmuneToPC(me, true);

                events.SetPhase(PHASE_NULL);

                // Respawn Mini Bosses
                for (uint8 i = DATA_RUNIC_COLOSSUS; i <= DATA_RUNE_GIANT; ++i)
                    if (Creature* miniBoss = GetThorimCreature(instance, i))
                        miniBoss->Respawn(true);

                // Spawn Pre Phase Adds
                for (SummonLocation const& s : PreAddLocations)
                    me->SummonCreature(s.entry, s.pos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);

                if (GameObject* lever = GetThorimObject(instance, DATA_THORIM_LEVER))
                    lever->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);

                // Remove trigger auras
                if (Creature* pillar = Unit::GetCreature(*me, _activePillarGUID))
                    pillar->RemoveAllAuras();

                if (Creature* controller = GetThorimCreature(instance, DATA_THORIM_CONTROLLER))
                    controller->RemoveAllAuras();

                _activePillarGUID = 0;
            }

            void EnterEvadeMode() OVERRIDE
            {
                summons.DespawnAll();
                instance->SetBossState(BOSS_THORIM, FAIL);
                _DespawnAtEvade();
            }

            void SetGUID(uint64 guid, int32 id) OVERRIDE
            {
                if (id == DATA_CHARGED_PILLAR)
                {
                    _activePillarGUID = guid;

                    if (Creature* pillar = Unit::GetCreature(*me, _activePillarGUID))
                    {
                        pillar->CastSpell(pillar, SPELL_LIGHTNING_ORB_CHARGED, true);
                        pillar->CastSpell((Unit*)NULL, SPELL_LIGHTNING_PILLAR_2, false);
                        events.ScheduleEvent(EVENT_LIGHTNING_CHARGE, 8000, 0, PHASE_2);
                    }
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case ACHIEVEMENT_DONT_STAND_IN_THE_LIGHTNING:
                        return _dontStandInTheLightning ? 1 : 0;
                    case ACHIEVEMENT_LOSE_YOUR_ILLUSION:
                    case DATA_THORIM_HARDMODE:
                        return _hardMode ? 1 : 0;
                    default:
                        break;
                }

                return 0;
            }

            void KilledUnit(Unit* who) OVERRIDE
            {
                if (who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spellInfo) OVERRIDE
            {
                if (spellInfo->Id == SPELL_TOUCH_OF_DOMINION_TRIGGERED)
                {
                    if (Creature* sif = GetThorimCreature(instance, DATA_SIF))
                    {
                        sif->AI()->Talk(SAY_SIF_DESPAWN);
                        sif->DespawnOrUnsummon(6000);
                        _hardMode = false;
                    }
                }
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spellInfo) OVERRIDE
            {
                if (target->GetTypeId() == TypeID::TYPEID_PLAYER && spellInfo->Id == SPELL_LIGHTNING_RELEASE)
                    _dontStandInTheLightning = false;
            }

            // Spells 64980 and 64985 are unknown to 5.4.8: give the achievement criteria they
            // would have triggered to every living player, with Thorim as the spell caster.
            void GrantCriteriaCredit(uint32 spellId)
            {
                Map::PlayerList const& players = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    Player* player = itr->GetSource();
                    if (!player || player->IsGameMaster() || !player->IsAlive())
                        continue;

                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, spellId, 0, 0, me);
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2, spellId);
                }
            }

            void FinishEncounter()
            {
                if (_encounterFinished)
                    return;

                _encounterFinished = true;

                // before the faction changes
                GrantCriteriaCredit(CRITERIA_SPELL_KILL);
                instance->UpdateEncounterState(EncounterCreditType::ENCOUNTER_CREDIT_KILL_CREATURE, me->GetEntry(), me);

                me->SetReactState(REACT_PASSIVE);
                me->InterruptNonMeleeSpells(true);
                me->RemoveAllAttackers();
                me->AttackStop();
                me->setFaction(35);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                if (Creature* controller = GetThorimCreature(instance, DATA_THORIM_CONTROLLER))
                    controller->RemoveAllAuras();
                if (Creature* pillar = Unit::GetCreature(*me, _activePillarGUID))
                    pillar->RemoveAllAuras();

                if (_hardMode)
                {
                    if (Creature* sif = GetThorimCreature(instance, DATA_SIF))
                    {
                        summons.Despawn(sif);
                        sif->DespawnOrUnsummon(10000);
                    }
                }

                // Thorim never dies, so nothing else locks the raid to this instance
                if (Player* looter = me->GetLootRecipient())
                    if (InstanceMap* instanceMap = instance->instance->ToInstanceMap())
                        if (instanceMap->IsRaidOrHeroicDungeon() && (me->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND))
                            instanceMap->PermBindAllPlayers(looter);

                _JustDied();

                Talk(SAY_DEATH);
                events.ScheduleEvent(EVENT_OUTRO_1, 4000);
                events.ScheduleEvent(EVENT_OUTRO_2, _hardMode ? 8000 : 11000);
                events.ScheduleEvent(EVENT_OUTRO_3, _hardMode ? 19000 : 21000);
                events.ScheduleEvent(EVENT_OUTRO_TELEPORT, 35000);
            }

            void MovementInform(uint32 type, uint32 id) OVERRIDE
            {
                if (type != EFFECT_MOTION_TYPE || id != EVENT_JUMP)
                    return;

                DoResetThreat();
                _boundary.Set(true);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO_1);

                events.SetPhase(PHASE_1);

                events.ScheduleEvent(EVENT_SAY_AGGRO_2, 9000, 0, PHASE_1);
                events.ScheduleEvent(EVENT_SAY_SIF_START, 16500, 0, PHASE_1);
                events.ScheduleEvent(EVENT_START_SIF_CHANNEL, 22500, 0, PHASE_1);

                events.ScheduleEvent(EVENT_STORMHAMMER, 40000, 0, PHASE_1);
                events.ScheduleEvent(EVENT_CHARGE_ORB, 30000, 0, PHASE_1);
                events.ScheduleEvent(EVENT_SUMMON_ADDS, 15000, 0, PHASE_1);
                events.ScheduleEvent(EVENT_BERSERK, 369000);

                DoCast(me, SPELL_SHEATH_OF_LIGHTNING);

                if (Creature* runicColossus = GetThorimCreature(instance, DATA_RUNIC_COLOSSUS))
                {
                    SetImmuneToPC(runicColossus, false);
                    runicColossus->AI()->DoAction(ACTION_ACTIVATE_ADDS);
                }

                if (GameObject* lever = GetThorimObject(instance, DATA_THORIM_LEVER))
                    lever->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);

                // Summon Sif
                me->SummonCreature(NPC_SIF, SifSpawnPosition);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                switch (summon->GetEntry())
                {
                    case NPC_LIGHTNING_ORB:
                    {
                        summon->SetReactState(REACT_PASSIVE);
                        summon->CastSpell(summon, SPELL_LIGHTNING_DESTRUCTION, true);

                        Movement::PointsArray path;
                        path.reserve(sizeof(LightningOrbPath) / sizeof(LightningOrbPath[0]));
                        for (Position const& pos : LightningOrbPath)
                            path.push_back(G3D::Vector3(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ()));

                        Movement::MoveSplineInit init(summon);
                        init.MovebyPath(path);
                        init.Launch();
                        break;
                    }
                    case NPC_DARK_RUNE_CHAMPION:
                    case NPC_DARK_RUNE_WARBRINGER:
                    case NPC_DARK_RUNE_EVOKER:
                    case NPC_DARK_RUNE_COMMONER:
                        summon->SetReactState(REACT_PASSIVE);
                        summon->m_Events.AddEvent(new TrashJumpEvent(summon), summon->m_Events.CalculateTime(3000));
                        break;
                    case NPC_SIF:
                        summon->SetReactState(REACT_PASSIVE);
                        break;
                    default:
                        break;
                }

                BossAI::JustSummoned(summon);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                // after the fight only the outro is left to run
                if (!_encounterFinished)
                {
                    if (!UpdateVictim())
                        return;

                    if (!_boundary.IsInBoundary(me))
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SAY_AGGRO_2:
                            Talk(SAY_AGGRO_2);
                            break;
                        case EVENT_SAY_SIF_START:
                            if (Creature* sif = GetThorimCreature(instance, DATA_SIF))
                                sif->AI()->Talk(SAY_SIF_START);
                            break;
                        case EVENT_START_SIF_CHANNEL:
                            if (Creature* sif = GetThorimCreature(instance, DATA_SIF))
                                sif->CastSpell(me, SPELL_TOUCH_OF_DOMINION, false);
                            break;
                        case EVENT_STORMHAMMER:
                            DoCastAOE(SPELL_STORMHAMMER);
                            events.ScheduleEvent(EVENT_STORMHAMMER, urand(15000, 20000), 0, PHASE_1);
                            break;
                        case EVENT_CHARGE_ORB:
                            DoCastAOE(SPELL_CHARGE_ORB);
                            events.ScheduleEvent(EVENT_CHARGE_ORB, urand(15000, 20000), 0, PHASE_1);
                            break;
                        case EVENT_SUMMON_ADDS:
                            SummonWave();
                            events.ScheduleEvent(EVENT_SUMMON_ADDS, _orbSummoned ? 3000 : 10000, 0, PHASE_1);
                            break;
                        case EVENT_JUMPDOWN:
                            if (_hardMode)
                                if (Creature* sif = GetThorimCreature(instance, DATA_SIF))
                                    sif->AI()->DoAction(ACTION_START_HARD_MODE);
                            me->RemoveAurasDueToSpell(SPELL_SHEATH_OF_LIGHTNING);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->SetDisableGravity(false);
                            me->SetControlled(false, UNIT_STATE_ROOT);
                            me->GetMotionMaster()->MoveJump(2134.8f, -263.056f, 419.983f, 30.0f, 20.0f);
                            events.ScheduleEvent(EVENT_START_PERIODIC_CHARGE, 2000, 0, PHASE_2);
                            events.ScheduleEvent(EVENT_UNBALANCING_STRIKE, 15000, 0, PHASE_2);
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 20000, 0, PHASE_2);
                            break;
                        case EVENT_UNBALANCING_STRIKE:
                            DoCastVictim(SPELL_UNBALANCING_STRIKE);
                            events.ScheduleEvent(EVENT_UNBALANCING_STRIKE, urand(15000, 20000), 0, PHASE_2);
                            break;
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_CHAIN_LIGHTNING);
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(7000, 15000), 0, PHASE_2);
                            break;
                        case EVENT_START_PERIODIC_CHARGE:
                            if (Creature* controller = GetThorimCreature(instance, DATA_THORIM_CONTROLLER))
                                controller->CastSpell(controller, SPELL_ACTIVATE_LIGHTNING_ORB_PERIODIC, true);
                            break;
                        case EVENT_LIGHTNING_CHARGE:
                            if (Creature* pillar = Unit::GetCreature(*me, _activePillarGUID))
                                DoCast(pillar, SPELL_LIGHTNING_RELEASE);
                            break;
                        case EVENT_BERSERK:
                            if (events.IsInPhase(PHASE_1))
                            {
                                Talk(SAY_WIPE);
                                DoCastAOE(SPELL_BERSERK_PHASE_1, true);
                                DoCast(me, SPELL_SUMMON_LIGHTNING_ORB, true);
                            }
                            else
                            {
                                Talk(SAY_BERSERK);
                                DoCast(me, SPELL_BERSERK_PHASE_2, true);
                            }
                            break;
                        case EVENT_ACTIVATE_LIGHTNING_FIELD:
                        {
                            std::list<Creature*> triggers;
                            me->GetCreatureListWithEntryInGrid(triggers, NPC_THORIM_EVENT_BUNNY, 100.0f);
                            triggers.remove_if([](Creature* bunny)
                            {
                                if (HeightPositionCheck(false)(bunny))
                                    return true;
                                return LightningFieldCenter.GetExactDist2dSq(bunny) > 1296.0f;
                            });

                            uint32 timer = 1000;
                            for (Creature* bunny : triggers)
                            {
                                timer += 100;
                                bunny->m_Events.AddEvent(new LightningFieldEvent(bunny), bunny->m_Events.CalculateTime(timer));
                            }

                            triggers.remove_if([](Creature* bunny)
                            {
                                return LightningFieldCenter.GetExactDist2dSq(bunny) < 576.0f;
                            });

                            triggers.sort([](Creature* a, Creature* b)
                            {
                                return a->GetPositionX() < b->GetPositionX();
                            });

                            for (std::list<Creature*>::const_iterator itr = triggers.begin(); itr != triggers.end();)
                            {
                                std::list<Creature*>::const_iterator prev = itr++;
                                if (itr != triggers.end())
                                    (*prev)->CastSpell(*itr, SPELL_LIGHTNING_BEAM_CHANNEL, false);
                            }
                            break;
                        }
                        case EVENT_OUTRO_1:
                            Talk(_hardMode ? SAY_END_HARD_1 : SAY_END_NORMAL_1);
                            if (_hardMode)
                                DoCast(me, SPELL_STORMHAMMER_SIF);
                            break;
                        case EVENT_OUTRO_2:
                            Talk(_hardMode ? SAY_END_HARD_2 : SAY_END_NORMAL_2);
                            if (_hardMode)
                                if (Creature* sif = GetThorimCreature(instance, DATA_SIF))
                                    sif->SetStandState(UNIT_STAND_STATE_DEAD);
                            break;
                        case EVENT_OUTRO_3:
                            Talk(_hardMode ? SAY_END_HARD_3 : SAY_END_NORMAL_3);
                            break;
                        case EVENT_OUTRO_TELEPORT:
                            DoCast(me, SPELL_TELEPORT_KEEPER_VISUAL, true);
                            me->DespawnOrUnsummon(1500);
                            break;
                        default:
                            break;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }

                if (!_encounterFinished)
                    DoMeleeAttackIfReady();
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_BERSERK:
                        if (events.IsInPhase(PHASE_2))
                            return;

                        if (!_orbSummoned)
                        {
                            _orbSummoned = true;
                            events.RescheduleEvent(EVENT_BERSERK, 1000);
                        }
                        return;
                    case ACTION_INCREASE_PREADDS_COUNT:
                        if (++_killedCount >= PRE_PHASE_ADD_COUNT)
                        {
                            // Event starts
                            SetImmuneToPC(me, false);
                            DoZoneInCombat(me);
                        }
                        break;
                    default:
                        break;
                }
            }

            void GetTrashSpawnTriggers(std::list<Creature*>& triggerList, uint32 count = 1)
            {
                me->GetCreatureListWithEntryInGrid(triggerList, NPC_THORIM_EVENT_BUNNY, 100.0f);
                triggerList.remove_if([](Creature* bunny)
                {
                    if (HeightPositionCheck(false)(bunny))
                        return true;
                    return ArenaCenter.GetExactDist2dSq(bunny) < 3025.0f;
                });

                if (triggerList.empty())
                    return;

                if (count == 1)
                {
                    Creature* bunny = Skyfire::Containers::SelectRandomContainerElement(triggerList);
                    triggerList.clear();
                    triggerList.push_back(bunny);
                }
                else
                    Skyfire::Containers::RandomResizeList(triggerList, count);
            }

            void SummonWave()
            {
                switch (_waveType)
                {
                    case 0:
                    {
                        // Dark Rune Commoner
                        std::list<Creature*> triggers;
                        GetTrashSpawnTriggers(triggers, urand(5, 6));

                        for (Creature* bunny : triggers)
                            me->SummonCreature(StaticThorimTrashInfo[6 + 3].Entry, *bunny, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);

                        ++_waveType;
                        break;
                    }
                    case 1:
                        if (urand(0, 1))
                        {
                            // Dark Rune Champion or Dark Rune Evoker
                            std::list<Creature*> triggers;
                            GetTrashSpawnTriggers(triggers, urand(2, 4));

                            for (Creature* bunny : triggers)
                                me->SummonCreature(StaticThorimTrashInfo[6 + RAND(0, 2)].Entry, *bunny, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                        }
                        else
                        {
                            // Dark Rune Warbringer
                            std::list<Creature*> triggers;
                            GetTrashSpawnTriggers(triggers);

                            for (Creature* bunny : triggers)
                                me->SummonCreature(StaticThorimTrashInfo[6 + 1].Entry, *bunny, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                        }
                        --_waveType;
                        break;
                }
            }

            bool CanStartPhase2(Unit* actor) const
            {
                if (!actor || actor->GetTypeId() != TypeID::TYPEID_PLAYER || !me->IsWithinDistInMap(actor, 10.0f))
                    return false;

                Creature* runicColossus = GetThorimCreature(instance, DATA_RUNIC_COLOSSUS);
                Creature* runeGiant = GetThorimCreature(instance, DATA_RUNE_GIANT);
                return runicColossus && !runicColossus->IsAlive() && runeGiant && !runeGiant->IsAlive();
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (_encounterFinished)
                {
                    damage = 0;
                    return;
                }

                if (events.IsInPhase(PHASE_1) && CanStartPhase2(attacker))
                {
                    Talk(SAY_JUMPDOWN);
                    events.SetPhase(PHASE_2);
                    events.ScheduleEvent(EVENT_JUMPDOWN, 8000);
                    events.ScheduleEvent(EVENT_ACTIVATE_LIGHTNING_FIELD, 15000);
                    events.RescheduleEvent(EVENT_BERSERK, 300000, 0, PHASE_2);

                    if (Creature* sif = GetThorimCreature(instance, DATA_SIF))
                        sif->InterruptNonMeleeSpells(false);

                    // Hard Mode
                    if (_hardMode)
                        GrantCriteriaCredit(CRITERIA_SPELL_SIFFED);
                }
                else if (me->HealthBelowPctDamaged(1, damage))
                {
                    damage = 0;
                    FinishEncounter();
                }
            }

        private:
            uint64 _activePillarGUID;
            uint8 _killedCount;
            uint8 _waveType;
            bool _hardMode;
            bool _encounterFinished;
            bool _orbSummoned;
            bool _dontStandInTheLightning;
            ArenaBoundary _boundary;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetUlduarAI<boss_thorimAI>(creature);
        }
};

struct npc_thorim_trashAI : public ScriptedAI
{
    npc_thorim_trashAI(Creature* creature) : ScriptedAI(creature), _info(NULL)
    {
        _instance = creature->GetInstanceScript();
        for (uint8 i = 0; i < ThorimTrashCount; ++i)
            if (me->GetEntry() == StaticThorimTrashInfo[i].Entry)
                _info = &StaticThorimTrashInfo[i];

        ASSERT(_info);
    }

    struct AIHelper
    {
        /// returns heal amount of the given spell including hots
        static uint32 GetTotalHeal(SpellInfo const* spellInfo, Unit const* caster)
        {
            uint32 heal = 0;
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                SpellEffectInfo const& spellEffectInfo = spellInfo->Effects[i];
                if (spellEffectInfo.IsEffect(SPELL_EFFECT_HEAL))
                    heal += spellEffectInfo.CalcValue(caster);

                if (spellEffectInfo.IsEffect(SPELL_EFFECT_APPLY_AURA) && spellEffectInfo.IsAura(SPELL_AURA_PERIODIC_HEAL))
                    heal += spellInfo->GetMaxTicks() * spellEffectInfo.CalcValue(caster);
            }
            return heal;
        }

        /// returns remaining heal amount on given target
        static uint32 GetRemainingHealOn(Unit* target)
        {
            uint32 heal = 0;
            Unit::AuraEffectList const& auras = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_HEAL);
            for (Unit::AuraEffectList::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                AuraEffect const* aurEff = *itr;
                int32 remainingTicks = aurEff->GetTotalTicks() - int32(aurEff->GetTickNumber());
                if (remainingTicks > 0)
                    heal += aurEff->GetAmount() * remainingTicks;
            }

            return heal;
        }

        class MostHPMissingInRange
        {
            public:
                MostHPMissingInRange(Unit const* referer, float range, uint32 hp, bool exclSelf = false)
                    : _referer(referer), _range(range), _hp(hp), _exclSelf(exclSelf) { }

                bool operator()(Unit* u)
                {
                    if (_exclSelf && u == _referer)
                        return false;

                    if ((u->GetHealth() + GetRemainingHealOn(u) + _hp) > u->GetMaxHealth())
                        return false;

                    uint32 missingHP = u->GetMaxHealth() - u->GetHealth();
                    if (u->IsAlive() && _referer->IsFriendlyTo(u) && _referer->IsWithinDistInMap(u, _range) && missingHP > _hp)
                    {
                        _hp = missingHP;
                        return true;
                    }

                    return false;
                }

            private:
                Unit const* _referer;
                float _range;
                uint32 _hp;
                bool _exclSelf;
        };

        static Unit* GetUnitWithMostMissingHp(SpellInfo const* spellInfo, Unit* caster)
        {
            // use positive range, it's a healing spell
            float const range = spellInfo->GetMaxRange(true);
            uint32 const heal = GetTotalHeal(spellInfo, caster);

            Unit* target = NULL;
            MostHPMissingInRange checker(caster, range, heal);
            Skyfire::UnitLastSearcher<MostHPMissingInRange> searcher(caster, target, checker);
            caster->VisitNearbyObject(range, searcher);

            return target;
        }

        static Unit* GetHealTarget(SpellInfo const* spellInfo, Unit* caster)
        {
            Unit* healTarget = NULL;
            if (!(spellInfo->AttributesEx & SPELL_ATTR1_CANT_TARGET_SELF) && !roll_chance_f(caster->GetHealthPct()) && ((caster->GetHealth() + GetRemainingHealOn(caster) + GetTotalHeal(spellInfo, caster)) <= caster->GetMaxHealth()))
                healTarget = caster;
            else
                healTarget = GetUnitWithMostMissingHp(spellInfo, caster);

            return healTarget;
        }
    };

    bool UseAbility(uint32 spellId)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
            return false;

        Unit* target = NULL;
        if (AIHelper::GetTotalHeal(spellInfo, me))
            target = AIHelper::GetHealTarget(spellInfo, me);
        else
            target = me->GetVictim();

        if (!target)
            return false;

        if (_info->Type == MERCENARY_SOLDIER)
        {
            bool allowMove = true;
            if (me->IsInRange(target, spellInfo->GetMinRange(), spellInfo->GetMaxRange()))
                allowMove = false;

            if (IsCombatMovementAllowed() != allowMove)
            {
                SetCombatMovement(allowMove);

                // need relaunch movement
                ScriptedAI::AttackStart(target);

                // give some time to allow reposition, try again in a second
                if (allowMove)
                    return false;
            }
        }

        DoCast(target, spellId);
        return true;
    }

    // Arena creatures leave combat when they end up outside of their area
    virtual bool IsInBoundary() const { return true; }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        if (!IsInBoundary())
        {
            EnterEvadeMode();
            return;
        }

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            HandleEvent(eventId);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        if (_info->Type == DARK_RUNE_ACOLYTE)
            DoSpellAttackIfReady(SPELL_HOLY_SMITE);
        else
            DoMeleeAttackIfReady();
    }

    virtual void HandleEvent(uint32 eventId) = 0;

protected:
    InstanceScript* _instance;
    EventMap _events;

    ThorimTrashInfo const* _info;
};

class npc_thorim_pre_phase : public CreatureScript
{
    public:
        npc_thorim_pre_phase() : CreatureScript("npc_thorim_pre_phase") { }

        struct npc_thorim_pre_phaseAI : public npc_thorim_trashAI
        {
            npc_thorim_pre_phaseAI(Creature* creature) : npc_thorim_trashAI(creature)
            {
                me->setActive(true); // prevent grid unload
            }

            void Reset() OVERRIDE
            {
                _events.Reset();
                if (_info->PrimaryAbility)
                    _events.ScheduleEvent(EVENT_PRIMARY_ABILITY, urand(3000, 6000));
                if (_info->SecondaryAbility)
                {
                    if (_info->SecondaryAbility == SPELL_SHOOT)
                        _events.ScheduleEvent(EVENT_SECONDARY_ABILITY, 2000);
                    else
                        _events.ScheduleEvent(EVENT_SECONDARY_ABILITY, urand(12000, 15000));
                }
                if (_info->ThirdAbility)
                    _events.ScheduleEvent(EVENT_THIRD_ABILITY, urand(6000, 8000));
                if (_info->Type == MERCENARY_SOLDIER)
                    SetCombatMovement(false);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (Creature* thorim = GetThorimCreature(_instance, BOSS_THORIM))
                    thorim->AI()->DoAction(ACTION_INCREASE_PREADDS_COUNT);
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                // nullify spell damage
                if (!attacker || !attacker->GetAffectingPlayer())
                    damage = 0;
            }

            void HandleEvent(uint32 eventId) OVERRIDE
            {
                switch (eventId)
                {
                    case EVENT_PRIMARY_ABILITY:
                        if (UseAbility(_info->PrimaryAbility))
                            _events.ScheduleEvent(eventId, urand(15000, 20000));
                        else
                            _events.ScheduleEvent(eventId, 1000);
                        break;
                    case EVENT_SECONDARY_ABILITY:
                        if (UseAbility(_info->SecondaryAbility))
                        {
                            if (_info->SecondaryAbility == SPELL_SHOOT)
                                _events.ScheduleEvent(eventId, 2000);
                            else
                                _events.ScheduleEvent(eventId, urand(4000, 8000));
                        }
                        else
                            _events.ScheduleEvent(eventId, 1000);
                        break;
                    case EVENT_THIRD_ABILITY:
                        if (UseAbility(_info->ThirdAbility))
                            _events.ScheduleEvent(eventId, urand(6000, 8000));
                        else
                            _events.ScheduleEvent(eventId, 1000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetUlduarAI<npc_thorim_pre_phaseAI>(creature);
        }
};

class npc_thorim_arena_phase : public CreatureScript
{
    public:
        npc_thorim_arena_phase() : CreatureScript("npc_thorim_arena_phase") { }

        struct npc_thorim_arena_phaseAI : public npc_thorim_trashAI
        {
            npc_thorim_arena_phaseAI(Creature* creature) : npc_thorim_trashAI(creature)
            {
                switch (_info->Type)
                {
                    case DARK_RUNE_CHAMPION:
                    case DARK_RUNE_WARBRINGER:
                    case DARK_RUNE_COMMONER:
                    case DARK_RUNE_EVOKER:
                        _isInArena = true;
                        break;
                    case DARK_RUNE_ACOLYTE:
                    {
                        _isInArena = (_info->Entry == NPC_DARK_RUNE_ACOLYTE_PRE);
                        _boundary.Set(true, !_isInArena);
                        break;
                    }
                    default:
                        _isInArena = false;
                        break;
                }
            }

            bool CanAIAttack(Unit const* who) const OVERRIDE
            {
                // don't try to attack players in balcony
                if (_isInArena && HeightPositionCheck(true)(who))
                    return false;

                return _boundary.IsInBoundary(who);
            }

            bool IsInBoundary() const OVERRIDE
            {
                return _boundary.IsInBoundary(me);
            }

            void DoAction(int32 action) OVERRIDE
            {
                // the arena creatures only stay bound to the arena once they have jumped into it
                if (action == ACTION_ENABLE_BOUNDARY)
                    _boundary.Set(true);
            }

            void Reset() OVERRIDE
            {
                _events.Reset();
                if (_info->PrimaryAbility)
                    _events.ScheduleEvent(EVENT_PRIMARY_ABILITY, urand(3000, 6000));
                if (_info->SecondaryAbility)
                    _events.ScheduleEvent(EVENT_SECONDARY_ABILITY, urand(7000, 9000));
                if (_info->ThirdAbility)
                    _events.ScheduleEvent(EVENT_THIRD_ABILITY, urand(6000, 8000));
                if (_info->Type == DARK_RUNE_CHAMPION)
                    _events.ScheduleEvent(EVENT_ABILITY_CHARGE, 8000);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (_info->Type == DARK_RUNE_WARBRINGER)
                    DoCast(me, SPELL_AURA_OF_CELERITY);

                if (!_isInArena)
                    if (Creature* colossus = GetThorimCreature(_instance, DATA_RUNIC_COLOSSUS))
                        colossus->AI()->DoAction(ACTION_ACTIVATE_RUNIC_SMASH);
            }

            void EnterEvadeMode() OVERRIDE
            {
                // this should only happen if theres no alive player in the arena -> summon orb
                if (Creature* thorim = GetThorimCreature(_instance, BOSS_THORIM))
                    thorim->AI()->DoAction(ACTION_BERSERK);
                ScriptedAI::EnterEvadeMode();
            }

            void HandleEvent(uint32 eventId) OVERRIDE
            {
                switch (eventId)
                {
                    case EVENT_PRIMARY_ABILITY:
                        if (UseAbility(_info->PrimaryAbility))
                            _events.ScheduleEvent(eventId, urand(3000, 6000));
                        else
                            _events.ScheduleEvent(eventId, 1000);
                        break;
                    case EVENT_SECONDARY_ABILITY:
                        if (UseAbility(_info->SecondaryAbility))
                            _events.ScheduleEvent(eventId, urand(12000, 16000));
                        else
                            _events.ScheduleEvent(eventId, 1000);
                        break;
                    case EVENT_THIRD_ABILITY:
                        if (UseAbility(_info->ThirdAbility))
                            _events.ScheduleEvent(eventId, urand(6000, 8000));
                        else
                            _events.ScheduleEvent(eventId, 1000);
                        break;
                    case EVENT_ABILITY_CHARGE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, PlayerInRangePredicate(me, 8.0f, 25.0f)))
                            DoCast(target, SPELL_CHARGE);
                        _events.ScheduleEvent(eventId, 12000);
                        break;
                    default:
                        break;
                }
            }

        private:
            bool _isInArena;
            ArenaBoundary _boundary;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetUlduarAI<npc_thorim_arena_phaseAI>(creature);
        }
};

struct npc_thorim_minibossAI : public ScriptedAI
{
    npc_thorim_minibossAI(Creature* creature) : ScriptedAI(creature), _summons(creature)
    {
        _instance = creature->GetInstanceScript();

        // the mini bosses and their adds only fight players who are not in the arena
        _boundary.Set(true, true);
    }

    bool CanAIAttack(Unit const* who) const OVERRIDE
    {
        return _boundary.IsInBoundary(who);
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        _summons.Summon(summon);
    }

    void SummonedCreatureDespawn(Creature* summon) OVERRIDE
    {
        _summons.Despawn(summon);
    }

    void DoAction(int32 action) OVERRIDE
    {
        if (action == ACTION_ACTIVATE_ADDS)
        {
            for (SummonList::const_iterator itr = _summons.begin(); itr != _summons.end(); ++itr)
                if (Creature* summon = Unit::GetCreature(*me, *itr))
                    SetImmuneToPC(summon, false);
        }
    }

protected:
    InstanceScript* _instance;
    EventMap _events;
    SummonList _summons;
    ArenaBoundary _boundary;
};

class npc_runic_colossus : public CreatureScript
{
    public:
        npc_runic_colossus() : CreatureScript("npc_runic_colossus") { }

        struct npc_runic_colossusAI : public npc_thorim_minibossAI
        {
            npc_runic_colossusAI(Creature* creature) : npc_thorim_minibossAI(creature)
            {
                Initialize();
            }

            void Initialize()
            {
                _runicActive = false;
            }

            void Reset() OVERRIDE
            {
                Initialize();
                _events.Reset();

                // close the Runic Door
                _instance->HandleGameObject(_instance->GetData64(DATA_RUNIC_DOOR), false);

                // Spawn trashes
                _summons.DespawnAll();
                for (SummonLocation const& s : ColossusAddLocations)
                    me->SummonCreature(s.entry, s.pos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            }

            void MoveInLineOfSight(Unit* /*who*/) OVERRIDE
            {
                // don't enter combat
            }

            void DoAction(int32 action) OVERRIDE
            {
                npc_thorim_minibossAI::DoAction(action);

                if (_runicActive)
                    return;

                if (action == ACTION_ACTIVATE_RUNIC_SMASH)
                {
                    _runicActive = true;
                    _events.ScheduleEvent(EVENT_RUNIC_SMASH, 7000);
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                // open the Runic Door
                _instance->HandleGameObject(_instance->GetData64(DATA_RUNIC_DOOR), true);

                if (Creature* thorim = GetThorimCreature(_instance, BOSS_THORIM))
                    thorim->AI()->Talk(SAY_SPECIAL);

                if (Creature* giant = GetThorimCreature(_instance, DATA_RUNE_GIANT))
                {
                    SetImmuneToPC(giant, false);
                    giant->AI()->DoAction(ACTION_ACTIVATE_ADDS);
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                DoZoneInCombat();
                _events.Reset();
                _events.ScheduleEvent(EVENT_RUNIC_BARRIER, urand(12000, 15000));
                _events.ScheduleEvent(EVENT_SMASH, urand(15000, 18000));
                _events.ScheduleEvent(EVENT_RUNIC_CHARGE, urand(20000, 24000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RUNIC_BARRIER:
                            Talk(EMOTE_RUNIC_BARRIER);
                            DoCastAOE(SPELL_RUNIC_BARRIER);
                            _events.ScheduleEvent(eventId, urand(35000, 45000));
                            break;
                        case EVENT_SMASH:
                            DoCastAOE(SPELL_SMASH);
                            _events.ScheduleEvent(eventId, urand(15000, 18000));
                            break;
                        case EVENT_RUNIC_CHARGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, PlayerInRangePredicate(me, 8.0f, 40.0f)))
                                DoCast(target, SPELL_RUNIC_CHARGE);
                            _events.ScheduleEvent(eventId, 20000);
                            break;
                        case EVENT_RUNIC_SMASH:
                            DoCast(me, urand(0, 1) ? SPELL_RUNIC_SMASH_LEFT : SPELL_RUNIC_SMASH_RIGHT);
                            _events.ScheduleEvent(eventId, 6000);
                            break;
                        default:
                            break;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:
            bool _runicActive;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetUlduarAI<npc_runic_colossusAI>(creature);
        }
};

class npc_ancient_rune_giant : public CreatureScript
{
    public:
        npc_ancient_rune_giant() : CreatureScript("npc_ancient_rune_giant") { }

        struct npc_ancient_rune_giantAI : public npc_thorim_minibossAI
        {
            npc_ancient_rune_giantAI(Creature* creature) : npc_thorim_minibossAI(creature) { }

            void Reset() OVERRIDE
            {
                _events.Reset();

                // close the Stone Door
                _instance->HandleGameObject(_instance->GetData64(DATA_STONE_DOOR), false);

                // Spawn trashes
                _summons.DespawnAll();
                for (SummonLocation const& s : GiantAddLocations)
                    me->SummonCreature(s.entry, s.pos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                DoZoneInCombat();
                _events.Reset();
                _events.ScheduleEvent(EVENT_RUNIC_FORTIFICATION, 1);
                _events.ScheduleEvent(EVENT_STOMP, urand(10000, 12000));
                _events.ScheduleEvent(EVENT_RUNE_DETONATION, 25000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                // open the Stone Door
                _instance->HandleGameObject(_instance->GetData64(DATA_STONE_DOOR), true);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RUNIC_FORTIFICATION:
                            Talk(EMOTE_RUNIC_MIGHT);
                            DoCastAOE(SPELL_RUNIC_FORTIFICATION);
                            break;
                        case EVENT_STOMP:
                            DoCastAOE(SPELL_STOMP);
                            _events.ScheduleEvent(eventId, urand(10000, 12000));
                            break;
                        case EVENT_RUNE_DETONATION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                                DoCast(target, SPELL_RUNE_DETONATION);
                            _events.ScheduleEvent(eventId, urand(10000, 12000));
                            break;
                        default:
                            break;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetUlduarAI<npc_ancient_rune_giantAI>(creature);
        }
};

class npc_sif : public CreatureScript
{
    public:
        npc_sif() : CreatureScript("npc_sif") { }

        struct npc_sifAI : public ScriptedAI
        {
            npc_sifAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
                _instance = creature->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                _events.Reset();
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spellInfo) OVERRIDE
            {
                if (spellInfo->Id == SPELL_STORMHAMMER_SIF)
                {
                    me->InterruptSpell(CURRENT_GENERIC_SPELL);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                }
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_START_HARD_MODE)
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoZoneInCombat(me);
                    Talk(SAY_SIF_EVENT);
                    _events.Reset();
                    _events.ScheduleEvent(EVENT_FROSTBOLT, 2000);
                    _events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 15000);
                    _events.ScheduleEvent(EVENT_BLINK, urand(20000, 25000));
                    _events.ScheduleEvent(EVENT_BLIZZARD, 30000);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLINK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BLINK);
                            _events.ScheduleEvent(EVENT_FROST_NOVA, 1);
                            _events.ScheduleEvent(eventId, urand(20000, 25000));
                            return;
                        case EVENT_FROST_NOVA:
                            DoCastAOE(SPELL_FROSTNOVA);
                            return;
                        case EVENT_FROSTBOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_FROSTBOLT);
                            _events.ScheduleEvent(eventId, 2000);
                            return;
                        case EVENT_FROSTBOLT_VOLLEY:
                            DoCastAOE(SPELL_FROSTBOLT_VOLLEY);
                            _events.ScheduleEvent(eventId, urand(15000, 20000));
                            return;
                        case EVENT_BLIZZARD:
                            DoCastAOE(SPELL_BLIZZARD);
                            _events.ScheduleEvent(eventId, urand(35000, 45000));
                            return;
                        default:
                            break;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }

                // no melee attack
            }

        private:
            EventMap _events;
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetUlduarAI<npc_sifAI>(creature);
        }
};

// 62576 - Blizzard
// 62602 - Blizzard
class spell_thorim_blizzard_effect : public SpellScriptLoader
{
    public:
        spell_thorim_blizzard_effect() : SpellScriptLoader("spell_thorim_blizzard_effect") { }

        class spell_thorim_blizzard_effect_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_thorim_blizzard_effect_AuraScript);

            bool CheckAreaTarget(Unit* target)
            {
                /// @todo: fix this for all dynobj auras
                if (target != GetOwner())
                {
                    // check if not stacking aura already on target
                    // this one prevents overriding auras periodically by 2 near area aura owners
                    Unit::AuraApplicationMap const& auraMap = target->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::const_iterator iter = auraMap.begin(); iter != auraMap.end(); ++iter)
                    {
                        Aura const* aura = iter->second->GetBase();
                        if (GetId() == aura->GetId() && GetOwner() != aura->GetOwner() /*!GetAura()->CanStackWith(aura)*/)
                            return false;
                    }
                }

                return true;
            }

            void Register() OVERRIDE
            {
                DoCheckAreaTarget += AuraCheckAreaTargetFn(spell_thorim_blizzard_effect_AuraScript::CheckAreaTarget);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_thorim_blizzard_effect_AuraScript();
        }
};

// 62580, 62604 - Frostbolt Volley
class spell_thorim_frostbolt_volley : public SpellScriptLoader
{
    public:
        spell_thorim_frostbolt_volley() : SpellScriptLoader("spell_thorim_frostbolt_volley") { }

        class spell_thorim_frostbolt_volley_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thorim_frostbolt_volley_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if([](WorldObject* object) -> bool
                {
                    if (object->GetTypeId() == TypeID::TYPEID_PLAYER)
                        return false;

                    if (Creature* creature = object->ToCreature())
                        return !creature->IsPet();

                    return true;
                });
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thorim_frostbolt_volley_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thorim_frostbolt_volley_SpellScript();
        }
};

// 62016 - Charge Orb
class spell_thorim_charge_orb : public SpellScriptLoader
{
    public:
        spell_thorim_charge_orb() : SpellScriptLoader("spell_thorim_charge_orb") { }

        class spell_thorim_charge_orb_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thorim_charge_orb_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                return sSpellMgr->GetSpellInfo(SPELL_LIGHTNING_PILLAR_1) != NULL;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(HeightPositionCheck(false));

                if (targets.empty())
                    return;

                WorldObject* target = Skyfire::Containers::SelectRandomContainerElement(targets);
                targets.clear();
                targets.push_back(target);
            }

            void HandleScript()
            {
                if (Unit* target = GetHitUnit())
                    target->CastSpell((Unit*)NULL, SPELL_LIGHTNING_PILLAR_1, true);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thorim_charge_orb_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                AfterHit += SpellHitFn(spell_thorim_charge_orb_SpellScript::HandleScript);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thorim_charge_orb_SpellScript();
        }
};

// 62466 - Lightning Charge
class spell_thorim_lightning_charge : public SpellScriptLoader
{
    public:
        spell_thorim_lightning_charge() : SpellScriptLoader("spell_thorim_lightning_charge") { }

        class spell_thorim_lightning_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thorim_lightning_charge_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                return sSpellMgr->GetSpellInfo(SPELL_LIGHTNING_CHARGE) != NULL;
            }

            void HandleFocus()
            {
                // the cast is triggered and instant, so face the target by hand
                if (WorldObject* target = GetExplTargetWorldObject())
                    GetCaster()->SetFacingToObject(target);
            }

            void HandleCharge()
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_LIGHTNING_CHARGE, false);
            }

            void Register() OVERRIDE
            {
                BeforeCast += SpellCastFn(spell_thorim_lightning_charge_SpellScript::HandleFocus);
                AfterCast += SpellCastFn(spell_thorim_lightning_charge_SpellScript::HandleCharge);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thorim_lightning_charge_SpellScript();
        }
};

// 61934 - Leap
class spell_thorim_arena_leap : public SpellScriptLoader
{
    public:
        spell_thorim_arena_leap() : SpellScriptLoader("spell_thorim_arena_leap") { }

        class spell_thorim_arena_leap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thorim_arena_leap_SpellScript);

            bool Load() OVERRIDE
            {
                return GetCaster()->GetTypeId() == TypeID::TYPEID_UNIT;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Position const* pos = GetHitDest())
                    GetCaster()->ToCreature()->SetHomePosition(*pos);
            }

            void Register() OVERRIDE
            {
                OnEffectLaunch += SpellEffectFn(spell_thorim_arena_leap_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_JUMP_DEST);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thorim_arena_leap_SpellScript();
        }
};

// 62042 - Stormhammer
class spell_thorim_stormhammer : public SpellScriptLoader
{
    public:
        spell_thorim_stormhammer() : SpellScriptLoader("spell_thorim_stormhammer") { }

        class spell_thorim_stormhammer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thorim_stormhammer_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                return sSpellMgr->GetSpellInfo(SPELL_STORMHAMMER_BOOMERANG) && sSpellMgr->GetSpellInfo(SPELL_DEAFENING_THUNDER);
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if([](WorldObject* target) -> bool { return HeightPositionCheck(true)(target) || OutOfArenaCheck()(target); });

                if (targets.empty())
                {
                    FinishCast(SpellCastResult::SPELL_FAILED_NO_VALID_TARGETS);
                    return;
                }

                WorldObject* target = Skyfire::Containers::SelectRandomContainerElement(targets);
                targets.clear();
                targets.push_back(target);
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    target->CastSpell(target, SPELL_DEAFENING_THUNDER, true);
                    target->CastSpell(GetCaster(), SPELL_STORMHAMMER_BOOMERANG, true);
                }
            }

            void LoseHammer()
            {
                GetCaster()->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID, 0);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thorim_stormhammer_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENEMY);
                AfterCast += SpellCastFn(spell_thorim_stormhammer_SpellScript::LoseHammer);
                OnEffectHitTarget += SpellEffectFn(spell_thorim_stormhammer_SpellScript::HandleScript, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thorim_stormhammer_SpellScript();
        }
};

// 64767 - Stormhammer
class spell_thorim_stormhammer_sif : public SpellScriptLoader
{
    public:
        spell_thorim_stormhammer_sif() : SpellScriptLoader("spell_thorim_stormhammer_sif") { }

        class spell_thorim_stormhammer_sif_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thorim_stormhammer_sif_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                return sSpellMgr->GetSpellInfo(SPELL_STORMHAMMER_BOOMERANG) && sSpellMgr->GetSpellInfo(SPELL_SIF_TRANSFORM);
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    target->CastSpell(GetCaster(), SPELL_STORMHAMMER_BOOMERANG, true);
                    target->CastSpell(target, SPELL_SIF_TRANSFORM, true);
                }
            }

            void LoseHammer()
            {
                GetCaster()->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID, 0);
            }

            void Register() OVERRIDE
            {
                AfterCast += SpellCastFn(spell_thorim_stormhammer_sif_SpellScript::LoseHammer);
                OnEffectHitTarget += SpellEffectFn(spell_thorim_stormhammer_sif_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thorim_stormhammer_sif_SpellScript();
        }
};

// 64909 - Stormhammer
class spell_thorim_stormhammer_boomerang : public SpellScriptLoader
{
    public:
        spell_thorim_stormhammer_boomerang() : SpellScriptLoader("spell_thorim_stormhammer_boomerang") { }

        class spell_thorim_stormhammer_boomerang_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thorim_stormhammer_boomerang_SpellScript);

            void RecoverHammer(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                    target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID, THORIM_WEAPON_ITEM_ID);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_thorim_stormhammer_boomerang_SpellScript::RecoverHammer, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thorim_stormhammer_boomerang_SpellScript();
        }
};

// 62057, 62058 - Runic Smash
class spell_thorim_runic_smash : public SpellScriptLoader
{
    public:
        spell_thorim_runic_smash() : SpellScriptLoader("spell_thorim_runic_smash") { }

        class spell_thorim_runic_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thorim_runic_smash_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                return sSpellMgr->GetSpellInfo(SPELL_RUNIC_SMASH) != NULL;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                std::list<Creature*> triggers;
                GetCaster()->GetCreatureListWithEntryInGrid(triggers, GetSpellInfo()->Id == SPELL_RUNIC_SMASH_LEFT ? NPC_GOLEM_LEFT_HAND_BUNNY : NPC_GOLEM_RIGHT_HAND_BUNNY, 150.0f);
                for (Creature* trigger : triggers)
                {
                    uint32 time = uint32(GetCaster()->GetExactDist(trigger) * 30.f);
                    trigger->m_Events.AddEvent(new RunicSmashExplosionEvent(trigger), trigger->m_Events.CalculateTime(time));
                }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_thorim_runic_smash_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_TRIGGER_SPELL);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_thorim_runic_smash_SpellScript();
        }
};

class UpperOrbCheck
{
    public:
        UpperOrbCheck() : _check(true) { }

        bool operator() (Creature* target) const
        {
            return target->GetEntry() == NPC_THUNDER_ORB && _check(target);
        }

    private:
        HeightPositionCheck const _check;
};

// 62184 - Activate Lightning Orb Periodic
class spell_thorim_activate_lightning_orb_periodic : public SpellScriptLoader
{
    public:
        spell_thorim_activate_lightning_orb_periodic() : SpellScriptLoader("spell_thorim_activate_lightning_orb_periodic") { }

        class spell_thorim_activate_lightning_orb_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_thorim_activate_lightning_orb_periodic_AuraScript);

            InstanceScript* instance;

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();

                Unit* caster = GetCaster();
                std::list<Creature*> triggers;

                UpperOrbCheck check;
                Skyfire::CreatureListSearcher<UpperOrbCheck> searcher(caster, triggers, check);
                caster->VisitNearbyObject(100.0f, searcher);

                if (!triggers.empty())
                {
                    Creature* target = Skyfire::Containers::SelectRandomContainerElement(triggers);
                    if (Creature* thorim = GetThorimCreature(instance, BOSS_THORIM))
                        thorim->AI()->SetGUID(target->GetGUID(), DATA_CHARGED_PILLAR);
                }
            }

            bool Load() OVERRIDE
            {
                instance = NULL;
                if (Unit* caster = GetCaster())
                    instance = caster->GetInstanceScript();

                return instance != NULL;
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_thorim_activate_lightning_orb_periodic_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_thorim_activate_lightning_orb_periodic_AuraScript();
        }
};

// 62320 - Aura of Celerity
class spell_thorim_aura_of_celerity : public SpellScriptLoader
{
    public:
        spell_thorim_aura_of_celerity() : SpellScriptLoader("spell_thorim_aura_of_celerity") { }

        class spell_thorim_aura_of_celerity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_thorim_aura_of_celerity_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                return sSpellMgr->GetSpellInfo(SPELL_AURA_OF_CELERITY_VISUAL) != NULL;
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(SPELL_AURA_OF_CELERITY_VISUAL);
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_thorim_aura_of_celerity_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MELEE_SLOW, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_thorim_aura_of_celerity_AuraScript();
        }
};

class achievement_dont_stand_in_the_lightning : public AchievementCriteriaScript
{
    public:
        achievement_dont_stand_in_the_lightning() : AchievementCriteriaScript("achievement_dont_stand_in_the_lightning") { }

        bool OnCheck(Player* /*source*/, Unit* target) OVERRIDE
        {
            return target && target->GetAI() && target->GetAI()->GetData(ACHIEVEMENT_DONT_STAND_IN_THE_LIGHTNING);
        }
};

class achievement_lose_your_illusion : public AchievementCriteriaScript
{
    public:
        achievement_lose_your_illusion() : AchievementCriteriaScript("achievement_lose_your_illusion") { }

        bool OnCheck(Player* /*source*/, Unit* target) OVERRIDE
        {
            return target && target->GetAI() && target->GetAI()->GetData(ACHIEVEMENT_LOSE_YOUR_ILLUSION);
        }
};

class achievement_i_ll_take_you_all_on : public AchievementCriteriaScript
{
    public:
        achievement_i_ll_take_you_all_on() : AchievementCriteriaScript("achievement_i_ll_take_you_all_on"), _check() { }

        bool OnCheck(Player* source, Unit* /*target*/) OVERRIDE
        {
            return _check(source);
        }

    private:
        OutOfArenaCheck _check;
};

class condition_thorim_arena_leap : public ConditionScript
{
    public:
        condition_thorim_arena_leap() : ConditionScript("condition_thorim_arena_leap"), _check(false) { }

        bool OnConditionCheck(Condition* condition, ConditionSourceInfo& sourceInfo) OVERRIDE
        {
            WorldObject* target = sourceInfo.mConditionTargets[condition->ConditionTarget];
            if (!target || !target->GetMap()->IsInstance())
                return false;

            // the bunny next to the balcony wall must not be picked as a landing point
            if (Creature* creature = target->ToCreature())
                if (creature->GetDBTableGUIDLow() == LEAP_EXCLUDED_BUNNY_GUID)
                    return false;

            return _check(target);
        }

    private:
        HeightPositionCheck _check;
};

void AddSC_boss_thorim()
{
    new boss_thorim();
    new npc_thorim_pre_phase();
    new npc_thorim_arena_phase();
    new npc_runic_colossus();
    new npc_ancient_rune_giant();
    new npc_sif();
    new spell_thorim_blizzard_effect();
    new spell_thorim_frostbolt_volley();
    new spell_thorim_charge_orb();
    new spell_thorim_lightning_charge();
    new spell_thorim_stormhammer();
    new spell_thorim_stormhammer_sif();
    new spell_thorim_stormhammer_boomerang();
    new spell_thorim_arena_leap();
    new spell_thorim_runic_smash();
    new spell_thorim_activate_lightning_orb_periodic();
    new spell_thorim_aura_of_celerity();
    new achievement_dont_stand_in_the_lightning();
    new achievement_lose_your_illusion();
    new achievement_i_ll_take_you_all_on();
    new condition_thorim_arena_leap();
}
