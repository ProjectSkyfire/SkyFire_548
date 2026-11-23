/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "zulaman.h"

enum Says
{
    SAY_WAVE_1              = 0,
    SAY_WAVE_2              = 1,
    SAY_WAVE_3              = 2,
    SAY_WAVE_4              = 3,
    SAY_AGGRO               = 4,
    SAY_PLAYER_KILL         = 5,
    SAY_SURGE               = 6,
    EMOTE_SURGE             = 7,
    EMOTE_BEAR              = 8,
    SAY_BEAR                = 9,
    SAY_TROLL               = 10,
    SAY_DEATH               = 11
};

enum Spells
{
    SPELL_BEAR_FORM         = 42377,
    SPELL_BRUTAL_STRIKE     = 42384,
    SPELL_SURGE             = 42402,
    SPELL_LACERATING_SLASH  = 42395,
    SPELL_DEAFENING_ROAR    = 49721
};

enum Events
{
    EVENT_BEAR_FORM         = 1,
    EVENT_BRUTAL_STRIKE     = 2,
    EVENT_SURGE             = 3,
    EVENT_LACERATING_SLASH  = 4,
    EVENT_DEAFENING_ROAR    = 5,
    EVENT_TROLL_FORM        = 6
};

class boss_nalorakk : public CreatureScript
{
    public:

        boss_nalorakk() : CreatureScript("boss_nalorakk") { }

        struct boss_nalorakkAI : public BossAI
        {
            boss_nalorakkAI(Creature* creature) : BossAI(creature, DATA_NALORAKK) { }

            void Reset() OVERRIDE
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);
                _EnterCombat();
                events.ScheduleEvent(EVENT_BEAR_FORM, 30000);
                events.ScheduleEvent(EVENT_BRUTAL_STRIKE, 6000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DEATH);
                _JustDied();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_PLAYER_KILL);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BRUTAL_STRIKE:
                            DoCastVictim(SPELL_BRUTAL_STRIKE);
                            events.ScheduleEvent(EVENT_SURGE, 2000);
                            events.ScheduleEvent(EVENT_BRUTAL_STRIKE, 8000);
                            break;
                        case EVENT_SURGE:
                            Talk(SAY_SURGE);
                            Talk(EMOTE_SURGE);
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 0.0f, true))
                                DoCast(target, SPELL_SURGE);
                            break;
                        case EVENT_BEAR_FORM:
                            Talk(EMOTE_BEAR);
                            Talk(SAY_BEAR);
                            events.CancelEvent(EVENT_BRUTAL_STRIKE);
                            events.CancelEvent(EVENT_SURGE);
                            DoCast(me, SPELL_BEAR_FORM);
                            events.ScheduleEvent(EVENT_LACERATING_SLASH, 6000);
                            events.ScheduleEvent(EVENT_DEAFENING_ROAR, urand(3000, 8000));
                            events.ScheduleEvent(EVENT_TROLL_FORM, 30000);
                            break;
                        case EVENT_LACERATING_SLASH:
                            DoCastVictim(SPELL_LACERATING_SLASH);
                            events.ScheduleEvent(EVENT_LACERATING_SLASH, 20000);
                            break;
                        case EVENT_DEAFENING_ROAR:
                            DoCast(me, SPELL_DEAFENING_ROAR);
                            events.ScheduleEvent(EVENT_DEAFENING_ROAR, 8000);
                            break;
                        case EVENT_TROLL_FORM:
                            Talk(SAY_TROLL);
                            events.CancelEvent(EVENT_LACERATING_SLASH);
                            events.CancelEvent(EVENT_DEAFENING_ROAR);
                            events.ScheduleEvent(EVENT_BEAR_FORM, 30000);
                            events.ScheduleEvent(EVENT_BRUTAL_STRIKE, 6000);
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
            return new boss_nalorakkAI(creature);
        }
};

void AddSC_boss_nalorakk()
{
    new boss_nalorakk();
}
