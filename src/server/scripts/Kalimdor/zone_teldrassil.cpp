/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Teldrassil
SD%Complete: 100
SDComment: Quest support: 938
SDCategory: Teldrassil
EndScriptData */

/* ContentData
npc_mist
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedFollowerAI.h"
#include "Player.h"

/*####
# npc_tarindrella
####*/

enum VileTouch
{
    QUEST_VILE_TOUCH = 28727,
    QUEST_SIGNS_OF_THINGS_TO_COME = 28728,

    NPC_GITHYISS = 1994,

    EVENT_CLEANSE_SPIRIT = 1,
    EVENT_ENTANGLING_ROOTS = 2,
    EVENT_SUMMON_NATURES_BITE = 3,

    SPELL_ENTANGLING_ROOTS = 33844,
    SPELL_SUMMON_NATURES_BITE = 92573,
    SPELL_CLEANSE_SPIRIT = 66056,
    SPELL_POISON = 11918

};
#define TARINDRELLA_TEXT_ON_COMPLETE "This totem has been corrupting the eggs! It seems a greater threat looms. The Gnarlpine remain tainted by something most foul."
#define TARINDRELLA_TEXT_SPAWN "You've come to help, $c? Let us stay together for a while."
#define TARINDRELLA_TEXT_ON_KILL "My dear friends... I'm so sorry..."

class npc_tarindrella : public CreatureScript
{
public:
    npc_tarindrella() : CreatureScript("npc_tarindrella") { }

    bool OnQuestReward(Player * player, Creature * creature, Quest const* quest, uint32 /*opt*/) OVERRIDE
    {
        if (player->GetQuestStatus(QUEST_SIGNS_OF_THINGS_TO_COME) == QUEST_STATUS_REWARDED)
            creature->DespawnOrUnsummon();
        return true;
    }
    struct npc_tarindrellaAI : public FollowerAI
    {
        npc_tarindrellaAI(Creature* creature) : FollowerAI(creature) { }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            FollowerAI::MoveInLineOfSight(who);
        }

        void IsSummonedBy(Unit* unit) OVERRIDE
        {
            me->MonsterSay(TARINDRELLA_TEXT_SPAWN, Language::LANG_UNIVERSAL, unit);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if ((victim->GetEntry() == NPC_GITHYISS)) { }
            else
            {
                me->MonsterSay(TARINDRELLA_TEXT_ON_KILL, Language::LANG_UNIVERSAL, me->GetOwner());
            }
        }

        void Reset() OVERRIDE
        {
            events.ScheduleEvent(EVENT_ENTANGLING_ROOTS, 2000);
            events.ScheduleEvent(EVENT_SUMMON_NATURES_BITE, 6000);
            events.ScheduleEvent(EVENT_CLEANSE_SPIRIT, 10000);
        }

        void UpdateFollowerAI(const uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ENTANGLING_ROOTS:
                    {
                        DoCast(me->GetVictim(), SPELL_ENTANGLING_ROOTS);
                        events.ScheduleEvent(EVENT_ENTANGLING_ROOTS, 10000);
                        break;
                    }
                    case EVENT_CLEANSE_SPIRIT:
                    {
                        if (me->HasAura(6751) || me->HasAura(11918))
                        {
                            DoCast(me, SPELL_CLEANSE_SPIRIT);
                            events.ScheduleEvent(EVENT_CLEANSE_SPIRIT, 10000);
                        }
                        else if (me->GetOwner()->HasAura(6751) || me->GetOwner()->HasAura(11918))
                        {
                            DoCast(me->GetOwner(), SPELL_CLEANSE_SPIRIT);
                            events.ScheduleEvent(EVENT_CLEANSE_SPIRIT, 10000);
                        }
                        else
                        {
                            events.ScheduleEvent(EVENT_CLEANSE_SPIRIT, 5000);
                        }
                        break;
                    }
                    case EVENT_SUMMON_NATURES_BITE:
                    {
                        DoCast(me->GetVictim(), SPELL_SUMMON_NATURES_BITE);
                        events.ScheduleEvent(EVENT_SUMMON_NATURES_BITE, 10000);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_tarindrellaAI(creature);
    }
};

/*####
# npc_githyiss
####*/
class npc_githyiss : public CreatureScript
{
public:
    npc_githyiss() : CreatureScript("npc_githyiss") { }

    struct npc_githyissAI : public CreatureAI
    {
        npc_githyissAI(Creature* creature) : CreatureAI(creature) { }

        uint32 PoisonTimer = 0;

        void Reset() OVERRIDE
        {
            PoisonTimer = 1000;
        }

        void JustDied(Unit* killer) OVERRIDE
        {
            if (Creature* pDryad = me->FindNearestCreature(49480, 15.0f, true))
            {
                pDryad->MonsterSay(TARINDRELLA_TEXT_ON_COMPLETE, Language::LANG_UNIVERSAL, killer);
            }
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (PoisonTimer <= diff)
            {
                DoCast(me->GetVictim(), SPELL_POISON);
                PoisonTimer = 15000; //Todo: Get retail timer from combat log
            }
            else
                PoisonTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_githyissAI(creature);
    }
};

/*####
# npc_mist
####*/

enum Mist
{
    SAY_AT_HOME             = 0,
    EMOTE_AT_HOME           = 1,
    QUEST_MIST              = 938,
    NPC_ARYNIA              = 3519,
    FACTION_DARNASSUS       = 79
};

class npc_mist : public CreatureScript
{
public:
    npc_mist() : CreatureScript("npc_mist") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() == QUEST_MIST)
            if (npc_mistAI* pMistAI = CAST_AI(npc_mist::npc_mistAI, creature->AI()))
                pMistAI->StartFollow(player, FACTION_DARNASSUS, quest);

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_mistAI(creature);
    }

    struct npc_mistAI : public FollowerAI
    {
        npc_mistAI(Creature* creature) : FollowerAI(creature) { }

        void Reset() OVERRIDE { }

        void MoveInLineOfSight(Unit* who) OVERRIDE

        {
            FollowerAI::MoveInLineOfSight(who);

            if (!me->GetVictim() && !HasFollowState(STATE_FOLLOW_COMPLETE) && who->GetEntry() == NPC_ARYNIA)
            {
                if (me->IsWithinDistInMap(who, 10.0f))
                {
                    Talk(SAY_AT_HOME, who);
                    DoComplete();
                }
            }
        }

        void DoComplete()
        {
            Talk(EMOTE_AT_HOME);

            Player* player = GetLeaderForFollower();
            if (player && player->GetQuestStatus(QUEST_MIST) == QUEST_STATUS_INCOMPLETE)
                player->GroupEventHappens(QUEST_MIST, me);

            //The follow is over (and for later development, run off to the woods before really end)
            SetFollowComplete();
        }

        //call not needed here, no known abilities
        /*void UpdateFollowerAI(const uint32 Diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }*/
    };

};

void AddSC_teldrassil()
{
    new npc_githyiss();
    new npc_tarindrella();
    new npc_mist();
}
