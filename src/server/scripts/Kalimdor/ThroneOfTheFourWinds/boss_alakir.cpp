/*
* This file is part of the Legends of Azeroth Pandaria. See THANKS file for Copyright information
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

#include "throne_of_the_four_winds.h"
#include "ScriptMgr.h"
#include "WorldPacket.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "GridNotifiers.h"
#include "MoveSplineInit.h"
#include "DBCStores.h"
#include "Containers.h"

enum ScriptTextsAlAkir
{
    SAY_AGGRO                       = 1,
    SAY_WIND_BURST                  = 2,
    SAY_PHASE_2                     = 3,
    SAY_STORMLING                   = 4,
    SAY_PHASE_3                     = 5,
    SAY_KILL                        = 6,
    SAY_BERSERK                     = 7,
    SAY_DEFEAT                      = 8,
};

enum Spells
{
    SPELL_BERSERK                   = 95211,

    // Phase One
    SPELL_WIND_BURST                = 87770,
    SPELL_SQUALL_LINE_ACTIVATE      = 88779,
    SPELL_SQUALL_LINE_SCRIPT_EFFECT = 91129,
    SPELL_SQUALL_LINE_SUMMON_SW     = 88781,
    SPELL_SQUALL_LINE_SUMMON_SE     = 91104,
    SPELL_LIGHTNING_STRIKE          = 88214,
    SPELL_LIGHTNING_STRIKE_VISUAL   = 88230,
    SPELL_LIGHTNING_STRIKE_OTHERS   = 95764,
    SPELL_ELECTROCUTE               = 88427,
    SPELL_STATIC_SHOCK              = 87873,

    // Phase Two
    SPELL_ACID_RAIN                 = 88290,
    SPELL_ACID_RAIN_REMOVE          = 91216,

    // Phase Three
    SPELL_RELENTLESS_STORM_TRIGGER  = 89528,
    SPELL_RELENTLESS_STORM_VEHICLE  = 89527,
    SPELL_RELENTLESS_STORM_TELEPORT = 89661,
    SPELL_RELENTLESS_STORM          = 88875,
    SPELL_WIND_BURST_AURA           = 89638,
    SPELL_WIND_BURST_PHASE_THREE    = 88858,
    SPELL_EYE_OF_THE_STORM_INITIAL  = 89107,
    SPELL_LIGHTNING_CLOUDS_AURA     = 89639,
    SPELL_LIGHTNING_CLOUDS          = 89628,
    SPELL_LIGHTNING_CLOUDS_SUMMON   = 89565,
    SPELL_LIGHTNING_CLOUDS_SUMMON_VISUAL = 89577,
    SPELL_LIGHTNING_CLOUDS_DAMAGE_AURA = 89587,
    SPELL_LIGHTNING_CLOUDS_VISUAL   = 89569,
    SPELL_LIGHTNING                 = 89641,
    SPELL_LIGHTING_ROD_AURA         = 89690,

    // Squall Line
    SPELL_SQUALL_LINE_PREPARATION   = 87652,
    SPELL_SQUALL_LINE_VEHICLE       = 87856,
    SPELL_SQUALL_LINE_SAFEGUARD     = 87860,
    SPELL_SQUALL_LINE_DISMOUNT      = 95757,

    // Ice Storm
    SPELL_ICE_STORM_INITIAL_SUMMON  = 88239,
    SPELL_ICE_STORM_PERIODIC_SUMMON = 87053,
    SPELL_ICE_STORM_SPAWN_VISUAL    = 87472,
    SPELL_ICE_STORM_AURA            = 87469,

    // Stormling
    SPELL_FEEDBACK                  = 87904,
    SPELL_STORMLING_AURA            = 87905,
    SPELL_STORMLING_AURA_VISUAL     = 87906,
    SPELL_STORMLING_SUMMON_VISUAL   = 87913,
    SPELL_STORMLING_SUMMON          = 88272,
};

enum Creatures
{
    NPC_LIGHTNING_STRIKE_TRIGGER    = 48977,
    NPC_SQUALL_LINE_VEHICLE_SW      = 47034,
    NPC_SQUALL_LINE_VEHICLE_SE      = 48852,
    NPC_SQUALL_LINE_SW              = 48854,
    NPC_SQUALL_LINE_SE              = 48855,
    NPC_ICE_STORM_INITIAL           = 46734,
    NPC_ICE_STORM_GROUND            = 46973,
    NPC_STORMLING_PRE_EFFECT        = 47177,
    NPC_STORMLING                   = 47175,
    NPC_LIGHTNING_CLOUD             = 48190,
    NPC_LIGHTNING_CLOUD_VISUAL      = 48196,
    NPC_RELENTLESS_STORM_VEHICLE    = 47806,
    NPC_RELENTLESS_STORM            = 47807,
};

enum Events
{
    EVENT_BERSERK = 1,      // Persistent: Applies a Berserker buff to Al'Akir and all allies if not killed within 10 minutes
    EVENT_CHECK_POSITION,   // Persistent: Remove flying buff if a player is far from Al'Akir
    EVENT_EVADE,            // Persistent: Evade if no player is near Al'Akir for 10 seconds
    EVENT_ICE_STORM,        // Phase 1: Summons a trigger that walks around the platform, spawning patches of ice every second
    EVENT_STATIC_SHOCK,     // Phase 1: (Heroic only) Interrupts cast for all players
    EVENT_WIND_BURST,       // Phase 1: Knocks all players back
    EVENT_LIGHTNING_STRIKE, // Phase 1: Attacks (in Heroic - continuously for 16 seconds) all players withing a 60 degree cone directed at a random player with lightning
    EVENT_ELECTROCUTE,      // Phase 1+2: Channel Electrocute if the current target is out of melee range
    EVENT_SQUALL_LINE,      // Phase 1+2: Activates one of the Squall Lines constantly spinning around the platform
    EVENT_SUMMON_STORMLING, // Phase 2: Summons Stormling Pre-Effect near a random player which summons Stormling after the aura runs out
    EVENT_LIGHTNING,        // Phase 3: Attacks a random target with lightning
    EVENT_WIND_BURST_START, // Phase 3: Apply an aura that will periodically trigger Wind Burst on players
    EVENT_DESTROY_PLATFORM, // Phase 3: Destroy the platform Al'Akir is standing on
    EVENT_ENTER_VEHICLES,   // Phase 3: Mount players onto Relentless Storm Initial Vehicle
    EVENT_EXIT_VEHICLES,    // Phase 3: Remove players from Relentless Storm Initial Vehicle
};

enum Phases
{
    PHASE_NONE                      = 0,
    PHASE_ONE                       = 1,
    PHASE_TWO                       = 2,
    PHASE_THREE                     = 3,
};

enum LightOverride
{
    NORMAL                          = 2768,
    DARK                            = 2810,
};

enum Achievements
{
    ACHIEVEMENT_ALAKIR_DEAD         = 5123,
    ACHIEVEMENT_FOUR_PLAY           = 5305,
    ACHIEVEMENT_KILLED_ALAKIR       = 5463,
};

void GetPositionAround(Position const* pos, float dist, float startAngle, int32 step, int32 steps, float& x, float& y, float& z)
{
    float angle = startAngle + 2 * M_PI * step / steps;
    pos->GetPosition(x, y, z);
    x += cos(angle) * dist;
    y += sin(angle) * dist;
}

class boss_alakir : public CreatureScript
{
    public:
        boss_alakir() : CreatureScript("boss_alakir") { }

        struct boss_alakirAI : public BossAI
        {
            boss_alakirAI(Creature* creature) : BossAI(creature, DATA_ALAKIR)
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
                me->setActive(true);
                _phase = PHASE_NONE;
            }

            void Reset() OVERRIDE
            {
                _Reset();
                persistentEvents.Reset();

                if (GameObject* floor = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_CENTER, 500))
                    floor->SetGoState(GOState::GO_STATE_READY);
                if (GameObject* floor = me->FindNearestGameObject(GO_FLOOR_ALAKIR, 500))
                    floor->SetDestructibleState(GO_DESTRUCTIBLE_INTACT);

                me->SetReactState(REACT_DEFENSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                if (instance->CheckRequiredBosses(DATA_ALAKIR))
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                else
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 45);

                Map::PlayerList const& PlayerList = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (Player* plr = i->GetSource())
                    {
                        // the last phase sets players flying directly; a wipe must undo it for the dead too
                        plr->RemoveAurasDueToSpell(SPELL_WIND_BURST_PHASE_THREE);
                        plr->RemoveAurasDueToSpell(SPELL_EYE_OF_THE_STORM);
                        plr->SetCanFly(false);
                        plr->SetDisableGravity(false);
                    }
                }

                SetZoneWeather(me->GetZoneId(), WEATHER_STATE_FINE, 0.5f);
                SetZoneOverrideLight(me->GetZoneId(), NORMAL, 5000);

                _phase = PHASE_NONE;
                lightningCloudsAltitudes.clear();
                squallLineCount = 0;
                windBurstSaid = false;
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void SpellHitTarget(Unit* target, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == SPELL_LIGHTNING_CLOUDS)
                {
                    lightningCloudsAltitudes[target->GetPositionZ()] = getMSTime() + 5000;

                    float const dists[2] = { 50, 80 };
                    int32 const steps[2] = { 8, 12 };
                    float const startAngles[2] = { M_PI / 8, 0 };

                    for (uint32 i = 0; i < 2; ++i)
                    {
                        for (int32 step = 0; step < steps[i]; ++step)
                        {
                            float x, y, z;
                            GetPositionAround(me, dists[i], startAngles[i], step, steps[i], x, y, z);
                            z = target->GetPositionZ();
                            me->CastSpell(x, y, z, SPELL_LIGHTNING_CLOUDS_SUMMON, true);
                            me->CastSpell(x, y, z, SPELL_LIGHTNING_CLOUDS_SUMMON_VISUAL, true);
                            for (uint32 i = 0; i < 5; ++i)
                                me->CastSpell(x + ((float)rand_norm() * 10 - 5), y + ((float)rand_norm() * 10 - 5), z + ((float)rand_norm() * 10 - 5), SPELL_LIGHTNING_CLOUDS_SUMMON_VISUAL, true);
                        }
                    }
                }
                else if (spell->Id == SPELL_WIND_BURST && !windBurstSaid)
                {
                    windBurstSaid = true;
                    Talk(SAY_WIND_BURST);
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);

                switch (summon->GetEntry())
                {
                    case NPC_STORMLING_PRE_EFFECT:
                        summon->SetReactState(REACT_PASSIVE);
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        summon->CastSpell(summon, SPELL_STORMLING_SUMMON_VISUAL, true);
                        break;
                    case NPC_ICE_STORM_INITIAL:
                        summon->SetReactState(REACT_PASSIVE);
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        summon->CastSpell(summon, SPELL_ICE_STORM_PERIODIC_SUMMON, true);
                        break;
                    case NPC_LIGHTNING_CLOUD:
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        summon->CastSpell(summon, SPELL_LIGHTNING_CLOUDS_DAMAGE_AURA);
                        summon->DespawnOrUnsummon(30000);
                        break;
                    case NPC_LIGHTNING_CLOUD_VISUAL:
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        summon->DespawnOrUnsummon(30000);
                        break;
                    default:
                        break;
                }
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                if (summon->GetEntry() == NPC_LIGHTNING_CLOUD)
                {
                    for (std::map<float, uint32>::iterator itr = lightningCloudsAltitudes.begin(); itr != lightningCloudsAltitudes.end(); ++itr)
                    {
                        if (abs(itr->first - summon->GetPositionZ()) < 0.1f)
                        {
                            lightningCloudsAltitudes.erase(itr);
                            break;
                        }
                    }
                }
            }

            void SetPhase(uint8 phase)
            {
                // a Squall Line that is not scheduled starts as at the pull
                uint32 nextSquallLine = events.GetNextEventTime(EVENT_SQUALL_LINE);
                uint32 timeToSquallLine = nextSquallLine ? std::max<int32>(int32(nextSquallLine) - int32(events.GetTimer()), 1) : 12000;

                events.Reset();
                events.SetPhase(phase);
                _phase = phase;

                switch (_phase)
                {
                    case PHASE_ONE:
                        events.ScheduleEvent(EVENT_WIND_BURST,          20000,  0, _phase);
                        events.ScheduleEvent(EVENT_SQUALL_LINE,         12000,  0, _phase);
                        events.ScheduleEvent(EVENT_ICE_STORM,           6000,   0, _phase);
                        events.ScheduleEvent(EVENT_LIGHTNING_STRIKE,    8000,   0, _phase);
                        events.ScheduleEvent(EVENT_ELECTROCUTE,         5000,   0, _phase);
                        if (IsHeroic())
                            events.ScheduleEvent(EVENT_STATIC_SHOCK,    5000,   0, _phase);
                        break;
                    case PHASE_TWO:
                        events.ScheduleEvent(EVENT_SQUALL_LINE,         timeToSquallLine, 0, _phase);
                        events.ScheduleEvent(EVENT_ELECTROCUTE,         1000,   0, _phase);
                        events.ScheduleEvent(EVENT_SUMMON_STORMLING,    10000,  0, _phase);
                        break;
                    case PHASE_THREE:
                        events.ScheduleEvent(EVENT_LIGHTNING,           25000,  0, _phase);
                        events.ScheduleEvent(EVENT_WIND_BURST_START,    5000,   0, _phase);
                        events.ScheduleEvent(EVENT_ENTER_VEHICLES,      500,    0, _phase);
                        events.ScheduleEvent(EVENT_DESTROY_PLATFORM,    1500,   0, _phase);
                        events.ScheduleEvent(EVENT_EXIT_VEHICLES,       7000,   0, _phase);
                        break;
                    default:
                        break;
                }
            }

            void StartPhaseTwo()
            {
                SetPhase(PHASE_TWO);
                SetZoneWeather(me->GetZoneId(), WEATHER_STATE_MEDIUM_RAIN, 0.5f);
                SetZoneOverrideLight(me->GetZoneId(), DARK, 5000);

                me->InterruptNonMeleeSpells(true);
                DoCast(me, SPELL_ACID_RAIN);

                Talk(SAY_PHASE_2);
            }

            void StartPhaseThree()
            {
                SetPhase(PHASE_THREE);
                SetZoneWeather(me->GetZoneId(), WEATHER_STATE_FINE, 0.5f);
                SetZoneOverrideLight(me->GetZoneId(), DARK, 5000);

                me->InterruptNonMeleeSpells(true);
                me->RemoveAurasDueToSpell(SPELL_ACID_RAIN);
                DoCast(me, SPELL_ACID_RAIN_REMOVE);
                DoCast(me, SPELL_RELENTLESS_STORM_TRIGGER);
                DoCast(me, SPELL_RELENTLESS_STORM);
                DoCast(me, SPELL_LIGHTING_ROD_AURA);
                DoCast(me, SPELL_LIGHTNING_CLOUDS_AURA);
                DoCast(me, SPELL_LIGHTNING_CLOUDS_DAMAGE_AURA);

                Talk(SAY_PHASE_3);

                me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 175);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 175);

                RemoveSummons(true);

                me->SummonCreature(NPC_RELENTLESS_STORM, *me);

                if (GameObject* floor = me->FindNearestGameObject(GO_WIND_DRAFTEFFECT_CENTER, 500))
                    floor->SetGoState(GOState::GO_STATE_ACTIVE);

                Map::PlayerList const& players = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->GetSource())
                    {
                        if (player->IsAlive())
                        {
                            float x, y, z, o;
                            player->GetPosition(x, y, z, o);
                            player->SetCanFly(true);
                            player->NearTeleportTo(x, y, z + 5, o);
                        }
                    }
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                Talk(SAY_AGGRO);

                SetPhase(PHASE_ONE);
                persistentEvents.Reset();
                persistentEvents.ScheduleEvent(EVENT_BERSERK, 10 * MINUTE * IN_MILLISECONDS);

                DoCast(me, SPELL_SQUALL_LINE_SUMMON_SW, true);
                DoCast(me, SPELL_SQUALL_LINE_SUMMON_SE, true);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            }

            void EnterEvadeMode() OVERRIDE
            {
                _EnterEvadeMode();
                Reset();

                RemoveSummons();
                if (Creature* trigger = me->FindNearestCreature(NPC_LIGHTNING_STRIKE_TRIGGER, 100))
                    trigger->AI()->SetGUID(0);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                RemoveSummons();

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Talk(SAY_DEFEAT);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                instance->DoRespawnGameObject(instance->GetData64(RAID_MODE(DATA_HEART_OF_WIND_10N, DATA_HEART_OF_WIND_25N, DATA_HEART_OF_WIND_10H, DATA_HEART_OF_WIND_25H,
                    DATA_HEART_OF_WIND_10N, DATA_HEART_OF_WIND_25N)), DAY);
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (attacker && attacker->GetExactDist2d(me) >= 150)
                {
                    damage = 0;
                    return;
                }

                if (_phase == PHASE_ONE && me->HealthBelowPctDamaged(80, damage))
                    StartPhaseTwo();
                if (_phase == PHASE_TWO && me->HealthBelowPctDamaged(25, damage))
                    StartPhaseThree();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                persistentEvents.Update(diff);
                while (uint32 eventId = persistentEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK);
                            Talk(SAY_BERSERK);
                            break;
                        case EVENT_CHECK_POSITION:
                        {
                            bool found = false;
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            {
                                if (Player* player = itr->GetSource())
                                {
                                    if (player->IsAlive())
                                    {
                                        if (player->GetExactDist2d(me) >= 150)
                                        {
                                            if (player->HasAura(SPELL_EYE_OF_THE_STORM))
                                                player->RemoveAurasDueToSpell(SPELL_EYE_OF_THE_STORM);
                                        }
                                        else
                                            found = true;
                                    }
                                }
                            }

                            persistentEvents.ScheduleEvent(EVENT_CHECK_POSITION, 1000);
                            if (found)
                                persistentEvents.CancelEvent(EVENT_EVADE);
                            else if (!persistentEvents.GetNextEventTime(EVENT_EVADE))
                                persistentEvents.ScheduleEvent(EVENT_EVADE, 10000);
                            break;
                        }
                        case EVENT_EVADE:
                            EnterEvadeMode();
                            return;
                        default:
                            break;
                    }
                }

                events.Update(diff);

                if (Spell* spell = me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                    if (spell->GetSpellInfo()->Id == SPELL_ELECTROCUTE)
                        if (me->GetVictim() && me->IsWithinMeleeRange(me->GetVictim()))
                            me->InterruptSpell(CURRENT_CHANNELED_SPELL);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                {
                    events.DelayEvents(diff);
                    return;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WIND_BURST:
                            InterruptElectrocute();
                            windBurstSaid = false;
                            DoCast(me, SPELL_WIND_BURST);
                            events.ScheduleEvent(EVENT_WIND_BURST, 30000, 0, PHASE_ONE);
                            break;
                        case EVENT_SQUALL_LINE:
                        {
                            Creature* triggerSW = me->FindNearestCreature(NPC_SQUALL_LINE_VEHICLE_SW, 100);
                            Creature* triggerSE = me->FindNearestCreature(NPC_SQUALL_LINE_VEHICLE_SE, 100);
                            Creature* active = squallLineCount % 2 ? triggerSE : triggerSW;
                            Creature* inactive = squallLineCount % 2 ? triggerSW : triggerSE;
                            active->CastSpell(active, SPELL_SQUALL_LINE_SAFEGUARD, true);
                            inactive->RemoveAurasDueToSpell(SPELL_SQUALL_LINE_SAFEGUARD);
                            DoCast(me, SPELL_SQUALL_LINE_ACTIVATE);
                            DoCast(me, SPELL_SQUALL_LINE_SCRIPT_EFFECT);
                            ++squallLineCount;
                            events.ScheduleEvent(EVENT_SQUALL_LINE, IsHeroic() ? 25000 : 40000);
                            break;
                        }
                        case EVENT_ICE_STORM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_ICE_STORM_INITIAL_SUMMON);
                            events.ScheduleEvent(EVENT_ICE_STORM, 25000);
                            break;
                        case EVENT_LIGHTNING_STRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            {
                                if (Creature* trigger = me->FindNearestCreature(NPC_LIGHTNING_STRIKE_TRIGGER, 100))
                                    trigger->AI()->SetGUID(target->GetGUID());
                                events.ScheduleEvent(EVENT_LIGHTNING_STRIKE, IsHeroic() ? 18000 : 8000, 0, PHASE_ONE);
                            }
                            else
                                events.ScheduleEvent(EVENT_LIGHTNING_STRIKE, 1000, 0, PHASE_ONE);
                            break;
                        case EVENT_ELECTROCUTE:
                            if (me->GetVictim() && !me->IsWithinMeleeRange(me->GetVictim()))
                                DoCastVictim(SPELL_ELECTROCUTE);
                            events.ScheduleEvent(EVENT_ELECTROCUTE, 1000);
                            break;
                        case EVENT_STATIC_SHOCK:
                            DoCast(me, SPELL_STATIC_SHOCK);
                            events.ScheduleEvent(eventId, urand(5000, 10000));
                            break;
                        case EVENT_SUMMON_STORMLING:
                        {
                            std::list<Player*> targets;
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                if (Player* player = itr->GetSource())
                                    if (!player->HasAura(SPELL_SQUALL_LINE_SAFEGUARD))
                                        targets.push_back(player);
                            if (!targets.empty())
                                DoCast(Skyfire::Containers::SelectRandomContainerElement(targets), SPELL_STORMLING_SUMMON);
                            Talk(SAY_STORMLING);
                            events.ScheduleEvent(EVENT_SUMMON_STORMLING, 20000, 0, PHASE_TWO);
                            break;
                        }
                        case EVENT_LIGHTNING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_LIGHTNING);
                            events.ScheduleEvent(EVENT_LIGHTNING, 20000, 0, PHASE_THREE);
                            break;
                        case EVENT_WIND_BURST_START:
                            DoCast(me, SPELL_WIND_BURST_AURA);
                            break;
                        case EVENT_ENTER_VEHICLES:
                        {
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            {
                                if (Player* player = itr->GetSource())
                                {
                                    if (player->IsAlive())
                                    {
                                        if (Creature* vehicle = me->SummonCreature(NPC_RELENTLESS_STORM_VEHICLE, -46.7309f, 815.668f, 214.106f, (float)rand_norm() * M_PI))
                                        {
                                            player->SetDisableGravity(true, true);

                                            player->CastSpell(player, SPELL_RELENTLESS_STORM_VEHICLE, true);

                                            float x, y, z, o;
                                            me->GetPosition(x, y, z, o);
                                            vehicle->SetSpeed(MOVE_WALK, 3, true);
                                            vehicle->SetSpeed(MOVE_RUN, 3, true);
                                            vehicle->SetSpeed(MOVE_FLIGHT, 3, true);
                                            vehicle->GetMotionMaster()->MovePoint(1, x, y, 320);

                                            player->SetDisableGravity(false, true);
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        case EVENT_DESTROY_PLATFORM:
                            if (GameObject* floor = me->FindNearestGameObject(GO_FLOOR_ALAKIR, 500))
                                floor->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                            break;
                        case EVENT_EXIT_VEHICLES:
                        {
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            {
                                if (Player* player = itr->GetSource())
                                {
                                    if (player->IsAlive())
                                    {
                                        Unit* vehicle = player->GetVehicleBase();
                                        player->ExitVehicle();
                                        if (vehicle && vehicle->ToCreature())
                                            vehicle->ToCreature()->DespawnOrUnsummon();
                                        DoCast(player, SPELL_EYE_OF_THE_STORM_INITIAL, true);
                                        player->CastSpell(-120, 830, 320, SPELL_RELENTLESS_STORM_TELEPORT, true);
                                    }
                                }
                            }
                            persistentEvents.ScheduleEvent(EVENT_CHECK_POSITION, 1000);
                            break;
                        }
                        default:
                            break;
                    }
                }

                if (_phase < PHASE_THREE)
                    DoMeleeAttackIfReady();
            }

            bool ShouldLightningCloudsTarget(WorldObject const* target) const
            {
                for (std::map<float, uint32>::const_iterator itr = lightningCloudsAltitudes.begin(); itr != lightningCloudsAltitudes.end(); ++itr)
                    if (abs(itr->first - target->GetPositionZ()) < 10)
                        if (getMSTime() >= itr->second)
                            return true;
                return false;
            }

        private:
            EventMap persistentEvents;
            std::map<float, uint32> lightningCloudsAltitudes;
            uint32 squallLineCount;
            bool windBurstSaid;
            uint8 _phase;

            void SendPacketToPlayers(WorldPacket const* data) const
            {
                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                if (!players.isEmpty())
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        if (Player* player = itr->GetSource())
                            if (player->GetAreaId() == AREA_TO4W)
                                player->GetSession()->SendPacket(data);
            }

            void InterruptElectrocute()
            {
                if (Spell* spell = me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                    if (spell->GetSpellInfo()->Id == SPELL_ELECTROCUTE)
                        me->InterruptSpell(CURRENT_CHANNELED_SPELL);
            }

            void RemoveSummons(bool phaseThree = false)
            {
                std::list<Creature*> spawns;
                GetCreatureListWithEntryInGrid(spawns, me, NPC_SQUALL_LINE_VEHICLE_SW, 500);
                GetCreatureListWithEntryInGrid(spawns, me, NPC_SQUALL_LINE_VEHICLE_SE, 500);
                GetCreatureListWithEntryInGrid(spawns, me, NPC_SQUALL_LINE_SW, 500);
                GetCreatureListWithEntryInGrid(spawns, me, NPC_SQUALL_LINE_SE, 500);
                GetCreatureListWithEntryInGrid(spawns, me, NPC_LIGHTNING_CLOUD, 500);
                GetCreatureListWithEntryInGrid(spawns, me, NPC_LIGHTNING_CLOUD_VISUAL, 500);
                GetCreatureListWithEntryInGrid(spawns, me, NPC_RELENTLESS_STORM_VEHICLE, 500);
                GetCreatureListWithEntryInGrid(spawns, me, NPC_RELENTLESS_STORM, 500);
                if (!phaseThree)
                {
                    GetCreatureListWithEntryInGrid(spawns, me, NPC_STORMLING_PRE_EFFECT, 500);
                    GetCreatureListWithEntryInGrid(spawns, me, NPC_STORMLING, 500);
                }
                for (std::list<Creature*>::const_iterator itr = spawns.begin(); itr != spawns.end(); ++itr)
                    if (Creature* spawn = *itr)
                        spawn->DespawnOrUnsummon();
            }

            void SetZoneWeather(uint32 zoneId, uint32 weatherId, float weatherGrade)
            {
                WorldPacket data(SMSG_WEATHER, 4 + 4 + 1);
                data << uint32(weatherId);
                data << float(weatherGrade);
                data.WriteBit(false);
                data.FlushBits();

                Map::PlayerList const& players = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->GetSource())
                        if (player->GetZoneId() == zoneId)
                            player->GetSession()->SendPacket(&data);
            }

            // The sky darkening of the last phase needs SMSG_OVERRIDE_LIGHT, which this core does not build yet.
            void SetZoneOverrideLight(uint32 /*zoneId*/, uint32 /*lightId*/, uint32 /*fadeInTime*/) { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_alakirAI(creature);
        }
};

class npc_lightning_strike_trigger : public CreatureScript
{
    public:
        npc_lightning_strike_trigger() : CreatureScript("npc_lightning_strike_trigger") { }

        struct npc_lightning_strike_triggerAI : public ScriptedAI
        {
            npc_lightning_strike_triggerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                lightningStrikeTarget = 0;
                lightningStrikeCount = 0;
            }

            void SetGUID(uint64 guid, int32 /*type*/) OVERRIDE
            {
                if (guid == 0)
                {
                    events.Reset();
                    return;
                }

                if (Unit* target = ObjectAccessor::GetUnit(*me, guid))
                {
                    lightningStrikeTarget = guid;
                    lightningStrikeCount = 0;
                    me->SetFacingToObject(target);
                    events.ScheduleEvent(EVENT_LIGHTNING_STRIKE, 1);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LIGHTNING_STRIKE:
                        {
                            if (Unit* target = ObjectAccessor::GetUnit(*me, lightningStrikeTarget))
                            {
                                // Main target
                                me->SetFacingToObject(target);
                                float targetAngle = me->GetAngle(target);
                                DoCast(target, SPELL_LIGHTNING_STRIKE);

                                // Other targets
                                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                    if (Player* player = itr->GetSource())
                                        if (player != target && me->HasInArc(M_PI / 3, player))
                                            DoCast(player, SPELL_LIGHTNING_STRIKE_OTHERS);

                                // Visuals
                                float fromAngle = targetAngle - M_PI / 6;
                                float const dist = 66;
                                float x, y, z;
                                me->GetPosition(x, y, z);
                                for (uint32 i = 0; i < 12; ++i)
                                {
                                    float angle = fromAngle + (M_PI / 3) * i / 12;
                                    me->CastSpell(x + cos(angle) * dist, y + sin(angle) * dist, z, SPELL_LIGHTNING_STRIKE_VISUAL, false);
                                }
                            }

                            // Repeat setup
                            if (IsHeroic() && ++lightningStrikeCount < 16)
                                events.ScheduleEvent(EVENT_LIGHTNING_STRIKE, 1000);
                            else
                            {
                                lightningStrikeTarget = 0;
                                lightningStrikeCount = 0;
                                events.CancelEvent(EVENT_LIGHTNING_STRIKE);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            uint64 lightningStrikeTarget;
            uint32 lightningStrikeCount;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_lightning_strike_triggerAI(creature);
        }
};

class npc_stormling : public CreatureScript
{
    public:
        npc_stormling() : CreatureScript("npc_stormling") { }

        struct npc_stormlingAI : public ScriptedAI
        {
            npc_stormlingAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                DoCast(me, SPELL_STORMLING_AURA_VISUAL, true);
                DoCast(me, SPELL_STORMLING_AURA, true);
                DoZoneInCombat();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->CastSpell(me, SPELL_FEEDBACK, true, NULL, NULL, me->GetInstanceScript()->GetData64(DATA_ALAKIR));
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_stormlingAI(creature);
        }
};

class npc_squall_line_vehicle : public CreatureScript
{
    public:
        npc_squall_line_vehicle() :  CreatureScript("npc_squall_line_vehicle") { }

        struct npc_squall_line_vehicleAI : public ScriptedAI
        {
            npc_squall_line_vehicleAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                float angle = summoner->GetAngle(me);
                for (uint32 i = 0; i < 8; ++i)
                {
                    float x, y, z;
                    summoner->GetPosition(x, y, z);
                    x += (27 + i * 5.5f) * cos(angle);
                    y += (27 + i * 5.5f) * sin(angle);
                    me->SummonCreature(me->GetEntry() == NPC_SQUALL_LINE_VEHICLE_SW ? NPC_SQUALL_LINE_SW : NPC_SQUALL_LINE_SE, x, y, z, angle);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_squall_line_vehicleAI(creature);
        }
};

class npc_squall_line : public CreatureScript
{
    public:
        npc_squall_line() :  CreatureScript("npc_squall_line") { }

        struct npc_squall_lineAI : public ScriptedAI
        {
            npc_squall_lineAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                // players must not stop or slow the line; its damage taken is already zeroed below
                for (uint32 i = MECHANIC_NONE + 1; i < MAX_MECHANIC; ++i)
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, i, true);
                if (Creature* alakir = me->FindNearestCreature(BOSS_ALAKIR, 100))
                {
                    float dist = alakir->GetExactDist2d(me);
                    float startAngle = alakir->GetAngle(me);
                    float length = 2 * M_PI * dist;
                    float const duration = 30;

                    Movement::PointsArray points;

                    int32 const steps = 36;
                    for (int32 step = 0; step < steps; ++step)
                    {
                        float x, y, z;
                        GetPositionAround(alakir, dist, startAngle, me->GetEntry() == NPC_SQUALL_LINE_SW ? step : -step, steps, x, y, z);
                        points.push_back(G3D::Vector3(x, y, z));
                    }

                    Movement::MoveSplineInit init(me);
                    init.SetCyclic();
                    init.MovebyPath(points);
                    init.SetSmooth();
                    init.SetVelocity(length / duration);
                    init.Launch();
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                damage = 0;
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_squall_lineAI(creature);
        }
};

class npc_ice_storm_initial : public CreatureScript
{
    public:
        npc_ice_storm_initial() :  CreatureScript("npc_ice_storm_initial") { }

        struct npc_ice_storm_initialAI : public ScriptedAI
        {
            npc_ice_storm_initialAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                bool dir = urand(0, 1);
                me->SetSpeed(MOVE_RUN, 0.75f, true);
                me->SetReactState(REACT_PASSIVE);
                if (Creature* alakir = me->FindNearestCreature(BOSS_ALAKIR, 100))
                {
                    float dist = alakir->GetExactDist2d(me);
                    float startAngle = alakir->GetAngle(me);
                    float length = 2 * M_PI * dist;
                    float const duration = 30;

                    Movement::PointsArray points;

                    int32 const steps = 6;
                    for (int32 step = 0; step < steps; ++step)
                    {
                        float x, y, z;
                        GetPositionAround(alakir, dist, startAngle, dir ? step : -step, steps, x, y, z);
                        points.push_back(G3D::Vector3(x, y, z));
                    }

                    Movement::MoveSplineInit init(me);
                    init.MovebyPath(points);
                    init.Launch();
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!me->HasAura(SPELL_ICE_STORM_PERIODIC_SUMMON))
                    me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_ice_storm_initialAI(creature);
        }
};

class npc_ice_storm_ground : public CreatureScript
{
    public:
        npc_ice_storm_ground() :  CreatureScript("npc_ice_storm_ground") { }

        struct npc_ice_storm_groundAI : public ScriptedAI
        {
            npc_ice_storm_groundAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                me->DespawnOrUnsummon(15000);
                DoCast(me, SPELL_ICE_STORM_SPAWN_VISUAL, true);
                DoCast(me, SPELL_ICE_STORM_AURA);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_ice_storm_groundAI(creature);
        }
};

class npc_lightning_clouds : public CreatureScript
{
    public:
        npc_lightning_clouds() :  CreatureScript("npc_lightning_clouds") { }

        struct npc_lightning_cloudsAI : public ScriptedAI
        {
            npc_lightning_cloudsAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                timerVisualCast = 5500;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (timerVisualCast <= diff)
                {
                    std::list<Creature*> clouds;
                    GetCreatureListWithEntryInGrid(clouds, me, NPC_LIGHTNING_CLOUD, 60);
                    clouds.remove_if(TargetCheck(me));
                    if (!clouds.empty())
                    {
                        float x, y, z;
                        Skyfire::Containers::SelectRandomContainerElement(clouds)->GetPosition(x, y, z);
                        me->CastSpell(x, y, z, SPELL_LIGHTNING_CLOUDS_VISUAL, false);
                    }
                    timerVisualCast = 500;
                }
                else
                    timerVisualCast -= diff;
            }

        private:
            uint32 timerVisualCast;

            class TargetCheck
            {
                public:
                    TargetCheck(Position* pos) : _pos(pos) { }
                    bool operator()(WorldObject* obj) const { return abs(_pos->GetPositionZ() - obj->GetPositionZ()) > 10; }
                private:
                    Position* _pos;
            };
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_lightning_cloudsAI(creature);
        }
};

class spell_squall_line_activate : public SpellScriptLoader
{
    public:
        spell_squall_line_activate() : SpellScriptLoader("spell_squall_line_activate") { }

        class spell_squall_line_activate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_squall_line_activate_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Unit* alakir = caster->ToTempSummon() ? caster->ToTempSummon()->GetSummoner() : NULL;
                if (!alakir)
                    return;

                targets.remove_if(SummonerCheck(caster));
                if (targets.size() != 8)
                    return;

                targets.sort(Skyfire::ObjectDistanceOrderPred(alakir));
                uint32 pos = urand(0, 6);
                std::list<WorldObject*>::iterator itr = targets.begin();
                std::advance(itr, pos);
                targets.erase(itr);
                itr = targets.begin();
                std::advance(itr, pos);
                targets.erase(itr);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_squall_line_activate_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }

            class SummonerCheck
            {
                public:
                    SummonerCheck(Unit* summoner) : _summoner(summoner) { }

                    bool operator()(WorldObject* obj) const
                    {
                        if (Unit* unit = obj->ToUnit())
                            if (TempSummon* summon = unit->ToTempSummon())
                                if (summon->GetSummoner() == _summoner)
                                    return false;

                        return true;
                    }

                private:
                    Unit* _summoner;
            };
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_squall_line_activate_SpellScript();
        }
};

class spell_squall_line : public SpellScriptLoader
{
    public:
        spell_squall_line() : SpellScriptLoader("spell_squall_line") { }

        class spell_squall_line_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_squall_line_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (Player* target = GetHitPlayer())
                {
                    if (!target->HasAura(SPELL_SQUALL_LINE_SAFEGUARD))
                    {
                        target->CastSpell(target, SPELL_SQUALL_LINE_SAFEGUARD, true);
                        target->CastSpell(GetCaster(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
                    }
                }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_squall_line_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_squall_line_SpellScript();
        }
};

class spell_wind_burst : public SpellScriptLoader
{
    public:
        spell_wind_burst() : SpellScriptLoader("spell_wind_burst") { }

        class spell_wind_burst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_wind_burst_SpellScript);

            void HandleEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->GetVehicle())
                        target->ExitVehicle();
                    target->RemoveAurasDueToSpell(SPELL_EYE_OF_THE_STORM);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_wind_burst_SpellScript::HandleEffect, EFFECT_1, SPELL_EFFECT_KNOCK_BACK);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_wind_burst_SpellScript();
        }
};

class spell_lightning_rod : public SpellScriptLoader
{
    public:
        spell_lightning_rod() : SpellScriptLoader("spell_lightning_rod") { }

        class spell_lightning_rod_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lightning_rod_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (GetCaster())
                    targets.remove_if(TargetCheck(GetCaster()));
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_lightning_rod_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }

            class TargetCheck
            {
                public:
                    TargetCheck(Position* pos) : _pos(pos) { }
                    bool operator()(WorldObject* obj) const { return abs(_pos->GetPositionZ() - obj->GetPositionZ()) > 5; }
                private:
                    Position* _pos;
            };
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_lightning_rod_SpellScript();
        }
};

class spell_lightning_clouds : public SpellScriptLoader
{
    public:
        spell_lightning_clouds() : SpellScriptLoader("spell_lightning_clouds") { }

        class spell_lightning_clouds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lightning_clouds_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (Unit* caster = GetCaster())
                    if (Creature* alakir = caster->ToCreature())
                        if (boss_alakir::boss_alakirAI* ai = CAST_AI(boss_alakir::boss_alakirAI, alakir->AI()))
                            targets.remove_if(TargetCheck(ai));
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_lightning_clouds_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }

            class TargetCheck
            {
                public:
                    TargetCheck(boss_alakir::boss_alakirAI* ai) : _ai(ai) { }
                    bool operator()(WorldObject* obj) const { return !_ai->ShouldLightningCloudsTarget(obj); }
                private:
                    boss_alakir::boss_alakirAI* _ai;
            };
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_lightning_clouds_SpellScript();
        }
};

void AddSC_boss_alakir()
{
    new boss_alakir();
    new npc_lightning_strike_trigger();
    new npc_stormling();
    new npc_squall_line_vehicle();
    new npc_squall_line();
    new npc_ice_storm_initial();
    new npc_ice_storm_ground();
    new npc_lightning_clouds();
    new spell_squall_line_activate();
    new spell_squall_line();
    new spell_wind_burst();
    new spell_lightning_rod();
    new spell_lightning_clouds();
}