/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuras.h"
#include "scarlet_halls.h"

enum Spells
{
    SPELL_BOOK_BURNER                = 113364,
    SPELL_BOOK_BURNER_COSMETIC       = 113366,
    SPELL_BURNING_BOOKS              = 113616, // aura on trigger
    SPELL_GREATER_DRAGONS_BREATH     = 113641,
    SPELL_QUICKENED_MIND             = 113682, // instant 3 casts
    SPELL_PYROBLAST                  = 113690,
    SPELL_FIREBALL_VALLEY            = 113691,
    SPELL_TELEPORT                   = 113626, // to center room
    SPELL_COSMETIC_FLAME             = 126642, // at intro
};

enum Events
{
    EVENT_BOOK_BURNER                = 1,
    EVENT_QUICKENED_MIND             = 2,
    EVENT_DRAGON_BREATH              = 3,
    EVENT_FIREBALL_VALLEY            = 4,
    EVENT_GREATER_DRAGONS_BREATH_END = 5,
    EVENT_PYROBLAST                  = 6,
    EVENT_TELEPORT                   = 7,
    EVENT_DRAGON_BREATH_INTRO        = 8,
};

enum Yells
{
    TALK_INTRO                       = 0,
    TALK_AGGRO                       = 1,
    TALK_DRAGON_BREATH               = 2,
    TALK_BOOK_BURNER_ANN             = 3,
    TALK_DRAGON_BREATH_ANN           = 4,
    TALK_DEATH                       = 5
};

// Point at the same distance from the caster, at the given angle around it
static Position CalculateMovement(Unit* caster, float orientation, float dist)
{
    float x, y;
    GetPositionWithDistInOrientation(caster, dist, orientation, x, y);

    Position pos;
    pos.Relocate(x, y, caster->GetPositionZ());
    pos.SetOrientation(pos.GetAngle(caster));
    return pos;
}

class boss_flameweaver_koegler : public CreatureScript
{
    public:
        boss_flameweaver_koegler() : CreatureScript("boss_flameweaver_koegler") { }

        struct boss_flameweaver_koeglerAI : public BossAI
        {
            boss_flameweaver_koeglerAI(Creature* creature) : BossAI(creature, BOSS_FLAMEWEAVER_KOEGLER) { }

            EventMap nonCombatEvents;
            bool InDragon, Intro;
            uint64 victimGUID;

            void Reset() OVERRIDE
            {
                _Reset();
                events.Reset();
                InDragon = false;
                victimGUID = 0;
                me->setRegeneratingHealth(true);
                me->SetReactState(REACT_AGGRESSIVE);
                nonCombatEvents.ScheduleEvent(EVENT_DRAGON_BREATH_INTRO, urand(1000, 2000));
            }

            void InitializeAI() OVERRIDE
            {
                Intro = false;
                Reset();

                if (instance)
                    if (Creature* crusader = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HOODED_CRUSADER)))
                        crusader->SetVisible(false);
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!Intro && who && who->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    Intro = true;
                    Talk(TALK_INTRO);
                }

                BossAI::MoveInLineOfSight(who);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(TALK_DEATH);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    if (Creature* crusader = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HOODED_CRUSADER)))
                    {
                        crusader->SetVisible(true);
                        if (crusader->AI())
                            crusader->AI()->DoAction(ACTION_KODEX);
                    }
                }
            }

            void EnterEvadeMode() OVERRIDE
            {
                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                summons.DespawnAll();
                _DespawnAtEvade();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(TALK_AGGRO);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                nonCombatEvents.CancelEvent(EVENT_DRAGON_BREATH_INTRO);
                me->RemoveAllAuras();
                GetEvents();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);
                events.Update(diff);

                if (nonCombatEvents.ExecuteEvent() == EVENT_DRAGON_BREATH_INTRO)
                {
                    me->CastSpell(me, SPELL_COSMETIC_FLAME, false);
                    nonCombatEvents.ScheduleEvent(EVENT_DRAGON_BREATH_INTRO, 4000);
                }

                if (!UpdateVictim())
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOOK_BURNER:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            me->InterruptNonMeleeSpells(true);
                            Talk(TALK_BOOK_BURNER_ANN);
                            DoCast(me, SPELL_BOOK_BURNER_COSMETIC);

                            if (Creature* bookCase = SelectBurningCupboard())
                            {
                                me->SetFacingTo(me->GetAngle(bookCase));
                                me->CastSpell(bookCase, SPELL_BOOK_BURNER, false);
                            }

                            events.ScheduleEvent(EVENT_BOOK_BURNER, 15000);
                            break;
                        case EVENT_QUICKENED_MIND:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            me->CastSpell(me, SPELL_QUICKENED_MIND, false);
                            break;
                        case EVENT_FIREBALL_VALLEY:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                me->CastSpell(target, SPELL_FIREBALL_VALLEY, ConsumeQuickenedMind());

                            events.ScheduleEvent(EVENT_FIREBALL_VALLEY, 13500);
                            break;
                        case EVENT_PYROBLAST:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            if (Unit* target = me->GetVictim())
                                me->CastSpell(target, SPELL_PYROBLAST, ConsumeQuickenedMind());

                            events.ScheduleEvent(EVENT_PYROBLAST, urand(4000, 8000));
                            break;
                        case EVENT_GREATER_DRAGONS_BREATH_END:
                            events.Reset();
                            InDragon = false;
                            GetEvents();
                            me->RemoveChanneledCast(victimGUID);
                            break;
                        case EVENT_TELEPORT:
                            Talk(TALK_DRAGON_BREATH);
                            Talk(TALK_DRAGON_BREATH_ANN);
                            me->CastSpell(me, SPELL_TELEPORT, false);
                            me->NearTeleportTo(TeleportPoint.GetPositionX(), TeleportPoint.GetPositionY(), TeleportPoint.GetPositionZ(), TeleportPoint.GetOrientation());
                            events.Reset();
                            if (Unit* victim = me->GetVictim())
                            {
                                victimGUID = victim->GetGUID();
                                me->PrepareChanneledCast(TeleportPoint.GetOrientation());
                            }
                            events.ScheduleEvent(EVENT_DRAGON_BREATH, 1500);
                            events.ScheduleEvent(EVENT_GREATER_DRAGONS_BREATH_END, 13500);
                            break;
                        case EVENT_DRAGON_BREATH:
                            DoCast(me, SPELL_GREATER_DRAGONS_BREATH);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
                EnterEvadeIfOutOfCombatArea(diff, 35.0f);
            }

        private:
            void GetEvents()
            {
                events.ScheduleEvent(EVENT_BOOK_BURNER, 7000);
                events.ScheduleEvent(EVENT_TELEPORT, 30000);
                events.ScheduleEvent(EVENT_QUICKENED_MIND, 10000);
                events.ScheduleEvent(EVENT_FIREBALL_VALLEY, 13500);
                events.ScheduleEvent(EVENT_PYROBLAST, urand(4000, 8000));
            }

            // Quickened Mind makes the next casts instant, one stack per cast
            bool ConsumeQuickenedMind()
            {
                Aura* quickenedMind = me->GetAura(SPELL_QUICKENED_MIND);
                if (!quickenedMind)
                    return false;

                if (quickenedMind->GetStackAmount() > 1)
                    quickenedMind->SetStackAmount(quickenedMind->GetStackAmount() - 1);
                else
                    me->RemoveAurasDueToSpell(SPELL_QUICKENED_MIND);

                return true;
            }

            Creature* SelectBurningCupboard()
            {
                std::list<Creature*> bookCases;
                GetCreatureListWithEntryInGrid(bookCases, me, NPC_BOOK_CASE, 200.0f);

                bookCases.remove_if([](Creature* bookCase) { return !bookCase->HasAura(SPELL_BOOK_BURNER_COSMETIC); });

                if (bookCases.empty())
                    return NULL;

                return Skyfire::Containers::SelectRandomContainerElement(bookCases);
            }

            bool HandleRescheduleEventsIfCastAny(uint32 eventId)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                {
                    events.RescheduleEvent(eventId, urand(200, 450));
                    return true;
                }

                return false;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_flameweaver_koeglerAI(creature);
        }
};

// Book Burner 113364
class spell_koegler_book_burner : public SpellScriptLoader
{
    public:
        spell_koegler_book_burner() : SpellScriptLoader("spell_koegler_book_burner") { }

        class spell_koegler_book_burner_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_koegler_book_burner_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                if (!target || target->GetTypeId() == TypeID::TYPEID_PLAYER)
                    return;

                if (!target->HasAura(SPELL_BURNING_BOOKS))
                    target->CastSpell(target, SPELL_BURNING_BOOKS, true);

                SetHitDamage(0);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_koegler_book_burner_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_koegler_book_burner_SpellScript();
        }
};

// Dragon Breath Targeting 59198
class npc_dragon_breath_targeting : public CreatureScript
{
    public:
        npc_dragon_breath_targeting() : CreatureScript("npc_dragon_breath_targeting") { }

        enum eEvents
        {
            EVENT_MOVE = 1
        };

        struct npc_dragon_breath_targetingAI : public ScriptedAI
        {
            npc_dragon_breath_targetingAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

            InstanceScript* instance;
            EventMap events;

            void Reset() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                me->SetReactState(REACT_PASSIVE);
                events.ScheduleEvent(EVENT_MOVE, 200);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == 0)
                    events.ScheduleEvent(EVENT_MOVE, 200);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() != EVENT_MOVE || !instance)
                    return;

                // Sweep the breath around Koegler, 30 degrees per step
                if (Unit* koegler = ObjectAccessor::GetUnit(*me, instance->GetData64(BOSS_FLAMEWEAVER_KOEGLER)))
                {
                    Position pos = CalculateMovement(koegler, Position::NormalizeOrientation(koegler->GetAngle(me) + M_PI / 6), koegler->GetExactDist2d(me));
                    me->GetMotionMaster()->MovePoint(0, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
                    koegler->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, me->GetGUID());
                    koegler->SetTarget(me->GetGUID());
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_dragon_breath_targetingAI(creature);
        }
};

// Greater Dragon's Breath 113641
class spell_dragon_breath_sc : public SpellScriptLoader
{
    public:
        spell_dragon_breath_sc() : SpellScriptLoader("spell_dragon_breath_sc") { }

        class spell_dragon_breath_sc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dragon_breath_sc_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    caster->SummonCreature(NPC_DRAGON_BREATH_TARGET, DragonBreathMover, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 10 * IN_MILLISECONDS);
            }

            void Register() OVERRIDE
            {
                AfterCast += SpellCastFn(spell_dragon_breath_sc_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_dragon_breath_sc_SpellScript();
        }
};

void AddSC_boss_flameweaver_koegler()
{
    new boss_flameweaver_koegler();
    new npc_dragon_breath_targeting();
    new spell_koegler_book_burner();
    new spell_dragon_breath_sc();
}
