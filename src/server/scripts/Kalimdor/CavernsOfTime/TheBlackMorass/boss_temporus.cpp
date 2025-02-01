/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
Name: Boss_Temporus
%Complete: 75
Comment: More abilities need to be implemented
Category: Caverns of Time, The Black Morass
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_black_morass.h"

enum Enums
{
    SAY_ENTER               = 0,
    SAY_AGGRO               = 1,
    SAY_BANISH              = 2,
    SAY_SLAY                = 3,
    SAY_DEATH               = 4,

    SPELL_HASTE             = 31458,
    SPELL_MORTAL_WOUND      = 31464,
    SPELL_WING_BUFFET       = 31475,
    H_SPELL_WING_BUFFET     = 38593,
    SPELL_REFLECT           = 38592                       //Not Implemented (Heroic mod)
};

enum Events
{
    EVENT_HASTE             = 1,
    EVENT_MORTAL_WOUND      = 2,
    EVENT_WING_BUFFET       = 3,
    EVENT_SPELL_REFLECTION  = 4
};

class boss_temporus : public CreatureScript
{
public:
    boss_temporus() : CreatureScript("boss_temporus") { }

    struct boss_temporusAI : public BossAI
    {
        boss_temporusAI(Creature* creature) : BossAI(creature, TYPE_TEMPORUS) { }

        void Reset() OVERRIDE { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_HASTE, std::rand() % 23000 + 15000);
            events.ScheduleEvent(EVENT_MORTAL_WOUND, 8000);
            events.ScheduleEvent(EVENT_WING_BUFFET, std::rand() % 35000 + 25000);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_SPELL_REFLECTION, 30000);

            Talk(SAY_AGGRO);
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_SLAY);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetData(TYPE_RIFT, SPECIAL);
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE

        {
            //Despawn Time Keeper
            if (who->GetTypeId() == TypeID::TYPEID_UNIT && who->GetEntry() == NPC_TIME_KEEPER)
            {
                if (me->IsWithinDistInMap(who, 20.0f))
                {
                    Talk(SAY_BANISH);

                    me->DealDamage(who, who->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
            }

            ScriptedAI::MoveInLineOfSight(who);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_HASTE:
                        DoCast(me, SPELL_HASTE);
                        events.ScheduleEvent(EVENT_HASTE, std::rand() % 25000 + 20000);
                        break;
                    case EVENT_MORTAL_WOUND:
                        DoCast(me, SPELL_MORTAL_WOUND);
                        events.ScheduleEvent(EVENT_MORTAL_WOUND, std::rand() % 20000 + 10000);
                        break;
                    case EVENT_WING_BUFFET:
                        DoCast(me, SPELL_WING_BUFFET);
                        events.ScheduleEvent(EVENT_WING_BUFFET, std::rand() % 30000 + 20000);
                        break;
                    case EVENT_SPELL_REFLECTION: // Only in Heroic
                        DoCast(me, SPELL_REFLECT);
                        events.ScheduleEvent(EVENT_SPELL_REFLECTION, std::rand() % 35000 + 25000);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_temporusAI(creature);
    }
};

void AddSC_boss_temporus()
{
    new boss_temporus();
}
