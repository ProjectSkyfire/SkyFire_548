/*
* Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
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

#include "ScriptMgr.h"
#include "CreatureAI.h"
#include "Player.h"
#include "Unit.h"
#include "UnitAI.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "timeless_isle.h"

enum Spells
{
	// Chi-Ji
	SPELL_FIRESTORM					= 144461,
	SPELL_INSPIRING_SONG			= 144468,
	SPELL_BEACON_OF_HOPE			= 144473,
	SPELL_BLAZING_SONG				= 144471,
	SPELL_CRANE_RUSH				= 144470,

	// Firestorm (Npc)
	SPELL_FIRESTORM_VISUAL			= 144463,

	// Child of Chi-Ji
	SPELL_BLAZING_NOVA				= 144494,
};

enum Events
{
	EVENT_FIRESTORM					= 0,
	EVENT_INSPIRING_SONG			= 1,
	EVENT_BEACON_OF_HOPE			= 2,
	EVENT_CRANE_RUSH				= 3,
	EVENT_DEFEATED_1				= 4,
	EVENT_DEFEATED_2				= 5,
	EVENT_DEFEATED_3				= 6,
};

enum Actions
{
	ACTION_DEFEATED					= 0,
};

enum Says
{
	SAY_AGGRO						= 0, // Chi-Ji yells: Then let us begin!
	SAY_BEACON_OF_HOPE				= 1, // Chi-Ji yells: Believe in one another, and let others carry hope for you.
	SAY_CRANE_RUSH					= 2, // Chi-Ji yells: Without hope, there is no brightness in the future. // Chi-Ji yells: Create the destiny you seek.
	SAY_KILL						= 3, // Chi-Ji yells: Do not give up on yourself!
	SAY_DEFEATED_1					= 4, // Chi-Ji yells: Your hope shines brightly, and even more brightly when you work together to overcome. It will ever light your way, in even the darkest of places.
};

enum Phases
{
	PHASE_INTRO						= 0,
	PHASE_COMBAT					= 1,
	PHASE_DEFEATED					= 2
};

class BlazingSongEvent : public BasicEvent
{
    public:
        BlazingSongEvent(Creature* ChiJi)
            : _owner(ChiJi)
        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            _owner->AI()->DoCastAOE(SPELL_BLAZING_SONG, false);

            return true;
        }

    private:
        Creature* _owner;
};

class boss_chi_ji_ti : public CreatureScript
{
    public:
        boss_chi_ji_ti() : CreatureScript("boss_chi_ji_ti") { }

        struct boss_chi_ji_tiAI : public BossAI
        {
            boss_chi_ji_tiAI(Creature* creature) : BossAI(creature, DATA_CHI_JI) {	}

            void CancelAllCombatEventsAndReset()
            {
            	me->InterruptNonMeleeSpells(true);

            	events.SetPhase(PHASE_DEFEATED);

            	events.CancelEvent(EVENT_FIRESTORM);
                events.CancelEvent(EVENT_INSPIRING_SONG);
                events.CancelEvent(EVENT_BEACON_OF_HOPE);
                events.CancelEvent(EVENT_CRANE_RUSH);

                me->m_Events.KillAllEvents(true);
            }

            void Reset() OVERRIDE
            {
                _Reset();
                me->setFaction(35);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                events.SetPhase(PHASE_INTRO); // It's for the intro with Emperor Shaohao. :P
            }

            void EnterCombat(Unit* /*target*/) OVERRIDE
            {
                me->setActive(true);
                Talk(SAY_AGGRO);

                events.SetPhase(PHASE_COMBAT);
                events.ScheduleEvent(EVENT_FIRESTORM, 10000, 0, PHASE_COMBAT);
                events.ScheduleEvent(EVENT_INSPIRING_SONG, 20000, 0, PHASE_COMBAT);
                events.ScheduleEvent(EVENT_BEACON_OF_HOPE, urand(29000, 31000), 0, PHASE_COMBAT); // When he casts Beacon of Hope, he casts Blazing Song right after that.
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();

                if (Defeated == true)
                {
                	CancelAllCombatEventsAndReset();

                	me->AI()->DoAction(ACTION_DEFEATED);
                	summons.DespawnAll();
                }

                else if (Defeated == false)
                	me->Respawn(true); // forced respawn, meaning if he's in combat, he will respawn anyway
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TYPEID_PLAYER && !me->IsInEvadeMode() && events.IsInPhase(PHASE_COMBAT))
                    Talk(SAY_KILL);

                if (!me->GetVictim() && !me->IsInEvadeMode())
                {
                	Defeated = false;
                	EnterEvadeMode();
                }
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                	case ACTION_DEFEATED:
                		events.ScheduleEvent(EVENT_DEFEATED_1, 1000, 0, PHASE_DEFEATED);
                		events.ScheduleEvent(EVENT_DEFEATED_2, 11000, 0, PHASE_DEFEATED);
                		events.ScheduleEvent(EVENT_DEFEATED_3, 17000, 0, PHASE_DEFEATED);
                		break;
                	default:
                		break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (events.IsInPhase(PHASE_COMBAT) && !HealthAbovePct(66))
                {
                	events.ScheduleEvent(EVENT_CRANE_RUSH, 1000, 0, PHASE_COMBAT);
                	return;
                }

                if (events.IsInPhase(PHASE_COMBAT) && !HealthAbovePct(33))
                {
                	events.ScheduleEvent(EVENT_CRANE_RUSH, 1000, 0, PHASE_COMBAT);
                	return;
                }

                if (events.IsInPhase(PHASE_COMBAT) && !HealthAbovePct(3))
                {
                    damage = 0;
                	Defeated = true;

                	EnterEvadeMode();
                	return;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (events.IsInPhase(PHASE_COMBAT))
                {
                    if (!UpdateVictim())
                        return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.IsInPhase(PHASE_INTRO))
                	return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    	case EVENT_FIRESTORM:
                    		DoCastAOE(SPELL_FIRESTORM, false);
                    		events.ScheduleEvent(EVENT_FIRESTORM, urand(15000, 30000), 0, PHASE_COMBAT); // I am not really sure about the timer
                    		break;

                    	case EVENT_INSPIRING_SONG:
                    		DoCast(me, SPELL_INSPIRING_SONG);
                    		events.ScheduleEvent(EVENT_INSPIRING_SONG, 30000, 0, PHASE_COMBAT); // pretty sure :)
                    		break;

                    	case EVENT_BEACON_OF_HOPE:
                    		if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    		{
                    			Talk(SAY_BEACON_OF_HOPE);
                    			DoCast(target, SPELL_BEACON_OF_HOPE);
                    			me->m_Events.AddEvent(new BlazingSongEvent(me), me->m_Events.CalculateTime(3000));
                    		}

                    		events.ScheduleEvent(EVENT_BEACON_OF_HOPE, urand(78000, 80000), 0, PHASE_COMBAT); // Not sure about this. :/
                    		break;

                    	case EVENT_CRANE_RUSH:
                    		Talk(SAY_CRANE_RUSH);
                    		DoCastAOE(SPELL_CRANE_RUSH, false);
                    		break;

                    	case EVENT_DEFEATED_1:
                    		Talk(SAY_DEFEATED_1);
                    		break;

                    	case EVENT_DEFEATED_2:
                    		me->Respawn();
                    		me->setActive(false);
                    		me->setFaction(35);
                    		me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    		break;

                    	case EVENT_DEFEATED_3:
                    		if (Creature* Shaohao = me->FindNearestCreature(NPC_EMPEROR_SHAOHAO_TI, 500.0f))
                    			Shaohao->AI()->Talk(SAY_DEFEATED_CHI_JI);
                    		break;
                    	default:
                    		break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
        	bool Defeated;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_chi_ji_tiAI(creature);
        }
};

void AddSC_worldboss_chiji()
{
    new boss_chi_ji_ti();
}
