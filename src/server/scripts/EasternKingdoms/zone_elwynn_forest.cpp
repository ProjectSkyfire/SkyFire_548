/*
* Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
* Copyright (C) 2008-2021 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2021 MaNGOS <https://www.getmangos.eu/>
* Copyright (C) 2006-2014 ScriptDev2 <https://github.com/scriptdev2/scriptdev2/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
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
    NPC_BLACKROCK_BATTLE_WORG = 49871,      //Blackrock Battle Worg NPC ID
    NPC_STORMWIND_INFANTRY    = 49869,      //Stormwind Infantry NPC ID
    WORG_FIGHTING_FACTION     = 232,        //Faction used by worgs to be able to attack infantry
    WORG_FACTION_RESTORE      = 32,         //Default Blackrock Battle Worg Faction
    WORG_GROWL                = 2649,       //Worg Growl Spell
    AI_HEALTH_MIN             = 85,         //Minimum health for AI staged fight between Blackrock Battle Worgs and Stormwind Infantry
    SAY_INFANTRY_YELL         = 1,          //Stormwind Infantry Yell phrase from Group 1
    INFANTRY_YELL_CHANCE      = 10,           //% Chance for Stormwind Infantry to Yell - May need further adjustment... should be low chance

    NPC_BLACKROCK_SPY_SPELL_SPYING = 92857,
    NPC_BLACKROCK_SPY_SPELL_SPYGLASS = 80676,

    NPC_GOBLIN_ASSASSIN_SPELL_SNEAKING = 93046,

    NPC_NORTHSHIRE_PEASANT_MODEL_CHOPWOOD = 11354,
    NPC_NORTHSHIRE_PEASANT_MODEL_WOODLOGS = 11355,
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
        bool isSpellGlass;
        uint32 spyGlassResetUpTimer;

        npc_blackrock_spyAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void EnterCombat(Unit* who) OVERRIDE
        {
            me->AI()->Talk(0, who);
        }

        void Reset() OVERRIDE
        {
            spyGlassResetUpTimer = 0;

            ChooseBehaviorAndCast();
        }

		void UpdateAI(uint32 diff) OVERRIDE
        {
            // need update reset if rogue use pickpocketing
            if (isSpellGlass && !me->HasAura(NPC_BLACKROCK_SPY_SPELL_SPYGLASS)) {
                spyGlassResetUpTimer += diff;

                // wait some time for reset up
                if (spyGlassResetUpTimer > 2500) {
                    spyGlassResetUpTimer = 0;
                    DoCast(me, NPC_BLACKROCK_SPY_SPELL_SPYGLASS);
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void ChooseBehaviorAndCast()
        {
            ChooseBehavior();
            CastGlassOrSpy();
        }

        void ChooseBehavior()
        {
            isSpellGlass = rand() % 2 == 0;
        }

        void CastGlassOrSpy()
        {
            (isSpellGlass) ? CastSpyGlass() : CastSpy();
        }

        void CastSpy()
        {
            // was added `isSpellGlass` that fix bug when they
            // is moving with a spy glass
            if (!isSpellGlass && !me->IsInCombat() && !me->HasAura(NPC_BLACKROCK_SPY_SPELL_SPYING)) {
                DoCast(me, NPC_BLACKROCK_SPY_SPELL_SPYING);
                me->SetDefaultMovementType(MovementGeneratorType::RANDOM_MOTION_TYPE);
                me->GetMotionMaster()->MoveRandom(9.0f);
            }
        }
        void CastSpyGlass()
        {
            if (!me->IsInCombat() && !me->HasAura(NPC_BLACKROCK_SPY_SPELL_SPYGLASS))
                DoCast(me, NPC_BLACKROCK_SPY_SPELL_SPYGLASS);
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
            //Talk(RAND(SAY_BLACKROCK_COMBAT_1, SAY_BLACKROCK_COMBAT_2, SAY_BLACKROCK_COMBAT_3, SAY_BLACKROCK_COMBAT_4, SAY_BLACKROCK_COMBAT_5), me);
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
        }

        void Reset() OVERRIDE
        {
            if (!me->HasAura(NPC_BLACKROCK_SPY_SPELL_SPYING))
                DoCast(NPC_GOBLIN_ASSASSIN_SPELL_SNEAKING);
        }

        void EnterCombat(Unit* who) OVERRIDE
        {
            me->AI()->Talk(0, who);
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
            //me->setFaction(WORG_FACTION_RESTORE);//Restore our faction on reset
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
                    // TODO: dont use WORG_GROWL that the guards do not shout at every change of target
                    //DoCast(me->GetVictim(), WORG_GROWL);//Do Growl if ready
                    //tGrowl=urand(8500,10000);
                }
                else
                {
                   tGrowl -= diff;
                   DoMeleeAttackIfReady();//Else do standard attack
                }
            }
            else
            {
                //me->setFaction(WORG_FACTION_RESTORE);//Reset my faction if not in combat
                return;
            }
        }
    };
};

/*######
## npc_northshire_peasant
######*/

class npc_northshire_peasant : public CreatureScript
{
public:
    npc_northshire_peasant() : CreatureScript("npc_northshire_peasant") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_northshire_peasantAI(creature);
    }

    struct npc_northshire_peasantAI : public ScriptedAI
    {
        const uint32 m_woods_stack = 100;

        uint32 m_phase;
        uint32 m_woods;
        uint32 m_time;

        enum {
            PHASE_CHOPWOOD = 0,
            PHASE_PICKETUP_WOODLOGS,
            PHASE_MOVE_TO_UNLOADING_START,
            PHASE_MOVE_TO_UNLOADING_STOP,
            PHASE_BACKUP_START,
            PHASE_BACKUP_STOP,
        };

        typedef uint32 time_throttle;
        typedef bool is_running;
        typedef std::vector<std::pair<time_throttle, is_running>> EventStore;
        
        // current event and throttle before do next event
        EventStore events = {
            {1000, false},  // PHASE_CHOPWOOD
            {1000, false},  // PHASE_PICKETUP_WOODLOGS
            {   0, false},  // PHASE_MOVE_TO_UNLOADING_START
            {1000, false},  // PHASE_MOVE_TO_UNLOADING_STOP
            {1000, false},  // PHASE_BACKUP_START
            {   0, false},  // PHASE_BACKUP_STOP
        };

        npc_northshire_peasantAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() OVERRIDE
        {
            m_phase = 0;
            m_woods = 0;
            m_time = 0;
            me->SetDefaultMovementType(IDLE_MOTION_TYPE);

            for (uint32 i = 0; i != events.size(); ++i)
                events[i].second = false;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!me->isMoving())
                m_time += diff;

            switch (m_phase)
            {
                case PHASE_CHOPWOOD:                    PhaseChopwood();                break;
                case PHASE_PICKETUP_WOODLOGS:           PhasePicketUpWoodlogs();        break;
                case PHASE_MOVE_TO_UNLOADING_START:     PhaseMoveToUnloadingStart();    break;
                case PHASE_MOVE_TO_UNLOADING_STOP:      PhaseMoveToUnloadingStop();     break;
                case PHASE_BACKUP_START:                PhaseBackupStart();             break;
                case PHASE_BACKUP_STOP:                 PhaseBackupStop();              break;

                default:                                Reset();                        break;
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        bool CanCallNextPhase() const
        {
            return events[m_phase].first < m_time;
        }
        bool isEvenRunning()
        {
            return events[m_phase].second == true;
        }
        void NextPhaseThrottle()
        {
            if (isEvenRunning() && CanCallNextPhase() && !me->isMoving()) {
                ++m_phase;
                m_time = 0;
            }
        }
        void EventRun()
        {
            events[m_phase].second = true;
        };

        void PhaseChopwood()
        {
            if (!isEvenRunning())
                EventRun();

            if (m_woods >= m_woods_stack)
                NextPhaseThrottle();
            else {
                me->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK2HTIGHT);
                ++m_woods;
            }
        }
        void PhasePicketUpWoodlogs()
        {
            if (!isEvenRunning()) {
                me->SetDisplayId(NPC_NORTHSHIRE_PEASANT_MODEL_WOODLOGS);
                EventRun();
            }

            NextPhaseThrottle();
        }
        void PhaseMoveToUnloadingStart()
        {
            if (!isEvenRunning()) {
                me->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                // start to move
                me->GetMotionMaster()->MovePath(me->GetGUIDLow() * 100, false);
                EventRun();
            }

            NextPhaseThrottle();
        }
        void PhaseMoveToUnloadingStop()
        {
            if (!isEvenRunning())
                EventRun();

            NextPhaseThrottle();
        }
        void PhaseBackupStart()
        {
            if (!isEvenRunning()) {
                me->SetDisplayId(NPC_NORTHSHIRE_PEASANT_MODEL_CHOPWOOD);
                // WARNING:
                // i'm split the ways to unload woodlogs and backup to work
                // that why path_id = +1 and why you got error like this
                // WaypointMovementGenerator::LoadPath: creature Northshire Peasant (Entry: 11260 GUID: 168426 DB GUID: 168426) doesn't have waypoint path id: 0
                // TODO: need fix WaypointMovementGenerator that we can to split different ways to one entry
                me->GetMotionMaster()->MovePath((me->GetGUIDLow() * 100) + 1, false);
                EventRun();
            }

            NextPhaseThrottle();
        }
        void PhaseBackupStop()
        {
            if (!isEvenRunning())
                EventRun();

            NextPhaseThrottle();
        }
    };
};

void AddSC_elwynn_forest()
{
    new npc_blackrock_spy();
    new npc_goblin_assassin();
    new npc_blackrock_invader();
    new npc_stormwind_infantry();
    new npc_blackrock_battle_worg();
    new npc_northshire_peasant();
}
