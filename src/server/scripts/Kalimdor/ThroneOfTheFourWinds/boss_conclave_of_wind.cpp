/*
* This file is part of the Legends of Azeroth Pandaria Project. See THANKS file for Copyright information
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

#include "Containers.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "GridNotifiers.h"
#include "TemporarySummon.h"
#include "throne_of_the_four_winds.h"

enum ScriptTextsConclave
{
    // Anshal
    SAY_ANSHAL_AGGRO      = 0,
    SAY_ANSHAL_KILL       = 1,
    SAY_ANSHAL_LOW        = 2,
    SAY_ANSHAL_ENRAGE     = 3,
    SAY_ANSHAL_ULTIMATE   = 4,
    SAY_ANSHAL_GATHER_TWO = 5,
    SAY_ANSHAL_GATHER_ONE = 6,
    SAY_ULTIMATE_ANNOUNCE = 7,

    // Nezir
    SAY_NEZIR_AGGRO       = 0,
    SAY_NEZIR_KILL        = 1,
    SAY_NEZIR_LOW         = 2,
    SAY_NEZIR_ENRAGE      = 3,
    SAY_NEZIR_ULTIMATE    = 4,
    SAY_NEZIR_GATHER_TWO  = 5,
    SAY_NEZIR_GATHER_ONE  = 6,

    // Rohash
    SAY_ROHASH_AGGRO      = 0,
    SAY_ROHASH_KILL       = 1,
    SAY_ROHASH_LOW        = 2,
    SAY_ROHASH_ENRAGE     = 3,
    SAY_ROHASH_ULTIMATE   = 4,
    SAY_ROHASH_GATHER_TWO = 5,
    SAY_ROHASH_GATHER_ONE = 6,

    // Al'Akir
    SAY_CONCLAVE_DEFEAT   = 0,
};

enum Spells
{
    SPELL_NO_REGEN                  = 96301,
    SPELL_WIND_DISTANCE_CHECKER     = 85763,
    SPELL_DEAD                      = 81238,
    SPELL_GATHER_STRENGTH           = 86307,

    // Anshal
    SPELL_SOOTHING_BREEZE           = 86205,
    SPELL_SOOTHING_BREEZE_HEAL      = 86206,
    SPELL_SOOTHING_BREEZE_SILENCE   = 86207,
    SPELL_SOOTHING_BREEZE_VISUAL    = 86208,

    SPELL_NURTURE                   = 85422,
    SPELL_NURTURE_SUMMON_VISUAL     = 85428,
    SPELL_NURTURE_CREEPER_SUMMON    = 85429,

    SPELL_TOXIC_SPORES              = 86290,

    SPELL_ZEPHYR_ULTIMATE           = 84638,
    SPELL_ZEPHYR_DUMMY              = 89813,

    SPELL_WITHERING_WINDS           = 85576,
    SPELL_WITHERING_WINDS_RESET     = 89137,
    SPELL_WITHERING_WINDS_DAMAGE    = 93168,

    // Nezir
    SPELL_ICE_PATCH_SUMMON          = 86122,
    SPELL_ICE_PATCH                 = 86107,
    SPELL_PERMAFROST                = 86082,
    SPELL_WIND_CHILL                = 84645,

    SPELL_SLEET_STORM_ULTIMATE      = 84644,

    SPELL_CHILLING_WINDS            = 85578,
    SPELL_CHILLING_WINDS_RESET      = 89135,
    SPELL_CHILLING_WINDS_DAMAGE     = 93163,

    // Rohash
    SPELL_SLICING_GALE              = 86182,

    SPELL_WIND_BLAST_PREPARATION    = 86193,
    SPELL_WIND_BLAST                = 85480,

    SPELL_TORNADO_AURA              = 86134,
    SPELL_TORNADO_SUMMON            = 86192,
    SPELL_TORNADO_TARGET            = 109442,

    SPELL_STORM_SHIELD              = 93059,

    SPELL_HURRICANE_ULTIMATE        = 84643,
    SPELL_HURRICANE_DUMMY_ENEMY     = 86498,
    SPELL_HURRICANE_VEHICLE         = 86481,
    SPELL_HURRICANE_FLIGHT_AURA     = 84895,

    SPELL_DEAFENING_WINDS           = 85573,
    SPELL_DEAFENING_WINDS_RESET     = 89136,
    SPELL_DEAFENING_WINDS_DAMAGE    = 93166,

    // Ravenous Creeper
    SPELL_BIRTH_1_SEC               = 34115,

    // All
    SPELL_BERSERK                   = 82395,

    // Achievements
    SPELL_ACHIEVEMENT_CONCLAVE      = 88835,
    SPELL_ACHIEVEMENT_STAY_CHILL    = 94119,
};

enum ExistingMobs
{
    NPC_SOOTHING_BREEZE             = 46246,
    NPC_RAVENOUS_CREEPER            = 45812,
    NPC_RAVENOUS_CREEPER_TRIGGER    = 45813,
    NPC_ICE_PATCH                   = 46186,
    NPC_TORNADO                     = 46207,
    NPC_HURRICANE                   = 46419,
    NPC_WIND_BLAST_TARGET           = 19871,
};

enum Events
{
    // Anshal
    EVENT_SOOTHING_BREEZE = 1,
    EVENT_NURTURE,
    EVENT_TOXIC_SPORES,

    // Nezir
    EVENT_ICE_PATCH,
    EVENT_PERMAFROST,
    EVENT_WIND_CHILL,

    // Rohash
    EVENT_SLICING_GALE,
    EVENT_WIND_BLAST,
    EVENT_WIND_BLAST_END,
    EVENT_TORNADO,
    EVENT_STORM_SHIELD,
    EVENT_HURRICANE,

    // All
    EVENT_ULTIMATE_CHECK,
    EVENT_BERSERK,

    // Ravenous Creeper
    EVENT_TARGET_CHECK,
};

enum Points
{
    POINT_HOME = 1
};

class CouncilGameObject
{
    public:
        CouncilGameObject() { }

        bool operator()(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_WIND_DRAFTEFFECT_CENTER:
                    go->SetGoState(GOState::GO_STATE_READY);
                    break;
                case GO_FLOOR_ALAKIR:
                    go->SetGoState(GOState::GO_STATE_READY);
                    break;
                default:
                    break;
            }
            return false;
        }
};

struct boss_conclave_of_wind : public BossAI
{
    boss_conclave_of_wind(Creature* creature) : BossAI(creature, DATA_CONCLAVE_OF_WIND)
    {
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
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
        creature->SetPower(POWER_MANA, 0);
        creature->SetMaxPower(POWER_MANA, 90);
        evading = false;
        delayedChase = false;
    }

    void Reset() OVERRIDE
    {
        _Reset();
        InternalReset();
        me->SetPower(POWER_MANA, 0);
        DoCast(me, COUNCIL_BOSS(SPELL_PRE_COMBAT_EFFECT_ANSHAL, SPELL_PRE_COMBAT_EFFECT_NEZIR, SPELL_PRE_COMBAT_EFFECT_ROHASH));
        DoCast(me, SPELL_NO_REGEN);
        if (IsRohash())
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        TogglePlatformEffects(false);
        evading = false;
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (me->FindNearestPlayer(500))
            return;

        BossAI::EnterEvadeMode();
        RemoveSummons();
        Position homePos = conclaveHomePositions[COUNCIL_BOSS(0, 1, 2)];
        me->NearTeleportTo(homePos.GetPositionX(), homePos.GetPositionY(), homePos.GetPositionZ(), homePos.GetOrientation());
        me->SetReactState(REACT_AGGRESSIVE);
        instance->SetBossState(DATA_CONCLAVE_OF_WIND, FAIL);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

        // If they are casting "Gather Strength" they won't update target properly
        if (Creature* anshal = GetAnshal())
            if (anshal->IsInCombat())
                anshal->AI()->EnterEvadeMode();
        if (Creature* nezir = GetNezir())
            if (nezir->IsInCombat())
                nezir->AI()->EnterEvadeMode();
        if (Creature* rohash = GetRohash())
            if (rohash->IsInCombat())
                rohash->AI()->EnterEvadeMode();
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        _EnterCombat();
        summons.DespawnAll(); // Just in case

        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

        me->SetPower(POWER_MANA, 0);

        me->RemoveAurasDueToSpell(COUNCIL_BOSS(SPELL_PRE_COMBAT_EFFECT_ANSHAL, SPELL_PRE_COMBAT_EFFECT_NEZIR, SPELL_PRE_COMBAT_EFFECT_ROHASH));
        DoCast(me, SPELL_NO_REGEN);
        Talk(COUNCIL_BOSS(SAY_ANSHAL_AGGRO, SAY_NEZIR_AGGRO, SAY_ROHASH_AGGRO));
        TogglePlatformEffects(false);

        persistentEvents.Reset();
        persistentEvents.ScheduleEvent(EVENT_ULTIMATE_CHECK, 1);
        persistentEvents.ScheduleEvent(EVENT_BERSERK, 480000);
        SetupEvents();

        timerPower = 1000;
        feignDeath = false;
        ultimateAnnounced = false;
        evading = false;
        delayedChase = false;

        if (IsRohash())
            DoStartNoMovement(who);

        DoZoneInCombat(me, 500.0f);

        if (Creature* anshal = GetAnshal())
            if (!anshal->IsInCombat())
                anshal->AI()->DoZoneInCombat(anshal, 500.0f);
        if (Creature* nezir = GetNezir())
            if (!nezir->IsInCombat())
                nezir->AI()->DoZoneInCombat(nezir, 500.0f);
        if (Creature* rohash = GetRohash())
            if (!rohash->IsInCombat())
                rohash->AI()->DoZoneInCombat(rohash, 500.0f);
    }

    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (attacker == me)
            return;

        if (me->GetHealth() <= damage)
        {
            damage = 0;

            if (feignDeath)
                return;

            Creature* anshal = GetAnshal();
            Creature* nezir = GetNezir();
            Creature* rohash = GetRohash();

            if (!anshal || !nezir || !rohash)
                return;

            uint32 remaining = 3;
            if (anshal->GetHealth() == 1 || IsAnshal())
                --remaining;
            if (nezir->GetHealth() == 1 || IsNezir())
                --remaining;
            if (rohash->GetHealth() == 1 || IsRohash())
                --remaining;

            if (!remaining)
            {
                // Kill criteria credit
                DoCastAOE(SPELL_ACHIEVEMENT_CONCLAVE, true);

                // "Stay Chill" achievement credit
                bool achieved = true;
                Map::PlayerList const& players = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->GetSource())
                        if (player->GetAuraCount(SPELL_WIND_CHILL) < 7 && !(achieved = false))
                            break;
                if (achieved)
                    DoCastAOE(SPELL_ACHIEVEMENT_STAY_CHILL, true);

                // Just in case
                anshal->LowerPlayerDamageReq(me->GetMaxHealth());
                nezir->LowerPlayerDamageReq(me->GetMaxHealth());
                rohash->LowerPlayerDamageReq(me->GetMaxHealth());

                attacker->Kill(anshal);
                attacker->Kill(nezir);
                attacker->Kill(rohash);
            }
            else if (!feignDeath)
            {
                if (me->HasAura(COUNCIL_BOSS(SPELL_ZEPHYR_ULTIMATE, SPELL_SLEET_STORM_ULTIMATE, SPELL_HURRICANE_ULTIMATE)))
                    me->SetPower(POWER_MANA, 0);
                me->SetHealth(1);
                me->AttackStop();
                me->CastStop();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveAurasDueToSpell(COUNCIL_BOSS(SPELL_WITHERING_WINDS, SPELL_CHILLING_WINDS, SPELL_DEAFENING_WINDS));
                me->RemoveAurasDueToSpell(COUNCIL_BOSS(SPELL_WITHERING_WINDS_RESET, SPELL_CHILLING_WINDS_RESET, SPELL_DEAFENING_WINDS_RESET));
                TogglePlatformEffects(false);
                DoCast(me, SPELL_DEAD, true);
                DoCast(me, SPELL_GATHER_STRENGTH);
                Talk(COUNCIL_BOSS(SAY_ANSHAL_LOW, SAY_NEZIR_LOW, SAY_ROHASH_LOW));
                if (remaining == 2)
                    Talk(COUNCIL_BOSS(SAY_ANSHAL_GATHER_TWO, SAY_NEZIR_GATHER_TWO, SAY_ROHASH_GATHER_TWO));
                else
                    Talk(COUNCIL_BOSS(SAY_ANSHAL_GATHER_ONE, SAY_NEZIR_GATHER_ONE, SAY_ROHASH_GATHER_ONE));
                events.Reset();
                feignDeath = true;
            }
        }
    }

    void HealReceived(Unit* /*healer*/, uint32& heal) OVERRIDE
    {
        if (feignDeath)
            heal = 0;
    }

    void SpellHitTarget(Unit* target, const SpellInfo* spell) OVERRIDE
    {
        if (spell->Id == SPELL_WIND_DISTANCE_CHECKER)
        {
            evading = false;
            me->RemoveAurasDueToSpell(COUNCIL_BOSS(SPELL_WITHERING_WINDS, SPELL_CHILLING_WINDS, SPELL_DEAFENING_WINDS));
            me->RemoveAurasDueToSpell(COUNCIL_BOSS(SPELL_WITHERING_WINDS_RESET, SPELL_CHILLING_WINDS_RESET, SPELL_DEAFENING_WINDS_RESET));
            TogglePlatformEffects(false);
            AttackStart(target);
            if (!IsRohash())
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    delayedChase = true;
                else
                    me->GetMotionMaster()->MoveChase(target);
            }
        }
        else if (spell->Id == SPELL_GATHER_STRENGTH)
        {
            InternalReset();
            SetupEvents();
        }
    }

    void KilledUnit(Unit* victim) OVERRIDE
    {
        if (victim != me)
            Talk(COUNCIL_BOSS(SAY_ANSHAL_KILL, SAY_NEZIR_KILL, SAY_ROHASH_KILL));
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (IsAnshal())
        {
            persistentEvents.Reset();
            _JustDied();
            RemoveSummons();
            if (Creature* AlAkir = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ALAKIR)))
            {
                AlAkir->AI()->Talk(SAY_CONCLAVE_DEFEAT);
                AlAkir->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }
        }
        else
        {
            persistentEvents.Reset();
            events.Reset();
            summons.DespawnAll();
        }

        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        TogglePlatformEffects(false);
    }

    uint32 GetData(uint32 /*type*/) const OVERRIDE
    {
        return me->HasAura(COUNCIL_BOSS(SPELL_ZEPHYR_ULTIMATE, SPELL_SLEET_STORM_ULTIMATE, SPELL_HURRICANE_ULTIMATE));
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (!evading || type != POINT_MOTION_TYPE || pointId != POINT_HOME)
            return;

        if (!me->HasAura(COUNCIL_BOSS(SPELL_WITHERING_WINDS_RESET, SPELL_CHILLING_WINDS_RESET, SPELL_DEAFENING_WINDS_RESET)))
        {
            Talk(COUNCIL_BOSS(SAY_ANSHAL_ENRAGE, SAY_NEZIR_ENRAGE, SAY_ROHASH_ENRAGE));
            DoCast(me, COUNCIL_BOSS(SPELL_WITHERING_WINDS, SPELL_CHILLING_WINDS, SPELL_DEAFENING_WINDS), true);
            DoCast(me, COUNCIL_BOSS(SPELL_WITHERING_WINDS_RESET, SPELL_CHILLING_WINDS_RESET, SPELL_DEAFENING_WINDS_RESET), true);
            TogglePlatformEffects(true);
        }
    }

    void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }

    bool CanAIAttack(Unit const* target) const OVERRIDE
    {
        return target && target->GetExactDist2d(&conclaveHomePositions[COUNCIL_BOSS(0, 1, 2)]) < 90.0f;
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!me->IsInCombat())
            return;

        persistentEvents.Update(diff);
        events.Update(diff);

        timerPower -= int32(diff);
        while (timerPower <= 0)
        {
            me->ModifyPower(POWER_MANA, 1);
            timerPower += 1000;
        }

        while (uint32 eventId = persistentEvents.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_ULTIMATE_CHECK:
                    if (IsAnshal() && me->GetPower(POWER_MANA) <= 10 && ultimateAnnounced)
                        ultimateAnnounced = false;
                    if (IsAnshal() && me->GetPower(POWER_MANA) >= 80 && !ultimateAnnounced)
                    {
                        ultimateAnnounced = true;
                        Talk(SAY_ULTIMATE_ANNOUNCE);
                    }
                    if (me->GetPower(POWER_MANA) >= 90 && !me->HasAura(COUNCIL_BOSS(SPELL_ZEPHYR_ULTIMATE, SPELL_SLEET_STORM_ULTIMATE, SPELL_HURRICANE_ULTIMATE)))
                    {
                        if (!feignDeath)
                        {
                            Position homePos = conclaveHomePositions[COUNCIL_BOSS(0, 1, 2)];
                            me->NearTeleportTo(homePos.GetPositionX(), homePos.GetPositionY(), homePos.GetPositionZ(), homePos.GetOrientation());
                            me->CastStop();
                            Talk(COUNCIL_BOSS(SAY_ANSHAL_ULTIMATE, SAY_NEZIR_ULTIMATE, SAY_ROHASH_ULTIMATE));
                            DoCast(me, COUNCIL_BOSS(SPELL_ZEPHYR_ULTIMATE, SPELL_SLEET_STORM_ULTIMATE, SPELL_HURRICANE_ULTIMATE));
                            UltimateStarted();
                        }
                        else
                            me->SetPower(POWER_MANA, 0);
                        timerPower = 16000;
                    }
                    if (!feignDeath && !evading && !me->HasAura(COUNCIL_BOSS(SPELL_ZEPHYR_ULTIMATE, SPELL_SLEET_STORM_ULTIMATE, SPELL_HURRICANE_ULTIMATE)))
                    {
                        Unit* target = me->FindNearestPlayer(90);
                        if (!target || target->GetExactDist2d(&conclaveHomePositions[COUNCIL_BOSS(0, 1, 2)]) > 90.0f ||
                            !me->GetVictim() || me->GetVictim()->GetExactDist2d(&conclaveHomePositions[COUNCIL_BOSS(0, 1, 2)]) > 90.0f ||
                            me->GetExactDist2d(&conclaveHomePositions[COUNCIL_BOSS(0, 1, 2)]) > 90.0f)
                        {
                            evading = true;
                            me->CastStop();
                            if (IsRohash())
                            {
                                me->RemoveAurasDueToSpell(SPELL_WIND_BLAST_PREPARATION);
                                MovementInform(POINT_MOTION_TYPE, POINT_HOME);
                            }
                            else
                            {
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MovePoint(POINT_HOME, conclaveHomePositions[COUNCIL_BOSS(0, 1, 2)]);
                            }
                        }
                    }
                    persistentEvents.ScheduleEvent(EVENT_ULTIMATE_CHECK, 1000);
                    break;
                case EVENT_BERSERK:
                    DoCast(me, SPELL_BERSERK, true);
                    break;
                default:
                    HandlePersistantEvent(eventId);
                    break;
            }
        }

        if (evading && !me->FindNearestPlayer(500))
        {
            EnterEvadeMode();
            return;
        }

        if (evading || !UpdateVictim() || !instance)
            return;

        UpdateHook();

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (delayedChase)
        {
            delayedChase = false;
            me->GetMotionMaster()->MoveChase(me->GetVictim());
        }

        while (uint32 eventId = events.ExecuteEvent())
            HandleEvent(eventId);

        if (!feignDeath)
            DoMeleeAttackIfReady();
    }

protected:
    EventMap persistentEvents;
    int32 timerPower;
    bool ultimateAnnounced;
    bool feignDeath;
    bool evading;
    bool delayedChase;

    template<class T> inline
    const T& COUNCIL_BOSS(const T& anshal, const T& nezir, const T& rohash) const
    {
        switch (me->GetEntry())
        {
            case BOSS_ANSHAL: return anshal;
            case BOSS_NEZIR: return nezir;
            case BOSS_ROHASH: return rohash;
            default: ASSERT(false);
        }
        return anshal; // Shut the compiler up
    }

    inline bool IsAnshal() const { return me->GetEntry() == BOSS_ANSHAL; }
    inline bool IsNezir() const { return me->GetEntry() == BOSS_NEZIR; }
    inline bool IsRohash() const { return me->GetEntry() == BOSS_ROHASH; }
    inline Creature* GetAnshal() const { return IsAnshal() ? me : me->FindNearestCreature(BOSS_ANSHAL, 500.0f); }
    inline Creature* GetNezir() const { return IsNezir() ? me : me->FindNearestCreature(BOSS_NEZIR, 500.0f); }
    inline Creature* GetRohash() const { return IsRohash() ? me : me->FindNearestCreature(BOSS_ROHASH, 500.0f); }

    void InternalReset()
    {
        summons.DespawnAll();

        events.Reset();

        me->SetHealth(me->GetMaxHealth());
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        if (!IsRohash())
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        me->ClearUnitState(UNIT_STATE_CANNOT_TURN);
        me->RemoveAurasDueToSpell(SPELL_DEAD);
        me->RemoveAurasDueToSpell(SPELL_GATHER_STRENGTH);

        feignDeath = false;
    }
    void TogglePlatformEffects(bool enabled)
    {
        if (GameObject* platform = me->FindNearestGameObject(COUNCIL_BOSS(GO_DJINN_HEALING, GO_DJINN_FROST, GO_DJINN_TORNADO), 100))
            platform->SetGoState(enabled ? GOState::GO_STATE_ACTIVE : GOState::GO_STATE_READY);
    }
    void RemoveSummons()
    {
        std::list<Creature*> summons;
        GetCreatureListWithEntryInGrid(summons, me, NPC_RAVENOUS_CREEPER, 1000);
        GetCreatureListWithEntryInGrid(summons, me, NPC_RAVENOUS_CREEPER_TRIGGER, 1000);
        GetCreatureListWithEntryInGrid(summons, me, NPC_TORNADO, 1000);
        GetCreatureListWithEntryInGrid(summons, me, NPC_ICE_PATCH, 1000);
        for (std::list<Creature*>::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
            if (Creature* summon = *itr)
                summon->DespawnOrUnsummon();
    }

    virtual void SetupEvents() = 0;
    virtual void HandleEvent(uint32 eventId) = 0;
    virtual void HandlePersistantEvent(uint32 eventId) { }
    virtual void UltimateStarted() { }
    virtual void UpdateHook() { }
};

class boss_anshal : public CreatureScript
{
    public:
        boss_anshal() : CreatureScript("boss_anshal") { }

        struct boss_anshalAI : public boss_conclave_of_wind
        {
            boss_anshalAI(Creature* creature) : boss_conclave_of_wind(creature) { }

            void SetupEvents() OVERRIDE
            {
                events.ScheduleEvent(EVENT_SOOTHING_BREEZE,     16000);
                events.ScheduleEvent(EVENT_NURTURE,             30000);
                events.ScheduleEvent(EVENT_TOXIC_SPORES,        20000);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void HandleEvent(uint32 eventId) OVERRIDE
            {
                switch (eventId)
                {
                    case EVENT_SOOTHING_BREEZE:
                        if (Creature* creeper = me->FindNearestCreature(NPC_RAVENOUS_CREEPER, 90.0f))
                            DoCast(creeper, SPELL_SOOTHING_BREEZE);
                        else
                            DoCast(me, SPELL_SOOTHING_BREEZE);
                        events.ScheduleEvent(EVENT_SOOTHING_BREEZE, 32000);
                        break;
                    case EVENT_NURTURE:
                        DoCast(me, SPELL_NURTURE);
                        //DoCast(me, SPELL_DUMMY_FOR_NURTURE);
                        events.ScheduleEvent(EVENT_NURTURE, 140000);
                        break;
                    case EVENT_TOXIC_SPORES:
                        DoCast(me, SPELL_TOXIC_SPORES);
                        events.ScheduleEvent(EVENT_TOXIC_SPORES, 21000);
                        break;
                    default:
                        break;
                }
            }

            void UltimateStarted() OVERRIDE
            {
                events.RescheduleEvent(EVENT_SOOTHING_BREEZE, 15000 + 16000);
                events.RescheduleEvent(EVENT_NURTURE, 15000 + 35000);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_anshalAI>(creature, "instance_throne_of_the_four_winds");
        }
};

class boss_nezir : public CreatureScript
{
    public:
        boss_nezir() : CreatureScript("boss_nezir") { }

        struct boss_nezirAI : public boss_conclave_of_wind
        {
            boss_nezirAI(Creature* creature) : boss_conclave_of_wind(creature) { }

            void SetupEvents() OVERRIDE
            {
                events.ScheduleEvent(EVENT_ICE_PATCH,           urand(10000,12000));
                events.ScheduleEvent(EVENT_PERMAFROST,          10000);
                events.ScheduleEvent(EVENT_WIND_CHILL,          10500);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);

                switch (summon->GetEntry())
                {
                    case NPC_ICE_PATCH:
                        summon->SetReactState(REACT_PASSIVE);
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        summon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                        summon->CastSpell(summon, SPELL_ICE_PATCH, false, NULL, NULL, me->GetGUID());
                        summon->DespawnOrUnsummon(30000);
                        break;
                    default:
                        break;
                }
                summon->AI()->DoZoneInCombat();
            }

            void HandleEvent(uint32 eventId) OVERRIDE
            {
                switch (eventId)
                {
                    case EVENT_ICE_PATCH:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, IcePatchTargetSelector(me)))
                            DoCast(target, SPELL_ICE_PATCH_SUMMON);
                        events.ScheduleEvent(EVENT_ICE_PATCH, urand(10000, 12000));
                        break;
                    case EVENT_PERMAFROST:
                        DoCastVictim(SPELL_PERMAFROST);
                        events.ScheduleEvent(EVENT_PERMAFROST, urand(10000, 20000));
                        break;
                    case EVENT_WIND_CHILL:
                        DoCast(me, SPELL_WIND_CHILL);
                        events.ScheduleEvent(EVENT_WIND_CHILL, 15000);
                        break;
                    default:
                        break;
                }
            }

        private:
            struct IcePatchTargetSelector
            {
                public:
                    IcePatchTargetSelector(Creature const* me) : _me(me) { }
                    bool operator()(Unit const* target) const
                    {
                        return target && _me->GetExactDist2d(target) <= 120 && _me->GetVictim() != target;
                    }

                private:
                    Creature const* _me;
            };
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_nezirAI>(creature, "instance_throne_of_the_four_winds");
        }
};

class boss_rohash : public CreatureScript
{
    public:
        boss_rohash() : CreatureScript("boss_rohash") { }

        struct boss_rohashAI : public boss_conclave_of_wind
        {
            boss_rohashAI(Creature* creature) : boss_conclave_of_wind(creature) { }

            void SetupEvents() OVERRIDE
            {
                if (Creature* trigger = me->FindNearestCreature(NPC_WIND_BLAST_TARGET, 100))
                {
                    trigger->SetReactState(REACT_PASSIVE);
                    trigger->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                    trigger->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                }

                windBlastCount = 0;
                affectedByHurricaneGuids.clear();

                events.ScheduleEvent(EVENT_SLICING_GALE,        1000);
                events.ScheduleEvent(EVENT_WIND_BLAST,          30000);
                events.ScheduleEvent(EVENT_TORNADO,             20000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_STORM_SHIELD,    30000);
            }

            void SpellHitTarget(Unit* target, const SpellInfo* spell) OVERRIDE
            {
                boss_conclave_of_wind::SpellHitTarget(target, spell);
                if (spell->Id == SPELL_HURRICANE_DUMMY_ENEMY)
                {
                    Position pos;
                    me->GetPosition(&pos);
                    pos.m_positionZ = 250;
                    if (me->SummonCreature(NPC_HURRICANE, pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 20000))
                    {
                        target->CastSpell(target, SPELL_HURRICANE_FLIGHT_AURA, true);
                        // Hack to make a player "fly". Without it the camera drops to the ground when boarding a flying vehicle
                        float x, y, z, o;
                        target->GetPosition(x, y, z, o);
                        target->NearTeleportTo(x, y, z + 5, o);
                        affectedByHurricaneGuids.push_back(target->GetGUID());
                        persistentEvents.ScheduleEvent(EVENT_HURRICANE, affectedByHurricaneGuids.size() * 200);
                    }
                }
                else if (spell->Id == SPELL_WIND_BLAST_PREPARATION && target->GetEntry() == NPC_WIND_BLAST_TARGET)
                {
                    target->SetSpeed(MOVE_FLIGHT, 2, true);
                    me->SetFacingToObject(target);
                }
            }

            void HandleEvent(uint32 eventId) OVERRIDE
            {
                switch (eventId)
                {
                    case EVENT_WIND_BLAST:
                        me->AddUnitState(UNIT_STATE_CANNOT_TURN);
                        DoCast(me, SPELL_WIND_BLAST_PREPARATION);
                        events.ScheduleEvent(EVENT_WIND_BLAST, ++windBlastCount == 1 ? 82000 : 60000);
                        events.ScheduleEvent(EVENT_WIND_BLAST_END, 11000);
                        events.RescheduleEvent(EVENT_SLICING_GALE, 12000);
                        break;
                    case EVENT_WIND_BLAST_END:
                        me->ClearUnitState(UNIT_STATE_CANNOT_TURN);
                        break;
                    case EVENT_SLICING_GALE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 90.0f))
                            DoCast(target, SPELL_SLICING_GALE);
                        events.ScheduleEvent(EVENT_SLICING_GALE, 2000);
                        break;
                    case EVENT_TORNADO:
                        DoCast(me, SPELL_TORNADO_SUMMON);
                        events.ScheduleEvent(EVENT_TORNADO, 45000);
                        break;
                    case EVENT_STORM_SHIELD:
                        DoCast(me, SPELL_STORM_SHIELD);
                        events.ScheduleEvent(EVENT_STORM_SHIELD, 120000);
                        break;
                    default:
                        break;
                }
            }

            void HandlePersistantEvent(uint32 eventId) OVERRIDE
            {
                if (eventId == EVENT_HURRICANE && !affectedByHurricaneGuids.empty())
                {
                    uint64 affectedGuid = Skyfire::Containers::SelectRandomContainerElement(affectedByHurricaneGuids);
                    std::list<uint64>::iterator itr = std::find(affectedByHurricaneGuids.begin(), affectedByHurricaneGuids.end(), affectedGuid);
                    affectedByHurricaneGuids.erase(itr);
                    if (Player* affected = ObjectAccessor::GetPlayer(*me, affectedGuid))
                        affected->CastSpell(affected, SPELL_HURRICANE_VEHICLE, true);
                }
            }

            void UltimateStarted() OVERRIDE
            {
                if (IsHeroic())
                    events.RescheduleEvent(EVENT_STORM_SHIELD, 15000 + 35000);
            }

            void UpdateHook() OVERRIDE
            {
                if (Spell* spell = me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                {
                    if (spell->GetSpellInfo()->Id == SPELL_WIND_BLAST_PREPARATION || spell->GetSpellInfo()->Id == SPELL_WIND_BLAST)
                    {
                        if (Creature* trigger = me->FindNearestCreature(NPC_WIND_BLAST_TARGET, 100))
                        {
                            me->SetFacingToObject(trigger);
                            me->SetOrientation(me->GetAngle(trigger));
                        }
                    }
                }
            }

        private:
            uint32 windBlastCount;
            std::list<uint64> affectedByHurricaneGuids;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_rohashAI>(creature, "instance_throne_of_the_four_winds");
        }
};

class npc_tornado : public CreatureScript
{
    public:
        npc_tornado() : CreatureScript("npc_tornado") { }

        struct npc_tornadoAI : public ScriptedAI
        {
            npc_tornadoAI(Creature* creature) : ScriptedAI(creature)
            {
                TargetSelectTimer = 1;
                creature->SetReactState(REACT_PASSIVE);
            }

            uint32 TargetSelectTimer;

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                DoCast(me, SPELL_TORNADO_AURA);
                me->SetSpeed(MOVE_RUN, 0.3f);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                me->DespawnOrUnsummon(20000);
                DoZoneInCombat();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (TargetSelectTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 90.0f, true))
                    {
                        me->GetMotionMaster()->MovePoint(1, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                        DoCast(target, SPELL_TORNADO_TARGET);
                        TargetSelectTimer = 6000;
                    }
                }
                else TargetSelectTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_tornadoAI(creature);
        }

};

class npc_ravenous_creeper : public CreatureScript
{
    public:
        npc_ravenous_creeper() : CreatureScript("npc_ravenous_creeper") { }

        struct npc_ravenous_creeperAI : public ScriptedAI
        {
            npc_ravenous_creeperAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                evading = false;
                me->GetPosition(&homePos);
                events.Reset();
                events.ScheduleEvent(EVENT_TARGET_CHECK, 1000);
                DoCast(SPELL_BIRTH_1_SEC);
                me->SetReactState(REACT_AGGRESSIVE);
                if (Player* target = me->FindNearestPlayer(90))
                    AttackStart(target);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
            }

            bool CanAIAttack(Unit const* target) const OVERRIDE
            {
                return target && target->GetExactDist2d(&conclaveHomePositions[0]) < 90.0f;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TARGET_CHECK:
                            if (evading || !me->GetVictim() ||
                                me->GetVictim()->GetExactDist2d(&conclaveHomePositions[0]) > 90.0f ||
                                me->GetExactDist2d(&conclaveHomePositions[0]) > 90.0f)
                            {
                                if (me->GetVictim())
                                    me->getThreatManager().modifyThreatPercent(me->GetVictim(), -100);
                                me->GetMotionMaster()->Clear();
                                Player* target = me->FindNearestPlayer(90);
                                if (target && target->GetExactDist2d(&conclaveHomePositions[0]) <= 90.0f)
                                {
                                    evading = false;
                                    AttackStart(target);
                                    me->GetMotionMaster()->MoveChase(target);
                                }
                                else if (!evading)
                                {
                                    evading = true;
                                    me->GetMotionMaster()->MovePoint(0, homePos);
                                }
                            }
                            events.ScheduleEvent(EVENT_TARGET_CHECK, 1000);
                            break;
                        default:
                            break;
                    }
                }

                if (UpdateVictim())
                    DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
            Position homePos;
            bool evading;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_ravenous_creeperAI(creature);
        }
};

class npc_ravenous_creeper_trigger : public CreatureScript
{
    public:
        npc_ravenous_creeper_trigger() : CreatureScript("npc_ravenous_creeper_trigger") { }

        struct npc_ravenous_creeper_triggerAI : public ScriptedAI
        {
            npc_ravenous_creeper_triggerAI(Creature* creature) : ScriptedAI(creature)
            {
                CreeperSummonTimer = 9000;
                DoCast(creature, SPELL_NURTURE_SUMMON_VISUAL);
                me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
            }

            void SummonedCreatureDespawn(Creature* /*summon*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!CreeperSummonTimer)
                    return;

                if (CreeperSummonTimer <= diff)
                {
                    me->RemoveAurasDueToSpell(SPELL_NURTURE_SUMMON_VISUAL);
                    DoCast(me, SPELL_NURTURE_CREEPER_SUMMON);
                    CreeperSummonTimer = 0;
                }
                else CreeperSummonTimer -= diff;
            }

        private:
            uint32 CreeperSummonTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_ravenous_creeper_triggerAI(creature);
        }
};

class npc_soothing_breeze : public CreatureScript
{
    public:
        npc_soothing_breeze() : CreatureScript("npc_soothing_breeze") { }

        struct npc_soothing_breezeAI : public ScriptedAI
        {
            npc_soothing_breezeAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                DoCast(me, SPELL_SOOTHING_BREEZE_VISUAL, true);
                DoCast(me, SPELL_SOOTHING_BREEZE_HEAL, true);
                DoCast(me, SPELL_SOOTHING_BREEZE_SILENCE, true);
                me->DespawnOrUnsummon(30000);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_soothing_breezeAI(creature);
        }
};

class spell_hurricane : public SpellScriptLoader
{
    public:
        spell_hurricane() : SpellScriptLoader("spell_hurricane") { }

        class spell_hurricane_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hurricane_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->RemoveAurasDueToSpell(SPELL_HURRICANE_FLIGHT_AURA);
            }

            void Register() OVERRIDE
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_hurricane_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_hurricane_AuraScript();
        }
};

class spell_conclave_distance_debuff : SpellScriptLoader
{
    public:
        spell_conclave_distance_debuff(char const* name, uint32 damageSpellId) : SpellScriptLoader(name), _damageSpellId(damageSpellId) { }

        class spell_conclave_distance_debuff_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_conclave_distance_debuff_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                uint32 ticks = aurEff->GetTickNumber();
                int32 damage = aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].BasePoints * ticks;
                if (Unit* caster = GetCaster())
                    caster->CastCustomSpell(caster, _damageSpellId, &damage, NULL, NULL, true);
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_conclave_distance_debuff_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }

            uint32 _damageSpellId;

        public:
            spell_conclave_distance_debuff_AuraScript(uint32 damageSpellId) : AuraScript(), _damageSpellId(damageSpellId) { }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_conclave_distance_debuff_AuraScript(_damageSpellId);
        }

    private:
        uint32 _damageSpellId;
};

void AddSC_boss_conclave_of_wind()
{
    new boss_anshal();
    new boss_nezir();
    new boss_rohash();
    new npc_tornado();
    new npc_ravenous_creeper();
    new npc_ravenous_creeper_trigger();
    new npc_soothing_breeze();
    new spell_hurricane();
    new spell_conclave_distance_debuff("spell_withering_winds", SPELL_WITHERING_WINDS_DAMAGE);
    new spell_conclave_distance_debuff("spell_chilling_winds", SPELL_CHILLING_WINDS_DAMAGE);
    new spell_conclave_distance_debuff("spell_deafening_winds", SPELL_DEAFENING_WINDS_DAMAGE);
}