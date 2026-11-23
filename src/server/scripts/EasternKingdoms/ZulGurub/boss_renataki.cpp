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
#include "SpellInfo.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "zulgurub.h"

enum Yells
{
    SAY_BLADES       = 0,
    SAY_AGGRO        = 1,
    SAY_DEATH        = 2,
    SAY_KILL         = 3,
    SAY_MOCKING      = 4
};

enum Spells
{
    SPELL_DEADLY_POSION   = 96648,
    SPELL_VANISH          = 96639,
    SPELL_AMBUSH          = 96640,
    SPELL_SHADOWSTEP      = 96683,
    SPELL_THOUSAND_BLADES = 96646,
    SPELL_FRENZY          = 8269
};

enum Events
{
    EVENT_DEADLY_POISON          = 1,
    EVENT_VANISH                 = 2,
    EVENT_AMBUSH                 = 3,
    EVENT_MOCK                   = 4,
    EVENT_MOCK_END               = 5,
    EVENT_THOUSAND_BLADES        = 6,
    EVENT_THOUSAND_BLADES_CHARGE = 7,
    EVENT_THOUSAND_BLADES_END    = 8
};

enum Phases
{
    PHASE_AMBUSH          = 1,
    PHASE_THOUSAND_BLADES = 2
};

class boss_renataki : public CreatureScript
{
    public:
        boss_renataki() : CreatureScript("boss_renataki") { }

        struct boss_renatakiAI : public BossAI
        {
            boss_renatakiAI(Creature* creature) : BossAI(creature, DATA_CACHE_OF_MADNESS_BOSS)
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
            }

            void Reset() OVERRIDE
            {
                _Reset();

                me->SetCanDualWield(true);
                CreatureTemplate const* cinfo = me->GetCreatureTemplate();
                float basedmg = sObjectMgr->GetCreatureBaseStats(me->getLevel(), cinfo->unit_class)->GenerateBaseDamage(cinfo);
                me->SetBaseWeaponDamage(WeaponAttackType::OFF_ATTACK, WeaponDamageRange::MINDAMAGE, basedmg);
                me->SetBaseWeaponDamage(WeaponAttackType::OFF_ATTACK, WeaponDamageRange::MAXDAMAGE, basedmg * 1.5f);
                me->UpdateDamagePhysical(WeaponAttackType::OFF_ATTACK);

                frenzied = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_DEADLY_POISON, 5000);
                events.ScheduleEvent(EVENT_VANISH, 15000);
                events.ScheduleEvent(EVENT_THOUSAND_BLADES, 34000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (!frenzied && me->HealthBelowPctDamaged(30, damage) && !me->HasAura(SPELL_FRENZY))
                {
                    DoCast(me, SPELL_FRENZY, true);
                    frenzied = true;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEADLY_POISON:
                            DoCastVictim(SPELL_DEADLY_POSION);
                            events.ScheduleEvent(EVENT_DEADLY_POISON, 5000);
                            break;
                        case EVENT_VANISH:
                            DoCast(me, SPELL_VANISH);
                            events.CancelEvent(EVENT_DEADLY_POISON);
                            events.ScheduleEvent(EVENT_AMBUSH, 2500);
                            events.ScheduleEvent(EVENT_VANISH, 46500);
                            events.AddPhase(PHASE_AMBUSH);
                            SetCombatMovement(false);
                            break;
                        case EVENT_AMBUSH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            {
                                me->SetTarget(target->GetGUID());
                                me->RemoveAurasDueToSpell(SPELL_VANISH);
                                DoCast(target, SPELL_AMBUSH);
                                DoCast(target, SPELL_SHADOWSTEP);
                                Talk(SAY_MOCKING);
                                events.ScheduleEvent(EVENT_MOCK, 1000);
                            }
                            else
                                events.RemovePhase(PHASE_AMBUSH);
                            break;
                        case EVENT_MOCK:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                            events.ScheduleEvent(EVENT_MOCK_END, 3000);
                            break;
                        case EVENT_MOCK_END:
                            events.RemovePhase(PHASE_AMBUSH);
                            events.ScheduleEvent(EVENT_DEADLY_POISON, 5000);
                            SetCombatMovement(true);
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 100, false))
                            {
                                me->SetTarget(target->GetGUID());
                                me->Attack(target, true);
                                me->GetMotionMaster()->MoveChase(target);
                            }
                            break;
                        case EVENT_THOUSAND_BLADES:
                            Talk(SAY_BLADES);
                            DoCast(me, SPELL_THOUSAND_BLADES);
                            events.AddPhase(PHASE_THOUSAND_BLADES);
                            events.CancelEvent(EVENT_DEADLY_POISON);
                            events.ScheduleEvent(EVENT_THOUSAND_BLADES, 45000);
                            break;
                        case EVENT_THOUSAND_BLADES_CHARGE:
                            if (events.IsInPhase(PHASE_THOUSAND_BLADES))
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                                {
                                    float x, y, z;
                                    target->GetPosition(x, y, z);
                                    me->GetMotionMaster()->MoveCharge(x, y, z);
                                }
                                events.ScheduleEvent(EVENT_THOUSAND_BLADES_CHARGE, 2000);
                            }
                            break;
                        case EVENT_THOUSAND_BLADES_END:
                            me->RemoveAurasDueToSpell(SPELL_THOUSAND_BLADES);
                            events.RemovePhase(PHASE_THOUSAND_BLADES);
                            events.ScheduleEvent(EVENT_DEADLY_POISON, 5000);
                            break;
                        default:
                            break;
                    }
                }

                if (!events.IsInPhase(PHASE_AMBUSH) && !events.IsInPhase(PHASE_THOUSAND_BLADES))
                    DoMeleeAttackIfReady();
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
            {
                if (spell->Id == SPELL_THOUSAND_BLADES)
                {
                    events.ScheduleEvent(EVENT_THOUSAND_BLADES_CHARGE, 1000);
                    events.ScheduleEvent(EVENT_THOUSAND_BLADES_END, 8000);
                }
            }

        protected:
            bool frenzied;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<boss_renatakiAI>(creature);
        }
};

class spell_renataki_thousand_blades : public SpellScriptLoader
{
    public:
        spell_renataki_thousand_blades() : SpellScriptLoader("spell_renataki_thousand_blades") { }

        class spell_renataki_thousand_blades_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_renataki_thousand_blades_SpellScript);

            void HandleHitTarget(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                    {
                        float distance = caster->GetDistance(target);
                        if (distance >= 50)
                            SetHitDamage(0);
                        else
                            SetHitDamage(20000 * (1 - distance / 50));
                    }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_renataki_thousand_blades_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_renataki_thousand_blades_SpellScript();
        }
};

void AddSC_boss_renataki()
{
    new boss_renataki();
    new spell_renataki_thousand_blades();
}
