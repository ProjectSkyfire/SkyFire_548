/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
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
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "Spell.h"
#include "zulgurub.h"

enum ScriptTexts
{
    SAY_AGGRO       = 0,
    SAY_SPELL       = 1,
    SAY_TRANSFORM_1 = 2,
    SAY_TRANSFORM_2 = 3,
    SAY_KILL        = 4,
    SAY_DEATH       = 5
};

enum Spells
{
    SPELL_SHADOW_BOLT           = 96516,
    SPELL_WAVE_OF_AGONY_AOE     = 98269, // select target
    SPELL_WAVE_OF_AGONY_END     = 96461, // summon stalker
    SPELL_WAVE_OF_AGONY_START   = 96457, // summon wave stalker
    SPELL_WAVE_OF_AGONY_DMG     = 96460,
    SPELL_WAVE_OF_AGONY_VISUAL  = 96532,
    SPELL_WAVE_OF_AGONY_TRAJ    = 96542,
    SPELL_TEARS_OF_BLOOD        = 96438,
    SPELL_TEARS_OF_BLOOD_AURA   = 96935,
    SPELL_WAIL_OF_SORROW        = 96909,
    SPELL_LASH_OF_ANGUISH       = 96423,
    SPELL_RAGE_OF_THE_ANCIENTS  = 96896,
    SPELL_VENGEFUL_SMASH        = 96593,
    SPELL_CAMOUFLAGE            = 96594,
    SPELL_RAVAGE                = 96592,
    SPELL_PRIMAL_BLESSING       = 96559,
    SPELL_CAVE_IN               = 97380,
    SPELL_CAVE_IN_DUMMY         = 96935,
    SPELL_GAPING_WOUND          = 97355,
    SPELL_DARK_SLUMBER          = 96446,

    SPELL_CLEAR_ACHIEVEMENT     = 98840 // unused: the Here, Kitty Kitty achievement is not scripted
};

enum Events
{
    EVENT_SHADOW_BOLT       = 1,
    EVENT_WAVE_OF_AGONY     = 2,
    EVENT_TEARS_OF_BLOOD    = 3,
    EVENT_WAIL_OF_SORROW    = 4,
    EVENT_LASH_OF_ANGUISH   = 5,
    EVENT_CAMOUFLAGE        = 6,
    EVENT_RAVAGE            = 7,
    EVENT_GAPING_WOUND      = 8,
    EVENT_CONTINUE          = 9,
    EVENT_VENGEFUL_SMASH    = 10,
    EVENT_PRIMAL_BLESSING   = 11,
};

enum Adds
{
    NPC_PRIDE_OF_BETHEKK    = 52061,
    NPC_WAVE_OF_AGONY       = 52147,
    NPC_WAVE_OF_AGONY_END   = 52160,
    NPC_CAVE_IN_STALKER     = 52387,
};

Position const pridePos[16] =
{
    {-11517.2f, -1646.82f, 44.4849f, 3.87463f},
    {-11519.2f, -1605.37f, 44.4849f, 3.56047f},
    {-11507.0f, -1644.55f, 44.4849f, 4.7822f},
    {-11519.7f, -1609.0f, 44.4849f, 2.26893f},
    {-11505.6f, -1607.56f, 44.4849f, 2.26893f},
    {-11504.6f, -1603.33f, 44.4849f, 3.87463f},
    {-11508.3f, -1607.37f, 44.4849f, 0.977384f},
    {-11508.7f, -1603.38f, 44.4849f, 5.41052f},
    {-11504.3f, -1645.56f, 44.4849f, 4.66003f},
    {-11523.2f, -1605.96f, 44.4849f, 5.41052f},
    {-11504.1f, -1650.26f, 44.4849f, 2.26893f},
    {-11506.6f, -1651.04f, 44.4849f, 1.6057f},
    {-11523.2f, -1609.31f, 44.4849f, 0.977384f},
    {-11521.8f, -1651.58f, 44.4849f, 0.977384f},
    {-11520.6f, -1646.0f, 44.4849f, 4.95674f},
    {-11518.1f, -1651.48f, 44.4849f, 2.26893f}
};

Position const cavePos[4] =
{
    {-11506.63f, -1605.05f, 44.41f, 0.0f},
    {-11506.26f, -1643.40f, 44.41f, 0.0f},
    {-11520.27f, -1644.65f, 44.41f, 0.0f},
    {-11522.52f, -1610.81f, 44.41f, 0.0f}
};

class boss_kilnara : public CreatureScript
{
    public:
        boss_kilnara() : CreatureScript("boss_kilnara") { }

        struct boss_kilnaraAI : public BossAI
        {
            boss_kilnaraAI(Creature* creature) : BossAI(creature, DATA_KILNARA)
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
            }

            bool bTwoPhase;

            void Reset() OVERRIDE
            {
                _Reset();

                for (uint8 i = 0; i < 16; ++i)
                    if (Creature* pPride = me->SummonCreature(NPC_PRIDE_OF_BETHEKK, pridePos[i]))
                    {
                        pPride->SetReactState(REACT_DEFENSIVE);
                        pPride->CastSpell(pPride, SPELL_DARK_SLUMBER, true);
                    }

                bTwoPhase = false;
                me->SetReactState(REACT_AGGRESSIVE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                bTwoPhase = false;

                events.ScheduleEvent(EVENT_SHADOW_BOLT, 1000);
                events.ScheduleEvent(EVENT_WAVE_OF_AGONY, urand(18000, 25000));
                events.ScheduleEvent(EVENT_LASH_OF_ANGUISH, 10000);
                events.ScheduleEvent(EVENT_WAIL_OF_SORROW, urand(15000, 20000));
                events.ScheduleEvent(EVENT_TEARS_OF_BLOOD, urand(12000, 15000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                BossAI::JustSummoned(summon);
                if (summon->GetEntry() == NPC_WAVE_OF_AGONY_END)
                {
                    Talk(SAY_SPELL);
                    DoCast(summon, SPELL_WAVE_OF_AGONY_START, true);
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (me->GetGUID() == attacker->GetGUID())
                    damage = 0;
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spellInfo) OVERRIDE
            {
                if (spellInfo->HasEffect(SPELL_EFFECT_INTERRUPT_CAST))
                {
                    if (Spell const* spell = me->GetCurrentSpell(CURRENT_GENERIC_SPELL))
                        if (spell->m_spellInfo->Id == SPELL_SHADOW_BOLT)
                            me->InterruptSpell(CURRENT_GENERIC_SPELL);

                    me->RemoveAurasDueToSpell(SPELL_TEARS_OF_BLOOD_AURA);
                    me->RemoveAurasDueToSpell(SPELL_TEARS_OF_BLOOD);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HealthBelowPct(50) && !bTwoPhase)
                {
                    bTwoPhase = true;
                    me->InterruptNonMeleeSpells(false);
                    events.Reset();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    Talk(SAY_TRANSFORM_1);
                    DoCast(me, SPELL_CAVE_IN);
                    for (uint8 i = 0; i < 4; ++i)
                        if (Creature* pCave = me->SummonCreature(NPC_CAVE_IN_STALKER, cavePos[i], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5000))
                            pCave->CastSpell(pCave, SPELL_CAVE_IN_DUMMY);
                    summons.DoZoneInCombat(NPC_PRIDE_OF_BETHEKK);
                    events.ScheduleEvent(EVENT_PRIMAL_BLESSING, 6000);
                    events.ScheduleEvent(EVENT_CONTINUE, 7000);
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOW_BOLT:
                            DoCastVictim(SPELL_SHADOW_BOLT);
                            events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(7000, 8000));
                            break;
                        case EVENT_WAIL_OF_SORROW:
                            DoCast(me, SPELL_WAIL_OF_SORROW);
                            events.ScheduleEvent(EVENT_WAIL_OF_SORROW, urand(15000, 20000));
                            break;
                        case EVENT_LASH_OF_ANGUISH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_LASH_OF_ANGUISH);
                            events.ScheduleEvent(EVENT_LASH_OF_ANGUISH, urand(15000, 20000));
                            break;
                        case EVENT_TEARS_OF_BLOOD:
                            DoCast(me, SPELL_TEARS_OF_BLOOD);
                            events.ScheduleEvent(EVENT_TEARS_OF_BLOOD, urand(30000, 35000));
                            break;
                        case EVENT_WAVE_OF_AGONY:
                            DoCast(me, SPELL_WAVE_OF_AGONY_AOE);
                            events.ScheduleEvent(EVENT_WAVE_OF_AGONY, urand(30000, 35000));
                            break;
                        case EVENT_PRIMAL_BLESSING:
                            Talk(SAY_TRANSFORM_2);
                            DoCast(me, SPELL_PRIMAL_BLESSING, true);
                            DoCast(me, SPELL_RAGE_OF_THE_ANCIENTS, true);
                            break;
                        case EVENT_CONTINUE:
                            me->SetReactState(REACT_AGGRESSIVE);
                            AttackStart(me->GetVictim());
                            events.ScheduleEvent(EVENT_VENGEFUL_SMASH, urand(4000, 10000));
                            events.ScheduleEvent(EVENT_RAVAGE, urand(2000, 6000));
                            break;
                        case EVENT_VENGEFUL_SMASH:
                            DoCast(me, SPELL_VENGEFUL_SMASH);
                            events.ScheduleEvent(EVENT_VENGEFUL_SMASH, urand(10000, 15000));
                            break;
                        case EVENT_RAVAGE:
                            DoCastVictim(SPELL_RAVAGE);
                            events.ScheduleEvent(EVENT_RAVAGE, urand(9000, 12000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<boss_kilnaraAI>(creature);
        }
};

class npc_kilnara_pride_of_bethekk : public CreatureScript
{
    public:
        npc_kilnara_pride_of_bethekk() : CreatureScript("npc_kilnara_pride_of_bethekk") { }

        struct npc_kilnara_pride_of_bethekkAI : public ScriptedAI
        {
            npc_kilnara_pride_of_bethekkAI(Creature* creature) : ScriptedAI(creature)  { }

            uint32 uiGapingWound;

            void Reset() OVERRIDE
            {
                uiGapingWound = urand(5000, 15000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (uiGapingWound <= diff)
                {
                    uiGapingWound = urand(5000, 15000);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                        DoCast(target, SPELL_GAPING_WOUND);
                }
                else
                    uiGapingWound -= diff;

                DoMeleeAttackIfReady();
            }

        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_kilnara_pride_of_bethekkAI>(creature);
        }
};

class spell_kilnara_wave_of_agony_target : public SpellScriptLoader
{
    public:
        spell_kilnara_wave_of_agony_target() : SpellScriptLoader("spell_kilnara_wave_of_agony_target") { }

        class spell_kilnara_wave_of_agony_target_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_kilnara_wave_of_agony_target_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_WAVE_OF_AGONY_END, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_kilnara_wave_of_agony_target_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_kilnara_wave_of_agony_target_SpellScript();
        }
};

class spell_kilnara_wave_of_agony_start : public SpellScriptLoader
{
    public:
        spell_kilnara_wave_of_agony_start() : SpellScriptLoader("spell_kilnara_wave_of_agony_start") { }

        class spell_kilnara_wave_of_agony_start_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_kilnara_wave_of_agony_start_AuraScript);

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                if (!GetUnitOwner())
                    return;

                if (Creature* pStart = GetUnitOwner()->FindNearestCreature(NPC_WAVE_OF_AGONY, 300.0f))
                {
                    pStart->CastSpell(GetUnitOwner(), SPELL_WAVE_OF_AGONY_TRAJ, true);
                    pStart->DespawnOrUnsummon(500);
                }
                if (GetUnitOwner()->ToCreature())
                    GetUnitOwner()->ToCreature()->DespawnOrUnsummon(500);
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_kilnara_wave_of_agony_start_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_kilnara_wave_of_agony_start_AuraScript();
        }
};

void AddSC_boss_kilnara()
{
    new boss_kilnara();
    new npc_kilnara_pride_of_bethekk();
    new spell_kilnara_wave_of_agony_target();
    new spell_kilnara_wave_of_agony_start();
}
