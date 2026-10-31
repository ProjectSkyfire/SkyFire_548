/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "MoveSplineInit.h"
#include "ObjectAccessor.h"
#include "Pet.h"
#include "Player.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"

#include <list>

enum TsulongEvents
{
    EVENT_NONE,
    EVENT_FLY,
    EVENT_WAYPOINT_FIRST,
    EVENT_WAYPOINT_SECOND,
    EVENT_SWITCH_TO_NIGHT_PHASE,
    EVENT_BERSERK,
    EVENT_SPAWN_SUNBEAM,
    EVENT_SHADOW_BREATH,
    EVENT_DARK_OF_NIGHT,
    EVENT_NIGHTMARES,
    EVENT_SUMMON_TERROR,
    EVENT_SUMMON_SHA,
    EVENT_SUN_BREATH,
    EVENT_EVADE_CHECK,
    EVENT_COSMETIC_BATTLE
};

enum TsulongSpells
{
    SPELL_DREAD_SHADOWS                 = 122767,
    SPELL_DREAD_SHADOWS_DEBUFF          = 122768,
    SPELL_SUNBEAM_DUMMY                 = 122782,
    SPELL_SUNBEAM_PROTECTION            = 122789,
    SPELL_NIGHT_PHASE_EFFECT            = 122841,
    SPELL_SHADOW_BREATH                 = 122752,
    SPELL_NIGHTMARES                    = 122775,
    SPELL_NIGHTMARES_EFF                = 122770,
    SPELL_SHA_ACTIVE                    = 122438,
    SPELL_TSULONG_KILL_CREDIT           = 123901,
    SPELL_WHOS_GOT_TWO_GREEN_THUMBS     = 125827,

    SPELL_GOLD_ACTIVE                   = 122453,
    SPELL_SUN_BREATH                    = 122855,
    SPELL_SUMMON_EMBODIED_TERROR        = 122995,
    SPELL_SUMMON_SHA_PERIODIC           = 122946,
    SPELL_SUMMON_SHA_PERIODIC_EFF       = 122952,
    SPELL_SUMMON_SHA_MISSILE            = 122953,
    SPELL_UNLEASHED_SHA_EXPLOSION       = 130008,
    SPELL_BATHED_IN_LIGHT               = 122858,

    SPELL_THE_DARK_OF_NIGHT             = 123739,
    SPELL_DARK_FIXATE_AURA              = 123740,
    SPELL_DARK_EXPLOSION                = 130013,

    SPELL_LIGHT_OF_DAY_BUFF             = 123716,
    SPELL_LIGHT_OF_DAY_PERIODIC_SUMM    = 123816,
    SPELL_LIGHT_OF_DAY_SUMM             = 123717,
    SPELL_LIGHT_OF_DAY_COSMETIC         = 131988
};

enum TsulongTimers
{
    TIMER_FIRST_WAYPOINT = 5000
};

enum TsulongPhase
{
    PHASE_NONE,
    PHASE_FLY,
    PHASE_DAY,
    PHASE_NIGHT
};

enum TsulongWaypoints
{
    WAYPOINT_FIRST  = 10001,
    WAYPOINT_SECOND = 10002,
    WAYPOINT_DAY    = 10003
};

enum TsulongDisplay
{
    DISPLAY_TSULON_NIGHT = 42532,
    DISPLAY_TSULON_DAY   = 42533,

    FACTION_DAY          = 2104,
    FACTION_NIGHT        = 1965
};

enum TsulongActions
{
    ACTION_SPAWN_SUNBEAM = 10,
    ACTION_REGEN_COMPLETE,
    ACTION_DARK_NIGHT_SWITCH
};

enum TsulongCreatures
{
    SUNBEAM_DUMMY_ENTRY = 62849,
    DARK_OF_NIGHT_ENTRY = 63346
};

enum
{
    SPELL_TERRORIZE                       = 123011,
    SPELL_TERRORIZE_TSULONG               = 123012,
    SPELL_TINY_TERROR                     = 123038,
    SPELL_TINY_TERROR_EFF                 = 123026,
    SPELL_SUMMON_TINY_TERROR              = 123508,
    SPELL_UNSTABLE_BOLT_SACRIFICE         = 122907,

    WORLD_STATE_NO_LFR                    = 12246,
    WORLD_STATE_WHOS_GOT_TWO_GREEN_THUMBS = 12630
};

class boss_tsulong : public CreatureScript
{
    enum Quotes
    {
        EMOTE_SUMMON_TERROR = 1,
        EMOTE_TERRORIZE,
        SAY_INTRO,
        SAY_AGGRO,
        SAY_NIGHTMARES,
        SAY_NIGHT_TO_DAY,
        SAY_DAY_TO_NIGHT,
        SAY_SLAY_DAY,
        SAY_SLAY_NIGHT,
        SAY_DEATH,
        SAY_WIPE,
        EMOTE_SUNBEAM
    };

public:
    boss_tsulong() : CreatureScript("boss_tsulong") { }

    struct boss_tsulongAI : public BossAI
    {
        boss_tsulongAI(Creature* creature) : BossAI(creature, DATA_TSULONG),
            phase(PHASE_NONE), victimGUID(0), firstSpecialEnabled(false),
            secondSpecialEnabled(false), inFly(false), hasBeenDefeated(false),
            landing(false), dayPhaseOri(0.0f) { }

        EventMap m_oocEvents;
        EventMap berserkEvents;
        uint8 phase;
        uint64 victimGUID;
        bool firstSpecialEnabled;
        bool secondSpecialEnabled;
        bool inFly;
        bool hasBeenDefeated;
        bool landing;

        void InitializeAI() OVERRIDE
        {
            landing = false;
            hasBeenDefeated = false;
            me->setActive(true);

            if (instance && instance->GetBossState(DATA_PROTECTORS) < DONE)
                m_oocEvents.ScheduleEvent(EVENT_COSMETIC_BATTLE, 1000);

            Reset();
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell) OVERRIDE
        {
            switch (spell->Id)
            {
                case SPELL_NIGHTMARES:
                    DoCast(target, SPELL_NIGHTMARES_EFF, true);
                    break;
                case SPELL_SUMMON_SHA_PERIODIC_EFF:
                    DoCast(target, SPELL_SUMMON_SHA_MISSILE, true);
                    break;
            }
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == SPELL_TERRORIZE_TSULONG)
                Talk(EMOTE_TERRORIZE);
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_EMBODIED_TERROR && summon->AI())
                summon->AI()->DoZoneInCombat();
        }

        void Reset() OVERRIDE
        {
            m_oocEvents.RescheduleEvent(EVENT_FLY, 2000);

            dayPhaseOri = 0.0f;
            victimGUID = 0;
            _Reset();
            events.Reset();
            berserkEvents.Reset();
            summons.DespawnAll();

            inFly = false;

            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->RemoveAurasDueToSpell(SPELL_DREAD_SHADOWS);
            me->RemoveAurasDueToSpell(SPELL_BERSERK);
            me->setPowerType(POWER_ENERGY);
            me->SetPower(POWER_ENERGY, 0);
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

            if (hasBeenDefeated)
                return;

            _Reset();

            if (instance)
            {
                RemoveBloodlustDebuffs();

                if (instance->GetData(TYPE_PROTECTORS) == DONE &&
                    instance->GetData(TYPE_TSULONG) != DONE && !landing)
                    DoAction(ACTION_START_TSULONG_WAYPOINT);
                else if (instance->GetData(TYPE_PROTECTORS) != DONE)
                {
                    phase = PHASE_DAY;
                    me->SetDisplayId(DISPLAY_TSULON_DAY);
                    me->setFaction(35);
                    me->HandleEmoteStateCommand(35);
                }
                else if (landing)
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveAurasDueToSpell(SPELL_GOLD_ACTIVE);
                    me->RemoveAurasDueToSpell(SPELL_SUMMON_SHA_PERIODIC);
                    me->setFaction(FACTION_NIGHT);
                    me->SetHealth(me->GetMaxHealth());
                    events.SetPhase(PHASE_NONE);
                    phase = PHASE_NONE;
                }
            }

            if (Creature* plant = GetEnchantedPlant(true))
                plant->Respawn();
        }

        void JustReachedHome() OVERRIDE
        {
            _JustReachedHome();

            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->SetBossState(DATA_TSULONG, FAIL);
            }

            summons.DespawnAll();
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            CreatureAI::MoveInLineOfSight(who);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            if (instance && instance->GetData(DATA_TSULONG) == DONE)
                return;

            _EnterCombat();

            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

            Talk(SAY_AGGRO);
            me->SetPower(POWER_ENERGY, 0);
            me->SetHealth(me->GetMaxHealth());
            SetPhase(PHASE_NIGHT);
            berserkEvents.ScheduleEvent(EVENT_BERSERK, 8 * MINUTE * IN_MILLISECONDS + 10 * IN_MILLISECONDS);

            if (Creature* plant = GetEnchantedPlant())
                plant->SetInCombatWith(me);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(phase == PHASE_DAY ? SAY_SLAY_DAY : SAY_SLAY_NIGHT);
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            if (me->IsInEvadeMode())
                damage = 0;
            else if (phase == PHASE_DAY)
            {
                if (me->GetHealth() <= damage)
                {
                    DoCast(me, SPELL_UNLEASHED_SHA_EXPLOSION, true);
                    damage = 0;
                    summons.DespawnAll();
                    EnterEvadeMode();
                }
            }
            else if (phase == PHASE_NIGHT)
            {
                if (me->GetHealthPct() < 1.1f || me->GetHealth() <= damage)
                {
                    damage = 0;
                    HandleDefeat(attacker);
                }
            }
        }

        void HealReceived(Unit* doneBy, uint32& heal) OVERRIDE
        {
            uint32 pct = me->GetMaxHealth() / 100;

            if (heal > pct * 5)
                heal = pct;

            if (instance && instance->GetBossState(DATA_TSULONG) != IN_PROGRESS)
                return;

            if (me->HealthAbovePctHealed(99, heal) && phase == PHASE_DAY)
                HandleDefeat(doneBy);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                RemoveBloodlustDebuffs();
            }

            _JustDied();
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            switch (actionId)
            {
                case ACTION_START_TSULONG_WAYPOINT:
                    SetPhase(PHASE_FLY);
                    Talk(SAY_INTRO);
                    break;
                case ACTION_SPAWN_SUNBEAM:
                    events.ScheduleEvent(EVENT_SPAWN_SUNBEAM, 0, 0, PHASE_NIGHT);
                    break;
                case ACTION_REGEN_COMPLETE:
                    me->SetPower(POWER_ENERGY, 0);
                    SetPhase(phase == PHASE_DAY ? PHASE_NIGHT : PHASE_DAY);
                    break;
            }
        }

        void SetPhase(uint8 newPhase)
        {
            uint8 oldPhase = phase;
            phase = newPhase;
            events.SetPhase(newPhase);

            switch (newPhase)
            {
                case PHASE_FLY:
                    me->RemoveAurasDueToSpell(SPELL_GOLD_ACTIVE);
                    me->RemoveAurasDueToSpell(SPELL_SHA_ACTIVE);
                    events.ScheduleEvent(EVENT_FLY, 5000, 0, PHASE_FLY);
                    break;
                case PHASE_DAY:
                {
                    if (Unit* victim = me->GetVictim())
                        victimGUID = victim->GetGUID();

                    me->SetReactState(REACT_PASSIVE);
                    me->setFaction(FACTION_DAY);
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveIdle();
                    me->AttackStop();
                    me->StopMoving();
                    me->SetTarget(0);
                    me->RemoveAurasDueToSpell(SPELL_SHA_ACTIVE);
                    me->RemoveAurasDueToSpell(SPELL_DREAD_SHADOWS);
                    me->RemoveAurasDueToSpell(SPELL_TERRORIZE_TSULONG);
                    DoCast(me, SPELL_GOLD_ACTIVE, true);

                    events.Reset();
                    me->GetMotionMaster()->MovePoint(WAYPOINT_DAY, -1017.841f, -3049.621f, 12.823f);
                    events.ScheduleEvent(EVENT_SUN_BREATH, 6000, 0, PHASE_DAY);
                    events.ScheduleEvent(EVENT_SUMMON_TERROR, 12000, 0, PHASE_DAY);
                    events.ScheduleEvent(EVENT_EVADE_CHECK, 2000, 0, PHASE_DAY);

                    if (oldPhase == PHASE_NIGHT)
                    {
                        Talk(SAY_NIGHT_TO_DAY);
                        me->SetHealth(me->GetMaxHealth() - me->GetHealth());
                    }
                    break;
                }
                case PHASE_NIGHT:
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveAurasDueToSpell(SPELL_GOLD_ACTIVE);
                    me->RemoveAurasDueToSpell(SPELL_SUMMON_SHA_PERIODIC);
                    DoCast(me, SPELL_SHA_ACTIVE, true);
                    me->setFaction(FACTION_NIGHT);

                    events.Reset();
                    events.ScheduleEvent(EVENT_SWITCH_TO_NIGHT_PHASE, 0, 0, PHASE_NIGHT);
                    events.ScheduleEvent(EVENT_SHADOW_BREATH, 10000, 0, PHASE_NIGHT);
                    events.ScheduleEvent(EVENT_NIGHTMARES, urand(15000, 16000), 0, PHASE_NIGHT);
                    events.ScheduleEvent(EVENT_SPAWN_SUNBEAM, 2000, 0, PHASE_NIGHT);

                    if (IsHeroic())
                    {
                        events.ScheduleEvent(EVENT_DARK_OF_NIGHT, 10000, 0, PHASE_NIGHT);
                        me->RemoveAurasDueToSpell(SPELL_LIGHT_OF_DAY_PERIODIC_SUMM);
                    }

                    if (oldPhase == PHASE_DAY)
                    {
                        Talk(SAY_DAY_TO_NIGHT);
                        me->SetHealth(me->GetMaxHealth() - me->GetHealth());
                    }
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 pointId) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (pointId)
            {
                case WAYPOINT_FIRST:
                    events.ScheduleEvent(EVENT_WAYPOINT_FIRST, 0, 0, PHASE_FLY);
                    break;
                case WAYPOINT_SECOND:
                    events.ScheduleEvent(EVENT_WAYPOINT_SECOND, 0, 0, PHASE_FLY);
                    break;
                case WAYPOINT_DAY:
                    if (Unit* victim = ObjectAccessor::GetUnit(*me, victimGUID))
                        dayPhaseOri = me->GetAngle(victim);

                    me->SetFacingTo(dayPhaseOri);
                    DoCast(me, SPELL_SUMMON_SHA_PERIODIC, true);

                    if (IsHeroic())
                        DoCast(me, SPELL_LIGHT_OF_DAY_PERIODIC_SUMM, false);
                    break;
            }
        }

        void HandleDefeat(Unit* attacker)
        {
            if (hasBeenDefeated)
                return;

            hasBeenDefeated = true;
            EnterEvadeMode();
            Talk(SAY_DEATH);

            me->CombatStop();
            me->SetReactState(REACT_PASSIVE);
            me->setFaction(FACTION_DAY);
            me->RemoveAllAuras();
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveIdle();

            events.Reset();
            HandleRemoveFrightSpawn();
            me->setRegeneratingHealth(false);

            DoCast(me, SPELL_GOLD_ACTIVE, true);
            me->SetDisplayId(DISPLAY_TSULON_DAY);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC |
                UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
            DoCastAOE(SPELL_TSULONG_KILL_CREDIT, true);

            if (attacker)
            {
                if (Player* player = attacker->GetCharmerOrOwnerPlayerOrPlayerItself())
                    me->GetMap()->ToInstanceMap()->PermBindAllPlayers(player);
            }

            // Personal loot and currency are not used by this port.
            if (instance)
            {
                if (GetEnchantedPlant())
                    instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_WHOS_GOT_TWO_GREEN_THUMBS);

                instance->SetBossState(DATA_TSULONG, DONE);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            events.Reset();
            summons.DespawnAll();

            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* player = itr->GetSource())
                    player->CombatStop();
            }

            me->StopMoving();
            me->SetDisableGravity(true);
            me->SetCanFly(true);

            Movement::MoveSplineInit init(me);
            init.MoveTo(TsulongLeavePos.GetPositionX(), TsulongLeavePos.GetPositionY(), TsulongLeavePos.GetPositionZ());
            init.SetSmooth();
            init.Launch();
            me->DespawnOrUnsummon(me->GetSplineDuration());
        }

        void HandleEvadeDuringDay()
        {
            if (me->IsInEvadeMode())
                return;

            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* player = itr->GetSource())
                {
                    if (player->IsAlive() && player->IsWithinDist(me, 40.0f))
                        return;
                }
            }

            summons.DespawnAll();
            EnterEvadeMode();
        }

        void EnterEvadeMode() OVERRIDE
        {
            _EnterEvadeMode();

            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            me->GetMotionMaster()->Clear();

            if (!hasBeenDefeated)
            {
                Talk(SAY_WIPE);
                Reset();
                _DespawnAtEvade();
                me->GetMap()->CreatureRelocation(me, -1017.841f, -3049.621f, 12.823f, 4.72f);

                if (instance)
                    instance->SetBossState(DATA_TSULONG, FAIL);
            }

            if (instance)
                RemoveBloodlustDebuffs();

            HandleRemoveFrightSpawn();

            if (Creature* plant = GetEnchantedPlant())
                plant->AI()->EnterEvadeMode();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!me->IsInEvadeMode() && !me->IsInCombat() && !inFly)
            {
                m_oocEvents.Update(diff);

                while (uint32 eventId = m_oocEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FLY:
                            summons.DespawnAll();
                            m_oocEvents.ScheduleEvent(EVENT_FLY, 1500);
                            break;
                        case EVENT_COSMETIC_BATTLE:
                            if (instance && instance->GetBossState(DATA_PROTECTORS) < DONE)
                            {
                                me->HandleEmoteCommand(urand(0, 1) ? EMOTE_ONESHOT_ATTACK1H : EMOTE_ONESHOT_ATTACK_UNARMED);
                                m_oocEvents.ScheduleEvent(EVENT_COSMETIC_BATTLE, urand(1500, 2000));
                            }
                            break;
                    }
                }
            }

            events.Update(diff);

            if (phase == PHASE_FLY)
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FLY:
                            me->setActive(true);
                            me->UpdateObjectVisibility(true);
                            me->setFaction(FACTION_NIGHT);
                            me->SetReactState(REACT_PASSIVE);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                            me->HandleEmoteStateCommand(EMOTE_STATE_NONE);
                            me->SetDisplayId(DISPLAY_TSULON_NIGHT);
                            me->GetMotionMaster()->MovePoint(WAYPOINT_FIRST, -1018.10f, -2947.431f, 50.12f);
                            inFly = true;
                            break;
                        case EVENT_WAYPOINT_FIRST:
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(WAYPOINT_SECOND, -1017.841f, -3049.621f, 12.823f);
                            break;
                        case EVENT_WAYPOINT_SECOND:
                            landing = true;
                            me->SetHomePosition(-1017.841f, -3049.621f, 12.823f, 4.72f);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                            me->SetCanFly(false);
                            me->SetDisableGravity(false);
                            inFly = false;
                            events.SetPhase(PHASE_NONE);
                            phase = PHASE_NONE;
                            break;
                    }
                }
            }

            berserkEvents.Update(diff);
            while (uint32 eventId = berserkEvents.ExecuteEvent())
            {
                if (eventId == EVENT_BERSERK)
                    DoCast(me, SPELL_BERSERK, true);
                break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (phase == PHASE_NIGHT)
            {
                if (!UpdateVictim())
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SWITCH_TO_NIGHT_PHASE:
                            me->setFaction(FACTION_NIGHT);
                            me->CastSpell(me, SPELL_DREAD_SHADOWS, true);
                            break;
                        case EVENT_SPAWN_SUNBEAM:
                        {
                            if (Creature* sunbeam = GetClosestCreatureWithEntry(me, SUNBEAM_DUMMY_ENTRY, 200.0f))
                                sunbeam->DisappearAndDie();

                            Talk(EMOTE_SUNBEAM, me);
                            Position pos;
                            me->GetRandomNearPosition(pos, 30.0f);
                            me->SummonCreature(SUNBEAM_DUMMY_ENTRY, pos);

                            if (IsHeroic())
                            {
                                std::list<Creature*> creatures;
                                GetCreatureListWithEntryInGrid(creatures, me, NPC_DARK_OF_NIGHT, 200.0f);

                                for (Creature* creature : creatures)
                                    creature->AI()->DoAction(ACTION_DARK_NIGHT_SWITCH);
                            }

                            events.ScheduleEvent(EVENT_SPAWN_SUNBEAM, 40000, 0, PHASE_NIGHT);
                            break;
                        }
                        case EVENT_NIGHTMARES:
                            Talk(SAY_NIGHTMARES);
                            DoCast(me, SPELL_NIGHTMARES, true);
                            events.ScheduleEvent(EVENT_NIGHTMARES, 15000, 0, PHASE_NIGHT);
                            break;
                        case EVENT_SHADOW_BREATH:
                            DoCastVictim(SPELL_SHADOW_BREATH, false);
                            events.ScheduleEvent(EVENT_SHADOW_BREATH, urand(25000, 30000), 0, PHASE_NIGHT);
                            break;
                        case EVENT_DARK_OF_NIGHT:
                            if (Creature* sunbeam = GetClosestCreatureWithEntry(me, SUNBEAM_DUMMY_ENTRY, 200.0f, true))
                                GenerateDarkOfNight(sunbeam);

                            events.ScheduleEvent(EVENT_DARK_OF_NIGHT, urand(25000, 45000), 0, PHASE_NIGHT);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
            else if (phase == PHASE_DAY)
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUMMON_TERROR:
                            Talk(EMOTE_SUMMON_TERROR, me);
                            DoCast(me, SPELL_SUMMON_EMBODIED_TERROR, true);
                            events.ScheduleEvent(EVENT_SUMMON_TERROR, 30000, 0, PHASE_DAY);
                            break;
                        case EVENT_SUN_BREATH:
                            me->SetFacingTo(dayPhaseOri);
                            me->SetOrientation(dayPhaseOri);
                            DoCast(me, SPELL_SUN_BREATH, false);
                            events.ScheduleEvent(EVENT_SUN_BREATH, 29000, 0, PHASE_DAY);
                            break;
                        case EVENT_SUMMON_SHA:
                            break;
                        case EVENT_EVADE_CHECK:
                            HandleEvadeDuringDay();
                            events.ScheduleEvent(EVENT_EVADE_CHECK, 2000, 0, PHASE_DAY);
                            break;
                    }
                }
            }
        }

        Creature* GetEnchantedPlant(bool ignoreDeathState = false)
        {
            if (!instance)
                return nullptr;

            Creature* plant = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_ENCHANTED_PLANT));
            if (plant && !plant->IsAlive() && !ignoreDeathState)
                return nullptr;

            return plant;
        }

    private:
        void RemoveBloodlustDebuffs()
        {
            instance->DoRemoveAurasDueToSpellOnPlayers(95809);
            instance->DoRemoveAurasDueToSpellOnPlayers(80354);
            instance->DoRemoveAurasDueToSpellOnPlayers(57723);
            instance->DoRemoveAurasDueToSpellOnPlayers(57724);
        }

        void GenerateDarkOfNight(Creature* sunbeam)
        {
            uint32 count = Is25ManRaid() ? 9 : 4;

            for (uint32 i = 0; i < count; ++i)
            {
                float x = 0.0f;
                float y = 0.0f;
                float orientation = Position::NormalizeOrientation(frand(0.0f, 2 * M_PI));
                GetPositionWithDistInOrientation(sunbeam, DARK_OF_NIGHT_RANGE, orientation, x, y);
                me->SummonCreature(DARK_OF_NIGHT_ENTRY, x, y, sunbeam->GetPositionZ(), orientation);
            }
        }

        void HandleRemoveFrightSpawn()
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, me, NPC_FRIGHT_SPAWN, 200.0f);

            for (Creature* creature : creatures)
                creature->DespawnOrUnsummon();
        }

        float dayPhaseOri;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_tsulongAI(creature);
    }
};

/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

class npc_sunbeam : public CreatureScript
{
public:
    npc_sunbeam() : CreatureScript("npc_sunbeam") { }

    struct npc_sunbeamAI : public ScriptedAI
    {
        npc_sunbeamAI(Creature* creature) : ScriptedAI(creature)
        {
            creature->SetObjectScale(5.0f);
            creature->SetReactState(REACT_PASSIVE);
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            creature->CastSpell(creature, SPELL_SUNBEAM_DUMMY, true);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (me->GetFloatValue(OBJECT_FIELD_SCALE) <= 1.0f)
            {
                Talk(0);
                me->DespawnOrUnsummon();
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_sunbeamAI(creature);
    }
};

class npc_embodied_terror : public CreatureScript
{
public:
    npc_embodied_terror() : CreatureScript("npc_embodied_terror") { }

    struct npc_embodied_terrorAI : public ScriptedAI
    {
        npc_embodied_terrorAI(Creature* creature) : ScriptedAI(creature), instance(nullptr), died(false) { }

        void Reset() OVERRIDE
        {
            instance = me->GetInstanceScript();
            events.ScheduleEvent(1, urand(3000, 6000));
            died = false;
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (died)
            {
                damage = 0;
                return;
            }

            if (me->GetHealth() <= damage)
            {
                died = true;
                me->DespawnOrUnsummon(5000);
                DoCast(me, SPELL_TINY_TERROR, true);
            }
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == SPELL_TINY_TERROR_EFF)
                for (uint32 i = 0; i < 5; ++i)
                    DoCast(me, SPELL_SUMMON_TINY_TERROR, true);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (!died && instance && instance->GetBossState(DATA_TSULONG) == IN_PROGRESS)
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == 1)
                    {
                        DoCast(me, SPELL_TERRORIZE, true);
                        DoCast(me, SPELL_TERRORIZE_TSULONG, true);
                        events.ScheduleEvent(1, urand(11000, 25000));
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
        bool died;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_embodied_terrorAI(creature);
    }
};

class spell_terrorize_periodic_player : public SpellScriptLoader
{
public:
    spell_terrorize_periodic_player() : SpellScriptLoader("spell_terrorize_periodic_player") { }

    class spell_terrorize_periodic_player_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_terrorize_periodic_player_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                SetHitDamage(target->CountPctFromCurHealth(GetHitDamage()));
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_terrorize_periodic_player_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_terrorize_periodic_player_SpellScript();
    }
};

class npc_fright_spawn : public CreatureScript
{
    enum
    {
        SPELL_FRIGHT = 123036
    };

public:
    npc_fright_spawn() : CreatureScript("npc_fright_spawn") { }

    struct npc_fright_spawnAI : public ScriptedAI
    {
        npc_fright_spawnAI(Creature* creature) : ScriptedAI(creature), frightTimer(0) { }

        void Reset() OVERRIDE
        {
            frightTimer = urand(3000, 5000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (frightTimer <= diff)
            {
                DoCast(me, SPELL_FRIGHT, false);
                frightTimer = urand(8000, 10000);
            }
            else
                frightTimer -= diff;

            DoMeleeAttackIfReady();
        }

    private:
        uint32 frightTimer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_fright_spawnAI(creature);
    }
};

class npc_unstable_sha : public CreatureScript
{
    enum
    {
        SPELL_INSTABILITY_RIDE      = 122928,
        SPELL_INSTABILITY_TRANSFORM = 122930,
        SPELL_INSTABILITY_DAMAGE    = 123697,
        SPELL_INSTABILITY_HEAL      = 130078,
        SPELL_UNSTABLE_BOLT         = 122881
    };

public:
    npc_unstable_sha() : CreatureScript("npc_unstable_sha") { }

    struct npc_unstable_shaAI : public ScriptedAI
    {
        npc_unstable_shaAI(Creature* creature) : ScriptedAI(creature), summonerGUID(0), riding(false), cast(false) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            me->SetSpeed(MOVE_RUN, 0.54f);
            DoZoneInCombat(me, 200.0f);
            SetGazeOn(summoner);
            summonerGUID = summoner->GetGUID();
            events.ScheduleEvent(1, urand(2000, 6000));
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell) OVERRIDE
        {
            if (!cast && target == me && spell->Id == SPELL_INSTABILITY_TRANSFORM)
            {
                if (Unit* summoner = ObjectAccessor::GetUnit(*me, summonerGUID))
                {
                    DoCast(summoner, summoner->getFaction() == FACTION_DAY ? SPELL_INSTABILITY_DAMAGE : SPELL_INSTABILITY_HEAL);
                    cast = true;
                    me->DespawnOrUnsummon(500);
                }
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictimWithGaze() || riding)
                return;

            if (Unit* summoner = ObjectAccessor::GetUnit(*me, summonerGUID))
            {
                if (me->GetExactDist2d(summoner) <= 5.5f)
                {
                    riding = true;
                    DoCast(summoner, SPELL_INSTABILITY_RIDE);
                }
            }

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case 1:
                        if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                        {
                            DoCast(unit, SPELL_UNSTABLE_BOLT);
                            events.ScheduleEvent(1, urand(8000, 16000));
                        }
                        break;
                }
            }
        }

    private:
        EventMap events;
        uint64 summonerGUID;
        bool riding;
        bool cast;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_unstable_shaAI(creature);
    }
};

class spell_dread_shadows_damage : public SpellScriptLoader
{
public:
    spell_dread_shadows_damage() : SpellScriptLoader("spell_dread_shadows_damage") { }

    class spell_dread_shadows_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dread_shadows_damage_SpellScript);

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            targets.clear();
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_damage_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_damage_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dread_shadows_damage_SpellScript();
    }
};

class DreadShadowsTargetCheck
{
public:
    bool operator()(WorldObject* object) const
    {
        return object->ToUnit() && object->ToUnit()->HasAura(SPELL_SUNBEAM_PROTECTION);
    }
};

class spell_dread_shadows_malus : public SpellScriptLoader
{
public:
    spell_dread_shadows_malus() : SpellScriptLoader("spell_dread_shadows_malus") { }

    class spell_dread_shadows_malus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dread_shadows_malus_SpellScript);

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
                targets.remove(caster);

            targets.remove_if(DreadShadowsTargetCheck());
            if (Creature* flower = GetCaster()->FindNearestCreature(NPC_ENCHANTED_PLANT, 200.0f))
                targets.push_back(flower);

            m_targets = targets;
        }

        void CopyTargets(std::list<WorldObject*>& targets)
        {
            targets = m_targets;
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_malus_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_malus_SpellScript::CopyTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
        }

    private:
        std::list<WorldObject*> m_targets;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dread_shadows_malus_SpellScript();
    }
};

class spell_sunbeam : public SpellScript
{
    PrepareSpellScript(spell_sunbeam);

    void CheckTargets(std::list<WorldObject*>& targets)
    {
        targets.clear();
        Map::PlayerList const& players = GetCaster()->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* player = itr->GetSource();
            if (!player)
                continue;

            if (Unit* caster = GetCaster())
            {
                float scale = caster->GetFloatValue(OBJECT_FIELD_SCALE);
                if (player->GetExactDist2d(caster) <= scale)
                    targets.push_back(player);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sunbeam::CheckTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
    }
};

class spell_sunbeam_aura : public AuraScript
{
    PrepareAuraScript(spell_sunbeam_aura);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* target = GetTarget())
        {
            if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (Pet* pet = target->ToPlayer()->GetPet())
                    pet->AddAura(SPELL_SUNBEAM_PROTECTION, pet);

                if (Unit* caster = GetCaster())
                {
                    float scale = caster->GetFloatValue(OBJECT_FIELD_SCALE);
                    float scaleDiff = caster->GetInstanceScript()->instance->Is25ManRaid() ? 0.1f : 0.2f;

                    if (scale > scaleDiff)
                        caster->SetObjectScale(scale - scaleDiff);
                }
            }

            target->RemoveAurasDueToSpell(SPELL_DREAD_SHADOWS_DEBUFF);
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* target = GetTarget())
        {
            if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (Pet* pet = target->ToPlayer()->GetPet())
                    pet->RemoveAurasDueToSpell(SPELL_SUNBEAM_PROTECTION);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_sunbeam_aura::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_sunbeam_aura::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_tsulong_sha_regen : public SpellScriptLoader
{
public:
    spell_tsulong_sha_regen() : SpellScriptLoader("spell_tsulong_sha_regen") { }

    class spell_tsulong_sha_regen_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tsulong_sha_regen_AuraScript);

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (Unit* target = GetUnitOwner())
            {
                if (target->GetPower(POWER_ENERGY) == 99)
                {
                    if (target->ToCreature() && target->ToCreature()->AI())
                        target->ToCreature()->AI()->DoAction(ACTION_REGEN_COMPLETE);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_tsulong_sha_regen_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_tsulong_sha_regen_AuraScript();
    }
};

class spell_tsulong_nightmares : public SpellScriptLoader
{
public:
    spell_tsulong_nightmares() : SpellScriptLoader("spell_tsulong_nightmares") { }

    class spell_tsulong_nightmares_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_tsulong_nightmares_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return true;
        }

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
            {
                targets.sort(Skyfire::ObjectDistanceOrderPred(caster, false));

                DifficultyID difficulty = caster->GetMap()->GetDifficulty();
                uint32 maxTargets = difficulty == DIFFICULTY_25MAN_NORMAL || difficulty == DIFFICULTY_25MAN_HEROIC ? 3 : 1;

                if (targets.size() > maxTargets)
                    Skyfire::Containers::RandomResizeList(targets, maxTargets);
            }
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tsulong_nightmares_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_tsulong_nightmares_SpellScript();
    }
};

class spell_dark_of_night_fixate : public SpellScript
{
    PrepareSpellScript(spell_dark_of_night_fixate);

    void SelectTargets(std::list<WorldObject*>& targets)
    {
        if (targets.size() > 1)
        {
            if (Unit* caster = GetCaster())
                targets.sort(Skyfire::ObjectDistanceOrderPred(caster));

            if (targets.size() > 1)
                targets.resize(1);
        }
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_of_night_fixate::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_tsulong_terrorize : public SpellScriptLoader
{
public:
    spell_tsulong_terrorize() : SpellScriptLoader("spell_tsulong_terrorize") { }

    class spell_tsulong_terrorize_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_tsulong_terrorize_SpellScript);

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
            {
                DifficultyID difficulty = caster->GetMap()->GetDifficulty();
                uint32 maxTargets = difficulty == DIFFICULTY_25MAN_NORMAL || difficulty == DIFFICULTY_25MAN_HEROIC ? 5 : 2;

                if (targets.size() > maxTargets)
                    Skyfire::Containers::RandomResizeList(targets, maxTargets);
            }
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tsulong_terrorize_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_tsulong_terrorize_SpellScript();
    }
};

/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

class npc_dark_of_night : public CreatureScript
{
public:
    npc_dark_of_night() : CreatureScript("npc_dark_of_night") { }

    struct npc_dark_of_nightAI : public ScriptedAI
    {
        npc_dark_of_nightAI(Creature* creature) : ScriptedAI(creature), sunbeamGUID(0), hasDied(false) { }

        void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
        {
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

            hasDied = false;

            DoZoneInCombat(me, 200.0f);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            DoCast(me, SPELL_DARK_FIXATE_AURA, true);
            me->SetSpeed(MOVE_RUN, 0.35f);
            me->PrepareChanneledCast(me->GetOrientation());

            MoveToSunbeam();
            ScheduleExplosion();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            me->StopMoving();
            hasDied = true;
            me->DespawnOrUnsummon();
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            if (actionId == ACTION_DARK_NIGHT_SWITCH)
            {
                me->StopMoving();
                me->m_Events.KillAllEvents(true);

                MoveToSunbeam();
                ScheduleExplosion();
            }
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE { }

    private:
        void MoveToSunbeam()
        {
            if (Creature* sunbeam = GetClosestCreatureWithEntry(me, SUNBEAM_DUMMY_ENTRY, 200.0f, true))
            {
                sunbeamGUID = sunbeam->GetGUID();

                Movement::MoveSplineInit init(me);
                init.MoveTo(sunbeam->GetPositionX(), sunbeam->GetPositionY(), sunbeam->GetPositionZ());
                init.SetUncompressed();
                init.Launch();
            }
        }

        void ScheduleExplosion()
        {
            me->m_Events.Schedule(me->GetSplineDuration(), 5, [this]()
            {
                if (!hasDied)
                    DoCast(me, SPELL_DARK_EXPLOSION, true);

                if (Creature* sunbeam = ObjectAccessor::GetCreature(*me, sunbeamGUID))
                    sunbeam->DespawnOrUnsummon();

                me->DespawnOrUnsummon();
            });
        }

        uint64 sunbeamGUID;
        bool hasDied;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_dark_of_nightAI(creature);
    }
};

class npc_light_of_day : public CreatureScript
{
public:
    npc_light_of_day() : CreatureScript("npc_light_of_day") { }

    struct npc_light_of_dayAI : public ScriptedAI
    {
        npc_light_of_dayAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->CastSpell(me, SPELL_LIGHT_OF_DAY_COSMETIC, false);
        }

        void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
        {
            clicker->CastSpell(clicker, SPELL_LIGHT_OF_DAY_BUFF, false);
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->DespawnOrUnsummon(1500);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE { }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_light_of_dayAI(creature);
    }
};

struct npc_enchanted_plant : public ScriptedAI
{
    npc_enchanted_plant(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        if (me->GetMap()->GetDifficulty() != DIFFICULTY_25MAN_LFR)
            me->GetMap()->SetWorldState(WORLD_STATE_NO_LFR, 1);

        me->GetMap()->SetWorldState(WORLD_STATE_WHOS_GOT_TWO_GREEN_THUMBS, 1);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->GetMap()->SetWorldState(WORLD_STATE_WHOS_GOT_TWO_GREEN_THUMBS, 0);
    }
};

class spell_tsulong_dark_of_night : public SpellScriptLoader
{
public:
    spell_tsulong_dark_of_night() : SpellScriptLoader("spell_tsulong_dark_of_night") { }

    class spell_tsulong_dark_of_night_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_tsulong_dark_of_night_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target) -> bool
            {
                if (Creature* creature = target->ToCreature())
                    return creature->GetEntry() != SUNBEAM_DUMMY_ENTRY;

                return true;
            });
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tsulong_dark_of_night_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_tsulong_dark_of_night_SpellScript();
    }
};

class SunBreathTargetSelector
{
public:
    bool operator()(WorldObject* object) const
    {
        return object && object->ToPlayer();
    }
};

class spell_tsulong_sun_breath : public SpellScriptLoader
{
public:
    spell_tsulong_sun_breath() : SpellScriptLoader("spell_tsulong_sun_breath") { }

    class spell_tsulong_sun_breath_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_tsulong_sun_breath_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->ToPlayer())
                    {
                        PreventHitAura();
                        caster->AddAura(SPELL_BATHED_IN_LIGHT, target);
                    }
                }
            }
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(SunBreathTargetSelector());
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_tsulong_sun_breath_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tsulong_sun_breath_SpellScript::FilterTargets, EFFECT_0, TARGET_DEST_UNK_110);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_tsulong_sun_breath_SpellScript();
    }
};

class SummonShaTargetSelector
{
public:
    bool operator()(WorldObject* object) const
    {
        return object && object->GetEntry() != NPC_UNSTABLE_SHA_TARGET;
    }
};

class spell_tsulong_summon_sha_periodic_effect : public SpellScriptLoader
{
public:
    spell_tsulong_summon_sha_periodic_effect() : SpellScriptLoader("spell_tsulong_summon_sha_periodic_effect") { }

    class spell_tsulong_summon_sha_periodic_effect_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_tsulong_summon_sha_periodic_effect_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(SummonShaTargetSelector());

            if (Unit* caster = GetCaster())
            {
                if (InstanceScript* instance = caster->GetInstanceScript())
                {
                    uint32 maxVictims = instance->instance->Is25ManRaid() ? 8 : 3;

                    if (targets.size() > maxVictims)
                        Skyfire::Containers::RandomResizeList(targets, maxVictims);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tsulong_summon_sha_periodic_effect_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_tsulong_summon_sha_periodic_effect_SpellScript();
    }
};

class spell_terrorize_tsulong : public SpellScript
{
    PrepareSpellScript(spell_terrorize_tsulong);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            if (!target->HasAura(SPELL_GOLD_ACTIVE))
                PreventHitAura();
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_terrorize_tsulong::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

class spell_tsulong_unstable_bolt : public SpellScript
{
    PrepareSpellScript(spell_tsulong_unstable_bolt);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(caster, SPELL_UNSTABLE_BOLT_SACRIFICE, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_tsulong_unstable_bolt::HandleEffectHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_tsulong_unstable_bolt_sacrifice : public SpellScript
{
    PrepareSpellScript(spell_tsulong_unstable_bolt_sacrifice);

    void DealDamage()
    {
        if (Unit* caster = GetCaster())
        {
            if (SpellInfo const* spellInfo = GetSpellInfo())
            {
                int32 bp = spellInfo->Effects[EFFECT_0].BasePoints;
                SetHitDamage((caster->GetMaxHealth() * bp) / 100);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnHit += SpellHitFn(spell_tsulong_unstable_bolt_sacrifice::DealDamage);
    }
};

void AddSC_boss_tsulong()
{
    new boss_tsulong();
    new npc_sunbeam();
    new spell_dread_shadows_damage();
    new spell_dread_shadows_malus();
    RegisterSpellScript(spell_sunbeam);
    RegisterAuraScript(spell_sunbeam_aura);
    new spell_tsulong_sha_regen();
    new spell_terrorize_periodic_player();
    new spell_tsulong_terrorize();
    new spell_tsulong_nightmares();
    RegisterSpellScript(spell_dark_of_night_fixate);
    new npc_embodied_terror();
    new npc_fright_spawn();
    new npc_unstable_sha();
    new npc_dark_of_night();
    new npc_light_of_day();
    RegisterCreatureAI(npc_enchanted_plant);
    new spell_tsulong_dark_of_night();
    new spell_tsulong_sun_breath();
    new spell_tsulong_summon_sha_periodic_effect();
    RegisterSpellScript(spell_terrorize_tsulong);
    RegisterSpellScript(spell_tsulong_unstable_bolt);
    RegisterSpellScript(spell_tsulong_unstable_bolt_sacrifice);
}
