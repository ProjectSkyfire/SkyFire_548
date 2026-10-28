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
#include "Player.h"
#include "ObjectAccessor.h"

enum spells
{
    SPELL_PIERCING_THROW          = 114004,
    SPELL_DEATH_BLOSSOM           = 114242, // damage in 8y
    SPELL_DEATH_BLOSSOM_LEAP      = 114241, // only leap to target
    SPELL_BLOODY_MESS             = 114056,
    SPELL_BLOODY_RAGE             = 116140, // at 50% healh
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

enum events
{
    /*Braun*/
    EVENT_PIERCING_THROW          = 1,
    EVENT_DEATH_BLOSSOM           = 2,
    EVENT_CALL_DOG                = 3,
    EVENT_AT_END                  = 4,
    EVENT_CALL_DOG_INTO           = 5,

    /*Dogs*/
    EVENT_RAKE                    = 1,
    EVENT_RAKE_AT_END             = 2
};

enum  Actions
{
    ACTION_ACTIVATE_DOG           = 1,
    ACTION_FINISH_HIM             = 2,
    ACTION_MOVE_TO                = 3,
    ACTION_EATING                 = 4,
    ACTION_RESET                  = 5
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
            int Phase;

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
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_AGGRESSIVE);
                GetRage = false;
                Scenario = false;
                SudenDeath = false;
                Phase = 0;
                HandleDoors(true);
                HandleSendActionOnHounds(ACTION_RESET);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(TALK_DEATH);
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }
                me->RemoveAllAuras();
                HandleSendActionOnHounds(ACTION_MOVE_TO);
                HandleDoors(true);
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }
                summons.DespawnAll();
                HandleDoors(true);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(TALK_INTO);
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }
                events.ScheduleEvent(EVENT_PIERCING_THROW, 7000);
                events.ScheduleEvent(EVENT_DEATH_BLOSSOM, urand(11000, 12000));
                HandleDoors();
            }

            void HandleDoors(bool reset = false)
            {
                if (instance)
                    if (GameObject* Idoor = GetClosestGameObjectWithEntry(me, GO_COMANDER_LINDON_EXIT, 150.0f))
                        instance->HandleGameObject(0, reset && instance->GetData(DATA_COMANDER_LINDON) == DONE, Idoor);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (me->HealthBelowPctDamaged(50, damage) && !GetRage)
                {
                    me->CastSpell(me, SPELL_BLOODY_RAGE, false);
                    GetRage = true;
                    Talk(TALK_DOGFAIL);
                }

                if (me->HealthBelowPctDamaged(15, damage) && !Scenario)
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

                if (!Scenario && ((Phase == 0 && me->HealthBelowPctDamaged(90, damage))
                    || (Phase == 1 && me->HealthBelowPctDamaged(80, damage))
                    || (Phase == 2 && me->HealthBelowPctDamaged(70, damage))
                    || (Phase == 3 && me->HealthBelowPctDamaged(60, damage))))
                {
                    Phase++;
                    CallObedientHound();
                }
            }

            // Braun calls one idle Obedient Hound into the fight at 90, 80, 70 and 60% health. The LoA script
            // counts these thresholds but never sends the hound.
            void CallObedientHound()
            {
                std::list<Creature*> hounds;
                GetCreatureListWithEntryInGrid(hounds, me, NPC_OBEDIEND_HOUND, 200.0f);

                for (Creature* hound : hounds)
                {
                    if (hound->IsAlive() && !hound->IsInCombat())
                    {
                        hound->AI()->DoAction(ACTION_ACTIVATE_DOG);
                        return;
                    }
                }
            }

            void HandleSendActionOnHounds(uint32 actionId)
            {
                std::list<Creature*> ObedientHounds;
                GetCreatureListWithEntryInGrid(ObedientHounds, me, NPC_OBEDIEND_HOUND, 200.0f);

                for (auto&& itr : ObedientHounds)
                    itr->AI()->DoAction(actionId);
            }

            Player* FindFinishingPlayer()
            {
                std::list<Player*> players;
                GetPlayerListInGrid(players, me, 100.0f);
                for (Player* player : players)
                    if (player->IsAlive() && !player->IsGameMaster())
                        return player;
                return NULL;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (Scenario && instance && instance->IsWipe(100.0f, me))
                {
                    EnterEvadeMode();
                    return;
                }

                if (!Scenario && !UpdateVictim())
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

                            if (SudenDeath)
                                if (Player* originalCaster = FindFinishingPlayer())
                                {
                                    me->SetLootRecipient(originalCaster);
                                    originalCaster->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                                    events.Reset();
                                }
                                else
                                    EnterEvadeMode();
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

// Obediend Hound 59309
class npc_obediend_hound : public CreatureScript
{
    public:
        npc_obediend_hound() : CreatureScript("npc_obediend_hound") { }

        struct npc_obediend_houndAI : public ScriptedAI
        {
            npc_obediend_houndAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner) OVERRIDE { }

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
                std::list<Creature*> Guardians;
                GetCreatureListWithEntryInGrid(Guardians, me, NPC_SCARLET_GUARDIAN, 15.0f);

                for (auto&& itr : Guardians)
                    itr->DealDamage(itr, itr->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                if (Creature* Verdone = GetClosestCreatureWithEntry(me, NPC_SERGEANT_VERDONE, 20.0f, true))
                    Verdone->DealDamage(Verdone, Verdone->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
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
                    case ACTION_RESET:
                        events.Reset();
                        me->RestoreFaction();
                        if (!me->IsAlive())
                            me->Respawn();
                        else
                            EnterEvadeMode();
                        break;
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
                            if (Unit* Braun = ObjectAccessor::GetUnit(*me, _instance->GetData64(BOSS_HOUNDMASTER_BRAUN)))
                            {
                                me->Attack(Braun, true);
                                me->GetMotionMaster()->MoveChase(Braun);
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
                            if (Unit* Braun = ObjectAccessor::GetUnit(*me, _instance->GetData64(BOSS_HOUNDMASTER_BRAUN)))
                                me->CastSpell(Braun, IsHeroic() ? SPELL_RAKE_HEROIC : SPELL_RAKE_NORMAL, true);
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                    if (eventId == EVENT_RAKE)
                    {
                        if (Unit* target = me->GetVictim())
                            me->CastSpell(target, IsHeroic() ? SPELL_RAKE_HEROIC : SPELL_RAKE_NORMAL, true);

                        events.ScheduleEvent(EVENT_RAKE, 3500);
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

// Bloody mess 114020
class spell_bloody_mess : public SpellScriptLoader
{
    public:
        spell_bloody_mess() : SpellScriptLoader("spell_bloody_mess") { }

        class spell_bloody_mess_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_bloody_mess_SpellScript);

        public:

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                std::list<Player*> PlayersInArea;
                GetPlayerListInGrid(PlayersInArea, caster, 100.0f);

                for (auto&& itr : PlayersInArea)
                {
                    if (itr->IsInBetween(caster, target, 3.0f) && itr->GetGUID() != target->GetGUID())
                        caster->AddAura(SPELL_BLOODY_MESS, itr);
                }
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

        public:

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* m_caster = GetCaster())
                    m_caster->CastSpell(m_caster, SPELL_DEATH_BLOSSOM, false);
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
