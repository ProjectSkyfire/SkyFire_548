/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
Name: Boss_Blackheart_the_Inciter
%Complete: 75
Comment: Incite Chaos charms the players through summoned dummies; the charmed players fight each other
Category: Auchindoun, Shadow Labyrinth
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "shadow_labyrinth.h"

enum BlackheartTheInciter
{
    SPELL_INCITE_CHAOS      = 33676,
    SPELL_INCITE_CHAOS_B    = 33684,                         //charm cast by the dummy on the player it was summoned by
    SPELL_CHARGE            = 33709,
    SPELL_WAR_STOMP         = 33707,
    SPELL_LAUGH             = 33722,

    // Dummies
    SPELL_THREAT            = 34915,
    SPELL_INCITER_THREAT_TRIGGER = 45339,
    SPELL_DUMMY_NUKE        = 21912,

    // Summon spells, one for each dummy entry
    SPELL_INCITE_CHAOS_SUMMON_1 = 33677,
    SPELL_INCITE_CHAOS_SUMMON_2 = 33680,
    SPELL_INCITE_CHAOS_SUMMON_3 = 33681,
    SPELL_INCITE_CHAOS_SUMMON_4 = 33682,
    SPELL_INCITE_CHAOS_SUMMON_5 = 33683,

    SAY_INTRO               = 0,
    SAY_AGGRO               = 1,
    SAY_SLAY                = 2,
    SAY_HELP                = 3,
    SAY_DEATH               = 4,

    //below, not used
    SAY2_INTRO              = 5,
    SAY2_AGGRO              = 6,
    SAY2_SLAY               = 7,
    SAY2_HELP               = 8,
    SAY2_DEATH              = 9
};

enum Events
{
    EVENT_INCITE_CHAOS          = 1,
    EVENT_CHARGE_ATTACK         = 2,
    EVENT_WAR_STOMP             = 3,

    // Dummies
    EVENT_DUMMY_THREAT_TRIGGER  = 4,
    EVENT_DUMMY_NUKE            = 5
};

uint32 const InciteChaosSummonSpells[] =
{
    SPELL_INCITE_CHAOS_SUMMON_1,
    SPELL_INCITE_CHAOS_SUMMON_2,
    SPELL_INCITE_CHAOS_SUMMON_3,
    SPELL_INCITE_CHAOS_SUMMON_4,
    SPELL_INCITE_CHAOS_SUMMON_5
};

class boss_blackheart_the_inciter : public CreatureScript
{
    public:
        boss_blackheart_the_inciter() : CreatureScript("boss_blackheart_the_inciter") { }

        struct boss_blackheart_the_inciterAI : public BossAI
        {
            boss_blackheart_the_inciterAI(Creature* creature) : BossAI(creature, DATA_BLACKHEART_THE_INCITER) { }

            void Reset() OVERRIDE
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                events.ScheduleEvent(EVENT_INCITE_CHAOS, urand(15000, 25000));
                events.ScheduleEvent(EVENT_CHARGE_ATTACK, 5000);
                events.ScheduleEvent(EVENT_WAR_STOMP, 15000);

                Talk(SAY_AGGRO);
            }

            void KilledUnit(Unit* who) OVERRIDE
            {
                if (who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INCITE_CHAOS:
                            if (GetThreatPlayerCount() > 1)
                            {
                                DoResetThreat();
                                Talk(SAY_HELP);
                                DoCast(me, SPELL_INCITE_CHAOS);
                                DoCast(me, SPELL_LAUGH, true);
                            }
                            events.ScheduleEvent(EVENT_INCITE_CHAOS, urand(50000, 80000));
                            break;
                        case EVENT_CHARGE_ATTACK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_CHARGE);
                            events.ScheduleEvent(EVENT_CHARGE_ATTACK, urand(15000, 25000));
                            break;
                        case EVENT_WAR_STOMP:
                            DoCast(me, SPELL_WAR_STOMP);
                            events.ScheduleEvent(EVENT_WAR_STOMP, urand(18000, 24000));
                            break;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint32 GetThreatPlayerCount() const
            {
                uint32 count = 0;
                ThreatContainer::StorageType const& threatList = me->getThreatManager().getThreatList();
                for (ThreatContainer::StorageType::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
                    if ((*itr)->getTarget()->GetTypeId() == TypeID::TYPEID_PLAYER)
                        ++count;

                return count;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetShadowLabyrinthAI<boss_blackheart_the_inciterAI>(creature);
        }
};

class npc_blackheart_the_inciter_dummy : public CreatureScript
{
    public:
        npc_blackheart_the_inciter_dummy() : CreatureScript("npc_blackheart_the_inciter_dummy") { }

        struct npc_blackheart_the_inciter_dummyAI : public ScriptedAI
        {
            npc_blackheart_the_inciter_dummyAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
            }

            // The five entries have different factions, so the players they charm fight each other
            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                if (Creature* blackheart = me->FindNearestCreature(NPC_BLACKHEART_THE_INCITER, 200.0f))
                    DoCast(blackheart, SPELL_THREAT, true);

                DoCast(summoner, SPELL_INCITE_CHAOS_B, true);

                events.ScheduleEvent(EVENT_DUMMY_THREAT_TRIGGER, 5000);
                events.ScheduleEvent(EVENT_DUMMY_NUKE, urand(5000, 10000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DUMMY_THREAT_TRIGGER:
                            DoCast(me, SPELL_INCITER_THREAT_TRIGGER);
                            break;
                        case EVENT_DUMMY_NUKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_DUMMY_NUKE);
                            events.ScheduleEvent(EVENT_DUMMY_NUKE, urand(2000, 4000));
                            break;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_blackheart_the_inciter_dummyAI(creature);
        }
};

class spell_blackheart_incite_chaos : public SpellScriptLoader
{
    public:
        spell_blackheart_incite_chaos() : SpellScriptLoader("spell_blackheart_incite_chaos") { }

        class spell_blackheart_incite_chaos_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_blackheart_incite_chaos_SpellScript);

            bool Validate(SpellInfo const* /*spell*/) OVERRIDE
            {
                for (uint32 spellId : InciteChaosSummonSpells)
                    if (!sSpellMgr->GetSpellInfo(spellId))
                        return false;
                return true;
            }

            // Each player hit gets the next dummy entry; the fifth one is used for any player after that
            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    target->CastSpell((Unit*)NULL, InciteChaosSummonSpells[std::min<uint32>(_targetCount, 4)], true);
                    ++_targetCount;
                }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_blackheart_incite_chaos_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }

            uint32 _targetCount = 0;
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_blackheart_incite_chaos_SpellScript();
        }
};

class spell_blackheart_incite_chaos_charm : public SpellScriptLoader
{
    public:
        spell_blackheart_incite_chaos_charm() : SpellScriptLoader("spell_blackheart_incite_chaos_charm") { }

        class spell_blackheart_incite_chaos_charm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_blackheart_incite_chaos_charm_AuraScript);

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (Creature* dummy = caster->ToCreature())
                        dummy->DespawnOrUnsummon();
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_blackheart_incite_chaos_charm_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_CHARM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_blackheart_incite_chaos_charm_AuraScript();
        }
};

void AddSC_boss_blackheart_the_inciter()
{
    new boss_blackheart_the_inciter();
    new npc_blackheart_the_inciter_dummy();
    new spell_blackheart_incite_chaos();
    new spell_blackheart_incite_chaos_charm();
}
