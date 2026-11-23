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
#include "ObjectAccessor.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "Util.h"
#include "vortex_pinnacle.h"
#include <cmath>

enum ErtanSpells
{
    SPELL_VORTEX_AURA = 86267,
    SPELL_CYCLONE_SHIELD = 86292,
    SPELL_STORMS_EDGE = 86309,
    SPELL_LIGHTNING_BOLT = 86331,
    SPELL_SUMMON_TEMPEST = 86340
};

enum ErtanEvents
{
    EVENT_BOLT = 1,
    EVENT_STORMS_EDGE,
    EVENT_RETRACT,
    EVENT_EXPAND,
    EVENT_TEMPEST
};

namespace
{
    float const CenterX = -719.97f;
    float const CenterY = 4.33f;
    float const CenterZ = 635.67f;
    float const OrbitRadius = 25.0f;
    float const InnerRadius = 5.0f;
    float const TwoPi = 6.2831853f;
    uint32 const VortexEntry = 46007;
    uint8 const VortexCount = 8;
}

class boss_grand_vizier_ertan : public CreatureScript
{
public:
    boss_grand_vizier_ertan() : CreatureScript("boss_grand_vizier_ertan") { }

    struct boss_grand_vizier_ertanAI : public BossAI
    {
        boss_grand_vizier_ertanAI(Creature* creature) : BossAI(creature, DATA_ERTAN) { }

        uint64 vortexGuids[VortexCount];
        uint32 moveTimer;
        float angle;
        bool retracted;

        void Reset() OVERRIDE
        {
            _Reset();
            for (uint8 i = 0; i < VortexCount; ++i)
                vortexGuids[i] = 0;
            moveTimer = 1000;
            angle = 0.0f;
            retracted = false;
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            summons.Summon(summon);
            if (summon->GetEntry() != VortexEntry && me->IsInCombat())
                DoZoneInCombat(summon);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            if (instance && instance->GetBossState(DATA_ERTAN) != IN_PROGRESS)
                return;
            Talk(0);
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            for (uint8 i = 0; i < VortexCount; ++i)
            {
                float a = TwoPi * i / VortexCount;
                if (Creature* vortex = me->SummonCreature(VortexEntry, CenterX + OrbitRadius * std::cos(a),
                    CenterY + OrbitRadius * std::sin(a), CenterZ, 0.0f))
                {
                    vortexGuids[i] = vortex->GetGUID();
                    vortex->SetReactState(REACT_PASSIVE);
                    vortex->SetDisableGravity(true);
                    vortex->SetSpeed(MOVE_RUN, 2.0f);
                    vortex->CastSpell(vortex, SPELL_VORTEX_AURA, true);
                }
            }
            events.ScheduleEvent(EVENT_BOLT, 3000);
            events.ScheduleEvent(EVENT_STORMS_EDGE, 1000);
            events.ScheduleEvent(EVENT_RETRACT, 24000);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_TEMPEST, 16000);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            _JustDied();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (moveTimer <= diff)
            {
                if (!retracted)
                    angle += 0.04f;
                float radius = retracted ? InnerRadius : OrbitRadius;
                for (uint8 i = 0; i < VortexCount; ++i)
                    if (Creature* vortex = ObjectAccessor::GetCreature(*me, vortexGuids[i]))
                    {
                        float a = angle + TwoPi * i / VortexCount;
                        vortex->GetMotionMaster()->MovePoint(0, CenterX + radius * std::cos(a),
                            CenterY + radius * std::sin(a), CenterZ, false);
                    }
                moveTimer = 1000;
            }
            else
                moveTimer -= diff;

            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BOLT:
                        if (!me->HasUnitState(UNIT_STATE_CASTING))
                            DoCastVictim(SPELL_LIGHTNING_BOLT);
                        events.ScheduleEvent(EVENT_BOLT, 2500);
                        break;
                    case EVENT_STORMS_EDGE:
                    {
                        Map::PlayerList const& players = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            if (Player* player = itr->GetSource())
                                if (player->IsAlive() && me->IsValidAttackTarget(player) && me->GetExactDist2d(player) < 70.0f &&
                                    player->GetExactDist2d(CenterX, CenterY) > (retracted ? InnerRadius : OrbitRadius))
                                    me->CastSpell(player, SPELL_STORMS_EDGE, true);
                        events.ScheduleEvent(EVENT_STORMS_EDGE, 1000);
                        break;
                    }
                    case EVENT_RETRACT:
                    {
                        Talk(1);
                        retracted = true;
                        Map::PlayerList const& players = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            if (Player* player = itr->GetSource())
                                if (player->IsAlive() && me->IsValidAttackTarget(player) && me->GetExactDist2d(player) <= 5.0f)
                                    player->KnockbackFrom(me->GetPositionX(), me->GetPositionY(), 15.0f, 10.0f);
                        events.ScheduleEvent(EVENT_EXPAND, 6000);
                        break;
                    }
                    case EVENT_EXPAND:
                        retracted = false;
                        events.ScheduleEvent(EVENT_RETRACT, 25000);
                        break;
                    case EVENT_TEMPEST:
                    {
                        float a = frand(0.0f, TwoPi);
                        me->CastSpell(CenterX + OrbitRadius * std::cos(a), CenterY + OrbitRadius * std::sin(a),
                            CenterZ, SPELL_SUMMON_TEMPEST, true);
                        events.ScheduleEvent(EVENT_TEMPEST, 17000);
                        break;
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_grand_vizier_ertanAI(creature);
    }
};

void AddSC_boss_grand_vizier_ertan()
{
    new boss_grand_vizier_ertan();
}
