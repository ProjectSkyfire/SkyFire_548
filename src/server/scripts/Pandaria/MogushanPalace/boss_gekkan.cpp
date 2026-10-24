/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "mogu_shan_palace.h"

const uint32 auiGekkanAdds[4] =
{
    CREATURE_GLINTROK_SKULKER,
    CREATURE_GLINTROK_HEXXER,
    CREATURE_GLINTROK_ORACLE,
    CREATURE_GLINTROK_IRONHIDE,
};

static bool NotAtGekkan(Unit* owner)
{
    return owner->GetPositionZ() > -50.0f;
}

static void NotifyGekkan(Creature* me, int32 action)
{
    if (Creature* pGekkan = GetClosestCreatureWithEntry(me, CREATURE_GEKKAN, 100.0f))
        if (pGekkan->AI())
            pGekkan->AI()->DoAction(action);
}

class boss_gekkan : public CreatureScript
{
    public:
        boss_gekkan() : CreatureScript("boss_gekkan") { }

        enum eSpells
        {
            SPELL_RECKLESS_INSPIRATION     = 118988,
            SPELL_RECKLESS_INSPIRATION_2   = 129262
        };

        enum eActions
        {
            ACTION_ADD_DEATH,
            ACTION_COMBAT
        };

        enum eEvents
        {
            EVENT_RECKLESS_INSPIRATION     = 1,
        };

        enum eTalks
        {
            TALK_INTRO,
            TALK_KILLING,
            TALK_SPELL,
            TALK_AGGRO,
            TALK_DEATH,
        };

        struct boss_gekkan_AI : public BossAI
        {
            boss_gekkan_AI(Creature* creature) : BossAI(creature, DATA_GEKKAN) { }

            std::list<uint64> m_uilGekkanAdds;
            bool m_bYelled, m_FeignDeath;

            void InitializeAI() OVERRIDE
            {
                Reset();

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS_DEST, true);
                m_FeignDeath = false;
                m_bYelled = false;
            }

            void Reset() OVERRIDE
            {
                _Reset();
                events.Reset();
                m_FeignDeath = false;
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void InitializeGekkan()
            {
                for (uint32 entry : auiGekkanAdds)
                    if (Creature* GekkanProtector = GetClosestCreatureWithEntry(me, entry, 50.0f, true))
                        m_uilGekkanAdds.push_back(GekkanProtector->GetGUID());
            }

            void HandleAddDeath()
            {
                Talk(TALK_SPELL);
                DoCast(SPELL_RECKLESS_INSPIRATION_2);
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                DoZoneInCombat();

                if (who)
                {
                    // Get the four adds.
                    for (uint64 guid : m_uilGekkanAdds)
                    {
                        Creature* pAdd = ObjectAccessor::GetCreature(*me, guid);
                        if (pAdd && pAdd->AI())
                            pAdd->Attack(who, false);
                    }

                    // The oracle does not join the fight on its own
                    if (Creature* Orakle = GetClosestCreatureWithEntry(me, CREATURE_GLINTROK_ORACLE, 50.0f, true))
                        Orakle->Attack(who, false);
                }

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(TALK_AGGRO);

                events.ScheduleEvent(EVENT_RECKLESS_INSPIRATION, urand(10000, 16000));
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (who && who->GetTypeId() == TypeID::TYPEID_PLAYER && !m_bYelled)
                {
                    Talk(TALK_INTRO);
                    m_bYelled = true;
                    InitializeGekkan();
                }

                CreatureAI::MoveInLineOfSight(who);
            }

            void JustReachedHome() OVERRIDE
            {
                // Respawn all adds on evade
                for (uint64 guid : m_uilGekkanAdds)
                {
                    if (Creature* pAdd = ObjectAccessor::GetCreature(*me, guid))
                    {
                        if (!pAdd->IsAlive())
                            pAdd->Respawn();

                        pAdd->RemoveAllAuras();
                    }
                }

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            Creature* GetRandomAliveAdd()
            {
                std::list<uint64> uiTempGuidList;

                for (uint64 guid : m_uilGekkanAdds)
                {
                    Creature* pAdd = ObjectAccessor::GetCreature(*me, guid);
                    if (pAdd && pAdd->IsAlive())
                        uiTempGuidList.push_back(guid);
                }

                if (uiTempGuidList.empty())
                    return NULL;

                return ObjectAccessor::GetCreature(*me, Skyfire::Containers::SelectRandomContainerElement(uiTempGuidList));
            }

            bool HasAnyProtectorAlive()
            {
                if (!m_bYelled)
                    return true;

                for (uint64 guid : m_uilGekkanAdds)
                    if (Creature* prt = ObjectAccessor::GetCreature(*me, guid))
                        if (prt->IsAlive())
                            return true;

                return false;
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->SetData(TYPE_GEKKAN, DONE);
                }
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                summons.DespawnAll();

                for (uint64 guid : m_uilGekkanAdds)
                {
                    Creature* pAdd = ObjectAccessor::GetCreature(*me, guid);
                    if (pAdd && pAdd->AI())
                        pAdd->AI()->EnterEvadeMode();
                }
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(TALK_KILLING);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_ADD_DEATH:
                        if (m_bYelled && !m_FeignDeath) // use this spell and talk only if alive
                            HandleAddDeath();

                        if (!HasAnyProtectorAlive() && m_FeignDeath)
                            me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        break;
                    case ACTION_COMBAT:
                        DoZoneInCombat();
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (m_FeignDeath && HasAnyProtectorAlive())
                    damage = 0;

                if (!m_FeignDeath && me->GetHealth() <= damage && HasAnyProtectorAlive())
                {
                    damage = 0;
                    me->RemoveAllAuras();
                    me->ClearAllReactives();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetStandState(UNIT_STAND_STATE_DEAD);
                    me->AttackStop();
                    m_FeignDeath = true;
                    events.Reset();

                    // If we die, all remaining adds should get buff
                    for (uint64 guid : m_uilGekkanAdds)
                    {
                        Creature* pAdd = ObjectAccessor::GetCreature(*me, guid);
                        if (pAdd && pAdd->IsAlive())
                            pAdd->CastSpell(pAdd, SPELL_RECKLESS_INSPIRATION_2, false);
                    }

                    Talk(TALK_DEATH);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                // Someone tries to taunt us up onto the balcony instead of our battle area
                if (Unit* victim = me->GetVictim())
                    if (NotAtGekkan(victim))
                    {
                        EnterEvadeMode();
                        return;
                    }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_RECKLESS_INSPIRATION)
                    {
                        Talk(TALK_SPELL);

                        if (Creature* pAdd = GetRandomAliveAdd())
                            DoCast(pAdd, SPELL_RECKLESS_INSPIRATION, false);

                        events.ScheduleEvent(EVENT_RECKLESS_INSPIRATION, urand(15000, 21000));
                    }
                    break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_gekkan_AI(creature);
        }
};

class npc_glintrok_skulker : public CreatureScript
{
    public:
        npc_glintrok_skulker() : CreatureScript("npc_glintrok_skulker") { }

        enum eSpells
        {
            SPELL_SHANK        = 118963,
            SPELL_STEALTH      = 118969
        };

        struct npc_glintrok_skulker_AI : public ScriptedAI
        {
            npc_glintrok_skulker_AI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                if (!me->HasAura(SPELL_STEALTH))
                    me->CastSpell(me, SPELL_STEALTH, false);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(1, 2000);

                if (!NotAtGekkan(me))
                    NotifyGekkan(me, 1);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                DoCast(SPELL_STEALTH);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (!NotAtGekkan(me))
                    NotifyGekkan(me, 0);
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
                    if (eventId == 1)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_SHANK, false);
                        events.ScheduleEvent(1, 7000);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_glintrok_skulker_AI(creature);
        }
};

class npc_glintrok_ironhide : public CreatureScript
{
    public:
        npc_glintrok_ironhide() : CreatureScript("npc_glintrok_ironhide") { }

        struct npc_glintrok_ironhide_AI : public ScriptedAI
        {
            npc_glintrok_ironhide_AI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                DoCast(me, SPELL_IRON_PROTECTOR);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->ClearUnitState(UNIT_STATE_CASTING);
                NotifyGekkan(me, 1);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                NotifyGekkan(me, 0);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_glintrok_ironhide_AI(creature);
        }
};

class npc_glintrok_oracle : public CreatureScript
{
    public:
        npc_glintrok_oracle() : CreatureScript("npc_glintrok_oracle") { }

        enum eSpells
        {
            SPELL_CLEANSING_FLAME        = 118940,
            SPELL_FIRE_BOLT              = 118936,
        };

        struct npc_glintrok_oracle_AI : public ScriptedAI
        {
            npc_glintrok_oracle_AI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                if (!NotAtGekkan(me))
                    SetCombatMovement(false);

                ScriptedAI::InitializeAI();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(1, 16000);
                events.ScheduleEvent(2, 4000);

                NotifyGekkan(me, 1);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                NotifyGekkan(me, 0);
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
                        case 1:
                            me->CastSpell(me, SPELL_CLEANSING_FLAME, false);
                            events.ScheduleEvent(1, 25000);
                            break;
                        case 2:
                            me->CastSpell(me->GetVictim(), SPELL_FIRE_BOLT, false);
                            events.ScheduleEvent(2, 7000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_glintrok_oracle_AI(creature);
        }
};

class npc_glintrok_hexxer : public CreatureScript
{
    public:
        npc_glintrok_hexxer() : CreatureScript("npc_glintrok_hexxer") { }

        enum eSpells
        {
            SPELL_HEX_OF_LETHARGY        = 118903,
            SPELL_DARK_BOLT              = 118917,
        };

        struct npc_glintrok_hexxer_AI : public ScriptedAI
        {
            npc_glintrok_hexxer_AI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                if (!NotAtGekkan(me))
                    SetCombatMovement(false);

                ScriptedAI::InitializeAI();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(1, 10000);
                events.ScheduleEvent(2, 4000);

                if (!NotAtGekkan(me))
                    NotifyGekkan(me, 1);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (!NotAtGekkan(me))
                    NotifyGekkan(me, 0);
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
                        case 1:
                            me->CastSpell(me->GetVictim(), SPELL_HEX_OF_LETHARGY, false);
                            events.ScheduleEvent(1, 20000);
                            break;
                        case 2:
                            me->CastSpell(me->GetVictim(), SPELL_DARK_BOLT, false);
                            events.ScheduleEvent(2, 5000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_glintrok_hexxer_AI(creature);
        }
};

void AddSC_boss_gekkan()
{
    new boss_gekkan();
    new npc_glintrok_hexxer();
    new npc_glintrok_skulker();
    new npc_glintrok_oracle();
    new npc_glintrok_ironhide();
}
