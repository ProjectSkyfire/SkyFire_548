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

/* ScriptData
SDName: The_Jade_Forest
SD%Complete: 0
SDComment: Placeholder
SDCategory: The_Jade_Forest
EndScriptData */

/* ContentData
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "ScriptedEscortAI.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Player.h"
#include "SpellInfo.h"
#include "CreatureTextMgr.h"




//Fix for quest The Trainer's Challenge: Ace Longpaw (id: 31717)
#define GOSSIP_ACCEPT_DUEL      "I challenge you!"
enum Spells_DKI
{
	SPELL_DUEL = 52996,
	//SPELL_DUEL_TRIGGERED        = 52990,
	SPELL_DUEL_VICTORY = 52994,
	SPELL_DUEL_FLAG = 52991,
};

enum Says_VBM
{
	SAY_DUEL = 0,
};

enum Misc_VBN
{
	QUEST_The_Trainer_s_Challenge_Ace_Longpaw = 31717,
	FACTION_HOSTILE = 2068
};

class npc_ace_longpaw_defeated : public CreatureScript
{
public:
	npc_ace_longpaw_defeated() : CreatureScript("npc_ace_longpaw_defeated") { }

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
	{
		player->PlayerTalkClass->ClearMenus();
		if (action == GOSSIP_ACTION_INFO_DEF)
		{
			player->CLOSE_GOSSIP_MENU();

			if (player->IsInCombat() || creature->IsInCombat())
				return true;

			if (npc_ace_longpaw_defeatedAI* pInitiateAI = CAST_AI(npc_ace_longpaw_defeated::npc_ace_longpaw_defeatedAI, creature->AI()))
			{
				if (pInitiateAI->m_bIsDuelInProgress)
					return true;
			}

			creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
			creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);

			sCreatureTextMgr->SendChat(creature, SAY_DUEL, 0, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_NORMAL, 0, TEAM_OTHER, false, player);

			player->CastSpell(creature, SPELL_DUEL, false);
			player->CastSpell(player, SPELL_DUEL_FLAG, true);
		}
		return true;
	}

	bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
	{
		if (player->GetQuestStatus(31717) == QUEST_STATUS_INCOMPLETE && creature->IsFullHealth())
		{
			if (player->HealthBelowPct(10))
				return true;

			if (player->IsInCombat() || creature->IsInCombat())
				return true;

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ACCEPT_DUEL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
			player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
		}
		return true;
	}

	CreatureAI* GetAI(Creature* creature) const OVERRIDE
	{
		return new npc_ace_longpaw_defeatedAI(creature);
	}

	struct npc_ace_longpaw_defeatedAI : public CombatAI
	{
		npc_ace_longpaw_defeatedAI(Creature* creature) : CombatAI(creature)
		{
			m_bIsDuelInProgress = false;
		}

		bool lose;
		uint64 m_uiDuelerGUID;
		uint32 m_uiDuelTimer;
		bool m_bIsDuelInProgress;

		void Reset() OVERRIDE
		{
			lose = false;
			me->RestoreFaction();
			CombatAI::Reset();

			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);

			m_uiDuelerGUID = 0;
			m_uiDuelTimer = 5000;
			m_bIsDuelInProgress = false;
		}

		void SpellHit(Unit* pCaster, const SpellInfo* pSpell) OVERRIDE
		{
			if (!m_bIsDuelInProgress && pSpell->Id == SPELL_DUEL)
			{
				m_uiDuelerGUID = pCaster->GetGUID();
				m_bIsDuelInProgress = true;
			}
		}

		void DamageTaken(Unit* pDoneBy, uint32 &uiDamage) OVERRIDE
		{
			if (m_bIsDuelInProgress && pDoneBy->IsControlledByPlayer())
			{
				if (pDoneBy->GetGUID() != m_uiDuelerGUID && pDoneBy->GetOwnerGUID() != m_uiDuelerGUID) // other players cannot help
					uiDamage = 0;
				else if (uiDamage >= me->GetHealth())
				{
					uiDamage = 0;

					if (!lose)
					{
						pDoneBy->RemoveGameObject(SPELL_DUEL_FLAG, true);
						pDoneBy->AttackStop();
						me->CastSpell(pDoneBy, SPELL_DUEL_VICTORY, true);
						lose = true;
						me->CastSpell(me, 7267, true);
						me->RestoreFaction();
					}
				}
			}
		}

		void UpdateAI(uint32 uiDiff) OVERRIDE
		{
			if (!UpdateVictim())
			{
				if (m_bIsDuelInProgress)
				{
					if (m_uiDuelTimer <= uiDiff)
					{
						me->setFaction(FACTION_HOSTILE);

						if (Unit* unit = Unit::GetUnit(*me, m_uiDuelerGUID))
							AttackStart(unit);
					}
					else
						m_uiDuelTimer -= uiDiff;
				}
				return;
			}

			if (m_bIsDuelInProgress)
			{
				if (lose)
				{
					if (!me->HasAura(7267))
						EnterEvadeMode();
					return;
				}
				else if (me->GetVictim()->GetTypeId() == TYPEID_PLAYER && me->GetVictim()->HealthBelowPct(10))
				{
					me->GetVictim()->CastSpell(me->GetVictim(), 7267, true); // beg
					me->GetVictim()->RemoveGameObject(SPELL_DUEL_FLAG, true);
					EnterEvadeMode();
					return;
				}
			}

			CombatAI::UpdateAI(uiDiff);
		}
	};

};

void AddSC_the_jade_forest()
{
    new npc_ace_longpaw_defeated();
}
