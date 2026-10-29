/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
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

/*
 * Anub'arak submerges at 75%, 50% and 25% health. While he is underground
 * Impale keeps hitting the players and the adds of that phase are sent in
 * (assassins from the ledge, guardians and venomancers down the corridor, and
 * on the last phase darters as well). He emerges when every guardian and
 * venomancer of the phase is dead.
 *
 * The summon spells of the reference are serverside spells that need world
 * triggers picked by guid; the adds are summoned directly at the same places.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "azjol_nerub.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "PassiveAI.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

enum Spells
{
    SPELL_EMERGE                = 53500,
    SPELL_SUBMERGE              = 53421,
    SPELL_IMPALE_AURA           = 53456,
    SPELL_IMPALE_VISUAL         = 53455,
    SPELL_IMPALE_DAMAGE         = 53454,
    SPELL_IMPALE_DAMAGE_HEROIC  = 59446,
    SPELL_LEECHING_SWARM        = 53467,
    SPELL_POUND                 = 59433,
    SPELL_POUND_DAMAGE          = 53509,
    SPELL_POUND_DAMAGE_HEROIC   = 59432,
    SPELL_CARRION_BEETLES       = 53520,
    SPELL_CARRION_BEETLE        = 53521,

    SPELL_DART                  = 59349,
    SPELL_BACKSTAB              = 52540,
    SPELL_ASSASSIN_VISUAL       = 53611,
    SPELL_SUNDER_ARMOR          = 53618,
    SPELL_POISON_BOLT           = 53617
};

enum Creatures
{
    NPC_DARTER                  = 29213,
    NPC_ASSASSIN                = 29214,
    NPC_GUARDIAN                = 29216,
    NPC_VENOMANCER              = 29217
};

enum Yells
{
    SAY_AGGRO       = 0,
    SAY_SLAY        = 1,
    SAY_DEATH       = 2,
    SAY_LOCUST      = 3,
    SAY_SUBMERGE    = 4,
    SAY_INTRO       = 5
};

enum Events
{
    EVENT_POUND = 1,
    EVENT_IMPALE,
    EVENT_LEECHING_SWARM,
    EVENT_CARRION_BEETLES,
    EVENT_SUBMERGE, // use event for this so we don't submerge mid-cast
    EVENT_DARTER,
    EVENT_ASSASSIN,
    EVENT_GUARDIAN,
    EVENT_VENOMANCER,
    EVENT_CLOSE_DOOR,

    EVENT_PET_BACKSTAB,
    EVENT_PET_SUNDER_ARMOR,
    EVENT_PET_POISON_BOLT
};

enum Actions
{
    ACTION_PET_DIED = 1,
    ACTION_PET_EVADE
};

enum Misc
{
    ACHIEV_GOTTA_GO_START_EVENT = 20381,
    ARENA_RADIUS                = 26,
    ASSASSIN_JUMP_MIN_DISTANCE  = 10,
    ASSASSIN_JUMP_MAX_DISTANCE  = 30
};

enum Phases
{
    PHASE_EMERGE = 1,
    PHASE_SUBMERGE
};

enum GUIDTypes
{
    GUID_TYPE_PET = 0,
    GUID_TYPE_IMPALE
};

static const Position arenaCenter = { 550.6178f, 253.5917f, 224.0f };

// The ledge the assassins and darters drop in from (the places of the world triggers in the reference)
static const uint8 NUM_LEDGE_POSITIONS = 8;
static const Position ledgePositions[NUM_LEDGE_POSITIONS] =
{
    { 478.739f, 252.850f, 250.544f, 0.0523599f },
    { 620.704f, 224.562f, 250.232f, 2.53073f   },
    { 620.622f, 298.263f, 250.544f, 3.7001f    },
    { 478.291f, 224.827f, 250.235f, 0.401426f  },
    { 478.547f, 297.045f, 250.544f, 5.79449f   },
    { 478.149f, 269.009f, 250.544f, 6.12611f   },
    { 622.904f, 252.945f, 250.544f, 3.12414f   },
    { 621.319f, 268.482f, 250.544f, 3.33358f   }
};

// Where the guardians and venomancers enter the arena
static const Position corridorPosition = { 549.622f, 352.047f, 240.899f, 3.45575f };

static Creature* GetAnubarak(Creature* me)
{
    InstanceScript* instance = me->GetInstanceScript();
    if (!instance)
        return NULL;
    return Unit::GetCreature(*me, instance->GetData64(DATA_ANUBARAK));
}

struct boss_anub_arak : public BossAI
{
    boss_anub_arak(Creature* creature) : BossAI(creature, DATA_ANUBARAK), _nextSubmerge(0), _petCount(0), _assassinCount(0), _guardianCount(0), _venomancerCount(0), _impaleTarget(0) { }

    void Reset() OVERRIDE
    {
        BossAI::Reset();
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_GOTTA_GO_START_EVENT);
        _nextSubmerge = 75;
        _petCount = 0;
        _assassinCount = 0;
        _guardianCount = 0;
        _venomancerCount = 0;
        _impaleTarget = 0;
    }

    void SetWallsOpen(bool open)
    {
        instance->HandleGameObject(instance->GetData64(DATA_ANUBARAK_WALL), open);
        instance->HandleGameObject(instance->GetData64(DATA_ANUBARAK_WALL_2), open);
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        BossAI::EnterCombat(who);

        SetWallsOpen(true); // open the walls for now, they close again after a few seconds

        Talk(SAY_AGGRO);
        instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_GOTTA_GO_START_EVENT);

        events.SetPhase(PHASE_EMERGE);
        events.ScheduleEvent(EVENT_CLOSE_DOOR, 5 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_POUND, urand(2, 4) * IN_MILLISECONDS, 0, PHASE_EMERGE);
        events.ScheduleEvent(EVENT_LEECHING_SWARM, urand(5, 7) * IN_MILLISECONDS, 0, PHASE_EMERGE);
        events.ScheduleEvent(EVENT_CARRION_BEETLES, urand(14, 17) * IN_MILLISECONDS, 0, PHASE_EMERGE);
    }

    void EnterEvadeMode() OVERRIDE
    {
        summons.DespawnAll();
        _DespawnAtEvade();
    }

    void SummonAddAt(uint32 entry, Position const& position)
    {
        me->SummonCreature(entry, position, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN);
    }

    void SummonAtLedge(uint32 entry)
    {
        SummonAddAt(entry, ledgePositions[urand(0, NUM_LEDGE_POSITIONS - 1)]);
    }

    void SummonAtCorridor(uint32 entry)
    {
        Position position = corridorPosition;
        position.m_positionX += frand(-3.0f, 3.0f);
        position.m_positionY += frand(-3.0f, 3.0f);
        SummonAddAt(entry, position);
    }

    // Each event summons two adds; the counter keeps the number still to come
    bool SummonWave(uint32 entry, uint8& remaining, bool ledge)
    {
        for (uint8 i = 0; i < 2; ++i)
        {
            if (ledge)
                SummonAtLedge(entry);
            else
                SummonAtCorridor(entry);
        }

        if (remaining > 2)
        {
            remaining -= 2;
            return true;
        }

        remaining = 0;
        return false;
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
                case EVENT_CLOSE_DOOR:
                    SetWallsOpen(false);
                    break;
                case EVENT_POUND:
                    DoCastVictim(SPELL_POUND);
                    events.ScheduleEvent(EVENT_POUND, urand(26, 32) * IN_MILLISECONDS, 0, PHASE_EMERGE);
                    break;
                case EVENT_LEECHING_SWARM:
                    Talk(SAY_LOCUST);
                    DoCast(me, SPELL_LEECHING_SWARM);
                    events.ScheduleEvent(EVENT_LEECHING_SWARM, urand(25, 28) * IN_MILLISECONDS, 0, PHASE_EMERGE);
                    break;
                case EVENT_CARRION_BEETLES:
                    DoCast(me, SPELL_CARRION_BEETLES);
                    events.ScheduleEvent(EVENT_CARRION_BEETLES, urand(24, 27) * IN_MILLISECONDS, 0, PHASE_EMERGE);
                    break;
                case EVENT_IMPALE:
                    if (Creature* impaleTarget = Unit::GetCreature(*me, _impaleTarget))
                        DoCast(impaleTarget, DUNGEON_MODE(SPELL_IMPALE_DAMAGE, SPELL_IMPALE_DAMAGE_HEROIC), true);
                    break;
                case EVENT_SUBMERGE:
                    Talk(SAY_SUBMERGE);
                    DoCast(me, SPELL_SUBMERGE);
                    break;
                case EVENT_DARTER:
                    SummonAtLedge(NPC_DARTER);
                    events.ScheduleEvent(EVENT_DARTER, 11 * IN_MILLISECONDS, 0, PHASE_SUBMERGE);
                    break;
                case EVENT_ASSASSIN:
                    if (SummonWave(NPC_ASSASSIN, _assassinCount, true))
                        events.ScheduleEvent(EVENT_ASSASSIN, 20 * IN_MILLISECONDS, 0, PHASE_SUBMERGE);
                    break;
                case EVENT_GUARDIAN:
                    if (SummonWave(NPC_GUARDIAN, _guardianCount, false))
                        events.ScheduleEvent(EVENT_GUARDIAN, 20 * IN_MILLISECONDS, 0, PHASE_SUBMERGE);
                    break;
                case EVENT_VENOMANCER:
                    if (SummonWave(NPC_VENOMANCER, _venomancerCount, false))
                        events.ScheduleEvent(EVENT_VENOMANCER, 20 * IN_MILLISECONDS, 0, PHASE_SUBMERGE);
                    break;
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        _JustDied();
        Talk(SAY_DEATH);
    }

    void KilledUnit(Unit* victim) OVERRIDE
    {
        if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
            Talk(SAY_SLAY);
    }

    void SetGUID(uint64 guid, int32 id = 0) OVERRIDE
    {
        if (id == GUID_TYPE_IMPALE)
        {
            _impaleTarget = guid;
            events.ScheduleEvent(EVENT_IMPALE, 4 * IN_MILLISECONDS);
        }
    }

    void DoAction(int32 action) OVERRIDE
    {
        if (!me->IsAlive())
            return;

        switch (action)
        {
            case ACTION_PET_DIED:
                if (!_petCount) // underflow check - something has gone horribly wrong
                {
                    EnterEvadeMode();
                    return;
                }
                if (!--_petCount) // last pet died, emerge
                {
                    me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                    me->RemoveAurasDueToSpell(SPELL_IMPALE_AURA);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    DoCast(me, SPELL_EMERGE);
                    events.SetPhase(PHASE_EMERGE);
                    events.ScheduleEvent(EVENT_POUND, urand(13, 18) * IN_MILLISECONDS, 0, PHASE_EMERGE);
                    events.ScheduleEvent(EVENT_LEECHING_SWARM, urand(3, 7) * IN_MILLISECONDS, 0, PHASE_EMERGE);
                    events.ScheduleEvent(EVENT_CARRION_BEETLES, urand(10, 15) * IN_MILLISECONDS, 0, PHASE_EMERGE);
                }
                break;
            case ACTION_PET_EVADE:
                EnterEvadeMode();
                break;
        }
    }

    void DamageTaken(Unit* /*source*/, uint32& damage) OVERRIDE
    {
        if (me->HasAura(SPELL_SUBMERGE))
            damage = 0;
        else if (_nextSubmerge && me->HealthBelowPctDamaged(_nextSubmerge, damage))
        {
            events.RescheduleEvent(EVENT_SUBMERGE, 0, 0, PHASE_EMERGE);
            _nextSubmerge = _nextSubmerge - 25;
        }
    }

    void SpellHit(Unit* /*caster*/, SpellInfo const* spellInfo) OVERRIDE
    {
        if (spellInfo->Id == SPELL_SUBMERGE)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveAurasDueToSpell(SPELL_LEECHING_SWARM);
            DoCast(me, SPELL_IMPALE_AURA, true);

            events.SetPhase(PHASE_SUBMERGE);
            switch (_nextSubmerge)
            {
                case 50: // first submerge phase
                    _assassinCount = 4;
                    _guardianCount = 2;
                    _venomancerCount = 0;
                    break;
                case 25: // second submerge phase
                    _assassinCount = 6;
                    _guardianCount = 2;
                    _venomancerCount = 2;
                    break;
                case 0:  // third submerge phase
                    _assassinCount = 6;
                    _guardianCount = 2;
                    _venomancerCount = 2;
                    events.ScheduleEvent(EVENT_DARTER, 0, 0, PHASE_SUBMERGE);
                    break;
            }
            _petCount = _guardianCount + _venomancerCount;
            if (_assassinCount)
                events.ScheduleEvent(EVENT_ASSASSIN, 0, 0, PHASE_SUBMERGE);
            if (_guardianCount)
                events.ScheduleEvent(EVENT_GUARDIAN, 4 * IN_MILLISECONDS, 0, PHASE_SUBMERGE);
            if (_venomancerCount)
                events.ScheduleEvent(EVENT_VENOMANCER, 20 * IN_MILLISECONDS, 0, PHASE_SUBMERGE);
        }
    }

    private:
        uint32 _nextSubmerge;
        uint32 _petCount;
        uint8 _assassinCount;
        uint8 _guardianCount;
        uint8 _venomancerCount;
        uint64 _impaleTarget;
};

class npc_anubarak_pet_template : public ScriptedAI
{
    public:
        npc_anubarak_pet_template(Creature* creature, bool isLarge) : ScriptedAI(creature), _isLarge(isLarge) { }

        void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
        {
            if (!GetAnubarak(me))
                me->DespawnOrUnsummon();
        }

        void JustDied(Unit* killer) OVERRIDE
        {
            ScriptedAI::JustDied(killer);
            if (_isLarge)
                if (Creature* anubarak = GetAnubarak(me))
                    anubarak->AI()->DoAction(ACTION_PET_DIED);
        }

        void EnterEvadeMode() OVERRIDE
        {
            if (Creature* anubarak = GetAnubarak(me))
                anubarak->AI()->DoAction(ACTION_PET_EVADE);
            else
                me->DespawnOrUnsummon();
        }

        void Reset() OVERRIDE
        {
            _events.Reset();
        }

    protected:
        EventMap _events;

    private:
        bool const _isLarge;
};

struct npc_anubarak_anub_ar_darter : public npc_anubarak_pet_template
{
    npc_anubarak_anub_ar_darter(Creature* creature) : npc_anubarak_pet_template(creature, false) { }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        npc_anubarak_pet_template::IsSummonedBy(summoner);
        DoCastAOE(SPELL_DART);
    }
};

struct npc_anubarak_anub_ar_assassin : public npc_anubarak_pet_template
{
    npc_anubarak_anub_ar_assassin(Creature* creature) : npc_anubarak_pet_template(creature, false) { }

    Position GetRandomPositionAround(Creature* anubarak)
    {
        double angle = rand_norm() * 2.0 * M_PI;
        float distance = frand(float(ASSASSIN_JUMP_MIN_DISTANCE), float(ASSASSIN_JUMP_MAX_DISTANCE));
        Position position = { anubarak->GetPositionX() + distance * float(std::sin(angle)), anubarak->GetPositionY() + distance * float(std::cos(angle)), anubarak->GetPositionZ(), 0.0f };
        return position;
    }

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        npc_anubarak_pet_template::IsSummonedBy(summoner);
        if (Creature* anubarak = GetAnubarak(me))
        {
            // The jump has to land inside the arena
            Position jumpTo;
            uint8 tries = 0;
            do
            {
                jumpTo = GetRandomPositionAround(anubarak);
                ++tries;
            }
            while (jumpTo.GetExactDist2d(&arenaCenter) > float(ARENA_RADIUS) && tries < 20);

            if (tries == 20)
                jumpTo = arenaCenter;

            me->GetMotionMaster()->MoveJump(jumpTo, 40.0f, 40.0f);
            DoCast(me, SPELL_ASSASSIN_VISUAL, true);
        }
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        _events.ScheduleEvent(EVENT_PET_BACKSTAB, 6 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            if (eventId == EVENT_PET_BACKSTAB)
            {
                if (me->GetVictim() && me->GetVictim()->isInBack(me))
                    DoCastVictim(SPELL_BACKSTAB);
                _events.ScheduleEvent(EVENT_PET_BACKSTAB, 6 * IN_MILLISECONDS);
            }
        }

        DoMeleeAttackIfReady();
    }

    void MovementInform(uint32 /*type*/, uint32 id) OVERRIDE
    {
        if (id == EVENT_JUMP)
        {
            me->RemoveAurasDueToSpell(SPELL_ASSASSIN_VISUAL);
            DoZoneInCombat();
        }
    }
};

struct npc_anubarak_anub_ar_guardian : public npc_anubarak_pet_template
{
    npc_anubarak_anub_ar_guardian(Creature* creature) : npc_anubarak_pet_template(creature, true) { }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        _events.ScheduleEvent(EVENT_PET_SUNDER_ARMOR, 6 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            if (eventId == EVENT_PET_SUNDER_ARMOR)
            {
                DoCastVictim(SPELL_SUNDER_ARMOR);
                _events.ScheduleEvent(EVENT_PET_SUNDER_ARMOR, 12 * IN_MILLISECONDS);
            }
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_anubarak_anub_ar_venomancer : public npc_anubarak_pet_template
{
    npc_anubarak_anub_ar_venomancer(Creature* creature) : npc_anubarak_pet_template(creature, true) { }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        _events.ScheduleEvent(EVENT_PET_POISON_BOLT, 5 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            if (eventId == EVENT_PET_POISON_BOLT)
            {
                DoCastVictim(SPELL_POISON_BOLT);
                _events.ScheduleEvent(EVENT_PET_POISON_BOLT, urand(2, 3) * IN_MILLISECONDS);
            }
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_anubarak_impale_target : public NullCreatureAI
{
    npc_anubarak_impale_target(Creature* creature) : NullCreatureAI(creature) { }

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        if (Creature* anubarak = GetAnubarak(me))
        {
            me->CastSpell(me, SPELL_IMPALE_VISUAL, true);
            me->DespawnOrUnsummon(6 * IN_MILLISECONDS);
            anubarak->AI()->SetGUID(me->GetGUID(), GUID_TYPE_IMPALE);
        }
        else
            me->DespawnOrUnsummon();
    }
};

// 53472, 59433 - Pound
class spell_anubarak_pound : public AuraScript
{
    PrepareAuraScript(spell_anubarak_pound);

    bool Validate(SpellInfo const* /*spell*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_POUND_DAMAGE) && sSpellMgr->GetSpellInfo(SPELL_POUND_DAMAGE_HEROIC);
    }

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();
        if (!caster || !target)
            return;

        bool heroic = caster->GetMap()->IsHeroic();
        caster->CastSpell(target, heroic ? SPELL_POUND_DAMAGE_HEROIC : SPELL_POUND_DAMAGE, true);
    }

    void Register() OVERRIDE
    {
        AfterEffectApply += AuraEffectApplyFn(spell_anubarak_pound::AfterApply, EFFECT_2, SPELL_AURA_FLY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 53520 - Carrion Beetles
class spell_anubarak_carrion_beetles : public AuraScript
{
    PrepareAuraScript(spell_anubarak_carrion_beetles);

    bool Validate(SpellInfo const* /*spell*/) OVERRIDE
    {
        return sSpellMgr->GetSpellInfo(SPELL_CARRION_BEETLE) != NULL;
    }

    void HandlePeriodic(AuraEffect const* /*eff*/)
    {
        if (Unit* caster = GetCaster())
        {
            caster->CastSpell(caster, SPELL_CARRION_BEETLE, true);
            caster->CastSpell(caster, SPELL_CARRION_BEETLE, true);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_anubarak_carrion_beetles::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

void AddSC_boss_anub_arak()
{
    RegisterCreatureAI(boss_anub_arak);

    RegisterCreatureAI(npc_anubarak_anub_ar_darter);
    RegisterCreatureAI(npc_anubarak_anub_ar_assassin);
    RegisterCreatureAI(npc_anubarak_anub_ar_guardian);
    RegisterCreatureAI(npc_anubarak_anub_ar_venomancer);
    RegisterCreatureAI(npc_anubarak_impale_target);

    RegisterAuraScript(spell_anubarak_pound);
    RegisterAuraScript(spell_anubarak_carrion_beetles);
}
