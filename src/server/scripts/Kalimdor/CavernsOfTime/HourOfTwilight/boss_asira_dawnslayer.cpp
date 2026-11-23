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

#include "hour_of_twilight.h"

enum ScriptTexts
{
    SAY_AGGRO                       = 0,
    SAY_DEATH                       = 1,
    SAY_EVENT_1                     = 2,
    SAY_EVENT_2                     = 3,
    SAY_EVENT_3                     = 4,
    SAY_KILL                        = 5,
    SAY_SPELL                       = 6,
};

enum Spells
{
    SPELL_MARK_OF_SILENCE           = 102726,
    SPELL_THROW_KNIFE               = 103597,
    SPELL_SILENCE                   = 103587,
    SPELL_CHOKING_SMOKE_BOMB        = 103558,
    SPELL_CHOKING_SMOKE_BOMB_DMG    = 103790,
    SPELL_BLADE_BARRIER             = 103419,
    SPELL_LESSER_BLADE_BARRIER      = 103562,
};

enum Events
{
    EVENT_MARK_OF_SILENCE           = 1,
    EVENT_CHOKING_SMOKE_BOMB        = 2,
    EVENT_JUMP_TO_HOME_POSITION     = 3
};

class boss_asira_dawnslayer : public CreatureScript
{
    public:
        boss_asira_dawnslayer() : CreatureScript("boss_asira_dawnslayer") { }

        struct boss_asira_dawnslayerAI : public BossAI
        {
            boss_asira_dawnslayerAI(Creature* creature) : BossAI(creature, DATA_ASIRA)
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
                bBarrier = false;
            }

            EventMap events, nonCombatEvents;

            void InitializeAI() OVERRIDE
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(HoTScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                Talk(SAY_EVENT_1);
                SetEquipmentSlots(false, AsiraDaggers, AsiraDaggers, EQUIP_NO_CHANGE);
            }


            void Reset() OVERRIDE
            {
                _Reset();
                bBarrier = false;
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_ASIRA_FALL)
                {
                    if (Creature* LifeWarden = GetClosestCreatureWithEntry(me, NPC_LIFE_WARDEN_1, 50.0f))
                    {
                        me->CastSpell(LifeWarden, 102924, true); // visual backstab
                        LifeWarden->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        LifeWarden->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        LifeWarden->SetStandState(UNIT_STAND_STATE_DEAD);
                    }

                    if (me->IsOnVehicle())
                        me->ExitVehicle();

                    nonCombatEvents.ScheduleEvent(EVENT_JUMP_TO_HOME_POSITION, 2000);
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);

                bBarrier = false;

                events.ScheduleEvent(EVENT_MARK_OF_SILENCE, urand(2000, 3000));
                events.ScheduleEvent(EVENT_CHOKING_SMOKE_BOMB, urand(10000, 12000));

                instance->SetBossState(DATA_ASIRA, IN_PROGRESS);
                DoZoneInCombat();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim && victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void CheckCast()
            {
                std::list<Player*> Silenced;
                GetPlayerListInGrid(Silenced, me, 150.0f);

                if (Silenced.empty())
                    return;

                for (auto itr : Silenced)
                    if (itr->HasAura(SPELL_MARK_OF_SILENCE))
                        if (itr->HasUnitState(UNIT_STATE_CASTING))
                            me->CastSpell(itr, SPELL_THROW_KNIFE, true);

            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);
                nonCombatEvents.Update(diff);

                if (uint32 eventId = nonCombatEvents.ExecuteEvent())
                    if (eventId == EVENT_JUMP_TO_HOME_POSITION)
                    {
                        Talk(SAY_EVENT_2);
                        me->SetHomePosition(AsiraHomePoint);
                        me->GetMotionMaster()->MoveJump(AsiraHomePoint, 30.0f, 20.0f, EVENT_JUMP);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(30) && !bBarrier)
                {
                    bBarrier = true;
                    DoCast(me, SPELL_BLADE_BARRIER);
                    return;
                }

                CheckCast();

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MARK_OF_SILENCE:
                            DoCastAOE(SPELL_MARK_OF_SILENCE);
                            events.ScheduleEvent(EVENT_MARK_OF_SILENCE, urand(21000, 23000));
                            break;
                        case EVENT_CHOKING_SMOKE_BOMB:
                            Talk(SAY_SPELL);
                            DoCast(me, SPELL_CHOKING_SMOKE_BOMB);
                            events.ScheduleEvent(EVENT_CHOKING_SMOKE_BOMB, urand(19000, 21000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool bBarrier;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<boss_asira_dawnslayerAI>(creature);
        }
};

class spell_asira_dawnslayer_blade_barrier : public SpellScriptLoader
{
    public:
        spell_asira_dawnslayer_blade_barrier() : SpellScriptLoader("spell_asira_dawnslayer_blade_barrier") { }

        class spell_asira_dawnslayer_blade_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_asira_dawnslayer_blade_barrier_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if (dmgInfo.GetDamage() < (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                    absorbAmount = dmgInfo.GetDamage() - 1;
                else
                    GetAura()->Remove();
            }

            void HandleAfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster())
                    GetCaster()->CastSpell(GetCaster(), SPELL_LESSER_BLADE_BARRIER, true);
            }

            void Register() OVERRIDE
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_asira_dawnslayer_blade_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_asira_dawnslayer_blade_barrier_AuraScript::Absorb, EFFECT_0);
                 if (m_scriptSpellId == SPELL_BLADE_BARRIER)
                     AfterEffectRemove += AuraEffectRemoveFn(spell_asira_dawnslayer_blade_barrier_AuraScript::HandleAfterRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_asira_dawnslayer_blade_barrier_AuraScript();
        }
};

class spell_asira_dawnslayer_throw_knife : public SpellScriptLoader
{
    public:
        spell_asira_dawnslayer_throw_knife() : SpellScriptLoader("spell_asira_dawnslayer_throw_knife") { }

        class spell_asira_dawnslayer_throw_knife_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_asira_dawnslayer_throw_knife_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (targets.size() <= 1)
                    return;

                Unit* player = GetExplTargetUnit();
                if (!player)
                    return;

                WorldObject* objTarget = NULL;
                for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if ((*itr)->IsInBetween(GetCaster(), player, 1.0f))
                        if (!objTarget || (GetCaster()->GetDistance(objTarget) > GetCaster()->GetDistance((*itr))))
                            objTarget = (*itr);

                if (!objTarget)
                    objTarget = player;

                targets.clear();
                targets.push_back(objTarget);
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (GetHitUnit()->HasAura(SPELL_MARK_OF_SILENCE))
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_SILENCE, true);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_asira_dawnslayer_throw_knife_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_24);
                OnEffectHitTarget += SpellEffectFn(spell_asira_dawnslayer_throw_knife_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_asira_dawnslayer_throw_knife_SpellScript();
        }
};

class spell_asira_dawnslayer_mark_of_silence : public SpellScriptLoader
{
    public:
        spell_asira_dawnslayer_mark_of_silence() : SpellScriptLoader("spell_asira_dawnslayer_mark_of_silence") { }

        class spell_asira_dawnslayer_mark_of_silence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_asira_dawnslayer_mark_of_silence_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (targets.empty())
                    return;

                targets.remove_if(CastersCheck());
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_asira_dawnslayer_mark_of_silence_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }

        private:

            class CastersCheck
            {
                public:
                    CastersCheck() {}

                    bool operator()(WorldObject* unit)
                    {
                        if (unit->GetTypeId() != TypeID::TYPEID_PLAYER)
                            return true;

                        switch (unit->ToPlayer()->getClass())
                        {
                            case CLASS_WARRIOR:
                            case CLASS_DEATH_KNIGHT:
                            case CLASS_ROGUE:
                            case CLASS_HUNTER:
                                return true;
                            case CLASS_DRUID:
                                if (unit->ToPlayer()->GetTalentSpecialization() == SPEC_DRUID_FERAL)
                                    return true;
                                return false;
                            case CLASS_PALADIN:
                                if (unit->ToPlayer()->GetTalentSpecialization() == SPEC_PALADIN_HOLY)
                                    return false;
                                return true;
                            default:
                                return false;
                        }

                        return false;
                    }
            };
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_asira_dawnslayer_mark_of_silence_SpellScript();
        }
};

void AddSC_boss_asira_dawnslayer()
{
    new boss_asira_dawnslayer();
    new spell_asira_dawnslayer_blade_barrier();
    new spell_asira_dawnslayer_throw_knife();
    new spell_asira_dawnslayer_mark_of_silence();
}
