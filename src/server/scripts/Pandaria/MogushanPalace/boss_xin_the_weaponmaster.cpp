/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"
#include "Player.h"
#include "mogu_shan_palace.h"

enum Spells
{
    SPELL_BLADES_1           = 124531,
    SPELL_BLADES_2           = 124534,

    SPELL_TURN_ON_BLADES     = 119315,
    SPELL_TURN_OFF_BLADES    = 124540,
    SPELL_BLADE_TRAP_VISUAL  = 119388,

    SPELL_SUMMON_STAFF       = 120109,
    SPELL_SUMMON_AXES        = 120083,

    SPELL_STAFF_FIREBALL     = 119528,

    SPELL_SUMMON_FIRE_RING   = 119541,
    SPELL_FIRE_RING_AURA     = 119544,
    SPELL_FIRE_RING_VISUAL   = 119549,
    SPELL_EXPLODE_FIRE_RING  = 119590,

    SPELL_GROUND_SLAM        = 119684,

    SPELL_INCITING_ROAR      = 122959,

    SPELL_DART_AURA          = 120143,
    SPELL_DART_DAMAGE        = 120142
};

enum Creatures
{
    CREATURE_SWORD_LAUNCHER  = 59481,
};

enum Actions
{
    ACTION_ACTIVATE_BUTTON   = 0,
    ACTION_DEACTIVATE_BUTTON = 1,
};

// Constants to define which world triggers can be chosen as blade launchers
static const float m_fLessThanYPos = -2647.7f;
static const float m_fMoreThanYPos = -2578.6f;

static const Position m_fCenterPos = { -4632.8f, -2615.f, 21.8f, 0.0f };

class boss_xin_the_weaponmaster : public CreatureScript
{
    public:
        boss_xin_the_weaponmaster() : CreatureScript("boss_xin_the_weaponmaster") { }

        enum eEvents
        {
            EVENT_RING_OF_FIRE          = 1,
            EVENT_GROUND_SLAM           = 2,
            EVENT_INCITING_ROAR         = 3,
            EVENT_AXES_ACTIVATE         = 4,
            EVENT_CHECK_SWORDS          = 5,
            EVENT_CHECK_CROSSBOW        = 6,
            EVENT_GROUND_SLAM_CANCEL    = 7,
        };

        enum eTalks
        {
            TALK_INTRO,
            TALK_AGGRO,
            TALK_AXES,
            EMOTE_AXES,
            EMOTE_BLADES,
            TALK_BLADES,
            TALK_CROSSBOW,
            EMOTE_CROSSBOW,
            TALK_DEATH,
            EMOTE_DEATH,
            TALK_SLAY,
            TALK_RESET
        };

        struct boss_xin_the_weaponmaster_AI : public BossAI
        {
            boss_xin_the_weaponmaster_AI(Creature* creature) : BossAI(creature, DATA_XIN_THE_WEAPONMASTER)
            {
                m_bHasYelled = false;
                targetGUID = 0;
            }

            bool m_bHasYelled;
            std::list<Creature*> gems;
            uint64 targetGUID;

            void InitializeAI() OVERRIDE
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS_DEST, true);
                me->HandleEmoteStateCommand(EMOTE_STATE_SIT_CHAIR_HIGH);
                targetGUID = 0;

                BossAI::InitializeAI();
            }

            void InitializeGems()
            {
                gems.clear();
                GetCreatureListWithEntryInGrid(gems, me, CREATURE_FAINTLY_GLOWING_GEM, 200.0f);
            }

            void ActivateGem()
            {
                if (gems.empty())
                    return;

                Creature* gem = Skyfire::Containers::SelectRandomContainerElement(gems);
                if (gem->AI())
                    gem->AI()->DoAction(ACTION_ACTIVATE_BUTTON);
                gems.remove(gem);
            }

            void Reset() OVERRIDE
            {
                _Reset();
                events.Reset();
                InitializeGems();
                targetGUID = 0;

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                for (Creature* gem : gems)
                    if (gem->AI())
                        gem->AI()->DoAction(ACTION_DEACTIVATE_BUTTON);
            }

            void JustReachedHome() OVERRIDE
            {
                me->HandleEmoteStateCommand(EMOTE_STATE_SIT_CHAIR_HIGH);
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!m_bHasYelled && who && who->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    m_bHasYelled = true;
                    Talk(TALK_INTRO);
                }

                ScriptedAI::MoveInLineOfSight(who);
            }

            void SetKingDoor(bool open)
            {
                if (instance)
                    instance->HandleGameObject(0, open, ObjectAccessor::GetGameObject(*me, instance->GetData64(GO_DOOR_BEFORE_KING)));
            }

            void EnterEvadeMode() OVERRIDE
            {
                _EnterEvadeMode();

                if (me->HasUnitState(UNIT_STATE_CANNOT_TURN))
                    me->ClearUnitState(UNIT_STATE_CANNOT_TURN);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->GetMotionMaster()->MoveTargetedHome();

                DeactivateWeapons();
                Talk(TALK_RESET);

                SetKingDoor(true);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(TALK_SLAY);
            }

            void DeactivateWeapons()
            {
                DeactivateAxes();
                DeactivateStaves();
                DeactivateCrossbows();
                DeactivateSwordLaunchers();
            }

            void DeactivateAxes()
            {
                if (instance)
                    instance->SetData(TYPE_ACTIVATE_ANIMATED_AXE, 0);
            }

            void DeactivateStaves()
            {
                std::list<Creature*> m_lStaveList;

                GetCreatureListWithEntryInGrid(m_lStaveList, me, CREATURE_ANIMATED_STAFF, 200.0f);

                for (Creature* creature : m_lStaveList)
                {
                    creature->CombatStop(true);

                    if (creature->AI())
                        creature->AI()->EnterEvadeMode();
                }
            }

            bool DoActivateSwordLaunchers()
            {
                if (me->GetHealthPct() > 66.5f)
                    return false;

                ActivateGem();

                std::list<Creature*> m_lLaunchersList;

                GetCreatureListWithEntryInGrid(m_lLaunchersList, me, CREATURE_SWORD_LAUNCHER, 200.0f);
                m_lLaunchersList.remove_if([=](WorldObject* obj) { return obj->GetPositionY() > m_fLessThanYPos && obj->GetPositionY() < m_fMoreThanYPos; });

                if (m_lLaunchersList.empty())
                {
                    SF_LOG_ERROR("scripts", "Xin the Weaponmaster found no world triggers for blade launcher in instance %u", me->GetInstanceId());
                    return true;
                }

                for (Creature* pTrap : m_lLaunchersList)
                {
                    me->AddAura(SPELL_BLADE_TRAP_VISUAL, pTrap);
                    me->AddAura(SPELL_TURN_ON_BLADES, pTrap);
                }

                Talk(EMOTE_BLADES);
                Talk(TALK_BLADES);

                return true;
            }

            void DeactivateSwordLaunchers()
            {
                std::list<Creature*> m_lLaunchersList;

                GetCreatureListWithEntryInGrid(m_lLaunchersList, me, CREATURE_SWORD_LAUNCHER, 200.0f);

                for (Creature* creature : m_lLaunchersList)
                    creature->RemoveAllAuras();
            }

            bool DoActivateCrossbows()
            {
                if (me->GetHealthPct() > 35.5f)
                    return false;

                ActivateGem();

                std::list<Creature*> m_lCrossbowList;

                GetCreatureListWithEntryInGrid(m_lCrossbowList, me, CREATURE_CROSSBOW, 200.0f);
                if (m_lCrossbowList.size() > 3)
                    m_lCrossbowList.resize(3);

                for (Creature* pCrossbow : m_lCrossbowList)
                {
                    pCrossbow->CastSpell(pCrossbow, SPELL_DART_AURA, true);
                    pCrossbow->RemoveAurasDueToSpell(SPELL_PERMANENT_FEIGN_DEATH);
                }

                Talk(EMOTE_CROSSBOW);
                Talk(TALK_CROSSBOW);

                return true;
            }

            void DeactivateCrossbows()
            {
                std::list<Creature*> m_lCrossbowList;

                GetCreatureListWithEntryInGrid(m_lCrossbowList, me, CREATURE_CROSSBOW, 200.0f);

                for (Creature* pCrossbow : m_lCrossbowList)
                {
                    pCrossbow->RemoveAurasDueToSpell(SPELL_DART_AURA);
                    pCrossbow->CastSpell(pCrossbow, SPELL_PERMANENT_FEIGN_DEATH, true);
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                events.ScheduleEvent(EVENT_RING_OF_FIRE, 15000);
                events.ScheduleEvent(EVENT_GROUND_SLAM, urand(10000, 15000));
                events.ScheduleEvent(EVENT_INCITING_ROAR, urand(15000, 25000));
                events.ScheduleEvent(EVENT_AXES_ACTIVATE, urand(15000, 25000));
                events.ScheduleEvent(EVENT_CHECK_SWORDS, 1000);
                events.ScheduleEvent(EVENT_CHECK_CROSSBOW, 1000);
                InitializeGems();
                me->HandleEmoteStateCommand(EMOTE_STATE_STAND);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(TALK_AGGRO);

                SetKingDoor(false);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                Talk(TALK_DEATH);
                Talk(EMOTE_DEATH);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                DeactivateWeapons();

                SetKingDoor(true);
            }

            void DoAction(int32 /*actionId*/) OVERRIDE
            {
                DeactivateCrossbows();
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
                        case EVENT_RING_OF_FIRE:
                            if (instance)
                                instance->SetData(TYPE_ACTIVATE_ANIMATED_STAFF, 0);
                            events.ScheduleEvent(EVENT_RING_OF_FIRE, 35000);
                            break;
                        case EVENT_GROUND_SLAM:
                            if (Unit* vict = me->GetVictim())
                            {
                                targetGUID = vict->GetGUID();
                                me->PrepareChanneledCast(me->GetAngle(vict), SPELL_GROUND_SLAM);
                            }

                            events.ScheduleEvent(EVENT_GROUND_SLAM_CANCEL, 3500);
                            events.ScheduleEvent(EVENT_GROUND_SLAM, urand(10000, 15000));
                            break;
                        case EVENT_INCITING_ROAR:
                            DoCast(SPELL_INCITING_ROAR);
                            events.ScheduleEvent(EVENT_INCITING_ROAR, 30000);
                            break;
                        case EVENT_AXES_ACTIVATE:
                            if (instance)
                                instance->SetData(TYPE_ACTIVATE_ANIMATED_AXE, 1);
                            Talk(EMOTE_AXES);
                            Talk(TALK_AXES);
                            break;
                        case EVENT_CHECK_SWORDS:
                            if (!DoActivateSwordLaunchers())
                                events.ScheduleEvent(EVENT_CHECK_SWORDS, 1000);
                            break;
                        case EVENT_CHECK_CROSSBOW:
                            if (!DoActivateCrossbows())
                                events.ScheduleEvent(EVENT_CHECK_CROSSBOW, 1000);
                            break;
                        case EVENT_GROUND_SLAM_CANCEL:
                            me->RemoveChanneledCast(targetGUID);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_xin_the_weaponmaster_AI(creature);
        }
};

class npc_animated_staff : public CreatureScript
{
    public:
        npc_animated_staff() : CreatureScript("npc_animated_staff") { }

        enum eActions
        {
            ACTION_ACTIVATE,
        };

        enum eEvents
        {
            EVENT_SUMMON_RING_OF_FIRE = 1,
            EVENT_FIREBALL            = 2,
            EVENT_MOVE                = 3,
            EVENT_FIRE_RING_VISUAL    = 4
        };

        struct npc_animated_staff_AI : public ScriptedAI
        {
            npc_animated_staff_AI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                DoCast(SPELL_PERMANENT_FEIGN_DEATH);
                me->SetDisplayId(42195);
                me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID, 76364);
                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                SetCombatMovement(false);
            }

            void MovementInform(uint32 type, uint32 /*pointId*/) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                me->SetVisible(true);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE { }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_ACTIVATE)
                {
                    me->RemoveAurasDueToSpell(SPELL_PERMANENT_FEIGN_DEATH);
                    events.ScheduleEvent(EVENT_SUMMON_RING_OF_FIRE, urand(9000, 14000));
                    events.ScheduleEvent(EVENT_FIREBALL, urand(2000, 3000));
                    events.ScheduleEvent(EVENT_MOVE, 200);
                    me->GetMotionMaster()->MovePoint(1, m_fCenterPos);
                    DoZoneInCombat();
                }
            }

            void Move()
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    float x, y;
                    float dist = me->GetDistance(target);
                    GetPositionWithDistInOrientation(me, dist < 3.0f ? frand(dist, 3.0f) : frand(3.0f, dist), me->GetAngle(target), x, y);
                    me->GetMotionMaster()->MovePoint(0, x, y, me->GetPositionZ());
                }

                events.ScheduleEvent(EVENT_MOVE, urand(4000, 7000));
            }

            void EnterEvadeMode() OVERRIDE
            {
                _EnterEvadeMode();

                events.Reset();
                me->SetVisible(false);
                me->SetReactState(REACT_PASSIVE);
                me->CombatStop(true);
                // Add the aura here to prevent our function cancelling movement
                me->AddAura(SPELL_PERMANENT_FEIGN_DEATH, me);

                me->GetMotionMaster()->MovePoint(0, me->GetHomePosition());
            }

            void HandleRing()
            {
                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                if (!target)
                    return;

                me->GetMotionMaster()->MovePoint(0, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                me->CastSpell(target, SPELL_SUMMON_FIRE_RING, false);
                events.ScheduleEvent(EVENT_FIRE_RING_VISUAL, 600);
                events.ScheduleEvent(EVENT_SUMMON_RING_OF_FIRE, urand(16000, 22000));
            }

            void HandleFireball()
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_STAFF_FIREBALL);

                events.ScheduleEvent(EVENT_FIREBALL, urand(4000, 8000));
                events.RescheduleEvent(EVENT_MOVE, urand(1000, 2000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUMMON_RING_OF_FIRE:
                            HandleRing();
                            break;
                        case EVENT_FIREBALL:
                            HandleFireball();
                            break;
                        case EVENT_MOVE:
                            Move();
                            break;
                        case EVENT_FIRE_RING_VISUAL:
                            DoCast(SPELL_FIRE_RING_VISUAL);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_animated_staff_AI(creature);
        }
};

class npc_fire_ring_trigger : public CreatureScript
{
    public:
        npc_fire_ring_trigger() : CreatureScript("npc_fire_ring_trigger") { }

        enum eEvents
        {
            EVENT_UNSUMMON = 1,
            EVENT_MOVE     = 2,
            EVENT_MOVE_MID = 3
        };

        struct npc_fire_ring_triggerAI : public ScriptedAI
        {
            npc_fire_ring_triggerAI(Creature* creature) : ScriptedAI(creature) { }

            float x, y;
            float m_fArcPoint;
            bool m_bCounterClockwise;
            bool m_bFirstPoint;
            EventMap events;

            void InitializeAI() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PACIFIED);
                me->GetMotionMaster()->Clear();

                events.ScheduleEvent(EVENT_MOVE, 500);
                x = me->GetPositionX();
                y = me->GetPositionY();
                m_fArcPoint = 0.0f;

                m_bCounterClockwise = urand(0, 1);

                m_bFirstPoint = true;
                me->SetDisplayId(39810);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == 0)
                {
                    if (m_fArcPoint < 11)
                        events.ScheduleEvent(EVENT_MOVE, 200);
                    else
                    {
                        events.CancelEvent(EVENT_MOVE);
                        events.ScheduleEvent(EVENT_MOVE_MID, 200);
                    }
                }
                else
                {
                    DoCast(SPELL_EXPLODE_FIRE_RING);
                    events.ScheduleEvent(EVENT_UNSUMMON, 2500);
                }
            }

            void Move()
            {
                if (!me->HasAura(SPELL_FIRE_RING_AURA) && !m_bFirstPoint)
                    DoCast(me, SPELL_FIRE_RING_AURA, true);

                float newX = x + 5.0f * std::cos(m_fArcPoint * M_PI / 5);
                float newY = m_bCounterClockwise ? y - 5.0f * std::sin(m_fArcPoint * M_PI / 5) : y + 5.0f * std::sin(m_fArcPoint * M_PI / 5);
                me->GetMotionMaster()->MovePoint(0, newX, newY, me->GetPositionZ());

                ++m_fArcPoint;

                m_bFirstPoint = false;
            }

            void MoveMiddle()
            {
                me->RemoveAllAuras();
                me->GetMotionMaster()->MovePoint(1, x, y, me->GetPositionZ());
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE:
                            Move();
                            break;
                        case EVENT_UNSUMMON:
                            me->DespawnOrUnsummon();
                            break;
                        case EVENT_MOVE_MID:
                            MoveMiddle();
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_fire_ring_triggerAI(creature);
        }
};

class npc_animated_axe : public CreatureScript
{
    public:
        npc_animated_axe() : CreatureScript("npc_animated_axe") { }

        enum eActions : int32
        {
            ACTION_START_COMBAT,
            ACTION_STOP_COMBAT
        };

        struct npc_animated_axeAI : public ScriptedAI
        {
            npc_animated_axeAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID, 30316);
                DoCast(SPELL_PERMANENT_FEIGN_DEATH);
                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());

                me->SetReactState(REACT_PASSIVE);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_START_COMBAT)
                {
                    me->RemoveAurasDueToSpell(SPELL_PERMANENT_FEIGN_DEATH);
                    DoCast(SPELL_WHIRLWIND);
                    me->GetMotionMaster()->MoveRandom(50.f);
                }
                else if (actionId == ACTION_STOP_COMBAT)
                    DeactivateAxe();
            }

            void MovementInform(uint32 type, uint32 /*pointId*/) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                me->SetVisible(true);
            }

            void DeactivateAxe()
            {
                me->SetVisible(false);
                me->CombatStop(true);
                me->RemoveAurasDueToSpell(SPELL_WHIRLWIND);
                DoCast(SPELL_PERMANENT_FEIGN_DEATH);
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(0, me->GetHomePosition());
            }

            void EnterEvadeMode() OVERRIDE { }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_animated_axeAI(creature);
        }
};

// Faintly glowing gem 63808
class npc_faintly_glowing_gem : public CreatureScript
{
    public:
        npc_faintly_glowing_gem() : CreatureScript("npc_faintly_glowing_gem") { }

        enum eSpells
        {
            SPELL_GLOWING_GEM  = 124524,
        };

        enum eEvents
        {
            EVENT_SPECIAL_BEAM = 1,
        };

        struct npc_faintly_glowing_gemAI : public ScriptedAI
        {
            npc_faintly_glowing_gemAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetDisplayId(35408);

                if (me->HasAura(SPELL_GLOWING_GEM))
                    me->RemoveAura(SPELL_GLOWING_GEM);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_ACTIVATE_BUTTON:
                        if (!me->HasAura(SPELL_GLOWING_GEM))
                            me->AddAura(SPELL_GLOWING_GEM, me);

                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        events.ScheduleEvent(EVENT_SPECIAL_BEAM, 6 * IN_MILLISECONDS);
                        break;
                    case ACTION_DEACTIVATE_BUTTON:
                        if (me->HasAura(SPELL_GLOWING_GEM))
                            me->RemoveAura(SPELL_GLOWING_GEM);

                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        break;
                }
            }

            void EnterEvadeMode() OVERRIDE { }

            void OnSpellClick(Unit* /*clicker*/, bool& /*result*/) OVERRIDE
            {
                DoAction(ACTION_DEACTIVATE_BUTTON);

                if (Creature* Xin = GetClosestCreatureWithEntry(me, CREATURE_XIN_THE_WEAPONMASTER, 200.0f, true))
                    if (Xin->AI())
                        Xin->AI()->DoAction(ACTION_ACTIVATE_BUTTON);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                    if (eventId == EVENT_SPECIAL_BEAM)
                        me->RemoveAura(SPELL_BLADES_1);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_faintly_glowing_gemAI(creature);
        }
};

// Quilen Guardian - 61387
class npc_mp_quilen_guardian : public CreatureScript
{
    public:
        npc_mp_quilen_guardian() : CreatureScript("npc_mp_quilen_guardian") { }

        enum eSpells
        {
            SPELL_CARNIVOROUS_BITE = 122962,
            SPELL_LEAPING_RUSH     = 121190,
        };

        enum eEvents
        {
            EVENT_CARNIVOROUS_BITE = 1,
            EVENT_LEAPING_RUSH     = 2,
        };

        struct npc_mp_quilen_guardianAI : public ScriptedAI
        {
            npc_mp_quilen_guardianAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                events.ScheduleEvent(EVENT_LEAPING_RUSH, urand(7000, 9000));
                events.ScheduleEvent(EVENT_CARNIVOROUS_BITE, 3000);
                CallQuilensForHelp(who);
            }

            void CallQuilensForHelp(Unit* attacker)
            {
                std::list<Creature*> Quilens;
                GetCreatureListWithEntryInGrid(Quilens, me, CREATURE_QUILEN_GUARDIAN, 50.0f);

                for (Creature* quilen : Quilens)
                {
                    if (!quilen->IsInCombat())
                    {
                        quilen->Attack(attacker, true);
                        quilen->CastSpell(attacker, SPELL_LEAPING_RUSH, false);
                    }
                }
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
                        case EVENT_LEAPING_RUSH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, LeapingRushPredicate(me)))
                                me->CastSpell(target, SPELL_LEAPING_RUSH, false);

                            events.ScheduleEvent(EVENT_LEAPING_RUSH, urand(7000, 9000));
                            break;
                        case EVENT_CARNIVOROUS_BITE:
                            if (Unit* target = me->GetVictim())
                                me->CastSpell(target, SPELL_CARNIVOROUS_BITE, false);

                            events.ScheduleEvent(EVENT_CARNIVOROUS_BITE, urand(8000, 12000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            struct LeapingRushPredicate
            {
                public:
                    LeapingRushPredicate(Creature const* me) : _me(me) { }
                    bool operator()(Unit const* target) const
                    {
                        return target && target->GetTypeId() == TypeID::TYPEID_PLAYER && target->GetDistance2d(_me) > 5.0f;
                    }

                private:
                    Creature const* _me;
            };
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_mp_quilen_guardianAI(creature);
        }
};

class OnlyTriggerInFrontPredicate
{
    public:
        OnlyTriggerInFrontPredicate(Unit* caster) : _caster(caster) { }

        bool operator()(WorldObject* target) const
        {
            return !_caster->isInFront(target, M_PI / 5) || target->GetGUID() == _caster->GetGUID() || target->GetDistance2d(_caster) > 80.f;
        }

    private:
        Unit* _caster;
};

struct CheckIfValidTarget
{
    bool operator ()(WorldObject const* target) const
    {
        return target->GetEntry() != CREATURE_SWORD_LAUNCHER;
    }
};

class spell_dart_damage : public SpellScriptLoader
{
    public:
        spell_dart_damage() : SpellScriptLoader("spell_dart_damage") { }

        class spell_dart_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dart_damage_SpellScript);

            void SelectTarget(std::list<WorldObject*>& targetList)
            {
                if (!GetCaster())
                    return;

                if (targetList.empty())
                {
                    FinishCast(SpellCastResult::SPELL_FAILED_NO_VALID_TARGETS);
                    return;
                }

                std::list<Creature*> targets;
                std::list<WorldObject*> finalTargets;
                GetCreatureListWithEntryInGrid(targets, GetCaster(), CREATURE_SWORD_LAUNCHER, 200.0f);

                targets.remove_if(OnlyTriggerInFrontPredicate(GetCaster()));

                for (WorldObject* target : targetList)
                    for (Creature* trigger : targets)
                        if (target->IsInBetween(GetCaster(), trigger, 2.0f))
                            finalTargets.push_back(target);

                targetList = finalTargets;
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dart_damage_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_dart_damage_SpellScript();
        }
};

class spell_dart : public SpellScriptLoader
{
    public:
        spell_dart() : SpellScriptLoader("spell_dart") { }

        class spell_dart_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dart_SpellScript);

            void SelectTarget(std::list<WorldObject*>& targetList)
            {
                if (!GetCaster())
                    return;

                if (targetList.empty())
                {
                    FinishCast(SpellCastResult::SPELL_FAILED_NO_VALID_TARGETS);
                    return;
                }

                targetList.remove_if(OnlyTriggerInFrontPredicate(GetCaster()));
                targetList.remove_if(CheckIfValidTarget());
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dart_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_dart_SpellScript();
        }
};

class BladeLauncherCheck
{
    public:
        BladeLauncherCheck(Unit* caster) : _caster(caster) { }

        bool operator()(WorldObject* target) const
        {
            return !target->ToCreature() || target->GetEntry() != CREATURE_SWORD_LAUNCHER || target->GetGUID() == _caster->GetGUID();
        }
    private:
        Unit* _caster;
};

class ActiveTrapCheck
{
    public:
        ActiveTrapCheck(uint32 aura) : _aura(aura) { }

        bool operator()(WorldObject* target) const
        {
            return target->ToCreature() && target->ToCreature()->HasAura(_aura);
        }

    private:
        uint32 _aura;
};

class spell_activate_blades : public SpellScriptLoader
{
    public:
        spell_activate_blades() : SpellScriptLoader("spell_activate_blades") { }

        class spell_activate_blades_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_activate_blades_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_BLADES_1) || !sSpellMgr->GetSpellInfo(SPELL_BLADES_2))
                    return false;
                return true;
            }

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(BladeLauncherCheck(GetCaster()));
                targets.remove_if(ActiveTrapCheck(SPELL_BLADES_1));
                targets.remove_if(ActiveTrapCheck(SPELL_BLADES_2));

                if (targets.size() > 1)
                    targets.resize(1);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_activate_blades_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_activate_blades_SpellScript();
        }
};

class spell_turn_off_blades : public SpellScriptLoader
{
    public:
        spell_turn_off_blades() : SpellScriptLoader("spell_turn_off_blades") { }

        class spell_turn_off_blades_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_turn_off_blades_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TURN_OFF_BLADES))
                    return false;
                return true;
            }

            void HandlePreCast()
            {
                if (GetCaster())
                {
                    GetCaster()->CastSpell(GetCaster(), SPELL_BLADES_1, false);
                    GetCaster()->CastSpell(GetCaster(), SPELL_BLADES_2, false);
                }
            }

            void Register() OVERRIDE
            {
                BeforeCast += SpellCastFn(spell_turn_off_blades_SpellScript::HandlePreCast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_turn_off_blades_SpellScript();
        }
};

class spell_groundslam_xin : public SpellScriptLoader
{
    public:
        spell_groundslam_xin() : SpellScriptLoader("spell_groundslam_xin") { }

        class spell_groundslam_xin_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_groundslam_xin_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_GROUND_SLAM))
                    return false;
                return true;
            }

            void HandlePrecast()
            {
                if (GetCaster())
                    GetCaster()->AddUnitState(UNIT_STATE_CANNOT_TURN);
            }

            void HandleAftercast()
            {
                if (GetCaster())
                    GetCaster()->ClearUnitState(UNIT_STATE_CANNOT_TURN);
            }

            void Register() OVERRIDE
            {
                BeforeCast += SpellCastFn(spell_groundslam_xin_SpellScript::HandlePrecast);
                AfterCast += SpellCastFn(spell_groundslam_xin_SpellScript::HandleAftercast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_groundslam_xin_SpellScript();
        }
};

class spell_crossbow_xin : public SpellScriptLoader
{
    public:
        spell_crossbow_xin() : SpellScriptLoader("spell_crossbow_xin") { }

        class spell_crossbow_xin_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_crossbow_xin_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DART_DAMAGE))
                    return false;
                return true;
            }

            void SelectTarget(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                WorldObject* target = Skyfire::Containers::SelectRandomContainerElement(targets);
                targets.clear();
                targets.push_back(target);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_crossbow_xin_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_crossbow_xin_SpellScript();
        }
};

void AddSC_boss_xin_the_weaponmaster()
{
    new boss_xin_the_weaponmaster();
    new npc_animated_axe();
    new npc_animated_staff();
    new npc_fire_ring_trigger();
    new npc_faintly_glowing_gem();
    new npc_mp_quilen_guardian();
    new spell_dart();
    new spell_dart_damage();
    new spell_activate_blades();
    new spell_turn_off_blades();
    new spell_groundslam_xin();
    new spell_crossbow_xin();
}
