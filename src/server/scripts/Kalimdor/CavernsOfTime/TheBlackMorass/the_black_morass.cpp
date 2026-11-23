/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
Name: The_Black_Morass
%Complete: 30
Comment: Misc NPC's and mobs for instance. Most here far from complete.
Category: Caverns of Time, The Black Morass
*/

/* ContentData
npc_medivh_bm
npc_time_rift
npc_saat
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "the_black_morass.h"
#include "Player.h"
#include "SpellInfo.h"

enum MedivhBm
{
    SAY_ENTER               = 0,                    //where does this belong?
    SAY_INTRO               = 1,
    SAY_WEAK75              = 2,
    SAY_WEAK50              = 3,
    SAY_WEAK25              = 4,
    SAY_DEATH               = 5,
    SAY_WIN                 = 6,
    SAY_ORCS_ENTER          = 7,
    SAY_ORCS_ANSWER         = 8,

    SPELL_CHANNEL           = 31556,
    SPELL_PORTAL_RUNE       = 32570,                      //aura(portal on ground effect)

    SPELL_BLACK_CRYSTAL     = 32563,                      //aura
    SPELL_PORTAL_CRYSTAL    = 32564,                      //summon

    SPELL_BANISH_PURPLE     = 32566,                      //aura
    SPELL_BANISH_GREEN      = 32567,                      //aura

    SPELL_CORRUPT           = 31326,
    SPELL_CORRUPT_AEONUS    = 37853,

    C_COUNCIL_ENFORCER      = 17023,

    GO_DARK_PORTAL          = 185103
};

enum MedivhEvents
{
    EVENT_EPILOGUE_1        = 1,
    EVENT_EPILOGUE_2,
    EVENT_EPILOGUE_3,
    EVENT_EPILOGUE_4,
    EVENT_EPILOGUE_5,
    EVENT_EPILOGUE_6,
    EVENT_EPILOGUE_7,
    EVENT_EPILOGUE_8
};

// The Shadow Council Enforcers come through the portal in four rows of six
// (sniffed positions of TrinityCore 3.3.5, one row per spawn group)
float const EnforcerRows[4][6][4] =
{
    {
        {-2086.85f, 7132.72f, 34.67f, 6.126f}, {-2087.19f, 7129.97f, 34.67f, 6.144f}, {-2087.48f, 7127.27f, 34.67f, 6.161f},
        {-2087.80f, 7124.51f, 34.67f, 6.178f}, {-2088.08f, 7121.89f, 34.67f, 6.196f}, {-2088.39f, 7119.38f, 34.67f, 6.213f}
    },
    {
        {-2086.57f, 7132.63f, 34.67f, 6.126f}, {-2086.98f, 7129.97f, 34.67f, 6.144f}, {-2087.41f, 7127.20f, 34.67f, 6.161f},
        {-2087.88f, 7124.38f, 34.67f, 6.178f}, {-2088.22f, 7121.99f, 34.67f, 6.196f}, {-2088.60f, 7119.24f, 34.67f, 6.213f}
    },
    {
        {-2086.51f, 7132.68f, 34.67f, 6.126f}, {-2086.95f, 7129.99f, 34.67f, 6.144f}, {-2087.31f, 7127.19f, 34.67f, 6.161f},
        {-2087.65f, 7124.36f, 34.67f, 6.178f}, {-2088.01f, 7121.87f, 34.67f, 6.196f}, {-2088.50f, 7119.22f, 34.67f, 6.213f}
    },
    {
        {-2086.46f, 7132.61f, 34.67f, 6.150f}, {-2086.88f, 7129.97f, 34.67f, 6.097f}, {-2087.15f, 7127.20f, 34.67f, 6.089f},
        {-2087.49f, 7124.41f, 34.67f, 6.144f}, {-2087.89f, 7121.92f, 34.67f, 6.139f}, {-2088.39f, 7119.20f, 34.67f, 6.178f}
    }
};

class npc_medivh_bm : public CreatureScript
{
public:
    npc_medivh_bm() : CreatureScript("npc_medivh_bm") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_medivh_bmAI(creature);
    }

    struct npc_medivh_bmAI : public ScriptedAI
    {
        npc_medivh_bmAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 SpellCorrupt_Timer;
        uint32 Check_Timer;

        bool Life75;
        bool Life50;
        bool Life25;
        bool silentDeath;

        EventMap events;

        void Reset() OVERRIDE
        {
            SpellCorrupt_Timer = 0;
            Check_Timer = 0;
            Life75 = true;
            Life50 = true;
            Life25 = true;
            silentDeath = false;
            events.Reset();

            if (!instance)
                return;

            if (instance->GetData(TYPE_MEDIVH) == IN_PROGRESS)
                DoCast(me, SPELL_CHANNEL, true);
            else if (me->HasAura(SPELL_CHANNEL))
                me->RemoveAura(SPELL_CHANNEL);

            DoCast(me, SPELL_PORTAL_RUNE, true);
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE

        {
            if (!instance)
                return;

            if (who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(who, 10.0f))
            {
                if (instance->GetData(TYPE_MEDIVH) == IN_PROGRESS || instance->GetData(TYPE_MEDIVH) == DONE)
                    return;

                Talk(SAY_INTRO);
                instance->SetData(TYPE_MEDIVH, IN_PROGRESS);
                DoCast(me, SPELL_CHANNEL, false);
                Check_Timer = 5000;
            }
            else if (who->GetTypeId() == TypeID::TYPEID_UNIT && me->IsWithinDistInMap(who, 15.0f))
            {
                if (instance->GetData(TYPE_MEDIVH) != IN_PROGRESS)
                    return;

                uint32 entry = who->GetEntry();
                if (entry == NPC_INFINITE_ASSASIN || entry == NPC_INFINITE_WHELP || entry == NPC_INFINITE_CRONOMANCER || entry == NPC_INFINITE_EXECUTIONER || entry == NPC_INFINITE_VANQUISHER)
                {
                    who->StopMoving();
                    who->CastSpell(me, SPELL_CORRUPT, false);
                }
                else if (entry == NPC_AEONUS)
                {
                    who->StopMoving();
                    who->CastSpell(me, SPELL_CORRUPT_AEONUS, false);
                }
            }
        }

        void AttackStart(Unit* /*who*/) OVERRIDE
        {
            //if (instance && instance->GetData(TYPE_MEDIVH) == IN_PROGRESS)
            //return;

            //ScriptedAI::AttackStart(who);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell) OVERRIDE
        {
            if (SpellCorrupt_Timer)
                return;

            if (spell->Id == SPELL_CORRUPT_AEONUS)
                SpellCorrupt_Timer = 1000;

            if (spell->Id == SPELL_CORRUPT)
                SpellCorrupt_Timer = 3000;
        }

        // Everything the event left behind goes when it ends
        void DoDespawnSpawns()
        {
            static uint32 const entries[] =
            {
                NPC_TIME_RIFT, NPC_RIFT_LORD, NPC_RIFT_KEEPER, NPC_CRONO_LORD_DEJA, NPC_TEMPORUS, NPC_AEONUS,
                NPC_INFINITE_ASSASIN, NPC_INFINITE_WHELP, NPC_INFINITE_CRONOMANCER, NPC_INFINITE_EXECUTIONER, NPC_INFINITE_VANQUISHER
            };

            for (uint32 entry : entries)
            {
                std::list<Creature*> spawns;
                me->GetCreatureListWithEntryInGrid(spawns, entry, 500.0f);
                for (Creature* spawn : spawns)
                    if (spawn->IsAlive())
                        spawn->DespawnOrUnsummon();
            }
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            DoDespawnSpawns();

            // the shield ran out; the silent death is the reset of an abandoned event
            if (!silentDeath)
                Talk(SAY_DEATH);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!instance)
                return;

            if (SpellCorrupt_Timer)
            {
                if (SpellCorrupt_Timer <= diff)
                {
                    instance->SetData(TYPE_MEDIVH, SPECIAL);

                    if (me->HasAura(SPELL_CORRUPT_AEONUS))
                        SpellCorrupt_Timer = 1000;
                    else if (me->HasAura(SPELL_CORRUPT))
                        SpellCorrupt_Timer = 3000;
                    else
                        SpellCorrupt_Timer = 0;
                } else SpellCorrupt_Timer -= diff;
            }

            if (Check_Timer)
            {
                if (Check_Timer <= diff)
                {
                    uint32 pct = instance->GetData(DATA_SHIELD);

                    Check_Timer = 5000;

                    if (Life25 && pct <= 25)
                    {
                        Talk(SAY_WEAK25);
                        Life25 = false;
                    }
                    else if (Life50 && pct <= 50)
                    {
                        Talk(SAY_WEAK50);
                        Life50 = false;
                    }
                    else if (Life75 && pct <= 75)
                    {
                        Talk(SAY_WEAK75);
                        Life75 = false;
                    }

                    //if we reach this it means event was running but at some point reset.
                    if (instance->GetData(TYPE_MEDIVH) == NOT_STARTED)
                    {
                        silentDeath = true;
                        me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        me->RemoveCorpse();
                        me->Respawn();
                        return;
                    }

                    if (instance->GetData(TYPE_RIFT) == DONE)
                    {
                        Talk(SAY_WIN);
                        Check_Timer = 0;

                        if (me->HasAura(SPELL_CHANNEL))
                            me->RemoveAura(SPELL_CHANNEL);

                        instance->SetData(TYPE_MEDIVH, DONE);
                        events.ScheduleEvent(EVENT_EPILOGUE_1, 1);
                    }
                } else Check_Timer -= diff;
            }

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_EPILOGUE_1:
                        DoDespawnSpawns();
                        events.ScheduleEvent(EVENT_EPILOGUE_2, 10000);
                        break;
                    case EVENT_EPILOGUE_2:
                        me->SetFacingTo(6.2657318f);
                        Talk(SAY_WIN);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                        me->SummonGameObject(GO_DARK_PORTAL, -2087.3372f, 7125.657f, 29.942312f, 6.143561f, 0.0f, 0.0f, -0.069755554f, 0.99756414f, 0);
                        events.ScheduleEvent(EVENT_EPILOGUE_3, 18000);
                        break;
                    case EVENT_EPILOGUE_3:
                        me->SetFacingTo(3.0019662f);
                        me->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, 0);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                        events.ScheduleEvent(EVENT_EPILOGUE_4, 1000);
                        break;
                    case EVENT_EPILOGUE_4:
                    case EVENT_EPILOGUE_5:
                    case EVENT_EPILOGUE_6:
                    case EVENT_EPILOGUE_7:
                        for (uint8 i = 0; i < 6; ++i)
                        {
                            float const* pos = EnforcerRows[eventId - EVENT_EPILOGUE_4][i];
                            me->SummonCreature(C_COUNCIL_ENFORCER, pos[0], pos[1], pos[2], pos[3], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 600000);
                        }
                        events.ScheduleEvent(eventId + 1, eventId == EVENT_EPILOGUE_4 ? 2000 : (eventId == EVENT_EPILOGUE_7 ? 8000 : 2400));
                        break;
                    case EVENT_EPILOGUE_8:
                        Talk(SAY_ORCS_ENTER);
                        break;
                    default:
                        break;
                }
            }
        }
    };
};

struct Wave
{
    uint32 PortalMob[4];                                    //spawns for portal waves (in order)
};

static Wave PortalWaves[]=
{
    { {NPC_INFINITE_ASSASIN, NPC_INFINITE_WHELP, NPC_INFINITE_CRONOMANCER, 0} },
    { {NPC_INFINITE_EXECUTIONER, NPC_INFINITE_CRONOMANCER, NPC_INFINITE_WHELP, NPC_INFINITE_ASSASIN} },
    { {NPC_INFINITE_EXECUTIONER, NPC_INFINITE_VANQUISHER, NPC_INFINITE_CRONOMANCER, NPC_INFINITE_ASSASIN} }
};

class npc_time_rift : public CreatureScript
{
public:
    npc_time_rift() : CreatureScript("npc_time_rift") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_time_riftAI(creature);
    }

    struct npc_time_riftAI : public ScriptedAI
    {
        npc_time_riftAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 TimeRiftWave_Timer;
        uint8 mRiftWaveCount;
        uint8 mPortalCount;
        uint8 mWaveId;

        void Reset() OVERRIDE
        {
            TimeRiftWave_Timer = 15000;
            mRiftWaveCount = 0;

            if (!instance)
                return;

            mPortalCount = instance->GetData(DATA_PORTAL_COUNT);

            if (mPortalCount < 6)
                mWaveId = 0;
            else if (mPortalCount > 12)
                mWaveId = 2;
            else mWaveId = 1;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void DoSummonAtRift(uint32 creature_entry)
        {
            if (!creature_entry)
                return;

            if (instance && instance->GetData(TYPE_MEDIVH) != IN_PROGRESS)
            {
                me->InterruptNonMeleeSpells(true);
                me->RemoveAllAuras();
                return;
            }

            Position pos;
            me->GetRandomNearPosition(pos, 10.0f);

            //normalize Z-level if we can, if rift is not at ground level.
            pos.m_positionZ = std::max(me->GetMap()->GetHeight(pos.m_positionX, pos.m_positionY, MAX_HEIGHT), me->GetMap()->GetWaterLevel(pos.m_positionX, pos.m_positionY));

            if (Unit* Summon = DoSummon(creature_entry, pos, 30000, TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT))
                if (Unit* temp = Unit::GetUnit(*me, instance ? instance->GetData64(DATA_MEDIVH) : 0))
                    Summon->AddThreat(temp, 0.0f);
        }

        void DoSelectSummon()
        {
            if ((mRiftWaveCount > 2 && mWaveId < 1) || mRiftWaveCount > 3)
                mRiftWaveCount = 0;

            uint32 entry = 0;

            entry = PortalWaves[mWaveId].PortalMob[mRiftWaveCount];
            SF_LOG_DEBUG("scripts", "npc_time_rift: summoning wave Creature (Wave %u, Entry %u).", mRiftWaveCount, entry);

            ++mRiftWaveCount;

            if (entry == NPC_INFINITE_WHELP)
            {
                for (uint8 i = 0; i < 3; ++i)
                    DoSummonAtRift(entry);
            } else DoSummonAtRift(entry);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!instance)
                return;

            if (TimeRiftWave_Timer <= diff)
            {
                DoSelectSummon();
                TimeRiftWave_Timer = 15000;
            } else TimeRiftWave_Timer -= diff;

            if (me->IsNonMeleeSpellCasted(false))
                return;

            SF_LOG_DEBUG("scripts", "npc_time_rift: not casting anylonger, i need to die.");
            me->setDeathState(DeathState::JUST_DIED);
        }
    };
};

enum Saat
{
    SPELL_CHRONO_BEACON     = 34975,
    ITEM_CHRONO_BEACON      = 24289
};

#define GOSSIP_ITEM_OBTAIN      "[PH] Obtain Chrono-Beacon"

class npc_saat : public CreatureScript
{
public:
    npc_saat() : CreatureScript("npc_saat") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->CastSpell(player, SPELL_CHRONO_BEACON, false);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(QUEST_OPENING_PORTAL) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(ITEM_CHRONO_BEACON))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_OBTAIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(10000, creature->GetGUID());
            return true;
        }
        else if (player->GetQuestRewardStatus(QUEST_OPENING_PORTAL) && !player->HasItemCount(ITEM_CHRONO_BEACON))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_OBTAIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(10001, creature->GetGUID());
            return true;
        }

        player->SEND_GOSSIP_MENU(10002, creature->GetGUID());
        return true;
    }
};

void AddSC_the_black_morass()
{
    new npc_medivh_bm();
    new npc_time_rift();
    new npc_saat();
}
