/*
 * Copyright (C) 2011-2022 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2022 MaNGOS <https://www.getmangos.eu/>
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
 SDName: Mulgore
 SD%Complete: 100
 SDComment: Support for quest: 14491
 SDCategory: Mulgore
 EndScriptData */

 /* ContentData
 npc_agitated_earth_spirit
 EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "SpellInfo.h"

enum eAgitatedEarthSpirit
{
    SPELL_SOOTHE_EARTH_SPIRIT       = 69453,
    SPELL_ROCK_BARRAGE              = 81305,

    NPC_EARTH_SPIRIT_CREDIT_BUNNY   = 36872,

    EVENT_ROCK_BARRAGE              = 1
};

// 36845 - Agitated Earth Spirit
class npc_agitated_earth_spirit : public CreatureScript
{
public:
    npc_agitated_earth_spirit() : CreatureScript("npc_agitated_earth_spirit") { }

    struct npc_agitated_earth_spiritAI : public ScriptedAI
    {
        npc_agitated_earth_spiritAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
            playerGUID = 0;
            me->setFaction(190);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == SPELL_SOOTHE_EARTH_SPIRIT && me->getFaction() == 190)
            {
                Position pos;
                caster->GetNearPoint(caster, pos.m_positionX, pos.m_positionY, pos.m_positionZ, 0.0f, MIN_MELEE_REACH, caster->GetAngle(me));
                me->GetMotionMaster()->MovePoint(1, pos);
                playerGUID = caster->GetGUID();
            }
        }

        void MovementInform(uint32 type, uint32 pointId) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE && pointId == 1)
            {
                switch (urand(0, 1))
                {
                    case 0:
                    {
                        me->setFaction(35);
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                            player->KilledMonsterCredit(NPC_EARTH_SPIRIT_CREDIT_BUNNY);

                        me->DespawnOrUnsummon(1000);
                        break;
                    }
                    case 1:
                        me->setFaction(14);
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                        {
                            Talk(0, player);
                            AttackStart(player);
                        }
                        break;
                }
            }
        }

        void EnterCombat(Unit* /*victim*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_ROCK_BARRAGE, urand(4000, 5000));
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
                    case EVENT_ROCK_BARRAGE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_ROCK_BARRAGE);
                        events.ScheduleEvent(EVENT_ROCK_BARRAGE, urand(18000, 21000));
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        uint64 playerGUID;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_agitated_earth_spiritAI(creature);
    }
};

void AddSC_mulgore()
{
    new npc_agitated_earth_spirit();
}
