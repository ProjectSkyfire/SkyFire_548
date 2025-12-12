/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* Script for Quest 14098 "Evacuate the Merchant Square"
* Based on TrinityCore implementation
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "TemporarySummon.h"

enum GilneasChapter1
{
    // Creatures
    NPC_EVACUATION_STALKER_FIRST = 35830,
    NPC_EVACUATION_STALKER_NEAR  = 35010,
    NPC_EVACUATION_STALKER_FAR   = 35011,
    NPC_FRIGHTENED_CITIZEN       = 34981,
    NPC_RAGING_WORGEN            = 34884,  // Rampaging Worgen that sometimes spawns

    // Events
    EVENT_TALK_FRIGHTENED        = 1,
    EVENT_CHECK_FOR_WORGEN       = 2,
    EVENT_MOVE_TO_NEAR_STALKER   = 3,
    EVENT_MOVE_TO_FAR_STALKER    = 4,
    EVENT_FRIGHTENED_DESPAWN     = 5,

    // Points
    POINT_STALKER_FIRST          = 1,
    POINT_STALKER_NEAR           = 2,
    POINT_STALKER_FAR            = 3,

    // Texts
    SAY_FRIGHTENED_CITIZEN_RESCUE = 0,
    SAY_FRIGHTENED_CITIZEN_WORGEN = 1  // "There's one after me!"
};

/*######
## npc_frightened_citizen - Quest 14098 "Evacuate the Merchant Square"
######*/

class npc_frightened_citizen : public CreatureScript
{
public:
    npc_frightened_citizen() : CreatureScript("npc_frightened_citizen") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_frightened_citizenAI(creature);
    }

    struct npc_frightened_citizenAI : public ScriptedAI
    {
        npc_frightened_citizenAI(Creature* creature) : ScriptedAI(creature), m_shouldSpawnWorgen(false), m_worgenSpawnX(0.0f), m_worgenSpawnY(0.0f), m_worgenSpawnZ(0.0f) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);

            // Store summoner location for worgen spawn
            // We'll spawn the worgen at the door position (summoner's position) after citizen moves out
            m_worgenSpawnX = me->GetPositionX();
            m_worgenSpawnY = me->GetPositionY();
            m_worgenSpawnZ = me->GetPositionZ();
            
            // Randomly decide if worgen should spawn (50% chance)
            m_shouldSpawnWorgen = (std::rand() % 2 == 0);

            // Find the nearest evacuation stalker and move towards it
            // Try finding within 20.0f first, then expand search if not found
            Creature* stalkerFirst = me->FindNearestCreature(NPC_EVACUATION_STALKER_FIRST, 20.0f);
            if (!stalkerFirst)
                stalkerFirst = me->FindNearestCreature(NPC_EVACUATION_STALKER_FIRST, 50.0f);
            
            if (stalkerFirst)
            {
                me->GetMotionMaster()->MovePoint(POINT_STALKER_FIRST, stalkerFirst->GetPositionX(), stalkerFirst->GetPositionY(), stalkerFirst->GetPositionZ());
            }
            else
            {
                // If no stalker found, trigger the talk event immediately to give quest credit anyway
                events.ScheduleEvent(EVENT_TALK_FRIGHTENED, 500);
            }
        }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case POINT_STALKER_FIRST:
                    // Spawn worgen at door position if we decided to spawn it
                    if (m_shouldSpawnWorgen)
                    {
                        // Spawn worgen at the door position (where citizen was summoned)
                        me->SummonCreature(NPC_RAGING_WORGEN, m_worgenSpawnX, m_worgenSpawnY, m_worgenSpawnZ, 0.0f, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000);
                    }
                    events.ScheduleEvent(EVENT_TALK_FRIGHTENED, 1000);
                    break;
                case POINT_STALKER_NEAR:
                    events.ScheduleEvent(EVENT_MOVE_TO_FAR_STALKER, 1);
                    break;
                case POINT_STALKER_FAR:
                    events.ScheduleEvent(EVENT_FRIGHTENED_DESPAWN, 1);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TALK_FRIGHTENED:
                    {
                        // Give quest credit and make the citizen talk
                        // Try to get player from summoner, or find nearest player with quest
                        Player* player = NULL;
                        
                        if (TempSummon* summon = me->ToTempSummon())
                        {
                            if (Unit* summoner = summon->GetSummoner())
                            {
                                // If summoner is a player, use it
                                if (Player* summonerPlayer = summoner->ToPlayer())
                                    player = summonerPlayer;
                            }
                        }
                        
                        // If no player from summoner, find nearest player with quest
                        if (!player)
                        {
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 50.0f);
                            
                            for (Player* plr : playerList)
                            {
                                if (plr->IsAlive() && plr->GetQuestStatus(14098) == QUEST_STATUS_INCOMPLETE)
                                {
                                    player = plr;
                                    break;
                                }
                            }
                        }
                        
                        if (player)
                        {
                            // Give quest credit for creature 35830 (Evacuation Stalker First)
                            player->KilledMonsterCredit(NPC_EVACUATION_STALKER_FIRST, 0);
                            
                            // Check if a Raging Worgen spawned nearby (within 30 yards)
                            // Schedule a check for worgen first, then talk
                            events.ScheduleEvent(EVENT_CHECK_FOR_WORGEN, 500);
                        }
                        
                        events.ScheduleEvent(EVENT_MOVE_TO_NEAR_STALKER, 3000);
                        break;
                    }
                    case EVENT_CHECK_FOR_WORGEN:
                    {
                        // Check if a Raging Worgen spawned nearby
                        Creature* worgen = me->FindNearestCreature(NPC_RAGING_WORGEN, 30.0f);
                        
                        Player* player = NULL;
                        if (TempSummon* summon = me->ToTempSummon())
                        {
                            if (Unit* summoner = summon->GetSummoner())
                            {
                                if (Player* summonerPlayer = summoner->ToPlayer())
                                    player = summonerPlayer;
                            }
                        }
                        
                        if (!player)
                        {
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 50.0f);
                            
                            for (Player* plr : playerList)
                            {
                                if (plr->IsAlive() && plr->GetQuestStatus(14098) == QUEST_STATUS_INCOMPLETE)
                                {
                                    player = plr;
                                    break;
                                }
                            }
                        }
                        
                        if (player)
                        {
                            // If worgen found, say "There's one after me!" otherwise say normal text
                            if (worgen && worgen->IsAlive())
                                Talk(SAY_FRIGHTENED_CITIZEN_WORGEN, player);
                            else
                                Talk(SAY_FRIGHTENED_CITIZEN_RESCUE, player);
                        }
                        break;
                    }
                    case EVENT_MOVE_TO_NEAR_STALKER:
                    {
                        // Try to find far stalker first, then near stalker
                        if (Creature* stalker = me->FindNearestCreature(NPC_EVACUATION_STALKER_FAR, 50.0f))
                        {
                            me->GetMotionMaster()->MovePoint(POINT_STALKER_FAR, stalker->GetPositionX(), stalker->GetPositionY(), stalker->GetPositionZ());
                        }
                        else if (Creature* stalker = me->FindNearestCreature(NPC_EVACUATION_STALKER_NEAR, 100.0f))
                        {
                            me->GetMotionMaster()->MovePoint(POINT_STALKER_NEAR, stalker->GetPositionX(), stalker->GetPositionY(), stalker->GetPositionZ());
                        }
                        break;
                    }
                    case EVENT_MOVE_TO_FAR_STALKER:
                    {
                        if (Creature* stalker = me->FindNearestCreature(NPC_EVACUATION_STALKER_FAR, 500.0f))
                        {
                            me->GetMotionMaster()->MovePoint(POINT_STALKER_FAR, stalker->GetPositionX(), stalker->GetPositionY(), stalker->GetPositionZ());
                        }
                        break;
                    }
                    case EVENT_FRIGHTENED_DESPAWN:
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }
                    default:
                        break;
                }
            }
        }

    private:
        EventMap events;
        bool m_shouldSpawnWorgen;
        float m_worgenSpawnX;
        float m_worgenSpawnY;
        float m_worgenSpawnZ;
    };
};

void AddSC_gilneas_chapter_1()
{
    new npc_frightened_citizen();
}

