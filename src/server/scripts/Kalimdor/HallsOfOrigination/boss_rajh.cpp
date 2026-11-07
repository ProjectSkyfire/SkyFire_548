/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
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
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "GridNotifiers.h"
#include "halls_of_origination.h"

// Sun-Touched Spriteling 39370
// Sun-Touched Sprite 39369

enum Spells
{
    // Rajh
    SPELL_SUN_STRIKE                    = 73872,
    SPELL_SUN_STRIKE_DMG                = 73874,
    //SPELL_INFERNO_LEAP                = 87647, // Value: 32375 to 37625
    //SPELL_INFERNO_LEAP_2              = 89876, // Value: 120250 to 139750
    //SPELL_SUMMON_SUN_ORB              = 80352,
    SPELL_BLESSING_OF_THE_SUN           = 76352,
    SPELL_BLESSING_OF_THE_SUN_AURA      = 76355,
    SPELL_ZERO_POWER                    = 72242,
    SPELL_SUMMON_SOLAR_WINDS            = 74104,
    SPELL_SOLAR_WINDS_AURA              = 74107,
    SPELL_SOLAR_WINDS_DMG               = 74108,
    SPELL_SOLAR_WINDS_VISUAL            = 74109,
    SPELL_SOLAR_FIRE                    = 89131,
    SPELL_SOLAR_FIRE_DMG                = 89133,
};

enum Texts
{
    SAY_DEATH    = 3,
    SAY_AGGRO    = 0,
    SAY_ENERGIZE = 1,
    SAY_KILL     = 2,
};

const Position pointcenter = {-319.68f, 193.42f, 343.94f, 4.76f};

enum Events
{
    EVENT_SUN_STRIKE           = 1,
    EVENT_SUMMON_SOLAR_WINDS   = 2,
};

enum Adds
{
    NPC_SOLAR_WINDS1            = 39634,
    NPC_SOLAR_WINDS2            = 39635, // 74109
    NPC_SOLAR_WINDS3            = 47922,
    NPC_SUN_TOUCHED_SPRITELING  = 39370, // 76159
    NPC_SUN_TOUCHED_SPRITE      = 39369, // 76158,
    NPC_ORB_OF_SUN              = 40835, // 73658
    NPC_INFERNO_LEAP            = 47040,
    NPC_BLAZING_INFERNO         = 40927,
};

class boss_rajh : public CreatureScript
{
    public:
        boss_rajh() : CreatureScript("boss_rajh") { }

        struct boss_rajhAI : public BossAI
        {
            boss_rajhAI(Creature* creature) : BossAI(creature, DATA_RAJH), phase(0)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            }

            uint8 phase;

            void Reset() OVERRIDE
            {
                _Reset();
                me->SetReactState(REACT_AGGRESSIVE);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->GetMap()->SetWorldState(WORLDSTATE_SUN_OF_A, 1);

                phase = 0;

                SetEquipmentSlots(false, 1728, 0, 0);

                DoCast(me, SPELL_ZERO_POWER, true);
                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 100);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Talk(SAY_DEATH);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_SUN_STRIKE, urand(5000, 10000));
                events.ScheduleEvent(EVENT_SUMMON_SOLAR_WINDS, urand(10000, 15000));

                DoZoneInCombat();
            }

            void JustReachedHome() OVERRIDE
            {
                _JustReachedHome();
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch (pointId)
                    {
                        case 1:
                            Talk(SAY_ENERGIZE);
                            DoCastAOE(SPELL_BLESSING_OF_THE_SUN);
                            break;
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->GetPower(POWER_ENERGY) < 1 && phase == 0)
                {
                    phase = 1;
                    events.Reset();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->GetMotionMaster()->MovePoint(1, pointcenter);
                    return;
                }
                else if (me->GetPower(POWER_ENERGY) > 99 && phase == 1)
                {
                    phase = 0;
                    me->GetMap()->SetWorldState(WORLDSTATE_SUN_OF_A, 0);
                    me->SetReactState(REACT_AGGRESSIVE);
                    events.ScheduleEvent(EVENT_SUN_STRIKE, urand(5000, 10000));
                    events.ScheduleEvent(EVENT_SUMMON_SOLAR_WINDS, urand(6000, 11000));
                    me->GetMotionMaster()->MoveChase(me->GetVictim());
                    return;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUN_STRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_SUN_STRIKE);
                            events.ScheduleEvent(EVENT_SUN_STRIKE, urand(20000, 25000));
                            break;
                        case EVENT_SUMMON_SOLAR_WINDS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_SUMMON_SOLAR_WINDS);
                            events.ScheduleEvent(EVENT_SUMMON_SOLAR_WINDS, urand(20000, 25000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_rajhAI(creature);
        }
};

class npc_rajh_solar_wind : public CreatureScript
{
    public:
        npc_rajh_solar_wind() : CreatureScript("npc_rajh_solar_wind") { }

        struct npc_rajh_solar_windAI : public ScriptedAI
        {
            npc_rajh_solar_windAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset() OVERRIDE
            {
                DoCast(me, SPELL_SOLAR_WINDS_VISUAL, true);
                DoCast(me, SPELL_SOLAR_WINDS_AURA, true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_rajh_solar_windAI(creature);
        }
};

void AddSC_boss_rajh()
{
    new boss_rajh();
    new npc_rajh_solar_wind();
}
