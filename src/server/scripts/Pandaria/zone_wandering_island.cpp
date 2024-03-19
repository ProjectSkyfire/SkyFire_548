/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "Spell.h"

const Position balloonPos1 = { 915.123, 4564.1523, 231.37447 };
const Position balloonPos2 = { 922.3584, 4567.7495, 234.48523 };
const Position balloonPos3 = { 954.0022, 4578.619, 230.6169 };
const Position balloonPos4 = { 1005.63586, 4599.781, 219.48938 };
const Position balloonPos5 = { 1035.7178, 4621.3955, 205.60277 };
const Position balloonPos6 = { 1056.7085, 4664.024, 186.15259 };
const Position balloonPos7 = { 1079.8335, 4795.34, 157.87029 };
const Position balloonPos8 = { 1091.337, 4865.452, 144.9579 };
const Position balloonPos9 = { 1090.1652, 4928.4194, 138.01312 };
const Position balloonPos10 = { 1062.7473, 5065.7837, 137.5515 };
const Position balloonPos11 = { 992.0422, 5163.364, 137.56874 };
const Position balloonPos12 = { 885.60614, 5206.965, 135.15846 };
const Position balloonPos13 = { 779.9042, 5208.6396, 135.60544 };
const Position balloonPos14 = { 736.7593, 5192.4, 137.05084 };
const Position balloonPos15 = { 649.841, 5145.8667, 141.09795 };
const Position balloonPos16 = { 623.2541, 5131.7183, 142.2216 };
const Position balloonPos17 = { 560.43494, 5053.6377, 132.38977 };
const Position balloonPos18 = { 485.70917, 4949.409, 125.14896 };
const Position balloonPos19 = { 429.59842, 4820.912, 110.39427 };
const Position balloonPos20 = { 305.63647, 4435.072, 79.58694 };
const Position balloonPos21 = { 156.1306, 4266.2583, 116.03974 };
const Position balloonPos22 = { 112.64921, 4032.78, 125.91718 };
const Position balloonPos23 = { 203.35933, 3835.7214, 136.13402 };
const Position balloonPos24 = { 395.93735, 3764.5327, 160.51057 };
const Position balloonPos25 = { 744.496, 3664.6455, 193.9989 };
class npc_hot_air_balloon_vehicle : public CreatureScript
{
    enum airBalloonEvents
    {
        EVENT_BALOON_MOVE_POS_1 = 1,
        EVENT_BALLON_MOVE_POS_2  = 2,
        EVENT_BALLON_AYSA_TALK_1 = 3,
        EVENT_BALLON_JI_TALK_2 = 4,
        EVENT_BALLOON_AYSA_TALK_2 = 5,
        EVENT_BALLON_JI_TALK_3 = 6,
        EVENT_BALLOON_AYSA_TALK_3 = 8,
        EVENT_BALLOON_AYSA_YELL_1 = 9,
        EVENT_BALLOON_AYSA_YELL_2 = 10,
        EVENT_BALLOON_SHENZINSU_TALK_1 = 11,
        EVENT_BALLOON_SHENZINSU_TALK_2 =12,
        EVENT_BALLOON_SHENZINSU_TALK_3 = 13,
        EVENT_BALLOON_SHENZINSU_TALK_4 = 14,
        EVENT_BALLOON_AYSA_YELL_3 = 15,
        EVENT_BALLOON_SHENZINSU_TALK_5 = 16,
        EVENT_BALLOON_AYSA_YELL_4 = 17,
        EVENT_BALLOON_SHENZINSU_TALK_6 = 18,
        EVENT_BALLON_JI_TALK_4 = 19,
        EVENT_BALLON_JI_TALK_5 = 20,
        EVENT_BALLON_AYSA_TALK_4 = 21,
        EVENT_BALLOON_JI_TALK_6 = 22,
        EVENT_BALLON_AYSA_TALK_5 = 23,
        EVENT_BALLOON_JI_TALK_7 = 24,
        EVENT_BALLON_AYSA_TALK_6 = 25,
        EVENT_BALLON_AYSA_TALK_7 = 26,
        EVENT_BALLON_DESPAWN = 27
    };
public:
    npc_hot_air_balloon_vehicle() : CreatureScript("npc_hot_air_balloon_vehicle") { }

    struct npc_hot_air_balloon_vehicleAI : public CreatureAI
    {
        EventMap events;
        npc_hot_air_balloon_vehicleAI(Creature* creature) : CreatureAI(creature) { }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
            case 0:
            {
                events.ScheduleEvent(EVENT_BALOON_MOVE_POS_1, 1000);
                break;
            }
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
                    case EVENT_BALOON_MOVE_POS_1:
                    {
                        me->GetMotionMaster()->MovePoint(1, balloonPos1);
                        events.ScheduleEvent(EVENT_BALLON_MOVE_POS_2, 1000);
                        break;
                    }
                    case EVENT_BALLON_MOVE_POS_2:
                    {
                        me->GetMotionMaster()->MovePoint(2, balloonPos2);
                        events.ScheduleEvent(EVENT_BALLON_AYSA_TALK_1, 2000);
                        break;
                    }
                    case EVENT_BALLON_AYSA_TALK_1:
                    {
                        me->GetMotionMaster()->MovePoint(3, balloonPos3);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterSay("...Ji, were in the Wood of Staves. You know where Master Shang is now.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27431, true);
                        }
                        events.ScheduleEvent(EVENT_BALLON_JI_TALK_2, 8000);
                        break;
                    }
                    case EVENT_BALLON_JI_TALK_2:
                    {
                        me->GetMotionMaster()->MovePoint(4, balloonPos4);
                        if (Creature* ji = me->FindNearestCreature(56663, 15.0f, true))
                        {
                            ji->MonsterSay("Bah, let a pandaren hope, would you? I'm going to miss the old man.", Language::LANG_UNIVERSAL, ji);
                            ji->SendPlaySound(27298, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_AYSA_TALK_2, 12000);
                        break;
                    }
                    case EVENT_BALLOON_AYSA_TALK_2:
                    {
                        me->GetMotionMaster()->MovePoint(5, balloonPos5);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterSay("Ji, be respectful when we speak to Shen-zin Su.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27432, true);
                        }
                        events.ScheduleEvent(EVENT_BALLON_JI_TALK_3, 4000);
                        break;
                    }
                    case EVENT_BALLON_JI_TALK_3:
                    {
                        me->GetMotionMaster()->MovePoint(6, balloonPos6);
                        if (Creature* ji = me->FindNearestCreature(56663, 15.0f, true))
                        {
                            ji->MonsterSay("When am I not respectful? You hurt me, Aysa.", Language::LANG_UNIVERSAL, ji);
                            ji->SendPlaySound(27299, true);
                        }
                        
                        events.ScheduleEvent(EVENT_BALLOON_AYSA_TALK_3, 5000);
                        break;
                    }
                    case EVENT_BALLOON_AYSA_TALK_3:
                    {
                        me->GetMotionMaster()->MovePoint(7, balloonPos7);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterSay("I might if you embarrass us.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27433, true);
                        }

                        events.ScheduleEvent(EVENT_BALLOON_AYSA_YELL_1, 8000);
                        break;
                    }
                    case EVENT_BALLOON_AYSA_YELL_1:
                    {
                        me->GetMotionMaster()->MovePoint(8, balloonPos8);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterYell("Shen-zin Su, we are the descendants of Liu Lang. We've sensed your pain, and we want to help.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27434, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_AYSA_YELL_2, 12000);
                        break;
                    }
                    case EVENT_BALLOON_AYSA_YELL_2:
                    {
                        me->GetMotionMaster()->MovePoint(9, balloonPos9);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterYell("What ails you Shen-zin Su? What can we do?", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27435, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_SHENZINSU_TALK_1, 7000);
                        break;
                    }
                    case EVENT_BALLOON_SHENZINSU_TALK_1:
                    {
                        me->GetMotionMaster()->MovePoint(10, balloonPos10);
                        if (Creature* shenzinsu = me->FindNearestCreature(57769, 90.0f, true))
                        {
                            shenzinsu->MonsterSay("I am in pain, but it warms my heart that Liu Lang's grandchildren have not forgotten me.", Language::LANG_UNIVERSAL, shenzinsu);
                            shenzinsu->SendPlaySound(27435, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_SHENZINSU_TALK_2, 15000);
                        break;
                    }
                    case EVENT_BALLOON_SHENZINSU_TALK_2:
                    {
                        me->GetMotionMaster()->MovePoint(11, balloonPos11);
                        if (Creature* shenzinsu = me->FindNearestCreature(57769, 90.0f, true))
                        {
                            shenzinsu->MonsterSay("There is a thorn in my side.  I cannot remove it.", Language::LANG_UNIVERSAL, shenzinsu);
                            shenzinsu->SendPlaySound(27823, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_SHENZINSU_TALK_3, 14000);
                        break;
                    }
                    case EVENT_BALLOON_SHENZINSU_TALK_3:
                    {
                        me->GetMotionMaster()->MovePoint(12, balloonPos12);
                        if (Creature* shenzinsu = me->FindNearestCreature(57769, 90.0f, true))
                        {
                            shenzinsu->MonsterSay("The pain is unbearable, and I can no longer swim straight.", Language::LANG_UNIVERSAL, shenzinsu);
                            shenzinsu->SendPlaySound(27824, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_SHENZINSU_TALK_4, 15000);
                        break;
                    }
                    case EVENT_BALLOON_SHENZINSU_TALK_4:
                    {
                        me->GetMotionMaster()->MovePoint(13, balloonPos13);
                        if (Creature* shenzinsu = me->FindNearestCreature(57769, 90.0f, true))
                        {
                            shenzinsu->MonsterSay("Please grandchildren, can you remove this thorn?  I cannot do so on my own.", Language::LANG_UNIVERSAL, shenzinsu);
                            shenzinsu->SendPlaySound(27825, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_AYSA_YELL_3, 15000);
                        break;
                    }
                    case EVENT_BALLOON_AYSA_YELL_3:
                    {
                        me->GetMotionMaster()->MovePoint(14, balloonPos14);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterYell("Of course, Shen-zin Su!  But your shell is large, and I do not know where this thorn could be.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27436, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_SHENZINSU_TALK_5, 7000);
                        break;
                    }
                    case EVENT_BALLOON_SHENZINSU_TALK_5:
                    {
                        me->GetMotionMaster()->MovePoint(15, balloonPos15);
                        if (Creature* shenzinsu = me->FindNearestCreature(57769, 90.0f, true))
                        {
                            shenzinsu->MonsterSay("It is in the forest where your feet do not walk.  Continue along the mountains and you will find it.", Language::LANG_UNIVERSAL, shenzinsu);
                            shenzinsu->SendPlaySound(27826, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_AYSA_YELL_4, 18000);
                        break;
                    }
                    case EVENT_BALLOON_AYSA_YELL_4:
                    {
                        me->GetMotionMaster()->MovePoint(16, balloonPos16);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterYell("We will find it, and we will remove it.  You have our word!", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27437, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_SHENZINSU_TALK_6, 8000);
                        break;
                    }
                    case EVENT_BALLOON_SHENZINSU_TALK_6:
                    {
                        me->GetMotionMaster()->MovePoint(17, balloonPos17);
                        if (Creature* shenzinsu = me->FindNearestCreature(57769, 90.0f, true))
                        {
                            shenzinsu->MonsterSay("Thank you, grandchildren.", Language::LANG_UNIVERSAL, shenzinsu);
                            shenzinsu->SendPlaySound(27827, true);
                        }
                        events.ScheduleEvent(EVENT_BALLON_JI_TALK_4, 5000);
                        break;
                    }
                    case EVENT_BALLON_JI_TALK_4:
                    {
                        me->SetSpeed(MOVE_RUN, 4.0f);
                        me->GetMotionMaster()->MovePoint(18, balloonPos18);
                        if (Creature* ji = me->FindNearestCreature(56663, 15.0f, true))
                        {
                            ji->MonsterSay("A thorn?  And I left my tweezers at home.", Language::LANG_UNIVERSAL, ji);
                            ji->SendPlaySound(27300, true);
                        }
                        events.ScheduleEvent(EVENT_BALLON_JI_TALK_5, 7000);
                        break;
                    }
                    case EVENT_BALLON_JI_TALK_5:
                    {
                        me->GetMotionMaster()->MovePoint(19, balloonPos19);
                        if (Creature* ji = me->FindNearestCreature(56663, 15.0f, true))
                        {
                            ji->MonsterSay("How could such a thing cause pain to something so large?", Language::LANG_UNIVERSAL, ji);
                            ji->SendPlaySound(27301, true);
                        }
                        events.ScheduleEvent(EVENT_BALLON_AYSA_TALK_4, 7000);
                        break;
                    }
                    case EVENT_BALLON_AYSA_TALK_4:
                    {
                        me->GetMotionMaster()->MovePoint(20, balloonPos20);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterSay("We'll know soon enough.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27438, true);
                        }
                        events.ScheduleEvent(EVENT_BALLOON_JI_TALK_6, 14000);
                        break;
                    }
                    case EVENT_BALLOON_JI_TALK_6:
                    {
                        me->GetMotionMaster()->MovePoint(21, balloonPos21);
                        if (Creature* ji = me->FindNearestCreature(56663, 15.0f, true))
                        {
                            ji->MonsterSay("Are you seeing what I'm seeing?!  Is that a boat?!", Language::LANG_UNIVERSAL, ji);
                            ji->SendPlaySound(27302, true);
                        }

                        events.ScheduleEvent(EVENT_BALLON_AYSA_TALK_5, 6000);
                        break;
                    }
                    case EVENT_BALLON_AYSA_TALK_5:
                    {
                        me->GetMotionMaster()->MovePoint(22, balloonPos22);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterSay("It is a boat... a whole airship!  That's a bigger thorn than I was expecting.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27439, true);
                        }

                        events.ScheduleEvent(EVENT_BALLOON_JI_TALK_7, 8000);
                        break;
                    }
                    case EVENT_BALLOON_JI_TALK_7:
                    {
                        me->GetMotionMaster()->MovePoint(23, balloonPos23);
                        if (Creature* ji = me->FindNearestCreature(56663, 15.0f, true))
                        {
                            ji->MonsterSay("And those aren't pandaren down there.  They've got no fur.", Language::LANG_UNIVERSAL, ji);
                            ji->SendPlaySound(27303, true);
                        }
                        events.ScheduleEvent(EVENT_BALLON_AYSA_TALK_6, 7000);
                        break;
                    }
                    case EVENT_BALLON_AYSA_TALK_6:
                    {
                        me->GetMotionMaster()->MovePoint(24, balloonPos24);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterSay("Someone has crashed into our island.  Removing this thorn may be more complicated than we thought.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27440, true);
                        }

                        events.ScheduleEvent(EVENT_BALLON_AYSA_TALK_7, 8000);
                        break;
                    }
                    case EVENT_BALLON_AYSA_TALK_7:
                    {
                        me->GetMotionMaster()->MovePoint(25, balloonPos25);
                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->MonsterSay("We should let Elder Shaopai know and then plan our next move.", Language::LANG_UNIVERSAL, aysa);
                            aysa->SendPlaySound(27441, true);
                        }

                        events.ScheduleEvent(EVENT_BALLON_DESPAWN, 20000);
                        break;
                    }
                    case EVENT_BALLON_DESPAWN:
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 15.0f);

                        for (auto&& player : playerList)
                        {
                            player->KilledMonsterCredit(56378);
                            player->KilledMonsterCredit(55939);
                        }

                        if (Creature* aysa = me->FindNearestCreature(56661, 15.0f, true))
                        {
                            aysa->DespawnOrUnsummon();
                        }
                        if (Creature* ji = me->FindNearestCreature(56663, 15.0f, true))
                        {
                            ji->DespawnOrUnsummon();
                        }
                        me->DespawnOrUnsummon();
                        break;
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_hot_air_balloon_vehicleAI(creature);
    }
};

class npc_hot_air_balloon : public CreatureScript
{
public:
    npc_hot_air_balloon() : CreatureScript("npc_hot_air_balloon") { }

    struct npc_hot_air_balloonAI : public CreatureAI
    {
        npc_hot_air_balloonAI(Creature* creature) : CreatureAI(creature) { }

        void OnSpellClick(Unit* Clicker, bool& /*result*/) OVERRIDE
        {
            if (Creature* vehicle = me->SummonCreature(55649, me->GetHomePosition(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN, 500000))
            {
                Clicker->ToPlayer()->EnterVehicle(vehicle, 0);

                if (Creature* ji = me->SummonCreature(56663, me->GetHomePosition(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN, 500000))
                {
                    ji->EnterVehicle(vehicle, 1);
                    ji->MonsterSay("$n, where's Master Shang?", Language::LANG_UNIVERSAL, Clicker);
                    ji->SendPlaySound(27297, true);
                }

                if (Creature* aysa = me->SummonCreature(56661, me->GetHomePosition(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN, 500000))
                    aysa->EnterVehicle(vehicle, 2);


                vehicle->GetMotionMaster()->MovePoint(0, me->GetHomePosition());
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_hot_air_balloonAI(creature);
    }
};

const Position masterShangXiWoodsPos1 = { 871.0573f, 4460.548f, 241.33667f };
const Position masterShangXiWoodsPos2 = { 868.00696f, 4464.8384f, 241.60161f };
const Position masterShangXiWoodsPos3 = { 869.67883f, 4467.752f, 241.66815f };
const Position masterShangXiWoodsPos4 = { 872.2448f, 4467.272f, 241.60721f };
const Position masterShangXiWoodsPos5 = { 872.7934f, 4465.126f, 241.33447f };
const Position masterShangXiWoodsPos6 = { 874.205f, 4464.75f, 241.35117f };
class npc_master_shang_xi_wood_of_staves : public CreatureScript
{
    enum masterShangXiWoodsEvents
    {
        EVENT_MASTER_SHANG_XI_WOODS_TALK1 = 1,
        EVENT_MASTER_SHANG_XI_WOODS_TALK2 = 2,
        EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS1 = 3,
        EVENT_MASTER_SHANG_XI_WOODS_TALK3 = 4,
        EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS2 = 5,
        EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS3 = 6,
        EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS4 = 7,
        EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS5 = 8,
        EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS6 = 9,
        EVENT_MASTER_SHANG_XI_WOODS_TALK4 = 10,
        EVENT_MASTER_SHANG_XI_WOODS_TALK5 = 11,
        EVENT_MASTER_SHANG_XI_WOODS_TALK6 = 12,
        EVENT_MASTER_SHANG_XI_WOODS_CREDIT = 13,
        EVENT_MASTER_SHANG_XI_WOODS_RESET = 14,
        EVENT_MASTER_SHANG_XI_WOODS_SET_FACING1 = 15,
        EVENT_MASTER_SHANG_XI_WOODS_KNEEL = 16,
    };
public:
    npc_master_shang_xi_wood_of_staves() : CreatureScript("npc_master_shang_xi_wood_of_staves") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() == 29790) // Passing Wisdom
        {
            creature->SetWalk(true);
            creature->GetMotionMaster()->MovePoint(0, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ());
            return true;
        }
        return false;
    }

    struct npc_master_shang_xi_wood_of_stavesAI : public CreatureAI
    {
        EventMap events;
        npc_master_shang_xi_wood_of_stavesAI(Creature* creature) : CreatureAI(creature) { }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
            case 0:
            {
                events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_TALK1, 1000);
                break;
            }
            default:
                break;
            }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MASTER_SHANG_XI_WOODS_TALK1:
                    {
                        me->MonsterSay("For 3,000 years, we have passed the knowledge of our people down. Elder to youth. Master to student.", Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_TALK2, 10000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_TALK2:
                    {
                        me->MonsterSay("Every elder reaches the day where he must pass on and plant his stave with the staves of his ancestors. Today is the day when my staff joins these woods.", Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS1, 6000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS1:
                    {
                        me->GetMotionMaster()->MovePoint(1, masterShangXiWoodsPos1);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_SET_FACING1, 2000);
                        break;
                    }

                    case EVENT_MASTER_SHANG_XI_WOODS_SET_FACING1:
                    {
                        me->SetFacingTo(-M_PI * 1.75);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_TALK3, 8000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_TALK3:
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 15.0f);

                        for (auto&& player : playerList)
                        {
                            me->MonsterSay("$p, our people have lived the wholes of their lives on this great turtle, Shen-zin Su, but not in hundreds of years has anyone spoken to him.", Language::LANG_UNIVERSAL, player);
                        }
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS2, 4000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS2:
                    {
                        me->GetMotionMaster()->MovePoint(2, masterShangXiWoodsPos2);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS3, 1000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS3:
                    {
                        me->GetMotionMaster()->MovePoint(3, masterShangXiWoodsPos3);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS4, 1000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS4:
                    {
                        me->GetMotionMaster()->MovePoint(4, masterShangXiWoodsPos4);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS5, 1000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS5:
                    {
                        me->GetMotionMaster()->MovePoint(5, masterShangXiWoodsPos5);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_TALK4, 1000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_TALK4:
                    {
                        if (Creature* staff = me->SummonCreature(57874, 873.09375f, 4462.259765625f, 241.41162109375f, 3.804818391799926757f, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 70000))
                            me->SetFacingToObject(staff);
                        me->MonsterSay("Now Shen-zin Su is ill, and we are all in danger. With the help of the elements, you will break the silence. You will speak to him.", Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS6, 5000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_MOVE_POS6:
                    {
                        me->GetMotionMaster()->MovePoint(6, masterShangXiWoodsPos6);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_TALK5, 2000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_TALK5:
                    {
                        me->SetFacingTo(M_PI * 1.25);
                        me->MonsterSay("Aysa and Ji have retrieved the spirits and brought them here. You are to go with them, speak to the great Shen-zin Su, and do what must be done to save our people.", Language::LANG_UNIVERSAL, me);

                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_TALK6, 5000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_TALK6:
                    {
                        me->MonsterSay("You've come far, my young student. I see within you a great hero. I leave the fate of this land to you.", Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_KNEEL, 5000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_KNEEL:
                    {
                        me->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_CREDIT, 2000);
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_CREDIT:
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 15.0f);

                        for (auto&& player : playerList)
                        {
                            player->KilledMonsterCredit(56688);
                        }
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_WOODS_RESET, 60000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_WOODS_RESET:
                    {
                        me->GetMotionMaster()->MovePoint(7, me->GetHomePosition());
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_master_shang_xi_wood_of_stavesAI(creature);
    }
};

class npc_guardian_of_the_elders : public CreatureScript
{
public:
    npc_guardian_of_the_elders() : CreatureScript("npc_guardian_of_the_elders") { }

    struct npc_guardian_of_the_eldersAI : public CreatureAI
    {
        npc_guardian_of_the_eldersAI(Creature* creature) : CreatureAI(creature) { }

        void JustDied(Unit* killer) OVERRIDE
        {
            if (GameObject* door = me->FindNearestGameObject(209922, 40.0f))
            {
                door->UseDoorOrButton(60000);

                if (Creature* masterShangXi = me->FindNearestCreature(56159, 40.0f, true))
                {
                    masterShangXi->SetWalk(false);
                    masterShangXi->GetMotionMaster()->MovePoint(10, masterShangXi->GetPositionX(), masterShangXi->GetPositionY(), masterShangXi->GetPositionZ());
                }
            }
        }
        void UpdateAI(uint32 /*diff*/)
        {
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_guardian_of_the_eldersAI(creature);
    }
};

const Position masterShangXiWorthyPos1 = { 776.52606f, 4178.652f, 206.94368f };
const Position masterShangXiWorthyPos2 = { 792.8524f, 4185.137f, 208.41711f };
const Position masterShangXiWorthyPos3 = { 827.27637f, 4205.244f, 199.61674f };
const Position masterShangXiWorthyPos4 = { 839.7535f, 4215.7915f, 197.77382f };
const Position masterShangXiWorthyPos5 = { 845.00696f, 4267.5884f, 196.7384f };
const Position masterShangXiWorthyPos6 = { 843.81946f, 4301.1743f, 210.9836f };
const Position masterShangXiWorthyPos7 = { 843.26215f, 4339.1035f, 223.98082f };
const Position masterShangXiWorthyPos8 = { 828.6528f, 4353.315f, 223.98082f };
const Position masterShangXiWorthyPos9 = { 830.25867f, 4368.5503f, 223.94623f };
const Position masterShangXiWorthyPos10 = { 845.59398f, 4372.6298f, 224.06399f };
const Position masterShangXiWorthyPos11 = { 844.69406f, 4401.2700f, 237.26740f };
const Position masterShangXiWorthyEndPos = { 874.10767f, 4459.6313f, 241.18892f };
class npc_master_shang_xi_worthy_questgiver : public CreatureScript
{
public:
    npc_master_shang_xi_worthy_questgiver() : CreatureScript("npc_master_shang_xi_worthy_questgiver") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() == 29787) // Worthy of Passing
        {
            if (Creature* masterShangXi = creature->SummonCreature(56159, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
            {
                masterShangXi->GetMotionMaster()->MovePoint(0,  masterShangXi->GetPositionX(), masterShangXi->GetPositionY(), masterShangXi->GetPositionZ());
            }
            
            return true;
        }
        return false;
    }

    struct npc_master_shang_xi_worthy_questgiverAI : public CreatureAI
    {
        npc_master_shang_xi_worthy_questgiverAI(Creature* creature) : CreatureAI(creature) { }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_master_shang_xi_worthy_questgiverAI(creature);
    }
};
class npc_master_shang_xi_worthy_of_passing : public CreatureScript
{
public:
    npc_master_shang_xi_worthy_of_passing() : CreatureScript("npc_master_shang_xi_worthy_of_passing") { }

    enum masterShangXiWorthyEvents
    {
        EVENT_WORTHY_TALK1 = 1,
        EVENT_WORTHY_MOVE_POS1 = 2,
        EVENT_WORTHY_MOVE_POS2 = 3,
        EVENT_WORTHY_MOVE_POS3 = 4,
        EVENT_WORTHY_MOVE_POS4 = 5,
        EVENT_WORTHY_MOVE_POS5 = 6,
        EVENT_WORTHY_TALK2     = 7,
        EVENT_WORTHY_MOVE_POS6 = 8,
        EVENT_WORTHY_TALK3     = 9,
        EVENT_WORTHY_MOVE_POS7 = 10,
        EVENT_WORTHY_TALK4     = 11,
        EVENT_WORTHY_MOVE_POS8 = 12,
        EVENT_WORTHY_MOVE_POS9 = 13,
        EVENT_WORTHY_MOVE_POS10 = 14,
        EVENT_WORTHY_MOVE_POS11 = 15,
    };

    struct npc_master_shang_xi_worthy_of_passingAI : public CreatureAI
    {
        npc_master_shang_xi_worthy_of_passingAI(Creature* creature) : CreatureAI(creature) { }
        EventMap events;
        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case 0:
                {
                    events.ScheduleEvent(EVENT_WORTHY_TALK1, 5000);
                    break;
                }
                case 10:
                {
                    events.ScheduleEvent(EVENT_WORTHY_MOVE_POS9, 1000);
                    break;
                }
                default:
                    break;
            }
        }
        void UpdateAI(uint32 diff)
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WORTHY_TALK1:
                    {
                        me->MonsterSay("Come child. We have one final journey to take together before your training is complete.", Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS1, 5000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS1:
                    {
                        me->GetMotionMaster()->MovePoint(1, masterShangXiWorthyPos1);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS2, 5000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS2:
                    {
                        me->GetMotionMaster()->MovePoint(2, masterShangXiWorthyPos2);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS3, 5000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS3:
                    {
                        me->GetMotionMaster()->MovePoint(3, masterShangXiWorthyPos3);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS4, 5000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS4:
                    {
                        me->GetMotionMaster()->MovePoint(4, masterShangXiWorthyPos4);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS5, 5000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS5:
                    {
                        me->GetMotionMaster()->MovePoint(5, masterShangXiWorthyPos5);
                        events.ScheduleEvent(EVENT_WORTHY_TALK2, 5000);
                        break;
                    }
                    case EVENT_WORTHY_TALK2:
                    {
                        me->SetWalk(true);
                        me->MonsterSay("Beyond the Elders' Path lies the Wood of Staves, a sacred place that only the worthy may enter.", Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS6, 5000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS6:
                    {
                        me->GetMotionMaster()->MovePoint(6, masterShangXiWorthyPos6);
                        events.ScheduleEvent(EVENT_WORTHY_TALK3, 15000);
                        break;
                    }
                    case EVENT_WORTHY_TALK3:
                    {
                        me->MonsterSay("Of the many ways to prove your worth, I require the simplest of you now. I must know that you will fight for our people. I must know that you can keep them safe.", Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS7, 10000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS7:
                    {
                        me->GetMotionMaster()->MovePoint(7, masterShangXiWorthyPos7);
                        events.ScheduleEvent(EVENT_WORTHY_TALK4, 20000);
                        break;
                    }
                    case EVENT_WORTHY_TALK4:
                    {
                        me->MonsterSay("Defeat the Guardian of the Elders, and we may pass.", Language::LANG_UNIVERSAL, me);
                        me->GetMotionMaster()->MovePoint(8, masterShangXiWorthyPos8);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS8, 5000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS8:
                    {
                        me->GetMotionMaster()->MovePoint(9, masterShangXiWorthyPos9);
                        break;
                    }

                    case EVENT_WORTHY_MOVE_POS9:
                    {
                        me->MonsterSay("You've become strong indeed, child. This is good. You will need that strength soon.", Language::LANG_UNIVERSAL, me);
                        me->GetMotionMaster()->MovePoint(11, masterShangXiWorthyPos10);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS10, 3000);
                        break;
                    }

                    case EVENT_WORTHY_MOVE_POS10:
                    {
                        me->GetMotionMaster()->MovePoint(12, masterShangXiWorthyPos11);
                        events.ScheduleEvent(EVENT_WORTHY_MOVE_POS11, 5000);
                        break;
                    }
                    case EVENT_WORTHY_MOVE_POS11:
                    {
                        me->GetMotionMaster()->MovePoint(13, masterShangXiWorthyEndPos);
                        me->DespawnOrUnsummon(10000);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_master_shang_xi_worthy_of_passingAI(creature);
    }
};

class npc_firework_launcher : public CreatureScript
{
public:
    npc_firework_launcher() : CreatureScript("npc_firework_launcher") { }

    struct npc_firework_launcherAI : public ScriptedAI
    {
        npc_firework_launcherAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 cooldown;

        void Reset() OVERRIDE
        {
            cooldown = 0;
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }

        void OnSpellClick(Unit* Clicker, bool& /*result*/) OVERRIDE
        {
            if (cooldown)
                return;

            Creature* const zhao = GetClosestCreatureWithEntry(me, 55786, 50.0f);

            if (zhao && zhao->IsWithinDist2d(Clicker, 25.0f))
                zhao->CastSpell(zhao, 104855, false);
            else
            {
                return;
            }

            cooldown = 5000;
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            return;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (cooldown)
            {
                if (cooldown <= diff)
                {
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    cooldown = 0;
                }
                else
                    cooldown -= diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_firework_launcherAI(creature);
    }
};

static Position const ZhaoPos[] =
{
    {719.36f, 4164.60f, 216.06f },
    {736.90f, 4183.85f, 221.41f },
    {704.77f, 4190.16f, 218.24f },
    {684.53f, 4173.24f, 216.98f },
    {689.62f, 4153.16f, 217.63f },
    {717.04f, 4141.16f, 219.83f },
    {745.91f, 4154.35f, 223.48f }
};

const Position zhaoStunPos = { 723.163025f, 4163.799805f, 202.082993f };

class boss_zhao_ren : public CreatureScript
{
    enum zhaoRenEvents
    {
        EVENT_MOVE_POSITION = 1,
        EVENT_STUNNED = 2,
        EVENT_LIGHTNING = 3,
    };
public:
    boss_zhao_ren() : CreatureScript("boss_zhao_ren") { }

    struct boss_zhao_renAI : public CreatureAI
    {
        EventMap events;
        bool encounterStarted;
        uint8 currentPos;
        uint8 fireworkHits;
        boss_zhao_renAI(Creature* creature) : CreatureAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
            me->SetReactState(REACT_PASSIVE);
            me->SetDisableGravity(true);
            encounterStarted = false;
            fireworkHits = 0;
            currentPos = 0;
            me->SetFullHealth();
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MovePoint(0, ZhaoPos[0].GetPositionX(), ZhaoPos[0].GetPositionY(), ZhaoPos[0].GetPositionZ());
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 60.0f);

            for (auto player : playerList)
            {
                player->KilledMonsterCredit(me->GetEntry());
                player->CastSpell(player, 60922);
                player->SetPhased(169, true, true);
                player->SetPhased(524, true, false);
            }
        }

        void MovementInform(uint32 type, uint32 pointId) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (!pointId)
                return;

            if (pointId == 100)
            {
                me->CastSpell(me, 125992, true);
                events.RescheduleEvent(EVENT_LIGHTNING, 17000);
                events.ScheduleEvent(EVENT_STUNNED, 12000);
            }
            else
                events.ScheduleEvent(EVENT_MOVE_POSITION, 1000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (checkParticipants())
            {
                if (!encounterStarted)  // Event not started, player found
                {
                    events.ScheduleEvent(EVENT_MOVE_POSITION, 1000);
                    events.ScheduleEvent(EVENT_LIGHTNING, 5000);
                    encounterStarted = true;
                }
            }
            else
            {
                if (encounterStarted)  // Event started, no player found
                    Reset();

                return;
            }

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_MOVE_POSITION:
                {
                    if (me->HasAura(125992))
                        events.ScheduleEvent(EVENT_MOVE_POSITION, 2000);

                    RotatePosition();
                    break;
                }
                case EVENT_STUNNED:
                {
                    me->RemoveAurasDueToSpell(125992);
                    me->CastSpell(me, 125990, false);
                    me->SetDisableGravity(true);
                    events.ScheduleEvent(EVENT_MOVE_POSITION, 4000);
                    break;
                }
                case EVENT_LIGHTNING:
                {
                    if (Player* player = GetRandomPlayer())
                        me->CastSpell(player, 126006, false);

                    events.ScheduleEvent(EVENT_LIGHTNING, 5000);
                    break;
                }
            }
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell) OVERRIDE
        {
            if (spell->Id == 104855)
            {
                if (++fireworkHits >= 5)
                {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(100, zhaoStunPos);
                    fireworkHits = 0;
                }
            }
        }

        void RotatePosition()
        {
            if (++currentPos > 6)
                currentPos = 1;

            me->GetMotionMaster()->MovePoint(currentPos, ZhaoPos[currentPos].GetPositionX(), ZhaoPos[currentPos].GetPositionY(), ZhaoPos[currentPos].GetPositionZ());
        }

        Player* GetRandomPlayer()
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 45.0f);

            if (playerList.empty())
                return NULL;

            Skyfire::Containers::RandomResizeList(playerList, 1);

            return *playerList.begin();
        }
        bool checkParticipants()
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 60.0f);

            for (auto player : playerList)
                if (player->GetQuestStatus(29786) == QUEST_STATUS_INCOMPLETE)
                    if (player->IsAlive())
                        return true;

            return false;
        }
    };


    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_zhao_renAI(creature);
    }
};

const Position aysaChamberMovePos1 = { 647.493f, 4224.63f, 202.90865f, 2.426f };
class npc_aysa_battle_for_the_skies : public CreatureScript
{
public:
    npc_aysa_battle_for_the_skies() : CreatureScript("npc_aysa_battle_for_the_skies") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() == 29786) // Battle for the skies
        {
            player->SetPhased(524, true, true);
            creature->SetPhased(524, true, true);
            creature->GetMotionMaster()->MovePoint(0, aysaChamberMovePos1);
            creature->DespawnOrUnsummon(10000);
        }
        return true;
    }
    struct npc_aysa_battle_for_the_skiesAI : public CreatureAI
    { 
        npc_aysa_battle_for_the_skiesAI(Creature* creature) : CreatureAI(creature) {}
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_aysa_battle_for_the_skiesAI(creature);
    }
};

const Position aysaChamberMovePos2 = { 598.57294f, 4266.661f, 206.54927f };
const Position aysaChamberMovePos3 = { 580.1649f, 4283.193f, 210.18248f };
const Position aysaChamberMoveEnd = { 543.9549, 4317.2744, 212.22935 };
class npc_aysa_chamber_of_whispers : public CreatureScript
{
public:
    npc_aysa_chamber_of_whispers() : CreatureScript("npc_aysa_chamber_of_whispers") { }

    struct npc_aysa_chamber_of_whispersAI : public CreatureAI
    {
        enum aysaChamberOfWhisperEvents
        {
            EVENT_INTRO = 1,
            EVENT_DEACTIVATE_1 = 2,
            EVENT_MOVE_POS_MID = 3,
            EVENT_MOVE_POS_3 = 4,
            EVENT_DEACTIVATE_2 = 5,
            EVENT_MOVE_POS_END = 6,
            EVENT_OUTRO = 7,
        };
        EventMap events;
        npc_aysa_chamber_of_whispersAI(Creature* creature) : CreatureAI(creature)
        {
            creature->MonsterSay("Wait!", Language::LANG_UNIVERSAL, creature);
            events.ScheduleEvent(EVENT_INTRO, 1000);
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_INTRO:
                {
                    me->MonsterSay("We need to wait for the winds to settle, then make a break for the cover of the far hallway.", Language::LANG_UNIVERSAL, me);
                    me->GetMotionMaster()->MovePoint(0, aysaChamberMovePos1);
                    events.ScheduleEvent(EVENT_DEACTIVATE_1, 2000);
                    break;
                }
                case EVENT_DEACTIVATE_1:
                {
                    if (GameObject* cloud1 = me->FindNearestGameObject(209685, 30.0f))
                    {
                        cloud1->UseDoorOrButton(60000);
                    }
                    events.ScheduleEvent(EVENT_MOVE_POS_MID, 5000);
                    break;
                }
                case EVENT_MOVE_POS_MID:
                {
                    me->GetMotionMaster()->MovePoint(1, aysaChamberMovePos2);
                    events.ScheduleEvent(EVENT_MOVE_POS_3, 10000);
                    break;
                }
                case EVENT_MOVE_POS_3:
                {
                    me->MonsterSay("Wait for another opening. I'll meet you on the far side.", Language::LANG_UNIVERSAL, me);
                    events.ScheduleEvent(EVENT_DEACTIVATE_2, 2000);
                    break;
                }
                case EVENT_DEACTIVATE_2:
                {
                    me->GetMotionMaster()->MovePoint(2, aysaChamberMovePos3);
                    std::list<GameObject*> clouds;
                    me->GetGameObjectListWithEntryInGrid(clouds, 209685, 60.0f);
                    if (!clouds.empty())
                    {
                        for (std::list<GameObject*>::iterator itr = clouds.begin(); itr != clouds.end(); ++itr)
                        {
                            if ((*itr)->GetGUIDLow() == 88728)
                            {
                                if (GameObject* cloud2 = (*itr))
                                    cloud2->UseDoorOrButton(60000);
                            }
                        }
                    }
                    events.ScheduleEvent(EVENT_MOVE_POS_END, 1000);
                    break;
                }
                case EVENT_MOVE_POS_END:
                {
                    me->GetMotionMaster()->MovePoint(3, aysaChamberMoveEnd);
                    events.ScheduleEvent(EVENT_OUTRO, 10000);
                    break;
                }
                case EVENT_OUTRO:
                {
                    me->MonsterSay("Dafeng! What's wrong? Why are you hiding back here?", Language::LANG_UNIVERSAL, me);
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 45.0f);

                    for (auto&& player : playerList)
                    {
                        player->KilledMonsterCredit(55666);
                    }
                    me->DespawnOrUnsummon();
                    break;
                }
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_aysa_chamber_of_whispersAI(creature);
    }
};


const Position shuPos1 = { 650.30f, 3127.16f, 89.62f };
const Position shuPos2 = { 625.25f, 3127.88f, 87.95f };
const Position shuPos3 = { 624.44f, 3142.94f, 87.75f };
class npc_shu_dailo : public CreatureScript
{
public:
    npc_shu_dailo() : CreatureScript("npc_shu_dailo") { }

    enum ShuDailoEvents
    {
        EVENT_MOVE_POS1 = 1,
        EVENT_MOVE_POS2 = 2,
        EVENT_MOVE_POS3 = 3,
        EVENT_CAST = 5,
        EVENT_GIVE_CREDIT = 6,
    };


    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(29774) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(29774) == QUEST_STATUS_REWARDED)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Can you please help us to wake up Wugou ?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            player->CLOSE_GOSSIP_MENU();
            player->KilledMonsterCredit(55548);

            creature->GetMotionMaster()->MovePoint(0, shuPos1);
        }
        return true;
    }

    struct npc_shu_dailoAI : public CreatureAI
    {
        EventMap events;

        npc_shu_dailoAI(Creature* creature) : CreatureAI(creature) { }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
            case 0:
            {
                events.ScheduleEvent(EVENT_MOVE_POS1, 1000);
                break;
            }
            default:
                break;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_MOVE_POS1:
                {
                    me->GetMotionMaster()->MovePoint(1, shuPos1);
                    events.ScheduleEvent(EVENT_MOVE_POS2, 500);
                    break;
                }
                case EVENT_MOVE_POS2:
                {
                    me->GetMotionMaster()->MovePoint(2, shuPos2);
                    events.ScheduleEvent(EVENT_MOVE_POS3, 4000);
                    break;
                }
                case EVENT_MOVE_POS3:
                {
                    me->GetMotionMaster()->MovePoint(3, shuPos3);
                    events.ScheduleEvent(EVENT_CAST, 3000);
                    break;
                }
                case EVENT_CAST:
                {
                    if (Creature* wugou = me->FindNearestCreature(55539, 15.0f, true))
                    {
                        wugou->CastSpell(wugou, 118027);
                        me->SetFacingToObject(wugou);
                        events.ScheduleEvent(EVENT_GIVE_CREDIT, 5000);
                    }
                    break;
                }
                case EVENT_GIVE_CREDIT:
                {
                    if (Creature* wugou = me->FindNearestCreature(55539, 15.0f, true))
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, wugou, 15.0f);

                        for (auto&& player : playerList)
                        {
                            player->KilledMonsterCredit(55547);

                            if (Creature* wugouCopy = player->SummonCreature(55539, wugou->GetHomePosition(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                wugouCopy->SetOwnerGUID(player->GetGUID());
                                wugouCopy->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
                            }

                            if (Creature* shuCopy = player->SummonCreature(55558, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                shuCopy->SetOwnerGUID(player->GetGUID());
                                shuCopy->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, -PET_FOLLOW_ANGLE);
                            }
                        }
                        wugou->DespawnOrUnsummon();
                        me->DespawnOrUnsummon();
                    }
                }
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_shu_dailoAI(creature);
    }
};

const Position shuJumpPos1 { 1102.05f, 2882.11f, 94.32f, 0.11f };
const Position shuJumpPos2 { 1120.01f, 2883.20f, 96.44f, 4.17f };
const Position shuJumpPos3 { 1128.09f, 2859.44f, 97.64f, 2.51f };
const Position shuJumpPos4 { 1111.52f, 2849.84f, 94.84f, 1.94f };

class npc_shu_pool_of_reflection : public CreatureScript
{
public:
    npc_shu_pool_of_reflection() : CreatureScript("npc_shu_pool_of_reflection") { }

    struct npc_shu_pool_of_reflectionAI : public CreatureAI
    {
        enum ShuEvents
        {
            EVENT_ROTATE_POSITION = 1,
            EVENT_SUMMON_WATER_SPOUT = 2,
            EVENT_WATER_SPOUT_BURST = 3,
            EVENT_WATER_SPOUT_DESPAWN = 4
        };
        EventMap events;
        uint64 waterSpoutGUID = 0;
        uint8 stationaryPosition;
        bool running;
        std::vector<Player*> players;

        npc_shu_pool_of_reflectionAI(Creature* creature) : CreatureAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
            stationaryPosition = 0;
            waterSpoutGUID = 0;
            running = false;
        }

        void UpdatePlayerList()
        {
            players.clear();

            std::list<Player*> PlayerList;
            me->GetPlayerListInGrid(PlayerList, 20.0f);

            for (auto&& player : PlayerList)
                if (!player->IsGameMaster() && player->GetQuestStatus(29679) == QUEST_STATUS_INCOMPLETE)
                    players.push_back(player);
        }
        
        void MoveInLineOfSight(Unit* who)
        {
            Player* const player = who->ToPlayer();
            if (!player)
                return;

            if (player->GetQuestStatus(29679) != QUEST_STATUS_INCOMPLETE || running)
                return;

            if (me->GetAreaId() == 5862) // Pool of Reflection Area
            {
                UpdatePlayerList();
                if (!players.empty())
                {
                    running = true;
                    events.ScheduleEvent(EVENT_ROTATE_POSITION, 5000);
                }
            }
        }
        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
            case 0:
                if (running)
                {
                    events.ScheduleEvent(EVENT_ROTATE_POSITION, 5000);
                }
                return;
            default:
                break;
            }
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_ROTATE_POSITION:
                {
                    uint8 newPlace = 0;

                    newPlace = urand(0, 3);

                    switch (newPlace)
                    {
                        case 0:
                        {
                            me->GetMotionMaster()->MoveJump(shuJumpPos1, 10.0f, 10.0f, 1);
                            break;
                        }
                        case 1:
                        {
                            me->GetMotionMaster()->MoveJump(shuJumpPos2, 10.0f, 10.0f, 1);
                            break;
                        }
                        case 2:
                        {
                            me->GetMotionMaster()->MoveJump(shuJumpPos3, 10.0f, 10.0f, 1);
                            break;
                        }
                        case 3:
                        {
                            me->GetMotionMaster()->MoveJump(shuJumpPos4, 10.0f, 10.0f, 1);
                            break;
                        }
                        default:
                            break;
                    }

                    events.ScheduleEvent(EVENT_SUMMON_WATER_SPOUT, 2000);
                    me->AddAura(116695, me);
                    stationaryPosition = newPlace;
                    break;
                }
                case EVENT_SUMMON_WATER_SPOUT:
                {
                    float x = 0.0f, y = 0.0f;
                    GetPositionWithDistInOrientation(me, 5.0f, me->GetOrientation() + frand(-M_PI, M_PI), x, y);

                    if (Creature* waterSpout = me->SummonCreature(60488, x, y, 92.189629f))
                    {
                        waterSpoutGUID = waterSpout->GetGUID();
                        waterSpout->CastSpell(waterSpout, 116695, true);
                    }

                    events.ScheduleEvent(EVENT_WATER_SPOUT_BURST, 7000);

                    break;
                }
                case EVENT_WATER_SPOUT_BURST:
                {
                    if (Creature* waterSpout = me->GetMap()->GetCreature(waterSpoutGUID))
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, waterSpout, 1.0f);

                        for (auto&& player : playerList)
                            player->CastSpell(player, 116696, true);

                        waterSpout->CastSpell(waterSpout, 117057, true);
                    }
                    events.ScheduleEvent(EVENT_WATER_SPOUT_DESPAWN, 3000);
                    break;
                }
                case EVENT_WATER_SPOUT_DESPAWN:
                {
                    if (Creature* waterSpout = me->GetMap()->GetCreature(waterSpoutGUID))
                        waterSpout->DespawnOrUnsummon();

                    UpdatePlayerList();
                    if (players.empty())
                    {
                        if (Creature* aysa = me->FindNearestCreature(54975, 60.0f, true))
                        {
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, aysa, 60.0f);

                            for (auto&& player : playerList)
                            {
                                if (player->GetQuestStatus(29679) == QUEST_STATUS_COMPLETE)
                                {
                                    if (!player->GetAura(128588)) // Aysa gz trigger aura
                                    {
                                        player->CastSpell(player, 128588);
                                        aysa->AI()->Talk(0);
                                    }
                                }
                            }
                        }
                        running = false;
                        events.CancelEvent(EVENT_ROTATE_POSITION);
                        events.CancelEvent(EVENT_SUMMON_WATER_SPOUT);
                        events.CancelEvent(EVENT_WATER_SPOUT_BURST);
                    }
                    else
                    {
                        UpdatePlayerList();
                        running = true;
                        events.ScheduleEvent(EVENT_ROTATE_POSITION, 5000);
                    }
                    break;
                }
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_shu_pool_of_reflectionAI(creature);
    }
};

const Position tornadoPos1 = { 922.9829f, 3602.6472f, 198.44557f };
const Position tornadoPos2 = { 958.7378f, 3610.3413f, 209.64407f };
const Position tornadoPos3 = { 934.45074f, 3609.4578f, 240.62868f };
const Position tornadoEndPos = { 920.4496f, 3604.7705f, 253.17314f };
class npc_uplift_draft : public CreatureScript
{
public:
    npc_uplift_draft() : CreatureScript("npc_uplift_draft") { }

    struct npc_uplift_draftAI : public CreatureAI
    {
        EventMap events;
        enum upliftDraftEvents
        {
            EVENT_MOVE_POS1 = 1,
            EVENT_MOVE_POS2,
            EVENT_MOVE_POS3,
            EVENT_MOVE_END_POS
        };
        npc_uplift_draftAI(Creature* creature) : CreatureAI(creature)
        {
            events.ScheduleEvent(EVENT_MOVE_POS1, 1000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_POS1:
                    {
                        me->GetMotionMaster()->MovePoint(0, tornadoPos1);
                        events.ScheduleEvent(EVENT_MOVE_POS2, 1000);
                        break;
                    }
                    case EVENT_MOVE_POS2:
                    {
                        me->GetMotionMaster()->MovePoint(1, tornadoPos2);
                        events.ScheduleEvent(EVENT_MOVE_POS3, 5000);
                        break;
                    }
                    case EVENT_MOVE_POS3:
                    {
                        me->GetMotionMaster()->MovePoint(2, tornadoPos3);
                        events.ScheduleEvent(EVENT_MOVE_END_POS, 5000);
                        break;
                    }
                    case EVENT_MOVE_END_POS:
                    {
                        me->GetMotionMaster()->MovePoint(4, tornadoEndPos);
                        break;
                    }
                }
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_uplift_draftAI(creature);
    }
};

#define MASTER_SHANG_XI_TEXT_1 "You have conquered every challenge I put before you, $n. You have found Huo and brought him safely to the temple."
#define MASTER_SHANG_XI_TEXT_2 "There is a much larger problem we face now, my students. Shen-zin Su is in pain. If we do not act the very land on which we stand could die, and all of us with it."
#define MASTER_SHANG_XI_TEXT_3 "We need to speak to Shen-zin Su and discover how to heal it. And to do that, we need the four elemental spirits returned. Huo was the first."
#define MASTER_SHANG_XI_TEXT_4 "Ji, I'd like you to go to the Dai-Lo Farmstead in search of Wugou, the spirit of earth."
#define JI_TEXT_1 "On It!"
#define MASTER_SHANG_XI_TEXT_5 "Aysa, I want you to go to the Singing Pools to find Shu, the spirit of water."
#define AYSA_TEXT_1 "Yes master."
#define MASTER_SHANG_XI_TEXT_6 "And $n, you shall be the hand that guides us all. Speak with me for a moment before you join Aysa at the Singing Pools to the east."

#define MASTER_SHANG_XI_TEXT_7 "You've returned with the spirits of water and earth. You make an old master proud."
#define MASTER_SHANG_XI_TEXT_8 "Wugou and Shu are welcome here. We will care for them well."
#define MASTER_SHANG_XI_TEXT_9 "The only remaining spirit is Dafeng, who hides somewhere across the Dawning Span to the west."


const Position jiTempleMovePoint_1 = { 966.1493f, 3607.0894f, 196.51373f };
const Position jiTempleMovePoint_2 = { 958.9819f, 3594.94f, 196.6083f };
const Position jiTempleMovePoint_3 = { 950.7555f, 3588.809f, 196.8154f };

const Position aysaTempleMovePoint_1 = { 966.3715f, 3602.764f, 196.47968f };
const Position aysaTempleMovePoint_2 = { 943.1327f, 3572.154f, 193.6543f };

const Position jiTempleSpawnPos = { 971.5566f, 3607.8015f, 195.71495f };
const Position aysaTempleSpawnPos = { 968.2688f, 3602.207f, 196.6442f }; 
const Position masterShangXiPos = { 960.043030f, 3606.050049f, 196.414001f, 0.0f };

class npc_master_shang_xi_temple : public CreatureScript
{
    enum EventsMasterShangXi
    {
        EVENT_MASTER_SHANG_XI_SPAWN_POS = 1,
        EVENT_MASTER_SHANG_XI_TALK_1,
        EVENT_MASTER_SHANG_XI_TALK_2,
        EVENT_MASTER_SHANG_XI_TALK_3,
        EVENT_MASTER_SHANG_XI_TALK_4,
        EVENT_MASTER_SHANG_XI_TALK_5,
        EVENT_MASTER_SHANG_XI_TALK_6,
        EVENT_MASTER_SHANG_XI_TALK_7,
        EVENT_MASTER_SHANG_XI_TALK_8,
        EVENT_MASTER_SHANG_XI_TALK_9,
    };
public:
    npc_master_shang_xi_temple() : CreatureScript("npc_master_shang_xi_temple") { }

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() == 29776) // Morning Breeze Village
        {
            if (Creature* vehicle = player->SummonCreature(55685, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 20000))
            {
                player->EnterVehicle(vehicle);
            }
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(29776) != QUEST_STATUS_NONE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to go back on the top of the temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (Creature* vehicle = player->SummonCreature(55685, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 20000))
            {
                player->EnterVehicle(vehicle);
            }
        }

        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_master_shang_xi_templeAI(creature);
    }

    struct npc_master_shang_xi_templeAI : public CreatureAI
    {
        std::vector<Player*> players;
        EventMap events;
        bool started = false;

        npc_master_shang_xi_templeAI(Creature* creature) : CreatureAI(creature) { }



        void Reset() OVERRIDE
        {
            started = false;
        }
        void MoveInLineOfSight(Unit* who)
        {
            Player* const player = who->ToPlayer();
            if (!player)
                return;

            if (player->GetQuestStatus(29423) != QUEST_STATUS_REWARDED || started)
                return;

            if (me->GetAreaId() == 5820) // Temple of Five Dawns Area
            {
                started = true;
                if (player->GetQuestStatus(29775) == QUEST_STATUS_REWARDED)
                {
                    events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_7, 1000);
                }
                else
                {
                    events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_1, 1000);
                }
            }
        }

        void UpdatePlayerList()
        {
            players.clear();

            std::list<Player*> PlayerList;
            me->GetPlayerListInGrid(PlayerList, 20.0f);

            for (auto&& player : PlayerList)
                if (!player->IsGameMaster() && player->GetQuestStatus(29423) == QUEST_STATUS_REWARDED)
                    players.push_back(player);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MASTER_SHANG_XI_TALK_1:
                    {
                        UpdatePlayerList();
                        for (auto&& player : players)
                        {
                            me->MonsterSay(MASTER_SHANG_XI_TEXT_1, Language::LANG_UNIVERSAL, player);
                            events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_2, 10000);
                        }
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_TALK_2:
                    {
                        me->MonsterSay(MASTER_SHANG_XI_TEXT_2, Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_3, 10000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_TALK_3:
                    {
                        me->MonsterSay(MASTER_SHANG_XI_TEXT_3, Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_4, 10000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_TALK_4:
                    {
                        me->MonsterSay(MASTER_SHANG_XI_TEXT_4, Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_5, 10000);
                        if (Creature* ji = me->FindNearestCreature(61127, 20.0f, true))
                        {
                            ji->MonsterSay(JI_TEXT_1, Language::LANG_UNIVERSAL, ji);
                            ji->GetMotionMaster()->MovePoint(1, jiTempleMovePoint_2);
                            ji->DespawnOrUnsummon(2500);
                        }
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_TALK_5:
                    {
                        me->MonsterSay(MASTER_SHANG_XI_TEXT_5, Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_6, 10000);
                        if (Creature* aysa = me->FindNearestCreature(61126, 20.0f, true))
                        {
                            aysa->MonsterSay(AYSA_TEXT_1, Language::LANG_UNIVERSAL, aysa);
                            aysa->GetMotionMaster()->MovePoint(2, aysaTempleMovePoint_2);
                            aysa->DespawnOrUnsummon(2500);
                        }
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_TALK_6:
                    {
                        UpdatePlayerList();
                        for (auto&& player : players)
                        {
                            me->MonsterSay(MASTER_SHANG_XI_TEXT_6, Language::LANG_UNIVERSAL, player);
                        }
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_TALK_7:
                    {
                        me->MonsterSay(MASTER_SHANG_XI_TEXT_7, Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_8, 10000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_TALK_8:
                    {
                        me->MonsterSay(MASTER_SHANG_XI_TEXT_8, Language::LANG_UNIVERSAL, me);
                        events.ScheduleEvent(EVENT_MASTER_SHANG_XI_TALK_9, 10000);
                        break;
                    }
                    case EVENT_MASTER_SHANG_XI_TALK_9:
                    {
                        me->MonsterSay(MASTER_SHANG_XI_TEXT_9, Language::LANG_UNIVERSAL, me);
                        break;
                    }
                }
            }
        }
    };
};

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
                    if (Creature* aysa = master->SummonCreature(61126, aysaTempleSpawnPos, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000))
                    {
                        aysa->GetMotionMaster()->MovePoint(0, aysaTempleMovePoint_1);
                    }
                    if (Creature* ji = master->SummonCreature(61127, jiTempleSpawnPos, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000))
                    {
                        ji->GetMotionMaster()->MovePoint(0, jiTempleMovePoint_1);
                    }
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
        player->CastSpell(player, 102630); // blessing of huo
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
    enum EventsLiFei
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

        void Reset() OVERRIDE
        {
            Power = 0;
            started = false;
        }

        void MoveInLineOfSight(Unit* /*who*/) { }

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
            if (playersParticipate.empty())
            {
                UpdatePlayerList();
                return;
            }
            else
            {
                for (auto&& player : playersParticipate)
                {
                    if (!started && player->GetQuestStatus(29414) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (me->GetAreaId() == 5848) // Cave of Meditation Area
                        {
                            started = true;
                            events.ScheduleEvent(EVENT_POWER, 1000);
                            events.ScheduleEvent(EVENT_ADDS, 1000);
                        }
                    }
                }
            }

            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_POWER:
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

class npc_jaomin_ro : public CreatureScript
{
    public:
        npc_jaomin_ro() : CreatureScript("npc_jaomin_ro") { }
 
        struct npc_jaomin_roAI : public ScriptedAI
        {
            npc_jaomin_roAI(Creature* creature) : ScriptedAI(creature) { }
 
            enum eEvents
            {
                EVENT_JAOMIN_JUMP   = 1,
                EVENT_JAOMIN_JUMP_DAMAGE = 2,
                EVENT_FALCON        = 3,
                EVENT_FALCON_VEHICLE = 4,
                EVENT_FALCON_VEHICLE_EXIT = 5,
                EVENT_FALCON_STUN = 6,
                EVENT_FALCON_STOP  =7,
                EVENT_END_OF_COMBAT = 8
            };
 
            EventMap events;
 
            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_JAOMIN_JUMP, 2000);
            }
 
            void Reset() OVERRIDE
            {
                events.Reset();
            }
 
            void MoveInLineOfSight(Unit*  who) OVERRIDE
            {
                Player * const player = who->ToPlayer();
                if (!player)
                    return;
 
                if (player->GetQuestStatus(29409) != QUEST_STATUS_INCOMPLETE)
                    return;
 
                if (who->GetDistance(me) < 15.f)
                {
                    if (me->getStandState() != UNIT_STAND_STATE_STAND)
                    {
                        Talk(0);
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                    }
                }
            }
 
            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (HealthBelowPct(10))
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 10.0f);
                    for (auto&& player: playerList)
                        player->KilledMonsterCredit(me->GetEntry(), 0);
 
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                    Talk(1);
                    
                    if (damage > me->GetHealth())
                        damage = 0;
                    
                    events.CancelEvent(EVENT_JAOMIN_JUMP);
                    events.CancelEvent(EVENT_JAOMIN_JUMP_DAMAGE);
                    events.CancelEvent(EVENT_FALCON);
                    events.CancelEvent(EVENT_FALCON_VEHICLE);
                    events.CancelEvent(EVENT_FALCON_VEHICLE_EXIT);
                    events.CancelEvent(EVENT_FALCON_STUN);
                    events.CancelEvent(EVENT_FALCON_STOP);

                    events.ScheduleEvent(EVENT_END_OF_COMBAT, 1000);
                }
 
            }
 
            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;
                
                events.Update(diff);
 
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_JAOMIN_JUMP:
                        {
                            me->CastSpell(me->GetVictim(), 108938);
                            me->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+10.0f, 20.0f, 20.0f);
                            events.ScheduleEvent(EVENT_JAOMIN_JUMP_DAMAGE, 2500);
                            break;
                        }
                        case EVENT_JAOMIN_JUMP_DAMAGE:
                        {
                            me->GetMotionMaster()->MoveJump(me->GetVictim()->GetPositionX(),me->GetVictim()->GetPositionY(),me->GetVictim()->GetPositionZ(), 20.0f, 20.0f);
                            me->CastSpell(me->GetVictim(), 108937);
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 5.0f);
                            for (auto&& player : playerList)
                            {
                                float horizontalSpeed = 10.0f;
                                float verticalSpeed = 8.0f;
                                player->KnockbackFrom(me->GetPositionX(), me->GetPositionY(), horizontalSpeed, verticalSpeed);
                            }
                            events.ScheduleEvent(EVENT_FALCON, 10000);
                            break;
                        }
                        case EVENT_FALCON:
                        {
                                
                            me->CastSpell(me, 108955);
                            if (Creature* hawk = me->SummonCreature(57750, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 3000))
                            {
                                hawk->SetDisplayId(39796);
                                me->EnterVehicle(hawk, 0);
                            }
                            events.ScheduleEvent(EVENT_FALCON_VEHICLE, 1000);
                            break;
                        }
                        case EVENT_FALCON_VEHICLE:
                        {
                            if (Creature* hawk = me->FindNearestCreature(57750, 25.0f))
                            {
                                float x, y, z;
                                me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 20.0f);
                                hawk->GetMotionMaster()->MoveCharge(x,y,z);
                            }
                            events.ScheduleEvent(EVENT_FALCON_VEHICLE_EXIT, 1000);
                            break;
                        }
                        case EVENT_FALCON_VEHICLE_EXIT:
                        {
                            me->ExitVehicle();
                            events.ScheduleEvent(EVENT_FALCON_STUN, 1000);
                            break;
                        }
                        case EVENT_FALCON_STUN:
                        {
                            me->CastSpell(me->GetVictim(), 108971);
                            events.ScheduleEvent(EVENT_FALCON_STOP, 2000);
                            break;
                        }
                        case EVENT_FALCON_STOP:
                        {
                            me->RemoveAurasByType(SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                            events.ScheduleEvent(EVENT_JAOMIN_JUMP, 10000);
                            break;
                        }
                        case EVENT_END_OF_COMBAT:
                        {
                            EnterEvadeMode();
                            break;
                        }
                        default:
                            break;
                    }
                }
 
                DoMeleeAttackIfReady();
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_jaomin_roAI(creature);
        }
};

void AddSC_wandering_island()
{
    new npc_hot_air_balloon_vehicle();
    new npc_hot_air_balloon();
    new npc_master_shang_xi_wood_of_staves();
    new npc_guardian_of_the_elders();
    new npc_master_shang_xi_worthy_questgiver();
    new npc_master_shang_xi_worthy_of_passing();
    new npc_firework_launcher();
    new boss_zhao_ren();
    new npc_aysa_battle_for_the_skies();
    new npc_aysa_chamber_of_whispers();
    new npc_uplift_draft();
    new npc_shu_dailo();
    new npc_shu_pool_of_reflection();
    new npc_master_shang_xi_temple();
    new AreaTrigger_at_temple_of_five_dawns();
    new npc_huo();
    new npc_li_fei();
    new npc_aysa_meditation();
    new npc_aysa_cloudsinger();
    new npc_jaomin_ro();
}
