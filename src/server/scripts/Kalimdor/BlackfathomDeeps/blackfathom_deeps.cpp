/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "blackfathom_deeps.h"
#include "ScriptedEscortAI.h"
#include "Player.h"

enum Spells
{
    SPELL_BLESSING_OF_BLACKFATHOM                           = 8733,
    SPELL_RAVAGE                                            = 8391,
    SPELL_FROST_NOVA                                        = 865,
    SPELL_FROST_BOLT_VOLLEY                                 = 8398,
    SPELL_TELEPORT_DARNASSUS                                = 9268
};

const Position HomePosition = {-815.817f, -145.299f, -25.870f, 0};

class go_blackfathom_altar : public GameObjectScript
{
public:
    go_blackfathom_altar() : GameObjectScript("go_blackfathom_altar") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) OVERRIDE
    {
        if (!player->HasAura(SPELL_BLESSING_OF_BLACKFATHOM))
            player->AddAura(SPELL_BLESSING_OF_BLACKFATHOM, player);
        return true;
    }
};

class go_blackfathom_fire : public GameObjectScript
{
public:
    go_blackfathom_fire() : GameObjectScript("go_blackfathom_fire") { }

    bool OnGossipHello(Player* /*player*/, GameObject* go) OVERRIDE
    {
        InstanceScript* instance = go->GetInstanceScript();

        if (instance)
        {
            go->SetGoState(GOState::GO_STATE_ACTIVE);
            go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
            instance->SetData(DATA_FIRE, instance->GetData(DATA_FIRE) + 1);
            return true;
        }
        return false;
    }
};

class npc_blackfathom_deeps_event : public CreatureScript
{
public:
    npc_blackfathom_deeps_event() : CreatureScript("npc_blackfathom_deeps_event") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_blackfathom_deeps_eventAI(creature);
    }

    struct npc_blackfathom_deeps_eventAI : public ScriptedAI
    {
        npc_blackfathom_deeps_eventAI(Creature* creature) : ScriptedAI(creature)
        {
            if (creature->IsSummon())
            {
                creature->SetHomePosition(HomePosition);
                AttackPlayer();
            }

            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 ravageTimer;
        uint32 frostNovaTimer;
        uint32 frostBoltVolleyTimer;

        bool Flee;

        void Reset() OVERRIDE
        {
            Flee = false;

            ravageTimer = std::rand() % 8000 + 5000;
            frostNovaTimer = std::rand() % 12000 + 9000;
            frostBoltVolleyTimer = std::rand() % 4000 + 2000;
        }

        void AttackPlayer()
        {
            Map::PlayerList const &PlList = me->GetMap()->GetPlayers();

            if (PlList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
            {
                if (Player* player = i->GetSource())
                {
                    if (player->IsGameMaster())
                        continue;

                    if (player->IsAlive())
                    {
                        me->SetInCombatWith(player);
                        player->SetInCombatWith(me);
                        me->AddThreat(player, 0.0f);
                    }
                }
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            switch (me->GetEntry())
            {
                case NPC_AKU_MAI_SNAPJAW:
                {
                    if (ravageTimer <= diff)
                    {
                        DoCastVictim(SPELL_RAVAGE);
                        ravageTimer = std::rand() % 14000 + 9000;
                    } else ravageTimer -= diff;
                    break;
                }
                case NPC_MURKSHALLOW_SOFTSHELL:
                case NPC_BARBED_CRUSTACEAN:
                {
                    if (!Flee && HealthBelowPct(15))
                    {
                        Flee = true;
                        me->DoFleeToGetAssistance();
                    }
                    break;
                }
                case NPC_AKU_MAI_SERVANT:
                {
                    if (frostBoltVolleyTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_FROST_BOLT_VOLLEY);
                        frostBoltVolleyTimer = std::rand() % 8000 + 5000;
                    }
                    else frostBoltVolleyTimer -= diff;

                    if (frostNovaTimer <= diff)
                    {
                        DoCastAOE(SPELL_FROST_NOVA, false);
                        frostNovaTimer = std::rand() % 30000 + 25000;
                    }
                    else frostNovaTimer -= diff;
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (me->IsSummon()) //we are not a normal spawn.
                if (instance)
                    instance->SetData(DATA_EVENT, instance->GetData(DATA_EVENT) + 1);
        }
    };
};

enum Morridune
{
    SAY_MORRIDUNE_1 = 0,
    SAY_MORRIDUNE_2 = 1
};

class npc_morridune : public CreatureScript
{
public:
    npc_morridune() : CreatureScript("npc_morridune") { }

    struct npc_morriduneAI : public npc_escortAI
    {
        npc_morriduneAI(Creature* creature) : npc_escortAI(creature)
        {
            Talk(SAY_MORRIDUNE_1);
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            Start(false, false, 0);
        }

        void WaypointReached(uint32 waypointId) OVERRIDE
        {
            switch (waypointId)
            {
                case 4:
                    SetEscortPaused(true);
                    me->SetFacingTo(1.775791f);
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    Talk(SAY_MORRIDUNE_2);
                    break;
            }
        }

        void sGossipSelect(Player* player, uint32 /*sender*/, uint32 /*action*/) OVERRIDE
        {
            DoCast(player, SPELL_TELEPORT_DARNASSUS);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_morriduneAI(creature);
    }
};

void AddSC_blackfathom_deeps()
{
    new go_blackfathom_altar();
    new go_blackfathom_fire();
    new npc_blackfathom_deeps_event();
    new npc_morridune();
}
