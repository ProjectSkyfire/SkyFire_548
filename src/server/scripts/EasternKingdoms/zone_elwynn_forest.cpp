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
SD%Complete: 86
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

    WORG_GROWL                = 2649,       //Worg Growl Spell

    NPC_BLACKROCK_SPY_SPELL_SPYING = 92857,
    NPC_BLACKROCK_SPY_SPELL_SPYGLASS = 80676,

    NPC_GOBLIN_ASSASSIN_SPELL_SNEAKING = 93046,

    NPC_NORTHSHIRE_PEASANT_MODEL_CHOPWOOD = 11354,
    NPC_NORTHSHIRE_PEASANT_MODEL_WOODLOGS = 11355,

    NPC_BROTHER_PAXTON_SPELL_HOLY_SMITE = 9734,
    NPC_BROTHER_PAXTON_SPELL_RENEW = 93094,
    NPC_BROTHER_PAXTON_SPELL_PRAYER_OF_HEALING = 93091,
    NPC_BROTHER_PAXTON_SPELL_POWER_WORD_FORTITUDE = 13864,

    NPC_BROTHER_PAXTON_SAY_INFANTRY_NORM_0 = 0,
    NPC_BROTHER_PAXTON_SAY_INFANTRY_NORM_1 = 1,
    NPC_BROTHER_PAXTON_SAY_INFANTRY_NORM_CHANCE = 25,

    //Minimum health for AI staged fight between Blackrock Battle Worgs and Stormwind Infantry
    NPC_STORMWIND_INFARTY_AI_HEALTH_PCT_LOCK = 70,
    //Minimum health for Paxhton heal support
    NPC_STORMWIND_INFARTY_AI_HEALTH_PCT_NEED_HELP = 85,
    //Stormwind Infantry NORM phrase from Group 0 to say help to Paxton
    NPC_STORMWIND_INFARTY_SAY_INFANTRY_NORM = 0,
    //% Chance for Stormwind Infantry to `HELP` to Paxton
    NPC_STORMWIND_INFARTY_SAY_INFANTRY_NORM_CHANCE = 25,
    //Stormwind Infantry Yell phrase from Group 1
    NPC_STORMWIND_INFARTY_SAY_INFANTRY_YELL = 1,
    //% Chance for Stormwind Infantry to Yell - May need further adjustment... should be low chance
    NPC_STORMWIND_INFARTY_SAY_INFANTRY_YELL_CHANCE = 10,
};

typedef Creature* TGuard;
typedef bool TGuardSupport;
typedef std::vector<std::pair<TGuard, TGuardSupport>> TPaxtonGuards;
TPaxtonGuards g_paxtonGroup = {
    {nullptr, false},
    {nullptr, false},
    {nullptr, false},
    {nullptr, false},
};

struct Guard
{
    enum GUIDs : uint32 {
        STORMWIND_INFARTY_LEFT_GUID = 168336,
        STORMWIND_INFARTY_MID_L_GUID = 168344,
        STORMWIND_INFARTY_MID_R_GUID = 168356,
        STORMWIND_INFARTY_RIGHT_GUID = 168357,
    };
    enum Alias : uint32 {
        GUARD_LEFT = 0,
        GUARD_MID_L,
        GUARD_MID_R,
        GUARD_RIGHT,
    };
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
            isSpellGlass = urand(0, 100) > 50;
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
## npc_brother_paxton
######*/

class npc_brother_paxton : public CreatureScript
{
public:
    npc_brother_paxton() : CreatureScript("npc_brother_paxton") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_brother_paxtonAI(creature);
    }

    struct npc_brother_paxtonAI : public ScriptedAI
    {
        npc_brother_paxtonAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 tSupport = 1 * IN_MILLISECONDS;
        uint32 tMoving = 0;
        uint32 sChanse = 0;

        void Reset() OVERRIDE
        {
            tSupport = 1 * IN_MILLISECONDS;
            tMoving = 0;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            // TODO: FIX-ME
            // --------------
            // POWER_WORD_FORTITUDE removed from Paxton when after
            // use NPC_BROTHER_PAXTON_SPELL_PRAYER_OF_HEALING!!!

            /*if (!me->HasAura(NPC_BROTHER_PAXTON_SPELL_POWER_WORD_FORTITUDE))
                DoCast(NPC_BROTHER_PAXTON_SPELL_POWER_WORD_FORTITUDE);*/

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            // start moving
            if (!me->isMoving() && tMoving < diff)
                me->GetMotionMaster()->MovePath(me->GetEntry(), true);
            else tMoving -= diff;

            // diff to first support
            if (tSupport < diff) 
            {
                // reset up diff for support
                tSupport = 1 * IN_MILLISECONDS;

                // we are looking for guards in `group` who need support
                CheckHelp();

                // check count guards with low health
                // and do some heal work
                switch (SupportCount())
                {
                    case 1: CastRenewOnGuard(); break;
                    case 2:
                    case 3:
                    case 4: CastPrayerOfHealing(); break;
                }
            }
            else tSupport -= diff;

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void sGossipHello(Player* /*player*/)
        {
            if (me->IsQuestGiver()) {
                me->StopMoving();
                // throttle to moving
                tMoving = 10 * IN_MILLISECONDS;
            }
        }

        void CheckHelp()
        {
            for (TPaxtonGuards::iterator it = g_paxtonGroup.begin(); it != g_paxtonGroup.end(); ++it) {
                if ((*it).first != nullptr) {
                    (*it).second = ((*it).first->HealthBelowPct(NPC_STORMWIND_INFARTY_AI_HEALTH_PCT_NEED_HELP)) ? true : false;
                }
            }
        }
        uint32 SupportCount()
        {
            return std::count_if(g_paxtonGroup.begin(), g_paxtonGroup.end(), [&](auto pair) { return pair.second == true; });
        }

        void CastRenewOnGuard()
        {
            auto it = std::find_if(g_paxtonGroup.begin(), g_paxtonGroup.end(), [&](auto pair) { return pair.second == true; });
            if (it != g_paxtonGroup.end()) {
                if (!(*it).first->HasAura(NPC_BROTHER_PAXTON_SPELL_RENEW)) 
                {
                    DoCast((*it).first, NPC_BROTHER_PAXTON_SPELL_RENEW, false);
                    
                    (*it).second = false;
                    
                    // chance
                    sChanse = urand(0, 100);

                    if (sChanse < NPC_BROTHER_PAXTON_SAY_INFANTRY_NORM_CHANCE)
                        me->AI()->Talk(NPC_BROTHER_PAXTON_SAY_INFANTRY_NORM_0, (*it).first);
                }
            }
        }
        void CastPrayerOfHealing()
        {
            me->StopMoving();

            DoCast(NPC_BROTHER_PAXTON_SPELL_PRAYER_OF_HEALING);

            // chance
            sChanse = urand(0, 100);

            if (sChanse < NPC_BROTHER_PAXTON_SAY_INFANTRY_NORM_CHANCE)
                me->AI()->Talk(NPC_BROTHER_PAXTON_SAY_INFANTRY_NORM_1);
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
        npc_stormwind_infantryAI(Creature* creature) : ScriptedAI(creature) 
        {
            // if someone of STORMWIND INFARTY should be in
            // Paxton group then we are join in
            JoinInPaxtonGroup();
        }

        uint32 tSeek, cYell,tYell, tHelp;

        void Reset() OVERRIDE
        {
            tSeek=urand(1000,2000);
            cYell=urand(0, 100);
            tYell=urand(5000, 60000);
            tHelp=urand(1 * IN_MILLISECONDS, 5 * IN_MILLISECONDS);
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
            if (who->GetEntry() == NPC_BLACKROCK_BATTLE_WORG)//If damage taken from Blackrock Battle Worg
            {
                if (me->HealthBelowPct(NPC_STORMWIND_INFARTY_AI_HEALTH_PCT_LOCK))
                    damage = 0;

                me->AddThreat(who, 1.0f);
                // dont move when attaking
                // stay in one line front
                me->Attack(who, true);
            }
        }

        // TODO: FIX-ME
        // -----------------------
        // It's wrong to do this. 
        // In theory, the spell should have global consequences and always work, 
        // and not when we manually prescribe it for a certain unit.
        // PS: may be i'm wrong and all project was builded like this
        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == NPC_BROTHER_PAXTON_SPELL_PRAYER_OF_HEALING)
                me->SetHealth(urand(108, 125));

            if (spell->Id == NPC_BROTHER_PAXTON_SPELL_RENEW)
                me->SetHealth(me->GetHealth() + urand(4, 5));
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            // Sey help every 10 to 120 sec if health below %
            if (tHelp <= diff)
            {
                // generate a chance
                cYell = urand(0, 100);

                if (cYell < NPC_STORMWIND_INFARTY_SAY_INFANTRY_NORM_CHANCE &&
                    isPaxtonGuard() && 
                    me->HealthBelowPct(NPC_STORMWIND_INFARTY_AI_HEALTH_PCT_NEED_HELP)
                ) {
                    me->AI()->Talk(0);
                    tHelp = urand(10 * IN_MILLISECONDS, 120 * IN_MILLISECONDS);
                }
            }
            else tHelp -= diff;

            //Chance to yell every 5 to 10 seconds
            if (tYell <= diff) 
            {
                // generate a chance
                cYell = urand(0, 100);

                //Roll for random chance to Yell phrase
                if (cYell < NPC_STORMWIND_INFARTY_SAY_INFANTRY_YELL_CHANCE) {
                    me->AI()->Talk(NPC_STORMWIND_INFARTY_SAY_INFANTRY_YELL); //Yell phrase
                    tYell = urand(10 * IN_MILLISECONDS, 120 * IN_MILLISECONDS);//After First yell, change time range from 10 to 120 seconds
                }
                else tYell = urand(10 * IN_MILLISECONDS, 120 * IN_MILLISECONDS);//From 10 to 120 seconds
            }
            else tYell -= diff;

            DoMeleeAttackIfReady();
        }

        bool isPaxtonGuard()
        {
            const uint32 guid = me->GetGUIDLow();

            return guid == Guard::STORMWIND_INFARTY_LEFT_GUID ||
                guid == Guard::STORMWIND_INFARTY_MID_L_GUID ||
                guid == Guard::STORMWIND_INFARTY_MID_R_GUID ||
                guid == Guard::STORMWIND_INFARTY_RIGHT_GUID;
        }
        void JoinInPaxtonGroup()
        {
            const uint32 guid = me->GetGUIDLow();

            if (guid == Guard::STORMWIND_INFARTY_LEFT_GUID) {
                g_paxtonGroup[Guard::GUARD_LEFT].first = me;
            }
            if (guid == Guard::STORMWIND_INFARTY_MID_L_GUID) {
                g_paxtonGroup[Guard::GUARD_MID_L].first = me;
            }
            if (guid == Guard::STORMWIND_INFARTY_MID_R_GUID) {
                g_paxtonGroup[Guard::GUARD_MID_R].first = me;
            }
            if (guid == Guard::STORMWIND_INFARTY_RIGHT_GUID) {
                g_paxtonGroup[Guard::GUARD_RIGHT].first = me;
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
            if (who->GetEntry() == NPC_STORMWIND_INFANTRY && me->HealthBelowPct(NPC_STORMWIND_INFARTY_AI_HEALTH_PCT_LOCK))//If damage taken from Stormwind Infantry
            {
                damage = 0;
                me->AddThreat(who, frand(1.0f, 100.0f));
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
                        me->AI()->AttackStart(enemy);
                        tSeek = urand(1000,2000);
                    }
            }
            else
                tSeek -= diff;

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
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
    new npc_brother_paxton();
    new npc_stormwind_infantry();
    new npc_blackrock_battle_worg();
    new npc_northshire_peasant();
}
