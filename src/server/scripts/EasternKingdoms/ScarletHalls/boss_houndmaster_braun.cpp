/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "scarlet_halls.h"

enum Spells
{
    SPELL_PIERCING_THROW          = 114004,
    SPELL_DEATH_BLOSSOM           = 114242, // damage in 8y
    SPELL_DEATH_BLOSSOM_LEAP      = 114241, // only leap to target
    SPELL_BLOODY_MESS             = 114056,
    SPELL_BLOODY_RAGE             = 116140, // at 50% health
    SPELL_RAKE_NORMAL             = 24331,
    SPELL_RAKE_HEROIC             = 120560,
    SPELL_HOUNDMASTER_BRAUN_EATEN = 122916
};

enum Yells
{
    TALK_INTO                     = 0,
    TALK_SUDENDEATH_01            = 1,
    TALK_SUDENDEATH_02            = 2,
    TALK_DEATH                    = 3,
    TALK_DOGFAIL                  = 4,
};

enum Events
{
    // Braun
    EVENT_PIERCING_THROW          = 1,
    EVENT_DEATH_BLOSSOM           = 2,
    EVENT_AT_END                  = 3,

    // Dogs
    EVENT_RAKE                    = 1,
};

enum Actions
{
    ACTION_ACTIVATE_DOG           = 1,
    ACTION_FINISH_HIM             = 2,
    ACTION_MOVE_TO                = 3,
    ACTION_EATING                 = 4
};

class boss_houndmaster_braun : public CreatureScript
{
    public:
        boss_houndmaster_braun() : CreatureScript("boss_houndmaster_braun") { }

        struct boss_houndmaster_braunAI : public BossAI
        {
            boss_houndmaster_braunAI(Creature* creature) : BossAI(creature, BOSS_HOUNDMASTER_BRAUN) { }

            bool GetRage;
            bool Scenario;
            bool SudenDeath;

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                events.Reset();
                me->setRegeneratingHealth(true);
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                GetRage = false;
                Scenario = false;
                SudenDeath = false;
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(TALK_DEATH);
                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                me->RemoveAllAuras();
                HandleSendActionOnHounds(ACTION_MOVE_TO);
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();
                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(TALK_INTO);
                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                events.ScheduleEvent(EVENT_PIERCING_THROW, 7000);
                events.ScheduleEvent(EVENT_DEATH_BLOSSOM, urand(11000, 12000));
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (HealthBelowPct(50) && !GetRage)
                {
                    me->CastSpell(me, SPELL_BLOODY_RAGE, false);
                    GetRage = true;
                    Talk(TALK_DOGFAIL);
                }

                if (HealthBelowPct(15) && !Scenario)
                {
                    damage = 0;
                    Scenario = true;
                    Talk(TALK_SUDENDEATH_01);
                    events.Reset();
                    events.ScheduleEvent(EVENT_AT_END, 5000);
                    me->RemoveAllAuras();
                    HandleSendActionOnHounds(ACTION_FINISH_HIM);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->AttackStop();
                }
            }

            void HandleSendActionOnHounds(uint32 actionId)
            {
                std::list<Creature*> obedientHounds;
                GetCreatureListWithEntryInGrid(obedientHounds, me, NPC_OBEDIEND_HOUND, 200.0f);

                for (Creature* hound : obedientHounds)
                    if (hound->AI())
                        hound->AI()->DoAction(actionId);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PIERCING_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_PIERCING_THROW, false);
                            events.ScheduleEvent(EVENT_PIERCING_THROW, 7000);
                            break;
                        case EVENT_DEATH_BLOSSOM:
                            if (Unit* target = me->SelectNearestTarget())
                                me->CastSpell(target, SPELL_DEATH_BLOSSOM_LEAP, false);
                            events.ScheduleEvent(EVENT_DEATH_BLOSSOM, urand(11000, 12000));
                            break;
                        case EVENT_AT_END:
                            if (!SudenDeath)
                            {
                                me->CastSpell(me, SPELL_HOUNDMASTER_BRAUN_EATEN, false);
                                Talk(TALK_SUDENDEATH_02);
                                SudenDeath = true;
                                HandleSendActionOnHounds(ACTION_EATING);
                                events.ScheduleEvent(EVENT_AT_END, urand(7000, 10000));
                                return;
                            }

                            if (Player* originalCaster = me->FindNearestPlayer(50.0f))
                            {
                                me->SetLootRecipient(originalCaster);
                                originalCaster->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                                events.Reset();
                            }
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_houndmaster_braunAI(creature);
        }
};

// Obedient Hound 59309
class npc_obediend_hound : public CreatureScript
{
    public:
        npc_obediend_hound() : CreatureScript("npc_obediend_hound") { }

        struct npc_obediend_houndAI : public ScriptedAI
        {
            npc_obediend_houndAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE && pointId == 1)
                    KillThemAll();
            }

            void KillThemAll()
            {
                std::list<Creature*> guardians;
                GetCreatureListWithEntryInGrid(guardians, me, NPC_SCARLET_GUARDIAN, 15.0f);

                for (Creature* guardian : guardians)
                    guardian->DealDamage(guardian, guardian->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                if (Creature* verdone = GetClosestCreatureWithEntry(me, NPC_SERGEANT_VERDONE, 20.0f, true))
                    verdone->DealDamage(verdone, verdone->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                me->DespawnOrUnsummon(1500);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_HUMANE_SOCIETY, 0);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_ACTIVATE_DOG:
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        if (Unit* target = me->SelectNearestTarget(100.0f))
                        {
                            me->GetMotionMaster()->MoveChase(target);
                            me->Attack(target, true);
                        }
                        break;
                    case ACTION_FINISH_HIM:
                        events.Reset();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
                        me->setFaction(35);

                        if (_instance)
                        {
                            if (Unit* braun = ObjectAccessor::GetUnit(*me, _instance->GetData64(BOSS_HOUNDMASTER_BRAUN)))
                            {
                                me->Attack(braun, true);
                                me->GetMotionMaster()->MoveChase(braun);
                            }
                        }

                        events.ScheduleEvent(EVENT_RAKE, 3500);
                        break;
                    case ACTION_MOVE_TO:
                        events.Reset();
                        me->GetMotionMaster()->MovePoint(1, EndScenario);
                        break;
                    case ACTION_EATING:
                        if (_instance)
                            if (Unit* braun = ObjectAccessor::GetUnit(*me, _instance->GetData64(BOSS_HOUNDMASTER_BRAUN)))
                                me->CastSpell(braun, IsHeroic() ? SPELL_RAKE_HEROIC : SPELL_RAKE_NORMAL, true);
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_RAKE)
                    {
                        if (Unit* target = me->GetVictim())
                            me->CastSpell(target, IsHeroic() ? SPELL_RAKE_HEROIC : SPELL_RAKE_NORMAL, true);

                        events.ScheduleEvent(EVENT_RAKE, 3500);
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_obediend_houndAI(creature);
        }
};

// Bloody Mess 114020
class spell_bloody_mess : public SpellScriptLoader
{
    public:
        spell_bloody_mess() : SpellScriptLoader("spell_bloody_mess") { }

        class spell_bloody_mess_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_bloody_mess_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                std::list<Player*> playersInArea;
                GetPlayerListInGrid(playersInArea, caster, 100.0f);

                for (Player* player : playersInArea)
                    if (player->IsInBetween(caster, target, 3.0f) && player->GetGUID() != target->GetGUID())
                        caster->AddAura(SPELL_BLOODY_MESS, player);
            }

            void Register() OVERRIDE
            {
                OnHit += SpellHitFn(spell_bloody_mess_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_bloody_mess_SpellScript();
        }
};

// Crushing Leap Visual 108481
class spell_crushing_leap_visual : public SpellScriptLoader
{
    public:
        spell_crushing_leap_visual() : SpellScriptLoader("spell_crushing_leap_visual") { }

        class spell_crushing_leap_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_crushing_leap_visual_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_DEATH_BLOSSOM, false);
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_crushing_leap_visual_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_MELEE_HASTE_2, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_crushing_leap_visual_AuraScript();
        }
};

void AddSC_boss_houndmaster_braun()
{
    new boss_houndmaster_braun();
    new npc_obediend_hound();
    new spell_bloody_mess();
    new spell_crushing_leap_visual();
}
