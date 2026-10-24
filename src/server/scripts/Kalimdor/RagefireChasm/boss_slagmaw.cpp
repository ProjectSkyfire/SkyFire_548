/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "Containers.h"
#include "SpellScript.h"
#include "ragefire_chasm.h"

enum SlagmawMisc
{
    DISPLAYID_INVIS = 11686,
};

class boss_slagmaw : public CreatureScript
{
    public:
        boss_slagmaw() : CreatureScript("boss_slagmaw") { }

        enum iSpells
        {
            SPELL_LAVA_SPIT         = 119434,
            SPELL_COSMETIC_SUBMERGE = 140483, // hides the boss, plays the rise from the lava on remove
            SPELL_SUBMERGE          = 120384, // plays the dive into the lava on apply
        };

        enum iEvents
        {
            EVENT_LAVA_SPIT = 1,
            EVENT_SUBMERGE,
            EVENT_RELOCATE,
            EVENT_EMERGED,
        };

        enum iMisc
        {
            SPITS_BEFORE_SUBMERGE = 5,
            POINT_RELOCATE        = 1,
        };

        struct boss_slagmawAI : public BossAI
        {
            boss_slagmawAI(Creature* creature) : BossAI(creature, BOSS_SLAGMAW)
            {
                m_cPoint = 1;
                spitCount = 0;
                submerged = false;
            }

            uint8 m_cPoint;
            uint8 spitCount;
            bool submerged;

            void InitializeAI() OVERRIDE
            {
                SetCombatMovement(false);
                BossAI::InitializeAI();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                spitCount = 0;
                SetSubmerged(false);
                me->RemoveAurasDueToSpell(SPELL_COSMETIC_SUBMERGE);

                if (m_cPoint != 1)
                {
                    m_cPoint = 1;
                    me->NearTeleportTo(SlagMawPoints[0].GetPositionX(), SlagMawPoints[0].GetPositionY(), SlagMawPoints[0].GetPositionZ(), SlagMawPoints[0].GetOrientation());
                }

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void SetSubmerged(bool apply)
            {
                submerged = apply;
                if (apply)
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    me->AttackStop();
                    me->InterruptNonMeleeSpells(false);
                    DoCast(me, SPELL_SUBMERGE, true);
                }
                else
                {
                    me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                }
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                spitCount = 0;
                events.ScheduleEvent(EVENT_LAVA_SPIT, urand(3 * IN_MILLISECONDS, 5 * IN_MILLISECONDS));
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE || pointId != POINT_RELOCATE)
                    return;

                if (Unit* victim = me->GetVictim())
                    me->SetFacingToObject(victim);

                // Hide first so dropping the dive aura doesn't show the boss popping up
                me->CastSpell(me, SPELL_COSMETIC_SUBMERGE, true);
                me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                events.ScheduleEvent(EVENT_EMERGED, 3500);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                // Keep the corpse floating at the platform instead of sinking into the lava
                me->GetMotionMaster()->Clear();
                me->StopMoving();
                me->SetDisableGravity(true);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
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
                        case EVENT_LAVA_SPIT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                DoCast(target, SPELL_LAVA_SPIT);
                            else if (Unit* victim = me->GetVictim())
                                DoCast(victim, SPELL_LAVA_SPIT);

                            if (++spitCount >= SPITS_BEFORE_SUBMERGE)
                                events.ScheduleEvent(EVENT_SUBMERGE, 3500);
                            else
                                events.ScheduleEvent(EVENT_LAVA_SPIT, urand(5 * IN_MILLISECONDS, 6 * IN_MILLISECONDS));
                            break;
                        case EVENT_SUBMERGE:
                            spitCount = 0;
                            SetSubmerged(true);
                            events.ScheduleEvent(EVENT_RELOCATE, 2500);
                            break;
                        case EVENT_RELOCATE:
                        {
                            // Swim under the lava to one of the other two spots
                            std::vector<uint8> points = { 1, 2, 3 };
                            points.erase(std::find(points.begin(), points.end(), m_cPoint));
                            m_cPoint = Skyfire::Containers::SelectRandomContainerElement(points);

                            me->GetMotionMaster()->MovePoint(POINT_RELOCATE, SlagMawPoints[m_cPoint - 1]);
                            break;
                        }
                        case EVENT_EMERGED:
                            SetSubmerged(false);
                            if (Unit* victim = me->GetVictim())
                                me->SetFacingToObject(victim);
                            events.ScheduleEvent(EVENT_LAVA_SPIT, 2 * IN_MILLISECONDS);
                            break;
                    }
                }

                if (!submerged)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_slagmawAI(creature);
        }
};

// 140483
class spell_submerge_cosmetic_aura : public SpellScriptLoader
{
    public:
        spell_submerge_cosmetic_aura() : SpellScriptLoader("spell_submerge_cosmetic_aura") { }

        class spell_submerge_cosmetic_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_submerge_cosmetic_aura_AuraScript);

            void HandleOnApply(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetOwner()->ToUnit())
                {
                    target->SetDisplayId(DISPLAYID_INVIS);
                    target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            void HandleOnRemove(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetOwner()->ToUnit())
                {
                    target->SetDisplayId(target->GetNativeDisplayId());
                    target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_submerge_cosmetic_aura_AuraScript::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_submerge_cosmetic_aura_AuraScript::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_submerge_cosmetic_aura_AuraScript();
        }
};

void AddSC_boss_slagmaw()
{
    new boss_slagmaw();
    new spell_submerge_cosmetic_aura();
}
