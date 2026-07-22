/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"

#define BOXTEXT "Are you sure you want to give this hobo money?"

#define GOSSIP_TRANSIENT1 "Did you see who killed the Furlbrows?"
#define GOSSIP_TRANSIENT2 "Maybe a couple copper will loosen your tongue. Now tell, did you see who killed the Furlbrows?"

#define TALK1 "Listen, $g. I don't want any trouble. ok ? I didn't see who murdered 'em, but I sure heard it! lot of yelling. Human voices... you dig? Now get out of here before I change my mind about beating you up and takin' your shoes."
#define TALK2 "I didn't see who killed 'em, $g, but i got a whiff. Smelled rich, kinda like you. Damn shame too. Furlbrows were a fixture around here. Nice people. always willin' to share a meal or a patch of dirt."
#define TALK3 "Who killed the Furlbrows? I'll tell you who killed the Furlbrows: KING VARIAN WRYNN. THAT'S WHO! And he's killin' the rest of us too. One bum at a time. The only thing I can tell you is that I saw some gnolls leavin' the place a few hours before the law arrived."
#define TALK4 "Between you, me, and the tree, murlocs killed the Furlbrows. Yep. saw 'em with my own two eyes. Think they'd been casin' the joint for days. maybe months. They left in a hurry once they got wind of \"Johnny Law\" and the idiot brigade over there..."

#define TALKAGGRO0 "You'll get nothing out of me!"
#define TALKAGGRO1 "You rich punks are all the same!"
#define TALKAGGRO2 "Maybe i can sell your liver for some gold."
#define TALKAGGRO3 "You got no business here, lout! Beat it!"

#define TALKOOC1 "I miss being robed by the Defias. At least they'd leave you a little something to eat from time to time."
#define TALKOOC2 "Looks like we'll be eating shoe leather for dinner again."

#define RAGAMUFFINTEXT "Does she have boots? Get'em!"

class npc_transient : public CreatureScript
{
public:
    npc_transient() : CreatureScript("npc_transient") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if ((std::rand() % 2) == 1)
            {
                creature->setFaction(14);
                CAST_AI(npc_transient::npc_transientAI, creature->AI())->AttackStart(player);
            }
            else
            {
                CAST_AI(npc_transient::npc_transientAI, creature->AI())->GiveClueCredit(player);
            }
            player->CLOSE_GOSSIP_MENU();
        }
        if (action == GOSSIP_ACTION_INFO_DEF + 2)
        {
            if ((std::rand() % 2) == 1)
            {
                creature->setFaction(14);
                CAST_AI(npc_transient::npc_transientAI, creature->AI())->AttackStart(player);
            }
            else
            {
                CAST_AI(npc_transient::npc_transientAI, creature->AI())->GiveClueCredit(player);
            }
            player->CLOSE_GOSSIP_MENU();
            
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        if (player->GetQuestStatus(26209) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TRANSIENT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(11635, GOSSIP_ICON_CHAT, GOSSIP_TRANSIENT2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2, BOXTEXT, 2, false);
            player->SEND_GOSSIP_MENU(16250, creature->GetGUID());
        }
        else
        {
            player->SEND_GOSSIP_MENU(16250, creature->GetGUID());
        }

        return true;
    }

    struct npc_transientAI : public ScriptedAI
    {
        npc_transientAI(Creature* creature) : ScriptedAI(creature)
        {
            cooldown = 0;
            talkCooldown = 0;
        }

        void GiveClueCredit(Player* player)
        {
            if (player->GetQuestObjectiveCounter(265754) < 1)
            {
                me->MonsterSay(TALK1, Language::LANG_COMMON, me);
                player->KilledMonsterCredit(42414);
                cooldown = 60000;
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                return;
            }
            if (player->GetQuestObjectiveCounter(265755) < 1)
            {
                me->MonsterSay(TALK2, Language::LANG_COMMON, me);
                player->KilledMonsterCredit(42415);
                cooldown = 60000;
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                return;
            }
            if (player->GetQuestObjectiveCounter(265756) < 1)
            {
                me->MonsterSay(TALK3, Language::LANG_COMMON, me);
                player->KilledMonsterCredit(42416);
                cooldown = 60000;
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                return;
            }
            if (player->GetQuestObjectiveCounter(265757) < 1)
            {
                me->MonsterSay(TALK4, Language::LANG_COMMON, me);
                player->KilledMonsterCredit(42417);
                cooldown = 60000;
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                return;
            }
        }

        void Reset() OVERRIDE
        {
            cooldown = 0;
            talkCooldown = std::rand() % 120000 + 60000;
            me->setFaction(7);
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }

        void JustRespawned() OVERRIDE
        {
            Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            switch (std::rand() % 3)
            {
                case 0:
                {
                    me->MonsterSay(TALKAGGRO0, Language::LANG_COMMON, me);
                    break;
                }
                case 1:
                {
                    me->MonsterSay(TALKAGGRO1, Language::LANG_COMMON, me);
                    break;
                }
                case 2:
                {
                    me->MonsterSay(TALKAGGRO2, Language::LANG_COMMON, me);
                    break;
                }
                case 3:
                {
                    me->MonsterSay(TALKAGGRO3, Language::LANG_COMMON, me);
                    break;
                }
                default:
                    break;
            }
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            float x = 0.0f, y = 0.0f;
            GetPositionWithDistInOrientation(me, 5.0f, me->GetOrientation() + (M_PI / 2), x, y);
            if (Creature* rm1 = me->SummonCreature(42413, x, y, 30.0f, M_PI / 2, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5000))
            {
                rm1->SetSpeed(MOVE_RUN, 0.5f);
                rm1->GetMotionMaster()->MoveFollow(me, 0.0f, M_PI / 2);

                if (me->getGender() == GENDER_FEMALE)
                    rm1->MonsterSay(RAGAMUFFINTEXT, Language::LANG_COMMON, me);
            }

            x = 0.0f, y = 0.0f;
            GetPositionWithDistInOrientation(me, 5.0f, me->GetOrientation() + M_PI, x, y);
            if (Creature* rm2 = me->SummonCreature(42413, x, y, 30.0f, M_PI, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5000))
            {
                rm2->SetSpeed(MOVE_RUN, 0.5f);
                rm2->GetMotionMaster()->MoveFollow(me, 0.0f, M_PI);
            }

            x = 0.0f, y = 0.0f;
            GetPositionWithDistInOrientation(me, 5.0f, me->GetOrientation() + (3*M_PI) / 2, x, y);
            if (Creature* rm3 = me->SummonCreature(42413, x, y, 30.0f, (3 * M_PI) / 2, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5000))
            {
                rm3->SetSpeed(MOVE_RUN, 0.5f);
                rm3->GetMotionMaster()->MoveFollow(me, 0.0f, (3 * M_PI) / 2);
            }

            x = 0.0f, y = 0.0f;
            GetPositionWithDistInOrientation(me, 5.0f, me->GetOrientation() + M_PI * 2, x, y);
            if (Creature* rm4 = me->SummonCreature(42413, x, y, 30.0f, M_PI * 2, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5000))
            {
                rm4->SetSpeed(MOVE_RUN, 0.5f);
                rm4->GetMotionMaster()->MoveFollow(me, 0.0f, M_PI * 2);
            }
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            Player* const player = who->ToPlayer();
            if (!player)
                return;

            if (player->GetQuestStatus(26209) != QUEST_STATUS_INCOMPLETE || talkCooldown)
                return;

            if (me->GetEntry() == 42386)
            {
                if ((std::rand() % 2 + 1) == 2)
                {
                    me->MonsterSay(TALKOOC1, Language::LANG_COMMON, me);
                }
                else
                {
                    me->MonsterSay(TALKOOC2, Language::LANG_COMMON, me);
                }
            }

            talkCooldown = std::rand() % 120000 + 60000;

        }
        
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (cooldown)
            {
                if (cooldown <= diff)
                {
                    Reset();
                    cooldown = 0;
                }
                else
                    cooldown -= diff;
            }

            if (talkCooldown)
            {
                if (talkCooldown <= diff)
                {
                    talkCooldown = 0;
                }
                else
                    talkCooldown -= diff;
            }

            if (me->getFaction() == 14)
                DoMeleeAttackIfReady();
        }
    private:
        uint32 cooldown;
        uint32 talkCooldown;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_transientAI(creature);
    }
};


void AddSC_westfall()
{
    new npc_transient();
}
