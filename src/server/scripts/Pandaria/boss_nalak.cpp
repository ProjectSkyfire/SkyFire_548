/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

enum NalakSpellData
{
    SPELL_STATIC_SHIELD                     = 136341,
    SPELL_ARC_NOVA                          = 136338,
    SPELL_STORM_CLOUD                       = 136340,
    SPELL_LIGHTNING_TETHER                  = 136339,
    SPELL_LIGHTNING_TETHER_EFF              = 136349,
    SPELL_LIGHTNING_TETHER_EFF_NEAR         = 136353
};

enum NalakTexts
{
    SAY_NALAK_AGGRO                         = 0,
    SAY_NALAK_DEATH,
    SAY_NALAK_INTRO,
    SAY_NALAK_SLAY,
    SAY_NALAK_STORMCLOUD,
    SAY_NALAK_LIGHTNING_TETHER
};

enum NalakEvents
{
    EVENT_ARC_NOVA                          = 1,
    EVENT_STORM_CLOUD,
    EVENT_LIGHTNING_TETHER
};

class boss_nalak : public CreatureScript
{
    public:
        boss_nalak() : CreatureScript("boss_nalak") { }
    
        struct boss_nalakAI : public ScriptedAI
        {
            boss_nalakAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                Talk(SAY_NALAK_INTRO);

                me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 100.0f);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 25.0f);
                events.Reset();
            }
    
            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_NALAK_AGGRO);
                DoCast(me, SPELL_STATIC_SHIELD, true);

                events.ScheduleEvent(EVENT_ARC_NOVA, 39s);
                events.ScheduleEvent(EVENT_LIGHTNING_TETHER, 28s);
                events.ScheduleEvent(EVENT_STORM_CLOUD, randtime(15s, 17s));
            }

            void EnterEvadeMode() OVERRIDE
            {
                ScriptedAI::EnterEvadeMode();

                uint32 corpseDelay = me->GetCorpseDelay();
                uint32 respawnDelay = me->GetRespawnDelay();

                me->SetCorpseDelay(1);
                me->SetRespawnDelay(29);

                me->DespawnOrUnsummon();

                me->SetCorpseDelay(corpseDelay);
                me->SetRespawnDelay(respawnDelay);
            }
    
            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_NALAK_SLAY);
            }
    
            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_NALAK_DEATH);
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
                        case EVENT_ARC_NOVA:
                            DoCast(me, SPELL_ARC_NOVA);
                            events.ScheduleEvent(EVENT_ARC_NOVA, 42s);
                            break;
                        case EVENT_LIGHTNING_TETHER:
                            Talk(SAY_NALAK_LIGHTNING_TETHER);
                            DoCast(me, SPELL_LIGHTNING_TETHER);
                            events.ScheduleEvent(EVENT_LIGHTNING_TETHER, 35s);
                            break;
                        case EVENT_STORM_CLOUD:
                            Talk(SAY_NALAK_STORMCLOUD);
                            DoCast(me, SPELL_STORM_CLOUD);
                            events.ScheduleEvent(EVENT_STORM_CLOUD, 24s);
                            break;
                    }
                }
    
                DoMeleeAttackIfReady();
            }
        };
    
        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_nalakAI(creature);
        }
};

class spell_nalak_lightning_tether : public SpellScript
{
    PrepareSpellScript(spell_nalak_lightning_tether);

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        if (Unit* caster = GetCaster())
        {
            std::list<Player*> pList;
            GetPlayerListInGrid(pList, caster, 200.0f);

            // Remove tanks and affected by select aura if possible
            pList.remove_if(TankSpecTargetSelector());
            pList.remove_if([=](Player* target) { return target && target->HasAura(SPELL_LIGHTNING_TETHER); });

            // if we have enough players without tanks and affected by aura 
            if (pList.size() >= 5)
            {
                targets.clear();

                for (auto&& itr : pList)
                    targets.push_back(itr);

                if (pList.size() > 5)
                    Skyfire::Containers::RandomResizeList(targets, 5);

                return;
            }

            // if we haven`t enough players without tanks and aura affected, then just select anyone
            targets.remove_if([=](WorldObject* target) { return target && target->ToUnit() && target->ToUnit()->HasAura(SPELL_LIGHTNING_TETHER); });

            if (targets.size() > 5)
                Skyfire::Containers::RandomResizeList(targets, 5);
        }
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_nalak_lightning_tether::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_nalak_lightning_tether_eff : public SpellScript
{
    PrepareSpellScript(spell_nalak_lightning_tether_eff);

    void HandleEffectHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            if (Unit* caster = GetCaster())
                target->CastSpell(caster, target->GetExactDist2d(caster) > 20.0f ? SPELL_LIGHTNING_TETHER_EFF : SPELL_LIGHTNING_TETHER_EFF_NEAR, true);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_nalak_lightning_tether_eff::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

class spell_nalak_lightning_teather_hit_eff : public SpellScript
{
    PrepareSpellScript(spell_nalak_lightning_teather_hit_eff);

    void HandleEffectHitTarget(SpellEffIndex /*eff_idx*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetHitUnit())
            {
                float dist = target->GetExactDist2d(caster);

                int32 damage = GetHitDamage();
                int32 reduction = GetHitDamage() + AddPct(damage, dist);
                SetHitDamage(reduction);
            }
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_nalak_lightning_teather_hit_eff::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_nalak_storm_cloud : public SpellScript
{
    PrepareSpellScript(spell_nalak_storm_cloud);

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        if (Unit* caster = GetCaster())
        {
            std::list<Player*> pList;
            GetPlayerListInGrid(pList, caster, 200.0f);

            // Remove tanks, meelee and affected by select aura if possible
            pList.remove_if(TankSpecTargetSelector());
            pList.remove_if(MeeleSpecTargetSelector());
            pList.remove_if([=](Player* target) { return target && target->HasAura(SPELL_STORM_CLOUD); });

            // if we have enough players without tanks, meelee and affected by aura 
            if (pList.size() >= 3)
            {
                targets.clear();

                for (auto&& itr : pList)
                    targets.push_back(itr);

                if (pList.size() > 3)
                    Skyfire::Containers::RandomResizeList(targets, 3);

                return;
            }

            // if we haven`t enough players without tanks, meelee and aura affected, then just select anyone
            targets.remove_if([=](WorldObject* target) { return target && target->ToUnit() && target->ToUnit()->HasAura(SPELL_STORM_CLOUD); });

            if (targets.size() > 3)
                Skyfire::Containers::RandomResizeList(targets, 3);
        }
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_nalak_storm_cloud::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

void AddSC_boss_nalak()
{
    new boss_nalak();
    new GenericSpellScriptLoader<spell_nalak_lightning_tether>("spell_nalak_lightning_tether");
    new GenericSpellScriptLoader<spell_nalak_lightning_tether_eff>("spell_nalak_lightning_tether_eff");
    new GenericSpellScriptLoader<spell_nalak_lightning_teather_hit_eff>("spell_nalak_lightning_teather_hit_eff");
    new GenericSpellScriptLoader<spell_nalak_storm_cloud>("spell_nalak_storm_cloud");
}
