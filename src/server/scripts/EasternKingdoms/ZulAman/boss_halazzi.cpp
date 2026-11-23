/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"
#include "zulaman.h"

enum Says
{
    SAY_AGGRO               = 0,
    SAY_PLAYER_KILL         = 1,
    SAY_ENRAGE              = 2,
    SAY_SPLIT               = 3,
    SAY_COMBINE             = 4,
    SAY_DEATH               = 5
};

enum Spells
{
    SPELL_ENRAGE                = 43139,
    SPELL_WATER_TOTEM           = 97499,
    SPELL_REFRESHING_STREAM     = 97502,
    SPELL_LIGHTNING_TOTEM       = 43302,
    SPELL_LIGHTNING             = 43301,
    SPELL_EARTH_SHOCK           = 43305,
    SPELL_FLAME_SHOCK           = 43303,
    SPELL_SUMMON_LYNX           = 43143,
    SPELL_TRANSFORM_HUMAN       = 43142,
    SPELL_TRANSFORM_HUMAN_AURA  = 43573,
    SPELL_TRANSFORM_LYNX        = 43271,

    // Spirit of the Lynx
    SPELL_FIXATE                = 97486,
    SPELL_SHRED_ARMOR           = 43243,
    SPELL_LYNX_FLURRY           = 43290
};

enum Creatures
{
    NPC_SPIRIT_LYNX             = 24143
};

enum Events
{
    EVENT_ENRAGE            = 1,
    EVENT_EARTH_SHOCK       = 2,
    EVENT_FLAME_SHOCK       = 3,
    EVENT_WATER_TOTEM       = 4,
    EVENT_LIGHTNING_TOTEM   = 5,
    EVENT_FIXATE            = 6,
    EVENT_LYNX_FLURRY       = 7,
    EVENT_SHRED_ARMOR       = 8,
    EVENT_LIGHTNING         = 9
};

enum Phases
{
    PHASE_LYNX              = 1,
    PHASE_HUMAN             = 2
};

enum Actions
{
    ACTION_COMBINE          = 1
};

class boss_halazzi : public CreatureScript
{
    public:
        boss_halazzi() : CreatureScript("boss_halazzi") { }

        struct boss_halazziAI : public BossAI
        {
            boss_halazziAI(Creature* creature) : BossAI(creature, DATA_HALAZZI), phase(0), health(0), isLynx(true) { }

            uint8 phase;
            uint32 health;
            bool isLynx;

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
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                phase = 0;
                isLynx = true;
                me->RemoveAurasDueToSpell(SPELL_TRANSFORM_HUMAN_AURA);
                DoCast(me, SPELL_TRANSFORM_LYNX, true);
                me->GetMap()->SetWorldState(WORLDSTATE_TUNNEL_VISION, 1);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                phase = 0;
                isLynx = true;
                health = me->GetHealth();
                me->HandleEmoteStateCommand(EMOTE_STATE_NONE);
                events.ScheduleEvent(EVENT_WATER_TOTEM, urand(5000, 15000));
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
            {
                if (spell->Id == SPELL_TRANSFORM_LYNX)
                    EnterPhase(PHASE_LYNX);
                else if (spell->Id == SPELL_TRANSFORM_HUMAN_AURA)
                    EnterPhase(PHASE_HUMAN);
            }

            void EnterPhase(Phases nextPhase)
            {
                if (!me->IsInCombat())
                    return;

                switch (nextPhase)
                {
                    case PHASE_LYNX:
                        Talk(SAY_COMBINE);
                        events.CancelEvent(EVENT_EARTH_SHOCK);
                        events.CancelEvent(EVENT_FLAME_SHOCK);
                        events.CancelEvent(EVENT_LIGHTNING_TOTEM);
                        events.ScheduleEvent(EVENT_ENRAGE, urand(3000, 7000));
                        summons.DespawnEntry(NPC_SPIRIT_LYNX);
                        me->RemoveAurasDueToSpell(SPELL_TRANSFORM_HUMAN_AURA);
                        me->SetHealth(health);
                        break;
                    case PHASE_HUMAN:
                        Talk(SAY_SPLIT);
                        events.CancelEvent(EVENT_ENRAGE);
                        DoCast(me, SPELL_SUMMON_LYNX, true);
                        events.ScheduleEvent(EVENT_EARTH_SHOCK, urand(5000, 20000));
                        events.ScheduleEvent(EVENT_FLAME_SHOCK, urand(5000, 20000));
                        events.ScheduleEvent(EVENT_LIGHTNING_TOTEM, urand(3000, 10000));
                        break;
                }
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_PLAYER_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DEATH);
                _JustDied();
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action != ACTION_COMBINE)
                    return;

                isLynx = true;
                ++phase;
                me->SetHealth(health);
                DoCast(me, SPELL_TRANSFORM_LYNX);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                // Two splits, at 60 and at 30 percent; the spirit's death or 20 percent of his own health ends each
                if (me->HealthBelowPct(60) && isLynx && phase == 0)
                {
                    isLynx = false;
                    ++phase;
                    health = me->GetHealth();
                    DoCast(me, SPELL_TRANSFORM_HUMAN);
                    return;
                }
                else if (me->HealthBelowPct(30) && isLynx && phase == 2)
                {
                    isLynx = false;
                    ++phase;
                    health = me->GetHealth();
                    DoCast(me, SPELL_TRANSFORM_HUMAN);
                    return;
                }
                else if (me->HealthBelowPct(20) && !isLynx && (phase == 1 || phase == 3))
                {
                    isLynx = true;
                    ++phase;
                    me->SetHealth(health);
                    DoCast(me, SPELL_TRANSFORM_LYNX);
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ENRAGE:
                            if (roll_chance_i(25))
                                Talk(SAY_ENRAGE);
                            DoCast(me, SPELL_ENRAGE);
                            events.ScheduleEvent(EVENT_ENRAGE, urand(18000, 20000));
                            break;
                        case EVENT_WATER_TOTEM:
                            DoCast(me, SPELL_WATER_TOTEM);
                            events.ScheduleEvent(EVENT_WATER_TOTEM, 30000);
                            break;
                        case EVENT_LIGHTNING_TOTEM:
                            DoCast(me, SPELL_LIGHTNING_TOTEM);
                            events.ScheduleEvent(EVENT_LIGHTNING_TOTEM, urand(20000, 25000));
                            break;
                        case EVENT_EARTH_SHOCK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_EARTH_SHOCK);
                            events.ScheduleEvent(EVENT_EARTH_SHOCK, urand(10000, 20000));
                            break;
                        case EVENT_FLAME_SHOCK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_FLAME_SHOCK);
                            events.ScheduleEvent(EVENT_FLAME_SHOCK, urand(10000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<boss_halazziAI>(creature);
        }
};

class npc_halazzi_lynx : public CreatureScript
{
    public:
        npc_halazzi_lynx() : CreatureScript("npc_halazzi_lynx") { }

        struct npc_halazzi_lynxAI : public ScriptedAI
        {
            npc_halazzi_lynxAI(Creature* creature) : ScriptedAI(creature), despawn(false) { }

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
                Reset();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                despawn = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SHRED_ARMOR, urand(3000, 10000));
                events.ScheduleEvent(EVENT_LYNX_FLURRY, urand(5000, 8000));
                events.ScheduleEvent(EVENT_FIXATE, urand(6000, 9000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HealthBelowPct(20) && !despawn)
                {
                    despawn = true;
                    if (Creature* halazzi = me->FindNearestCreature(NPC_HALAZZI, 300.0f))
                        halazzi->AI()->DoAction(ACTION_COMBINE);
                    return;
                }

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FIXATE:
                            DoResetThreat();
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                DoCastVictim(SPELL_FIXATE, true);
                                AttackStart(target);
                            }
                            events.ScheduleEvent(EVENT_FIXATE, 10000);
                            break;
                        case EVENT_LYNX_FLURRY:
                            DoCast(me, SPELL_LYNX_FLURRY);
                            events.ScheduleEvent(EVENT_LYNX_FLURRY, urand(12000, 20000));
                            break;
                        case EVENT_SHRED_ARMOR:
                            DoCastVictim(SPELL_SHRED_ARMOR);
                            events.ScheduleEvent(EVENT_SHRED_ARMOR, 7000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
            bool despawn;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<npc_halazzi_lynxAI>(creature);
        }
};

class npc_halazzi_water_totem : public CreatureScript
{
    public:
        npc_halazzi_water_totem() : CreatureScript("npc_halazzi_water_totem") { }

        struct npc_halazzi_water_totemAI : public ScriptedAI
        {
            npc_halazzi_water_totemAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                DoCast(me, SPELL_REFRESHING_STREAM, true);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_TUNNEL_VISION, 0);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<npc_halazzi_water_totemAI>(creature);
        }
};

class npc_halazzi_lightning_totem : public CreatureScript
{
    public:
        npc_halazzi_lightning_totem() : CreatureScript("npc_halazzi_lightning_totem") { }

        struct npc_halazzi_lightning_totemAI : public ScriptedAI
        {
            npc_halazzi_lightning_totemAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_LIGHTNING, 1000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_TUNNEL_VISION, 0);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_LIGHTNING)
                    {
                        DoCastVictim(SPELL_LIGHTNING);
                        events.ScheduleEvent(EVENT_LIGHTNING, 3000);
                    }
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<npc_halazzi_lightning_totemAI>(creature);
        }
};

void AddSC_boss_halazzi()
{
    new boss_halazzi();
    new npc_halazzi_lynx();
    new npc_halazzi_water_totem();
    new npc_halazzi_lightning_totem();
}
