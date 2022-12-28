/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Isle_of_Queldanas
SD%Complete: 100
SDComment: Quest support: 11524, 11525, 11532, 11533, 11542, 11543, 11541
SDCategory: Isle Of Quel'Danas
EndScriptData */

/* ContentData
npc_converted_sentry
npc_greengill_slave
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "Pet.h"
#include "SpellInfo.h"

/*######
## npc_converted_sentry
######*/
enum ConvertedSentry
{
    SAY_CONVERTED           = 0,

    SPELL_CONVERT_CREDIT    = 45009
};

class npc_converted_sentry : public CreatureScript
{
public:
    npc_converted_sentry() : CreatureScript("npc_converted_sentry") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_converted_sentryAI(creature);
    }

    struct npc_converted_sentryAI : public ScriptedAI
    {
        npc_converted_sentryAI(Creature* creature) : ScriptedAI(creature) { }

        bool Credit;
        uint32 Timer;

        void Reset() override
        {
            Credit = false;
            Timer = 2500;
        }

        void MoveInLineOfSight(Unit* /*who*/) override { }

        void EnterCombat(Unit* /*who*/) override { }

        void UpdateAI(uint32 diff) override
        {
            if (!Credit)
            {
                if (Timer <= diff)
                {
                    Talk(SAY_CONVERTED);

                    DoCast(me, SPELL_CONVERT_CREDIT);
                    if (me->IsPet())
                        me->ToPet()->SetDuration(7500);
                    Credit = true;
                } else Timer -= diff;
            }
        }
    };
};

/*######
## npc_greengill_slave
######*/

#define ENRAGE  45111
#define ORB     45109
#define QUESTG  11541
#define DM      25060

class npc_greengill_slave : public CreatureScript
{
public:
    npc_greengill_slave() : CreatureScript("npc_greengill_slave") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_greengill_slaveAI(creature);
    }

    struct npc_greengill_slaveAI : public ScriptedAI
    {
        npc_greengill_slaveAI(Creature* creature) : ScriptedAI(creature) { }

        void EnterCombat(Unit* /*who*/) override { }

        void SpellHit(Unit* caster, SpellInfo const* spellInfo) override
        {
            Player* player = caster->ToPlayer();
            if (!player)
                return;

            if (spellInfo->Id == ORB && !me->HasAura(ENRAGE))
            {
                if (player->GetQuestStatus(QUESTG) == QUEST_STATUS_INCOMPLETE)
                    DoCast(player, 45110, true);

                DoCast(me, ENRAGE);

                if (Creature* Myrmidon = me->FindNearestCreature(DM, 70))
                {
                    me->AddThreat(Myrmidon, 100000.0f);
                    AttackStart(Myrmidon);
                }
            }
        }

        void UpdateAI(uint32 /*diff*/) override
        {
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_isle_of_queldanas()
{
    new npc_converted_sentry();
    new npc_greengill_slave();
}
