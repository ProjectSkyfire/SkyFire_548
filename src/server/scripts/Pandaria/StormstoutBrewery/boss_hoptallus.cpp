/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stormstout_brewery.h"
#include "Player.h"
#include "SpellScript.h"

enum Spells
{
    SPELL_FURLWIND = 112992,
    SPELL_CARROT_BREATH = 112944,
    SPELL_HOPTALLUS_SCREECH = 114367,
};
static const uint32 hoppers[2] = { 56718, 59426 };
static const Position hopperSpawnPos[] =
{
    { -726.7f, 1257.9f, 166.8f, 0.24f },
    { -720.86f, 1253.44f, 166.8f, 0.24f },
    { -726.86f, 1247.41f, 166.8f, 0.25f },
    { -719.38f, 1255.37f, 166.8f, 0.25f}
};

static const uint32 hoplings[5] = { 59461, 56631, 59459, 59458, 59460 };
static const Position hoplingSpawnPos[] =
{
    { -719.31f, 1248.27f, 166.8f, 0.24f },
    { -719.95f, 1250.89f, 166.8f, 0.24f },
    { -720.66f, 1253.79f, 166.8f, 0.24f },
    { -721.25f, 1256.18f, 166.8f, 0.24f },
    { -721.94f, 1259.02f, 166.8f, 0.24f }
};

class boss_hoptallus : public CreatureScript
{
    enum hoptalusEvents
    {
        EVENT_FURLWIND = 1,
        EVENT_CARROT_BREATH = 2,
        EVENT_SCREECH = 3,
    };
public:
    boss_hoptallus() : CreatureScript("boss_hoptallus") { }

    struct boss_hoptallusAI : public BossAI
    {
        boss_hoptallusAI(Creature* creature) : BossAI(creature, DATA_HOPTALLUS) { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();

            //Talk(SAY_AGGRO);

            events.ScheduleEvent(EVENT_SCREECH, 30000);
            events.ScheduleEvent(EVENT_FURLWIND, DUNGEON_MODE(16000, 10000));
            events.ScheduleEvent(EVENT_CARROT_BREATH, DUNGEON_MODE(30000, 15000));
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();

            //Talk(SAY_DEATH);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (me->HasAura(SPELL_FURLWIND) || me->HasAura(SPELL_CARROT_BREATH) || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FURLWIND:
                {
                    DoCastVictim(SPELL_FURLWIND);
                    events.ScheduleEvent(EVENT_FURLWIND, DUNGEON_MODE(50000, 40000));
                    break;
                }
                case EVENT_CARROT_BREATH:
                {
                    // Carrot Breath crashing server after 1st tick knockback
                    // DoCastVictim(SPELL_CARROT_BREATH);
                    events.ScheduleEvent(EVENT_CARROT_BREATH, DUNGEON_MODE(27000, 25000));
                    break;
                }
                case EVENT_SCREECH:
                {
                    for (uint8 i = 0; i < 5; i++)
                    {
                        if (Creature* creature = me->SummonCreature(hoplings[i], hoplingSpawnPos[i], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000))
                        {
                            creature->setFaction(14);
                            creature->AddAura(114284, creature);

                            Position pos;
                            me->GetRandomNearPosition(pos, 10.f);
                            creature->GetMotionMaster()->MoveJump(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 15.f, 15.f, EVENT_JUMP);
                        }

                    }
                    for (uint8 j = 0; j < 4; j++)
                    {
                        if (Creature* creature = me->SummonCreature(hoppers[j], hopperSpawnPos[j], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000))
                        {
                            creature->setFaction(14);
                            Position pos;
                            me->GetRandomNearPosition(pos, 10.f);
                            creature->GetMotionMaster()->MoveJump(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 15.f, 15.f, EVENT_JUMP);
                        }

                    }
                    
                    DoCast(SPELL_HOPTALLUS_SCREECH);
                    events.ScheduleEvent(EVENT_SCREECH, 30000);
                    break;
                }
                default:
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetStormstoutBreweryAI<boss_hoptallusAI>(creature);
    }
};

void AddSC_boss_hoptallus()
{
    new boss_hoptallus();
}
