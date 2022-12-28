/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
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

        void Reset() override
        {
            events.Reset();
            playerGUID = 0;
            me->setFaction(190);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_SOOTHE_EARTH_SPIRIT && me->getFaction() == 190)
            {
                Position pos;
                caster->GetNearPoint(caster, pos.m_positionX, pos.m_positionY, pos.m_positionZ, 0.0f, MIN_MELEE_REACH, caster->GetAngle(me));
                me->GetMotionMaster()->MovePoint(1, pos);
                playerGUID = caster->GetGUID();
            }
        }

        void MovementInform(uint32 type, uint32 pointId) override
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

        void EnterCombat(Unit* /*victim*/) override
        {
            events.ScheduleEvent(EVENT_ROCK_BARRAGE, urand(4000, 5000));
        }

        void UpdateAI(uint32 diff) override
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

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_agitated_earth_spiritAI(creature);
    }
};

void AddSC_mulgore()
{
    new npc_agitated_earth_spirit();
}
