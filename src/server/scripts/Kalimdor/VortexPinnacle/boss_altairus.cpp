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

#include "Map.h"
#include "MotionMaster.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "Util.h"
#include "vortex_pinnacle.h"

namespace
{
    uint32 const SpellCallWind = 88244;
    uint32 const SpellUpwind = 88282;
    uint32 const SpellDownwind = 88286;
    uint32 const SpellBreath = 88308;
    uint32 const SpellLightning = 88357;
    uint32 const SpellTwisterAura = 88313;
    uint32 const TwisterEntry = 47342;
    float const CenterX = -1183.97f;
    float const CenterY = 51.14f;
    float const CenterZ = 734.17f;
    float const PlatformRadius = 55.0f;
}

enum AltairusEvents
{
    EVENT_WIND = 1,
    EVENT_PLAYERS,
    EVENT_BREATH,
    EVENT_TWISTER
};

class boss_altairus : public CreatureScript
{
public:
    boss_altairus() : CreatureScript("boss_altairus") { }

    struct boss_altairusAI : public BossAI
    {
        boss_altairusAI(Creature* creature) : BossAI(creature, DATA_ALTAIRUS), wind(0), windActive(false) { }

        uint8 wind;
        bool windActive;

        void ClearWind()
        {
            if (instance)
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SpellUpwind);
                instance->DoRemoveAurasDueToSpellOnPlayers(SpellDownwind);
            }
        }

        void Reset() OVERRIDE
        {
            _Reset();
            windActive = false;
            ClearWind();
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            summons.Summon(summon);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            if (instance && instance->GetBossState(DATA_ALTAIRUS) != IN_PROGRESS)
                return;
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            events.ScheduleEvent(EVENT_WIND, 2000);
            events.ScheduleEvent(EVENT_PLAYERS, 1000);
            events.ScheduleEvent(EVENT_BREATH, urand(5000, 10000));
            if (IsHeroic())
                events.ScheduleEvent(EVENT_TWISTER, 1000);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            ClearWind();
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            _JustDied();
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
                    case EVENT_WIND:
                        wind = urand(0, 3);
                        windActive = true;
                        me->CastSpell(me, SpellCallWind, true);
                        events.ScheduleEvent(EVENT_WIND, 18000);
                        break;
                    case EVENT_PLAYERS:
                    {
                        Map::PlayerList const& players = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        {
                            Player* player = itr->GetSource();
                            if (!player || !player->IsAlive() || !me->IsValidAttackTarget(player))
                                continue;
                            float dx = player->GetPositionX() - me->GetPositionX();
                            float dy = player->GetPositionY() - me->GetPositionY();
                            bool near = player->GetExactDist2d(CenterX, CenterY) < 100.0f;
                            bool onPlatform = player->GetExactDist2d(CenterX, CenterY) <= PlatformRadius &&
                                player->GetPositionZ() > CenterZ - 10.0f && player->GetPositionZ() < CenterZ + 25.0f;
                            if (near && !onPlatform)
                                me->CastSpell(player, SpellLightning, true);
                            if (!near || !onPlatform || !windActive)
                            {
                                player->RemoveAurasDueToSpell(SpellUpwind);
                                player->RemoveAurasDueToSpell(SpellDownwind);
                                continue;
                            }
                            float projection = wind == 0 ? dx : wind == 1 ? -dx : wind == 2 ? dy : -dy;
                            uint32 desired = projection < 0.0f ? SpellUpwind : SpellDownwind;
                            uint32 other = projection < 0.0f ? SpellDownwind : SpellUpwind;
                            player->RemoveAurasDueToSpell(other);
                            if (!player->HasAura(desired))
                                me->AddAura(desired, player);
                        }
                        events.ScheduleEvent(EVENT_PLAYERS, 1000);
                        break;
                    }
                    case EVENT_BREATH:
                        if (!me->HasUnitState(UNIT_STATE_CASTING))
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SpellBreath);
                        events.ScheduleEvent(EVENT_BREATH, urand(10000, 16000));
                        break;
                    case EVENT_TWISTER:
                        if (Creature* twister = me->SummonCreature(TwisterEntry, CenterX + frand(-20.0f, 20.0f),
                            CenterY + frand(-20.0f, 20.0f), CenterZ, 0.0f))
                        {
                            twister->SetReactState(REACT_PASSIVE);
                            twister->SetSpeed(MOVE_RUN, 2.0f);
                            twister->CastSpell(twister, SpellTwisterAura, true);
                            twister->GetMotionMaster()->MoveRandom(10.0f);
                        }
                        events.ScheduleEvent(EVENT_TWISTER, urand(8000, 12000));
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_altairusAI(creature);
    }
};

void AddSC_boss_altairus()
{
    new boss_altairus();
}
