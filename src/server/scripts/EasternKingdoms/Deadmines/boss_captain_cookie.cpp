/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "Player.h"
#include "deadmines.h"

enum Spells
{
    SPELL_WHO_IS_THAT                       = 89339,
    SPELL_SETIATED                          = 89267,
    SPELL_NAUSEATED                         = 89732,
    SPELL_ROTTEN_AURA                       = 89734,
    SPELL_ROTTEN_AURA_DMG                   = 89734,
    SPELL_CAULDRON                          = 89250,
    SPELL_CAULDRON_VISUAL                   = 89251,
    SPELL_CAULDRON_FIRE                     = 89252,

    SPELL_THROW_FOOD_TARGETING_CORN         = 89268,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_CORN  = 89740,
    SPELL_THROW_FOOD_TARGETING_MELON        = 90561,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_MELON = 90582,
    SPELL_THROW_FOOD_TARGETING_STEAK        = 90562,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_STEAK = 90583,
    SPELL_THROW_FOOD_TARGETING_MYSTERY_MEAT = 90563,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_MM    = 90584,
    SPELL_THROW_FOOD_TARGETING_LOAF         = 90564,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_LOAF  = 90585,
    SPELL_THROW_FOOD_TARGETING_BUN          = 90565,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_BUN   = 90586,

    SPELL_ACHIEVEMENT_AURA                  = 95650,
    SPELL_ACHIEVEMENT_AURA_REMOVE           = 95651,
};

enum Adds
{
    NPC_CAULDRON                            = 47754,
};

enum Events
{
    EVENT_THROW_FOOD                        = 1,
    EVENT_CAULDRON_1                        = 2,
    EVENT_CAULDRON_2                        = 3,
    EVENT_MOVE                              = 4,
};

const uint32 ThrowFoodSpells[12] =
{
    SPELL_THROW_FOOD_TARGETING_CORN,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_CORN,
    SPELL_THROW_FOOD_TARGETING_MELON,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_MELON,
    SPELL_THROW_FOOD_TARGETING_STEAK,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_STEAK,
    SPELL_THROW_FOOD_TARGETING_MYSTERY_MEAT,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_MM,
    SPELL_THROW_FOOD_TARGETING_LOAF,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_LOAF,
    SPELL_THROW_FOOD_TARGETING_BUN,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_BUN
};

#define POINT_MOVE  1

const Position movePos = { -64.07f, -820.27f, 41.17f, 0.04f };

class boss_captain_cookie : public CreatureScript
{
    public:
        boss_captain_cookie() : CreatureScript("boss_captain_cookie") { }

        struct boss_captain_cookieAI : public BossAI
        {
            boss_captain_cookieAI(Creature* creature) : BossAI(creature, DATA_CAPTAIN)
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
            }

            void Reset() OVERRIDE
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->SetReactState(REACT_AGGRESSIVE);
                DoCast(SPELL_WHO_IS_THAT);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!instance || instance->GetBossState(DATA_ADMIRAL) != DONE)
                    return;

                if (me->GetDistance(who) > 5.0f)
                    return;

                BossAI::MoveInLineOfSight(who);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                me->RemoveAura(SPELL_WHO_IS_THAT);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->AttackStop();
                me->SetReactState(REACT_PASSIVE);

                events.ScheduleEvent(EVENT_MOVE, 1000);

                DoZoneInCombat();

                DoCastAOE(SPELL_ACHIEVEMENT_AURA, true);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == POINT_MOVE)
                    events.ScheduleEvent(EVENT_CAULDRON_1, 2000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                if (IsHeroic())
                    me->SummonCreature(NPC_NOTE_FROM_VANESSA, notePos);
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
                        case EVENT_MOVE:
                            me->GetMotionMaster()->MovePoint(POINT_MOVE, movePos);
                            break;
                        case EVENT_CAULDRON_1:
                            me->CastSpell(centershipPos.GetPositionX(), centershipPos.GetPositionY(), centershipPos.GetPositionZ(), SPELL_CAULDRON, true);
                            events.ScheduleEvent(EVENT_CAULDRON_2, 2000);
                            break;
                        case EVENT_CAULDRON_2:
                            if (Creature* pCauldron = me->FindNearestCreature(NPC_CAULDRON, 20.0f))
                                me->GetMotionMaster()->MoveJump(pCauldron->GetPositionX(), pCauldron->GetPositionY(), pCauldron->GetPositionZ(), 5.0f, 10.0f);
                            events.ScheduleEvent(EVENT_THROW_FOOD, 3000);
                            break;
                        case EVENT_THROW_FOOD:
                            DoCastAOE(ThrowFoodSpells[urand(0, 11)]);
                            events.ScheduleEvent(EVENT_THROW_FOOD, 4000);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_captain_cookieAI>(creature);
        }
};

class npc_captain_cookie_cauldron : public CreatureScript
{
    public:
        npc_captain_cookie_cauldron() : CreatureScript("npc_captain_cookie_cauldron") { }

        struct npc_captain_cookie_cauldronAI : public ScriptedAI
        {
            npc_captain_cookie_cauldronAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void Reset() OVERRIDE
            {
                DoCast(me, SPELL_CAULDRON_VISUAL, true);
                DoCast(me, SPELL_CAULDRON_FIRE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_captain_cookie_cauldronAI>(creature);
        }
};

class npc_captain_cookie_good_food : public CreatureScript
{
    public:
        npc_captain_cookie_good_food() : CreatureScript("npc_captain_cookie_good_food") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            InstanceScript* instance = creature->GetInstanceScript();
            if (!instance)
                return true;

            if (instance->GetBossState(DATA_CAPTAIN) != IN_PROGRESS)
                return true;

            player->CastSpell(player, SPELL_SETIATED, true);

            creature->DespawnOrUnsummon();
            return true;
        }

        struct npc_captain_cookie_good_foodAI : public ScriptedAI
        {
            npc_captain_cookie_good_foodAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!instance || instance->GetBossState(DATA_CAPTAIN) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_captain_cookie_good_foodAI>(creature);
        }
};

class npc_captain_cookie_bad_food : public CreatureScript
{
    public:
        npc_captain_cookie_bad_food() : CreatureScript("npc_captain_cookie_bad_food") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            InstanceScript* instance = creature->GetInstanceScript();
            if (!instance)
                return true;

            if (instance->GetBossState(DATA_CAPTAIN) != IN_PROGRESS)
                return true;

            player->CastSpell(player, SPELL_NAUSEATED, true);
            player->CastSpell(player, SPELL_ACHIEVEMENT_AURA_REMOVE, true);

            creature->DespawnOrUnsummon();
            return true;
        }

        struct npc_captain_cookie_bad_foodAI : public ScriptedAI
        {
            npc_captain_cookie_bad_foodAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!instance || instance->GetBossState(DATA_CAPTAIN) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_captain_cookie_bad_foodAI>(creature);
        }
};

// Satiated 89267
class spell_captain_cookie_setiated : public SpellScriptLoader
{
    public:
        spell_captain_cookie_setiated() : SpellScriptLoader("spell_captain_cookie_setiated") { }

        class spell_captain_cookie_setiated_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_captain_cookie_setiated_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->RemoveAuraFromStack(SPELL_NAUSEATED);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_captain_cookie_setiated_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_captain_cookie_setiated_SpellScript();
        }
};

// Nauseated 89732
class spell_captain_cookie_nauseated : public SpellScriptLoader
{
    public:
        spell_captain_cookie_nauseated() : SpellScriptLoader("spell_captain_cookie_nauseated") { }

        class spell_captain_cookie_nauseated_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_captain_cookie_nauseated_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->RemoveAuraFromStack(SPELL_SETIATED);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_captain_cookie_nauseated_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_captain_cookie_nauseated_SpellScript();
        }
};

// Throw Food Targeting 89268, 89740, 90561 - 90565, 90582 - 90586
class spell_captain_cookie_throw_food_targeting : public SpellScriptLoader
{
    public:
        spell_captain_cookie_throw_food_targeting() : SpellScriptLoader("spell_captain_cookie_throw_food_targeting") { }

        class spell_captain_cookie_throw_food_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_captain_cookie_throw_food_targeting_SpellScript);

            void HandleAfterHit()
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                uint32 spellId = uint32(GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                if (!sSpellMgr->GetSpellInfo(spellId))
                    return;

                GetCaster()->CastSpell(GetHitUnit(), spellId, false);
            }

            void Register() OVERRIDE
            {
                AfterHit += SpellHitFn(spell_captain_cookie_throw_food_targeting_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_captain_cookie_throw_food_targeting_SpellScript();
        }
};

void AddSC_boss_captain_cookie()
{
    new boss_captain_cookie();
    new npc_captain_cookie_cauldron();
    new npc_captain_cookie_good_food();
    new npc_captain_cookie_bad_food();
    new spell_captain_cookie_setiated();
    new spell_captain_cookie_nauseated();
    new spell_captain_cookie_throw_food_targeting();
}
