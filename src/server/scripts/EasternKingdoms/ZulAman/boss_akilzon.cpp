/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "zulaman.h"

enum Says
{
    SAY_AGGRO               = 0,
    SAY_PLAYER_KILL         = 1,
    EMOTE_STORM             = 2,
    SAY_SUMMON              = 3,
    SAY_DEATH               = 6
};

enum Spells
{
    SPELL_STATIC_DISRUPTION = 44008,
    SPELL_CALL_LIGHTNING    = 43661,
    SPELL_ELECTRICAL_STORM  = 43648,
    SPELL_ELECTRICAL_SAFE   = 44007,
    SPELL_PLUCKED           = 97318,
    SPELL_EAGLE_SWOOP       = 44732
};

enum Creatures
{
    NPC_SOARING_EAGLE       = 24858,
    NPC_AMANI_KIDNAPPER     = 52638
};

enum Events
{
    EVENT_STATIC_DISRUPTION = 1,
    EVENT_CALL_LIGHTNING    = 2,
    EVENT_ELECTRICAL_STORM  = 3,
    EVENT_SUMMON_EAGLE      = 4,
    EVENT_SUMMON_KIDNAPPER  = 5,
    EVENT_EAGLE_SWOOP       = 6,
    EVENT_GO_BACK           = 7
};

enum Points
{
    POINT_HOME              = 1
};

uint8 const MAX_EAGLES = 8;

class boss_akilzon : public CreatureScript
{
    public:
        boss_akilzon() : CreatureScript("boss_akilzon") { }

        struct boss_akilzonAI : public BossAI
        {
            boss_akilzonAI(Creature* creature) : BossAI(creature, DATA_AKILZON)
            {
                memset(&eagles, 0, sizeof(eagles));
            }

            uint64 eagles[MAX_EAGLES];

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
                memset(&eagles, 0, sizeof(eagles));
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_ELECTRICAL_STORM, 50000);
                events.ScheduleEvent(EVENT_STATIC_DISRUPTION, 7000);
                events.ScheduleEvent(EVENT_SUMMON_KIDNAPPER, 35000);
                events.ScheduleEvent(EVENT_CALL_LIGHTNING, urand(3000, 5000));
                events.ScheduleEvent(EVENT_SUMMON_EAGLE, 10000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_PLAYER_KILL);
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
                        case EVENT_CALL_LIGHTNING:
                            DoCastVictim(SPELL_CALL_LIGHTNING);
                            events.ScheduleEvent(EVENT_CALL_LIGHTNING, urand(8000, 14000));
                            break;
                        case EVENT_STATIC_DISRUPTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_STATIC_DISRUPTION);
                            events.ScheduleEvent(EVENT_STATIC_DISRUPTION, urand(8000, 12000));
                            break;
                        case EVENT_ELECTRICAL_STORM:
                            Talk(EMOTE_STORM);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_ELECTRICAL_STORM);
                            events.ScheduleEvent(EVENT_ELECTRICAL_STORM, 58000);
                            events.ScheduleEvent(EVENT_SUMMON_EAGLE, 13000);
                            events.RescheduleEvent(EVENT_STATIC_DISRUPTION, urand(13000, 17000));
                            events.RescheduleEvent(EVENT_CALL_LIGHTNING, urand(13000, 20000));
                            events.RescheduleEvent(EVENT_SUMMON_KIDNAPPER, urand(40000, 49000));
                            break;
                        case EVENT_SUMMON_EAGLE:
                            Talk(SAY_SUMMON);
                            for (uint8 i = 0; i < MAX_EAGLES; ++i)
                            {
                                if (Unit::GetUnit(*me, eagles[i]))
                                    continue;

                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                    if (Creature* eagle = me->SummonCreature(NPC_SOARING_EAGLE, target->GetPositionX() + irand(-10, 10), target->GetPositionY() + irand(-10, 10), target->GetPositionZ() + urand(10, 15), target->GetOrientation(), TempSummonType::TEMPSUMMON_CORPSE_DESPAWN, 0))
                                        eagles[i] = eagle->GetGUID();
                            }
                            break;
                        case EVENT_SUMMON_KIDNAPPER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_PLUCKED))
                                if (Creature* kidnapper = me->SummonCreature(NPC_AMANI_KIDNAPPER, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ() + 3, target->GetOrientation(), TempSummonType::TEMPSUMMON_DEAD_DESPAWN, 5000))
                                    target->CastSpell(kidnapper, SPELL_PLUCKED, true);
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
            return GetZulAmanAI<boss_akilzonAI>(creature);
        }
};

class npc_akilzon_eagle : public CreatureScript
{
    public:
        npc_akilzon_eagle() : CreatureScript("npc_akilzon_eagle") { }

        struct npc_akilzon_eagleAI : public ScriptedAI
        {
            npc_akilzon_eagleAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset() OVERRIDE
            {
                me->SetCanFly(true);
                me->GetPosition(&homePos);
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_EAGLE_SWOOP, urand(100, 6000));
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == EVENT_CHARGE)
                    events.ScheduleEvent(EVENT_GO_BACK, 3000);
                else if (pointId == POINT_HOME)
                    events.ScheduleEvent(EVENT_EAGLE_SWOOP, urand(2000, 10000));
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
                        case EVENT_GO_BACK:
                            me->GetMotionMaster()->MovePoint(POINT_HOME, homePos);
                            break;
                        case EVENT_EAGLE_SWOOP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_EAGLE_SWOOP);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            Position homePos;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<npc_akilzon_eagleAI>(creature);
        }
};

// The player the storm targets is lifted, held and shielded; everyone else in range takes the damage
class spell_akilzon_electrical_storm : public SpellScriptLoader
{
    public:
        spell_akilzon_electrical_storm() : SpellScriptLoader("spell_akilzon_electrical_storm") { }

        class spell_akilzon_electrical_storm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_akilzon_electrical_storm_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* player = GetTarget()->ToPlayer())
                {
                    player->CastSpell(player, SPELL_ELECTRICAL_SAFE, true);
                    player->SetClientControl(player, 0);
                    player->SetCanFly(true);
                    player->MonsterMoveWithSpeed(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ() + 12.0f, 1.5f);
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* player = GetTarget()->ToPlayer())
                {
                    player->SetClientControl(player, 1);
                    player->SetCanFly(false);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_akilzon_electrical_storm_AuraScript::OnApply, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_akilzon_electrical_storm_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_akilzon_electrical_storm_AuraScript();
        }
};

class ElectricalSafeCheck
{
    public:
        bool operator()(WorldObject* object) const
        {
            Unit* unit = object->ToUnit();
            return unit && unit->HasAura(SPELL_ELECTRICAL_SAFE);
        }
};

class spell_akilzon_electrical_storm_dmg : public SpellScriptLoader
{
    public:
        spell_akilzon_electrical_storm_dmg() : SpellScriptLoader("spell_akilzon_electrical_storm_dmg") { }

        class spell_akilzon_electrical_storm_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_akilzon_electrical_storm_dmg_SpellScript);

            void CheckTarget(std::list<WorldObject*>& targets)
            {
                targets.remove_if(ElectricalSafeCheck());
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_akilzon_electrical_storm_dmg_SpellScript::CheckTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_akilzon_electrical_storm_dmg_SpellScript::CheckTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_akilzon_electrical_storm_dmg_SpellScript();
        }
};

void AddSC_boss_akilzon()
{
    new boss_akilzon();
    new npc_akilzon_eagle();
    new spell_akilzon_electrical_storm();
    new spell_akilzon_electrical_storm_dmg();
}
