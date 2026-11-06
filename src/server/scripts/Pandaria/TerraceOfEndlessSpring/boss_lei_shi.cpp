/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"
#include "GameObject.h"
#include "HostileRefManager.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "AreaTrigger.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

#include <list>

static const Position aLeiWps[] =
{
    { -1018.36f, -2880.54f, 19.64f, 4.71f },
    { -1018.20f, -2851.89f, 38.09f, 1.56f },
    { -1017.8f,  -2771.9f,  38.09f, 4.71f }
};

enum LeiShiSpells
{
    SPELL_AFRAID              = 123181,
    SPELL_SPRAY               = 123121,
    SPELL_HIDE                = 123244,
    SPELL_HIDE_SUMMON         = 123213,
    SPELL_HIDE_STACKS         = 123233,
    SPELL_GET_AWAY            = 123461,
    SPELL_GET_AWAY_TELEPORT   = 123441,
    SPELL_PROTECT             = 123250,
    SPELL_PROTECT_RESPAWN     = 123493,
    SPELL_PROTECT_VISUAL       = 123505,
    SPELL_CLOUDED_REFLECTION  = 123620,
    SPELL_CLOUDED_IMPACT      = 123625,
    SPELL_SCARY_FOG_CIRCLE    = 123797,
    SPELL_SCARY_FOG_DOT       = 123705,
    SPELL_SCARY_FOG_STACKS    = 123712,
    SPELL_LEI_SHI_TRANSFORM   = 127535,
    SPELL_PARASITIC_CLUTCH    = 125652,
    SPELL_FACE_CLUTCHERS      = 125724,
    SPELL_PLAYER_VEHICLE_AURA = 125697,
    SPELL_LEI_SHI_KILL_CREDIT = 123903
};

enum LeiShiEvents
{
    EVENT_SPECIAL,
    EVENT_SPRAY,
    EVENT_GET_AWAY,
    EVENT_GET_AWAY_CAST,
    EVENT_PROTECT,
    EVENT_HIDE,
    EVENT_HIDDEN_SPRAY,
    EVENT_BERSERK,
    EVENT_WP_1,
    EVENT_WP_2,
    EVENT_WP_3,
    EVENT_CHECK_WIPE = 12
};

enum LeiShiParasitoidEvents
{
    EVENT_SUMMON_PARASITOID = 1,
    EVENT_ENFORCE_PARASITOID_LEFT,
    EVENT_ENFORCE_PARASITOID_RIGHT
};

enum LeiShiActions
{
    ACTION_ANIMATED_PROTECTOR_DIED,
    ACTION_ACTIVATE_ANIMATED_PROTECTORS,
    ACTION_TERMINATE_GET_AWAY_PHASE,
    ACTION_TERMINATE_HIDE_PHASE,
    ACTION_DISPERSE
};

enum LeiShiSays
{
    TALK_AGGRO = 1,
    TALK_HIDE,
    TALK_GET_AWAY,
    TALK_SLAY,
    TALK_INTRO,
    TALK_DEFEATED,
    TALK_OUTRO_1,
    TALK_OUTRO_2
};

enum ReflectionSays
{
    EMOTE_RIPPLE,
    EMOTE_RIPPLE_2,
    EMOTE_APPEAR
};

Position leiShiPos = { -1017.93f, -2911.3f, 19.902f, 4.74f };

Position hidePositions[4] =
{
    { -990.678f,  -2890.043f, 19.172f, 3.781f },
    { -994.835f,  -2933.835f, 19.172f, 2.377f },
    { -1041.316f, -2932.84f,  19.172f, 0.733f },
    { -1045.101f, -2890.742f, 19.172f, 5.646f }
};

#define HEROIC_DIST_TO_VORTEX 21.0f
#define DIST_TO_SCARY_FOG_DOT 4.5f

enum
{
    NPC_PARASITOID_SHA        = 64346,
    PARASITOID_DATA_LOCATION  = 1,
    PARASITOID_LOCATION_LEFT  = 0,
    PARASITOID_LOCATION_RIGHT = 1,
    PARASITOID_LOCATION_MAX   = 2
};

class boss_lei_shi : public CreatureScript
{
public:
    boss_lei_shi() : CreatureScript("boss_lei_shi") { }

    struct boss_lei_shiAI : public BossAI
    {
        boss_lei_shiAI(Creature* creature) : BossAI(creature, DATA_LEI_SHI),
            leiShiFreed(false), getAwayHealthPct(0.0f)
        {
            me->SetReactState(REACT_PASSIVE);
            SetCombatMovement(false);
        }

        EventMap m_mEvents;
        EventMap wipeEvents;
        EventMap parasitoidEvents;
        uint8 nextAfraidPct;
        uint8 nextProtectPct;
        uint8 endCombatPct;
        std::list<uint64> animatedProtectors;
        bool hidden;
        bool shielded;
        bool getAwayPhase;
        bool protectScheduled;
        bool leiShiFreed;
        bool nextSpecial;
        float getAwayHealthPct;

        void RemoveBloodlustDebuffs()
        {
            if (!instance)
                return;

            instance->DoRemoveAurasDueToSpellOnPlayers(95809);
            instance->DoRemoveAurasDueToSpellOnPlayers(80354);
            instance->DoRemoveAurasDueToSpellOnPlayers(57723);
            instance->DoRemoveAurasDueToSpellOnPlayers(57724);
        }

        void Reset() OVERRIDE
        {
            _Reset();

            summons.DespawnAll();
            wipeEvents.Reset();
            parasitoidEvents.Reset();

            hidden = false;
            getAwayPhase = false;
            shielded = false;
            protectScheduled = false;
            nextSpecial = bool(urand(0, 1));

            nextAfraidPct = 90;
            nextProtectPct = 80;
            endCombatPct = 2;

            if (instance && instance->GetData(TYPE_LEI_INTRO) == DONE)
            {
                if (instance->GetData(DATA_LEI_SHI) == NOT_STARTED)
                    instance->SetData(TYPE_LEI_INTRO, DONE);
                else
                {
                    me->RestoreDisplayId();
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            if (instance)
            {
                if (instance->GetData(TYPE_LEI_INTRO) == DONE)
                {
                    if (instance->GetData(DATA_LEI_SHI) == NOT_STARTED)
                        instance->SetData(TYPE_LEI_INTRO, DONE);
                    else
                    {
                        me->RestoreDisplayId();
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    }
                }
                else if (instance->GetData(TYPE_TSULONG) == DONE)
                {
                    me->m_Events.Schedule(2000, 10, [this]()
                    {
                        instance->SetData(TYPE_LEI_INTRO, SPECIAL);
                    });
                }
            }

            me->RemoveAura(SPELL_AFRAID);
            me->RemoveAura(SPELL_HIDE);
            me->RemoveAura(SPELL_HIDE_STACKS);

            if (Creature* reflection = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_REFLECTION_OF_LEI_SHI) : 0))
                reflection->RemoveAura(SPELL_SCARY_FOG_CIRCLE);

            me->RemoveAura(SPELL_SCARY_FOG_DOT);
            me->RemoveAura(SPELL_SCARY_FOG_STACKS);
            me->RemoveAllAreasTrigger();

            events.Reset();
            animatedProtectors.clear();
            HandleProtectors(false);

            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_CIRCLE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_DOT);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_STACKS);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPRAY);
                RemoveBloodlustDebuffs();
            }

            if (leiShiFreed)
                me->CastSpell(me, SPELL_LEI_SHI_TRANSFORM, true);

            if (IsHeroic())
                if (Creature* reflection = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_REFLECTION_OF_LEI_SHI) : 0))
                    reflection->RemoveAurasDueToSpell(SPELL_SCARY_FOG_CIRCLE);
        }

        void ScheduleSpecialSpell(bool first = true)
        {
            events.ScheduleEvent(nextSpecial ? EVENT_GET_AWAY : EVENT_HIDE,
                first ? 30 * IN_MILLISECONDS : 50 * IN_MILLISECONDS);
            nextSpecial = !nextSpecial;
        }

        void JustReachedHome() OVERRIDE
        {
            _JustReachedHome();

            if (instance)
                instance->SetBossState(DATA_LEI_SHI, FAIL);
        }

        void MovementInform(uint32 type, uint32 pointId) OVERRIDE
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (pointId == 45)
            {
                me->SetFacingTo(4.71f);
                m_mEvents.ScheduleEvent(EVENT_WP_2, 500);
            }

            if (pointId == 47)
            {
                if (Creature* sha = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_SHA_OF_FEAR) : 0))
                    if (sha->AI())
                        sha->AI()->DoAction(ACTION_SHA_INTRO);

                me->SetDisplayId(17612);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                events.Reset();
                me->GetMotionMaster()->MoveIdle();
            }
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();

            if (me->GetMap()->GetDifficulty() != DIFFICULTY_25MAN_LFR)
                parasitoidEvents.ScheduleEvent(EVENT_SUMMON_PARASITOID, 20 * IN_MILLISECONDS);

            if (instance)
            {
                me->CastSpell(me, SPELL_AFRAID, true);

                if (AuraEffect* afraid = me->GetAuraEffect(SPELL_AFRAID, EFFECT_0))
                    afraid->ChangeAmount(0);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                if (IsHeroic())
                    if (Creature* reflection = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_REFLECTION_OF_LEI_SHI)))
                        reflection->CastSpell(reflection, SPELL_SCARY_FOG_CIRCLE, true);

                Talk(TALK_AGGRO);
                HandleProtectors();

                events.ScheduleEvent(EVENT_SPRAY, 400);
                events.ScheduleEvent(EVENT_BERSERK, 10 * MINUTE * IN_MILLISECONDS);
                ScheduleSpecialSpell();
                wipeEvents.ScheduleEvent(EVENT_CHECK_WIPE, 3 * IN_MILLISECONDS);
            }

            DoCast(SPELL_PLAYER_VEHICLE_AURA);
        }

        void HandleProtectors(bool apply = true)
        {
            std::list<Creature*> protectors;
            me->GetCreatureListWithEntryInGrid(protectors, NPC_ANIMATED_PROTECTOR, 100.0f);

            for (Creature* protector : protectors)
            {
                if (apply)
                    animatedProtectors.push_back(protector->GetGUID());

                protector->Respawn();
                protector->AddAura(SPELL_PROTECT_RESPAWN, protector);
                protector->RemoveAura(SPELL_PROTECT_VISUAL);
                protector->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE |
                    UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
            }
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(TALK_SLAY);
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            if (!instance)
                return;

            if (nextAfraidPct)
            {
                if (me->HealthBelowPctDamaged(nextAfraidPct, damage))
                {
                    if (AuraEffect* afraid = me->GetAuraEffect(SPELL_AFRAID, EFFECT_0))
                        afraid->ChangeAmount(afraid->GetAmount() + 8);

                    nextAfraidPct -= 10;
                }

                if (me->HealthBelowPctDamaged(int32(getAwayHealthPct - 4.0f), damage))
                    me->RemoveAura(SPELL_GET_AWAY);

                if (nextProtectPct >= 20)
                {
                    if (me->HealthBelowPctDamaged(nextProtectPct, damage) && !protectScheduled)
                    {
                        protectScheduled = true;
                        events.ScheduleEvent(EVENT_PROTECT, 100);
                    }
                }
            }

            if (me->HealthBelowPctDamaged(endCombatPct, damage))
            {
                damage = 0;

                if (leiShiFreed)
                    return;

                // The credit spell is not in the 5.4.8 client; its criteria name it as their spell target.
                if (instance)
                    instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_LEI_SHI_KILL_CREDIT);
                // Personal loot and currency rewards are not implemented here.
                HandleAchievement();
                DespawnParasitoids();

                summons.DespawnAll();
                instance->SetBossState(DATA_LEI_SHI, DONE);

                if (IsHeroic())
                    if (Creature* reflection = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_REFLECTION_OF_LEI_SHI)))
                        reflection->RemoveAurasDueToSpell(SPELL_SCARY_FOG_CIRCLE);

                events.Reset();
                wipeEvents.Reset();
                parasitoidEvents.Reset();

                EnterEvadeMode();

                me->CombatStop();
                me->getHostileRefManager().deleteReferences();
                me->DeleteThreatList();
                me->setFaction(35);
                me->CastSpell(me, SPELL_LEI_SHI_TRANSFORM, true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->RestoreDisplayId();
                me->RemoveAura(SPELL_AFRAID);
                me->RemoveAura(SPELL_HIDE);
                me->RemoveAura(SPELL_HIDE_STACKS);

                if (Creature* reflection = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_REFLECTION_OF_LEI_SHI)))
                    reflection->RemoveAura(SPELL_SCARY_FOG_CIRCLE);

                me->RemoveAura(SPELL_SCARY_FOG_DOT);
                me->RemoveAura(SPELL_SCARY_FOG_STACKS);
                me->RemoveAura(SPELL_BERSERK);
                me->RemoveAllAreasTrigger();
                me->setRegeneratingHealth(false);

                if (attacker && attacker->GetTypeId() == TypeID::TYPEID_PLAYER)
                    me->GetMap()->ToInstanceMap()->PermBindAllPlayers(attacker->ToPlayer());
                else if (attacker && attacker->GetTypeId() == TypeID::TYPEID_UNIT &&
                    attacker->GetOwner() && attacker->GetOwner()->ToPlayer())
                    me->GetMap()->ToInstanceMap()->PermBindAllPlayers(attacker->GetOwner()->ToPlayer());

                leiShiFreed = true;
                Talk(TALK_DEFEATED);

                m_mEvents.ScheduleEvent(EVENT_WP_1, 15000);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_CIRCLE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_DOT);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_STACKS);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPRAY);
                RemoveBloodlustDebuffs();

                me->RemoveAura(SPELL_PLAYER_VEHICLE_AURA);
            }
        }

        void EnterEvadeMode() OVERRIDE
        {
            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                RemoveBloodlustDebuffs();

                if (instance->GetBossState(DATA_LEI_SHI) == DONE)
                    return;

                instance->SetBossState(DATA_LEI_SHI, FAIL);
            }

            _EnterEvadeMode();
            wipeEvents.Reset();
            parasitoidEvents.Reset();
            summons.DespawnAll();
            DespawnParasitoids();

            _DespawnAtEvade();
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) OVERRIDE
        {
            if (!hidden || !caster)
                return;

            if (caster->GetTypeId() == TypeID::TYPEID_UNIT &&
                (caster->GetEntry() == NPC_LEI_SHI || caster->GetEntry() == NPC_LEI_SHI_HIDDEN))
                return;

            if (!spell)
                return;

            if (!spell->HasEffect(SPELL_EFFECT_SCHOOL_DAMAGE) &&
                !spell->HasEffect(SPELL_EFFECT_NORMALIZED_WEAPON_DMG) &&
                !spell->HasEffect(SPELL_EFFECT_WEAPON_DAMAGE) &&
                !spell->HasEffect(SPELL_EFFECT_WEAPON_PERCENT_DAMAGE) &&
                !spell->HasEffect(SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL))
                return;

            if (Aura* hide = me->GetAura(SPELL_HIDE_STACKS))
                hide->ModCharges(-1);
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            switch (actionId)
            {
                case ACTION_ANIMATED_PROTECTOR_DIED:
                {
                    me->RemoveAura(SPELL_PROTECT);
                    shielded = false;
                    protectScheduled = false;
                    nextProtectPct -= 20;

                    Creature* protector = NULL;

                    for (uint64 guid : animatedProtectors)
                    {
                        if (Creature* elemental = me->GetMap()->GetCreature(guid))
                        {
                            if (!elemental->IsAlive())
                                protector = elemental;

                            elemental->RemoveAura(SPELL_PROTECT_VISUAL);
                            elemental->AddAura(SPELL_PROTECT_RESPAWN, elemental);
                            elemental->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE |
                                UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                        }
                    }

                    if (protector)
                        animatedProtectors.remove(protector->GetGUID());

                    break;
                }
                case ACTION_ACTIVATE_ANIMATED_PROTECTORS:
                {
                    if (animatedProtectors.empty())
                        break;

                    me->CastSpell(me, SPELL_PROTECT, false);
                    shielded = true;
                    uint8 protectorsActivated = 0;

                    for (uint64 guid : animatedProtectors)
                    {
                        if (((me->GetMap()->IsHeroic() && protectorsActivated >= 4) ||
                            (!me->GetMap()->IsHeroic() && protectorsActivated >= 3)) &&
                            nextProtectPct > 40)
                            break;

                        if (((me->GetMap()->IsHeroic() && protectorsActivated >= 5) ||
                            (!me->GetMap()->IsHeroic() && protectorsActivated >= 4)) &&
                            nextProtectPct <= 40)
                            break;

                        if (Creature* protector = me->GetMap()->GetCreature(guid))
                        {
                            protector->SetHealth(protector->GetMaxHealth());
                            protector->RemoveAura(SPELL_PROTECT_RESPAWN);
                            protector->CastSpell(protector, SPELL_PROTECT_VISUAL, true);
                            protector->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE |
                                UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);

                            if (protector->AI())
                                protector->AI()->DoZoneInCombat();

                            ++protectorsActivated;
                        }
                    }

                    break;
                }
                case ACTION_TERMINATE_GET_AWAY_PHASE:
                {
                    getAwayPhase = false;
                    me->RemoveAllAreasTrigger();
                    ScheduleSpecialSpell(false);
                    break;
                }
                case ACTION_TERMINATE_HIDE_PHASE:
                {
                    me->RemoveAura(SPELL_HIDE);
                    me->RemoveAura(SPELL_HIDE_STACKS);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->RestoreDisplayId();
                    hidden = false;

                    if (IsHeroic())
                        if (Creature* reflection = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_REFLECTION_OF_LEI_SHI) : 0))
                            reflection->CastSpell(me, SPELL_SCARY_FOG_CIRCLE, true);

                    summons.DespawnAll();
                    ScheduleSpecialSpell(false);
                    break;
                }
                case ACTION_LEISHI_INTRO:
                {
                    me->SetDisplayId(43783);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE |
                        UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                    me->SetReactState(REACT_AGGRESSIVE);
                    Talk(TALK_INTRO);

                    if (Creature* reflection = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_REFLECTION_OF_LEI_SHI) : 0))
                    {
                        reflection->SetDisplayId(17612);
                        reflection->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE |
                            UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                    }

                    if (GameObject* wall = ObjectAccessor::GetGameObject(*me, instance ? instance->GetData64(GO_WALL_OF_LEI_SHI) : 0))
                        wall->SetGoState(GOState::GO_STATE_ACTIVE);

                    break;
                }
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            m_mEvents.Update(diff);

            switch (m_mEvents.ExecuteEvent())
            {
                case EVENT_WP_1:
                    Talk(TALK_OUTRO_1);
                    me->SetWalk(true);
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(45, aLeiWps[0]);
                    break;
                case EVENT_WP_2:
                    Talk(TALK_OUTRO_2);
                    me->GetMotionMaster()->MovePoint(46, aLeiWps[1]);
                    m_mEvents.ScheduleEvent(EVENT_WP_3, 8000);
                    break;
                case EVENT_WP_3:
                    me->SetWalk(false);
                    me->GetMotionMaster()->MovePoint(47, aLeiWps[2]);
                    break;
            }

            wipeEvents.Update(diff);

            while (uint32 eventId = wipeEvents.ExecuteEvent())
            {
                if (eventId == EVENT_CHECK_WIPE)
                {
                    if (instance && instance->IsWipe(50.0f, me))
                    {
                        events.Reset();
                        EnterEvadeMode();
                        break;
                    }

                    wipeEvents.ScheduleEvent(EVENT_CHECK_WIPE, IN_MILLISECONDS);
                }

                break;
            }

            if (!UpdateVictim())
                return;

            if (hidden)
                return;

            events.Update(diff);
            parasitoidEvents.Update(diff);

            while (uint32 eventId = parasitoidEvents.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_PARASITOID:
                        parasitoidEvents.ScheduleEvent(EVENT_SUMMON_PARASITOID,
                            TryToSummonParasitoid(PARASITOID_LOCATION_MAX) ?
                            15 * IN_MILLISECONDS : IN_MILLISECONDS);
                        break;
                    case EVENT_ENFORCE_PARASITOID_LEFT:
                        TryToSummonParasitoid(PARASITOID_LOCATION_LEFT);
                        break;
                    case EVENT_ENFORCE_PARASITOID_RIGHT:
                        TryToSummonParasitoid(PARASITOID_LOCATION_RIGHT);
                        break;
                }
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        break;
                    case EVENT_SPRAY:
                    {
                        if (getAwayPhase || me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.ScheduleEvent(EVENT_SPRAY, 400);
                            break;
                        }

                        if (Unit* target = me->GetVictim())
                            me->CastSpell(target, SPELL_SPRAY, false);

                        events.ScheduleEvent(EVENT_SPRAY, 400);
                        break;
                    }
                    case EVENT_GET_AWAY:
                    {
                        if (shielded)
                        {
                            events.ScheduleEvent(EVENT_GET_AWAY, IN_MILLISECONDS);
                            break;
                        }

                        me->InterruptNonMeleeSpells(true);
                        Talk(TALK_GET_AWAY);
                        me->CastSpell(me, SPELL_GET_AWAY_TELEPORT, true);
                        getAwayPhase = true;
                        getAwayHealthPct = me->GetHealthPct();
                        events.ScheduleEvent(EVENT_GET_AWAY_CAST, 300);
                        break;
                    }
                    case EVENT_GET_AWAY_CAST:
                        me->CastSpell(me, SPELL_GET_AWAY, false);
                        break;
                    case EVENT_PROTECT:
                    {
                        if (getAwayPhase)
                        {
                            events.RescheduleEvent(EVENT_PROTECT, IN_MILLISECONDS);
                            break;
                        }

                        me->InterruptNonMeleeSpells(true);
                        DoAction(ACTION_ACTIVATE_ANIMATED_PROTECTORS);
                        break;
                    }
                    case EVENT_HIDE:
                    {
                        if (shielded)
                        {
                            events.RescheduleEvent(EVENT_HIDE, IN_MILLISECONDS);
                            break;
                        }

                        me->InterruptNonMeleeSpells(true);

                        if (Creature* reflection = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_REFLECTION_OF_LEI_SHI) : 0))
                            reflection->RemoveAurasDueToSpell(SPELL_SCARY_FOG_CIRCLE);

                        Talk(TALK_HIDE);
                        me->CastSpell(me, SPELL_HIDE, false);
                        hidden = true;
                        break;
                    }
                }
            }
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            if (summon->GetEntry() != NPC_PARASITOID_SHA)
            {
                summons.Summon(summon);
                if (me->IsInCombat())
                    DoZoneInCombat(summon);
            }
        }

        bool TryToSummonParasitoid(uint32 preferredLocation)
        {
            bool locationBusy[PARASITOID_LOCATION_MAX] = {};

            for (TempSummon* summon : me->GetSummons())
            {
                if (summon->GetEntry() == NPC_PARASITOID_SHA)
                    if (!summon->GetVehicle() && summon->AI()->GetData(1) < 2)
                        locationBusy[summon->AI()->GetData(PARASITOID_DATA_LOCATION)] = true;
            }

            if (locationBusy[0] && locationBusy[1])
                return false;

            uint32 location = preferredLocation == PARASITOID_LOCATION_MAX ?
                urand(PARASITOID_LOCATION_LEFT, PARASITOID_LOCATION_RIGHT) : preferredLocation;

            if (locationBusy[location])
            {
                for (uint32 i = 0; i < PARASITOID_LOCATION_MAX; ++i)
                    if (!locationBusy[i])
                        location = i;
            }

            SummonParasitoid(location);
            return true;
        }

        void SummonParasitoid(uint32 location)
        {
            Position const pos[PARASITOID_LOCATION_MAX] =
            {
                { -1056.78f, -2950.57f, 2.29f, 5.63f },
                { -979.63f,  -2949.90f, 2.29f, 3.70f }
            };

            if (Creature* parasitoid = me->SummonCreature(NPC_PARASITOID_SHA, pos[location]))
                parasitoid->AI()->SetData(PARASITOID_DATA_LOCATION, location);
        }

        void EnforceParasitoidSpawn(uint32 parasitoidSpawnLocation)
        {
            parasitoidEvents.ScheduleEvent(
                parasitoidSpawnLocation == PARASITOID_LOCATION_LEFT ?
                    EVENT_ENFORCE_PARASITOID_LEFT : EVENT_ENFORCE_PARASITOID_RIGHT,
                5 * IN_MILLISECONDS);
        }

        void DespawnParasitoids()
        {
            std::list<TempSummon*> parasitoids;
            me->GetSummons(parasitoids, NPC_PARASITOID_SHA);

            for (TempSummon* parasitoid : parasitoids)
                parasitoid->UnSummon();
        }

        void HandleAchievement()
        {
            if (!instance)
                return;

            for (auto const& player : instance->instance->GetPlayers())
                if (!player.GetSource()->HasAura(SPELL_PARASITIC_CLUTCH))
                    return;

            instance->DoUpdateAchievementCriteria(
                ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 125724, 0, me);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_lei_shiAI(creature);
    }
};

class npc_animated_protector : public CreatureScript
{
public:
    npc_animated_protector() : CreatureScript("npc_animated_protector") { }

    struct npc_animated_protectorAI : public ScriptedAI
    {
        npc_animated_protectorAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (Creature* leiShi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_LEI_SHI) : 0))
                leiShi->AI()->DoAction(ACTION_ANIMATED_PROTECTOR_DIED);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_animated_protectorAI(creature);
    }
};

// Corrupted Protector - 63275
class npc_lei_shi_corrupted_protector : public CreatureScript
{
public:
    npc_lei_shi_corrupted_protector() : CreatureScript("npc_lei_shi_corrupted_protector") { }

    enum Spells
    {
        SPELL_DISPERSE = 123610
    };

    enum Events
    {
        EVENT_DISPERSE = 1
    };

    struct npc_lei_shi_corrupted_protectorAI : public ScriptedAI
    {
        npc_lei_shi_corrupted_protectorAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;
        bool InFlee;

        void Reset() OVERRIDE
        {
            me->GetMotionMaster()->MoveRandom(1.5f);
            me->RemoveAurasDueToSpell(SPELL_DISPERSE);
            InFlee = false;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_DISPERSE, 1000);
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            if (actionId == ACTION_DISPERSE)
                events.ScheduleEvent(EVENT_DISPERSE, 2500);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_DISPERSE)
                {
                    me->RemoveAurasDueToSpell(SPELL_DISPERSE);
                    DoCast(me, SPELL_DISPERSE);
                    me->ClearUnitState(UNIT_STATE_CASTING);
                }
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lei_shi_corrupted_protectorAI(creature);
    }
};

// Lei Shi (hidden) - 63099
class npc_lei_shi_hidden : public CreatureScript
{
    enum Spells
    {
        SPELL_HIDE_VISUAL  = 132363,
        SPELL_HIDE_PROC    = 123233,
        SPELL_HIDE_TRIGGER = 132362
    };

public:
    npc_lei_shi_hidden() : CreatureScript("npc_lei_shi_hidden") { }

    struct npc_lei_shi_hiddenAI : public ScriptedAI
    {
        npc_lei_shi_hiddenAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void SpellHitTarget(Unit* target, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == SPELL_HIDE_TRIGGER)
                target->CastSpell(target, SPELL_HIDE_VISUAL, true);
        }

        void Reset() OVERRIDE
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            events.Reset();
            events.ScheduleEvent(EVENT_HIDDEN_SPRAY, 400);

            if (Creature* leiShi = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(NPC_LEI_SHI) : 0))
            {
                AuraEffect const* afraid = leiShi->GetAuraEffect(SPELL_AFRAID, EFFECT_0);
                if (!afraid)
                    return;

                if (!me->HasAura(SPELL_AFRAID))
                {
                    if (Aura* newAfraid = me->AddAura(SPELL_AFRAID, me))
                        if (newAfraid->GetEffect(EFFECT_0))
                            newAfraid->GetEffect(EFFECT_0)->ChangeAmount(afraid->GetAmount());
                }
                else
                {
                    if (AuraEffect* newAfraid = me->GetAuraEffect(SPELL_AFRAID, EFFECT_0))
                        newAfraid->ChangeAmount(afraid->GetAmount());
                }
            }

            DoCast(me, SPELL_HIDE_PROC, true);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_HIDDEN_SPRAY:
                {
                    if (me->HasUnitState(UNIT_STATE_CASTING))
                    {
                        events.ScheduleEvent(EVENT_HIDDEN_SPRAY, 0);
                        break;
                    }

                    uint64 leiShiGuid = instance ? instance->GetData64(NPC_LEI_SHI) : 0;

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f))
                        me->CastSpell(target, SPELL_SPRAY, false, NULL, NULL, leiShiGuid);

                    events.ScheduleEvent(EVENT_HIDDEN_SPRAY, 400);
                    break;
                }
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lei_shi_hiddenAI(creature);
    }
};

class npc_lei_shi_reflection : public CreatureScript
{
    enum Events
    {
        EVENT_NONE,
        EVENT_RIPPLE_1,
        EVENT_RIPPLE_2,
        EVENT_APPEAR
    };

public:
    npc_lei_shi_reflection() : CreatureScript("npc_lei_shi_reflection") { }

    struct npc_lei_shi_reflectionAI : public ScriptedAI
    {
        npc_lei_shi_reflectionAI(Creature* creature) : ScriptedAI(creature)
        {
            eventDone = false;
        }

        EventMap events;
        bool eventDone;

        void Reset() OVERRIDE
        {
            if (IsHeroic())
                me->RemoveAurasDueToSpell(SPELL_SCARY_FOG_CIRCLE);
        }

        void DoAction(int32 actionId) OVERRIDE
        {
            if (actionId == ACTION_LEISHI_INTRO && !eventDone)
            {
                eventDone = true;
                me->ClearUnitState(UNIT_STATE_STUNNED);
                events.ScheduleEvent(EVENT_RIPPLE_1, 2000);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RIPPLE_1:
                        DoCast(SPELL_CLOUDED_IMPACT);
                        Talk(EMOTE_RIPPLE);
                        events.ScheduleEvent(EVENT_RIPPLE_2, 14000);
                        break;
                    case EVENT_RIPPLE_2:
                        DoCast(SPELL_CLOUDED_IMPACT);
                        Talk(EMOTE_RIPPLE_2);
                        events.ScheduleEvent(EVENT_APPEAR, 10000);
                        break;
                    case EVENT_APPEAR:
                        if (Creature* leiShi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_LEI_SHI) : 0))
                        {
                            Talk(EMOTE_APPEAR, leiShi);

                            if (leiShi->AI())
                                leiShi->AI()->DoAction(ACTION_LEISHI_INTRO);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lei_shi_reflectionAI(creature);
    }
};

// Parasitoid Sha - 64346
struct npc_parasitoid_sha : public ScriptedAI
{
    npc_parasitoid_sha(Creature* creature) : ScriptedAI(creature) { }

    uint32 spawnLocation = PARASITOID_LOCATION_MAX;

    void SetData(uint32 type, uint32 data) OVERRIDE
    {
        if (type == PARASITOID_DATA_LOCATION)
            spawnLocation = data;
    }

    uint32 GetData(uint32 type) const OVERRIDE
    {
        if (type == PARASITOID_DATA_LOCATION)
            return spawnLocation;
        return 0;
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE
    {
        if (me->GetVehicle())
            return;

        if (!UpdateVictim())
            return;

        std::list<Player*> players;
        me->GetPlayerListInGrid(players, 1.0f);

        players.remove_if([](Player* player)
        {
            return player->HasAuraType(SPELL_AURA_CONTROL_VEHICLE);
        });

        if (!players.empty())
        {
            DoCast(*players.begin(), SPELL_PARASITIC_CLUTCH);
            if (me->GetInstanceScript())
                if (Creature* leiShi = ObjectAccessor::GetCreature(*me, me->GetInstanceScript()->GetData64(NPC_LEI_SHI)))
                    if (boss_lei_shi::boss_lei_shiAI* ai = dynamic_cast<boss_lei_shi::boss_lei_shiAI*>(leiShi->AI()))
                        ai->EnforceParasitoidSpawn(spawnLocation);
        }
    }
};

// Get Away! - 123461
class spell_get_away : public SpellScriptLoader
{
public:
    spell_get_away() : SpellScriptLoader("spell_get_away") { }

    class spell_get_away_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_get_away_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->ToCreature() && caster->ToCreature()->AI())
                    caster->ToCreature()->AI()->DoAction(ACTION_TERMINATE_GET_AWAY_PHASE);
        }

        void Register() OVERRIDE
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_get_away_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_get_away_AuraScript();
    }
};

// Hide - 123244
class spell_hide : public SpellScriptLoader
{
public:
    spell_hide() : SpellScriptLoader("spell_hide") { }

    class spell_hide_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hide_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                uint8 pos = urand(0, 3);

                caster->NearTeleportTo(hidePositions[pos].GetPositionX(),
                    hidePositions[pos].GetPositionY(),
                    hidePositions[pos].GetPositionZ(),
                    hidePositions[pos].GetOrientation());

                caster->CastSpell(caster, SPELL_HIDE_STACKS, true);
                caster->CastSpell(caster, SPELL_HIDE_SUMMON, true);

                caster->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                caster->SetDisplayId(11686);
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                caster->RemoveAura(SPELL_HIDE_STACKS);
                caster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                caster->RestoreDisplayId();
            }
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hide_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_hide_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hide_AuraScript();
    }
};

// Get Away damage - 123467
class spell_get_away_dmg : public SpellScriptLoader
{
public:
    spell_get_away_dmg() : SpellScriptLoader("spell_get_away_dmg") { }

    class spell_get_away_dmg_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_get_away_dmg_SpellScript);

        void ModifyDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Player* victim = GetHitPlayer();

            if (!caster || !victim)
                return;

            if (victim->isMoving() && !victim->m_movementInfo.HasMovementFlag(MOVEMENTFLAG_BACKWARD) && victim->isInFront(caster))
                SetHitDamage(GetHitDamage() / 2);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_get_away_dmg_SpellScript::ModifyDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_get_away_dmg_SpellScript();
    }
};

// Hide stacks - 123233
class spell_hide_stacks : public SpellScriptLoader
{
public:
    spell_hide_stacks() : SpellScriptLoader("spell_hide_stacks") { }

    class spell_hide_stacks_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hide_stacks_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->ToCreature() && caster->ToCreature()->AI())
                    caster->ToCreature()->AI()->DoAction(ACTION_TERMINATE_HIDE_PHASE);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_hide_stacks_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hide_stacks_AuraScript();
    }
};

// Scary Fog damage over time - 123705
class spell_scary_fog_dot : public SpellScriptLoader
{
public:
    spell_scary_fog_dot() : SpellScriptLoader("spell_scary_fog_dot") { }

    class spell_scary_fog_dot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_scary_fog_dot_SpellScript);

        void CorrectRange(std::list<WorldObject*>& targets)
        {
            targets.clear();

            if (Unit* caster = GetCaster())
            {
                Map::PlayerList const& players = caster->GetMap()->GetPlayers();

                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->GetSource())
                    {
                        if (player->GetExactDist2d(caster->GetPositionX(), caster->GetPositionY()) >= HEROIC_DIST_TO_VORTEX)
                            targets.push_back(player);
                    }
                }

                for (WorldObject* target : targets)
                {
                    if (Unit* unit = target->ToUnit())
                    {
                        if (Aura* scaryFog = caster->AddAura(SPELL_SCARY_FOG_STACKS, unit))
                        {
                            scaryFog->SetDuration(35000);
                            scaryFog->SetMaxDuration(35000);
                        }
                    }
                }
            }
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_scary_fog_dot_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_scary_fog_dot_SpellScript();
    }

    class spell_scary_fog_dot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_scary_fog_dot_AuraScript);

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* target = GetTarget())
                target->CastSpell(target, SPELL_SCARY_FOG_STACKS, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_scary_fog_dot_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_scary_fog_dot_AuraScript();
    }
};

// Scary Fog stacks - 123712
class spell_scary_fog_stacks : public SpellScriptLoader
{
public:
    spell_scary_fog_stacks() : SpellScriptLoader("spell_scary_fog_stacks") { }

    class spell_scary_fog_stacks_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_scary_fog_stacks_SpellScript);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            for (WorldObject* target : targets)
            {
                if (target->ToUnit() && target->ToUnit()->GetEntry() != NPC_LEI_SHI_HIDDEN)
                {
                    if (Unit* caster = GetCaster())
                    {
                        if (Aura* scary = caster->GetAura(SPELL_SCARY_FOG_STACKS))
                        {
                            if (Aura* scaryTarget = target->ToUnit()->GetAura(SPELL_SCARY_FOG_STACKS))
                                scaryTarget->SetStackAmount(scary->GetStackAmount());
                            else if (Aura* scaryTarget = caster->AddAura(SPELL_SCARY_FOG_STACKS, target->ToUnit()))
                                scaryTarget->SetStackAmount(scary->GetStackAmount());
                        }
                    }
                }
            }

            targets.clear();
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_scary_fog_stacks_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_scary_fog_stacks_SpellScript();
    }
};

// Disperse - 123610
class spell_lei_shi_disperse : public SpellScriptLoader
{
public:
    spell_lei_shi_disperse() : SpellScriptLoader("spell_lei_shi_disperse") { }

    class spell_lei_shi_disperse_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_lei_shi_disperse_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
                if (caster->ToCreature() && caster->ToCreature()->AI())
                    caster->ToCreature()->AI()->DoAction(ACTION_DISPERSE);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_lei_shi_disperse_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_lei_shi_disperse_SpellScript();
    }
};

// Player Vehicle Aura - 125697
class spell_player_vehicle_aura : public AuraScript
{
    PrepareAuraScript(spell_player_vehicle_aura);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
    }

    void Register() OVERRIDE
    {
        AfterEffectApply += AuraEffectApplyFn(spell_player_vehicle_aura::HandleApply, EFFECT_0, SPELL_AURA_SET_VEHICLE_ID, AURA_EFFECT_HANDLE_REAL);
    }
};

// Area triggers 408 and 409, summoned by Get Away! - 123461
class sat_leishi_get_away : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering && triggering->ToPlayer() && triggering->ToPlayer()->IsAlive();
    }

    void OnTriggeringUpdate(WorldObject* triggering) OVERRIDE
    {
        if (Player* target = triggering->ToPlayer())
        {
            if (Unit* caster = GetCaster())
            {
                if (target->IsAlive() && !target->HasForcedMovement())
                    target->SendApplyMovementForce(true, *caster, caster->GetMap()->IsHeroic() ? -7.5f : -5.0f);
                else if (!target->IsAlive() && target->HasForcedMovement())
                    target->SendApplyMovementForce(false, *caster);
            }
        }
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* target = triggering->ToPlayer())
            if (Unit* caster = GetCaster())
                if (target->HasForcedMovement())
                    target->SendApplyMovementForce(false, *caster);
    }
};

void AddSC_boss_lei_shi()
{
    new boss_lei_shi();
    new npc_animated_protector();
    new npc_lei_shi_corrupted_protector();
    new npc_lei_shi_hidden();
    new npc_lei_shi_reflection();
    new creature_script<npc_parasitoid_sha>("npc_parasitoid_sha");
    new spell_get_away();
    new spell_get_away_dmg();
    new spell_hide();
    new spell_hide_stacks();
    new spell_scary_fog_dot();
    new spell_scary_fog_stacks();
    new spell_lei_shi_disperse();
    RegisterAuraScript(spell_player_vehicle_aura);
    new atrigger_script<sat_leishi_get_away>("sat_leishi_get_away");
}
