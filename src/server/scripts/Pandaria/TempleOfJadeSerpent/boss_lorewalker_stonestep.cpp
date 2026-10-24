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

enum LorewalkerEvents
{
    EVENT_INTRO_0 = 1,
    EVENT_INTRO_1,
    EVENT_INTRO_2,
    EVENT_INTRO_3,
    EVENT_SUN_0,
    EVENT_SUN_1,
    EVENT_SUN_2,
    EVENT_SUN_3,
    EVENT_SUN_4,
    EVENT_STRIFE_0,
    EVENT_STRIFE_1,
    EVENT_STRIFE_2,
    EVENT_STRIFE_3,
    EVENT_STRIFE_4,
    EVENT_CHECK_WIPE,

    EVENT_SUNFIRE_RAYS,
    EVENT_ZAO_ENTER_COMBAT,
    EVENT_ZAO_TALK_AGGRO,
    EVENT_ZAO_ATTACK,
    EVENT_HAUNTING_GAZE,
    EVENT_AGONY,
    EVENT_CLAW,
    EVENT_SERRATED_SLASH,
    EVENT_SHROUD_OF_GOLD,
    EVENT_SLOW,
    EVENT_AERIALISTS_KICK,
    EVENT_THROW_TORCH,
    EVENT_SONGBIRD_SERENADE,
    EVENT_BUBBLE_SPRAY,
    EVENT_FISH_TALK_1,
    EVENT_FISH_TALK_2,
    EVENT_FISH_TALK_3,
};

enum LorewalkerTexts
{
    SAY_INTRO_0             = 0,
    SAY_INTRO_1             = 1,
    SAY_INTRO_2             = 2,
    SAY_INTRO_3             = 3,
    SAY_ZAO_APPEARS_0       = 4,
    SAY_ZAO_APPEARS_1       = 5,
    SAY_ZAO_APPEARS_2       = 6,
    SAY_ZAO_APPEARS_4       = 7,
    SAY_STRIFE_0            = 9,
    SAY_STRIFE_1            = 10,
    SAY_STRIFE_2            = 11,
    SAY_STRIFE_3            = 12,
    SAY_STRIFE_4            = 13,

    SAY_ZAO_INTRO           = 0,
    SAY_ZAO_AGGRO           = 1,

    SAY_FISH_0              = 0,
    SAY_FISH_1              = 1,
    SAY_FISH_2              = 2,
    SAY_FISH_3              = 3,
};

enum LorewalkerSpells
{
    SPELL_SUNFIRE_RAYS      = 107223,
    SPELL_SUN               = 107349,
    SPELL_GROW_LOW          = 104921,
    SPELL_SHOOT_SUN         = 112084,
    SPELL_HELLFIRE_ARROW    = 113017,
    SPELL_HAUNTING_GAZE_DMG = 114646,
    SPELL_AGONY             = 114571,
    SPELL_SHA_CORRUPTION_SP = 115086,
    SPELL_INTENSITY_AURA    = 113316,
    SPELL_INTENSITY         = 113315,
    SPELL_ULTIMATE_POWER    = 113309,
    SPELL_DISSIPATION       = 113379,
    SPELL_CLAW              = 31289,
    SPELL_SERRATED_SLASH    = 128051,
    SPELL_SHROUD_OF_GOLD    = 88023,
    SPELL_SLOW              = 31589,
    SPELL_AERIALISTS_KICK   = 114805,
    SPELL_THROW_TORCH       = 114803,
    SPELL_SONGBIRD_SERENADE = 114826,
    SPELL_BUBBLE_SPRAY      = 114811,
    SPELL_SCROLL_FLOOR      = 107350,
    SPELL_JADE_ENERGY_2     = 111452,
    SPELL_SHA_BURNING       = 111588,
    SPELL_SHA_EXPLOSION     = 111579,
    SPELL_DEATH             = 98391,
};

enum LorewalkerActions
{
    ACTION_ZAO              = 0,
    ACTION_TRIAL            = 1,
    ACTION_RESET            = 2,

    ACTION_SUN_SELECTABLE   = 0,
    ACTION_ZAO_TALK         = 1,
    ACTION_ZAO_COMBAT       = 2,
    ACTION_HAUNT_ZAO        = 0,
    ACTION_TRIAL_IN_COMBAT  = 1,
};

enum LorewalkerMisc
{
    CREDIT_STONESTEP          = 56843,
    QUEST_RESTORE_JADE_PURITY = 31355,
    CURRENCY_JUSTICE_POINTS   = 395,
    FACTION_HOSTILE           = 14,
};

Position const LorewalkerWalkPos[2] =
{
    { 838.033f, -2480.518f, 176.744f, 0.0f },
    { 834.643f, -2490.361f, 179.897f, 0.0f },
};

float const LorewalkerFacing = 1.239f;

static void CreditRestoreJadePurity(Map* map)
{
    Map::PlayerList const& players = map->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        if (Player* player = itr->GetSource())
            if (player->GetQuestStatus(QUEST_RESTORE_JADE_PURITY) == QUEST_STATUS_INCOMPLETE)
                player->KilledMonsterCredit(CREDIT_STONESTEP);
}

class boss_lorewalker_stonestep : public CreatureScript
{
public:
    boss_lorewalker_stonestep() : CreatureScript("boss_lorewalker_stonestep") { }

    struct boss_lorewalker_stonestepAI : public BossAI
    {
        boss_lorewalker_stonestepAI(Creature* creature) : BossAI(creature, DATA_LOREWALKER)
        {
            _introDone = false;
        }

        void InitializeAI() OVERRIDE
        {
            if (instance->GetBossState(DATA_LOREWALKER) == DONE)
            {
                _introDone = true;
                DoCast(me, SPELL_MEDITATION);
                me->SetReactState(REACT_PASSIVE);
            }
            else
            {
                _Reset();
                _introDone = false;
                me->GetMotionMaster()->MoveTargetedHome();
            }
        }

        void Reset() OVERRIDE { }

        void DoAction(int32 action) OVERRIDE
        {
            switch (action)
            {
                case ACTION_ZAO:
                    events.ScheduleEvent(EVENT_SUN_0, 500);
                    break;
                case ACTION_TRIAL:
                    events.ScheduleEvent(EVENT_STRIFE_0, 500);
                    break;
                case ACTION_RESET:
                    events.Reset();
                    break;
                default:
                    break;
            }
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            if (!_introDone && who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                _introDone = true;
                events.ScheduleEvent(EVENT_INTRO_0, 500);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHECK_WIPE:
                        if (instance->IsWipe())
                            instance->SetData(DATA_LOREWALKER, FAIL);
                        else
                            events.ScheduleEvent(EVENT_CHECK_WIPE, 1 * IN_MILLISECONDS);
                        break;
                    case EVENT_INTRO_0:
                        Talk(SAY_INTRO_0);
                        events.ScheduleEvent(EVENT_INTRO_1, 8019);
                        break;
                    case EVENT_INTRO_1:
                        Talk(SAY_INTRO_1);
                        events.ScheduleEvent(EVENT_INTRO_2, 16162);
                        break;
                    case EVENT_INTRO_2:
                        Talk(SAY_INTRO_2);
                        events.ScheduleEvent(EVENT_INTRO_3, 9578);
                        break;
                    case EVENT_INTRO_3:
                        Talk(SAY_INTRO_3);
                        break;
                    case EVENT_SUN_0:
                        me->RemoveAurasDueToSpell(SPELL_ROOT_SELF);
                        Talk(SAY_ZAO_APPEARS_0);
                        events.ScheduleEvent(EVENT_SUN_1, 9641);
                        break;
                    case EVENT_SUN_1:
                        Talk(SAY_ZAO_APPEARS_1);
                        me->GetMotionMaster()->MovePoint(0, LorewalkerWalkPos[0]);
                        events.ScheduleEvent(EVENT_SUN_2, 12220);
                        break;
                    case EVENT_SUN_2:
                        Talk(SAY_ZAO_APPEARS_2);
                        me->GetMotionMaster()->MovePoint(0, LorewalkerWalkPos[1]);
                        events.ScheduleEvent(EVENT_SUN_3, 8280);
                        break;
                    case EVENT_SUN_3:
                        me->SetFacingTo(LorewalkerFacing);
                        instance->SetData(DATA_SUN, DONE);
                        if (Creature* zao = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ZAO)))
                            if (zao->IsAIEnabled)
                                zao->AI()->DoAction(ACTION_ZAO_TALK);
                        events.ScheduleEvent(EVENT_SUN_4, 4214);
                        break;
                    case EVENT_SUN_4:
                        DoCast(me, SPELL_MEDITATION);
                        Talk(SAY_ZAO_APPEARS_4);
                        events.ScheduleEvent(EVENT_CHECK_WIPE, 1 * IN_MILLISECONDS);
                        break;
                    case EVENT_STRIFE_0:
                        me->RemoveAurasDueToSpell(SPELL_ROOT_SELF);
                        Talk(SAY_STRIFE_0);
                        events.ScheduleEvent(EVENT_STRIFE_1, 8790);
                        break;
                    case EVENT_STRIFE_1:
                        Talk(SAY_STRIFE_1);
                        me->GetMotionMaster()->MovePoint(0, LorewalkerWalkPos[0]);
                        events.ScheduleEvent(EVENT_STRIFE_2, 10780);
                        break;
                    case EVENT_STRIFE_2:
                        Talk(SAY_STRIFE_2);
                        me->GetMotionMaster()->MovePoint(0, LorewalkerWalkPos[1]);
                        events.ScheduleEvent(EVENT_STRIFE_3, 9790);
                        break;
                    case EVENT_STRIFE_3:
                        Talk(SAY_STRIFE_3);
                        me->SetFacingTo(LorewalkerFacing);
                        ReleaseTrial();
                        events.ScheduleEvent(EVENT_STRIFE_4, 6480);
                        break;
                    case EVENT_STRIFE_4:
                        DoCast(me, SPELL_MEDITATION);
                        Talk(SAY_STRIFE_4);
                        events.ScheduleEvent(EVENT_CHECK_WIPE, 1 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        void ReleaseTrial()
        {
            if (Creature* peril = me->FindNearestCreature(NPC_STRIFE, 60.0f))
            {
                peril->SetReactState(REACT_AGGRESSIVE);
                peril->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, peril);

                if (Creature* osong = me->SummonCreature(NPC_OSONG, 842.752f, -2468.911f, 174.959f))
                    if (osong->IsAIEnabled)
                        osong->AI()->AttackStart(peril);
            }

            if (Creature* strife = me->FindNearestCreature(NPC_PERIL, 60.0f))
            {
                strife->SetReactState(REACT_AGGRESSIVE);
                strife->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, strife);
            }
        }

        bool _introDone;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetJadeSerpentAI<boss_lorewalker_stonestepAI>(creature);
    }
};

class npc_sun : public CreatureScript
{
public:
    npc_sun() : CreatureScript("npc_sun") { }

    struct npc_sunAI : public ScriptedAI
    {
        npc_sunAI(Creature* creature) : ScriptedAI(creature) { }

        void InitializeAI() OVERRIDE
        {
            DoCast(me, SPELL_SUN);
            DoCast(me, SPELL_GROW_LOW);
            me->AddUnitState(UNIT_STATE_ROOT);
            me->SetDisableGravity(true);
            me->SetOrientation(LorewalkerFacing);
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_SUN_SELECTABLE)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                _events.ScheduleEvent(EVENT_SUNFIRE_RAYS, 2 * IN_MILLISECONDS);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            _events.Update(diff);

            if (_events.ExecuteEvent() == EVENT_SUNFIRE_RAYS)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(target, SPELL_SUNFIRE_RAYS, true);
                _events.ScheduleEvent(EVENT_SUNFIRE_RAYS, urand(2, 5) * IN_MILLISECONDS);
            }
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_sunAI(creature);
    }
};

class npc_zao : public CreatureScript
{
public:
    npc_zao() : CreatureScript("npc_zao") { }

    struct npc_zaoAI : public ScriptedAI
    {
        npc_zaoAI(Creature* creature) : ScriptedAI(creature), _summons(creature)
        {
            _instance = creature->GetInstanceScript();
            _corrupted = false;
        }

        void InitializeAI() OVERRIDE
        {
            me->AddUnitState(UNIT_STATE_ROOT);
        }

        void DamageTaken(Unit* attacker, uint32& /*damage*/) OVERRIDE
        {
            Creature* sha = attacker->ToCreature();
            if (!sha || (sha->GetEntry() != NPC_HAUNTING_SHA && sha->GetEntry() != NPC_HAUNTING_SHA_2))
                return;

            sha->DespawnOrUnsummon();
            _corrupted = true;
            me->setFaction(FACTION_HOSTILE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoCast(me, SPELL_SHA_CORRUPTION_2);
            if (_instance)
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            _summons.Summon(summon);
        }

        void EnterEvadeMode() OVERRIDE
        {
            ScriptedAI::EnterEvadeMode();
            _summons.DespawnAll();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (_instance)
            {
                _instance->SetData(DATA_ZAO, DONE);
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                CreditRestoreJadePurity(me->GetMap());
            }
            _summons.DespawnAll();
        }

        void DoAction(int32 action) OVERRIDE
        {
            switch (action)
            {
                case ACTION_ZAO_COMBAT:
                    _events.Reset();
                    _events.ScheduleEvent(EVENT_ZAO_ENTER_COMBAT, 1 * IN_MILLISECONDS);
                    break;
                case ACTION_ZAO_TALK:
                    Talk(SAY_ZAO_INTRO);
                    _events.ScheduleEvent(EVENT_ZAO_ATTACK, 1 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ZAO_ENTER_COMBAT:
                        _events.ScheduleEvent(EVENT_ZAO_TALK_AGGRO, 5500);
                        break;
                    case EVENT_ZAO_TALK_AGGRO:
                        Talk(SAY_ZAO_AGGRO);
                        _events.ScheduleEvent(EVENT_ZAO_ATTACK, 5 * IN_MILLISECONDS);
                        break;
                    case EVENT_ZAO_ATTACK:
                        if (_corrupted)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_HELLFIRE_ARROW, true);
                        }
                        else
                        {
                            std::list<Creature*> suns;
                            me->GetCreatureListWithEntryInGrid(suns, NPC_ZAO_SUN, 100.0f);
                            for (Creature* sun : suns)
                                DoCast(sun, SPELL_SHOOT_SUN);
                        }
                        _events.ScheduleEvent(EVENT_ZAO_ATTACK, 3 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* _instance;
        SummonList _summons;
        EventMap _events;
        bool _corrupted;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_zaoAI(creature);
    }
};

class npc_haunting_sha : public CreatureScript
{
public:
    npc_haunting_sha() : CreatureScript("npc_haunting_sha") { }

    struct npc_haunting_shaAI : public ScriptedAI
    {
        npc_haunting_shaAI(Creature* creature) : ScriptedAI(creature)
        {
            _instance = creature->GetInstanceScript();
        }

        void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
        {
            if (Creature* zao = GetZao())
                if (zao->IsAIEnabled)
                    zao->AI()->JustSummoned(me);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _events.ScheduleEvent(EVENT_HAUNTING_GAZE, 1 * IN_MILLISECONDS);
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action != ACTION_HAUNT_ZAO)
                return;

            if (Creature* zao = GetZao())
            {
                me->AddThreat(zao, 1000000.0f);
                AttackStart(zao);
            }
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (_instance)
                _instance->SetData(DATA_SUN_STATE, 1);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!me->GetVictim())
            {
                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->GetSource())
                        me->AddThreat(player, 1.0f);

                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    AttackStart(target);
            }

            _events.Update(diff);

            if (_events.ExecuteEvent() == EVENT_HAUNTING_GAZE)
            {
                DoCastVictim(SPELL_HAUNTING_GAZE_DMG);
                _events.ScheduleEvent(EVENT_HAUNTING_GAZE, 2 * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }

    private:
        Creature* GetZao() const
        {
            return _instance ? ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_ZAO)) : NULL;
        }

        InstanceScript* _instance;
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_haunting_shaAI(creature);
    }
};

class npc_strife_peril : public CreatureScript
{
public:
    npc_strife_peril() : CreatureScript("npc_strife_peril") { }

    struct npc_strife_perilAI : public ScriptedAI
    {
        npc_strife_perilAI(Creature* creature) : ScriptedAI(creature)
        {
            _instance = creature->GetInstanceScript();
            _inFight = false;
        }

        void InitializeAI() OVERRIDE
        {
            _inFight = false;
            me->setFaction(FACTION_HOSTILE);
            DoCast(me, SPELL_SHA_CORRUPTION_SP);
            ScriptedAI::InitializeAI();
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            if (attacker->GetTypeId() == TypeID::TYPEID_UNIT && !attacker->IsPet())
            {
                if (me->GetHealth() < me->GetMaxHealth() || me->GetHealth() <= damage)
                    damage = 0;
                return;
            }

            if (StartIntensity())
            {
                uint32 partner = me->GetEntry() == NPC_STRIFE ? NPC_PERIL : NPC_STRIFE;
                if (Creature* other = me->FindNearestCreature(partner, 50.0f))
                    if (other->IsAIEnabled)
                        other->AI()->DoAction(ACTION_TRIAL_IN_COMBAT);
            }
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_TRIAL_IN_COMBAT)
                StartIntensity();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _events.ScheduleEvent(EVENT_AGONY, 1 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            me->RemoveAurasDueToSpell(SPELL_INTENSITY_AURA);
            if (!_instance)
                return;

            _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            _instance->SetData(DATA_TRIAL, DONE);
            CreditRestoreJadePurity(me->GetMap());

            if (_instance->GetData(DATA_LOREWALKER) != DONE)
            {
                me->SetLootRecipient(NULL);
                return;
            }

            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (Player* player = itr->GetSource())
                    player->ModifyCurrency(CURRENCY_JUSTICE_POINTS, DUNGEON_MODE(70, 125) * CURRENCY_PRECISION);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (_events.ExecuteEvent() == EVENT_AGONY)
            {
                DoCastVictim(SPELL_AGONY);
                _events.ScheduleEvent(EVENT_AGONY, 2 * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }

    private:
        bool StartIntensity()
        {
            if (_inFight || me->HasAura(SPELL_INTENSITY_AURA))
                return false;

            _inFight = true;
            DoCast(me, SPELL_INTENSITY_AURA);
            return true;
        }

        InstanceScript* _instance;
        EventMap _events;
        bool _inFight;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_strife_perilAI(creature);
    }
};

class npc_nodding_tiger : public CreatureScript
{
public:
    npc_nodding_tiger() : CreatureScript("npc_nodding_tiger") { }

    struct npc_nodding_tigerAI : public ScriptedAI
    {
        npc_nodding_tigerAI(Creature* creature) : ScriptedAI(creature) { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _events.ScheduleEvent(EVENT_CLAW, 2 * IN_MILLISECONDS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (_events.ExecuteEvent() == EVENT_CLAW)
            {
                DoCast(SPELL_CLAW);
                _events.ScheduleEvent(EVENT_CLAW, 3 * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_nodding_tigerAI(creature);
    }
};

class npc_golden_beetle : public CreatureScript
{
public:
    npc_golden_beetle() : CreatureScript("npc_golden_beetle") { }

    struct npc_golden_beetleAI : public ScriptedAI
    {
        npc_golden_beetleAI(Creature* creature) : ScriptedAI(creature) { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _events.ScheduleEvent(EVENT_SERRATED_SLASH, 2 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_SHROUD_OF_GOLD, 4 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_SLOW, 6 * IN_MILLISECONDS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SERRATED_SLASH:
                        DoCastVictim(SPELL_SERRATED_SLASH);
                        _events.ScheduleEvent(EVENT_SERRATED_SLASH, 10 * IN_MILLISECONDS);
                        break;
                    case EVENT_SHROUD_OF_GOLD:
                        DoCastVictim(SPELL_SHROUD_OF_GOLD);
                        _events.ScheduleEvent(EVENT_SHROUD_OF_GOLD, 5 * IN_MILLISECONDS);
                        break;
                    case EVENT_SLOW:
                        DoCastVictim(SPELL_SLOW);
                        _events.ScheduleEvent(EVENT_SLOW, 15 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_golden_beetleAI(creature);
    }
};

class npc_jiang_xiang : public CreatureScript
{
public:
    npc_jiang_xiang() : CreatureScript("npc_jiang_xiang") { }

    struct npc_jiang_xiangAI : public ScriptedAI
    {
        npc_jiang_xiangAI(Creature* creature) : ScriptedAI(creature) { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _events.ScheduleEvent(EVENT_AERIALISTS_KICK, 2 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_THROW_TORCH, 4 * IN_MILLISECONDS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_AERIALISTS_KICK:
                        DoCastVictim(SPELL_AERIALISTS_KICK);
                        _events.ScheduleEvent(EVENT_AERIALISTS_KICK, 10 * IN_MILLISECONDS);
                        break;
                    case EVENT_THROW_TORCH:
                        DoCastVictim(SPELL_THROW_TORCH);
                        _events.ScheduleEvent(EVENT_THROW_TORCH, 5 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_jiang_xiangAI(creature);
    }
};

class npc_songbird_queen : public CreatureScript
{
public:
    npc_songbird_queen() : CreatureScript("npc_songbird_queen") { }

    struct npc_songbird_queenAI : public ScriptedAI
    {
        npc_songbird_queenAI(Creature* creature) : ScriptedAI(creature) { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _events.ScheduleEvent(EVENT_SONGBIRD_SERENADE, 2 * IN_MILLISECONDS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (_events.ExecuteEvent() == EVENT_SONGBIRD_SERENADE)
            {
                DoCastVictim(SPELL_SONGBIRD_SERENADE);
                _events.ScheduleEvent(EVENT_SONGBIRD_SERENADE, 10 * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_songbird_queenAI(creature);
    }
};

class npc_corrupted_scroll : public CreatureScript
{
public:
    npc_corrupted_scroll() : CreatureScript("npc_corrupted_scroll") { }

    struct npc_corrupted_scrollAI : public ScriptedAI
    {
        npc_corrupted_scrollAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
            me->setRegeneratingHealth(false);
            DoCast(me, SPELL_SCROLL_FLOOR);
            DoCast(me, SPELL_JADE_ENERGY_2);
            DoCast(me, SPELL_GROW_LOW);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            me->RemoveAllAuras();
            DoCast(me, SPELL_SHA_BURNING);
            DoCast(me, SPELL_SHA_EXPLOSION);
            DoCast(me, SPELL_DEATH);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE { }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_corrupted_scrollAI(creature);
    }
};

class npc_talking_fish : public CreatureScript
{
public:
    npc_talking_fish() : CreatureScript("npc_talking_fish") { }

    struct npc_talking_fishAI : public ScriptedAI
    {
        npc_talking_fishAI(Creature* creature) : ScriptedAI(creature) { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_FISH_0);
            _events.ScheduleEvent(EVENT_BUBBLE_SPRAY, 2 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_FISH_TALK_1, 3 * IN_MILLISECONDS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BUBBLE_SPRAY:
                        DoCastVictim(SPELL_BUBBLE_SPRAY);
                        _events.ScheduleEvent(EVENT_BUBBLE_SPRAY, 10 * IN_MILLISECONDS);
                        break;
                    case EVENT_FISH_TALK_1:
                        Talk(SAY_FISH_1);
                        _events.ScheduleEvent(EVENT_FISH_TALK_2, 3500);
                        break;
                    case EVENT_FISH_TALK_2:
                        Talk(SAY_FISH_2);
                        _events.ScheduleEvent(EVENT_FISH_TALK_3, 3 * IN_MILLISECONDS);
                        break;
                    case EVENT_FISH_TALK_3:
                        Talk(SAY_FISH_3);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_talking_fishAI(creature);
    }
};

// 113316 - Intensity
class spell_tjs_intensity : public SpellScriptLoader
{
public:
    spell_tjs_intensity() : SpellScriptLoader("spell_tjs_intensity") { }

    class spell_tjs_intensity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tjs_intensity_AuraScript);

        bool Load() OVERRIDE
        {
            _canProc = false;
            _stacks = 0;
            _previousStacks = 0;
            if (Unit* owner = GetUnitOwner())
                if (Aura* intensity = owner->GetAura(SPELL_INTENSITY))
                    _stacks = intensity->GetStackAmount();
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!_canProc)
                return false;

            Unit* target = eventInfo.GetActionTarget();
            if (target && target->HasAura(SPELL_ULTIMATE_POWER))
                return false;

            Unit* actor = eventInfo.GetActor();
            return actor && actor->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            if (Unit* owner = GetUnitOwner())
            {
                if (Aura* intensity = owner->GetAura(SPELL_INTENSITY))
                {
                    _stacks = intensity->GetStackAmount();
                    if (_stacks == 10)
                    {
                        owner->RemoveAurasDueToSpell(SPELL_INTENSITY);
                        owner->CastSpell(owner, SPELL_ULTIMATE_POWER, false);
                    }
                }

                owner->RemoveAurasDueToSpell(SPELL_DISSIPATION);
            }

            _canProc = false;
        }

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();

            Unit* owner = GetUnitOwner();
            if (_canProc && owner)
            {
                if (Aura* intensity = owner->GetAura(SPELL_INTENSITY))
                {
                    if (_stacks <= _previousStacks)
                    {
                        intensity->ModStackAmount(-1);
                        --_stacks;
                        --_previousStacks;
                    }
                    else
                        _previousStacks = intensity->GetStackAmount();
                }
                else if (!owner->HasAura(SPELL_ULTIMATE_POWER))
                    owner->CastSpell(owner, SPELL_DISSIPATION, false);
            }

            _canProc = true;
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* owner = GetUnitOwner())
            {
                owner->RemoveAurasDueToSpell(SPELL_INTENSITY);
                owner->RemoveAurasDueToSpell(SPELL_DISSIPATION);
                owner->RemoveAurasDueToSpell(SPELL_ULTIMATE_POWER);
            }
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_tjs_intensity_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_tjs_intensity_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_tjs_intensity_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            AfterEffectRemove += AuraEffectRemoveFn(spell_tjs_intensity_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }

        bool _canProc;
        uint8 _stacks;
        uint8 _previousStacks;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_tjs_intensity_AuraScript();
    }
};

void AddSC_boss_lorewalker_stonestep()
{
    new boss_lorewalker_stonestep();
    new npc_sun();
    new npc_zao();
    new npc_haunting_sha();
    new npc_strife_peril();
    new npc_corrupted_scroll();
    new npc_nodding_tiger();
    new npc_golden_beetle();
    new npc_jiang_xiang();
    new npc_songbird_queen();
    new npc_talking_fish();
    new spell_tjs_intensity();
}
