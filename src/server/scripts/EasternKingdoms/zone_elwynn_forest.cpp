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
        }

        void EnterCombat(Unit* who) OVERRIDE
        {
            me->AI()->Talk(0, who);
        }

        void Reset() OVERRIDE
        {
            CastGlassOrSpy();
        }

		void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void CastGlassOrSpy()
        {
            const bool isSpellGlass = rand() % 2 == 0;

            (isSpellGlass) ? 
                CastSpyGlass() : 
                CastSpy();
        }

        void CastSpy()
        {
            if (!me->IsInCombat() && !me->HasAura(NPC_BLACKROCK_SPY_SPELL_SPYING)) {
                DoCast(me, NPC_BLACKROCK_SPY_SPELL_SPYING);
                me->SetDefaultMovementType(MovementGeneratorType::RANDOM_MOTION_TYPE);
                me->GetMotionMaster()->MoveRandom(15.0f);
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

void AddSC_elwynn_forest()
{
    new npc_blackrock_spy();
    new npc_goblin_assassin();
    new npc_blackrock_invader();
    new npc_stormwind_infantry();
    new npc_blackrock_battle_worg();
}
