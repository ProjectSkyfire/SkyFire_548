/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "Player.h"
#include "temple_of_jade_serpent.h"

enum LiuSpells
{
    SPELL_DUST_VISUAL              = 110518,
    SPELL_SERPENT_STRIKE           = 106823,
    SPELL_SERPENT_KICK             = 106856,
    SPELL_JADE_SERPENT_STRIKE      = 106841,
    SPELL_JADE_SERPENT_KICK        = 106864,
    SPELL_SUMMON_JADE_SERPENT      = 106895,
    SPELL_JADE_SOUL                = 106909,
    SPELL_JADE_SERPENT_WAVE_VISUAL = 107002,
    SPELL_SERPENT_WAVE_VISUAL      = 106939,
    SPELL_SERPENT_WAVE_PERIODIC    = 106959,
    SPELL_JADE_FIRE_SUMMON         = 107103,
    SPELL_JADE_FIRE_MISSILE        = 107098,

    SPELL_CLEANSING_BREATH         = 132387,
    SPELL_JADE_FIRE                = 107045,
    SPELL_QUIET_SUICIDE            = 115372,

    SPELL_SHADOW_OF_DOUBT          = 110099,
    SPELL_SHATTERED_RESOLVE        = 110125,
    SPELL_SHA_SCREECH              = 122527,
};

enum LiuEvents
{
    EVENT_SERPENT_STRIKE = 1,
    EVENT_SERPENT_KICK,
    EVENT_SERPENT_WAVE,
    EVENT_JADE_SERPENT_STRIKE,
    EVENT_JADE_SERPENT_KICK,
    EVENT_JADE_SERPENT_WAVE,
    EVENT_SUMMON_YULON,
    EVENT_AURA_JADE,

    EVENT_YULON_JADE_FIRE,
    EVENT_YULON_BREATH,
    EVENT_YULON_LEAVE,

    EVENT_WAVE_LAUNCH,
    EVENT_WAVE_JADE_FIRE,

    EVENT_SHADOW_OF_DOUBT,
    EVENT_SHATTERED_RESOLVE,
    EVENT_SHA_SCREECH,
};

enum LiuTexts
{
    SAY_LIU_AGGRO        = 0,
    SAY_LIU_DEATH        = 1,
    SAY_LIU_JADE_PHASE   = 2,
    SAY_LIU_YULON_PHASE  = 3,
    SAY_LIU_KILL         = 4,
    SAY_LIU_INTRO        = 5,
    SAY_CLEANSED         = 9,

    SAY_YULON_LEAVE      = 0,

    SAY_TRASH_MOVE_AWAY  = 0,
};

enum LiuMisc
{
    ACTION_SERPENT_WAVE  = 0,
    ACTION_JADE_WAVE     = 1,

    NPC_TRIGGER_WAVE     = 56789,
    FACTION_FRIENDLY     = 35,
    AREA_BRIDGE_1        = 6118,
    AREA_BRIDGE_2        = 5956,
};

enum LiuPhases
{
    PHASE_SERPENT,
    PHASE_JADE_SERPENT,
    PHASE_YULON,
};

float const WaveAngle = float(M_PI) / 4.0f;

float const YuLonLeavePath[3][3] =
{
    { 938.1285f, -2542.773f, 226.7539f },
    { 965.3004f, -2489.177f, 256.0829f },
    { 978.6389f, -2419.773f, 299.6773f },
};

float const TrashLeavePath[2][3] =
{
    { 937.553f, -2536.750f, 179.821f },
    { 955.057f, -2483.663f, 180.499f },
};

static void LaunchPath(Creature* creature, float const (*points)[3], uint8 count, float velocity, bool fly)
{
    Movement::MoveSplineInit init(creature);
    for (uint8 i = 0; i < count; ++i)
        init.Path().push_back(G3D::Vector3(points[i][0], points[i][1], points[i][2]));
    init.SetSmooth();
    if (fly)
        init.SetFly();
    else
        init.SetUncompressed();
    init.SetVelocity(velocity);
    init.Launch();
    creature->DespawnOrUnsummon(creature->GetSplineDuration());
}

// Trash around the Liu courtyard is only damaged by players and pets; other creatures just hold aggro.
static void PreventNpcKills(Creature* me, Unit* attacker, uint32& damage)
{
    if (attacker->GetTypeId() != TypeID::TYPEID_UNIT || attacker->IsPet())
        return;

    if (me->GetHealth() < me->GetMaxHealth() || me->GetHealth() <= damage)
    {
        damage = 0;
        me->AddThreat(attacker, 0.0f);
    }
}

static bool ShouldTauntPlayer(Creature* me, Unit* who, bool targeted)
{
    bool fightingNpc = me->IsInCombat() && me->GetVictim() && me->GetVictim()->GetTypeId() != TypeID::TYPEID_PLAYER;
    if (!((who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(who, 15.0f) && fightingNpc) || !me->IsInCombat()))
        return false;

    // Players crossing the bridges above must not pull the courtyard.
    return targeted || (me->GetAreaId() != AREA_BRIDGE_1 && me->GetAreaId() != AREA_BRIDGE_2);
}

class boss_liu_flameheart : public CreatureScript
{
public:
    boss_liu_flameheart() : CreatureScript("boss_liu_flameheart") { }

    struct boss_liu_flameheartAI : public BossAI
    {
        boss_liu_flameheartAI(Creature* creature) : BossAI(creature, DATA_LIU)
        {
            _phase = PHASE_SERPENT;
        }

        void InitializeAI() OVERRIDE
        {
            me->setActive(true);
            Talk(SAY_LIU_INTRO);
            DoCast(me, SPELL_POSSESSED_BY_SHA);
            DoCast(me, SPELL_DUST_VISUAL);
            BossAI::InitializeAI();
        }

        void Reset() OVERRIDE
        {
            _Reset();
            _phase = PHASE_SERPENT;
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(SAY_LIU_KILL);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            Talk(SAY_LIU_AGGRO);
            events.ScheduleEvent(EVENT_SERPENT_STRIKE, 5 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SERPENT_KICK, 5 * IN_MILLISECONDS);
            instance->SetData(DATA_TWIN_DRAGONS_DOOR, NOT_STARTED);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
        }

        void EnterEvadeMode() OVERRIDE
        {
            summons.DespawnAll();
            me->RemoveAllAuras();
            me->ClearUnitState(UNIT_STATE_ROOT);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            BossAI::EnterEvadeMode();
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            instance->SetBossState(DATA_LIU, FAIL);
            instance->SetData(DATA_TWIN_DRAGONS_DOOR, DONE);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
            Talk(SAY_LIU_DEATH);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            instance->SetData(DATA_LIU, DONE);
            instance->SetData(DATA_TWIN_DRAGONS_DOOR, DONE);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
        {
            if (_phase == PHASE_SERPENT && me->GetHealthPct() <= 70.0f)
            {
                _phase = PHASE_JADE_SERPENT;
                Talk(SAY_LIU_JADE_PHASE);
                DoCast(me, SPELL_JADE_ESSENCE);
                events.Reset();
                events.ScheduleEvent(EVENT_JADE_SERPENT_STRIKE, 5 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_JADE_SERPENT_KICK, 10 * IN_MILLISECONDS);
            }

            if (_phase == PHASE_JADE_SERPENT && me->GetHealthPct() <= 30.0f)
            {
                _phase = PHASE_YULON;
                Talk(SAY_LIU_YULON_PHASE);
                events.Reset();
                events.ScheduleEvent(EVENT_SUMMON_YULON, 500);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
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
                    case EVENT_SERPENT_STRIKE:
                        DoCastVictim(SPELL_SERPENT_STRIKE);
                        events.ScheduleEvent(EVENT_SERPENT_STRIKE, 10 * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_SERPENT_WAVE, 4 * IN_MILLISECONDS);
                        break;
                    case EVENT_SERPENT_KICK:
                        DoCastVictim(SPELL_SERPENT_KICK);
                        events.ScheduleEvent(EVENT_SERPENT_KICK, 10 * IN_MILLISECONDS);
                        break;
                    case EVENT_SERPENT_WAVE:
                        SummonWaves(0, ACTION_SERPENT_WAVE);
                        break;
                    case EVENT_JADE_SERPENT_STRIKE:
                    {
                        DoCast(SPELL_JADE_SERPENT_STRIKE);
                        std::list<Player*> players;
                        GetPlayerListInGrid(players, me, 10.0f);
                        for (Player* player : players)
                            player->KnockbackFrom(me->GetPositionX(), me->GetPositionY(), 10.0f, 10.0f);
                        events.ScheduleEvent(EVENT_JADE_SERPENT_STRIKE, 10 * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_JADE_SERPENT_WAVE, 4 * IN_MILLISECONDS);
                        break;
                    }
                    case EVENT_JADE_SERPENT_KICK:
                        DoCastVictim(SPELL_JADE_SERPENT_KICK);
                        events.ScheduleEvent(EVENT_JADE_SERPENT_KICK, 10 * IN_MILLISECONDS);
                        break;
                    case EVENT_JADE_SERPENT_WAVE:
                        SummonWaves(4, ACTION_JADE_WAVE);
                        break;
                    case EVENT_SUMMON_YULON:
                        DoCast(me, SPELL_SUMMON_JADE_SERPENT);
                        DoCast(me, SPELL_JADE_SOUL);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        me->SetReactState(REACT_PASSIVE);
                        me->AddUnitState(UNIT_STATE_ROOT);
                        events.ScheduleEvent(EVENT_AURA_JADE, 3 * IN_MILLISECONDS);
                        break;
                    case EVENT_AURA_JADE:
                        DoCast(me, SPELL_JADE_SOUL);
                        events.ScheduleEvent(EVENT_AURA_JADE, 2500);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        void SummonWaves(uint8 first, int32 action)
        {
            for (uint8 i = first; i < first + 4; ++i)
            {
                float x, y;
                me->GetNearPoint2D(x, y, 7.5f, Position::NormalizeOrientation(WaveAngle + i * float(M_PI) / 2.0f));
                if (Creature* trigger = me->SummonCreature(NPC_TRIGGER_WAVE, x, y, me->GetPositionZ(), me->GetAngle(x, y)))
                    if (trigger->IsAIEnabled)
                        trigger->AI()->DoAction(action);
            }
        }

        LiuPhases _phase;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetJadeSerpentAI<boss_liu_flameheartAI>(creature);
    }
};

class boss_yu_lon_tjs : public CreatureScript
{
public:
    boss_yu_lon_tjs() : CreatureScript("boss_yu_lon_tjs") { }

    struct boss_yu_lon_tjsAI : public ScriptedAI
    {
        boss_yu_lon_tjsAI(Creature* creature) : ScriptedAI(creature)
        {
            _instance = creature->GetInstanceScript();
            _leaving = false;
        }

        void InitializeAI() OVERRIDE
        {
            me->SetHealth(me->CountPctFromMaxHealth(30));
            me->SetReactState(REACT_AGGRESSIVE);
            _events.ScheduleEvent(EVENT_YULON_JADE_FIRE, 100);
            if (_instance)
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (me->GetHealth() > damage)
                return;

            damage = 0;
            if (_leaving)
                return;

            _leaving = true;
            if (_instance)
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            me->SetReactState(REACT_PASSIVE);
            me->CombatStop(true);
            me->getHostileRefManager().deleteReferences();
            me->DeleteThreatList();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            _events.Reset();

            if (Creature* liu = GetLiu())
            {
                float x, y;
                GetPositionWithDistInOrientation(liu, 15.0f, liu->GetOrientation(), x, y);
                Movement::MoveSplineInit init(me);
                init.MoveTo(x, y, liu->GetPositionZ());
                init.SetVelocity(9.0f);
                init.SetFly();
                init.Launch();
                _events.ScheduleEvent(EVENT_YULON_BREATH, me->GetSplineDuration());
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_YULON_JADE_FIRE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_JADE_FIRE);
                        _events.ScheduleEvent(EVENT_YULON_JADE_FIRE, urand(4, 6) * IN_MILLISECONDS);
                        break;
                    case EVENT_YULON_BREATH:
                        if (Creature* liu = GetLiu())
                        {
                            me->SetFacingToObject(liu);
                            liu->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        }
                        DoCast(SPELL_CLEANSING_BREATH);
                        _events.ScheduleEvent(EVENT_YULON_LEAVE, 2 * IN_MILLISECONDS);
                        break;
                    case EVENT_YULON_LEAVE:
                        Talk(SAY_YULON_LEAVE);
                        me->setFaction(FACTION_FRIENDLY);
                        LaunchPath(me, YuLonLeavePath, 3, 15.0f, true);
                        break;
                    default:
                        break;
                }
            }

            if (!_leaving && UpdateVictim())
                DoMeleeAttackIfReady();
        }

    private:
        Creature* GetLiu() const
        {
            return _instance ? ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_LIU)) : NULL;
        }

        InstanceScript* _instance;
        EventMap _events;
        bool _leaving;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_yu_lon_tjsAI(creature);
    }
};

class npc_wave_trigger_liu_flameheart : public CreatureScript
{
public:
    npc_wave_trigger_liu_flameheart() : CreatureScript("npc_wave_trigger_liu_flameheart") { }

    struct npc_wave_trigger_liu_flameheartAI : public ScriptedAI
    {
        npc_wave_trigger_liu_flameheartAI(Creature* creature) : ScriptedAI(creature) { }

        void DoAction(int32 action) OVERRIDE
        {
            uint32 visual = SPELL_SERPENT_WAVE_VISUAL;
            uint32 periodic = SPELL_SERPENT_WAVE_PERIODIC;
            if (action == ACTION_JADE_WAVE)
            {
                visual = SPELL_JADE_SERPENT_WAVE_VISUAL;
                periodic = SPELL_JADE_FIRE_PERIODIC;
                _events.ScheduleEvent(EVENT_WAVE_JADE_FIRE, 2 * IN_MILLISECONDS);
            }

            me->SetReactState(REACT_PASSIVE);
            me->SetDisplayId(11686);
            DoCast(me, visual);
            DoCast(me, periodic);
            _events.ScheduleEvent(EVENT_WAVE_LAUNCH, 1500);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WAVE_LAUNCH:
                    {
                        float x, y;
                        GetPositionWithDistInOrientation(me, 100.0f, me->GetOrientation(), x, y);
                        Movement::MoveSplineInit init(me);
                        init.MoveTo(x, y, me->GetPositionZ());
                        init.SetVelocity(11.5f);
                        init.Launch();
                        me->DespawnOrUnsummon(me->GetSplineDuration());
                        break;
                    }
                    case EVENT_WAVE_JADE_FIRE:
                        DoCast(me, SPELL_JADE_FIRE_SUMMON);
                        _events.ScheduleEvent(EVENT_WAVE_JADE_FIRE, 1 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_wave_trigger_liu_flameheartAI(creature);
    }
};

class npc_minion_of_doubt : public CreatureScript
{
public:
    npc_minion_of_doubt() : CreatureScript("npc_minion_of_doubt") { }

    struct npc_minion_of_doubtAI : public ScriptedAI
    {
        npc_minion_of_doubtAI(Creature* creature) : ScriptedAI(creature)
        {
            _playerEngaged = false;
            _targeted = false;
        }

        void Reset() OVERRIDE
        {
            _events.Reset();
            _playerEngaged = false;
            _targeted = false;
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            PreventNpcKills(me, attacker, damage);

            if (attacker->GetTypeId() == TypeID::TYPEID_PLAYER && !_playerEngaged)
            {
                _playerEngaged = true;
                _events.ScheduleEvent(EVENT_SHADOW_OF_DOUBT, urand(2500, 8000));
                _events.ScheduleEvent(EVENT_SHATTERED_RESOLVE, urand(4500, 12000));
            }
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            if (!ShouldTauntPlayer(me, who, _targeted))
                return;

            _targeted = true;
            DoResetThreat();
            me->AddThreat(who, 100.0f);
            AttackStart(who);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHADOW_OF_DOUBT:
                        DoCastVictim(SPELL_SHADOW_OF_DOUBT);
                        _events.ScheduleEvent(EVENT_SHADOW_OF_DOUBT, urand(4, 8) * IN_MILLISECONDS);
                        break;
                    case EVENT_SHATTERED_RESOLVE:
                        DoCastVictim(SPELL_SHATTERED_RESOLVE);
                        _events.ScheduleEvent(EVENT_SHATTERED_RESOLVE, urand(4500, 12000));
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        bool _playerEngaged;
        bool _targeted;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_minion_of_doubtAI(creature);
    }
};

class npc_lesser_sha : public CreatureScript
{
public:
    npc_lesser_sha() : CreatureScript("npc_lesser_sha") { }

    struct npc_lesser_shaAI : public ScriptedAI
    {
        npc_lesser_shaAI(Creature* creature) : ScriptedAI(creature)
        {
            _targeted = false;
        }

        void Reset() OVERRIDE
        {
            _events.Reset();
            _targeted = false;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _events.ScheduleEvent(EVENT_SHA_SCREECH, 2 * IN_MILLISECONDS);
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            PreventNpcKills(me, attacker, damage);
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            if (!ShouldTauntPlayer(me, who, _targeted))
                return;

            _targeted = true;
            DoResetThreat();
            me->AddThreat(who, 100.0f);
            AttackStart(who);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (_events.ExecuteEvent() == EVENT_SHA_SCREECH)
            {
                DoCastVictim(SPELL_SHA_SCREECH);
                _events.ScheduleEvent(EVENT_SHA_SCREECH, urand(4, 7) * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        bool _targeted;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lesser_shaAI(creature);
    }
};

class npc_lie_flameheart_trash_base_ai : public CreatureScript
{
public:
    npc_lie_flameheart_trash_base_ai() : CreatureScript("npc_lie_flameheart_trash_base_ai") { }

    struct npc_lie_flameheart_trash_base_aiAI : public ScriptedAI
    {
        npc_lie_flameheart_trash_base_aiAI(Creature* creature) : ScriptedAI(creature)
        {
            _leaving = false;
        }

        void InitializeAI() OVERRIDE
        {
            _leaving = false;
            me->SetReactState(REACT_AGGRESSIVE);
            ScriptedAI::InitializeAI();
        }

        bool CanAIAttack(Unit const* who) const OVERRIDE
        {
            return who->GetEntry() != NPC_LIU_FLAMEHEART && who->GetEntry() != NPC_YU_LON;
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            PreventNpcKills(me, attacker, damage);
        }

        void EnterEvadeMode() OVERRIDE
        {
            if (_leaving)
                return;

            _leaving = true;
            Talk(SAY_TRASH_MOVE_AWAY);
            me->SetReactState(REACT_PASSIVE);
            LaunchPath(me, TrashLeavePath, 2, 7.0f, false);
        }

    private:
        bool _leaving;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_lie_flameheart_trash_base_aiAI(creature);
    }
};

// 132387 - Cleansing Breath
class spell_liu_flameheart_cleansing_breath : public SpellScriptLoader
{
public:
    spell_liu_flameheart_cleansing_breath() : SpellScriptLoader("spell_liu_flameheart_cleansing_breath") { }

    class spell_liu_flameheart_cleansing_breath_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_liu_flameheart_cleansing_breath_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            Creature* target = GetHitCreature();
            if (!target)
                return;

            if (target->IsAIEnabled)
                target->AI()->Talk(SAY_CLEANSED);

            if (Player* looter = target->GetLootRecipient())
                target->CastSpell(target, SPELL_QUIET_SUICIDE, false, NULL, NULL, looter->GetGUID());
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_liu_flameheart_cleansing_breath_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_liu_flameheart_cleansing_breath_SpellScript();
    }
};

// 107045 - Jade Fire
class spell_yu_lon_tjs_jade_fire : public SpellScriptLoader
{
public:
    spell_yu_lon_tjs_jade_fire() : SpellScriptLoader("spell_yu_lon_tjs_jade_fire") { }

    class spell_yu_lon_tjs_jade_fire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_yu_lon_tjs_jade_fire_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Unit* target = GetHitUnit())
                    caster->CastSpell(target, SPELL_JADE_FIRE_MISSILE, false);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_yu_lon_tjs_jade_fire_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_yu_lon_tjs_jade_fire_SpellScript();
    }
};

void AddSC_boss_liu_flameheart()
{
    new boss_liu_flameheart();
    new boss_yu_lon_tjs();
    new npc_wave_trigger_liu_flameheart();
    new npc_minion_of_doubt();
    new npc_lesser_sha();
    new npc_lie_flameheart_trash_base_ai();
    new spell_liu_flameheart_cleansing_breath();
    new spell_yu_lon_tjs_jade_fire();
}
