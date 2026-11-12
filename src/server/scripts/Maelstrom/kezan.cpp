/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"

/*####
## npc_defiant_troll (http://www.wowhead.com/quest=14069)
####*/

enum DefiantTrollEnum
{
    DEFFIANT_KILL_CREDIT               = 34830,
    SPELL_BELT_SHOCKER                 = 66306,
    SPELL_ENRAGE                       = 45111,
    QUEST_GOOD_HELP_IS_HARD_TO_FIND    = 14069,
    GO_DEPOSIT                         = 195489,
    SAY_WORK                           = 0
};

class npc_defiant_troll : public CreatureScript
{
public:
    npc_defiant_troll() : CreatureScript("npc_defiant_troll") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_defiant_trollAI(creature);
    }

    struct npc_defiant_trollAI : public ScriptedAI
    {
        npc_defiant_trollAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 rebuffTimer;
        bool work;
        bool shocked;

        void Reset () OVERRIDE
        {
            rebuffTimer = 0;
            work = false;
            shocked = false;
            me->CastSpell(me, SPELL_ENRAGE, true);
        }

        // The player's spellclick casts the shocker on the troll
        void SpellHit(Unit* caster, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id != SPELL_BELT_SHOCKER || shocked)
                return;

            Player* player = caster->ToPlayer();
            if (!player || player->GetQuestStatus(QUEST_GOOD_HELP_IS_HARD_TO_FIND) != QUEST_STATUS_INCOMPLETE)
                return;

            shocked = true;
            player->KilledMonsterCredit(DEFFIANT_KILL_CREDIT, me->GetGUID());
            Talk(SAY_WORK);
            me->RemoveAllAuras();

            if (GameObject* deposit = me->FindNearestGameObject(GO_DEPOSIT, 25.0f))
                me->GetMotionMaster()->MovePoint(1, deposit->GetPositionX() - 1.0f, deposit->GetPositionY(), deposit->GetPositionZ());
        }

        void MovementInform(uint32 /*type*/, uint32 id) OVERRIDE
        {
            if (id == 1)
                work = true;
        }

        bool IsWorking() const { return work; }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (IsWorking())
                me->HandleEmoteCommand(EMOTE_ONESHOT_WORK_MINING);

            if (rebuffTimer <= diff)
            {
                Reset();

                switch (urand(0, 2))
                {
                    case 0:
                        me->HandleEmoteCommand(EMOTE_STATE_EXCLAIM);
                        break;
                    case 1:
                        me->HandleEmoteCommand(EMOTE_STATE_DANCE);
                        break;
                    case 2:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_NONE);
                        break;
                }
                rebuffTimer = 120000; // Rebuff again in 2 minutes
            }
            else
                rebuffTimer -= diff;

            if (!UpdateVictim())
                return;

            //DoMeleeAttackIfReady();
        }
    };
};

void AddSC_kezan()
{
    new npc_defiant_troll();
}
