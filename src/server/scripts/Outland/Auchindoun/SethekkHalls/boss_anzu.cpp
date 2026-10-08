/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "sethekk_halls.h"
#include "SpellMgr.h"

#include <set>

enum Says
{
    SAY_SUMMON_BROOD            = 0,
    SAY_SPELL_BOMB              = 1
};

enum Spells
{
    SPELL_PARALYZING_SCREECH    = 40184,
    SPELL_SPELL_BOMB            = 40303,
    SPELL_CYCLONE_OF_FEATHERS   = 40321,
    SPELL_BANISH_SELF           = 42354,
    SPELL_FLESH_RIP             = 40199,
    SPELL_DIVE                 = 40279,
    SPELL_BROOD_SCREECH         = 31273
};

enum Events
{
    EVENT_PARALYZING_SCREECH    = 1,
    EVENT_SPELL_BOMB            = 2,
    EVENT_CYCLONE_OF_FEATHERS   = 3,
    EVENT_FLESH_RIP             = 4,
    EVENT_DIVE                 = 5
};

Position const PosSummonBrood[7] =
{
    { -118.1717f, 284.5299f, 121.2287f, 2.775074f },
    { -98.15528f, 293.4469f, 109.2385f, 0.174533f },
    { -99.70160f, 270.1699f, 98.27389f, 6.178465f },
    { -69.25543f, 303.0768f, 97.84479f, 5.532694f },
    { -87.59662f, 263.5181f, 92.70478f, 1.658063f },
    { -73.54323f, 276.6267f, 94.25807f, 2.802979f },
    { -81.70527f, 280.8776f, 44.58830f, 0.526849f }
};

class boss_anzu : public CreatureScript
{
public:
    boss_anzu() : CreatureScript("boss_anzu") { }

    struct boss_anzuAI : public BossAI
    {
        boss_anzuAI(Creature* creature) : BossAI(creature, DATA_ANZU),
            _thresholds(0), _pendingWaves(0), _waveActive(false) { }

        void Reset() OVERRIDE
        {
            _thresholds = 0;
            _pendingWaves = 0;
            _waveActive = false;
            _brood.clear();
            me->RemoveAurasDueToSpell(SPELL_BANISH_SELF);
            _Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_PARALYZING_SCREECH, 14000);
            events.ScheduleEvent(EVENT_CYCLONE_OF_FEATHERS, 5000);
            // Provisional cadence for the missing abilities; the 18414 journal
            // describes their effects but does not specify encounter cooldowns.
            events.ScheduleEvent(EVENT_FLESH_RIP, 10000);
            events.ScheduleEvent(EVENT_DIVE, 20000);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _waveActive = false;
            _pendingWaves = 0;
            _brood.clear();
            _JustDied();
        }

        void DamageTaken(Unit* /*killer*/, uint32& damage) OVERRIDE
        {
            // Do not clamp lethal damage or force unverified phase immunities.
            // Queue both crossed thresholds, but never overlap their waves.
            if (damage >= me->GetHealth())
                return;

            if (!(_thresholds & 1) && me->HealthBelowPctDamaged(75, damage))
            {
                _thresholds |= 1;
                ++_pendingWaves;
            }
            if (!(_thresholds & 2) && me->HealthBelowPctDamaged(35, damage))
            {
                _thresholds |= 2;
                ++_pendingWaves;
            }
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            if (summon->GetEntry() == NPC_BROOD_OF_ANZU)
                _brood.insert(summon->GetGUID());
            BossAI::JustSummoned(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) OVERRIDE
        {
            // GUID membership excludes surviving adds from a previous wave.
            // Despawning a live add is not equivalent to killing it.
            if (_brood.erase(summon->GetGUID()) && _brood.empty() && _waveActive)
                me->RemoveAurasDueToSpell(SPELL_BANISH_SELF);
        }

        void StartBroodWave()
        {
            me->InterruptNonMeleeSpells(false);
            DoCast(me, SPELL_BANISH_SELF, true);
            if (!me->HasAura(SPELL_BANISH_SELF))
                return; // Retry without consuming a threshold or creating adds.

            --_pendingWaves;
            _waveActive = true;
            _brood.clear();
            Talk(SAY_SUMMON_BROOD);
            // TODO: Verify the legacy aerial spawn positions and brood pathing in-game.
            for (uint8 i = 0; i < 7; ++i)
                me->SummonCreature(NPC_BROOD_OF_ANZU, PosSummonBrood[i], TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 46000);

            if (_brood.empty())
                me->RemoveAurasDueToSpell(SPELL_BANISH_SELF);
            events.RescheduleEvent(EVENT_SPELL_BOMB, 12000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);
            if (_waveActive && !me->HasAura(SPELL_BANISH_SELF))
            {
                _waveActive = false;
                _brood.clear();
            }
            if (!_waveActive && _pendingWaves)
                StartBroodWave();

            // Banish pacifies and roots Anzu; it must not stop his spell events.
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_PARALYZING_SCREECH:
                        DoCastVictim(SPELL_PARALYZING_SCREECH);
                        events.ScheduleEvent(EVENT_PARALYZING_SCREECH, 26000);
                        break;
                    case EVENT_CYCLONE_OF_FEATHERS:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            DoCast(target, SPELL_CYCLONE_OF_FEATHERS);
                        events.ScheduleEvent(EVENT_CYCLONE_OF_FEATHERS, 21000);
                        break;
                    case EVENT_SPELL_BOMB:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, [](Unit* unit)
                            { return unit && unit->IsAlive() && unit->GetTypeId() == TypeID::TYPEID_PLAYER && unit->getPowerType() == POWER_MANA; }))
                        {
                            DoCast(target, SPELL_SPELL_BOMB);
                            Talk(SAY_SPELL_BOMB, target);
                        }
                        break;
                    case EVENT_FLESH_RIP:
                        if (!me->HasAura(SPELL_BANISH_SELF) && me->IsWithinMeleeRange(me->GetVictim()))
                        {
                            DoCastVictim(SPELL_FLESH_RIP);
                            events.ScheduleEvent(EVENT_FLESH_RIP, 15000);
                        }
                        else
                            events.ScheduleEvent(EVENT_FLESH_RIP, 1000);
                        break;
                    case EVENT_DIVE:
                    {
                        SpellInfo const* spell = sSpellMgr->GetSpellInfo(SPELL_DIVE);
                        Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, [this, spell](Unit* unit)
                        {
                            return spell && unit && unit->IsAlive() && unit->GetTypeId() == TypeID::TYPEID_PLAYER &&
                                !me->IsWithinMeleeRange(unit) && me->IsWithinDistInMap(unit, spell->GetMaxRange(false, me)) &&
                                me->IsWithinLOSInMap(unit);
                        });
                        if (!me->HasAura(SPELL_BANISH_SELF) && target)
                        {
                            DoCast(target, SPELL_DIVE);
                            events.ScheduleEvent(EVENT_DIVE, 30000);
                        }
                        else
                            events.ScheduleEvent(EVENT_DIVE, 1000);
                        break;
                    }
                    default:
                        break;
                }
                // Keep other due casts queued until the current cast completes.
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    break;
            }
            DoMeleeAttackIfReady();
        }

    private:
        uint8 _thresholds;
        uint8 _pendingWaves;
        bool _waveActive;
        std::set<uint64> _brood;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetSethekkHallsAI<boss_anzuAI>(creature);
    }
};

class npc_brood_of_anzu : public CreatureScript
{
public:
    npc_brood_of_anzu() : CreatureScript("npc_brood_of_anzu") { }

    struct npc_brood_of_anzuAI : public ScriptedAI
    {
        npc_brood_of_anzuAI(Creature* creature) : ScriptedAI(creature) { }
        EventMap events;

        void Reset() OVERRIDE { events.Reset(); }
        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            // Provisional cadence; Screech's client duration is eight seconds.
            events.ScheduleEvent(1, 5000);
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;
            events.Update(diff);
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            if (events.ExecuteEvent())
            {
                DoCastAOE(SPELL_BROOD_SCREECH);
                events.ScheduleEvent(1, 10000);
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetSethekkHallsAI<npc_brood_of_anzuAI>(creature);
    }
};

void AddSC_boss_anzu()
{
    new boss_anzu();
    new npc_brood_of_anzu();
}
