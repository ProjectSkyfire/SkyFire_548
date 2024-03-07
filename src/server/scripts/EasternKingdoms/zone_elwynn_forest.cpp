/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Elwynn_Forest
SD%Complete: 50
SDCategory: Elwynn Forest
EndScriptData */

/* ContentData
npc_blackrock_spy
npc_blackrock_invader
npc_stormwind_infantry
npc_blackrock_battle_worg
npc_goblin_assassin
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Northshire
{
    SAY_BLACKROCK_COMBAT_1    = -1000015,
    SAY_BLACKROCK_COMBAT_2    = -1000016,
    SAY_BLACKROCK_COMBAT_3    = -1000017,
    SAY_BLACKROCK_COMBAT_4    = -1000018,
    SAY_BLACKROCK_COMBAT_5    = -1000019,
    SAY_ASSASSIN_COMBAT_1     = -1000020,
    SAY_ASSASSIN_COMBAT_2     = -1000021,
    SPELL_SPYING              = 92857,
    SPELL_SNEAKING            = 93046,
    SPELL_SPYGLASS            = 80676,
    NPC_BLACKROCK_BATTLE_WORG = 49871,      //Blackrock Battle Worg NPC ID
    NPC_STORMWIND_INFANTRY    = 49869,      //Stormwind Infantry NPC ID
    WORG_FIGHTING_FACTION     = 232,        //Faction used by worgs to be able to attack infantry
    WORG_FACTION_RESTORE      = 32,         //Default Blackrock Battle Worg Faction
    WORG_GROWL                = 2649,       //Worg Growl Spell
    AI_HEALTH_MIN             = 85,         //Minimum health for AI staged fight between Blackrock Battle Worgs and Stormwind Infantry
    SAY_INFANTRY_YELL         = 1,          //Stormwind Infantry Yell phrase from Group 1
    INFANTRY_YELL_CHANCE      = 10           //% Chance for Stormwind Infantry to Yell - May need further adjustment... should be low chance
};

/*######
## npc_blackrock_spy
######*/

class npc_blackrock_spy : public CreatureScript
{
public:
    npc_blackrock_spy() : CreatureScript("npc_blackrock_spy") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blackrock_spyAI (creature);
    }

    struct npc_blackrock_spyAI : public ScriptedAI
    {
        npc_blackrock_spyAI(Creature* creature) : ScriptedAI(creature)
        {
            CastSpying();
        }

        void CastSpying()
        {
            GetCreature(-8868.88f, -99.1016f);
            GetCreature(-8936.5f, -246.743f);
            GetCreature(-8922.44f, -73.9883f);
            GetCreature(-8909.68f, -40.0247f);
            GetCreature(-8834.85f, -119.701f);
            GetCreature(-9022.08f, -163.965f);
            GetCreature(-8776.55f, -79.158f);
            GetCreature(-8960.08f, -63.767f);
            GetCreature(-8983.12f, -202.827f);
        }

        void GetCreature(float X, float Y)
        {
            if (me->GetHomePosition().GetPositionX() == X && me->GetHomePosition().GetPositionY() == Y)
                if (!me->IsInCombat() && !me->HasAura(SPELL_SPYING))
                    DoCast(me, SPELL_SPYING);

            CastSpyglass();
        }

        void CastSpyglass()
        {
            Spyglass(-8868.88f, -99.1016f, -8936.5f, -246.743f, -8922.44f, -73.9883f, -8909.68f, -40.0247f, -8834.85f,
                -119.701f, -9022.08f, -163.965f, -8776.55f, -79.158f, -8960.08f, -63.767f, -8983.12f, -202.827f);
        }

        void Spyglass(float X1, float Y1, float X2, float Y2, float X3, float Y3, float X4, float Y4, float X5, float Y5,
            float X6, float Y6, float X7, float Y7, float X8, float Y8, float X9, float Y9)
        {
            if ((me->GetHomePosition().GetPositionX() != X1 && me->GetHomePosition().GetPositionY() != Y1) &&
             (me->GetHomePosition().GetPositionX() != X2 && me->GetHomePosition().GetPositionY() != Y2) &&
             (me->GetHomePosition().GetPositionX() != X3 && me->GetHomePosition().GetPositionY() != Y3) &&
             (me->GetHomePosition().GetPositionX() != X4 && me->GetHomePosition().GetPositionY() != Y4) &&
             (me->GetHomePosition().GetPositionX() != X5 && me->GetHomePosition().GetPositionY() != Y5) &&
             (me->GetHomePosition().GetPositionX() != X6 && me->GetHomePosition().GetPositionY() != Y6) &&
             (me->GetHomePosition().GetPositionX() != X7 && me->GetHomePosition().GetPositionY() != Y7) &&
             (me->GetHomePosition().GetPositionX() != X8 && me->GetHomePosition().GetPositionY() != Y8) &&
             (me->GetHomePosition().GetPositionX() != X9 && me->GetHomePosition().GetPositionY() != Y9))
                if (me->GetHomePosition().GetPositionX() == me->GetPositionX() && me->GetHomePosition().GetPositionY() == me->GetPositionY())
                    if (!me->IsInCombat() && !me->HasAura(SPELL_SPYGLASS))
                        DoCast(me, SPELL_SPYGLASS);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(RAND(SAY_BLACKROCK_COMBAT_1, SAY_BLACKROCK_COMBAT_2, SAY_BLACKROCK_COMBAT_3, SAY_BLACKROCK_COMBAT_4, SAY_BLACKROCK_COMBAT_5), me);
        }

		void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            CastSpyglass();

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_blackrock_invader
######*/

class npc_blackrock_invader : public CreatureScript
{
public:
    npc_blackrock_invader() : CreatureScript("npc_blackrock_invader") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blackrock_invaderAI (creature);
    }

    struct npc_blackrock_invaderAI : public ScriptedAI
    {
        npc_blackrock_invaderAI(Creature* creature) : ScriptedAI(creature) {}

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(RAND(SAY_BLACKROCK_COMBAT_1, SAY_BLACKROCK_COMBAT_2, SAY_BLACKROCK_COMBAT_3, SAY_BLACKROCK_COMBAT_4, SAY_BLACKROCK_COMBAT_5), me);
        }

		void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_goblin_assassin
######*/

class npc_goblin_assassin : public CreatureScript
{
public:
    npc_goblin_assassin() : CreatureScript("npc_goblin_assassin") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_goblin_assassinAI (creature);
    }

    struct npc_goblin_assassinAI : public ScriptedAI
    {
        npc_goblin_assassinAI(Creature* creature) : ScriptedAI(creature)
        {
            if (!me->IsInCombat() && !me->HasAura(SPELL_SPYING))
                DoCast(SPELL_SNEAKING);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(RAND(SAY_ASSASSIN_COMBAT_1, SAY_ASSASSIN_COMBAT_2), me);
        }

		void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_stormwind_infantry
######*/

class npc_stormwind_infantry : public CreatureScript
{
public:
    npc_stormwind_infantry() : CreatureScript("npc_stormwind_infantry") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_stormwind_infantryAI (creature);
    }

    struct npc_stormwind_infantryAI : public ScriptedAI
    {
        npc_stormwind_infantryAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 tSeek, cYell,tYell;

        void Reset() OVERRIDE
        {
            tSeek=urand(1000,2000);
            cYell=urand(0, 100);
            tYell=urand(5000, 60000);
        }

        void DamageTaken(Unit* who, uint32& damage) OVERRIDE
        {
            if (who->GetTypeId() == TypeID::TYPEID_PLAYER)//If damage taken from player
            {
                me->getThreatManager().resetAllAggro();
                who->AddThreat(me, 1.0f);
                me->AddThreat(who, 1.0f);
                me->AI()->AttackStart(who);
            }
            else if (who->IsPet())//If damage taken from pet
            {
                me->getThreatManager().resetAllAggro();
                me->AddThreat(who, 1.0f);
                me->AI()->AttackStart(who);
            }
            if (who->GetEntry() == NPC_BLACKROCK_BATTLE_WORG && me->HealthBelowPct(AI_HEALTH_MIN))//If damage taken from Blackrock Battle Worg
            {
                damage = 0;//We do not want to do damage if Blackrock Battle Worg is below preset HP level (currently 85% - Blizzlike)
                me->AddThreat(who, 1.0f);
                me->AI()->AttackStart(who);
            }
        }

		void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;
            else
            {
                if (tYell <= diff)//Chance to yell every 5 to 10 seconds
                {
                    if (cYell < INFANTRY_YELL_CHANCE)//Roll for random chance to Yell phrase
                    {
                        me->AI()->Talk(SAY_INFANTRY_YELL); //Yell phrase
                        tYell=urand(10000, 120000);//After First yell, change time range from 10 to 120 seconds
                    }
                    else
                        tYell=urand(10000, 120000);//From 10 to 120 seconds
                }
                else
                {
                    tYell -= diff;
                    DoMeleeAttackIfReady(); //Else do standard attack
                }
            }
        }
    };
};

/*######
## npc_blackrock_battle_worg
######*/

class npc_blackrock_battle_worg : public CreatureScript
{
public:
    npc_blackrock_battle_worg() : CreatureScript("npc_blackrock_battle_worg") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blackrock_battle_worgAI (creature);
    }

    struct npc_blackrock_battle_worgAI : public ScriptedAI
    {
        npc_blackrock_battle_worgAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 tSeek, tGrowl;

        void Reset() OVERRIDE
        {
            tSeek=urand(1000,2000);
            tGrowl=urand(8500,10000);
            me->setFaction(WORG_FACTION_RESTORE);//Restore our faction on reset
        }

        void DamageTaken(Unit* who, uint32& damage) OVERRIDE
        {
            if (who->GetTypeId() == TypeID::TYPEID_PLAYER)//If damage taken from player
            {
                me->getThreatManager().resetAllAggro();
                who->AddThreat(me, 1.0f);
                me->AddThreat(who, 1.0f);
                me->AI()->AttackStart(who);
            }
            else if (who->IsPet())//If damage taken from pet
            {
                me->getThreatManager().resetAllAggro();
                me->AddThreat(who, 1.0f);
                me->AI()->AttackStart(who);
            }
            if (who->GetEntry() == NPC_STORMWIND_INFANTRY && me->HealthBelowPct(AI_HEALTH_MIN))//If damage taken from Stormwind Infantry
            {
                damage = 0;//We do not want to do damage if Stormwind Infantry is below preset HP level (currently 85% - Blizzlike)
                me->AddThreat(who, 1.0f);
                me->AI()->AttackStart(who);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (tSeek <= diff)
            {
                if ((me->IsAlive()) && (!me->IsInCombat() && (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) <= 1.0f)))
                    if (Creature* enemy = me->FindNearestCreature(NPC_STORMWIND_INFANTRY,1.0f, true))
                    {
                        me->setFaction(WORG_FIGHTING_FACTION);//We must change our faction to one which is able to attack Stormwind Infantry (Faction 232 works well)
                        me->AI()->AttackStart(enemy);
                        tSeek = urand(1000,2000);
                    }
            }
            else
                tSeek -= diff;

            if (UpdateVictim())
            {
                if (tGrowl <=diff)
                {
                    DoCast(me->GetVictim(), WORG_GROWL);//Do Growl if ready
                    tGrowl=urand(8500,10000);
                }
                else
                {
                   tGrowl -= diff;
                   DoMeleeAttackIfReady();//Else do standard attack
                }
            }
            else
            {
                me->setFaction(WORG_FACTION_RESTORE);//Reset my faction if not in combat
                return;
            }
        }
    };
};

class npc_hogger_elwynn : public CreatureScript
{
public:
    npc_hogger_elwynn() : CreatureScript("npc_hogger_elwynn") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_hogger_elwynnAI(creature);
    }

    struct npc_hogger_elwynnAI : public ScriptedAI
    {
        const Position dumasSpawnPos = { -10132.9f, 653.561f, 36.0503f, 2.021f };
        const Position andromathSpawnPos = { -10123.0f, 656.875f, 36.0511f, 1.97181f };
        const Position ragaMuffin1SpawnPos = { -10122.5f, 660.198f, 36.0366f, 2.83775f };
        const Position ragaMuffin2SpawnPos = { -10130.9f, 653.302f, 36.0501f, 1.65242f };
        const Position jonathanSpawnPos = { -10128.3f, 656.465f, 36.0544f, 2.04543f };
        uint8 phase = 0;
        enum HoggerTexts
        {
            WARNING_EATING = 0,
            YELL_YIPE = 1,
            SAY_GRR = 2,
            SAY_NO = 3,
            YELL_NO_HURT = 4
        };
        enum HoggerNPCS
        {
            NPC_TRIGGER_MEAT = 45979,
            NPC_DUMAS = 46940,
            NPC_ANDROMATH = 46941,
            NPC_JONATHAN = 46942,
            NPC_RAGAMUFFIN = 46943,
        };
        enum HoggerSpells
        {
            SPELL_VICIOUS_SLICE = 87337,
            SPELL_BLOODY_STRIKE = 87359,
            SPELL_UPSET_STOMACH = 87352,
            SPELL_SUMMON_MINIONS = 87366,
            SPELL_TELEPORT_VISUAL_ONLY = 64446
        };
        EventMap events;
        enum HoggerEvents
        {
            EVENT_VICIOUS_SLICE = 1,
            EVENT_EATING = 2,
            EVENT_BLOODY_STRIKE = 3,
            EVENT_UPSET_STOMACH = 4,
            EVENT_OUTRO = 5,
            EVENT_KID_TALK1 = 6,
            EVENT_HOGGER_GRR = 7,
            EVENT_JONATHAN_TALK2 = 8,
            EVENT_JONATHAN_TALK3 = 9,
            EVENT_HOGGER_NOO = 10,
            EVENT_JONATHAN_TALK4 = 11,
            EVENT_TELEPORT_AND_DESPAWN = 12
        };

        npc_hogger_elwynnAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            phase = 0;
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_VICIOUS_SLICE, 3000);
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            if (phase == 0 && !HealthAbovePct(50))
            {
                phase = 1;
                events.CancelEvent(EVENT_VICIOUS_SLICE);
                events.ScheduleEvent(EVENT_EATING, 1000);
            }

            if (phase == 1 && damage >= me->GetHealth())
            {
                phase = 2;
                events.ScheduleEvent(EVENT_OUTRO, 1000);
                me->RemoveAllAuras();
                me->AttackStop();
                attacker->AttackStop();
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->SetWalk(true);
                Talk(YELL_NO_HURT, me);
                me->GetMotionMaster()->MovePoint(1, -10136.9f, 670.009f, 36.03682f);
                damage = me->GetHealth() - 1;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_VICIOUS_SLICE:
                {
                    me->CastSpell(me->GetVictim(), SPELL_VICIOUS_SLICE, false);
                    events.ScheduleEvent(EVENT_VICIOUS_SLICE, 12000);
                    break;
                }
                case EVENT_EATING:
                {
                    if (phase == 1)
                    {
                        Talk(WARNING_EATING, me);
                        if (Creature* meat = me->FindNearestCreature(NPC_TRIGGER_MEAT, 30.0f))
                        {
                            Talk(YELL_YIPE, me);
                            me->CastSpell(me, SPELL_SUMMON_MINIONS, false);
                            me->GetMotionMaster()->MoveFollow(meat, 0.01f, 1.57079f);
                            me->SetFacingToObject(meat);
                            events.ScheduleEvent(EVENT_BLOODY_STRIKE, 3000);
                        }
                    }
                    break;
                }
                case EVENT_BLOODY_STRIKE:
                {
                    if (Creature* meat = me->FindNearestCreature(NPC_TRIGGER_MEAT, 30.0f))
                        me->CastSpell(meat, SPELL_BLOODY_STRIKE, false);

                    events.ScheduleEvent(EVENT_BLOODY_STRIKE, 3000);
                    events.ScheduleEvent(EVENT_UPSET_STOMACH, 10000);
                    break;
                }
                case EVENT_UPSET_STOMACH:
                {
                    events.CancelEvent(EVENT_UPSET_STOMACH);
                    events.CancelEvent(EVENT_BLOODY_STRIKE);
                    me->CastSpell(me, SPELL_UPSET_STOMACH, false);

                    if (Unit* victim = me->SelectVictim())
                    {
                        me->GetMotionMaster()->MoveFollow(victim, 0.1f, 0.0f);
                        AttackStart(victim);
                    }
                    
                    events.ScheduleEvent(EVENT_VICIOUS_SLICE, 3000);
                    break;
                }
                case EVENT_OUTRO:
                {
                    events.CancelEvent(EVENT_VICIOUS_SLICE);
                    events.CancelEvent(EVENT_EATING);
                    events.CancelEvent(EVENT_UPSET_STOMACH);
                    events.CancelEvent(EVENT_BLOODY_STRIKE);

                    if (Creature* dumas = me->SummonCreature(NPC_DUMAS, dumasSpawnPos, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                    {
                        dumas->CastSpell(dumas, SPELL_TELEPORT_VISUAL_ONLY);
                        float x, y, z;
                        dumas->GetClosePoint(x, y, z, dumas->GetObjectSize() / 3, 2.5f);
                        dumas->GetMotionMaster()->MovePoint(0, x, y, z);
                    }

                    if (Creature* andromath = me->SummonCreature(NPC_ANDROMATH, andromathSpawnPos, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                    {
                        andromath->CastSpell(andromath, SPELL_TELEPORT_VISUAL_ONLY);
                        float x, y, z;
                        andromath->GetClosePoint(x, y, z, andromath->GetObjectSize() / 3, 2.5f);
                        andromath->GetMotionMaster()->MovePoint(0, x, y, z);
                    }

                    if (Creature* ragamuffin1 = me->SummonCreature(NPC_RAGAMUFFIN, ragaMuffin1SpawnPos, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                    {
                        ragamuffin1->CastSpell(ragamuffin1, SPELL_TELEPORT_VISUAL_ONLY);
                        float x, y, z;
                        ragamuffin1->GetClosePoint(x, y, z, ragamuffin1->GetObjectSize() / 3, 2.5f);
                        ragamuffin1->GetMotionMaster()->MovePoint(0, x, y, z);
                        events.ScheduleEvent(EVENT_KID_TALK1, 5000);
                    }

                    if (Creature* ragamuffin2 = me->SummonCreature(NPC_RAGAMUFFIN, ragaMuffin2SpawnPos, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                    {
                        ragamuffin2->CastSpell(ragamuffin2, SPELL_TELEPORT_VISUAL_ONLY);
                        float x, y, z;
                        ragamuffin2->GetClosePoint(x, y, z, ragamuffin2->GetObjectSize() / 3, 2.5f);
                        ragamuffin2->GetMotionMaster()->MovePoint(0, x, y, z);
                    }

                    if (Creature* jonathan = me->SummonCreature(NPC_JONATHAN, jonathanSpawnPos, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                    {
                        jonathan->CastSpell(jonathan, SPELL_TELEPORT_VISUAL_ONLY);
                        jonathan->MonsterYell("Hold your blade, adventurer!", Language::LANG_UNIVERSAL, me);
                        jonathan->SetWalk(true);
                        jonathan->Dismount();
                        float x, y, z;
                        jonathan->GetClosePoint(x, y, z, jonathan->GetObjectSize() / 3, 10.0f);
                        jonathan->GetMotionMaster()->MovePoint(0, x, y, z);
                        me->SetFacingToObject(jonathan);
                        events.ScheduleEvent(EVENT_HOGGER_GRR, 8000);
                        events.ScheduleEvent(EVENT_JONATHAN_TALK2, 15000);
                    }
                    break;
                }
                case EVENT_KID_TALK1:
                {
                    if (Creature* ragaMuffin1 = me->FindNearestCreature(NPC_RAGAMUFFIN, 25.0f))
                    {
                        ragaMuffin1->MonsterSay("General Hammond Clay!", Language::LANG_UNIVERSAL, ragaMuffin1);

                        if (Creature* ragaMuffin2 = ragaMuffin1->FindNearestCreature(NPC_RAGAMUFFIN, 25.0f))
                            ragaMuffin2->MonsterSay("Wow!", Language::LANG_UNIVERSAL, ragaMuffin2);
                    }
                    break;
                }
                case EVENT_HOGGER_GRR:
                {
                    Talk(SAY_GRR);
                    break;
                }
                case EVENT_JONATHAN_TALK2:
                {
                    if (Creature* jonathan = me->FindNearestCreature(NPC_JONATHAN, 25.0f))
                    {
                        jonathan->MonsterSay("This beast leads the Riverpaw gnoll gang and may be the key to ending gnoll aggression in Elwynn.", Language::LANG_UNIVERSAL, jonathan);
                        jonathan->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                    }
                    events.ScheduleEvent(EVENT_JONATHAN_TALK3, 5000);
                    break;
                }
                case EVENT_JONATHAN_TALK3:
                {
                    if (Creature* jonathan = me->FindNearestCreature(NPC_JONATHAN, 25.0f))
                    {
                        jonathan->MonsterSay("We're taking him into custody in the name of King Varian Wrynn.", Language::LANG_UNIVERSAL, jonathan);
                    }
                    events.ScheduleEvent(EVENT_HOGGER_NOO, 5000);
                    break;
                }
                case EVENT_HOGGER_NOO:
                {
                    Talk(SAY_NO);
                    events.ScheduleEvent(EVENT_JONATHAN_TALK4, 5000);
                    break;
                }
                case EVENT_JONATHAN_TALK4:
                {
                    if (Creature* jonathan = me->FindNearestCreature(NPC_JONATHAN, 25.0f))
                    {
                        if (Creature* andromath = me->FindNearestCreature(NPC_ANDROMATH, 25.0f))
                        {
                            jonathan->SetFacingToObject(andromath);
                        }
                        jonathan->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                        jonathan->MonsterSay("Take us to the Stockades, Andromath.", Language::LANG_UNIVERSAL, jonathan);
                    }
                    events.ScheduleEvent(EVENT_TELEPORT_AND_DESPAWN, 3000);
                    break;
                }
                case EVENT_TELEPORT_AND_DESPAWN:
                {
                    if (Creature* dumas = me->FindNearestCreature(NPC_DUMAS, 25.0f))
                    {
                        dumas->CastSpell(dumas, SPELL_TELEPORT_VISUAL_ONLY);
                        dumas->DespawnOrUnsummon();
                    }

                    if (Creature* andromath = me->FindNearestCreature(NPC_ANDROMATH, 25.0f))
                    {
                        andromath->CastSpell(andromath, SPELL_TELEPORT_VISUAL_ONLY);
                        andromath->DespawnOrUnsummon();
                    }

                    if (Creature* jonathan = me->FindNearestCreature(NPC_JONATHAN, 25.0f))
                    {
                        jonathan->CastSpell(jonathan, SPELL_TELEPORT_VISUAL_ONLY);
                        jonathan->DespawnOrUnsummon();
                    }

                    for (uint8 i = 0; i < 2; i++)
                    {
                        if (Creature* ragaMuffin = me->FindNearestCreature(NPC_RAGAMUFFIN, 25.0f))
                        {
                            ragaMuffin->CastSpell(ragaMuffin, SPELL_TELEPORT_VISUAL_ONLY);
                            ragaMuffin->DespawnOrUnsummon();
                        }
                    }

                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 15.0f);

                    for (auto&& player : playerList)
                    {
                        player->KilledMonsterCredit(448);
                    }
                    
                    me->CastSpell(me, SPELL_TELEPORT_VISUAL_ONLY);
                    me->DespawnOrUnsummon();
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_elwynn_forest()
{
    new npc_hogger_elwynn();
    new npc_blackrock_spy();
    new npc_goblin_assassin();
    new npc_blackrock_invader();
    new npc_stormwind_infantry();
    new npc_blackrock_battle_worg();
}
