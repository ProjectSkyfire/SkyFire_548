/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "Player.h"
#include "zulaman.h"

enum Says
{
    SAY_AGGRO               = 1,
    SAY_PLAYER_KILL         = 2,
    SAY_FIRE_BREATH         = 3,
    SAY_BEAR                = 4,
    SAY_EAGLE               = 5,
    SAY_LYNX                = 6,
    SAY_DRAGONHAWK          = 7,
    EMOTE_BEAR              = 8,
    EMOTE_EAGLE             = 9,
    EMOTE_LYNX              = 10,
    EMOTE_DRAGONHAWK        = 11,
    SAY_DEATH               = 12
};

enum Spells
{
    SPELL_WHIRLWIND                 = 17207,
    SPELL_GRIEVOUS_THROW            = 43093,

    // Bear
    SPELL_CREEPING_PARALYSIS        = 43095,
    SPELL_OVERPOWER                 = 43456,
    SPELL_SURGE                     = 42402,

    // Lynx
    SPELL_SUMMON_AMANI_LYNX         = 97686,
    SPELL_LYNX_RUSH                 = 43153,
    SPELL_CLAW_RAGE_CHARGE          = 42583,
    SPELL_CLAW_RAGE                 = 43149,

    // Dragonhawk
    SPELL_FLAME_BREATH              = 43140,
    SPELL_FLAME_WHIRL               = 43213,
    SPELL_PILLAR_OF_FLAME_SUMMON    = 43216,
    SPELL_PILLAR_OF_FLAME_VISUAL    = 43541,
    SPELL_PILLAR_OF_FLAME_AURA      = 43218,

    // Eagle
    SPELL_SWEEPING_WINDS            = 97647,
    SPELL_FEATHER_VORTEX_VISUAL     = 97835,
    SPELL_FEATHER_VORTEX_PASSIVE    = 43120,
    SPELL_LIGHTNING_TOTEM           = 97930,

    SPELL_SHAPE_OF_THE_BEAR         = 42594,
    SPELL_SHAPE_OF_THE_EAGLE        = 42606,
    SPELL_SHAPE_OF_THE_LYNX         = 42607,
    SPELL_SHAPE_OF_THE_DRAGONHAWK   = 42608,

    SPELL_BERSERK                   = 45078,

    // Amani Lynx
    SPELL_FERAL_SWIPE               = 43357
};

enum Events
{
    EVENT_WHIRLWIND             = 1,
    EVENT_GRIEVOUS_THROW        = 2,
    EVENT_CREEPING_PARALYSIS    = 3,
    EVENT_OVERPOWER             = 4,
    EVENT_SURGE                 = 5,
    EVENT_LYNX_RUSH             = 6,
    EVENT_CLAW_RAGE             = 7,
    EVENT_SWEEPING_WINDS        = 8,
    EVENT_FLAME_BREATH          = 9,
    EVENT_FLAME_WHIRL           = 10,
    EVENT_PILLAR_OF_FLAME       = 11,
    EVENT_BERSERK               = 12,
    EVENT_FERAL_SWIPE           = 14,
    EVENT_LIGHTNING_TOTEM       = 15,
    EVENT_CHECK_PLAYERS         = 16
};

enum Creatures
{
    NPC_FEATHER_VORTEX      = 24136
};

enum Actions
{
    ACTION_MOVE_TO_PLAYER   = 1
};

enum Forms
{
    DAAKARA_FORM_BEAR               = 0,
    DAAKARA_FORM_LYNX               = 1,
    DAAKARA_FORM_DRAGONHAWK         = 2,
    DAAKARA_FORM_EAGLE              = 3
};

// The platform of the fight; a player outside of it costs the Ring Out achievement
float const MIN_X = 104.217331f;
float const MAX_X = 136.199707f;
float const MIN_Y = 697.285217f;
float const MAX_Y = 715.623901f;

uint32 const DAAKARA_WEAPON = 33975;

class boss_daakara : public CreatureScript
{
    public:
        boss_daakara() : CreatureScript("boss_daakara") { }

        struct boss_daakaraAI : public BossAI
        {
            boss_daakaraAI(Creature* creature) : BossAI(creature, DATA_DAAKARA), phase(0), achieve(true) { }

            void InitializeAI() OVERRIDE
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
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                phase = 0;
                achieve = true;
                me->SetReactState(REACT_AGGRESSIVE);
                SetEquipmentSlots(false, DAAKARA_WEAPON, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                phase = 0;
                achieve = true;
                me->SetReactState(REACT_AGGRESSIVE);
                events.ScheduleEvent(EVENT_BERSERK, 660000);
                events.ScheduleEvent(EVENT_GRIEVOUS_THROW, urand(3000, 8000));
                events.ScheduleEvent(EVENT_WHIRLWIND, urand(6000, 8000));
                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 5000);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_PLAYER_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
            {
                // A taunt breaks Claw Rage
                if (spell->HasEffect(SPELL_EFFECT_ATTACK_ME) || spell->HasAura(SPELL_AURA_MOD_TAUNT))
                    me->RemoveAurasDueToSpell(SPELL_CLAW_RAGE);
            }

            bool AllowAchieve() const
            {
                return achieve;
            }

            void EnterPhase(uint8 form)
            {
                DoResetThreat();
                SetEquipmentSlots(false, 0, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                switch (form)
                {
                    case DAAKARA_FORM_BEAR:
                        Talk(SAY_BEAR);
                        Talk(EMOTE_BEAR);
                        me->RemoveAurasDueToSpell(SPELL_WHIRLWIND);
                        DoCast(me, SPELL_SHAPE_OF_THE_BEAR, true);
                        events.ScheduleEvent(EVENT_SURGE, urand(4000, 10000));
                        events.ScheduleEvent(EVENT_OVERPOWER, urand(2000, 9000));
                        events.ScheduleEvent(EVENT_CREEPING_PARALYSIS, urand(10000, 15000));
                        break;
                    case DAAKARA_FORM_LYNX:
                        Talk(SAY_LYNX);
                        Talk(EMOTE_LYNX);
                        me->RemoveAurasDueToSpell(SPELL_WHIRLWIND);
                        DoCast(me, SPELL_SHAPE_OF_THE_LYNX, true);
                        DoCast(me, SPELL_SUMMON_AMANI_LYNX, true);
                        events.ScheduleEvent(EVENT_LYNX_RUSH, urand(5000, 12000));
                        events.ScheduleEvent(EVENT_CLAW_RAGE, urand(8000, 15000));
                        break;
                    case DAAKARA_FORM_DRAGONHAWK:
                        Talk(SAY_DRAGONHAWK);
                        Talk(EMOTE_DRAGONHAWK);
                        me->AttackStop();
                        me->SetReactState(REACT_PASSIVE);
                        me->RemoveAurasDueToSpell(SPELL_SHAPE_OF_THE_LYNX);
                        me->RemoveAurasDueToSpell(SPELL_SHAPE_OF_THE_BEAR);
                        DoCast(me, SPELL_SHAPE_OF_THE_DRAGONHAWK, true);
                        events.ScheduleEvent(EVENT_FLAME_BREATH, urand(5000, 12000));
                        events.ScheduleEvent(EVENT_FLAME_WHIRL, urand(3000, 5000));
                        events.ScheduleEvent(EVENT_PILLAR_OF_FLAME, urand(12000, 17000));
                        break;
                    case DAAKARA_FORM_EAGLE:
                    {
                        Talk(SAY_EAGLE);
                        Talk(EMOTE_EAGLE);
                        me->AttackStop();
                        me->SetReactState(REACT_PASSIVE);
                        me->RemoveAurasDueToSpell(SPELL_SHAPE_OF_THE_LYNX);
                        me->RemoveAurasDueToSpell(SPELL_SHAPE_OF_THE_BEAR);
                        DoCast(me, SPELL_SHAPE_OF_THE_EAGLE, true);
                        std::list<Unit*> targets;
                        SelectTargetList(targets, 5, SELECT_TARGET_RANDOM, 0.0f, true);
                        for (std::list<Unit*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                            if (Creature* vortex = me->SummonCreature(NPC_FEATHER_VORTEX, (*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ(), (*itr)->GetOrientation()))
                                vortex->AI()->SetGUID((*itr)->GetGUID());
                        events.ScheduleEvent(EVENT_SWEEPING_WINDS, 5000);
                        events.ScheduleEvent(EVENT_LIGHTNING_TOTEM, urand(10000, 13000));
                        break;
                    }
                    default:
                        break;
                }
            }

            void CheckPlayers()
            {
                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    Player* player = itr->GetSource();
                    if (!player)
                        continue;

                    if (player->GetPositionX() < MIN_X || player->GetPositionX() > MAX_X ||
                        player->GetPositionY() < MIN_Y || player->GetPositionY() > MAX_Y)
                    {
                        achieve = false;
                        return;
                    }
                }

                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 5000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                // The bear or the lynx at 80 percent, the dragonhawk at 40
                if (me->HealthBelowPct(80) && phase == 0)
                {
                    ++phase;
                    events.CancelEvent(EVENT_WHIRLWIND);
                    events.CancelEvent(EVENT_GRIEVOUS_THROW);
                    EnterPhase(urand(DAAKARA_FORM_BEAR, DAAKARA_FORM_LYNX));
                    return;
                }
                else if (me->HealthBelowPct(40) && phase == 1)
                {
                    ++phase;
                    events.CancelEvent(EVENT_SURGE);
                    events.CancelEvent(EVENT_CREEPING_PARALYSIS);
                    events.CancelEvent(EVENT_OVERPOWER);
                    events.CancelEvent(EVENT_LYNX_RUSH);
                    events.CancelEvent(EVENT_CLAW_RAGE);
                    EnterPhase(DAAKARA_FORM_DRAGONHAWK);
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK);
                            break;
                        case EVENT_CHECK_PLAYERS:
                            CheckPlayers();
                            break;
                        case EVENT_WHIRLWIND:
                            DoCast(me, SPELL_WHIRLWIND);
                            events.ScheduleEvent(EVENT_WHIRLWIND, urand(12000, 15000));
                            break;
                        case EVENT_GRIEVOUS_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_GRIEVOUS_THROW);
                            events.ScheduleEvent(EVENT_GRIEVOUS_THROW, urand(9000, 11000));
                            break;
                        case EVENT_CREEPING_PARALYSIS:
                            DoCast(me, SPELL_CREEPING_PARALYSIS);
                            events.ScheduleEvent(EVENT_CREEPING_PARALYSIS, urand(15000, 18000));
                            break;
                        case EVENT_SURGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 0.0f, true))
                                DoCast(target, SPELL_SURGE);
                            events.ScheduleEvent(EVENT_SURGE, urand(10000, 15000));
                            break;
                        case EVENT_OVERPOWER:
                            DoCastVictim(SPELL_OVERPOWER);
                            events.ScheduleEvent(EVENT_OVERPOWER, 10000);
                            break;
                        case EVENT_LYNX_RUSH:
                            DoCastVictim(SPELL_LYNX_RUSH);
                            events.ScheduleEvent(EVENT_LYNX_RUSH, 10000);
                            break;
                        case EVENT_CLAW_RAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_CLAW_RAGE_CHARGE);
                            events.ScheduleEvent(EVENT_CLAW_RAGE, urand(20000, 25000));
                            break;
                        case EVENT_FLAME_WHIRL:
                            DoCast(me, SPELL_FLAME_WHIRL);
                            events.ScheduleEvent(EVENT_FLAME_WHIRL, 12000);
                            break;
                        case EVENT_FLAME_BREATH:
                            Talk(SAY_FIRE_BREATH);
                            DoCast(me, SPELL_FLAME_BREATH);
                            events.ScheduleEvent(EVENT_FLAME_BREATH, 15000);
                            break;
                        case EVENT_PILLAR_OF_FLAME:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_PILLAR_OF_FLAME_SUMMON);
                            events.ScheduleEvent(EVENT_PILLAR_OF_FLAME, urand(12000, 18000));
                            break;
                        case EVENT_SWEEPING_WINDS:
                            DoCast(me, SPELL_SWEEPING_WINDS);
                            events.ScheduleEvent(EVENT_SWEEPING_WINDS, 8500);
                            break;
                        case EVENT_LIGHTNING_TOTEM:
                            DoCast(me, SPELL_LIGHTNING_TOTEM);
                            events.ScheduleEvent(EVENT_LIGHTNING_TOTEM, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint8 phase;
            bool achieve;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<boss_daakaraAI>(creature);
        }
};

class npc_daakara_vortex : public CreatureScript
{
    public:
        npc_daakara_vortex() : CreatureScript("npc_daakara_vortex") { }

        struct npc_daakara_vortexAI : public ScriptedAI
        {
            npc_daakara_vortexAI(Creature* creature) : ScriptedAI(creature), started(false), startTimer(2000), victimGUID(0)
            {
                DoCast(me, SPELL_FEATHER_VORTEX_VISUAL, true);
                me->SetReactState(REACT_PASSIVE);
                SetCombatMovement(false);
            }

            void SetGUID(uint64 guid, int32 /*type*/) OVERRIDE
            {
                victimGUID = guid;
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_MOVE_TO_PLAYER)
                    if (Unit* target = ObjectAccessor::GetUnit(*me, victimGUID))
                        me->GetMotionMaster()->MoveChase(target);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (started)
                    return;

                if (startTimer <= diff)
                {
                    started = true;
                    DoCast(me, SPELL_FEATHER_VORTEX_PASSIVE, true);
                }
                else
                    startTimer -= diff;
            }

        private:
            bool started;
            uint32 startTimer;
            uint64 victimGUID;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<npc_daakara_vortexAI>(creature);
        }
};

class npc_daakara_pillar_of_flame : public CreatureScript
{
    public:
        npc_daakara_pillar_of_flame() : CreatureScript("npc_daakara_pillar_of_flame") { }

        struct npc_daakara_pillar_of_flameAI : public ScriptedAI
        {
            npc_daakara_pillar_of_flameAI(Creature* creature) : ScriptedAI(creature), explode(false), explodeTimer(2000)
            {
                me->SetReactState(REACT_PASSIVE);
                DoCast(me, SPELL_PILLAR_OF_FLAME_VISUAL, true);
                SetCombatMovement(false);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (explode)
                    return;

                if (explodeTimer <= diff)
                {
                    explode = true;
                    me->RemoveAllAuras();
                    DoCast(me, SPELL_PILLAR_OF_FLAME_AURA, true);
                }
                else
                    explodeTimer -= diff;
            }

        private:
            bool explode;
            uint32 explodeTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<npc_daakara_pillar_of_flameAI>(creature);
        }
};

class npc_daakara_amani_lynx : public CreatureScript
{
    public:
        npc_daakara_amani_lynx() : CreatureScript("npc_daakara_amani_lynx") { }

        struct npc_daakara_amani_lynxAI : public ScriptedAI
        {
            npc_daakara_amani_lynxAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_FERAL_SWIPE, urand(6000, 9000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_FERAL_SWIPE)
                    {
                        DoCastVictim(SPELL_FERAL_SWIPE);
                        events.ScheduleEvent(EVENT_FERAL_SWIPE, urand(7000, 12000));
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulAmanAI<npc_daakara_amani_lynxAI>(creature);
        }
};

class spell_daakara_claw_rage_charge : public SpellScriptLoader
{
    public:
        spell_daakara_claw_rage_charge() : SpellScriptLoader("spell_daakara_claw_rage_charge") { }

        class spell_daakara_claw_rage_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_daakara_claw_rage_charge_SpellScript);

            void HandleCharge(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                if (caster->IsAIEnabled)
                    caster->GetAI()->AttackStart(target);
                caster->CastSpell(target, SPELL_CLAW_RAGE, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_daakara_claw_rage_charge_SpellScript::HandleCharge, EFFECT_0, SPELL_EFFECT_CHARGE);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_daakara_claw_rage_charge_SpellScript();
        }
};

class spell_daakara_sweeping_winds : public SpellScriptLoader
{
    public:
        spell_daakara_sweeping_winds() : SpellScriptLoader("spell_daakara_sweeping_winds") { }

        class spell_daakara_sweeping_winds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_daakara_sweeping_winds_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                if (target && target->GetEntry() == NPC_FEATHER_VORTEX && target->IsAIEnabled)
                    target->GetAI()->DoAction(ACTION_MOVE_TO_PLAYER);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_daakara_sweeping_winds_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_daakara_sweeping_winds_SpellScript();
        }
};

class achievement_ring_out : public AchievementCriteriaScript
{
    public:
        achievement_ring_out() : AchievementCriteriaScript("achievement_ring_out") { }

        bool OnCheck(Player* /*source*/, Unit* target) OVERRIDE
        {
            if (!target)
                return false;

            if (boss_daakara::boss_daakaraAI* ai = CAST_AI(boss_daakara::boss_daakaraAI, target->GetAI()))
                return ai->AllowAchieve();

            return false;
        }
};

void AddSC_boss_daakara()
{
    new boss_daakara();
    new npc_daakara_vortex();
    new npc_daakara_pillar_of_flame();
    new npc_daakara_amani_lynx();
    new spell_daakara_claw_rage_charge();
    new spell_daakara_sweeping_winds();
    new achievement_ring_out();
}
