/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"

const Position huoPos = { 955.11584f, 3604.04f, 200.71805f, 0.0f };
// at 7835
class AreaTrigger_at_temple_of_five_dawns : AreaTriggerScript
{
public:
    AreaTrigger_at_temple_of_five_dawns() : AreaTriggerScript("at_temple_of_five_dawns") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
    {
        if (player->GetQuestStatus(29423) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* huo = player->FindNearestCreature(54958, 15.0f, true)) // Check if Huo is with us.
            {
                if (Creature* master = player->FindNearestCreature(54786, 25.0f, true))
                {
                    master->MonsterSay("Welcome, Huo. The people have missed your warmth.", Language::LANG_UNIVERSAL, player);
                    master->SendPlaySound(27788, true);
                    player->KilledMonsterCredit(61128);
                    huo->GetMotionMaster()->MovePoint(0, huoPos);
                    huo->DeleteCharmInfo();
                    huo->DespawnOrUnsummon(60000);
                    return true;
                }
            }
        }
        return false;
    }
};

class npc_huo : public CreatureScript
{
public:
    npc_huo() : CreatureScript("npc_huo") { }

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* /*quest*/ ) OVERRIDE
    {
        player->CastSpell(player, 128700);
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_huoAI(creature);
    }
    struct npc_huoAI : public ScriptedAI
    {
        npc_huoAI(Creature* creature) : ScriptedAI(creature) { }
    };
};

enum CaveOfMeditation
{
    SPELL_SUMMON_LI_FEI = 102445, //
    //SPELL_LI_FEI_VISUAL_GHOST_AURA = 22650,
    //SPELL_Aysa_Cave_of_Scrolls_Comp_Timer_Aura = 128598,
    SPELL_SEE_QUEST_INVIS_7 = 102396,
    //SPELL_GENERIC_QUEST_INVISIBILITY_7 = 85096,
    //SPELL_AYSA_BAR = 116421,
};


/*####
# npc_li_fei
####*/

Position const LiFeiSpawnPos = { 1126.9323f, 3428.6692f, 105.89006f };
Position const LiFeiMovePos1 = { 1131.3281f, 3437.5156f, 105.45826f };
Position const LiFeiMovePos2 = { 1131.8889f, 3428.2065f, 105.51409f };
Position const LiFeiMovePos3 = { 1130.5278f, 3425.5027f, 105.88636f };
Position const LiFeiMovePos4 = { 1130.5278f, 3425.5027f, 105.88636f };
Position const LiFeiMovePos5 = { 1129.7743f, 3433.302f, 105.531296f };
Position const LiFeiMovePos6 = { 1130.5573f, 3436.087f, 105.483864f };
class npc_li_fei : public CreatureScript
{
    enum EventsAysaCloudsinger
    {
        EVENT_LI_FEI_SPAWN_POS = 1,
        EVENT_LI_FEI_MOVE_POS_1,
        EVENT_LI_FEI_MOVE_POS_2,
        EVENT_LI_FEI_MOVE_POS_3,
        EVENT_LI_FEI_MOVE_POS_4,
        EVENT_LI_FEI_MOVE_POS_5,
        EVENT_LI_FEI_MOVE_POS_6,
        EVENT_LI_FEI_DESPAWN,
    };
public:
    npc_li_fei() : CreatureScript("npc_li_fei") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_li_feiAI(creature);
    }
    struct npc_li_feiAI : public ScriptedAI
    {
        EventMap events;
        npc_li_feiAI(Creature* creature) : ScriptedAI(creature)
        {
            if (me->GetAreaId() == 5848) // Cave of Meditation Area
            {
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(0, LiFeiSpawnPos);

            }
        }
        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
            case 0:
                events.ScheduleEvent(EVENT_LI_FEI_MOVE_POS_1, 10000);
                me->GetMotionMaster()->MovePoint(1, LiFeiMovePos1);
                return;
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
                case EVENT_LI_FEI_MOVE_POS_1:
                    me->GetMotionMaster()->MovePoint(1, LiFeiMovePos1);
                    events.ScheduleEvent(EVENT_LI_FEI_MOVE_POS_2, 10000);
                    break;
                case EVENT_LI_FEI_MOVE_POS_2:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                    me->MonsterTextEmote("Master Li Fei's voice echoes, \"The way of the Tushui... enlightenment through patience and meditation... the principled life.\"", me, false);
                    me->GetMotionMaster()->MovePoint(2, LiFeiMovePos2);
                    events.ScheduleEvent(EVENT_LI_FEI_MOVE_POS_3, 10000);
                    break;
                case EVENT_LI_FEI_MOVE_POS_3:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                    me->MonsterTextEmote("Master Li Fei's voice echoes, \"It is good to see you again, Aysa.You've come with respect, and so I shall give you the answers you seek.\"", me, false);
                    me->GetMotionMaster()->MovePoint(3, LiFeiMovePos3);
                    events.ScheduleEvent(EVENT_LI_FEI_MOVE_POS_4, 10000);
                    break;
                case EVENT_LI_FEI_MOVE_POS_4:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                    me->MonsterTextEmote("Master Li Fei's voice echoes, \"Huo, the spirit of fire, is known for his hunger.He wants for tinder to eat.He needs the caress of the wind to rouse him.\"", me, false);
                    me->GetMotionMaster()->MovePoint(4, LiFeiMovePos4);
                    events.ScheduleEvent(EVENT_LI_FEI_MOVE_POS_5, 10000);
                    break;
                case EVENT_LI_FEI_MOVE_POS_5:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                    me->MonsterTextEmote("Master Li Fei's voice echoes, \"If you find these things and bring them to his cave, on the far side of Wu - Song Village, you will face a challenge within.\"", me, false);
                    me->GetMotionMaster()->MovePoint(5, LiFeiMovePos5);
                    events.ScheduleEvent(EVENT_LI_FEI_MOVE_POS_6, 10000);
                case EVENT_LI_FEI_MOVE_POS_6:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                    me->MonsterTextEmote("Master Li Fei's voice echoes, \"Overcome that challenge, and you shall be graced by Huo's presence. Rekindle his flame, and if your spirit is pure, he shall follow you.\"", me, false);
                    me->GetMotionMaster()->MovePoint(6, LiFeiMovePos6);
                    events.ScheduleEvent(EVENT_LI_FEI_DESPAWN, 10000);
                    break;
                case EVENT_LI_FEI_DESPAWN:
                    me->MonsterTextEmote("Master Li Fei's voice echoes, \"Go, children.We shall meet again very soon.\"", me, false);
                    me->MonsterTextEmote("Master Li Fei fades away.", me, false);
                    me->DespawnOrUnsummon(1000);
                    break;
                }
            }
        }

    };
};

const Position AddSpawnPos[2] =
{
    { 1184.7f, 3448.3f, 102.5f, 0.0f },
    { 1186.7f, 3439.8f, 102.5f, 0.0f },
};

const Position AddPointPos[2] =
{
    { 1145.13f, 3432.88f, 105.268f, 0.0f },
    { 1143.36f, 3437.39f, 104.973f, 0.0f },
};
/*####
# npc_aysa_meditation
####*/
class npc_aysa_meditation : public CreatureScript
{
    enum EventsAysaMeditation
    {
        EVENT_POWER = 1,
        EVENT_ADDS,
        EVENT_FINISHED,
    };

public:
    npc_aysa_meditation() : CreatureScript("npc_aysa_meditation") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_aysa_meditationAI(creature);
    }
    struct npc_aysa_meditationAI : public ScriptedAI
    {
        npc_aysa_meditationAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;
        uint32 Power = 0;
        bool started = false;
        std::vector<Player*> playersParticipate;

        void Reset() override
        {
            Power = 0;
            started = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            Player* const player = who->ToPlayer();
            if (!player)
                return;

            if (player->GetQuestStatus(29414) != QUEST_STATUS_INCOMPLETE || started)
                return;

            if (me->GetAreaId() == 5848) // Cave of Meditation Area
            {
                started = true;
                events.ScheduleEvent(EVENT_POWER, 1000);
                events.ScheduleEvent(EVENT_ADDS, 1000);
            }
        }

        void UpdatePlayerList()
        {
            playersParticipate.clear();

            std::list<Player*> PlayerList;
            me->GetPlayerListInGrid(PlayerList, 20.0f);

            for (auto&& player : PlayerList)
                if (!player->IsGameMaster() && player->GetQuestStatus(29414) == QUEST_STATUS_INCOMPLETE)
                    playersParticipate.push_back(player);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId) {
                case EVENT_POWER:
                {
                    UpdatePlayerList();
                    if (!playersParticipate.empty())
                    {
                        Power++;

                        if (Power == 1)
                        {
                            Talk(0);
                            me->CastSpell(me, SPELL_SUMMON_LI_FEI);
                        }

                        for (auto&& player : playersParticipate)
                        {
                            player->SetPower(POWER_ALTERNATE_POWER, Power);
                            player->SetMaxPower(POWER_ALTERNATE_POWER, 90);
                        }

                        if (Power >= 90)
                            events.ScheduleEvent(EVENT_FINISHED, 100);
                    }
                    events.ScheduleEvent(EVENT_POWER, 1000);
                    break;
                }
                case EVENT_ADDS:
                {
                    for (uint8 i = 0; i < 2; ++i)
                    {
                        if (Creature* troublemaker = me->SummonCreature(61801, AddSpawnPos[i], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                        {
                            troublemaker->GetMotionMaster()->MovePoint(0, AddPointPos[i]);
                        }
                    }
                    events.ScheduleEvent(EVENT_ADDS, 60000);
                    break;
                }
                case EVENT_FINISHED:
                {
                    Talk(1);
                    events.CancelEvent(EVENT_POWER);
                    events.CancelEvent(EVENT_ADDS);
                    for (auto&& player : playersParticipate)
                    {
                        player->CastSpell(player, SPELL_SEE_QUEST_INVIS_7);
                        player->KilledMonsterCredit(54856, 0);
                        player->RemoveAura(116421);
                    }
                    Reset();
                    break;
                }
                default:
                    break;
                }
            }
        }
    };
};

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
    enum EventsAysaCloudsinger
    {
        EVENT_AYSA_JUMP_POS_1 = 1,
        EVENT_AYSA_JUMP_POS_2,
        EVENT_AYSA_JUMP_POS_3,
        EVENT_AYSA_MOVE_POS_4,
        EVENT_AYSA_MOVE_POS_5,
        EVENT_AYSA_DESPAWN,
    };

public:
    npc_aysa_cloudsinger() : CreatureScript("npc_aysa_cloudsinger") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() == 29414)
        {
            creature->MonsterSay("Meet me up in the cave if you would. friend.", Language::LANG_UNIVERSAL, player);
            creature->SendPlaySound(27397, true);
            creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
            creature->GetMotionMaster()->MovePoint(0, AysaSpawnPos);
            return true;
        }
        return false;
    }

    struct npc_aysa_cloudsingerAI : public ScriptedAI
    {
        npc_aysa_cloudsingerAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE)
                return;

            switch (id)
            {
            case 0:
                events.ScheduleEvent(EVENT_AYSA_JUMP_POS_1, 5000);
                return;
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
                case EVENT_AYSA_JUMP_POS_1:
                    me->GetMotionMaster()->MoveJump(AysaJumpPos1, 15.0f, 15.0f, 1);
                    events.ScheduleEvent(EVENT_AYSA_JUMP_POS_2, 2000);
                    break;
                case EVENT_AYSA_JUMP_POS_2:
                    me->GetMotionMaster()->MoveJump(AysaJumpPos2, 15.0f, 25.0f, 2);
                    events.ScheduleEvent(EVENT_AYSA_JUMP_POS_3, 2000);
                    break;
                case EVENT_AYSA_JUMP_POS_3:
                    me->GetMotionMaster()->MoveJump(AysaJumpPos3, 15.0f, 15.0f, 3);
                    events.ScheduleEvent(EVENT_AYSA_MOVE_POS_4, 2000);
                    break;
                case EVENT_AYSA_MOVE_POS_4:
                    me->GetMotionMaster()->MovePoint(4, AysaMovePos4);
                    events.ScheduleEvent(EVENT_AYSA_MOVE_POS_5, 2000);
                    break;
                case EVENT_AYSA_MOVE_POS_5:
                    me->GetMotionMaster()->MovePoint(5, AysaMovePos5);
                    events.ScheduleEvent(EVENT_AYSA_DESPAWN, 4000);
                    break;
                case EVENT_AYSA_DESPAWN:
                    me->DespawnOrUnsummon(1000);
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_aysa_cloudsingerAI(creature);
    }
};

void AddSC_wandering_island()
{
    new AreaTrigger_at_temple_of_five_dawns();
    new npc_huo();
    new npc_li_fei();
    new npc_aysa_meditation();
    new npc_aysa_cloudsinger();
}
