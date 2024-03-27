/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "Player.h"

class npc_zuni_bridge : public CreatureScript
{
public:
    npc_zuni_bridge() : CreatureScript("npc_zuni_bridge") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_zuni_bridgeAI(creature);
    }
    struct npc_zuni_bridgeAI : public ScriptedAI
    {
        bool talk1 = false;
        bool talk2 = false;
        enum zuniEvents
        {
            EVENT_ZUNI_TALK_1 = 1,
            EVENT_ZUNI_TALK_2 = 2,
        };
        EventMap events;
        npc_zuni_bridgeAI(Creature* creature) : ScriptedAI(creature) { }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type == FOLLOW_MOTION_TYPE)
            {
                if (talk1 == false)
                {
                    talk1 = true;
                    events.ScheduleEvent(EVENT_ZUNI_TALK_1, 5000);
                }
                if (me->FindNearestCreature(37969, 30.0f, true))
                    me->GetMotionMaster()->MovePoint(1, -1553.0f, -5297.35f, 9.1979f);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ZUNI_TALK_1:
                    {
                        me->MonsterSay("You finished with ya trainin' too? Glad you make it through, mon!", Language::LANG_UNIVERSAL, me);
                        me->SendPlaySound(21370, false);
                        if (talk2 == false)
                        {
                            talk2 = true;
                            events.ScheduleEvent(EVENT_ZUNI_TALK_2, 5000);
                        }
                        break;
                    }
                    case EVENT_ZUNI_TALK_2:
                    {
                        me->MonsterSay("I know dis is prolly busy work, but I don't mind. Dese baby raptors are cute lil devils.", Language::LANG_UNIVERSAL, me);
                        me->SendPlaySound(21371, false);
                        break;
                    }
                }
            }
        }
    };
};

class npc_voljin_darkspear_hold : public CreatureScript
{
public:
    npc_voljin_darkspear_hold() : CreatureScript("npc_voljin_darkspear_hold") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_voljin_darkspear_holdAI(creature);
    }

    bool OnQuestReward(Player* /*player*/, Creature* creature, Quest const* quest, uint32 /*opt*/) OVERRIDE
    {
        if (quest->GetQuestId() == 24643 || quest->GetQuestId() == 24755 ||
            quest->GetQuestId() == 24763 || quest->GetQuestId() == 24769 ||
            quest->GetQuestId() == 24775 || quest->GetQuestId() == 24781 ||
            quest->GetQuestId() == 24787 || quest->GetQuestId() == 31163)
        {
            creature->GetMotionMaster()->MovePoint(0, creature->GetHomePosition());
            return true;
        }
        return false;
    }

    struct npc_voljin_darkspear_holdAI : public ScriptedAI
    {
        enum voljinEvents
        {
            EVENT_VOLJIN_INTRO = 1,
            EVENT_GARROSH_VISION1 = 2,
            EVENT_VOLJIN_VISION1 = 3,
            EVENT_VOLJIN_VISION2 = 4,
            EVENT_GARROSH_VISION2 = 5,
            EVENT_VOLJIN_VISION3 = 6,
            EVENT_GARROSH_VISION3 = 7,
            EVENT_VOLJIN_VISION4 = 8,
            EVENT_VOLJIN_VISION5 = 9,
            EVENT_VOLJIN_VISION6 = 10,
            EVENT_GARROSH_VISION4 = 11,
            EVENT_VOLJIN_VISION7 = 12,
            EVENT_VOLJIN_TALK1 = 13,
            EVENT_VOLJIN_TALK2 = 14,
            EVENT_VOLJIN_OUTRO = 15
        };
        EventMap events;
        npc_voljin_darkspear_holdAI(Creature* creature) : ScriptedAI(creature) { }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE && id == 0)
            {
                events.ScheduleEvent(EVENT_VOLJIN_INTRO, 1000);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_VOLJIN_INTRO:
                    {
                        //clear visions.
                        if (Creature* voljinVision = me->FindNearestCreature(38953, 15.0f))
                            voljinVision->DespawnOrUnsummon(1000);
                        if (Creature* garroshVision = me->FindNearestCreature(38938, 15.0f))
                            garroshVision->DespawnOrUnsummon(1000);
                        
                        //TODO: Let him cast spell here
                        me->MonsterSay("I have sometin' to show ya. It be easier to understand if ya see it yourself.", Language::LANG_UNIVERSAL, me);
                        me->SendPlaySound(20097, false);
                        events.ScheduleEvent(EVENT_GARROSH_VISION1, 8000);
                        break;
                    }
                    case EVENT_GARROSH_VISION1:
                    {
                        if (Creature* garroshVision = me->SummonCreature(38938, -1321.64f, -5610.25f, 25.4541f, 2.46091, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 130000))
                        {
                            garroshVision->MonsterSay("Don't talk back to me, troll. You know who was left in charge here. Haven't you stopped to ask yourself why Thrall chose me instead of you?", Language::LANG_UNIVERSAL, garroshVision);
                            me->SendPlaySound(20508, false);
                        }
                        events.ScheduleEvent(EVENT_VOLJIN_VISION1, 12000);
                        break;
                    }
                    case EVENT_VOLJIN_VISION1:
                    {
                        if (Creature* voljinVision = me->SummonCreature(38953, -1323.86f, -5608.56f, 25.4523f, 5.48033f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 130000))
                        {
                            voljinVision->MonsterSay("Dere be no question why, Garrosh. He gave ya tha title because ya be Grom's son and because tha people be wantin' a war hero.", Language::LANG_UNIVERSAL, voljinVision);
                            me->SendPlaySound(20098, false);
                        }
                        events.ScheduleEvent(EVENT_VOLJIN_VISION2, 13000);
                        break;
                    }
                    case EVENT_VOLJIN_VISION2:
                    {
                        if (Creature* voljinVision = me->FindNearestCreature(38953, 15.0f))
                        {
                            voljinVision->MonsterSay("I think ya be even more like ya father den ya thought, even witout da demon blood.", Language::LANG_UNIVERSAL, voljinVision);
                            me->SendPlaySound(20099, false);
                        }
                            
                        events.ScheduleEvent(EVENT_GARROSH_VISION2, 7500);
                        break;
                    }
                    case EVENT_GARROSH_VISION2:
                    {
                        if (Creature* garroshVision = me->FindNearestCreature(38938, 15.0f))
                        {
                            garroshVision->MonsterSay("You are lucky that i don't gut you right here, whelp. You are foolish to think that you can speak to your Warchief in such ways.", Language::LANG_UNIVERSAL, garroshVision);
                            me->SendPlaySound(20512, false);
                        }
                            
                        events.ScheduleEvent(EVENT_VOLJIN_VISION3, 11000);
                        break;
                    }
                    case EVENT_VOLJIN_VISION3:
                    {
                        if (Creature* voljinVision = me->FindNearestCreature(38953, 15.0f))
                        {
                            voljinVision->MonsterSay("Ya be no Warchief of mine. Ya not earned my respect and I'll not be seein' tha Horde destroyed by ya foolish thirst for War.", Language::LANG_UNIVERSAL, voljinVision);
                            me->SendPlaySound(20100, false);
                        }
                        events.ScheduleEvent(EVENT_GARROSH_VISION3, 12000);
                        break;
                    }
                    case EVENT_GARROSH_VISION3:
                    {
                        if (Creature* garroshVision = me->FindNearestCreature(38938, 15.0f))
                        {
                            garroshVision->MonsterSay("And what exactly do you think that you are going to do about it? Your threats are hollow. Go slink away with the rest of your kind to the slums, I will endure your filth in my throne room no longer.", Language::LANG_UNIVERSAL, garroshVision);
                            me->SendPlaySound(20509, false);
                        }
                            
                        events.ScheduleEvent(EVENT_VOLJIN_VISION4, 15000);
                        break;
                    }
                    case EVENT_VOLJIN_VISION4:
                    {
                        if (Creature* voljinVision = me->FindNearestCreature(38953, 15.0f))
                        {
                            voljinVision->MonsterSay("I know exactly what I'll be doin' about it, son of Hellscream. I'll watch and wait as ya people slowly become aware of ya ineptitude. I'll laugh as dey grow ta despise ya as I do.", Language::LANG_UNIVERSAL, voljinVision);
                            me->SendPlaySound(20101, false);
                        }
                           
                        events.ScheduleEvent(EVENT_VOLJIN_VISION5, 15000);
                        break;
                    }
                    case EVENT_VOLJIN_VISION5:
                    {
                        if (Creature* voljinVision = me->FindNearestCreature(38953, 15.0f))
                        {
                            voljinVision->MonsterSay("And when tha time commes dat ya failure is complete and ya \"power\" is meaningless, I will be dere to end ya rule swiftly and silently.", Language::LANG_UNIVERSAL, voljinVision);
                            me->SendPlaySound(20102, false);
                        }
                        events.ScheduleEvent(EVENT_VOLJIN_VISION6, 15000);
                        break;
                    }
                    case EVENT_VOLJIN_VISION6:
                    {
                        if (Creature* voljinVision = me->FindNearestCreature(38953, 15.0f))
                        {
                            voljinVision->MonsterSay("Ya will spend ya reign glancin' over ya shoulda and fearin' tha shadows, for when tha time comes and ya blood be slowly drainin' out, ya will know exactly who fired tha arrow dat pierced ya black heart.", Language::LANG_UNIVERSAL, voljinVision);
                            me->SendPlaySound(20103, false);
                        }
                        events.ScheduleEvent(EVENT_GARROSH_VISION4, 18000);
                        break;
                    }
                    case EVENT_GARROSH_VISION4:
                    {
                        if (Creature* garroshVision = me->FindNearestCreature(38938, 15.0f))
                        {
                            garroshVision->MonsterSay("You have sealed your fate, troll.", Language::LANG_UNIVERSAL, garroshVision);
                            me->SendPlaySound(20510, false);
                        }
                        events.ScheduleEvent(EVENT_VOLJIN_VISION7, 5000);
                        break;
                    }
                    case EVENT_VOLJIN_VISION7:
                    {
                        if (Creature* voljinVision = me->FindNearestCreature(38953, 15.0f))
                        {
                            voljinVision->MonsterSay("And you yours, \"Warchief.\"", Language::LANG_UNIVERSAL, voljinVision);
                            me->SendPlaySound(20104, false);
                        }

                        events.ScheduleEvent(EVENT_VOLJIN_TALK1, 10000);
                        break;
                    }
                    case EVENT_VOLJIN_TALK1:
                    {
                        if (Creature* voljinVision = me->FindNearestCreature(38953, 15.0f))
                            voljinVision->DespawnOrUnsummon(1000);
                        if (Creature* garroshVision = me->FindNearestCreature(38938, 15.0f))
                            garroshVision->DespawnOrUnsummon(1000);

                        me->MonsterSay("The Darkspear are 'ere because I led dem here. Orgrimmar be no home as long as it be under Hellscream's hand.", Language::LANG_UNIVERSAL, me);
                        me->SendPlaySound(20105, false);
                        events.ScheduleEvent(EVENT_VOLJIN_TALK2, 8000);
                        break;
                    }
                    case EVENT_VOLJIN_TALK2:
                    {
                        me->MonsterSay("Still. I fear I was lettin' my temper drive me ta bein'rash. Threall devoted himself to makin' the Horde what it is, so I've no eagerness to be leavin' it on a whim. Dis will be needin' much more thought.", Language::LANG_UNIVERSAL, me);
                        me->SendPlaySound(20106, false);
                        events.ScheduleEvent(EVENT_VOLJIN_OUTRO, 16000);
                        break;
                    }
                    case EVENT_VOLJIN_OUTRO:
                    {
                        me->MonsterSay("But dese be worries for older minds. Ya still have much to learn. Go help tha people of tha Darkspear. I am sure we'll be speakin' again real soon.", Language::LANG_UNIVERSAL, me);
                        me->SendPlaySound(20107, false);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

    };
};


enum darkspearshore
{
    NPC_ZUNI = 37988
};
Position const ZuniStartPos = { -1173.345f, -5267.1723f, 0.909172f, 0 };
Position const ZuniMovePos1 = { -1172.816f, -5299.0522f, 5.1754074f, 0 };
Position const ZuniMovePos2 = { -1176.1216f, -5326.733f, 13.596682f, 0 };
Position const ZuniMovePos3 = { -1172.816f, -5344.54f, 15.331013f, 0 };
Position const ZuniMovePos4 = { -1164.625f, -5369.5225f, 14.166052f, 0 };
Position const ZuniMovePos5 = { -1162.0435f, -5391.7466f, 12.003188f };
/*####
# npc_zuni
####*/
class npc_zuni : public CreatureScript
{
public:
    npc_zuni() : CreatureScript("npc_zuni") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_zuniAI(creature);
    }

    struct npc_zuniAI : public ScriptedAI
    {
        bool wp1 = false;
        bool wp2 = false;
        bool wp3 = false;
        bool wp4 = false;
        bool wp5 = false;
        bool wp6 = false;
        uint32 MoveTimer = 5000;

        npc_zuniAI(Creature* creature) : ScriptedAI(creature) { }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE)
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
            else
                return;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (wp1 == true)
            {
                if (MoveTimer <= diff)
                {
                    me->GetMotionMaster()->MovePoint(1, ZuniMovePos1);
                    wp1 = false;
                    MoveTimer = 5000;
                }
                else MoveTimer -= diff;
            }

            if (wp2 == true)
            {
                me->GetMotionMaster()->MovePoint(2, ZuniMovePos2);
                wp2 = false;
            }

            if (wp3 == true)
            {
                me->GetMotionMaster()->MovePoint(3, ZuniMovePos3);
                wp3 = false;
            }

            if (wp4 == true)
            {
                me->GetMotionMaster()->MovePoint(4, ZuniMovePos4);
                wp4 = false;
            }

            if (wp5 == true)
            {
                me->GetMotionMaster()->MovePoint(5, ZuniMovePos5);
                wp5 = false;
            }

            if (wp6 == true)
            {
                me->MonsterSay("Ya trainer should be somewhere in the grounds'ere. I'll catch you lata mon.", Language::LANG_UNIVERSAL, me);
                me->SendPlaySound(21367, true);
                me->DespawnOrUnsummon(10000);
                wp6 = false;
            }
        }
    };
};

/*####
# npc_jinthala
####*/
class npc_jinthala : public CreatureScript
{
public:
    npc_jinthala() : CreatureScript("npc_jinthala") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_jinthalaAI(creature);
    }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* /*quest*/) OVERRIDE
    {
        creature->MonsterSay("$n. Zuni. Ya'll find ya trainer in the trainin' grounds to the east. Bring pride to the Darkspear.", Language::LANG_UNIVERSAL, player);
        
        if (Creature* zuni = player->FindNearestCreature(NPC_ZUNI, 10.0f, true))
        {
            zuni->MonsterSay("Ya. mon. Let's crack some tiki target skulls! ", Language::LANG_UNIVERSAL, player);
            zuni->SendPlaySound(21366, true);
            zuni->GetMotionMaster()->MovePoint(0, ZuniStartPos);
            return true;
        }
        return false;
    }

    struct npc_jinthalaAI : public ScriptedAI
    {
        npc_jinthalaAI(Creature* creature) : ScriptedAI(creature) { }
    };
};

/*######
## Quest: Proving Pit: 24642, 24754, 24762, 24768, 24774, 24780, 24786, 26276, 31161
## npc_darkspear_jailor
######*/

Position DarkSpearJailorPos = { -1153.53f, -5519.42f, 11.98f, 6.27f };
Position DarkSpearJailorPos2 = { -1135.68f, -5417.78f, 13.26f, 1.47f };
Position NagaPos = { -1149.90f, -5527.76f, 8.10f, 4.76f };
Position NagaPos2 = { -1141.74f, -5414.54f, 10.59f, 3.42f };

class npc_darkspear_jailor : public CreatureScript
{
public:
    npc_darkspear_jailor() : CreatureScript("npc_darkspear_jailor") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_darkspear_jailorAI(creature);
    }

    struct npc_darkspear_jailorAI : public ScriptedAI
    {
        enum JailorEvents
        {
            EVENT_JAILOR_OPENING = 1,
            EVENT_JAILOR_OPEN_CAGE = 2,
            EVENT_JAILOR_RESET = 3,
        };
        EventMap events;
        npc_darkspear_jailorAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
        }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case 0:
                {
                    events.ScheduleEvent(EVENT_JAILOR_OPENING, 1000);
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
                    case EVENT_JAILOR_OPENING:
                    {
                        me->HandleEmoteCommand(EMOTE_ONESHOT_USE_STANDING);
                        events.ScheduleEvent(EVENT_JAILOR_OPEN_CAGE, 1000);
                        break;
                    }
                    case EVENT_JAILOR_OPEN_CAGE:
                    {
                        if (GameObject* cage = me->FindNearestGameObject(201968, 5.0f))
                            cage->UseDoorOrButton(120);

                        if (Creature* monk = me->FindNearestCreature(63310, 25.0f))
                        {
                            if (Creature* Naga = me->FindNearestCreature(38142, 5.0f))
                            {
                                Naga->setFaction(14);
                                Naga->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                Naga->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                Naga->GetMotionMaster()->MovePoint(0, NagaPos2);
                                Naga->MonsterSay("I sshal ssslaughter you, Darksspear runt!", Language::LANG_UNIVERSAL, 0);
                            }
                        }
                        else
                        {
                            if (Creature* Naga2 = me->FindNearestCreature(38142, 5.0f))
                            {
                                Naga2->setFaction(14);
                                Naga2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                Naga2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                Naga2->GetMotionMaster()->MovePoint(0, NagaPos);
                                Naga2->MonsterSay("I sshal ssslaughter you, Darksspear runt!", Language::LANG_UNIVERSAL, 0);
                            }
                        }

                        events.ScheduleEvent(EVENT_JAILOR_RESET, 120000);
                        break;
                    }
                    case EVENT_JAILOR_RESET:
                    {
                        me->GetMotionMaster()->MovePoint(1, me->GetHomePosition());
                        Reset();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    };

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF)
        {
            player->CLOSE_GOSSIP_MENU();
            if (Creature* monk = creature->FindNearestCreature(63310, 25.0f))
            {
                creature->GetMotionMaster()->MovePoint(0, DarkSpearJailorPos2);
            }
            else
            {
                creature->GetMotionMaster()->MovePoint(0, DarkSpearJailorPos);
            }
            const char* text = player->getGender() == GENDER_MALE ? "Get in the pit and show us your stuff, boy." : "Get in the pit and show us your stuff, girl.";
            creature->MonsterSay(text, Language::LANG_UNIVERSAL, creature);
            player->KilledMonsterCredit(39062);            
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I'm ready to face my challenge.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};



/*######
## Quest 25134: Lazy Peons
## npc_lazy_peon
######*/

enum LazyPeonYells
{
    SAY_SPELL_HIT                                 = 0
};

enum LazyPeon
{
    QUEST_LAZY_PEONS    = 25134,
    GO_LUMBERPILE       = 175784,
    SPELL_BUFF_SLEEP    = 17743,
    SPELL_AWAKEN_PEON   = 19938
};

class npc_lazy_peon : public CreatureScript
{
public:
    npc_lazy_peon() : CreatureScript("npc_lazy_peon") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lazy_peonAI(creature);
    }

    struct npc_lazy_peonAI : public ScriptedAI
    {
        npc_lazy_peonAI(Creature* creature) : ScriptedAI(creature) { }

        uint64 PlayerGUID;

        uint32 RebuffTimer;
        bool work;

        void Reset() OVERRIDE
        {
            PlayerGUID = 0;
            RebuffTimer = 0;
            work = false;
        }

        void MovementInform(uint32 /*type*/, uint32 id) OVERRIDE
        {
            if (id == 1)
                work = true;
        }

        void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
        {
            if (spell->Id != SPELL_AWAKEN_PEON)
                return;

            Player* player = caster->ToPlayer();
            if (player && player->GetQuestStatus(QUEST_LAZY_PEONS) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
                Talk(SAY_SPELL_HIT, caster);
                me->RemoveAllAuras();
                if (GameObject* Lumberpile = me->FindNearestGameObject(GO_LUMBERPILE, 20))
                    me->GetMotionMaster()->MovePoint(1, Lumberpile->GetPositionX()-1, Lumberpile->GetPositionY(), Lumberpile->GetPositionZ());
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (work == true)
                me->HandleEmoteCommand(EMOTE_ONESHOT_WORK_CHOPWOOD);
            if (RebuffTimer <= diff)
            {
                DoCast(me, SPELL_BUFF_SLEEP);
                RebuffTimer = 300000; //Rebuff agian in 5 minutes
            }
            else
                RebuffTimer -= diff;
            if (!UpdateVictim())
                return;
            DoMeleeAttackIfReady();
        }
    };
};

enum VoodooSpells
{
    SPELL_BREW      = 16712, // Special Brew
    SPELL_GHOSTLY   = 16713, // Ghostly
    SPELL_HEX1      = 16707, // Hex
    SPELL_HEX2      = 16708, // Hex
    SPELL_HEX3      = 16709, // Hex
    SPELL_GROW      = 16711, // Grow
    SPELL_LAUNCH    = 16716, // Launch (Whee!)
};

// 17009
class spell_voodoo : public SpellScriptLoader
{
    public:
        spell_voodoo() : SpellScriptLoader("spell_voodoo") { }

        class spell_voodoo_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_voodoo_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_BREW) || !sSpellMgr->GetSpellInfo(SPELL_GHOSTLY) ||
                    !sSpellMgr->GetSpellInfo(SPELL_HEX1) || !sSpellMgr->GetSpellInfo(SPELL_HEX2) ||
                    !sSpellMgr->GetSpellInfo(SPELL_HEX3) || !sSpellMgr->GetSpellInfo(SPELL_GROW) ||
                    !sSpellMgr->GetSpellInfo(SPELL_LAUNCH))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint32 spellid = RAND(SPELL_BREW, SPELL_GHOSTLY, RAND(SPELL_HEX1, SPELL_HEX2, SPELL_HEX3), SPELL_GROW, SPELL_LAUNCH);
                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, spellid, false);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_voodoo_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_voodoo_SpellScript();
        }
};

// 3125
class npc_clattering_scorpid : public CreatureScript
{
    
public:
    npc_clattering_scorpid() : CreatureScript("npc_clattering_scorpid") { }


    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_clattering_scorpidAI(creature);
    }

    struct npc_clattering_scorpidAI : public ScriptedAI
    {
        enum clatteringScorpidEvents
        {
            EVENT_CAST_VENOM = 1,
            EVENT_KC_TOTEM = 2
        };
        EventMap events;
        npc_clattering_scorpidAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_CAST_VENOM, 2000);
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
                    case EVENT_CAST_VENOM:
                    {
                        if (!me->FindNearestCreature(39236, 5.0f))
                            Talk(0, me);
            
                        me->CastSpell(me->GetVictim(), 73672);
                        events.ScheduleEvent(EVENT_KC_TOTEM, 2500);
                        events.ScheduleEvent(EVENT_CAST_VENOM, 7500);
                        break;
                    }
                    case EVENT_KC_TOTEM:
                    {
                        if (Creature* totem = me->FindNearestCreature(39236, 5.0f))
                            totem->GetOwner()->ToPlayer()->KilledMonsterCredit(39236);
                        break;
                    }
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

// 73705 - Injured Razor Hill Grunt - Cancel Feign Death
class spell_injured_razor_hill_grunt_cancel_feign_death : public SpellScript
{
    PrepareSpellScript(spell_injured_razor_hill_grunt_cancel_feign_death);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            if (target->getStandState() == UNIT_STAND_STATE_DEAD)
                target->SetStandState(UNIT_STAND_STATE_STAND);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_injured_razor_hill_grunt_cancel_feign_death::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

enum NpcInjuredRazorHillGruntUtilities
{
    EVENT_TEXT_BEFORE_HELP = 1,
    EVENT_AFTER_HELP       = 2,
    EVENT_DESPAWN          = 3,

    TEXT_BEFORE_HELP       = 0,
    TEXT_AFTER_HELP        = 1,
    TEXT_DESPAWN           = 2,

    QUEST_LOSS_REDUCTION   = 25179
};

// 39270 - Injured Razor Hill Grunt
struct npc_injured_razor_hill_grunt : public ScriptedAI
{
    npc_injured_razor_hill_grunt(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        me->setRegeneratingHealth(false);
        me->SetHealth(me->CountPctFromMaxHealth(10));
        _events.ScheduleEvent(EVENT_TEXT_BEFORE_HELP, 60 * IN_MILLISECONDS, 120 * IN_MILLISECONDS);
    }

    void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
    {
        if (Player* player = clicker->ToPlayer())
        {
            _player = player;
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            me->GetMotionMaster()->MoveFollow(player, 0.f, 0.f);
            player->KilledMonsterCredit(me->GetEntry());
            _events.ScheduleEvent(EVENT_AFTER_HELP, 2 * IN_MILLISECONDS);

            if (Quest const* quest = sObjectMgr->GetQuestTemplate(QUEST_LOSS_REDUCTION))
                if (player->GetQuestStatus(quest->GetQuestId()) == QUEST_STATUS_NONE)
                {
                    player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, me->GetGUID(), true);
                    player->AddQuest(quest, NULL);
                }
        }
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_TEXT_BEFORE_HELP:
                {
                    Talk(TEXT_BEFORE_HELP);
                    _events.ScheduleEvent(EVENT_TEXT_BEFORE_HELP, 60 * IN_MILLISECONDS, 120 * IN_MILLISECONDS);
                    break;
                }
                case EVENT_AFTER_HELP:
                {
                    Talk(TEXT_AFTER_HELP, _player);
                    _events.CancelEvent(EVENT_TEXT_BEFORE_HELP);
                    _events.ScheduleEvent(EVENT_DESPAWN, 60 * IN_MILLISECONDS, 80 * IN_MILLISECONDS);
                    break;
                }
                case EVENT_DESPAWN:
                {
                    Talk(TEXT_DESPAWN, _player);
                    me->GetMotionMaster()->MovementExpired(true);
                    me->DespawnOrUnsummon(3 * IN_MILLISECONDS);
                    break;
                }
                default:
                    break;
            }
        }
    }

private:
    Player* _player;
    EventMap _events;
};

void AddSC_durotar()
{
    new npc_zuni_bridge();
    new npc_voljin_darkspear_hold();
    new npc_zuni();
    new npc_jinthala();
    new npc_darkspear_jailor();
    new npc_lazy_peon();
    new spell_voodoo();
    new npc_clattering_scorpid();
    RegisterCreatureAI(npc_injured_razor_hill_grunt);
    RegisterSpellScript(spell_injured_razor_hill_grunt_cancel_feign_death);
}
