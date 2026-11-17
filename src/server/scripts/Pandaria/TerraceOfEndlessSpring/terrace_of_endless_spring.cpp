/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Config.h"
#include "terrace_of_endless_spring.h"

#include <list>
#include <vector>

class npc_terrace_springtender_ashani : public CreatureScript
{
public:
    npc_terrace_springtender_ashani() : CreatureScript("npc_terrace_springtender_ashani") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
            creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        player->CLOSE_GOSSIP_MENU();
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (sConfigMgr->GetIntDefault("FirstOfTheKingdom.TerraceSpring", 1))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Turn off dampening?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    struct npc_terrace_springtender_ashaniAI : public ScriptedAI
    {
        npc_terrace_springtender_ashaniAI(Creature* creature) : ScriptedAI(creature) { }

        void InitializeAI() OVERRIDE
        {
            if (me->GetMap()->GetDifficulty() != DIFFICULTY_25MAN_LFR)
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_terrace_springtender_ashaniAI(creature);
    }
};

enum ApparitionSpells
{
    SPELL_CHANNELING_CORRUPTION  = 125706,
    SPELL_CORRUPTION_BOLT       = 125713,
    SPELL_OVERWHELMING_FEAR     = 125758,
    SPELL_NIGHT_TERRORS_MISSILE = 125736,
    SPELL_NIGHT_TERRORS_PERIODIC = 125745,
    SPELL_ENVELOPING_DARKNESS   = 125760,
    SPELL_GRIP_OF_FEAR          = 130115,
    SPELL_UNLEASHED_TERROR      = 130120
};

enum ApparitionEvents
{
    EVENT_OVERWHELMING_FEAR = 1,
    EVENT_NIGHT_TERRORS_MISSILE = 2,
    EVENT_ENVELOPING_DARKNESS = 4,
    EVENT_GRIP_OF_FEAR = 5,
    EVENT_UNLEASHED_TERROR = 6
};

struct apparition_ai : public ScriptedAI
{
    apparition_ai(Creature* creature, bool isFear)
        : ScriptedAI(creature), summons(me), instance(creature->GetInstanceScript()),
          corruptionBoltTimer(0), fearApparition(isFear) { }

    SummonList summons;
    InstanceScript* instance;
    EventMap events;
    uint32 corruptionBoltTimer;
    bool fearApparition;

    void Reset() OVERRIDE
    {
        me->CastSpell(me, SPELL_CHANNELING_CORRUPTION, true);
        events.Reset();
        corruptionBoltTimer = 3000;
        summons.DespawnAll();

        if (instance)
            instance->SetData(INTRO_DONE, NOT_STARTED);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->RemoveAurasDueToSpell(SPELL_CHANNELING_CORRUPTION);
        events.Reset();

        if (fearApparition)
        {
            events.ScheduleEvent(EVENT_OVERWHELMING_FEAR, 8000);
            events.ScheduleEvent(EVENT_NIGHT_TERRORS_MISSILE, 13000);
        }
        else
        {
            events.ScheduleEvent(EVENT_GRIP_OF_FEAR, 5000);
            events.ScheduleEvent(EVENT_UNLEASHED_TERROR, 9000);
        }
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);
    }

    void SummonedCreatureDespawn(Creature* summon) OVERRIDE
    {
        summons.Despawn(summon);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        std::list<Creature*> fear;
        me->GetCreatureListWithEntryInGrid(fear, NPC_APPARITION_OF_FEAR, 100.0f);
        std::list<Creature*> terror;
        me->GetCreatureListWithEntryInGrid(fear, NPC_APPARITION_OF_TERROR, 100.0f);

        bool done = true;
        for (std::list<Creature*>::const_iterator itr = fear.begin(); itr != fear.end(); ++itr)
        {
            if ((*itr)->IsAlive())
            {
                done = false;
                break;
            }
        }

        for (std::list<Creature*>::const_iterator itr = terror.begin(); itr != terror.end(); ++itr)
        {
            if ((*itr)->IsAlive())
            {
                done = false;
                break;
            }
        }

        if (done && instance)
            instance->SetData(INTRO_DONE, DONE);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
        {
            if (corruptionBoltTimer)
            {
                if (corruptionBoltTimer <= diff)
                {
                    corruptionBoltTimer = 3000;

                    if (instance)
                    {
                        Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI));
                        Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL));
                        Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN));
                        std::vector<uint64> targets;

                        if (asani)
                            targets.push_back(asani->GetGUID());
                        if (regail)
                            targets.push_back(regail->GetGUID());
                        if (kaolan)
                            targets.push_back(kaolan->GetGUID());

                        if (targets.empty())
                            return;

                        if (Unit* target = ObjectAccessor::GetUnit(*me, targets[urand(0, uint32(targets.size() - 1))]))
                            me->CastSpell(target, SPELL_CORRUPTION_BOLT, true);
                    }
                }
                else
                    corruptionBoltTimer -= diff;
            }

            return;
        }

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_OVERWHELMING_FEAR:
                    me->CastSpell(me, SPELL_OVERWHELMING_FEAR, false);
                    events.ScheduleEvent(EVENT_OVERWHELMING_FEAR, 12000);
                    break;
                case EVENT_NIGHT_TERRORS_MISSILE:
                    me->CastSpell(me, SPELL_NIGHT_TERRORS_MISSILE, false);
                    events.ScheduleEvent(EVENT_NIGHT_TERRORS_MISSILE, 20000);
                    break;
                case EVENT_GRIP_OF_FEAR:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(target, SPELL_GRIP_OF_FEAR, false);
                    events.ScheduleEvent(EVENT_GRIP_OF_FEAR, 12000);
                    break;
                case EVENT_UNLEASHED_TERROR:
                    me->CastSpell(me, SPELL_UNLEASHED_TERROR, false);
                    events.ScheduleEvent(EVENT_UNLEASHED_TERROR, 18000);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_apparition_of_fear : public apparition_ai
{
    npc_apparition_of_fear(Creature* creature) : apparition_ai(creature, true) { }
};

struct npc_apparition_of_terror : public apparition_ai
{
    npc_apparition_of_terror(Creature* creature) : apparition_ai(creature, false) { }
};

struct npc_night_terrors : public ScriptedAI
{
    npc_night_terrors(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        me->CastSpell(me, SPELL_NIGHT_TERRORS_PERIODIC, true);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

struct npc_night_terror_summon : public ScriptedAI
{
    npc_night_terror_summon(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
        events.ScheduleEvent(EVENT_ENVELOPING_DARKNESS, 2000);
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST))
                AttackStart(target);

            return;
        }

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_ENVELOPING_DARKNESS)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    me->CastSpell(target, SPELL_ENVELOPING_DARKNESS, false);

                events.ScheduleEvent(EVENT_ENVELOPING_DARKNESS, 8000);
            }

            break;
        }

        DoMeleeAttackIfReady();
    }
};

class spell_night_terrors_missile : public SpellScript
{
    PrepareSpellScript(spell_night_terrors_missile);

    void SummonNightTerrors(SpellEffIndex /*effIndex*/)
    {
        PreventHitDefaultEffect(EFFECT_0);

        if (!GetExplTargetDest())
            return;

        if (Unit* caster = GetCaster())
            caster->SummonCreature(NPC_NIGHT_TERRORS, GetExplTargetDest()->GetPositionX(), GetExplTargetDest()->GetPositionY(),
                GetExplTargetDest()->GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15000);
    }

    void Register() OVERRIDE
    {
        OnEffectHit += SpellEffectFn(spell_night_terrors_missile::SummonNightTerrors, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
    }
};

class spell_night_terrors_periodic : public AuraScript
{
    PrepareAuraScript(spell_night_terrors_periodic);

    void OnTick(AuraEffect const* /*aurEff*/)
    {
        if (Unit* caster = GetCaster())
        {
            Position pos;
            caster->GetPosition(&pos);
            caster->SummonCreature(NPC_NIGHT_TERROR_SUMMON, pos);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_night_terrors_periodic::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

void AddSC_terrace_of_endless_spring()
{
    new npc_terrace_springtender_ashani();
    RegisterCreatureAI(npc_apparition_of_fear);
    RegisterCreatureAI(npc_apparition_of_terror);
    RegisterCreatureAI(npc_night_terrors);
    RegisterCreatureAI(npc_night_terror_summon);
    RegisterSpellScript(spell_night_terrors_missile);
    RegisterAuraScript(spell_night_terrors_periodic);
}
