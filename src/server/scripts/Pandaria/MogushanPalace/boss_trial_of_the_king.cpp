/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "MoveSplineInit.h"
#include "GridNotifiers.h"
#include "Player.h"
#include "mogu_shan_palace.h"

enum eMisc
{
    SPELL_TRIAL_OF_THE_KING_CREDIT = 126952,
    TRIAL_RETIRE_DELAY             = 11000,
};

static const Position pXinWaypoints[] =
{
    { -4365.8f, -2613.6f, 22.3f, 0.0f },
    { -4365.6f, -2647.3f, 22.3f, 0.0f },
    { -4399.3f, -2647.0f, 22.3f, 0.0f }
};

static const Position pTrialHomePositions[] =
{
    { -4257.5f, -2613.5f, 17.5f, 6.25f },
    { -4215.9f, -2583.2f, 17.5f, 4.70f },
    { -4215.8f, -2644.2f, 17.5f, 1.57f }
};

static const Position apSaurokWaypoints[] =
{
    { -4235.7f, -2672.3f,  17.50f, 0.0f },
    { -4226.6f, -2686.2f,  13.70f, 0.0f },
    { -4211.7f, -2688.2f,  10.90f, 0.0f },
    { -4199.6f, -2681.4f,  7.700f, 0.0f },
    { -4194.6f, -2666.6f,  3.600f, 0.0f },
    { -4193.5f, -2643.3f, -3.600f, 0.0f },
    { -4196.6f, -2626.6f, -7.800f, 0.0f },
    { -4191.7f, -2596.7f, -9.400f, 0.0f },
    { -4194.9f, -2574.7f, -9.700f, 0.0f },
    { -4192.1f, -2552.4f, -20.80f, 0.0f },
    { -4196.7f, -2529.2f, -28.40f, 0.0f },
    { -4218.5f, -2518.3f, -28.39f, 0.0f },
    { -4264.0f, -2519.9f, -39.00f, 0.0f },
    { -4284.3f, -2538.9f, -39.08f, 0.0f }
};

static const Position apCaveBatPositions[6]
{
    { -4268.7f, -2664.5f, -22.2f, 0.0f },
    { -4293.1f, -2669.8f, -36.1f, 0.0f },
    { -4262.9f, -2654.9f, -1.06f, 0.0f },
    { -4283.3f, -2461.4f, -14.3f, 0.0f },
    { -4289.3f, -2466.3f, -20.0f, 0.0f },
    { -4282.4f, -2470.2f, -22.0f, 0.0f }
};

// The bosses use different action numbers for their idle talk
struct BossAction
{
    uint32 m_uiBoss;
    const int32 m_uiAction;
};

static const BossAction aBosses[3] =
{
    { CREATURE_HAIYAN_THE_UNSTOPPABLE, 2 },
    { CREATURE_MING_THE_CUNNING, 2 },
    { CREATURE_KUAI_THE_BRUTE, 5 }
};

static const Position pXinJumpPoint { -4296.391f, -2613.577f, 22.325f, 0.0f };

const uint32 auiStanceSpells[5] =
{
    SPELL_REPLACE_STAND_APPLAUD,
    SPELL_REPLACE_STAND_CHEER,
    SPELL_REPLACE_STAND_LAUGH,
    SPELL_REPLACE_STAND_RUDE,
    SPELL_REPLACE_STAND_SHOUT
};

const uint32 auiRetireSpells[3] =
{
    SPELL_REPLACE_STAND_CRY,
    SPELL_REPLACE_STAND_READY1H,
    SPELL_REPLACE_STAND_NO
};

const uint32 auiGruntScaleSpells[3] =
{
    SPELL_SCALE_95_100,
    SPELL_SCALE_105_110,
    SPELL_SCALE_115_120
};

static void ApplyTrialBossImmunities(Creature* me)
{
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS_DEST, true);
}

// Shared behaviour of the three trial bosses. They take part in a single encounter
// (DATA_TRIAL_OF_THE_KING) that the instance script drives, so they are no BossAI.
struct trial_bossAI : public ScriptedAI
{
    trial_bossAI(Creature* creature, uint32 trialIndex) : ScriptedAI(creature), _trialIndex(trialIndex)
    {
        instance = creature->GetInstanceScript();
        m_bIsMovingHome = false;
        me->SetHomePosition(pTrialHomePositions[_trialIndex]);
    }

    InstanceScript* instance;
    EventMap events;
    bool m_bIsMovingHome;
    uint32 const _trialIndex;

    void InitializeAI() OVERRIDE
    {
        Reset();
        ApplyTrialBossImmunities(me);
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (instance && instance->GetBossState(DATA_TRIAL_OF_THE_KING) == DONE)
            return;

        _EnterEvadeMode();
        events.Reset();

        // A retired boss leaving combat is not a wipe
        if (m_bIsMovingHome)
            return;

        if (instance)
            instance->SetData(TYPE_WIPE_FIRST_BOSS, _trialIndex);

        Movement::MoveSplineInit init(me);
        init.MoveTo(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY(), me->GetHomePosition().GetPositionZ());
        init.Launch();

        uint32 delay = 0;
        me->m_Events.Schedule(delay += me->GetSplineDuration(), 11, [this]()
        {
            me->StopMoving();
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);
        });
    }

    bool CanStartTrialFight() const
    {
        return !instance || instance->GetData(TYPE_SHUFFLE_ORDER) != DONE;
    }

    void Retire(uint32 retiredType)
    {
        me->RemoveAllAuras();
        me->SetReactState(REACT_PASSIVE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        me->SetHomePosition(pTrialHomePositions[_trialIndex]);
        me->CastStop();
        me->ClearUnitState(UNIT_STATE_CANNOT_TURN);
        me->GetMotionMaster()->MoveTargetedHome();
        me->AttackStop();
        events.Reset();

        if (instance)
            instance->SetData(retiredType, 0);

        m_bIsMovingHome = true;

        me->m_Events.Schedule(TRIAL_RETIRE_DELAY, [this]()
        {
            if (instance)
                instance->SetData(TYPE_SHUFFLE_ORDER, 0);
        });
    }

    // Returns true once the boss has been beaten and must not take damage anymore
    bool HandleTrialDamage(uint32& damage, uint32 defeatedTalk, uint32 retiredType)
    {
        // We need to retire the boss and let the next one enter combat.
        if ((me->GetHealthPct() < 10.f || damage > me->GetHealth()) && !m_bIsMovingHome)
        {
            Talk(defeatedTalk);
            damage = 0;

            Retire(retiredType);
        }

        if (m_bIsMovingHome)
            damage = 0;

        return m_bIsMovingHome;
    }
};

class npc_xian_the_weaponmaster_trigger : public CreatureScript
{
    public:
        npc_xian_the_weaponmaster_trigger() : CreatureScript("npc_xian_the_weaponmaster_trigger") { }

        enum eEvents
        {
            EVENT_INITIATE_SPEECH = 1,
            EVENT_TALK_0          = 2,
            EVENT_TALK_1          = 3,
            EVENT_JUMP            = 4,
            EVENT_INITIATE_FIGHT  = 5,
            EVENT_START_COMBAT    = 6,
            EVENT_MOVECHECK       = 7,
            EVENT_SET_ORIENTATION = 8,
            EVENT_IDLE_TALK       = 9,
        };

        enum eTalks
        {
            TALK_INTRO_01,
            TALK_INTRO_02,
        };

        struct npc_xian_the_weaponmaster_trigger_AI : public ScriptedAI
        {
            npc_xian_the_weaponmaster_trigger_AI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
                m_uiWaypointCount = 0;
                m_uiLastBoss = 0;
                m_bEventGo = false;
            }

            EventMap m_mEvents;
            bool m_bEventGo;

            uint32 m_uiWaypointCount;
            // Which boss talked last (used to prevent bosses talking twice in a row)
            uint32 m_uiLastBoss;
            InstanceScript* instance;

            void Reset() OVERRIDE
            {
                m_bEventGo = false;
                me->GetMotionMaster()->MoveTargetedHome();
                SetCanSeeEvenInPassiveMode(true);

                if (instance && instance->GetData(TYPE_TRIAL_CHEST) == DONE)
                {
                    m_bEventGo = true;
                    me->SetVisible(false);
                }
            }

            void InitializeAI() OVERRIDE
            {
                m_uiLastBoss = 0;
                m_bEventGo = false;
                m_mEvents.ScheduleEvent(EVENT_IDLE_TALK, urand(5000, 25000));
                me->GetMotionMaster()->MoveRandom(7.5f);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (pointId)
                {
                    case 0:
                    case 1:
                        m_mEvents.ScheduleEvent(EVENT_MOVECHECK, 200);
                        ++m_uiWaypointCount;
                        break;
                    case 2:
                        m_mEvents.Reset();
                        me->SetVisible(false);
                        break;
                }
            }

            void HandleIdleTalk()
            {
                uint32 m_uiRand = urand(0, 2);
                // Which boss will talk next
                uint32 m_uiNewBoss = aBosses[m_uiRand].m_uiBoss;

                while (m_uiNewBoss == m_uiLastBoss)
                {
                    m_uiRand = urand(0, 2);
                    m_uiNewBoss = aBosses[m_uiRand].m_uiBoss;
                }

                m_uiLastBoss = m_uiNewBoss;

                if (Creature* pBoss = GetClosestCreatureWithEntry(me, m_uiNewBoss, 99.f))
                    if (pBoss->AI())
                        pBoss->AI()->DoAction(aBosses[m_uiRand].m_uiAction);

                m_mEvents.ScheduleEvent(EVENT_IDLE_TALK, urand(15000, 25000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                m_mEvents.Update(diff);

                // Launch the speech when a player steps close to Xin the Weaponmaster
                if (!m_bEventGo && me->FindNearestPlayer(35.0f))
                {
                    m_bEventGo = true;
                    m_mEvents.ScheduleEvent(EVENT_INITIATE_SPEECH, 3000);
                    m_mEvents.CancelEvent(EVENT_IDLE_TALK);
                }

                while (uint32 eventId = m_mEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INITIATE_SPEECH:
                            me->GetMotionMaster()->Clear(false);
                            me->GetMotionMaster()->MoveIdle();
                            m_mEvents.ScheduleEvent(EVENT_TALK_0, 400);
                            break;
                        case EVENT_TALK_0:
                            Talk(TALK_INTRO_01);
                            m_mEvents.ScheduleEvent(EVENT_TALK_1, 11000);
                            break;
                        case EVENT_TALK_1:
                            Talk(TALK_INTRO_02);
                            m_mEvents.ScheduleEvent(EVENT_JUMP, 5000);
                            break;
                        case EVENT_JUMP:
                            DoCast(SPELL_MOGU_JUMP);
                            m_mEvents.ScheduleEvent(EVENT_INITIATE_FIGHT, 8000);
                            m_mEvents.ScheduleEvent(EVENT_SET_ORIENTATION, 2800);
                            break;
                        case EVENT_INITIATE_FIGHT:
                            // here is when we should start running away
                            me->SetWalk(false);
                            m_mEvents.ScheduleEvent(EVENT_START_COMBAT, 1400);
                            m_mEvents.ScheduleEvent(EVENT_MOVECHECK, 200);
                            m_uiWaypointCount = 0;
                            break;
                        case EVENT_START_COMBAT:
                            if (instance)
                                instance->SetData(TYPE_SHUFFLE_ORDER, 0);
                            break;
                        case EVENT_MOVECHECK:
                            if (m_uiWaypointCount < 3)
                                me->GetMotionMaster()->MovePoint(m_uiWaypointCount, pXinWaypoints[m_uiWaypointCount]);
                            break;
                        case EVENT_SET_ORIENTATION:
                            me->SetFacingTo(6.25f);
                            break;
                        case EVENT_IDLE_TALK:
                            HandleIdleTalk();
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_xian_the_weaponmaster_trigger_AI(creature);
        }
};

class boss_ming_the_cunning : public CreatureScript
{
    public:
        boss_ming_the_cunning() : CreatureScript("boss_ming_the_cunning") { }

        enum eEvents
        {
            EVENT_LIGHTNING_BOLT   = 1,
            EVENT_WHIRLING_DERVISH = 2,
            EVENT_MAGNETIC_FIELD   = 3,
        };

        enum eActions
        {
            ACTION_INTRO,
            ACTION_TALK_1,
            ACTION_TALK_IDLE
        };

        enum eTalks
        {
            TALK_INTRO,
            TALK_DEFEATED,
            TALK_KILLING,
            TALK_POSTCOMBAT,
            TALK_IDLE_1,
            TALK_WHIRLING_DERVISH,
            TALK_MAGNETIC_FIELD
        };

        struct boss_ming_the_cunning_AI : public trial_bossAI
        {
            boss_ming_the_cunning_AI(Creature* creature) : trial_bossAI(creature, TYPE_MING) { }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (!CanStartTrialFight())
                    return;

                me->CastSpell(me, SPELL_GUARDIAN_TAUNT, false);
                events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 5000);
                events.ScheduleEvent(EVENT_WHIRLING_DERVISH, 20000);
                events.ScheduleEvent(EVENT_MAGNETIC_FIELD, 30000);

                m_bIsMovingHome = false;
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(TALK_KILLING);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_INTRO:
                        Talk(TALK_INTRO);
                        break;
                    case ACTION_TALK_1:
                        Talk(TALK_POSTCOMBAT);
                        break;
                    case ACTION_TALK_IDLE:
                        Talk(TALK_IDLE_1);
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                HandleTrialDamage(damage, TALK_DEFEATED, TYPE_MING_RETIRED);
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
                        case EVENT_LIGHTNING_BOLT:
                            me->CastSpell(me->GetVictim(), SPELL_LIGHTNING_BOLT, false);
                            events.ScheduleEvent(EVENT_LIGHTNING_BOLT, urand(5000, 6000));
                            break;
                        case EVENT_WHIRLING_DERVISH:
                            Talk(TALK_WHIRLING_DERVISH);
                            DoCast(SPELL_WHIRLING_DERVISH);
                            events.ScheduleEvent(EVENT_WHIRLING_DERVISH, 20000);
                            break;
                        case EVENT_MAGNETIC_FIELD:
                            Talk(TALK_MAGNETIC_FIELD);
                            DoCast(SPELL_MAGNETIC_FIELD_AURA);
                            events.ScheduleEvent(EVENT_MAGNETIC_FIELD, 30000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_ming_the_cunning_AI(creature);
        }
};

class npc_whirling_dervish : public CreatureScript
{
    public:
        npc_whirling_dervish() : CreatureScript("npc_whirling_dervish") { }

        enum eEvents
        {
            EVENT_INITIALIZE_MYSELF = 1,
            EVENT_MOVE              = 2,
        };

        struct npc_whirling_dervish_AI : public ScriptedAI
        {
            npc_whirling_dervish_AI(Creature* creature) : ScriptedAI(creature)
            {
                events.ScheduleEvent(EVENT_INITIALIZE_MYSELF, 300);
                events.ScheduleEvent(EVENT_MOVE, 3000);
                m_bClockWise = urand(0, 1);
                me->GetPosition(x, y, z);

                // Radius of our circle should be 20 yards give or take
                m_uiArcRadius = 20.f;
                m_uiArcOffset = 0;
            }

            bool m_bClockWise;
            uint32 m_uiArcOffset;
            float m_uiArcRadius;
            float x, y, z;
            EventMap events;

            void HandleCircularMovement()
            {
                float newX = x + m_uiArcRadius * std::cos(m_uiArcOffset * M_PI / 12);
                float newY = m_bClockWise ? y + m_uiArcRadius * std::sin(m_uiArcOffset * M_PI / 12) : y - m_uiArcRadius * std::sin(m_uiArcOffset * M_PI / 12);
                float newZ = me->GetMap()->GetHeight(newX, newY, z);

                me->GetMotionMaster()->MovementExpired();
                me->GetMotionMaster()->MovePoint(0, newX, newY, newZ);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == 0)
                {
                    // make sure we offset next point in circle
                    ++m_uiArcOffset;
                    events.ScheduleEvent(EVENT_MOVE, 100);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INITIALIZE_MYSELF:
                            if (!me->HasAura(SPELL_WHIRLING_DERVISH_AURA))
                                DoCast(SPELL_WHIRLING_DERVISH_AURA);
                            events.ScheduleEvent(EVENT_INITIALIZE_MYSELF, 2500);
                            break;
                        case EVENT_MOVE:
                            HandleCircularMovement();
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_whirling_dervish_AI(creature);
        }
};

class npc_adepts : public CreatureScript
{
    public:
        npc_adepts() : CreatureScript("npc_adepts") { }

        enum eEvents
        {
            EVENT_CHECK_MOVE = 1,
        };

        enum eActions
        {
            ACTION_ENCOURAGE,
            ACTION_RETIRE,
            ACTION_ATTACK,
        };

        enum eStatus
        {
            STATUS_ATTACK_PLAYER,
            STATUS_ATTACK_GRUNTS,
        };

        enum eTalks
        {
            TALK_00,
        };

        struct npc_adepts_AI : public ScriptedAI
        {
            npc_adepts_AI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                status = STATUS_ATTACK_PLAYER;
                m_uiKickCooldownTimer = 0;
            }

            uint8 status;
            uint32 m_uiKickCooldownTimer;
            EventMap events;
            InstanceScript* instance;

            void InitializeAI() OVERRIDE
            {
                me->SetWalk(false);
                status = STATUS_ATTACK_PLAYER;
                m_uiKickCooldownTimer = 0;
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                DoCast(auiGruntScaleSpells[urand(0, 2)]);
                events.ScheduleEvent(EVENT_CHECK_MOVE, 2500);
            }

            void MoveRelative(float dist)
            {
                float x, y;
                GetPositionWithDistInOrientation(me, dist, me->GetOrientation(), x, y);

                Movement::MoveSplineInit init(me);
                init.MoveTo(x, y, me->GetMap()->GetHeight(me->GetPhaseMask(), x, y, me->GetPositionZ()));
                init.Launch();
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_ENCOURAGE:
                    {
                        MoveRelative(5.0f);

                        uint32 delay = 0;
                        me->m_Events.Schedule(delay += me->GetSplineDuration(), 11, [this]()
                        {
                            me->StopMoving();
                        });

                        DoCast(auiStanceSpells[urand(0, 4)]);
                        break;
                    }
                    case ACTION_RETIRE:
                    {
                        MoveRelative(-5.0f);

                        uint32 delay = 0;
                        me->m_Events.Schedule(delay += me->GetSplineDuration(), 11, [this]()
                        {
                            me->StopMoving();
                            me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI));
                        });

                        me->RemoveAurasByType(SPELL_AURA_ANIM_REPLACEMENT_SET);
                        DoCast(auiRetireSpells[urand(0, 2)]);

                        me->CombatStop();
                        break;
                    }
                    case ACTION_ATTACK:
                    {
                        me->CombatStop();
                        status = STATUS_ATTACK_GRUNTS;
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_PACIFIED);

                        MoveRelative(30.0f);

                        uint32 delay = 0;
                        me->m_Events.Schedule(delay += me->GetSplineDuration(), 11, [this]()
                        {
                            me->StopMoving();
                        });

                        me->RemoveAurasByType(SPELL_AURA_ANIM_REPLACEMENT_SET);

                        events.CancelEvent(EVENT_CHECK_MOVE);
                        break;
                    }
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (who->GetTypeId() == TypeID::TYPEID_PLAYER && who->GetAreaId() == 6471
                        && status != STATUS_ATTACK_GRUNTS
                        && me->GetDistance2d(who) < 2.0f
                        && who->isInFront(me)
                        && who->GetPositionZ() <= (me->GetPositionZ() + 3.f)
                        && instance && instance->GetBossState(DATA_TRIAL_OF_THE_KING) < DONE)
                {
                    if (me->isMoving() || m_uiKickCooldownTimer)
                        return;

                    m_uiKickCooldownTimer = 1 * IN_MILLISECONDS;

                    DoCast(who, SPELL_GRUNT_KNOCK_2, true);
                    who->CastSpell(who, SPELL_GRUNT_KNOCK, true);

                    if (urand(0, 2))
                        Talk(TALK_00, who);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (m_uiKickCooldownTimer > diff)
                    m_uiKickCooldownTimer -= diff;
                else
                    m_uiKickCooldownTimer = 0;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_MOVE:
                            me->CombatStop();
                            if (me->GetDistance(me->GetHomePosition()) > 7.f)
                                me->GetMotionMaster()->MoveTargetedHome();
                            events.ScheduleEvent(EVENT_CHECK_MOVE, 2500);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_adepts_AI(creature);
        }
};

class boss_kuai_the_brute : public CreatureScript
{
    public:
        boss_kuai_the_brute() : CreatureScript("boss_kuai_the_brute") { }

        enum eEvents
        {
            EVENT_SHOCKWAVE = 1,
        };

        enum eCreatures
        {
            CREATURE_MU_SHIBA = 61453,
        };

        enum eActions
        {
            ACTION_ATTACK,
            ACTION_ATTACK_STOP,
            ACTION_INTRO,
            ACTION_TALK_1,
            ACTION_TALK_2,
            ACTION_TALK_IDLE
        };

        enum eTalks
        {
            TALK_INTRO,
            TALK_DEFEATED,
            TALK_KILLING,
            TALK_POSTCOMBAT_1,
            TALK_POSTCOMBAT_2,
            TALK_IDLE_1,
            TALK_SHOCKWAVE
        };

        struct boss_kuai_the_brute_AI : public trial_bossAI
        {
            boss_kuai_the_brute_AI(Creature* creature) : trial_bossAI(creature, TYPE_KUAI)
            {
                pet_guid = 0;
            }

            uint64 pet_guid;

            void SummonMuShiba()
            {
                if (pet_guid || !me->IsInWorld())
                    return;

                if (TempSummon* sum = me->SummonCreature(CREATURE_MU_SHIBA, me->GetPositionX() + 3 * std::cos(M_PI / 4), me->GetPositionY() + 3 * std::sin(M_PI / 4), me->GetPositionZ(), me->GetOrientation()))
                {
                    pet_guid = sum->GetGUID();
                    sum->setFaction(me->getFaction());
                }
            }

            Creature* GetMuShiba()
            {
                return pet_guid ? ObjectAccessor::GetCreature(*me, pet_guid) : NULL;
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(TALK_KILLING);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_INTRO:
                        Talk(TALK_INTRO);
                        break;
                    case ACTION_TALK_1:
                        Talk(TALK_POSTCOMBAT_1);
                        break;
                    case ACTION_TALK_2:
                        Talk(TALK_POSTCOMBAT_2);
                        break;
                    case ACTION_TALK_IDLE:
                        Talk(TALK_IDLE_1);
                        break;
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (!CanStartTrialFight())
                    return;

                events.ScheduleEvent(EVENT_SHOCKWAVE, 15 * IN_MILLISECONDS);

                SummonMuShiba();
                if (Creature* mu_shiba = GetMuShiba())
                    if (mu_shiba->AI())
                        mu_shiba->AI()->DoAction(ACTION_ATTACK);

                m_bIsMovingHome = false;
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (m_bIsMovingHome)
                {
                    damage = 0;
                    return;
                }

                if (!HandleTrialDamage(damage, TALK_DEFEATED, TYPE_KUAI_RETIRED))
                    return;

                Creature* mu_shiba = GetMuShiba();
                if (mu_shiba && mu_shiba->IsAlive())
                {
                    mu_shiba->CastStop();
                    mu_shiba->GetMotionMaster()->MoveTargetedHome();
                    if (mu_shiba->AI())
                        mu_shiba->AI()->DoAction(ACTION_ATTACK_STOP);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                SummonMuShiba();

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHOCKWAVE:
                            Talk(TALK_SHOCKWAVE);
                            DoCast(SPELL_PICK_SHOCKWAVE_TARGET);
                            events.ScheduleEvent(EVENT_SHOCKWAVE, 15000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_kuai_the_brute_AI(creature);
        }
};

class npc_mu_shiba : public CreatureScript
{
    public:
        npc_mu_shiba() : CreatureScript("npc_mu_shiba") { }

        enum eSpells
        {
            SPELL_RAVAGE = 119948,
        };

        enum eActions
        {
            ACTION_ATTACK,
            ACTION_ATTACK_STOP,
        };

        struct npc_mu_shiba_AI : public ScriptedAI
        {
            npc_mu_shiba_AI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                Reset();
                ApplyTrialBossImmunities(me);
            }

            void Reset() OVERRIDE
            {
                DoAction(ACTION_ATTACK_STOP);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(1, 7000);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_ATTACK:
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        DoZoneInCombat();

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            AttackStart(target);
                        break;
                    case ACTION_ATTACK_STOP:
                        events.Reset();
                        me->AttackStop();
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == 1)
                    {
                        me->CastSpell(me->GetVictim(), SPELL_RAVAGE, false);
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->Attack(target, false);
                        events.ScheduleEvent(1, 25000);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_mu_shiba_AI(creature);
        }
};

class boss_haiyan_the_unstoppable : public CreatureScript
{
    public:
        boss_haiyan_the_unstoppable() : CreatureScript("boss_haiyan_the_unstoppable") { }

        enum eEvents
        {
            EVENT_TRAUMATIC_BLOW = 1,
            EVENT_CONFLAGRATE    = 2,
            EVENT_METEOR         = 3,
        };

        enum eTalks
        {
            TALK_INTRO,
            TALK_DEFEATED,
            TALK_KILLING,
            TALK_POSTCOMBAT,
            TALK_IDLE_1,
            TALK_CONFLAGRATE,
            TALK_METEOR
        };

        enum eActions
        {
            ACTION_INTRO,
            ACTION_TALK_1,
            ACTION_TALK_IDLE
        };

        struct boss_haiyan_the_unstoppable_AI : public trial_bossAI
        {
            boss_haiyan_the_unstoppable_AI(Creature* creature) : trial_bossAI(creature, TYPE_HAIYAN) { }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (!CanStartTrialFight())
                    return;

                m_bIsMovingHome = false;
                events.ScheduleEvent(EVENT_TRAUMATIC_BLOW, 10000);
                events.ScheduleEvent(EVENT_CONFLAGRATE, 20000);
                events.ScheduleEvent(EVENT_METEOR, 40000);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_INTRO:
                        Talk(TALK_INTRO);
                        break;
                    case ACTION_TALK_1:
                        Talk(TALK_POSTCOMBAT);
                        break;
                    case ACTION_TALK_IDLE:
                        Talk(TALK_IDLE_1);
                        break;
                }
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(TALK_KILLING);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                HandleTrialDamage(damage, TALK_DEFEATED, TYPE_HAIYAN_RETIRED);
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
                        case EVENT_TRAUMATIC_BLOW:
                            me->CastSpell(me->GetVictim(), SPELL_TRAUMATIC_BLOW, false);
                            events.ScheduleEvent(EVENT_TRAUMATIC_BLOW, 20 * IN_MILLISECONDS);
                            break;
                        case EVENT_CONFLAGRATE:
                            Talk(TALK_CONFLAGRATE);
                            DoCast(SPELL_CONFLAGRATE);
                            events.ScheduleEvent(EVENT_CONFLAGRATE, 20000);
                            break;
                        case EVENT_METEOR:
                            Talk(TALK_METEOR);
                            DoCast(SPELL_METEOR_TARGETING);
                            events.ScheduleEvent(EVENT_METEOR, urand(30000, 40000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_haiyan_the_unstoppable_AI(creature);
        }
};

class npc_glintrok_scout : public CreatureScript
{
    public:
        npc_glintrok_scout() : CreatureScript("npc_glintrok_scout") { }

        enum eSpells : uint32
        {
            SPELL_TAKE_LOOT      = 125265,
            SPELL_CAVEBAT_VISUAL = 119319
        };

        enum eTalks
        {
            EMOTE_1
        };

        enum eActions
        {
            ACTION_INITIALIZE,
        };

        enum eEvents
        {
            EVENT_LOOT_CHEST    = 1,
            EVENT_START_MOVING  = 2,
            EVENT_MOVECHECK     = 3,
            EVENT_TALK_1        = 4,
            EVENT_TALK_2        = 5,
            EVENT_TALK_3        = 6,
            EVENT_TALK_4        = 7,
            EVENT_START_BATTLE  = 8,
            EVENT_SUMMON_BAT    = 9,
            EVENT_JUMP_LAIR     = 10,
        };

        struct npc_glintrok_scoutAI : public ScriptedAI
        {
            npc_glintrok_scoutAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                m_uiWayPointCount = 0;
            }

            uint32 m_uiWayPointCount;
            EventMap events;
            InstanceScript* instance;

            void InitializeAI() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_INITIALIZE:
                        me->SetVisible(true);
                        me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING);
                        events.ScheduleEvent(EVENT_LOOT_CHEST, 5000);
                        events.ScheduleEvent(EVENT_TALK_1, 10000);

                        if (instance)
                            instance->SetData(TYPE_TRIAL_CHEST, DONE);
                        break;
                }
            }

            void HandleBatSummons()
            {
                for (uint8 n = 0; n < 6; ++n)
                {
                    if (Creature* pBat = me->SummonCreature(CREATURE_CAVE_BAT, apCaveBatPositions[n], TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000))
                    {
                        pBat->SetCanFly(true);
                        pBat->CastSpell(pBat, SPELL_CAVEBAT_VISUAL, true);
                        pBat->GetMotionMaster()->MovePoint(0, n > 2 ? apCaveBatPositions[urand(0, 2)] : apCaveBatPositions[urand(3, 5)]);
                    }
                }
            }

            Creature* GetAffectedBoss(uint32 type)
            {
                return instance ? ObjectAccessor::GetCreature(*me, instance->GetData64(type)) : NULL;
            }

            void ClearCombat()
            {
                Map::PlayerList const& lPlayers = me->GetMap()->GetPlayers();

                for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    if (Player* player = itr->GetSource())
                        player->CombatStop();
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (pointId == EVENT_JUMP)
                    me->SetVisible(false);

                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId >= 13)
                {
                    events.CancelEvent(EVENT_MOVECHECK);
                    events.ScheduleEvent(EVENT_JUMP_LAIR, 100);
                }
                else
                {
                    events.ScheduleEvent(EVENT_MOVECHECK, 100);
                    ++m_uiWayPointCount;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LOOT_CHEST:
                            me->HandleEmoteCommand(EMOTE_STATE_NONE);
                            DoCast(SPELL_TAKE_LOOT);
                            events.ScheduleEvent(EVENT_START_MOVING, 500);

                            ClearCombat();
                            break;
                        case EVENT_START_MOVING:
                            Talk(EMOTE_1);
                            me->SetWalk(false);
                            events.ScheduleEvent(EVENT_MOVECHECK, 200);
                            events.ScheduleEvent(EVENT_SUMMON_BAT, 4000);
                            m_uiWayPointCount = 0;
                            break;
                        case EVENT_MOVECHECK:
                            me->GetMotionMaster()->Clear(false);
                            me->GetMotionMaster()->MovePoint(m_uiWayPointCount, apSaurokWaypoints[m_uiWayPointCount]);
                            break;
                        case EVENT_JUMP_LAIR:
                            me->GetMotionMaster()->MoveJump(pSkoutJump, 15.5f, 20.5f, EVENT_JUMP);
                            break;
                        case EVENT_TALK_1:
                            if (Creature* pKuai = GetAffectedBoss(TYPE_KUAI))
                            {
                                if (pKuai->AI())
                                    pKuai->AI()->DoAction(3);

                                events.ScheduleEvent(EVENT_TALK_2, 10000);

                                pKuai->CombatStop();
                            }
                            break;
                        case EVENT_TALK_2:
                            if (Creature* pMing = GetAffectedBoss(TYPE_MING))
                            {
                                if (pMing->AI())
                                    pMing->AI()->DoAction(1);

                                events.ScheduleEvent(EVENT_TALK_3, 14000);

                                pMing->CombatStop();
                            }
                            break;
                        case EVENT_TALK_3:
                            if (Creature* pHaiyan = GetAffectedBoss(TYPE_HAIYAN))
                            {
                                if (pHaiyan->AI())
                                    pHaiyan->AI()->DoAction(1);

                                events.ScheduleEvent(EVENT_TALK_4, 13000);

                                pHaiyan->CombatStop();
                            }
                            break;
                        case EVENT_TALK_4:
                            if (Creature* pKuai = GetAffectedBoss(TYPE_KUAI))
                            {
                                if (pKuai->AI())
                                    pKuai->AI()->DoAction(4);

                                events.ScheduleEvent(EVENT_START_BATTLE, 4800);
                            }
                            break;
                        case EVENT_START_BATTLE:
                            if (instance)
                                instance->SetData(TYPE_TRIAL_ENDED, 0);
                            DoCastAOE(SPELL_TRIAL_OF_THE_KING_CREDIT);
                            break;
                        case EVENT_SUMMON_BAT:
                            HandleBatSummons();
                            events.ScheduleEvent(EVENT_SUMMON_BAT, urand(9000, 14000));
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_glintrok_scoutAI(creature);
        }
};

class npc_glintrok_scout_2 : public CreatureScript
{
    public:
        npc_glintrok_scout_2() : CreatureScript("npc_glintrok_scout_2") { }

        enum eTalks
        {
            TALK_EMOTE
        };

        struct npc_glintrok_scout_2AI : public ScriptedAI
        {
            npc_glintrok_scout_2AI(Creature* creature) : ScriptedAI(creature)
            {
                m_bHasCalledMobs = false;
            }

            bool m_bHasCalledMobs;

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (m_bHasCalledMobs)
                {
                    ScriptedAI::MoveInLineOfSight(who);
                    return;
                }

                if (who && who->GetTypeId() == TypeID::TYPEID_PLAYER && who->GetDistance(me) < 15.0f)
                {
                    m_bHasCalledMobs = true;
                    me->SetOrientation(M_PI);
                    me->SetFacingTo(M_PI);
                    DoCast(SPELL_GLINTROK_SCOUT_CALL);
                    Talk(TALK_EMOTE);
                }
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
            return new npc_glintrok_scout_2AI(creature);
        }
};

class spell_xin_mogu_jump_targeting : public SpellScriptLoader
{
    public:
        spell_xin_mogu_jump_targeting() : SpellScriptLoader("spell_xin_mogu_jump_targeting") { }

        class xin_mogu_jump_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(xin_mogu_jump_targeting_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MOGU_JUMP))
                    return false;
                return true;
            }

            void HandlePosition()
            {
                if (GetCaster() && GetCaster()->GetDistance(pXinJumpPoint) < 99.f)
                    GetCaster()->GetMotionMaster()->MoveJump(pXinJumpPoint.GetPositionX(), pXinJumpPoint.GetPositionY(), pXinJumpPoint.GetPositionZ(), 45.0f, 25.0f);

                FinishCast(SpellCastResult::SPELL_CAST_OK);
            }

            void Register() OVERRIDE
            {
                BeforeCast += SpellCastFn(xin_mogu_jump_targeting_SpellScript::HandlePosition);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new xin_mogu_jump_targeting_SpellScript();
        }
};

class PlayerCheck
{
    public:
        bool operator()(WorldObject* target) const
        {
            return !target->ToPlayer();
        }
};

class spell_magnetic_field : public SpellScriptLoader
{
    public:
        spell_magnetic_field() : SpellScriptLoader("spell_magnetic_field") { }

        class spell_magnetic_field_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_magnetic_field_AuraScript);

            uint32 m_uiAuraTicks;
            uint32 m_uiAuraTimer;

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGNETIC_FIELD_AURA))
                    return false;
                return true;
            }

            bool Load() OVERRIDE
            {
                m_uiAuraTicks = 0;
                m_uiAuraTimer = 500;
                return true;
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                ++m_uiAuraTicks;
            }

            void AuraUpdate(uint32 diff)
            {
                if (m_uiAuraTimer <= diff)
                {
                    m_uiAuraTimer = 500;

                    uint32 uiUsedSpell = 0;

                    if (m_uiAuraTicks < 4)
                        uiUsedSpell = SPELL_MAGNETIC_FIELD_10;
                    else if (m_uiAuraTicks < 6)
                        uiUsedSpell = SPELL_MAGNETIC_FIELD_20;
                    else
                        uiUsedSpell = SPELL_MAGNETIC_FIELD_40;

                    if (Unit* caster = GetCaster())
                        caster->CastSpell(caster, uiUsedSpell, true);
                }
                else
                    m_uiAuraTimer -= diff;
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_magnetic_field_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                OnAuraUpdate += AuraUpdateFn(spell_magnetic_field_AuraScript::AuraUpdate);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_magnetic_field_AuraScript();
        }
};

// Bosses should never be sucked in
class CreatureTargetCheck
{
    public:
        bool operator()(WorldObject* target) const
        {
            return target && target->ToCreature() && !target->ToPlayer();
        }
};

class spell_magnetic_pull : public SpellScriptLoader
{
    public:
        spell_magnetic_pull() : SpellScriptLoader("spell_magnetic_pull") { }

        class spell_magnetic_pull_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_magnetic_pull_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGNETIC_FIELD_10) || !sSpellMgr->GetSpellInfo(SPELL_MAGNETIC_FIELD_20) || !sSpellMgr->GetSpellInfo(SPELL_MAGNETIC_FIELD_40))
                    return false;
                return true;
            }

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(CreatureTargetCheck());
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unit = GetHitUnit())
                    if (Unit* caster = GetCaster())
                        if (unit->GetDistance(caster) > 8.f)
                            unit->CastSpell(caster, SPELL_MAGNETIC_PULL, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_magnetic_pull_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_magnetic_pull_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_magnetic_pull_SpellScript();
        }
};

class spell_whirling_dervish : public SpellScriptLoader
{
    public:
        spell_whirling_dervish() : SpellScriptLoader("spell_whirling_dervish") { }

        class spell_whirling_dervish_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_whirling_dervish_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WHIRLING_DERVISH_AURA))
                    return false;
                return true;
            }

            void HandleAuraRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->ToCreature())
                        caster->AddObjectToRemoveList();
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_whirling_dervish_AuraScript::HandleAuraRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_whirling_dervish_AuraScript();
        }
};

class AuraCheck
{
    public:
        AuraCheck(uint32 aura) : _aura(aura) { }

        bool operator()(WorldObject* target) const
        {
            return target->ToUnit() && target->ToUnit()->HasAura(_aura);
        }
    private:
        uint32 _aura;
};

class spell_whirling_dervish_knock : public SpellScriptLoader
{
    public:
        spell_whirling_dervish_knock() : SpellScriptLoader("spell_whirling_dervish_knock") { }

        class spell_whirling_dervish_knock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_whirling_dervish_knock_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WHIRLING_DERVISH_TARGETS))
                    return false;
                return true;
            }

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(AuraCheck(SPELL_WHIRLING_DERVISH_AURA));
            }

            void HandleAfterHit()
            {
                if (GetCaster() && GetHitUnit())
                {
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_THROW, false);
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_THROW_2, false);
                }
            }

            void Register() OVERRIDE
            {
                AfterHit += SpellHitFn(spell_whirling_dervish_knock_SpellScript::HandleAfterHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_whirling_dervish_knock_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_whirling_dervish_knock_SpellScript();
        }
};

// Shockwave 119922
class spell_shockwave_kuai : public SpellScriptLoader
{
    public:
        spell_shockwave_kuai() : SpellScriptLoader("spell_shockwave_kuai") { }

        class spell_shockwave_kuai_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_shockwave_kuai_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHOCKWAVE))
                    return false;
                return true;
            }

            void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster() && GetCaster()->ToCreature() && !GetCaster()->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC))
                    GetCaster()->ToCreature()->RemoveChanneledCast(0);
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_shockwave_kuai_AuraScript::HandleOnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_shockwave_kuai_AuraScript();
        }
};

// Shockwave 119922, 119929, 119931, 119932, 119933
class spell_shockwave_kuai_eff : public SpellScriptLoader
{
    public:
        spell_shockwave_kuai_eff() : SpellScriptLoader("spell_shockwave_kuai_eff") { }

        class spell_shockwave_kuai_eff_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_shockwave_kuai_eff_SpellScript);

            void HandleOnHit(SpellEffIndex effIndex)
            {
                if (GetCaster() && GetCaster()->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC))
                    PreventHitEffect(effIndex);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_shockwave_kuai_eff_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_shockwave_kuai_eff_SpellScript();
        }
};

class spell_shockwave_targeting : public SpellScriptLoader
{
    public:
        spell_shockwave_targeting() : SpellScriptLoader("spell_shockwave_targeting") { }

        class spell_shockwave_kuai_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_shockwave_kuai_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PICK_SHOCKWAVE_TARGET))
                    return false;
                return true;
            }

            void SelectTarget(std::list<WorldObject*>& targets)
            {
                targets.remove_if(PlayerCheck());

                if (!targets.empty())
                    targets.resize(1);
            }

            void HandleSpellcast()
            {
                if (GetHitUnit() && GetCaster() && GetCaster()->ToCreature())
                    GetCaster()->ToCreature()->PrepareChanneledCast(GetCaster()->GetAngle(GetHitUnit()), SPELL_SHOCKWAVE);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_shockwave_kuai_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                AfterHit += SpellHitFn(spell_shockwave_kuai_SpellScript::HandleSpellcast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_shockwave_kuai_SpellScript();
        }
};

class spell_haiyan_conflagrate_targeting : public SpellScriptLoader
{
    public:
        spell_haiyan_conflagrate_targeting() : SpellScriptLoader("spell_haiyan_conflagrate_targeting") { }

        class spell_haiyan_conflagrate_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_haiyan_conflagrate_targeting_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CONFLAGRATE))
                    return false;
                return true;
            }

            void SelectTarget(std::list<WorldObject*>& targets)
            {
                targets.remove_if(PlayerCheck());

                if (targets.size() > 1)
                    targets.resize(1);
            }

            void HandleSpellcast()
            {
                if (GetHitUnit() && GetCaster())
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_CONFLAGRATE_AURA, false);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_haiyan_conflagrate_targeting_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                AfterHit += SpellHitFn(spell_haiyan_conflagrate_targeting_SpellScript::HandleSpellcast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_haiyan_conflagrate_targeting_SpellScript();
        }
};

class spell_haiyan_conflagrate_aura : public SpellScriptLoader
{
    public:
        spell_haiyan_conflagrate_aura() : SpellScriptLoader("spell_haiyan_conflagrate_aura") { }

        class spell_haiyan_conflagrate_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_haiyan_conflagrate_aura_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CONFLAGRATE_SPREAD))
                    return false;
                return true;
            }

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(AuraCheck(SPELL_CONFLAGRATE_AURA));
                targets.remove_if(CreatureTargetCheck());
                // can't deal with loads of adepts/grunts dying from this spell
                targets.remove_if(PlayerCheck());
            }

            void HandleSpellcast()
            {
                if (GetHitUnit() && GetCaster())
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_CONFLAGRATE_AURA, true);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_haiyan_conflagrate_aura_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                AfterHit += SpellHitFn(spell_haiyan_conflagrate_aura_SpellScript::HandleSpellcast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_haiyan_conflagrate_aura_SpellScript();
        }
};

class spell_haiyan_meteor_targeting : public SpellScriptLoader
{
    public:
        spell_haiyan_meteor_targeting() : SpellScriptLoader("spell_haiyan_meteor_targeting") { }

        class spell_haiyan_meteor_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_haiyan_meteor_targeting_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_METEOR_TARGETING))
                    return false;
                return true;
            }

            void SelectTarget(std::list<WorldObject*>& targets)
            {
                targets.remove_if(PlayerCheck());
                if (!targets.empty())
                    targets.resize(1);
            }

            void HandleSpellcast()
            {
                if (GetHitUnit() && GetCaster())
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_METEOR, false);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_haiyan_meteor_targeting_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                AfterHit += SpellHitFn(spell_haiyan_meteor_targeting_SpellScript::HandleSpellcast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_haiyan_meteor_targeting_SpellScript();
        }
};

class spell_haiyan_meteor : public SpellScriptLoader
{
    public:
        spell_haiyan_meteor() : SpellScriptLoader("spell_haiyan_meteor") { }

        class spell_haiyan_meteor_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_haiyan_meteor_SpellScript);

            uint32 m_targets;

            bool Load() OVERRIDE
            {
                m_targets = 0;
                return true;
            }

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(PlayerCheck());
                m_targets = targets.size();
            }

            void OnBeforeHit()
            {
                if (m_targets != 0)
                    SetHitDamage(GetHitDamage() / int32(m_targets));
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_haiyan_meteor_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnHit += SpellHitFn(spell_haiyan_meteor_SpellScript::OnBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_haiyan_meteor_SpellScript();
        }
};

class CorrectUnitCheck
{
    public:
        CorrectUnitCheck(Unit const* caster) : casterGuid(caster->GetGUID()) { }

        bool operator ()(WorldObject const* target) const
        {
            return target && ((target->ToCreature() && target->ToCreature()->getFaction() != 16) || target->ToPlayer() || target->GetGUID() == casterGuid);
        }

    private:
        uint64 casterGuid;
};

class spell_saurok_help_call : public SpellScriptLoader
{
    public:
        spell_saurok_help_call() : SpellScriptLoader("spell_saurok_help_call") { }

        class spell_saurok_help_call_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_saurok_help_call_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_GLINTROK_SCOUT_CALL))
                    return false;
                return true;
            }

            void SelectTarget(std::list<WorldObject*>& targets)
            {
                if (GetCaster())
                {
                    targets.remove_if(CorrectUnitCheck(GetCaster()));
                    targets.sort(Skyfire::ObjectDistanceOrderPred(GetCaster()));
                    if (targets.size() > 3)
                        targets.resize(3);
                }
            }

            void HandleSpellcast()
            {
                Unit* target = GetHitUnit();
                if (!target)
                    return;

                float x, y;
                GetPositionWithDistInOrientation(target, 30.0f, target->GetOrientation(), x, y);
                target->GetMotionMaster()->MovePoint(0, x, y, target->GetMap()->GetHeight(x, y, target->GetPositionZ()));

                if (target->GetEntry() == CREATURE_GLINTROK_IRONHIDE)
                    target->CastSpell(target, SPELL_IRON_PROTECTOR, false);

                target->CastSpell(target, SPELL_REPLACE_STAND_READY1H, true);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_saurok_help_call_SpellScript::SelectTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
                AfterHit += SpellHitFn(spell_saurok_help_call_SpellScript::HandleSpellcast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_saurok_help_call_SpellScript();
        }
};

void AddSC_boss_trial_of_the_king()
{
    new npc_xian_the_weaponmaster_trigger();
    new boss_ming_the_cunning();
    new npc_whirling_dervish();
    new npc_adepts();
    new boss_kuai_the_brute();
    new npc_mu_shiba();
    new boss_haiyan_the_unstoppable();
    new npc_glintrok_scout();
    new npc_glintrok_scout_2();
    new spell_xin_mogu_jump_targeting();
    new spell_magnetic_field();
    new spell_magnetic_pull();
    new spell_whirling_dervish();
    new spell_whirling_dervish_knock();
    new spell_shockwave_kuai();
    new spell_shockwave_kuai_eff();
    new spell_shockwave_targeting();
    new spell_haiyan_conflagrate_targeting();
    new spell_haiyan_conflagrate_aura();
    new spell_haiyan_meteor_targeting();
    new spell_haiyan_meteor();
    new spell_saurok_help_call();
}
