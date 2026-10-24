/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "stormstout_brewery.h"

enum Spells
{
    SPELL_BARREL_EXPLOSION_HOSTILE = 106769,
    SPELL_BARREL_EXPLOSION_PLAYER  = 107016,
    SPELL_ROLLING_BARREL_COSMETIC  = 106647,
    SPELL_BARREL_TOSS              = 106847,
    SPELL_BARREL_RIDE              = 106614,
    SPELL_GOING_BANANAS            = 106651,
    SPELL_GROUND_POUND             = 106807,
};

enum Creatures
{
    NPC_HOZEN_HOLLERER             = 56783,
};

Position const OokOokJumpPos = { -755.68f, 1351.83f, 146.92f, 1.82f };

// A rolling barrel explodes when it runs into a wall bunny, a player on foot or Ook-Ook.
static bool IsBarrelBlocked(Unit* barrel)
{
    if (Unit* bunny = barrel->SelectNearbyTarget(NULL, 3.0f))
        if (bunny->GetEntry() == NPC_BARREL_TOSS_BUNNY)
            return true;

    if (Creature* bunny = GetClosestCreatureWithEntry(barrel, NPC_BARREL_TOSS_BUNNY, 10.0f))
        if (barrel->GetDistance(bunny) < 3.3f)
            return true;

    if (Player* player = barrel->FindNearestPlayer(1.0f))
        if (barrel->GetDistance(player) < 3.0f)
            if (!player->GetVehicle() || !barrel->GetCharmer())
                return true;

    if (Creature* ookOok = GetClosestCreatureWithEntry(barrel, NPC_OOK_OOK, 100.0f, true))
        if (barrel->GetDistance(ookOok) < 3.0f)
            return true;

    return false;
}

class boss_ook_ook : public CreatureScript
{
    public:
        boss_ook_ook() : CreatureScript("boss_ook_ook") { }

        enum Events
        {
            EVENT_GOING_BANANAS = 1,
            EVENT_GROUND_POUND,
            EVENT_GROUND_POUND_CANCEL,
        };

        enum Talks
        {
            TALK_INTRO,
            TALK_AGGRO,
            TALK_SPELL,
            EMOTE_GOING_BANANAS,
            TALK_DEATH
        };

        struct boss_ook_ookAI : public BossAI
        {
            boss_ook_ookAI(Creature* creature) : BossAI(creature, DATA_OOK_OOK), _targetGUID(0) { }

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);

                if (instance->GetBossState(DATA_OOK_OOK) == SPECIAL)
                    StartIntro();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void StartIntro()
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_AGGRESSIVE);

                Talk(TALK_INTRO);
                me->GetMotionMaster()->MoveJump(OokOokJumpPos.GetPositionX(), OokOokJumpPos.GetPositionY(), OokOokJumpPos.GetPositionZ(), 25.0f, 25.0f);
                me->SetHomePosition(OokOokJumpPos);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(TALK_AGGRO);

                events.ScheduleEvent(EVENT_GOING_BANANAS, 2000);
                events.ScheduleEvent(EVENT_GROUND_POUND, urand(8000, 14000));

                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == 0)
                    StartIntro();
            }

            // Going Bananas triggers at 90%, 60% and 30%
            float GetNextBananasPct() const
            {
                if (Aura* bananas = me->GetAura(SPELL_GOING_BANANAS))
                    return 90.0f - bananas->GetStackAmount() * 30.0f;
                return 90.0f;
            }

            void EnterEvadeMode() OVERRIDE
            {
                me->ClearUnitState(UNIT_STATE_CANNOT_TURN);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                BossAI::EnterEvadeMode();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(TALK_DEATH);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                std::list<Creature*> hollerers;
                GetCreatureListWithEntryInGrid(hollerers, me, NPC_HOZEN_HOLLERER, 150.0f);
                for (Creature* hollerer : hollerers)
                    hollerer->DespawnOrUnsummon();
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
                        case EVENT_GOING_BANANAS:
                            if (me->GetHealthPct() < GetNextBananasPct() && (!me->GetAura(SPELL_GOING_BANANAS) || me->GetAura(SPELL_GOING_BANANAS)->GetStackAmount() < 3))
                            {
                                DoCast(me, SPELL_GOING_BANANAS);
                                Talk(TALK_SPELL);
                                Talk(EMOTE_GOING_BANANAS);
                            }
                            events.ScheduleEvent(EVENT_GOING_BANANAS, 2000);
                            break;
                        case EVENT_GROUND_POUND:
                            if (Unit* victim = me->GetVictim())
                            {
                                _targetGUID = victim->GetGUID();
                                me->PrepareChanneledCast(me->GetAngle(victim), SPELL_GROUND_POUND);
                            }
                            Talk(TALK_SPELL);
                            events.ScheduleEvent(EVENT_GROUND_POUND_CANCEL, 5 * IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_GROUND_POUND, urand(10000, 14000));
                            break;
                        case EVENT_GROUND_POUND_CANCEL:
                            me->RemoveChanneledCast(_targetGUID);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint64 _targetGUID;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetStormstoutBreweryAI<boss_ook_ookAI>(creature);
        }
};

class npc_barrel : public CreatureScript
{
    public:
        npc_barrel() : CreatureScript("npc_barrel") { }

        enum Events
        {
            EVENT_MOVE = 1,
            EVENT_ARM,
        };

        struct npc_barrelAI : public ScriptedAI
        {
            npc_barrelAI(Creature* creature) : ScriptedAI(creature), _armed(false) { }

            void Reset() OVERRIDE { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->AddAura(SPELL_ROLLING_BARREL_COSMETIC, me);
                me->SetSpeed(MOVE_WALK, 0.85f);
                me->SetSpeed(MOVE_RUN, 0.85f);
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                RollForward();
                _armed = false;
                _events.ScheduleEvent(EVENT_ARM, 2000);
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
            {
                clicker->CastSpell(me, SPELL_BARREL_RIDE, false);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void RollForward()
            {
                float x, y;
                GetPositionWithDistInOrientation(me, 5.0f, me->GetOrientation(), x, y);
                me->GetMotionMaster()->MovePoint(100, x, y, me->GetPositionZ());
                _events.ScheduleEvent(EVENT_MOVE, 300);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (_armed && me->IsAlive() && IsBarrelBlocked(me))
                {
                    DoCast(me, SPELL_BARREL_EXPLOSION_HOSTILE, true);
                    DoCast(me, SPELL_BARREL_EXPLOSION_PLAYER, true);
                    me->Kill(me);
                    return;
                }

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE:
                            RollForward();
                            break;
                        case EVENT_ARM:
                            _armed = true;
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            bool _armed;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_barrelAI(creature);
        }
};

class npc_hozen_hollerer : public CreatureScript
{
    public:
        npc_hozen_hollerer() : CreatureScript("npc_hozen_hollerer") { }

        enum Events
        {
            EVENT_BARREL_TOSS = 1
        };

        struct npc_hozen_hollererAI : public ScriptedAI
        {
            npc_hozen_hollererAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()) { }

            void InitializeAI() OVERRIDE
            {
                _events.ScheduleEvent(EVENT_BARREL_TOSS, 1000);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void Reset() OVERRIDE { }

            uint32 GetBarrelTimer() const
            {
                if (_instance)
                    if (Creature* ookOok = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_OOK_OOK)))
                        if (Aura* bananas = ookOok->GetAura(SPELL_GOING_BANANAS))
                        {
                            switch (bananas->GetStackAmount())
                            {
                                case 1: return urand(8000, 12000);
                                case 2: return urand(6000, 10000);
                                default: return urand(4000, 7000);
                            }
                        }

                return urand(10000, 14000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    if (eventId == EVENT_BARREL_TOSS)
                    {
                        float x, y;
                        GetPositionWithDistInOrientation(me, frand(5.0f, 10.0f), me->GetOrientation(), x, y);
                        me->CastSpell(x, y, 146.79f, SPELL_BARREL_TOSS, false);
                        _events.ScheduleEvent(EVENT_BARREL_TOSS, GetBarrelTimer());
                    }
                }
            }

        private:
            InstanceScript* _instance;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_hozen_hollererAI(creature);
        }
};

// 106614 - Brew Barrel Ride
class spell_brew_barrel_ride : public SpellScriptLoader
{
    public:
        spell_brew_barrel_ride() : SpellScriptLoader("spell_brew_barrel_ride") { }

        class spell_brew_barrel_ride_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_brew_barrel_ride_AuraScript);

            void HandlePeriodic(AuraEffect const* /*aurEff*/)
            {
                Unit* barrel = GetTarget();
                if (barrel->IsAlive() && IsBarrelBlocked(barrel))
                {
                    barrel->CastSpell(barrel, SPELL_BARREL_EXPLOSION_HOSTILE, true);
                    barrel->CastSpell(barrel, SPELL_BARREL_EXPLOSION_PLAYER, true);
                    barrel->Kill(barrel);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_brew_barrel_ride_AuraScript::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_brew_barrel_ride_AuraScript();
        }
};

// 107351 - Barrel Explosion (hostile)
class spell_barrel_hostile : public SpellScriptLoader
{
    public:
        spell_barrel_hostile() : SpellScriptLoader("spell_barrel_hostile") { }

        class spell_barrel_hostile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_barrel_hostile_SpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if([](WorldObject* obj) { return obj->GetTypeId() == TypeID::TYPEID_PLAYER || obj->GetEntry() == NPC_BARREL_TOSS_BUNNY; });
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_barrel_hostile_SpellScript::SelectTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_barrel_hostile_SpellScript();
        }
};

// 106807 - Ground Pound
class spell_ground_pound : public SpellScriptLoader
{
    public:
        spell_ground_pound() : SpellScriptLoader("spell_ground_pound") { }

        class spell_ground_pound_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ground_pound_AuraScript);

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddUnitState(UNIT_STATE_CANNOT_TURN);
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->ClearUnitState(UNIT_STATE_CANNOT_TURN);
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_ground_pound_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_ground_pound_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_ground_pound_AuraScript();
        }
};

void AddSC_boss_ook_ook()
{
    new boss_ook_ook();
    new npc_barrel();
    new npc_hozen_hollerer();
    new spell_brew_barrel_ride();
    new spell_barrel_hostile();
    new spell_ground_pound();
}
