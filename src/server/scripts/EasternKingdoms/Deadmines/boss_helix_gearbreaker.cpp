/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "Map.h"
#include "Vehicle.h"
#include "deadmines.h"

enum ScriptTexts
{
    SAY_AGGRO         = 0,
    SAY_BOMBS         = 1,
    SAY_THROW         = 2,
    SAY_CHEST_BOMB    = 3,
    SAY_OAF_DEAD      = 4,
    SAY_KILL          = 5,
    SAY_DEATH         = 6,

    SAY_OAF_DEATH     = 0,
    SAY_OAF_SMASH     = 1,
};

enum Spells
{
    SPELL_CHEST_BOMB            = 88352,
    SPELL_CHEST_BOMB_DMG        = 88250,

    SPELL_OAF_SMASH             = 88300,
    SPELL_FORCE_PLAYER_RIDE_OAF = 88278,
    SPELL_RIDE_OAF              = 88277,
    SPELL_HELIX_RIDE            = 88337,
    SPELL_RIDE_FACE             = 88360,
};

enum Adds
{
    NPC_STICKY_BOMB = 47314,
    NPC_HELIX_CREW  = 49136,
    NPC_MINE_RAT    = 51462,
};

enum Events
{
    // Helix
    EVENT_STICKY_BOMB = 1,
    EVENT_HELIX_CREW,
    EVENT_THROW_HELIX,
    EVENT_LEAP,
    EVENT_RIDE_FACE,
    EVENT_LEAVE_FACE,
    EVENT_RETURN_TO_OAF,
    EVENT_BOARD_OAF,
    EVENT_CHECK_BOARDED,

    // Lumbering Oaf
    EVENT_OAF_SMASH,
    EVENT_GRAB,
    EVENT_GRAB_FAILED,
    EVENT_RAM,
    EVENT_KNOCKBACK_PLAYER,
    EVENT_SMASH_RECOVER,
};

enum Points
{
    POINT_GRAB  = 1,
    POINT_START = 2,
    POINT_END   = 3,
    POINT_LEAP  = 4,
    POINT_OAF   = 5,
};

enum Actions
{
    ACTION_OAF_DEAD = 1,
};

enum Misc
{
    DATA_HELIX_ON_OAF = 1,
    DATA_OAF_BUSY     = 2,

    SEAT_HELIX        = 0,
    SEAT_PLAYER       = 1,
};

enum HelixState
{
    STATE_ON_OAF,
    STATE_LEAPING,
    STATE_ON_PLAYER,
    STATE_GROUND,
    STATE_RETURNING,
};

const Position lumberingoafPos[3] =
{
    {-301.93f, -516.32f, 51.71f, 2.03f},
    {-289.98f, -528.06f, 49.75f, 1.59f},  // runway start, by the mast room door
    {-289.67f, -488.46f, 49.80f, 1.54f}   // woodpile
};

const Position woodpilePos = {-289.60f, -482.00f, 49.80f, 0.0f};

const Position helixcrewPos[4] =
{
    {-295.26f, -503.38f, 60.16f, 0.0f},
    {-280.85f, -503.98f, 60.16f, 0.0f},
    {-291.45f, -503.37f, 60.16f, 0.0f},
    {-285.63f, -504.04f, 60.16f, 0.0f}
};

static void ApplyBossImmunities(Creature* me)
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
}

// Players that are free to be grabbed by the Oaf or ridden by Helix
struct FreePlayerSelector
{
    explicit FreePlayerSelector(uint64 exclude = 0) : _exclude(exclude) { }

    bool operator()(Unit const* target) const
    {
        return target && target->GetTypeId() == TypeID::TYPEID_PLAYER && target->IsAlive()
            && !target->GetVehicle() && !target->GetVehicleKit() && target->GetGUID() != _exclude;
    }

private:
    uint64 _exclude;
};

// Lobs a sticky bomb from the thrower so it lands on the ground near the target
static void ThrowStickyBomb(Creature* thrower, Unit* target, uint32 despawnTime)
{
    Position dest;
    target->GetRandomPoint(*target, 5.0f, dest);
    thrower->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK_THROWN);
    if (Creature* bomb = thrower->SummonCreature(NPC_STICKY_BOMB, thrower->GetPositionX(), thrower->GetPositionY(), thrower->GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, despawnTime))
        bomb->GetMotionMaster()->MoveJump(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), 20.0f, 8.0f);
}

class boss_helix_gearbreaker : public CreatureScript
{
    public:
        boss_helix_gearbreaker() : CreatureScript("boss_helix_gearbreaker") { }

        struct boss_helix_gearbreakerAI : public BossAI
        {
            boss_helix_gearbreakerAI(Creature* creature) : BossAI(creature, DATA_HELIX)
            {
                state = STATE_ON_OAF;
                phaseTwo = false;
                leapTargetGUID = 0;
                riddenGUID = 0;
                boardAttempts = 0;
            }

            HelixState state;
            bool phaseTwo;
            uint64 leapTargetGUID;
            uint64 riddenGUID;
            uint8 boardAttempts;

            void InitializeAI() OVERRIDE
            {
                ApplyBossImmunities(me);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->setActive(true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                state = STATE_ON_OAF;
                phaseTwo = false;
                leapTargetGUID = 0;
                riddenGUID = 0;
                me->SetReactState(REACT_PASSIVE);
                SetAttackable(false);
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type == DATA_HELIX_ON_OAF)
                    return state == STATE_ON_OAF && me->GetVehicle() ? 1 : 0;
                return 0;
            }

            void SetAttackable(bool attackable)
            {
                if (attackable)
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                else
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            Creature* GetOaf() const
            {
                if (!instance)
                    return NULL;
                return ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_OAF));
            }

            bool OafCanCarry() const
            {
                Creature* oaf = GetOaf();
                return oaf && oaf->IsAlive() && !phaseTwo && oaf->IsVehicle() && oaf->IsAIEnabled && !oaf->AI()->GetData(DATA_OAF_BUSY);
            }

            Unit* SelectFreePlayer(uint64 exclude = 0)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, FreePlayerSelector(exclude)))
                    return target;
                return SelectTarget(SELECT_TARGET_RANDOM, 0, FreePlayerSelector());
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_STICKY_BOMB, 8000);
                events.ScheduleEvent(EVENT_THROW_HELIX, 20000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_HELIX_CREW, 5000);

                Talk(SAY_AGGRO);
                DoZoneInCombat();
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action != ACTION_OAF_DEAD || phaseTwo)
                    return;

                phaseTwo = true;
                Talk(SAY_OAF_DEAD);
                SetAttackable(true);
                DoResetThreat();

                events.CancelEvent(EVENT_THROW_HELIX);
                events.CancelEvent(EVENT_RETURN_TO_OAF);
                events.CancelEvent(EVENT_BOARD_OAF);
                events.RescheduleEvent(EVENT_STICKY_BOMB, 2000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_HELIX_CREW, 1000);

                // Thrown to the ground by the Oaf; leap straight onto someone unless already riding a face
                if (state != STATE_ON_PLAYER && state != STATE_LEAPING)
                {
                    state = STATE_GROUND;
                    me->AttackStop();
                    leapTargetGUID = 0;
                    events.ScheduleEvent(EVENT_LEAP, 1500);
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                BossAI::JustSummoned(summon);

                if (summon->GetEntry() == NPC_HELIX_CREW)
                    DoZoneInCombat(summon);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != EFFECT_MOTION_TYPE)
                    return;

                if (pointId == POINT_LEAP && state == STATE_LEAPING)
                    events.RescheduleEvent(EVENT_RIDE_FACE, 1);
                else if (pointId == POINT_OAF && state == STATE_RETURNING)
                    events.RescheduleEvent(EVENT_BOARD_OAF, 1);
            }

            void LeapTo(Unit* target)
            {
                state = STATE_LEAPING;
                leapTargetGUID = target->GetGUID();
                SetAttackable(true);
                me->AttackStop();
                me->GetMotionMaster()->MoveJump(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 25.0f, 10.0f, POINT_LEAP);
                events.ScheduleEvent(EVENT_RIDE_FACE, 3000);
            }

            void BoardRider(Unit* target)
            {
                if (!target->HasAura(SPELL_HELIX_RIDE))
                    me->AddAura(SPELL_HELIX_RIDE, target);
                DoCast(target, SPELL_RIDE_FACE, true);
            }

            void ReleaseRider(Unit* rider)
            {
                if (me->GetVehicle())
                    me->ExitVehicle();

                if (!rider)
                    return;

                rider->RemoveAurasDueToSpell(SPELL_HELIX_RIDE);
                if (rider->IsAlive())
                {
                    // Chest Bomb only has the heroic damage values in the client data
                    if (IsHeroic())
                    {
                        Talk(SAY_CHEST_BOMB, rider);
                        me->AddAura(SPELL_CHEST_BOMB, rider);
                    }
                    DoModifyThreatPercent(rider, -100);
                }
            }

            void EnterEvadeMode() OVERRIDE
            {
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_HELIX_RIDE);
                    instance->SetBossState(DATA_HELIX, NOT_STARTED);
                }

                summons.DespawnAll();
                me->DespawnOrUnsummon();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_HELIX_RIDE);
                }

                Talk(SAY_DEATH);

                if (Creature* oaf = GetOaf())
                    if (oaf->IsAlive())
                        me->Kill(oaf);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (state == STATE_ON_PLAYER)
                {
                    Unit* rider = ObjectAccessor::GetUnit(*me, riddenGUID);
                    if (!rider || !rider->IsAlive())
                        events.RescheduleEvent(EVENT_LEAVE_FACE, 1);
                }

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STICKY_BOMB:
                            for (uint8 i = 0; i < (phaseTwo ? 2 : 1); ++i)
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                    ThrowStickyBomb(me, target, 40000);
                            events.ScheduleEvent(EVENT_STICKY_BOMB, phaseTwo ? urand(3000, 4000) : urand(8000, 10000));
                            break;
                        case EVENT_HELIX_CREW:
                            Talk(SAY_BOMBS);
                            for (uint8 i = 0; i < 4; ++i)
                                me->SummonCreature(NPC_HELIX_CREW, helixcrewPos[i], TempSummonType::TEMPSUMMON_CORPSE_DESPAWN);
                            break;
                        case EVENT_THROW_HELIX:
                            if (state != STATE_ON_OAF || !OafCanCarry())
                            {
                                events.ScheduleEvent(EVENT_THROW_HELIX, 3000);
                                break;
                            }
                            if (Unit* target = SelectFreePlayer())
                            {
                                Talk(SAY_THROW);
                                leapTargetGUID = target->GetGUID();
                                state = STATE_GROUND;
                                me->ExitVehicle();
                                me->AttackStop();
                                // Wait for the exit fall to land before leaping
                                events.ScheduleEvent(EVENT_LEAP, 1500);
                            }
                            events.ScheduleEvent(EVENT_THROW_HELIX, 30000);
                            break;
                        case EVENT_LEAP:
                        {
                            Unit* target = ObjectAccessor::GetUnit(*me, leapTargetGUID);
                            if (!FreePlayerSelector()(target))
                                target = SelectFreePlayer();
                            if (target)
                                LeapTo(target);
                            else
                                events.ScheduleEvent(EVENT_LEAP, 1000);
                            break;
                        }
                        case EVENT_RIDE_FACE:
                        {
                            if (state != STATE_LEAPING)
                                break;

                            Unit* target = ObjectAccessor::GetUnit(*me, leapTargetGUID);
                            if (!FreePlayerSelector()(target))
                            {
                                events.ScheduleEvent(EVENT_LEAP, 1);
                                break;
                            }

                            state = STATE_ON_PLAYER;
                            riddenGUID = target->GetGUID();
                            boardAttempts = 0;
                            me->GetMotionMaster()->Clear();
                            BoardRider(target);
                            me->Attack(target, true);
                            events.ScheduleEvent(EVENT_CHECK_BOARDED, 1000);
                            events.ScheduleEvent(EVENT_LEAVE_FACE, 10000);
                            break;
                        }
                        case EVENT_CHECK_BOARDED:
                        {
                            if (state != STATE_ON_PLAYER || me->GetVehicle())
                                break;

                            Unit* target = ObjectAccessor::GetUnit(*me, riddenGUID);
                            if (!target)
                                break;

                            if (++boardAttempts <= 2)
                            {
                                BoardRider(target);
                                events.ScheduleEvent(EVENT_CHECK_BOARDED, 1000);
                                break;
                            }

                            // Could not board: punch the target from the ground instead
                            target->RemoveAurasDueToSpell(SPELL_HELIX_RIDE);
                            AttackStart(target);
                            break;
                        }
                        case EVENT_LEAVE_FACE:
                        {
                            if (state != STATE_ON_PLAYER)
                                break;

                            uint64 previous = riddenGUID;
                            ReleaseRider(ObjectAccessor::GetUnit(*me, riddenGUID));
                            riddenGUID = 0;
                            me->AttackStop();
                            me->GetMotionMaster()->Clear();

                            if (phaseTwo)
                            {
                                state = STATE_GROUND;
                                Unit* next = SelectFreePlayer(previous);
                                leapTargetGUID = next ? next->GetGUID() : 0;
                                events.ScheduleEvent(EVENT_LEAP, 1000);
                            }
                            else
                            {
                                state = STATE_GROUND;
                                events.ScheduleEvent(EVENT_RETURN_TO_OAF, 1000);
                            }
                            break;
                        }
                        case EVENT_RETURN_TO_OAF:
                        {
                            Creature* oaf = GetOaf();
                            if (phaseTwo || !oaf || !oaf->IsAlive())
                                break;

                            if (!OafCanCarry())
                            {
                                events.ScheduleEvent(EVENT_RETURN_TO_OAF, 2000);
                                break;
                            }

                            state = STATE_RETURNING;
                            me->GetMotionMaster()->MoveJump(oaf->GetPositionX(), oaf->GetPositionY(), oaf->GetPositionZ(), 25.0f, 10.0f, POINT_OAF);
                            events.ScheduleEvent(EVENT_BOARD_OAF, 3000);
                            break;
                        }
                        case EVENT_BOARD_OAF:
                        {
                            if (state != STATE_RETURNING || phaseTwo)
                                break;

                            Creature* oaf = GetOaf();
                            if (!oaf || !oaf->IsAlive())
                                break;

                            state = STATE_ON_OAF;
                            riddenGUID = 0;
                            me->AttackStop();
                            me->EnterVehicle(oaf, SEAT_HELIX);
                            SetAttackable(false);
                            break;
                        }
                    }
                }

                if (state == STATE_ON_PLAYER)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_helix_gearbreakerAI>(creature);
        }
};

class npc_lumbering_oaf : public CreatureScript
{
    public:
        npc_lumbering_oaf() : CreatureScript("npc_lumbering_oaf") { }

        struct npc_lumbering_oafAI : public ScriptedAI
        {
            npc_lumbering_oafAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                m_fake = false;
                grabbedGUID = 0;
            }

            EventMap events;
            InstanceScript* instance;
            bool m_fake;
            uint64 grabbedGUID;

            void InitializeAI() OVERRIDE
            {
                ApplyBossImmunities(me);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->setActive(true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                events.Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                m_fake = false;
                grabbedGUID = 0;
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->SetReactState(REACT_AGGRESSIVE);
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type == DATA_OAF_BUSY)
                    return grabbedGUID || m_fake ? 1 : 0;
                return 0;
            }

            Creature* GetHelix() const
            {
                if (!instance)
                    return NULL;
                return ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HELIX));
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                if (Creature* helix = GetHelix())
                    if (helix->IsAlive() && !helix->IsInCombat())
                        DoZoneInCombat(helix);

                events.ScheduleEvent(EVENT_OAF_SMASH, 12000);
            }

            void EnterEvadeMode() OVERRIDE
            {
                ReleaseGrabbed();

                Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                uint32 corpseDelay = me->GetCorpseDelay();
                uint32 respawnDelay = me->GetRespawnDelay();

                me->SetCorpseDelay(1);
                me->SetRespawnDelay(29);

                me->DespawnOrUnsummon();

                me->SetCorpseDelay(corpseDelay);
                me->SetRespawnDelay(respawnDelay);

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED | UNIT_FLAG_NON_ATTACKABLE);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (me->GetHealth() <= damage && !m_fake)
                {
                    damage = 0;
                    m_fake = true;
                    grabbedGUID = 0;

                    if (Vehicle* vehicle = me->GetVehicleKit())
                        vehicle->RemoveAllPassengers();

                    Talk(SAY_OAF_DEATH);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetStandState(UNIT_STAND_STATE_DEAD);
                    me->AttackStop();
                    me->GetMotionMaster()->Clear();
                    me->StopMoving();

                    if (instance)
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    events.Reset();

                    if (Creature* helix = GetHelix())
                        if (helix->IsAlive() && helix->IsAIEnabled)
                            helix->AI()->DoAction(ACTION_OAF_DEAD);
                }
            }

            void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) OVERRIDE
            {
                if (apply && seatId == SEAT_PLAYER && passenger->GetGUID() == grabbedGUID)
                {
                    events.CancelEvent(EVENT_GRAB_FAILED);
                    me->GetMotionMaster()->MovePoint(POINT_START, lumberingoafPos[1]);
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (pointId)
                {
                    case POINT_GRAB:
                        events.ScheduleEvent(EVENT_GRAB, 1);
                        break;
                    case POINT_START:
                        events.ScheduleEvent(EVENT_RAM, 1000);
                        break;
                    case POINT_END:
                        Smash();
                        break;
                }
            }

            void Smash()
            {
                Unit* grabbed = ObjectAccessor::GetUnit(*me, grabbedGUID);

                // Oaf Smash only has the normal damage in its default effect row; heroic takes 60% of the target's health
                if (grabbed && IsHeroic())
                {
                    int32 damage = int32(grabbed->CountPctFromMaxHealth(60));
                    me->CastCustomSpell(grabbed, SPELL_OAF_SMASH, NULL, &damage, NULL, true);
                }
                else
                    DoCast(me, SPELL_OAF_SMASH, true);

                ReleaseGrabbed();

                me->GetMotionMaster()->MoveKnockbackFrom(woodpilePos.GetPositionX(), woodpilePos.GetPositionY(), 8.0f, 4.0f);

                for (uint32 i = 0; i < 9; ++i)
                {
                    Position src = { -290.447998f, -484.328003f, 49.887901f, 0.0f };
                    Position dst = { -289.535614f, -493.059448f, 50.117603f, 0.0f };
                    me->MovePosition(src, (float)rand_norm() * 2, (float)rand_norm() * static_cast<float>(M_PI) * 2);
                    me->MovePosition(dst, (float)rand_norm() * 3, (float)rand_norm() * static_cast<float>(M_PI) * 2);
                    if (Creature* rat = me->SummonCreature(NPC_MINE_RAT, src, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                        rat->GetMotionMaster()->MovePoint(0, dst, false);
                }

                events.ScheduleEvent(EVENT_KNOCKBACK_PLAYER, 300);
                events.ScheduleEvent(EVENT_SMASH_RECOVER, 2500);
            }

            // Ride Oaf is a control vehicle aura owned by the Oaf, so the passenger has to leave the seat itself
            void ReleaseGrabbed()
            {
                if (Unit* grabbed = ObjectAccessor::GetUnit(*me, grabbedGUID))
                {
                    if (grabbed->GetVehicleBase() == me)
                        grabbed->ExitVehicle();
                    grabbed->RemoveAurasDueToSpell(SPELL_RIDE_OAF);
                }
            }

            void EndSmash()
            {
                grabbedGUID = 0;
                me->SetReactState(REACT_AGGRESSIVE);
                if (Unit* victim = SelectTarget(SELECT_TARGET_TOPAGGRO))
                    AttackStart(victim);
                events.ScheduleEvent(EVENT_OAF_SMASH, urand(30000, 35000));
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
                        case EVENT_OAF_SMASH:
                        {
                            Creature* helix = GetHelix();
                            if (!helix || !helix->IsAIEnabled || !helix->AI()->GetData(DATA_HELIX_ON_OAF))
                            {
                                events.ScheduleEvent(EVENT_OAF_SMASH, 3000);
                                break;
                            }

                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, FreePlayerSelector());
                            if (!target)
                            {
                                events.ScheduleEvent(EVENT_OAF_SMASH, 5000);
                                break;
                            }

                            Talk(SAY_OAF_SMASH);
                            grabbedGUID = target->GetGUID();
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->GetMotionMaster()->MoveCharge(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 30.0f, POINT_GRAB);
                            events.ScheduleEvent(EVENT_GRAB_FAILED, 6000);
                            break;
                        }
                        case EVENT_GRAB:
                        {
                            Unit* target = ObjectAccessor::GetUnit(*me, grabbedGUID);
                            if (!target || !target->IsAlive() || target->GetVehicle() || me->GetDistance(target) > 10.0f)
                            {
                                events.CancelEvent(EVENT_GRAB_FAILED);
                                EndSmash();
                                break;
                            }

                            target->CastSpell(me, SPELL_RIDE_OAF, true);
                            events.RescheduleEvent(EVENT_GRAB_FAILED, 2000);
                            break;
                        }
                        case EVENT_GRAB_FAILED:
                            ReleaseGrabbed();
                            EndSmash();
                            break;
                        case EVENT_RAM:
                            me->SetFacingTo(lumberingoafPos[2].GetOrientation());
                            me->GetMotionMaster()->MoveCharge(lumberingoafPos[2].GetPositionX(), lumberingoafPos[2].GetPositionY(), lumberingoafPos[2].GetPositionZ(), 30.0f, POINT_END);
                            break;
                        case EVENT_KNOCKBACK_PLAYER:
                            if (Unit* grabbed = ObjectAccessor::GetUnit(*me, grabbedGUID))
                                if (grabbed->IsAlive() && !grabbed->GetVehicle())
                                    grabbed->KnockbackFrom(woodpilePos.GetPositionX(), woodpilePos.GetPositionY(), 15.0f, 7.0f);
                            break;
                        case EVENT_SMASH_RECOVER:
                            EndSmash();
                            break;
                    }
                }

                if (!grabbedGUID)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_lumbering_oafAI>(creature);
        }
};

class npc_sticky_bomb : public CreatureScript
{
    public:
        npc_sticky_bomb() : CreatureScript("npc_sticky_bomb") { }

        enum iEvents
        {
            EVENT_ARMING_ARMED  = 1,
            EVENT_ARMING_ORANGE = 2,
            EVENT_ARMING_YELLOW = 3,
            EVENT_ARMING_RED    = 4,
            EVENT_BOOM          = 5,
            EVENT_CHECK_PLAYER  = 6,
        };

        enum iSpells
        {
            SPELL_ARMING_VISUAL_YELLOW    = 88315,
            SPELL_ARMING_VISUAL_RED       = 88317,
            SPELL_ARMING_VISUAL_ORANGE    = 88316,
            SPELL_STICKY_BOMB_ARMED_STATE = 88319,
            SPELL_EXPLODE                 = 88974,
        };

        struct npc_sticky_bombAI : public ScriptedAI
        {
            npc_sticky_bombAI(Creature* creature) : ScriptedAI(creature)
            {
                canBoom = false;
            }

            EventMap events;
            bool canBoom;

            void InitializeAI() OVERRIDE
            {
                ApplyBossImmunities(me);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                SetCombatMovement(false);
                canBoom = false;
                events.Reset();
                events.ScheduleEvent(EVENT_ARMING_YELLOW, 1000);
            }

            // The bomb template is neutral; its explosion has to be hostile to the players like its thrower
            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->setFaction(summoner->getFaction());
            }

            Player* GetNearbyPlayer()
            {
                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->GetSource())
                        if (player->IsAlive() && !player->IsGameMaster() && !player->GetVehicle() && me->GetExactDist2d(player) < 1.5f)
                            return player;
                return NULL;
            }

            void Explode()
            {
                DoCast(me, SPELL_EXPLODE, true);
                me->SetDisplayId(11686);
                me->DespawnOrUnsummon(2000);
                events.Reset();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ARMING_YELLOW:
                            me->CastSpell(me, canBoom ? SPELL_ARMING_VISUAL_RED : SPELL_ARMING_VISUAL_YELLOW, true);
                            events.ScheduleEvent(EVENT_ARMING_ORANGE, 2000);
                            break;
                        case EVENT_ARMING_ORANGE:
                            me->CastSpell(me, canBoom ? SPELL_ARMING_VISUAL_RED : SPELL_ARMING_VISUAL_ORANGE, true);
                            events.ScheduleEvent(EVENT_ARMING_RED, 2000);
                            break;
                        case EVENT_ARMING_RED:
                            me->CastSpell(me, SPELL_ARMING_VISUAL_RED, true);
                            events.ScheduleEvent(canBoom ? EVENT_BOOM : EVENT_ARMING_ARMED, 1500);
                            break;
                        case EVENT_ARMING_ARMED:
                            canBoom = true;
                            me->CastSpell(me, SPELL_STICKY_BOMB_ARMED_STATE, true);
                            events.ScheduleEvent(EVENT_CHECK_PLAYER, 250);
                            events.ScheduleEvent(EVENT_ARMING_YELLOW, 13000);
                            break;
                        case EVENT_BOOM:
                            Explode();
                            return;
                        case EVENT_CHECK_PLAYER:
                            if (GetNearbyPlayer())
                            {
                                Explode();
                                return;
                            }
                            events.ScheduleEvent(EVENT_CHECK_PLAYER, 250);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_sticky_bombAI>(creature);
        }
};

class npc_helix_crew : public CreatureScript
{
    public:
        npc_helix_crew() : CreatureScript("npc_helix_crew") { }

        struct npc_helix_crewAI : public ScriptedAI
        {
            npc_helix_crewAI(Creature* creature) : ScriptedAI(creature), summons(me)
            {
                ApplyBossImmunities(me);
                SetCombatMovement(false);
            }

            EventMap events;
            SummonList summons;

            void Reset() OVERRIDE
            {
                summons.DespawnAll();
                SetCombatMovement(false);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                summons.Despawn(summon);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_STICKY_BOMB, urand(5000, 8000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                summons.DespawnAll();
                me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STICKY_BOMB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                ThrowStickyBomb(me, target, 30000);
                            events.ScheduleEvent(EVENT_STICKY_BOMB, urand(15000, 20000));
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_helix_crewAI>(creature);
        }
};

// Chest Bomb 88352
class spell_helix_chest_bomb : public SpellScriptLoader
{
    public:
        spell_helix_chest_bomb() : SpellScriptLoader("spell_helix_chest_bomb") { }

        class spell_helix_chest_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_helix_chest_bomb_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = aurEff->GetBase()->GetUnitOwner())
                    target->CastSpell(target, SPELL_CHEST_BOMB_DMG, true);
            }

            void Register() OVERRIDE
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_helix_chest_bomb_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_helix_chest_bomb_AuraScript();
        }
};

// Force Player to Ride Oaf 88278
class spell_helix_force_player_to_ride_oaf : public SpellScriptLoader
{
    public:
        spell_helix_force_player_to_ride_oaf() : SpellScriptLoader("spell_helix_force_player_to_ride_oaf") { }

        class spell_helix_force_player_to_ride_oaf_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_helix_force_player_to_ride_oaf_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                target->CastSpell(caster, SPELL_RIDE_OAF, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_helix_force_player_to_ride_oaf_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_helix_force_player_to_ride_oaf_SpellScript();
        }
};

void AddSC_boss_helix_gearbreaker()
{
    new boss_helix_gearbreaker();
    new npc_lumbering_oaf();
    new npc_helix_crew();
    new npc_sticky_bomb();
    new spell_helix_force_player_to_ride_oaf();
    new spell_helix_chest_bomb();
}
