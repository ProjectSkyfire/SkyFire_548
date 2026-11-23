/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Razorfen_Downs
SD%Complete: 100
SDComment: Support for Henry Stern(2 recipes)
SDCategory: Razorfen Downs
EndScriptData */

/* ContentData
npc_henry_stern
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "razorfen_downs.h"
#include "Player.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "ScriptSystem.h"

/*###
# npc_henry_stern
####*/

enum Spells
{
    SPELL_TEACHING_GOLDTHORN_TEA                = 13029,
    SPELL_TEACHING_MIGHTY_TROLLS_BLOOD_POTION   = 13030
};

enum Gossips
{
    GOSSIP_COOKING_SKILL_HIGH                   = 1444,
    GOSSIP_COOKING_SKILL_LOW                    = 1501,
    GOSSIP_ALCHEMY_SKILL_HIGH                   = 1442,
    GOSSIP_ALCHEMY_SKILL_LOW                    = 1502
};

class npc_henry_stern : public CreatureScript
{
public:
    npc_henry_stern() : CreatureScript("npc_henry_stern") { }

    struct npc_henry_sternAI : public ScriptedAI
    {
        npc_henry_sternAI(Creature* creature) : ScriptedAI(creature) { }

        void sGossipSelect(Player* player, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            if (action == 0)
            {
                if (player->GetBaseSkillValue(SKILL_COOKING) >= 175)
                {
                    player->PrepareGossipMenu(me, GOSSIP_COOKING_SKILL_HIGH);
                    player->SendPreparedGossip(me);
                    DoCast(player, SPELL_TEACHING_GOLDTHORN_TEA);
                }
                else
                {
                    player->PrepareGossipMenu(me, GOSSIP_COOKING_SKILL_LOW);
                    player->SendPreparedGossip(me);
                }
            }

            if (action == 1)
            {
                if (player->GetBaseSkillValue(SKILL_ALCHEMY) >= 180)
                {
                    player->PrepareGossipMenu(me, GOSSIP_ALCHEMY_SKILL_HIGH);
                    player->SendPreparedGossip(me);
                    DoCast(player, SPELL_TEACHING_MIGHTY_TROLLS_BLOOD_POTION);
                }
                else
                {
                    player->PrepareGossipMenu(me, GOSSIP_ALCHEMY_SKILL_LOW);
                    player->SendPreparedGossip(me);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_henry_sternAI(creature);
    }
};

/*######
## go_gong
######*/

class go_gong : public GameObjectScript
{
public:
    go_gong() : GameObjectScript("go_gong") { }

    bool OnGossipHello(Player* /*player*/, GameObject* go) OVERRIDE
    {
        //basic support, not blizzlike data is missing...
        InstanceScript* instance = go->GetInstanceScript();

        if (instance)
        {
            instance->SetData(DATA_GONG_WAVES, instance->GetData(DATA_GONG_WAVES)+1);
            return true;
        }

        return false;
    }
};

enum TombCreature
{
    SPELL_WEB                   = 745
};

class npc_tomb_creature : public CreatureScript
{
public:
    npc_tomb_creature() : CreatureScript("npc_tomb_creature") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_tomb_creatureAI(creature);
    }

    struct npc_tomb_creatureAI : public ScriptedAI
    {
        npc_tomb_creatureAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 uiWebTimer;

        void Reset() OVERRIDE
        {
            uiWebTimer = urand(5000, 8000);
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //from acid
            if (me->GetEntry() == NPC_TOMB_REAVER)
            {
                if (uiWebTimer <= uiDiff)
                {
                    DoCastVictim(SPELL_WEB);
                    uiWebTimer = urand(7000, 16000);
                } else uiWebTimer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
                instance->SetData(DATA_GONG_WAVES, instance->GetData(DATA_GONG_WAVES)+1);
        }
    };
};

/*######
## npc_belnistrasz: quest "Extinguishing the Idol"
######*/

Position const PosSummonSpawner[3] =
{
    { 2582.789f, 954.3925f, 52.48214f, 3.787364f },
    { 2569.42f,  956.3801f, 52.27323f, 5.427974f },
    { 2570.62f,  942.3934f, 53.7433f,  0.715585f }
};

enum Belnistrasz
{
    EVENT_CHANNEL                = 1,
    EVENT_IDOL_ROOM_SPAWNER      = 2,
    EVENT_PROGRESS               = 3,
    EVENT_COMPLETE               = 4,
    EVENT_FIREBALL               = 5,
    EVENT_FROST_NOVA             = 6,

    NPC_BELNISTRASZ              = 8516,
    PATH_ESCORT                  = 871710,
    POINT_REACH_IDOL             = 17,

    QUEST_EXTINGUISHING_THE_IDOL = 27022,

    SAY_QUEST_ACCEPTED           = 0,
    SAY_EVENT_START              = 1,
    SAY_EVENT_THREE_MIN_LEFT     = 2,
    SAY_EVENT_TWO_MIN_LEFT       = 3,
    SAY_EVENT_ONE_MIN_LEFT       = 4,
    SAY_EVENT_END                = 5,
    SAY_AGGRO                    = 6,
    SAY_WATCH_OUT                = 7,

    SPELL_ARCANE_INTELLECT       = 13326,
    SPELL_FIREBALL               = 9053,
    SPELL_FROST_NOVA             = 11831,
    SPELL_IDOL_SHUTDOWN_VISUAL   = 12774,
    SPELL_IDOM_ROOM_CAMERA_SHAKE = 12816
};

class npc_belnistrasz : public CreatureScript
{
public:
    npc_belnistrasz() : CreatureScript("npc_belnistrasz") { }

    struct npc_belnistraszAI : public ScriptedAI
    {
        npc_belnistraszAI(Creature* creature) : ScriptedAI(creature), eventInProgress(false), channeling(false), eventProgress(0), spawnerCount(0) { }

        void Reset() OVERRIDE
        {
            if (!eventInProgress)
            {
                if (!me->HasAura(SPELL_ARCANE_INTELLECT))
                    DoCast(me, SPELL_ARCANE_INTELLECT);

                channeling = false;
                eventProgress = 0;
                spawnerCount = 0;
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }
        }

        void EnterCombat(Unit* who) OVERRIDE
        {
            if (channeling)
                Talk(SAY_WATCH_OUT, who);
            else
            {
                events.ScheduleEvent(EVENT_FIREBALL, 1000);
                events.ScheduleEvent(EVENT_FROST_NOVA, urand(8000, 12000));
                if (urand(0, 100) > 40)
                    Talk(SAY_AGGRO, who);
            }
        }

        void EnterEvadeMode() OVERRIDE
        {
            // Rooted at the idol, a normal evade would walk him home, which he cannot, and an evading creature gets no
            // AI updates, so the ritual would never move on after the first wave
            if (channeling)
            {
                me->DeleteThreatList();
                me->CombatStop(true);
                return;
            }

            ScriptedAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            // The quest can be abandoned and taken again once he is back
            eventInProgress = false;
            channeling = false;
            events.Reset();
            me->DespawnOrUnsummon(5000);
        }

        void sQuestAccept(Player* /*player*/, Quest const* quest) OVERRIDE
        {
            if (quest->GetQuestId() == QUEST_EXTINGUISHING_THE_IDOL)
            {
                eventInProgress = true;
                Talk(SAY_QUEST_ACCEPTED);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                me->setFaction(FACTION_ESCORT_N_NEUTRAL_ACTIVE);
                me->GetMotionMaster()->MovePath(PATH_ESCORT, false);
            }
        }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type == WAYPOINT_MOTION_TYPE && id == POINT_REACH_IDOL)
            {
                channeling = true;
                events.ScheduleEvent(EVENT_CHANNEL, 2000);
            }
        }

        void CompleteForPlayersAndPutOutFires()
        {
            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (Player* player = itr->GetSource())
                    if (player->IsWithinDistInMap(me, 50.0f) && player->GetQuestStatus(QUEST_EXTINGUISHING_THE_IDOL) == QUEST_STATUS_INCOMPLETE)
                        player->CompleteQuest(QUEST_EXTINGUISHING_THE_IDOL);

            for (uint32 fire : { GO_IDOL_OVEN_FIRE, GO_IDOL_CUP_FIRE, GO_IDOL_MOUTH_FIRE })
            {
                std::list<GameObject*> fires;
                me->GetGameObjectListWithEntryInGrid(fires, fire, 50.0f);
                for (GameObject* go : fires)
                    go->Delete();
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!eventInProgress)
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHANNEL:
                        // He holds the ritual at the idol and leaves the fighting to the players
                        events.CancelEvent(EVENT_FIREBALL);
                        events.CancelEvent(EVENT_FROST_NOVA);
                        me->AttackStop();
                        me->SetReactState(REACT_PASSIVE);
                        me->SetControlled(true, UNIT_STATE_ROOT);
                        Talk(SAY_EVENT_START);
                        DoCast(me, SPELL_IDOL_SHUTDOWN_VISUAL);
                        events.ScheduleEvent(EVENT_IDOL_ROOM_SPAWNER, 100);
                        events.ScheduleEvent(EVENT_PROGRESS, 120000);
                        break;
                    case EVENT_IDOL_ROOM_SPAWNER:
                        if (Creature* creature = me->SummonCreature(NPC_IDOL_ROOM_SPAWNER, PosSummonSpawner[urand(0, 2)], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 4000))
                            creature->AI()->SetData(0, spawnerCount);
                        if (++spawnerCount < 8)
                            events.ScheduleEvent(EVENT_IDOL_ROOM_SPAWNER, 35000);
                        break;
                    case EVENT_PROGRESS:
                        switch (eventProgress)
                        {
                            case 0:
                                Talk(SAY_EVENT_THREE_MIN_LEFT);
                                ++eventProgress;
                                events.ScheduleEvent(EVENT_PROGRESS, 60000);
                                break;
                            case 1:
                                Talk(SAY_EVENT_TWO_MIN_LEFT);
                                ++eventProgress;
                                events.ScheduleEvent(EVENT_PROGRESS, 60000);
                                break;
                            case 2:
                                Talk(SAY_EVENT_ONE_MIN_LEFT);
                                ++eventProgress;
                                events.ScheduleEvent(EVENT_PROGRESS, 60000);
                                break;
                            case 3:
                                events.CancelEvent(EVENT_IDOL_ROOM_SPAWNER);
                                me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                                Talk(SAY_EVENT_END);
                                events.ScheduleEvent(EVENT_COMPLETE, 3000);
                                break;
                        }
                        break;
                    case EVENT_COMPLETE:
                        DoCast(me, SPELL_IDOM_ROOM_CAMERA_SHAKE);
                        // No respawn time and no owner: a unit-owned summon with a respawn time counts as not spawned
                        // here, and he would take the brazier (the quest ender) with him when he leaves
                        if (GameObject* brazier = me->SummonGameObject(GO_BELNISTRASZS_BRAZIER, 2577.196f, 947.0781f, 53.16757f, 2.356195f, 0.0f, 0.0f, 0.9238796f, 0.3826832f, 0))
                            me->RemoveGameObject(brazier, false);
                        CompleteForPlayersAndPutOutFires();
                        me->DespawnOrUnsummon();
                        break;
                    case EVENT_FIREBALL:
                        if (me->HasUnitState(UNIT_STATE_CASTING) || !UpdateVictim())
                        {
                            events.ScheduleEvent(EVENT_FIREBALL, 1000);
                            break;
                        }
                        DoCastVictim(SPELL_FIREBALL);
                        events.ScheduleEvent(EVENT_FIREBALL, 8000);
                        break;
                    case EVENT_FROST_NOVA:
                        if (me->HasUnitState(UNIT_STATE_CASTING) || !UpdateVictim())
                        {
                            events.ScheduleEvent(EVENT_FROST_NOVA, 2000);
                            break;
                        }
                        DoCast(me, SPELL_FROST_NOVA);
                        events.ScheduleEvent(EVENT_FROST_NOVA, 15000);
                        break;
                }
            }

            if (!channeling)
                DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        bool eventInProgress;
        bool channeling;
        uint8 eventProgress;
        uint8 spawnerCount;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_belnistraszAI(creature);
    }
};

class npc_idol_room_spawner : public CreatureScript
{
public:
    npc_idol_room_spawner() : CreatureScript("npc_idol_room_spawner") { }

    struct npc_idol_room_spawnerAI : public ScriptedAI
    {
        npc_idol_room_spawnerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE { }

        void SetData(uint32 /*type*/, uint32 data) OVERRIDE
        {
            float x = me->GetPositionX(), y = me->GetPositionY(), z = me->GetPositionZ(), o = me->GetOrientation();
            if (data < 7)
            {
                Send(me->SummonCreature(NPC_WITHERED_BATTLE_BOAR, x, y, z, o));
                if (data > 0 && o < 4.0f)
                    Send(me->SummonCreature(NPC_WITHERED_BATTLE_BOAR, x, y, z, o));
                Send(me->SummonCreature(NPC_DEATHS_HEAD_GEOMANCER, x + std::cos(o - float(M_PI) / 2) * 2, y + std::sin(o - float(M_PI) / 2) * 2, z, o));
                Send(me->SummonCreature(NPC_WITHERED_QUILGUARD, x + std::cos(o + float(M_PI) / 2) * 2, y + std::sin(o + float(M_PI) / 2) * 2, z, o));
            }
            else if (data == 7)
                Send(me->SummonCreature(NPC_PLAGUEMAW_THE_ROTTING, x, y, z, o));
        }

    private:
        // The waves go for Belnistrasz, so they do not stand idle when nobody is close
        void Send(Creature* attacker)
        {
            if (attacker)
                if (Creature* belnistrasz = me->FindNearestCreature(NPC_BELNISTRASZ, 60.0f))
                    attacker->AI()->AttackStart(belnistrasz);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_idol_room_spawnerAI(creature);
    }
};

void AddSC_razorfen_downs()
{
    new npc_henry_stern();
    new go_gong();
    new npc_tomb_creature();
    new npc_belnistrasz();
    new npc_idol_room_spawner();
}
