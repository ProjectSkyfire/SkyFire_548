/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Hinterlands
SD%Complete: 100
SDComment: Quest support: 836
SDCategory: The Hinterlands
EndScriptData */

/* ContentData
npc_00x09hl
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "Player.h"

/*######
## npc_00x09hl
######*/

enum eOOX
{
    SAY_OOX_START           = 0,
    SAY_OOX_AGGRO           = 1,
    SAY_OOX_AMBUSH          = 3,
    SAY_OOX_AMBUSH_REPLY    = 4,
    SAY_OOX_END             = 5,

    QUEST_RESQUE_OOX_09     = 836,

    NPC_MARAUDING_OWL       = 7808,
    NPC_VILE_AMBUSHER       = 7809,

    FACTION_ESCORTEE_A      = 774,
    FACTION_ESCORTEE_H      = 775
};

class npc_00x09hl : public CreatureScript
{
public:
    npc_00x09hl() : CreatureScript("npc_00x09hl") { }

    bool OnQuestAccept(Player* player, Creature* creature, const Quest* quest) override
    {
        if (quest->GetQuestId() == QUEST_RESQUE_OOX_09)
        {
            creature->SetStandState(UNIT_STAND_STATE_STAND);

            if (player->GetTeam() == ALLIANCE)
                creature->setFaction(FACTION_ESCORTEE_A);
            else if (player->GetTeam() == HORDE)
                creature->setFaction(FACTION_ESCORTEE_H);

            creature->AI()->Talk(SAY_OOX_START, player);

            if (npc_00x09hlAI* pEscortAI = CAST_AI(npc_00x09hl::npc_00x09hlAI, creature->AI()))
                pEscortAI->Start(false, false, player->GetGUID(), quest);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_00x09hlAI(creature);
    }

    struct npc_00x09hlAI : public npc_escortAI
    {
        npc_00x09hlAI(Creature* creature) : npc_escortAI(creature) { }

        void Reset() override { }

        void WaypointReached(uint32 waypointId) override
        {
            switch (waypointId)
            {
                case 26:
                    Talk(SAY_OOX_AMBUSH);
                    break;
                case 43:
                    Talk(SAY_OOX_AMBUSH);
                    break;
                case 64:
                    Talk(SAY_OOX_END);
                    if (Player* player = GetPlayerForEscort())
                        player->GroupEventHappens(QUEST_RESQUE_OOX_09, me);
                    break;
            }
        }

        void WaypointStart(uint32 uiPointId) override
        {
            switch (uiPointId)
            {
                case 27:
                    for (uint8 i = 0; i < 3; ++i)
                    {
                        const Position src = {147.927444f, -3851.513428f, 130.893f, 0};
                        Position dst;
                        me->GetRandomPoint(src, 7.0f, dst);
                        DoSummon(NPC_MARAUDING_OWL, dst, 25000, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                    }
                    break;
                case 44:
                    for (uint8 i = 0; i < 3; ++i)
                    {
                        const Position src = {-141.151581f, -4291.213867f, 120.130f, 0};
                        Position dst;
                        me->GetRandomPoint(src, 7.0f, dst);
                        me->SummonCreature(NPC_VILE_AMBUSHER, dst, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 25000);
                    }
                    break;
            }
        }

        void EnterCombat(Unit* who) override
        {
            if (who->GetEntry() == NPC_MARAUDING_OWL || who->GetEntry() == NPC_VILE_AMBUSHER)
                return;

            Talk(SAY_OOX_AGGRO);
        }

        void JustSummoned(Creature* summoned) override
        {
            summoned->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
        }
    };
};

void AddSC_hinterlands()
{
    new npc_00x09hl();
}
