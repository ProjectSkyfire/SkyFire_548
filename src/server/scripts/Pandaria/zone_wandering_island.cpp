/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"

/*####
# npc_aysa_cloudsinger
####*/
Position const AysaSpawnPos = { 1206.31f, 3507.45f, 85.99f };
Position const AysaJumpPos1 = { 1196.72f, 3492.85f, 90.9836f, 0 };
Position const AysaJumpPos2 = { 1192.29f, 3478.69f, 108.788f, 0 };
Position const AysaJumpPos3 = { 1197.99f, 3460.63f, 103.04f, 0 };
Position const AysaMovePos4 = { 1176.1909f, 3444.8743f, 103.35291f, 0 };
Position const AysaMovePos5 = { 1149.9497f, 3437.1702f, 104.967064f, 0 };

class npc_aysa_cloudsinger : public CreatureScript
{
public:
    npc_aysa_cloudsinger() : CreatureScript("npc_aysa_cloudsinger") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_aysa_cloudsingerAI(creature);
    }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() == 29414)
        {
            creature->MonsterSay("Meet me up in the cave if you would. friend.", Language::LANG_UNIVERSAL, player);
            creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
            creature->GetMotionMaster()->MovePoint(0, AysaSpawnPos);
            return true;
        }
        return false;
    }

    struct npc_aysa_cloudsingerAI : public ScriptedAI
    {
        bool wp1 = false;
        bool wp2 = false;
        bool wp3 = false;
        bool wp4 = false;
        bool wp5 = false;
        bool wp6 = false;
        uint32 JumpTimer = 5000;

        npc_aysa_cloudsingerAI(Creature* creature) : ScriptedAI(creature) { }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (id == 0)
                wp1 = true;

            if (id == 1)
                wp2 = true;

            if (id == 2)
                wp3 = true;

            if (id == 3)
                wp4 = true;

            if (id == 4)
                wp5 = true;

            if (id == 5)
                wp6 = true;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (wp1 == true)
            {
                if (JumpTimer <= diff)
                {
                    me->GetMotionMaster()->MoveJump(AysaJumpPos1, 15.0f, 15.0f, 1);
                    wp1 = false;
                    JumpTimer = 5000;
                }
                else JumpTimer -= diff;
            }

            if (wp2 == true)
            {
                me->GetMotionMaster()->MoveJump(AysaJumpPos2, 15.0f, 25.0f, 2);
                wp2 = false;
            }

            if (wp3 == true)
            {
                me->GetMotionMaster()->MoveJump(AysaJumpPos3, 15.0f, 15.0f, 3);
                wp3 = false;
            }

            if (wp4 == true)
            {
                me->GetMotionMaster()->MovePoint(4, AysaMovePos4);
                wp4 = false;
            }

            if (wp5 == true)
            {
                me->GetMotionMaster()->MovePoint(5, AysaMovePos5);
                wp5 = false;
            }

            if (wp6 == true)
            {
                me->DespawnOrUnsummon(1000);
                wp6 = false;
            }
        }
    };
};

void AddSC_wandering_island()
{
    new npc_aysa_cloudsinger();
}
