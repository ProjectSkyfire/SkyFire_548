/*
 * Ported from Legends of Azeroth 5.4.8.
 * Authors: Jason Dove, Kulyadev, lee, leelf00
 * See LICENSE.md for copyright information.
 */
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"
#include "scarlet_monastery_mop.h"

enum WhitemaneSpells
{
    SPELL_FLASH_OF_STEEL = 115629,
    SPELL_DASHING_STRIKE = 115739,
    SPELL_DASHING_TARGET = 122519,
    SPELL_DASHING_INVISIBLE = 115676,
    SPELL_DASHING_AURA = 115704,
    SPELL_POWER_WORD_SHIELD = 22187,
    SPELL_SMITE = 114848,
    SPELL_HEAL = 36678,
    SPELL_MASS_RESURRECTION = 113134,
    SPELL_DEEP_SLEEP = 9256,
    SPELL_SCARLET_RESURRECTION = 9232,
    SPELL_CORPSE_FOCUS = 126789,
    SPELL_SOUL_TARGET = 126791,
    SPELL_UNQUENCHABLE = 126802,
    SPELL_HAND_OF_PROVIDENCE = 126832,
    SPELL_SOUL_MISSILE = 126795
};

enum WhitemaneEncounter
{
    ACTION_INTRO = 1, ACTION_REVIVED, ACTION_LAST_PHASE, ACTION_RESET_PAIR, ACTION_OUTRO,
    EVENT_FLASH = 1, EVENT_DASH, EVENT_INTRO, EVENT_SMITE, EVENT_HEAL, EVENT_MASS_RESURRECTION,
    EVENT_REVIVE, EVENT_OUTRO_MOVE, EVENT_OUTRO_SAY, EVENT_OUTRO_CORPSE, EVENT_OUTRO_LINE,
    NPC_SCARLET_FANATIC = 58555, NPC_SCARLET_PURIFIER = 58569, NPC_SCARLET_ZEALOT = 58590,
    NPC_SCARLET_JUDICATOR = 58605, NPC_SCARLET_INITIATE = 58783
};

static Position const whitemaneIntro = { 740.48f, 606.35f, 15.027f, 6.26f };

class boss_commander_durand : public CreatureScript
{
public:
    boss_commander_durand() : CreatureScript("boss_commander_durand") { }
    struct boss_commander_durandAI : public BossAI
    {
        boss_commander_durandAI(Creature* creature) : BossAI(creature, BOSS_DURAND), fakeDeath(false), awaitingRevival(false), flashCount(0), dashCount(0) { }
        bool fakeDeath, awaitingRevival;
        uint8 flashCount, dashCount;

        void Reset() OVERRIDE
        {
            _Reset();
            fakeDeath = awaitingRevival = false;
            flashCount = dashCount = 0;
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetStandState(UNIT_STAND_STATE_STAND);
        }
        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            if (!me->IsInCombat())
                return;
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            Talk(0);
            events.ScheduleEvent(EVENT_FLASH, 9000);
            events.ScheduleEvent(EVENT_DASH, urand(24000, 25000));
        }
        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (dashCount)
                damage = 0;
            if (!fakeDeath && damage >= me->GetHealth() && instance)
            {
                Creature* whitemane = instance->instance->GetCreature(instance->GetData64(BOSS_WHITEMANE));
                if (!whitemane || !whitemane->IsAlive())
                    return;
                damage = 0;
                fakeDeath = true;
                awaitingRevival = true;
                events.Reset();
                me->InterruptNonMeleeSpells(false);
                me->RemoveAllAuras();
                me->SetHealth(1);
                me->AttackStop();
                me->SetReactState(REACT_PASSIVE);
                me->SetStandState(UNIT_STAND_STATE_DEAD);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->HandleGameObject(instance->GetData64(GO_WHITEMANE_ENTRANCE), true);
                whitemane->AI()->DoAction(ACTION_INTRO);
            }
        }
        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_REVIVED)
            {
                awaitingRevival = false;
                dashCount = flashCount = 0;
                events.ScheduleEvent(EVENT_FLASH, 9000);
                events.ScheduleEvent(EVENT_DASH, urand(24000, 25000));
                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            }
            else if (action == ACTION_RESET_PAIR)
            {
                if (!me->IsAlive())
                    me->Respawn();
                else
                    EnterEvadeMode();
            }
        }
        void EnterEvadeMode() OVERRIDE
        {
            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                if (Creature* other = instance->instance->GetCreature(instance->GetData64(BOSS_WHITEMANE)))
                    if (other->IsAlive() && other->IsInCombat())
                        other->AI()->DoAction(ACTION_RESET_PAIR);
            }
            BossAI::EnterEvadeMode();
        }
        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (awaitingRevival || !UpdateVictim())
                return;
            events.Update(diff);
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FLASH:
                        if (flashCount++ < 6)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_FLASH_OF_STEEL, false);
                            events.ScheduleEvent(EVENT_FLASH, urand(500, 1000));
                        }
                        else
                        {
                            flashCount = 0;
                            events.ScheduleEvent(EVENT_FLASH, 21000);
                        }
                        break;
                    case EVENT_DASH:
                        if (dashCount++ < 6)
                        {
                            if (dashCount == 1)
                            {
                                Talk(1);
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                me->CastSpell(me, SPELL_DASHING_INVISIBLE, true);
                            }
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            {
                                DoCastAOE(SPELL_DASHING_TARGET, true);
                                me->CastSpell(me, SPELL_DASHING_AURA, true);
                                me->CastSpell(target, SPELL_DASHING_STRIKE, false);
                            }
                            events.ScheduleEvent(EVENT_DASH, urand(500, 1000));
                        }
                        else
                        {
                            dashCount = 0;
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            events.ScheduleEvent(EVENT_DASH, urand(24000, 25000));
                        }
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new boss_commander_durandAI(creature); }
};

class boss_high_inqusitior_whitemane : public CreatureScript
{
public:
    boss_high_inqusitior_whitemane() : CreatureScript("boss_high_inqusitior_whitemane") { }
    struct boss_high_inqusitior_whitemaneAI : public BossAI
    {
        boss_high_inqusitior_whitemaneAI(Creature* creature) : BossAI(creature, BOSS_WHITEMANE), revived(false), resurrecting(false), resetting(false) { }
        bool revived, resurrecting, resetting;
        void Reset() OVERRIDE
        {
            _Reset();
            revived = resurrecting = resetting = false;
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            if (instance)
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEEP_SLEEP);
        }
        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_INTRO)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->GetMotionMaster()->MovePoint(0, whitemaneIntro);
                events.ScheduleEvent(EVENT_INTRO, 8000);
            }
            else if (action == ACTION_LAST_PHASE && resurrecting && instance)
            {
                resurrecting = false;
                instance->SetBossState(BOSS_DURAND, SPECIAL);
                if (Creature* durand = instance->instance->GetCreature(instance->GetData64(BOSS_DURAND)))
                    durand->AI()->DoAction(ACTION_REVIVED);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEEP_SLEEP);
                me->SetReactState(REACT_AGGRESSIVE);
                if (Player* player = me->FindNearestPlayer(80.0f))
                    AttackStart(player);
                events.ScheduleEvent(EVENT_SMITE, 500);
                events.ScheduleEvent(EVENT_HEAL, 15000);
                events.ScheduleEvent(EVENT_MASS_RESURRECTION, 10000);
            }
            else if (action == ACTION_RESET_PAIR && !resetting)
                EnterEvadeMode();
        }
        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE && id == 0 && !me->IsInCombat())
                StartEncounter();
            else if (type == POINT_MOTION_TYPE && id == 1 && resurrecting)
                events.RescheduleEvent(EVENT_REVIVE, 1);
        }
        void StartEncounter()
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            if (Player* player = me->FindNearestPlayer(80.0f))
                AttackStart(player);
            else
                EnterEvadeMode();
        }
        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            if (!me->IsInCombat())
                return;
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            events.ScheduleEvent(EVENT_SMITE, 500);
            events.ScheduleEvent(EVENT_HEAL, urand(9000, 13000));
            events.ScheduleEvent(EVENT_MASS_RESURRECTION, 20000);
        }
        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (resurrecting && damage >= me->GetHealth())
                damage = 0;
            if (!revived && me->HealthBelowPctDamaged(50, damage) && instance)
            {
                damage = 0;
                revived = resurrecting = true;
                events.Reset();
                me->InterruptNonMeleeSpells(true);
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                DoCastAOE(SPELL_DEEP_SLEEP, true);
                me->CastSpell(me, SPELL_POWER_WORD_SHIELD, true);
                if (Creature* durand = instance->instance->GetCreature(instance->GetData64(BOSS_DURAND)))
                    me->GetMotionMaster()->MovePoint(1, durand->GetPositionX(), durand->GetPositionY(), durand->GetPositionZ());
                events.ScheduleEvent(EVENT_REVIVE, 8000);
            }
        }
        void EnterEvadeMode() OVERRIDE
        {
            if (resetting)
                return;
            resetting = true;
            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEEP_SLEEP);
                instance->HandleGameObject(instance->GetData64(GO_WHITEMANE_ENTRANCE), false);
                if (Creature* durand = instance->instance->GetCreature(instance->GetData64(BOSS_DURAND)))
                    durand->AI()->DoAction(ACTION_RESET_PAIR);
            }
            BossAI::EnterEvadeMode();
        }
        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
            if (!instance)
                return;
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEEP_SLEEP);
            me->CastSpell(me, SPELL_CORPSE_FOCUS, true);
            me->CastSpell(me, SPELL_SOUL_TARGET, true);
            me->CastSpell(me, SPELL_UNQUENCHABLE, true);
            me->CastSpell(me, SPELL_HAND_OF_PROVIDENCE, true);
            me->CastSpell(me, SPELL_SOUL_MISSILE, true);
            if (Creature* hooded = instance->instance->GetCreature(instance->GetData64(NPC_HOODED_CRUSADER_OUTRO)))
                hooded->AI()->DoAction(ACTION_OUTRO);
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);
            if (resurrecting)
            {
                if (events.ExecuteEvent() == EVENT_REVIVE && instance)
                {
                    if (Creature* durand = instance->instance->GetCreature(instance->GetData64(BOSS_DURAND)))
                        if (!me->HasUnitState(UNIT_STATE_CASTING))
                            me->CastSpell(durand, SPELL_SCARLET_RESURRECTION, false);
                    events.ScheduleEvent(EVENT_REVIVE, 5000);
                }
                return;
            }
            if (!UpdateVictim())
            {
                if (events.ExecuteEvent() == EVENT_INTRO)
                    StartEncounter();
                return;
            }
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SMITE:
                        me->CastSpell(me->GetVictim(), SPELL_SMITE, false);
                        events.ScheduleEvent(EVENT_SMITE, urand(2600, 3000));
                        break;
                    case EVENT_HEAL:
                        {
                            Creature* target = me;
                            if (instance && urand(0, 1))
                                if (Creature* durand = instance->instance->GetCreature(instance->GetData64(BOSS_DURAND)))
                                    if (durand->IsAlive())
                                        target = durand;
                            me->CastSpell(target, SPELL_HEAL, false);
                        }
                        events.ScheduleEvent(EVENT_HEAL, urand(9000, 13000));
                        break;
                    case EVENT_MASS_RESURRECTION:
                        me->CastSpell(me, SPELL_MASS_RESURRECTION, false);
                        events.ScheduleEvent(EVENT_MASS_RESURRECTION, 20000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return creature->GetMapId() == 1004 ? new boss_high_inqusitior_whitemaneAI(creature) : NULL;
    }
};

class spell_sc_scarlet_ressurection : public SpellScript
{
    PrepareSpellScript(spell_sc_scarlet_ressurection);
public:
    void HandleAfterCast()
    {
        if (Creature* caster = GetCaster()->ToCreature())
            if (caster->GetMapId() == 1004 && caster->GetEntry() == NPC_HIGH_INQUISITOR_WHITEMANE)
                caster->AI()->DoAction(ACTION_LAST_PHASE);
    }
    void Register() OVERRIDE { AfterCast += SpellCastFn(spell_sc_scarlet_ressurection::HandleAfterCast); }
};

class spell_sc_mass_ressurection : public SpellScript
{
    PrepareSpellScript(spell_sc_mass_ressurection);
public:
    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (GetCaster()->GetMapId() != 1004 || GetCaster()->GetEntry() != NPC_HIGH_INQUISITOR_WHITEMANE)
            return;
        targets.remove_if([this](WorldObject* target)
        {
            if (target->GetTypeId() != TypeID::TYPEID_UNIT)
                return true;
            switch (target->GetEntry())
            {
                case NPC_SCARLET_FANATIC: case NPC_SCARLET_PURIFIER: case NPC_SCARLET_ZEALOT:
                case NPC_SCARLET_JUDICATOR: case NPC_SCARLET_INITIATE: break;
                default: return true;
            }
            return target->ToCreature()->IsAlive() || GetCaster()->GetExactDist(target) > 80.0f;
        });
    }
    void HandleHit()
    {
        if (GetCaster()->GetMapId() != 1004 || GetCaster()->GetEntry() != NPC_HIGH_INQUISITOR_WHITEMANE)
            return;
        if (Creature* target = GetHitCreature())
        {
            target->Respawn();
            if (target->IsAlive())
                if (Player* player = target->FindNearestPlayer(100.0f))
                    target->AI()->AttackStart(player);
        }
    }
    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sc_mass_ressurection::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        OnHit += SpellHitFn(spell_sc_mass_ressurection::HandleHit);
    }
};

struct npc_hooded_crusader_c64842 : public ScriptedAI
{
    npc_hooded_crusader_c64842(Creature* creature) : ScriptedAI(creature), line(0) { }
    EventMap events;
    uint8 line;
    void Reset() OVERRIDE { events.Reset(); line = 0; me->SetVisible(false); }
    void DoAction(int32 action) OVERRIDE
    {
        if (action == ACTION_OUTRO)
        {
            me->SetVisible(true);
            events.Reset();
            events.ScheduleEvent(EVENT_OUTRO_MOVE, 1);
        }
    }
    void UpdateAI(uint32 diff) OVERRIDE
    {
        events.Update(diff);
        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_OUTRO_MOVE:
                    me->GetMotionMaster()->MovePoint(0, whitemaneIntro);
                    events.ScheduleEvent(EVENT_OUTRO_SAY, 1000);
                    break;
                case EVENT_OUTRO_SAY:
                    Talk(0);
                    events.ScheduleEvent(EVENT_OUTRO_CORPSE, 4000);
                    break;
                case EVENT_OUTRO_CORPSE:
                    if (Creature* corpse = me->FindNearestCreature(NPC_HIGH_INQUISITOR_WHITEMANE, 100.0f, false))
                        me->GetMotionMaster()->MovePoint(1, corpse->GetPositionX(), corpse->GetPositionY(), corpse->GetPositionZ());
                    line = 1;
                    events.ScheduleEvent(EVENT_OUTRO_LINE, 1000);
                    break;
                case EVENT_OUTRO_LINE:
                    Talk(line++);
                    if (line < 5)
                        events.ScheduleEvent(EVENT_OUTRO_LINE, 4000);
                    break;
            }
        }
    }
};

void AddSC_boss_whitemane_and_durand()
{
    new boss_commander_durand();
    new boss_high_inqusitior_whitemane();
    RegisterSpellScript(spell_sc_scarlet_ressurection);
    RegisterSpellScript(spell_sc_mass_ressurection);
    new creature_script<npc_hooded_crusader_c64842>("npc_hooded_crusader_c64842");
}
