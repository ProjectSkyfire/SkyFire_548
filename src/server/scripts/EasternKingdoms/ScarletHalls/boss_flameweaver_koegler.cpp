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
#include "scarlet_halls.h"
#include "SpellScript.h"
#include "ObjectAccessor.h"
#include "Containers.h"

static Position const TeleportPoint = { 1298.21f, 549.49f, 12.79f, 3.12f };
static Position const DragonBreathMover = { 1288.46f, 549.30f, 12.81f, 3.12f };

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

// Move the breath target around Koegler.
Position CalculateMovement(Creature* me, float m_ori, uint64 casterGUID, float m_dist = 8.0f)
{
    Unit* caster = ObjectAccessor::GetUnit(*me, casterGUID);

    if (!caster)
        return{ 0.0f, 0.0f, 0.0f, 0.0f };

    float x, y;
    GetPositionWithDistInOrientation(caster, m_dist, m_ori, x, y);

    Position pos = { x, y, caster->GetPositionZ(), 0.0f };

    // Face Koegler.
    return{ x, y, caster->GetPositionZ(), pos.GetAngle(caster) };
}

// current range to Koegler
float GetFocusDistance(Creature* me, uint64 casterGUID, uint64 targetGUID)
{
    if (Unit* m_caster = ObjectAccessor::GetUnit(*me, casterGUID))
        if (Unit* target = ObjectAccessor::GetUnit(*me, targetGUID))
            return m_caster->GetExactDist2d(target);

    return 0.0f;
}

class boss_flameweaver_koegler : public CreatureScript
{
    public:
        boss_flameweaver_koegler() : CreatureScript("boss_flameweaver_koegler") { }

        struct boss_flameweaver_koeglerAI : public BossAI
        {
            boss_flameweaver_koeglerAI(Creature* creature) : BossAI(creature, BOSS_FLAMEWEAVER_KOEGLER) { }

            EventMap nonCombatEvents;
            bool Intro;
            uint64 victimGUID;

            void Reset() OVERRIDE
            {
                _Reset();
                events.Reset();
                nonCombatEvents.Reset();
                victimGUID = 0;
                me->setRegeneratingHealth(true);
                me->SetReactState(REACT_AGGRESSIVE);
                std::list<Creature*> bookcases;
                GetCreatureListWithEntryInGrid(bookcases, me, NPC_BOOK_CASE, 100.0f);
                for (Creature* bookcase : bookcases)
                {
                    bookcase->RemoveAurasDueToSpell(SPELL_BOOK_BURNER_COSMETIC);
                    bookcase->RemoveAurasDueToSpell(SPELL_BURNING_BOOKS);
                }
                nonCombatEvents.ScheduleEvent(EVENT_DRAGON_BREATH_INTRO, urand(1000, 2000));

            }

            void InitializeAI() OVERRIDE
            {
                Intro = false;
                Reset();

            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!Intro && who && who->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    Intro = true;
                    Talk(TALK_INTRO);
                }

                ScriptedAI::MoveInLineOfSight(who);
            }

            void JustDied(Unit* killer) OVERRIDE
            {
                _JustDied();
                Talk(TALK_DEATH);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                if (instance)
                {
                    if (Creature* Crusader = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_HOODED_CRUSADER)))
                    {
                        Crusader->SetVisible(true);
                        Crusader->AI()->DoAction(ACTION_KODEX);
                    }
                }
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }
                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(TALK_AGGRO);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }

                nonCombatEvents.CancelEvent(EVENT_DRAGON_BREATH_INTRO);
                me->RemoveAllAuras();
                GetEvents();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);
                events.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_DRAGON_BREATH_INTRO)
                    {
                        me->CastSpell(me, SPELL_COSMETIC_FLAME, false);

                        nonCombatEvents.ScheduleEvent(EVENT_DRAGON_BREATH_INTRO, 4000);
                    }
                    break;
                }

                if (instance && instance->IsWipe(100.0f, me) && me->IsInCombat())
                {
                    EnterEvadeMode();
                    return;
                }
                if (!UpdateVictim() && !victimGUID)
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

                            if (Creature* BookCase = SelectedBurningCupboard(me->GetGUID()))
                            {
                                me->SetFacingTo(me->GetAngle(BookCase));
                                me->CastSpell(BookCase, SPELL_BOOK_BURNER, false);
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
                            {
                                if (Aura* QuckMind = me->GetAura(SPELL_QUICKENED_MIND))
                                {
                                    // Remove the stacks.
                                    if (QuckMind->GetStackAmount() > 1)
                                        QuckMind->SetStackAmount(QuckMind->GetStackAmount() - 1);
                                    else
                                        me->RemoveAurasDueToSpell(SPELL_QUICKENED_MIND);

                                    me->CastSpell(target, SPELL_FIREBALL_VALLEY, true);
                                }
                                else
                                    me->CastSpell(target, SPELL_FIREBALL_VALLEY, false);
                            }

                            events.ScheduleEvent(EVENT_FIREBALL_VALLEY, 13500);
                            break;
                        case EVENT_PYROBLAST:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            if (Unit* target = me->GetVictim())
                            {
                                if (Aura* QuckMind = me->GetAura(SPELL_QUICKENED_MIND))
                                {
                                    // Remove the stacks.
                                    if (QuckMind->GetStackAmount() > 1)
                                        QuckMind->SetStackAmount(QuckMind->GetStackAmount() - 1);
                                    else
                                        me->RemoveAurasDueToSpell(SPELL_QUICKENED_MIND);

                                    me->CastSpell(target, SPELL_PYROBLAST, true);
                                }
                                else
                                    me->CastSpell(target, SPELL_PYROBLAST, false);
                            }

                            events.ScheduleEvent(EVENT_PYROBLAST, urand(4000, 8000));
                            break;
                        case EVENT_GREATER_DRAGONS_BREATH_END:
                            events.Reset();
                            GetEvents();
                            me->RemoveChanneledCast(victimGUID);
                            victimGUID = 0;
                            break;
                        case EVENT_TELEPORT:
                            Talk(TALK_DRAGON_BREATH);
                            Talk(TALK_DRAGON_BREATH_ANN);
                            me->CastSpell(me, SPELL_TELEPORT, false);
                            me->NearTeleportTo(TeleportPoint.GetPositionX(), TeleportPoint.GetPositionY(), TeleportPoint.GetPositionZ(), TeleportPoint.GetOrientation());
                            events.Reset();
                            if (Unit* vict = me->GetVictim())
                            {
                                victimGUID = vict->GetGUID();
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

                Creature* SelectedBurningCupboard(uint64 ownerGUID)
                {
                    Unit* owner = ObjectAccessor::GetUnit(*me, ownerGUID);

                    if (!owner)
                        return NULL;

                    std::list<Creature*> BurningCupboards;
                    GetCreatureListWithEntryInGrid(BurningCupboards, owner, NPC_BOOK_CASE, 200.0f);

                    BurningCupboards.remove_if([](WorldObject* target) -> bool
                    {
                        if (Creature* creature = target->ToCreature())
                            return !creature->HasAura(SPELL_BOOK_BURNER_COSMETIC);

                        return true;
                    });

                    if (!BurningCupboards.empty())
                        return Skyfire::Containers::SelectRandomContainerElement(BurningCupboards);

                    return NULL;
                }

                bool HandleRescheduleEventsIfCastAny(uint32 eventId)
                {
                    if (me->HasUnitState(UNIT_STATE_CASTING))
                    {
                        events.RescheduleEvent(eventId, urand(0.2 * IN_MILLISECONDS, 0.45 * IN_MILLISECONDS));
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

// Book burner 113364
class spell_koegler_book_burner : public SpellScriptLoader
{
    public:
        spell_koegler_book_burner() : SpellScriptLoader("spell_koegler_book_burner") { }

        class spell_koegler_book_burner_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_koegler_book_burner_SpellScript);

        public:

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                Unit* caster = GetCaster();

                if (!target || !caster)
                    return;

                if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
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

        enum eEvents : uint32
        {
            EVENT_NONE,
            EVENT_MOVE
        };

        struct npc_dragon_breath_targetingAI : public ScriptedAI
        {
            npc_dragon_breath_targetingAI(Creature* creature) : ScriptedAI(creature) { }

            InstanceScript* instance;
            EventMap events;

            void Reset() OVERRIDE
            {
                instance = me->GetInstanceScript();
                events.Reset();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                me->SetReactState(REACT_PASSIVE);
                events.ScheduleEvent(EVENT_MOVE, 200);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == 0)
                    events.ScheduleEvent(EVENT_MOVE, 200);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_MOVE)
                    {
                        if (instance)
                        {
                            if (Unit* Koegler = ObjectAccessor::GetUnit(*me, instance->GetData64(BOSS_FLAMEWEAVER_KOEGLER)))
                            {
                                Position destination = CalculateMovement(me, Position::NormalizeOrientation(Koegler->GetAngle(me) + M_PI / 6), Koegler->GetGUID(), GetFocusDistance(me, me->GetGUID(), Koegler->GetGUID()));
                                me->GetMotionMaster()->MovePoint(0, destination);
                                Koegler->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, me->GetGUID());
                                Koegler->SetTarget(me->GetGUID());
                            }
                        }
                    }
                    break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_dragon_breath_targetingAI(creature);
        }
};

// Dragon Breath 113641
class spell_dragon_breath_sc : public SpellScriptLoader
{
    public:
        spell_dragon_breath_sc() : SpellScriptLoader("spell_dragon_breath_sc") { }

        class spell_dragon_breath_sc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dragon_breath_sc_SpellScript);

        public:

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

// The library Crusader starts at the reserved spawn after Koegler dies.
struct npc_scarlet_hooded_crusader : public ScriptedAI
{
    npc_scarlet_hooded_crusader(Creature* creature) : ScriptedAI(creature), started(false) { }

    EventMap events;
    bool started;

    void InitializeAI() OVERRIDE
    {
        if (me->GetDBTableGUIDLow() == 8300128)
            if (InstanceScript* instance = me->GetInstanceScript())
                if (instance->GetBossState(BOSS_FLAMEWEAVER_KOEGLER) == DONE)
                    DoAction(ACTION_KODEX);
    }

    void DoAction(int32 action) OVERRIDE
    {
        if (action != ACTION_KODEX || started)
            return;
        started = true;
        me->SetVisible(true);
        events.ScheduleEvent(1, 100);
    }

    void MovementInform(uint32 type, uint32 point) OVERRIDE
    {
        if (type != POINT_MOTION_TYPE)
            return;
        if (point == 0)
        {
            Talk(0);
            events.ScheduleEvent(2, 6000);
        }
        else if (point == 1)
            Talk(1);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        events.Update(diff);
        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == 1)
                me->GetMotionMaster()->MovePoint(0, 1276.34f, 549.60f, 12.82f);
            else if (eventId == 2)
                me->GetMotionMaster()->MovePoint(1, 1295.33f, 549.61f, 12.79f);
        }
    }
};

void AddSC_boss_flameweaver_koegler()
{
    new boss_flameweaver_koegler();
    new npc_dragon_breath_targeting();
    RegisterCreatureAI(npc_scarlet_hooded_crusader);
    new spell_koegler_book_burner();
    new spell_dragon_breath_sc();
}
