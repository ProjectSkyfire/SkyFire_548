/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "Group.h"
#include "ObjectAccessor.h"

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
            talkCooldown = urand(60000, 120000);
            me->setFaction(7);
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }

        void JustRespawned() OVERRIDE
        {
            Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            switch (urand(0, 3))
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

            talkCooldown = urand(60000, 120000);

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

enum LousPartingThoughts
{
    QUEST_LOUS_PARTING_THOUGHTS = 26232,

    NPC_THUG                    = 42387,
    NPC_LOUS_THOUGHTS_TRIGGER   = 42562,
    NPC_THUGS_EAVESDROPPED      = 42417,

    SAY_WARNING                 = 0,

    DATA_THUG_DEATH             = 1,
    DATA_THUG_EVADE             = 2,

    SOUND_SHOOTING              = 15071,
    SOUND_SCREAM                = 17852
};

enum LousPartingThoughtsEvents
{
    EVENT_THUG1_SAY_0 = 1,
    EVENT_THUG2_SAY_1,
    EVENT_THUG2_SAY_2,
    EVENT_THUG3_SAY_3,
    EVENT_THUG1_SAY_4,
    EVENT_THUG1_SAY_5,
    EVENT_THUG1_SAY_6,
    EVENT_THUG_CREDIT,
    EVENT_THUG_SHOOT_1,
    EVENT_THUG_SHOOT_2,
    EVENT_THUG_SCREAM,
    EVENT_THUG_WARNING
};

class at_westfall_two_shoed_lou_thugs : public AreaTriggerScript
{
public:
    at_westfall_two_shoed_lou_thugs() : AreaTriggerScript("at_westfall_two_shoed_lou_thugs") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
    {
        if (player->IsAlive() && player->GetQuestStatus(QUEST_LOUS_PARTING_THOUGHTS) == QUEST_STATUS_INCOMPLETE)
            if (Creature* trigger = player->FindNearestCreature(NPC_LOUS_THOUGHTS_TRIGGER, 50.0f))
                if (trigger->IsAIEnabled)
                    trigger->AI()->SetGUID(player->GetGUID());

        return true;
    }
};

struct npc_westfall_thug : public ScriptedAI
{
    npc_westfall_thug(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        // The spawn is immune to players until the trigger lets the thugs attack.
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        NotifyTrigger(DATA_THUG_EVADE);
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        NotifyTrigger(DATA_THUG_DEATH);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    void NotifyTrigger(uint32 data)
    {
        if (Creature* trigger = me->FindNearestCreature(NPC_LOUS_THOUGHTS_TRIGGER, 50.0f))
            if (trigger->IsAIEnabled)
                trigger->AI()->SetData(data, 0);
    }
};

struct npc_westfall_lous_parting_thoughts_trigger : public ScriptedAI
{
    npc_westfall_lous_parting_thoughts_trigger(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void SetGUID(uint64 guid, int32 /*id*/) OVERRIDE
    {
        if (_invokerGUID)
            return;

        // The thugs are static spawns: wait until all four are alive and idle again.
        std::list<Creature*> thugs;
        me->GetCreatureListWithEntryInGrid(thugs, NPC_THUG, 20.0f);
        if (thugs.size() != 4)
            return;

        uint8 index = 0;
        for (Creature* thug : thugs)
        {
            if (!thug->IsAlive() || thug->IsInCombat())
                return;

            _thugs[index++] = thug->GetGUID();
        }

        _invokerGUID = guid;
        _events.ScheduleEvent(EVENT_THUG1_SAY_0, 0);
    }

    void SetData(uint32 type, uint32 /*data*/) OVERRIDE
    {
        if (type == DATA_THUG_EVADE)
        {
            _events.Reset();
            Initialize();
        }
        else if (type == DATA_THUG_DEATH && _invokerGUID && ++_thugDeathCount == 4)
            _events.ScheduleEvent(EVENT_THUG_CREDIT, 0);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_THUG1_SAY_0:
                    ThugTalk(0, 0);
                    _events.ScheduleEvent(EVENT_THUG2_SAY_1, 5000);
                    break;
                case EVENT_THUG2_SAY_1:
                    ThugTalk(1, 1);
                    _events.ScheduleEvent(EVENT_THUG2_SAY_2, 5000);
                    break;
                case EVENT_THUG2_SAY_2:
                    ThugTalk(1, 2);
                    _events.ScheduleEvent(EVENT_THUG3_SAY_3, 8500);
                    break;
                case EVENT_THUG3_SAY_3:
                    ThugTalk(2, 3);
                    _events.ScheduleEvent(EVENT_THUG1_SAY_4, 5000);
                    break;
                case EVENT_THUG1_SAY_4:
                {
                    ThugTalk(0, 4);
                    if (Player* invoker = ObjectAccessor::GetPlayer(*me, _invokerGUID))
                        for (uint8 i = 0; i < 4; ++i)
                            if (Creature* thug = ObjectAccessor::GetCreature(*me, _thugs[i]))
                                thug->SetFacingToObject(invoker);

                    _events.ScheduleEvent(EVENT_THUG1_SAY_5, 8500);
                    break;
                }
                case EVENT_THUG1_SAY_5:
                    ThugTalk(0, 5);
                    _events.ScheduleEvent(EVENT_THUG1_SAY_6, 5000);
                    break;
                case EVENT_THUG1_SAY_6:
                {
                    Player* invoker = ObjectAccessor::GetPlayer(*me, _invokerGUID);
                    if (!invoker)
                    {
                        Initialize();
                        break;
                    }

                    ThugTalk(0, 6);
                    for (uint8 i = 0; i < 4; ++i)
                    {
                        if (Creature* thug = ObjectAccessor::GetCreature(*me, _thugs[i]))
                        {
                            thug->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                            thug->AI()->AttackStart(invoker);
                        }
                    }
                    break;
                }
                case EVENT_THUG_CREDIT:
                    GiveCredit();
                    _events.ScheduleEvent(EVENT_THUG_SHOOT_1, 1500);
                    break;
                case EVENT_THUG_SHOOT_1:
                    PlaySound(SOUND_SHOOTING);
                    _events.ScheduleEvent(EVENT_THUG_SHOOT_2, 1200);
                    break;
                case EVENT_THUG_SHOOT_2:
                    PlaySound(SOUND_SHOOTING);
                    _events.ScheduleEvent(EVENT_THUG_SCREAM, 1200);
                    break;
                case EVENT_THUG_SCREAM:
                    PlaySound(SOUND_SCREAM);
                    _events.ScheduleEvent(EVENT_THUG_WARNING, 2500);
                    break;
                case EVENT_THUG_WARNING:
                    if (Player* invoker = ObjectAccessor::GetPlayer(*me, _invokerGUID))
                        Talk(SAY_WARNING, invoker);
                    Initialize();
                    break;
                default:
                    break;
            }
        }
    }

private:
    void Initialize()
    {
        _invokerGUID = 0;
        _thugDeathCount = 0;
    }

    void ThugTalk(uint8 thug, uint8 text)
    {
        Player* invoker = ObjectAccessor::GetPlayer(*me, _invokerGUID);
        Creature* speaker = ObjectAccessor::GetCreature(*me, _thugs[thug]);
        if (invoker && speaker)
            speaker->AI()->Talk(text, invoker);
    }

    void PlaySound(uint32 sound)
    {
        if (Player* invoker = ObjectAccessor::GetPlayer(*me, _invokerGUID))
            me->PlayDistanceSound(sound, invoker);
    }

    void GiveCredit()
    {
        Player* invoker = ObjectAccessor::GetPlayer(*me, _invokerGUID);
        if (!invoker)
            return;

        invoker->KilledMonsterCredit(NPC_THUGS_EAVESDROPPED);

        if (Group* group = invoker->GetGroup())
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if (Player* member = itr->GetSource())
                    if (member->IsInMap(invoker) && member->GetQuestStatus(QUEST_LOUS_PARTING_THOUGHTS) == QUEST_STATUS_INCOMPLETE && member->GetDistance(me) <= 75.0f)
                        member->KilledMonsterCredit(NPC_THUGS_EAVESDROPPED);
    }

    EventMap _events;
    uint64 _invokerGUID;
    uint64 _thugs[4];
    uint8 _thugDeathCount;
};

void AddSC_westfall()
{
    new npc_transient();
    new at_westfall_two_shoed_lou_thugs();
    RegisterCreatureAI(npc_westfall_thug);
    RegisterCreatureAI(npc_westfall_lous_parting_thoughts_trigger);
}
