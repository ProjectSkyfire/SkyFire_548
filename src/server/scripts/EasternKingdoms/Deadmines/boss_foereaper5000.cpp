/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "deadmines.h"

enum ScriptTexts
{
    SAY_AGGRO     = 0,
    SAY_DEATH     = 1,
    SAY_KILL      = 2,
    SAY_SPELL1    = 3,
    SAY_SPELL2    = 4,
    SAY_SPELL3    = 5,
    SAY_SPELL4    = 6,
};

enum Spells
{
    SPELL_REAPER_STRIKE        = 88490,
    SPELL_SAFETY               = 88522,
    SPELL_HARVEST              = 88495,
    SPELL_HARVEST_AURA         = 88497,
    SPELL_HARVEST_DMG          = 88501,
    SPELL_HARVEST_SWEEP        = 88521,
    SPELL_OVERDRIVE            = 88481,
    SPELL_OVERDRIVE_DMG        = 88484,
    SPELL_TARGET_BUNNY         = 71371, // rocket artillery
};

enum Adds
{
    NPC_TARGETING_BUNNY     = 47468,
    NPC_MOLTEN_SLAG         = 49229,
    NPC_PROTOTYPE_REAPER    = 49208, // 87239 91731
};

enum Events
{
    EVENT_OVERDRIVE         = 1,
    EVENT_OVERDRIVE1        = 2,
    EVENT_REAPER_STRIKE     = 3,
    EVENT_HARVEST           = 4,
    EVENT_HARVEST1          = 5,
    EVENT_OVERDRIVE_END     = 6,
};

enum Points
{
    POINT_HARVEST   = 1001,
    POINT_OVERDRIVE = 1002,
};

class boss_foereaper5000 : public CreatureScript
{
    public:
        boss_foereaper5000() : CreatureScript("boss_foereaper5000") { }

        struct boss_foereaper5000AI : public BossAI
        {
            boss_foereaper5000AI(Creature* creature) : BossAI(creature, DATA_FOEREAPER)
            {
                harvestTargetGuid = 0;
                bEnrage = false;
                bOverdrive = false;
            }

            uint64 harvestTargetGuid;
            bool bEnrage;
            bool bOverdrive;

            // Overdrive: run to random spots around the room until the aura ends
            void MoveOverdrive()
            {
                Position pos;
                me->GetRandomPoint(me->GetHomePosition(), 12.0f, pos);
                me->GetMotionMaster()->MovePoint(POINT_OVERDRIVE, pos);
            }

            void EndOverdrive()
            {
                bOverdrive = false;
                me->SetReactState(REACT_AGGRESSIVE);
                if (Unit* victim = SelectTarget(SELECT_TARGET_TOPAGGRO))
                {
                    me->Attack(victim, true);
                    me->GetMotionMaster()->MoveChase(victim);
                }
            }

            void InitializeAI() OVERRIDE
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);

                me->setActive(true);
                me->CastSpell(me, SPELL_OFF_LINE, true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                bEnrage = false;
                bOverdrive = false;
                harvestTargetGuid = 0;
                me->SetReactState(REACT_AGGRESSIVE);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                // Overdrive is channeled for its whole duration; its events must keep running
                bool overdriveActive = bOverdrive || me->HasAura(SPELL_OVERDRIVE);
                if (me->HasUnitState(UNIT_STATE_CASTING) && !overdriveActive)
                    return;

                if (HealthBelowPct(30) && !bEnrage && !overdriveActive)
                {
                    Talk(SAY_SPELL4);
                    bEnrage = true;
                    DoCast(me, SPELL_SAFETY);
                    return;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_OVERDRIVE:
                            DoCast(me, SPELL_OVERDRIVE);
                            Talk(SAY_SPELL3);
                            // 3 sec cast, then 2 sec of spinning in place before it starts running around
                            events.ScheduleEvent(EVENT_OVERDRIVE1, 5000);
                            events.ScheduleEvent(EVENT_OVERDRIVE, urand(25000, 30000));
                            break;
                        case EVENT_OVERDRIVE1:
                            if (!me->HasAura(SPELL_OVERDRIVE) || bOverdrive)
                                break;
                            bOverdrive = true;
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            MoveOverdrive();
                            events.ScheduleEvent(EVENT_OVERDRIVE_END, 500);
                            break;
                        case EVENT_OVERDRIVE_END:
                            if (!bOverdrive)
                                break;
                            if (me->HasAura(SPELL_OVERDRIVE))
                                events.ScheduleEvent(EVENT_OVERDRIVE_END, 500);
                            else
                                EndOverdrive();
                            break;
                        case EVENT_REAPER_STRIKE:
                            if (overdriveActive)
                            {
                                events.ScheduleEvent(EVENT_REAPER_STRIKE, 2000);
                                break;
                            }
                            DoCastVictim(SPELL_REAPER_STRIKE);
                            events.ScheduleEvent(EVENT_REAPER_STRIKE, urand(7000, 10000));
                            break;
                        case EVENT_HARVEST:
                            if (overdriveActive)
                            {
                                events.ScheduleEvent(EVENT_HARVEST, 3000);
                                break;
                            }
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                DoCast(target, SPELL_HARVEST);
                                Talk(SAY_SPELL1);
                            }
                            events.ScheduleEvent(EVENT_HARVEST, urand(30000, 40000));
                            break;
                    }
                }

                if (!overdriveActive)
                    DoMeleeAttackIfReady();
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == POINT_OVERDRIVE)
                {
                    if (bOverdrive && me->HasAura(SPELL_OVERDRIVE))
                        MoveOverdrive();
                    return;
                }

                if (pointId != POINT_HARVEST)
                    return;

                DoCast(me, SPELL_HARVEST_SWEEP, true);
                me->RemoveAurasDueToSpell(SPELL_HARVEST_AURA);

                if (harvestTargetGuid)
                    if (Creature* harvestTarget = ObjectAccessor::GetCreature(*me, harvestTargetGuid))
                        harvestTarget->DespawnOrUnsummon();

                if (Unit* vict = me->GetVictim())
                    me->GetMotionMaster()->MoveChase(vict);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(SAY_AGGRO);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                events.ScheduleEvent(EVENT_REAPER_STRIKE, urand(5000, 8000));
                events.ScheduleEvent(EVENT_OVERDRIVE, urand(10000, 15000));
                events.ScheduleEvent(EVENT_HARVEST, urand(25000, 30000));
                DoZoneInCombat();
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                BossAI::JustSummoned(summon);
                if (summon->GetEntry() == NPC_TARGETING_BUNNY)
                {
                    harvestTargetGuid = summon->GetGUID();
                    Talk(SAY_SPELL2);
                    me->GetMotionMaster()->MovePoint(POINT_HARVEST, summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ());
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Talk(SAY_DEATH);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_foereaper5000AI>(creature);
        }
};

class npc_foereaper_targeting_bunny : public CreatureScript
{
    public:
        npc_foereaper_targeting_bunny() : CreatureScript("npc_foereaper_targeting_bunny") { }

        struct npc_foereaper_targeting_bunnyAI : public ScriptedAI
        {
            npc_foereaper_targeting_bunnyAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            void Reset() OVERRIDE
            {
                DoCast(SPELL_TARGET_BUNNY);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_foereaper_targeting_bunnyAI>(creature);
        }
};

void AddSC_boss_foereaper5000()
{
    new boss_foereaper5000();
    new npc_foereaper_targeting_bunny();
}
