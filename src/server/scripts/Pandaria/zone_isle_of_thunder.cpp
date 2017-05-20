/*
* Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
* Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
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
SDName: Isle_Of_Thunder
SD%Complete: 0
SDComment: Placeholder
SDCategory: Isle_Of_Thunder
EndScriptData */

/* ContentData
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum NalakTalk
{
	SAY_INTRO = 0, // "I am born of thunder!"
	SAY_AGGRO = 1, // "Can you feel the chill windblow? The storm is comming."

	SAY_SPELL1 = 2, // ARC_NOVA "The clouds arc with vengeance!"
	SAY_SPELL2 = 3, // LIGHTNING_TETHER "The air crackles with anger!"

	SAY_KILL = 4, // "The sky weeps for your demise!"
	SAY_DEATH = 5, // "I am but... The darkness... Before the storm..."
};

enum Nalak
{
	TIMER_ARC_NOVA = 10000,
	TIMER_LIGHTNING_TETHER = 25000,
	TIMER_STORMCLOUD = 15000,

	SPELL_ARC_NOVA = 136338,
	SPELL_LIGHTNING_TETHER = 136339,
	SPELL_STORMCLOUD = 136340
};

class boss_nalak : public CreatureScript
{
public:
	boss_nalak() : CreatureScript("boss_nalak") { }

	CreatureAI* GetAI(Creature* creature) const OVERRIDE
	{
		return new boss_nalakAI(creature);
	}

	struct boss_nalakAI : public ScriptedAI
	{
		boss_nalakAI(Creature* creature) : ScriptedAI(creature) { }

		uint32 Arc_Nova_Timer;
		uint32 Lightning_Tether_Timer;
		uint32 Stormcloud_Timer;

		void Reset() OVERRIDE
		{
			Arc_Nova_Timer = TIMER_ARC_NOVA;
			Lightning_Tether_Timer = TIMER_LIGHTNING_TETHER;
			Stormcloud_Timer = TIMER_STORMCLOUD;
		}

		void JustRespawned() override
		{
			Talk(SAY_INTRO);
		}

		void EnterCombat(Unit* /*who*/) override
		{
			Talk(SAY_AGGRO);
		}

		void KilledUnit(Unit* victim) override
		{
			Talk(SAY_KILL);
		}

		void JustDied(Unit* /*killer*/) override
		{
			Talk(SAY_DEATH);
		}

		void UpdateAI(uint32 diff) OVERRIDE
		{
			if (!UpdateVictim())
				return;

			if (Arc_Nova_Timer <= diff)
			{
				Talk(SAY_SPELL1);
				DoCastVictim(SPELL_ARC_NOVA);
				Arc_Nova_Timer = TIMER_ARC_NOVA;
			}
			else Arc_Nova_Timer -= diff;

			if (Lightning_Tether_Timer <= diff)
			{
				Talk(SAY_SPELL2);
				DoCastVictim(SPELL_LIGHTNING_TETHER);
				Lightning_Tether_Timer = TIMER_LIGHTNING_TETHER;
			}
			else Lightning_Tether_Timer -= diff;

			if (Stormcloud_Timer <= diff)
			{
				DoCastVictim(SPELL_STORMCLOUD);
				Stormcloud_Timer = TIMER_STORMCLOUD;
			}
			else Stormcloud_Timer -= diff;

			DoMeleeAttackIfReady();
		}
	};
};

void AddSC_isle_of_thunder()
{
	new boss_nalak();
}
