/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "temple_of_jade_serpent.h"

enum WiseMariTexts
{
    SAY_BOSS_EMOTE_AGGRO   = 0,
    SAY_AGGRO              = 1,
    SAY_CALL_WATER         = 2,
    SAY_PHASE_SWITCH       = 3,
    SAY_PHASE_SWITCH_EMOTE = 4,
    SAY_DEATH              = 5,
    SAY_KILL_PLAYER        = 8,

    SAY_LIVING_WATER_RISE  = 0,
};

enum WiseMariEvents
{
    EVENT_CALL_WATER = 1,
    EVENT_HYDROLANCE,
    EVENT_HYDROLANCE_START,
    EVENT_SWITCH_PHASE_TWO,
    EVENT_DONE,
    EVENT_TALK_DEATH,
    EVENT_SUMMON_TRIGGERS,
    EVENT_BUBBLE_BURST,
};

enum WiseMariSpells
{
    SPELL_WATER_BUBBLE_WISE       = 106062,
    SPELL_CALL_WATER              = 106526,
    SPELL_CORRUPTED_FOUNTAIN      = 106518,
    SPELL_HYDROLANCE_PRECAST      = 115220,
    SPELL_HYDROLANCE_DMG_BOTTOM   = 106267,
    SPELL_HYDROLANCE_VISUAL       = 106055,
    SPELL_HYDROLANCE_DMG          = 106105,
    SPELL_HYDROLANCE_PULSE        = 106104,
    SPELL_HYDROLANCE_PULSE_AURA   = 106098,
    SPELL_WASH_AWAY               = 106331,
    SPELL_RELEASED_FROM_SHA       = 106770,
    SPELL_CLEANSING_WATERS        = 106771,
    SPELL_CLAIMING_WATERS         = 115622,
    SPELL_QUIET_SUICIDE           = 115372,
    SPELL_BLESSING_OF_WATERSPEAKER = 121483,
    SPELL_KNEEL                   = 115368,
    SPELL_BUBBLE_BURST            = 106612,
    SPELL_PURIFIED_WATER          = 118714,
    SPELL_DRY                     = 128437, // achievement requirement, removed by any ability that fails it

    SPELL_SUMMON_CORRUPT_DROPLET  = 106762,
    SPELL_SHA_RESIDUE             = 106653,
    SPELL_CORRUPTED_WATERS        = 115165,
};

enum WiseMariActions
{
    ACTION_LIVING_WATER_DEAD = 0,
    ACTION_HYDROLANCE        = 1,
    ACTION_WASH_AWAY         = 2,

    ACTION_STALKER_START     = 1,
    ACTION_STALKER_STOP      = 2,
};

enum HydrolancePhase
{
    HYDROLANCE_BOTTOM = 1,
    HYDROLANCE_LEFT   = 2,
    HYDROLANCE_RIGHT  = 3,
};

uint32 const HydrolanceStartTimer = 10 * IN_MILLISECONDS;

float const HydrolanceLeftTrigger[5][3] =
{
    { 1061.411f, -2570.721f, 174.2403f },
    { 1058.921f, -2573.487f, 174.2403f },
    { 1055.910f, -2575.674f, 174.2403f },
    { 1052.511f, -2577.188f, 174.2403f },
    { 1048.871f, -2577.961f, 174.2403f },
};

float const HydrolanceRightTrigger[5][3] =
{
    { 1035.333f, -2573.693f, 174.2403f },
    { 1032.795f, -2570.971f, 174.2403f },
    { 1030.878f, -2567.781f, 174.2403f },
    { 1029.667f, -2564.263f, 174.2403f },
    { 1029.213f, -2560.569f, 174.2403f },
};

static void SignalWiseStalker(Creature* source, int32 action)
{
    if (Creature* stalker = source->FindNearestCreature(NPC_WISE_STALKER, 100.0f))
        if (stalker->IsAIEnabled)
            stalker->AI()->DoAction(action);
}

class boss_wase_mari : public CreatureScript
{
public:
    boss_wase_mari() : CreatureScript("boss_wase_mari") { }

    struct boss_wise_mariAI : public BossAI
    {
        boss_wise_mariAI(Creature* creature) : BossAI(creature, DATA_WISE_MARI)
        {
            _fightWon = false;
            _defeated = false;
            _fountainCount = 0;
            _hydrolancePhase = HYDROLANCE_BOTTOM;
            memset(_fountainTriggers, 0, sizeof(_fountainTriggers));
        }

        void InitializeAI() OVERRIDE
        {
            me->setActive(true);
            BossAI::InitializeAI();
        }

        void Reset() OVERRIDE
        {
            _Reset();
            _hydrolancePhase = HYDROLANCE_BOTTOM;
            _fountainCount = 0;
            _cosmeticEvents.Reset();
            _cosmeticEvents.ScheduleEvent(EVENT_SUMMON_TRIGGERS, 500);
            me->RemoveAurasDueToSpell(SPELL_WATER_BUBBLE_WISE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetReactState(REACT_PASSIVE);
            me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 16.5f);
            me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 16.5f);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            DoCast(me, SPELL_DRY);
            me->PrepareChanneledCast(me->GetOrientation());
            Talk(SAY_AGGRO);
            Talk(SAY_BOSS_EMOTE_AGGRO);
            me->SetInCombatWithZone();
            DoCast(me, SPELL_WATER_BUBBLE_WISE, true);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

            _hydrolancePhase = HYDROLANCE_BOTTOM;
            events.ScheduleEvent(EVENT_CALL_WATER, 8 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_HYDROLANCE_START, HydrolanceStartTimer);

            if (!me->FindNearestCreature(NPC_WISE_STALKER, 100.0f))
                me->SummonCreature(NPC_WISE_STALKER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            SignalWiseStalker(me, ACTION_STALKER_START);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(SAY_KILL_PLAYER);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (me->GetHealth() > damage)
                return;

            if (!_fightWon)
                damage = 0;

            if (_defeated)
                return;

            _defeated = true;
            _JustDied();
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            me->CastStop();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetFacingTo(me->GetAngle(1049.362f, -2552.748f));
            me->RemoveAurasDueToSpell(SPELL_WASH_AWAY);
            DoCast(me, SPELL_KNEEL);
            DoCast(me, SPELL_RELEASED_FROM_SHA);
            DoCast(me, SPELL_CLEANSING_WATERS);
            DoCast(me, SPELL_CLAIMING_WATERS);
            _cosmeticEvents.ScheduleEvent(EVENT_TALK_DEATH, 1 * IN_MILLISECONDS);
            SignalWiseStalker(me, ACTION_STALKER_STOP);
        }

        void EnterEvadeMode() OVERRIDE
        {
            BossAI::EnterEvadeMode();
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            std::list<Creature*> droplets;
            GetCreatureListWithEntryInGrid(droplets, me, NPC_CORRUPT_DROPLET, 100.0f);
            for (Creature* droplet : droplets)
                if (droplet->IsSummon())
                    droplet->DespawnOrUnsummon();

            SignalWiseStalker(me, ACTION_STALKER_STOP);
            _DespawnAtEvade();
        }

        void DoAction(int32 action) OVERRIDE
        {
            switch (action)
            {
                case ACTION_LIVING_WATER_DEAD:
                    if (++_fountainCount == 4)
                        _cosmeticEvents.ScheduleEvent(EVENT_BUBBLE_BURST, urand(4, 6) * IN_MILLISECONDS);
                    else
                        events.ScheduleEvent(EVENT_CALL_WATER, urand(12, 16) * IN_MILLISECONDS);
                    break;
                case ACTION_HYDROLANCE:
                    events.ScheduleEvent(EVENT_HYDROLANCE, 250);
                    events.ScheduleEvent(EVENT_HYDROLANCE_START, urand(8, 12) * IN_MILLISECONDS);
                    break;
                case ACTION_WASH_AWAY:
                    events.Reset();
                    events.ScheduleEvent(EVENT_SWITCH_PHASE_TWO, 500);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            _cosmeticEvents.Update(diff);
            while (uint32 eventId = _cosmeticEvents.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_DONE:
                    {
                        _fightWon = true;
                        instance->DoCastSpellOnPlayers(SPELL_BLESSING_OF_WATERSPEAKER);
                        instance->DoAddAuraOnPlayers(SPELL_PURIFIED_WATER);

                        if (Player* looter = me->FindNearestPlayer(50.0f))
                        {
                            me->SetLootRecipient(looter);
                            me->CastSpell(me, SPELL_QUIET_SUICIDE, false, NULL, NULL, looter->GetGUID());
                        }
                        break;
                    }
                    case EVENT_TALK_DEATH:
                        Talk(SAY_DEATH);
                        _cosmeticEvents.ScheduleEvent(EVENT_DONE, 9 * IN_MILLISECONDS);
                        break;
                    case EVENT_SUMMON_TRIGGERS:
                        for (uint8 i = 0; i < 4; ++i)
                            if (Creature* trigger = me->SummonCreature(NPC_FOUNTAIN_TRIGGER, FountainTriggerSummonPos[i]))
                                _fountainTriggers[i] = trigger->GetGUID();
                        break;
                    case EVENT_BUBBLE_BURST:
                        DoCast(me, SPELL_BUBBLE_BURST);
                        break;
                    default:
                        break;
                }
            }

            events.Update(diff);

            if (!UpdateVictim())
                return;

            if (me->GetTarget())
                me->SetTarget(0);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CALL_WATER:
                        Talk(SAY_CALL_WATER);
                        if (_fountainCount < 4)
                        {
                            if (Creature* trigger = ObjectAccessor::GetCreature(*me, _fountainTriggers[_fountainCount]))
                            {
                                DoCast(trigger, SPELL_CALL_WATER);
                                trigger->AddAura(SPELL_CORRUPTED_FOUNTAIN, trigger);
                            }
                        }
                        break;
                    case EVENT_HYDROLANCE_START:
                        StartHydrolance();
                        break;
                    case EVENT_HYDROLANCE:
                        FireHydrolance();
                        break;
                    case EVENT_SWITCH_PHASE_TWO:
                        Talk(SAY_PHASE_SWITCH);
                        Talk(SAY_PHASE_SWITCH_EMOTE);
                        me->RemoveAurasDueToSpell(SPELL_WATER_BUBBLE_WISE);
                        DoCast(me, SPELL_WASH_AWAY, true);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        void StartHydrolance()
        {
            float facing = 0.0f;
            switch (_hydrolancePhase)
            {
                case HYDROLANCE_BOTTOM:
                {
                    std::list<Creature*> triggers;
                    me->GetCreatureListWithEntryInGrid(triggers, NPC_HYDROLANCE_TRIGGER, 50.0f);
                    for (Creature* trigger : triggers)
                        trigger->CastSpell(trigger, SPELL_HYDROLANCE_PRECAST, true);
                    facing = 1.23f;
                    break;
                }
                case HYDROLANCE_RIGHT:
                    for (uint8 i = 0; i < 5; ++i)
                        me->CastSpell(HydrolanceRightTrigger[i][0], HydrolanceRightTrigger[i][1], HydrolanceRightTrigger[i][2], SPELL_HYDROLANCE_PRECAST, true);
                    facing = 3.55f;
                    break;
                case HYDROLANCE_LEFT:
                    for (uint8 i = 0; i < 5; ++i)
                        me->CastSpell(HydrolanceLeftTrigger[i][0], HydrolanceLeftTrigger[i][1], HydrolanceLeftTrigger[i][2], SPELL_HYDROLANCE_PRECAST, true);
                    facing = 5.25f;
                    break;
                default:
                    break;
            }

            DoCast(me, SPELL_HYDROLANCE_VISUAL);
            me->UpdatePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), facing);
            me->SetFacingTo(facing);
        }

        void FireHydrolance()
        {
            switch (_hydrolancePhase)
            {
                case HYDROLANCE_BOTTOM:
                {
                    std::list<Creature*> triggers;
                    me->GetCreatureListWithEntryInGrid(triggers, NPC_HYDROLANCE_TRIGGER, 50.0f);
                    for (Creature* trigger : triggers)
                        trigger->CastSpell(trigger->GetPositionX(), trigger->GetPositionY(), trigger->GetPositionZ(), SPELL_HYDROLANCE_DMG_BOTTOM, true);
                    break;
                }
                case HYDROLANCE_RIGHT:
                    for (uint8 i = 0; i < 5; ++i)
                        me->CastSpell(HydrolanceRightTrigger[i][0], HydrolanceRightTrigger[i][1], HydrolanceRightTrigger[i][2], SPELL_HYDROLANCE_DMG, true);
                    break;
                case HYDROLANCE_LEFT:
                    for (uint8 i = 0; i < 5; ++i)
                        me->CastSpell(HydrolanceLeftTrigger[i][0], HydrolanceLeftTrigger[i][1], HydrolanceLeftTrigger[i][2], SPELL_HYDROLANCE_DMG, true);
                    break;
                default:
                    break;
            }

            _hydrolancePhase = _hydrolancePhase == HYDROLANCE_RIGHT ? HYDROLANCE_BOTTOM : _hydrolancePhase + 1;
        }

        EventMap _cosmeticEvents;
        bool _fightWon;
        bool _defeated;
        uint8 _fountainCount;
        uint32 _hydrolancePhase;
        uint64 _fountainTriggers[4];
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetJadeSerpentAI<boss_wise_mariAI>(creature);
    }
};

class npc_corrupt_living_water : public CreatureScript
{
public:
    npc_corrupt_living_water() : CreatureScript("npc_corrupt_living_water") { }

    struct npc_corrupt_living_waterAI : public ScriptedAI
    {
        npc_corrupt_living_waterAI(Creature* creature) : ScriptedAI(creature)
        {
            _instance = creature->GetInstanceScript();
            _dying = false;
        }

        void InitializeAI() OVERRIDE
        {
            if (me->GetEntry() == NPC_CORRUPT_WATER_WISE)
                Talk(SAY_LIVING_WATER_RISE);
            ScriptedAI::InitializeAI();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (me->GetEntry() != NPC_CORRUPT_WATER_WISE || !_instance)
                return;

            if (Creature* mari = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_WISE_MARI)))
                if (mari->IsAIEnabled)
                    mari->AI()->DoAction(ACTION_LIVING_WATER_DEAD);
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            summon->SetInCombatWithZone();
            if (Player* player = me->FindNearestPlayer(100.0f))
                summon->AI()->AttackStart(player);
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            if (me->GetHealth() > damage)
                return;

            damage = 0;
            if (_dying)
                return;

            _dying = true;
            for (uint8 i = 0; i < 3; ++i)
                DoCast(me, SPELL_SUMMON_CORRUPT_DROPLET, true);
            DoCast(me, SPELL_SHA_RESIDUE, true);
            attacker->Kill(me);
        }

    private:
        InstanceScript* _instance;
        bool _dying;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_corrupt_living_waterAI(creature);
    }
};

class npc_wise_stalker : public CreatureScript
{
public:
    npc_wise_stalker() : CreatureScript("npc_wise_stalker") { }

    struct npc_wise_stalkerAI : public ScriptedAI
    {
        npc_wise_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_STALKER_START)
                DoCast(me, SPELL_CORRUPTED_WATERS);
            else if (action == ACTION_STALKER_STOP)
                me->RemoveAurasDueToSpell(SPELL_CORRUPTED_WATERS);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE { }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_wise_stalkerAI(creature);
    }
};

// 106098 - Hydrolance
class spell_wise_hydrolance_pulse : public SpellScriptLoader
{
public:
    spell_wise_hydrolance_pulse() : SpellScriptLoader("spell_wise_hydrolance_pulse") { }

    class spell_wise_hydrolance_pulse_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_wise_hydrolance_pulse_AuraScript);

        bool Load() OVERRIDE
        {
            _tickCount = 0;
            return true;
        }

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            if (!caster)
                return;

            float x, y;
            caster->GetNearPoint2D(x, y, _tickCount * 2.0f, caster->GetOrientation());
            caster->CastSpell(x, y, caster->GetPositionZ(), SPELL_HYDROLANCE_PULSE, false);
            ++_tickCount;
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* owner = GetOwner()->ToCreature())
                if (owner->IsAIEnabled)
                    owner->AI()->DoAction(ACTION_HYDROLANCE);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_wise_hydrolance_pulse_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectRemove += AuraEffectRemoveFn(spell_wise_hydrolance_pulse_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }

        uint8 _tickCount;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_wise_hydrolance_pulse_AuraScript();
    }
};

// 106055 - Hydrolance
class spell_wise_hydrolance : public SpellScriptLoader
{
public:
    spell_wise_hydrolance() : SpellScriptLoader("spell_wise_hydrolance") { }

    class spell_wise_hydrolance_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_wise_hydrolance_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_HYDROLANCE_PULSE_AURA, false);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_wise_hydrolance_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_wise_hydrolance_SpellScript();
    }
};

// 106612 - Bubble Burst
class spell_wise_bubble_brust : public SpellScriptLoader
{
public:
    spell_wise_bubble_brust() : SpellScriptLoader("spell_wise_bubble_brust") { }

    class spell_wise_bubble_brust_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_wise_bubble_brust_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
                if (caster->IsAIEnabled)
                    caster->GetAI()->DoAction(ACTION_WASH_AWAY);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_wise_bubble_brust_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_wise_bubble_brust_SpellScript();
    }
};

class CorruptedWatersTargetCheck
{
public:
    bool operator()(WorldObject* object) const
    {
        Player* player = object->ToPlayer();
        if (!player)
            return true;

        Position pos;
        player->GetPosition(&pos);
        float dist = player->GetExactDist(&WiseMariRoomCenter);
        float z = player->GetPositionZ();
        bool inFront = WiseMariRoomCenter.HasInArc(float(M_PI), &pos);

        if (((inFront && dist < 20.0f) || (!inFront && dist < 14.0f)) && z > 174.05f && z < 174.23f)
            return false;

        if (dist < 30.0f && z > 170.19f && z < 170.215f)
            return false;

        return true;
    }
};

// 115167 - Corrupted Waters
class spell_tjs_corrupted_waters_damage : public SpellScriptLoader
{
public:
    spell_tjs_corrupted_waters_damage() : SpellScriptLoader("spell_tjs_corrupted_waters_damage") { }

    class spell_tjs_corrupted_waters_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_tjs_corrupted_waters_damage_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(CorruptedWatersTargetCheck());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tjs_corrupted_waters_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tjs_corrupted_waters_damage_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_tjs_corrupted_waters_damage_SpellScript();
    }
};

// 106653 - Sha Residue
class spell_tjs_sha_residue : public SpellScriptLoader
{
public:
    spell_tjs_sha_residue() : SpellScriptLoader("spell_tjs_sha_residue") { }

    class spell_tjs_sha_residue_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tjs_sha_residue_AuraScript);

        void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& /*isPeriodic*/, int32& amplitude)
        {
            amplitude = 1 * IN_MILLISECONDS;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_tjs_sha_residue_AuraScript::CalcPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_tjs_sha_residue_AuraScript();
    }
};

// 106331 - Wash Away
class spell_wash_away_tjs : public SpellScriptLoader
{
public:
    spell_wash_away_tjs() : SpellScriptLoader("spell_wash_away_tjs") { }

    class spell_wash_away_tjs_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_wash_away_tjs_AuraScript);

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            if (Unit* owner = GetUnitOwner())
                owner->SetFacingTo(Position::NormalizeOrientation(owner->GetOrientation() + float(M_PI) / 40.0f));
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_wash_away_tjs_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_wash_away_tjs_AuraScript();
    }
};

void AddSC_boss_wise_mari()
{
    new boss_wase_mari();
    new npc_corrupt_living_water();
    new npc_wise_stalker();
    new spell_wise_hydrolance_pulse();
    new spell_wise_hydrolance();
    new spell_wise_bubble_brust();
    new spell_tjs_corrupted_waters_damage();
    new spell_tjs_sha_residue();
    new spell_wash_away_tjs();
}
