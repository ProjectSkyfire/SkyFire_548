/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "zulgurub.h"

enum Says
{
    SAY_AGGRO               = 0,
    SAY_WHISPER_OF_HETHISS  = 1,
    SAY_TRANSFORM           = 2,
    SAY_BLOODVENOM          = 3,
    EMOTE_WHISPER           = 4,
    EMOTE_EXHAUSTED         = 5,
    SAY_PLAYER_KILL         = 6,
    SAY_DEATH               = 7
};

enum Spells
{
    SPELL_TOXIC_LINK                = 96475,
    SPELL_TOXIC_LINK_AURA           = 96477, // On players
    SPELL_TOXIC_EXPLOSION           = 96489,
    SPELL_WORD_OF_HETHISS           = 96560,
    SPELL_BREATH_OF_HETHISS         = 96509,
    SPELL_POOL_OF_ACID_TEARS        = 96515,
    SPELL_POOL_OF_ACID_TEARS_AURA   = 96520, // makes the pool grow
    SPELL_BLESSING_OF_THE_SNAKE_GOD = 96512,
    SPELL_BLOODVENOM                = 96842,
    SPELL_BLOODVENOM_DUMMY          = 97110,
    SPELL_BLOODVENOM_AURA           = 97099,
    SPELL_VENOM_WITHDRAWAL          = 96653,
    SPELL_VENOMOUS_EFFUSION         = 96678,
    SPELL_VENOM_TOTEM_BEAM_CENTER   = 97098
};

enum Events
{
    EVENT_TOXIC_LINK            = 1,
    EVENT_BREATH_OF_HETHISS     = 3,
    EVENT_POOL_OF_ACID_TEARS    = 4,
    EVENT_MOVE_DOWN             = 5,
    EVENT_VENOM_WITHDRAWAL      = 6,
    EVENT_TRANSFORM             = 7,
    EVENT_WHISPER_OF_HETHISS    = 8,
    EVENT_MOVE_UP               = 9,
    EVENT_BLOODVENOM            = 10
};

enum Creatures
{
    NPC_VENOMOUS_EFFUSION_STALKER   = 52302,
    NPC_POOL_OF_ACID_TEARS          = 52197,
    NPC_BLOODVENOM                  = 52525
};

enum Points
{
    POINT_DOWN          = 1,
    POINT_UP            = 2,
    POINT_WITHDRAWAL    = 3
};

uint32 const VENOXIS_TIMED_EVENT = 28223;

Position const downPos = {-12000.54f, -1685.88f, 32.28f, 0.68f};
Position const upPos = {-12020.12f, -1699.96f, 39.53f, 0.60f};

Position const mazeleftPos[18] =
{
    {-12003.12f, -1700.04f, 32.29f, 0.0f},
    {-11999.17f, -1705.85f, 32.29f, 0.0f},
    {-12000.37f, -1714.15f, 32.34f, 0.0f},
    {-11992.49f, -1719.70f, 32.39f, 0.0f},
    {-11985.03f, -1714.68f, 32.50f, 0.0f},
    {-11981.08f, -1718.16f, 32.95f, 0.0f},
    {-11975.54f, -1714.41f, 32.90f, 0.0f},
    {-11978.24f, -1705.43f, 32.40f, 0.0f},
    {-11972.51f, -1702.20f, 32.60f, 0.0f},
    {-11974.63f, -1694.00f, 32.40f, 0.0f},
    {-11970.75f, -1691.41f, 32.70f, 0.0f},
    {-11976.65f, -1683.74f, 32.80f, 0.0f},
    {-11988.19f, -1692.20f, 32.30f, 0.0f},
    {-11983.91f, -1696.76f, 32.30f, 0.0f},
    {-11989.18f, -1701.09f, 32.30f, 0.0f},
    {-11995.53f, -1697.08f, 32.30f, 0.0f},
    {-11992.38f, -1694.31f, 32.30f, 0.0f},
    {-12001.17f, -1688.04f, 32.30f, 0.0f}
};


class boss_venoxis : public CreatureScript
{
    public:
        boss_venoxis() : CreatureScript("boss_venoxis") { }

        struct boss_venoxisAI : public BossAI
        {
            boss_venoxisAI(Creature* creature) : BossAI(creature, DATA_VENOXIS), phase(0) { }

            uint8 phase;

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
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                phase = 0;
                me->SetReactState(REACT_AGGRESSIVE);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                phase = 0;
                me->SetReactState(REACT_AGGRESSIVE);
                events.ScheduleEvent(EVENT_TOXIC_LINK, 7000);
                events.ScheduleEvent(EVENT_WHISPER_OF_HETHISS, 6000);
                events.ScheduleEvent(EVENT_TRANSFORM, 35000);
                DoCast(me, SPELL_WORD_OF_HETHISS);
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, VENOXIS_TIMED_EVENT);
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

            void MovementInform(uint32 /*type*/, uint32 id) OVERRIDE
            {
                if (id == POINT_DOWN && phase == 2)
                    events.ScheduleEvent(EVENT_MOVE_UP, 400);
                else if (id == POINT_UP && phase == 2)
                    events.ScheduleEvent(EVENT_BLOODVENOM, 1000);
                else if (id == POINT_WITHDRAWAL)
                {
                    Talk(EMOTE_EXHAUSTED);
                    me->RemoveAura(SPELL_BLESSING_OF_THE_SNAKE_GOD);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
                    DoCast(me, SPELL_VENOM_WITHDRAWAL);
                    me->SetReactState(REACT_AGGRESSIVE);
                    events.ScheduleEvent(EVENT_TOXIC_LINK, 17000);
                    events.ScheduleEvent(EVENT_WHISPER_OF_HETHISS, 16000);
                    events.ScheduleEvent(EVENT_TRANSFORM, 45000);
                }
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
                        case EVENT_TOXIC_LINK:
                            DoCast(me, SPELL_TOXIC_LINK);
                            events.ScheduleEvent(EVENT_TOXIC_LINK, urand(15000, 19000));
                            break;
                        case EVENT_WHISPER_OF_HETHISS:
                            Talk(SAY_WHISPER_OF_HETHISS);
                            events.ScheduleEvent(EVENT_WHISPER_OF_HETHISS, 10000);
                            break;
                        case EVENT_TRANSFORM:
                            Talk(SAY_TRANSFORM);
                            phase = 1;
                            events.CancelEvent(EVENT_TOXIC_LINK);
                            events.CancelEvent(EVENT_WHISPER_OF_HETHISS);
                            DoCast(me, SPELL_BLESSING_OF_THE_SNAKE_GOD);
                            events.ScheduleEvent(EVENT_BREATH_OF_HETHISS, urand(6000, 9000));
                            events.ScheduleEvent(EVENT_POOL_OF_ACID_TEARS, urand(8000, 10000));
                            events.ScheduleEvent(EVENT_MOVE_DOWN, 40000);
                            break;
                        case EVENT_BREATH_OF_HETHISS:
                            DoCast(me, SPELL_BREATH_OF_HETHISS);
                            events.ScheduleEvent(EVENT_BREATH_OF_HETHISS, 10000);
                            break;
                        case EVENT_POOL_OF_ACID_TEARS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_POOL_OF_ACID_TEARS);
                            events.ScheduleEvent(EVENT_POOL_OF_ACID_TEARS, 7000);
                            break;
                        case EVENT_MOVE_DOWN:
                            events.CancelEvent(EVENT_BREATH_OF_HETHISS);
                            events.CancelEvent(EVENT_POOL_OF_ACID_TEARS);
                            phase = 2;
                            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);
                            me->GetMotionMaster()->MovePoint(POINT_DOWN, downPos);
                            break;
                        case EVENT_MOVE_UP:
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(POINT_UP, upPos);
                            break;
                        case EVENT_BLOODVENOM:
                            Talk(SAY_BLOODVENOM);
                            DoCast(me, SPELL_BLOODVENOM);
                            events.ScheduleEvent(EVENT_VENOM_WITHDRAWAL, 15500);
                            break;
                        case EVENT_VENOM_WITHDRAWAL:
                            phase = 0;
                            me->GetMotionMaster()->MovePoint(POINT_WITHDRAWAL, downPos);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<boss_venoxisAI>(creature);
        }
};

class npc_venoxis_bloodvenom : public CreatureScript
{
    public:
        npc_venoxis_bloodvenom() : CreatureScript("npc_venoxis_bloodvenom") { }

        struct npc_venoxis_bloodvenomAI : public ScriptedAI
        {
            npc_venoxis_bloodvenomAI(Creature* creature) : ScriptedAI(creature), moving(false), moveTimer(1000)
            {
                me->SetSpeed(MOVE_RUN, 0.3f);
                me->SetReactState(REACT_PASSIVE);
                instance = creature->GetInstanceScript();
                DoCast(me, SPELL_BLOODVENOM_DUMMY, true);
                SetCombatMovement(false);
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                if (Creature* venoxis = me->FindNearestCreature(NPC_VENOXIS, 120.0f))
                    DoCast(venoxis, SPELL_VENOM_TOTEM_BEAM_CENTER);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!instance || instance->GetBossState(DATA_VENOXIS) != IN_PROGRESS)
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                if (moving)
                    return;

                if (moveTimer <= diff)
                {
                    if (Unit* owner = me->GetOwner())
                    {
                        moving = true;
                        DoCast(me, SPELL_BLOODVENOM_AURA, true);
                        me->GetMotionMaster()->MoveFollow(owner, 0.0f, 0.0f);
                    }
                }
                else
                    moveTimer -= diff;
            }

        private:
            InstanceScript* instance;
            bool moving;
            uint32 moveTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_venoxis_bloodvenomAI>(creature);
        }
};

class npc_venoxis_venomous_effusion_stalker : public CreatureScript
{
    public:
        npc_venoxis_venomous_effusion_stalker() : CreatureScript("npc_venoxis_venomous_effusion_stalker") { }

        struct npc_venoxis_venomous_effusion_stalkerAI : public ScriptedAI
        {
            npc_venoxis_venomous_effusion_stalkerAI(Creature* creature) : ScriptedAI(creature), waypoint(1), moveTimer(1)
            {
                me->SetSpeed(MOVE_RUN, 0.3f);
                me->SetReactState(REACT_PASSIVE);
                instance = creature->GetInstanceScript();
                DoCast(me, SPELL_VENOMOUS_EFFUSION, true);
                SetCombatMovement(false);
            }

            void MovementInform(uint32 /*type*/, uint32 /*pointId*/) OVERRIDE
            {
                if (waypoint == 18)
                    me->DespawnOrUnsummon();
                else
                    moveTimer = 1;
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (me->IsInCombat())
                    DoZoneInCombat(summon);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (instance->GetBossState(DATA_VENOXIS) != IN_PROGRESS)
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                if (moveTimer)
                {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(0, mazeleftPos[waypoint]);
                    ++waypoint;
                    moveTimer = 0;
                }
            }

        private:
            InstanceScript* instance;
            uint8 waypoint;
            uint32 moveTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_venoxis_venomous_effusion_stalkerAI>(creature);
        }
};

class npc_venoxis_venomous_effusion : public CreatureScript
{
    public:
        npc_venoxis_venomous_effusion() : CreatureScript("npc_venoxis_venomous_effusion") { }

        struct npc_venoxis_venomous_effusionAI : public ScriptedAI
        {
            npc_venoxis_venomous_effusionAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                instance = creature->GetInstanceScript();
                SetCombatMovement(false);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (instance->GetBossState(DATA_VENOXIS) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_venoxis_venomous_effusionAI>(creature);
        }
};

class npc_venoxis_pool_of_acid_tears : public CreatureScript
{
    public:
        npc_venoxis_pool_of_acid_tears() : CreatureScript("npc_venoxis_venoxis_pool_of_acid_tears") { }

        struct npc_venoxis_pool_of_acid_tearsAI : public ScriptedAI
        {
            npc_venoxis_pool_of_acid_tearsAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                DoCast(me, SPELL_POOL_OF_ACID_TEARS_AURA, true);
                SetCombatMovement(false);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_venoxis_pool_of_acid_tearsAI>(creature);
        }
};

// Links two random players; the link holds while they stay within 30 yards of each other and explodes when it ends
class spell_venoxis_toxic_link : public SpellScriptLoader
{
    public:
        spell_venoxis_toxic_link() : SpellScriptLoader("spell_venoxis_toxic_link") { }

        class spell_venoxis_toxic_link_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_venoxis_toxic_link_SpellScript);

            bool Load() OVERRIDE
            {
                canLink = false;
                target1 = NULL;
                target2 = NULL;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.size() < 2)
                {
                    canLink = false;
                    return;
                }

                std::list<WorldObject*>::const_iterator itr = targets.begin();
                target1 = (*itr)->ToUnit();
                ++itr;
                target2 = (*itr)->ToUnit();
                canLink = true;
            }

            void HandleLink()
            {
                Unit* caster = GetCaster();
                if (!caster || !canLink)
                    return;

                if (target1 && target1->IsAlive() && target1->IsInWorld() && target2 && target2->IsAlive() && target2->IsInWorld())
                {
                    caster->CastSpell(target1, SPELL_TOXIC_LINK_AURA, true);
                    caster->CastSpell(target2, SPELL_TOXIC_LINK_AURA, true);
                }
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_venoxis_toxic_link_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                AfterCast += SpellCastFn(spell_venoxis_toxic_link_SpellScript::HandleLink);
            }

        private:
            bool canLink;
            Unit* target1;
            Unit* target2;
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_venoxis_toxic_link_SpellScript();
        }
};

class spell_venoxis_toxic_link_aura : public SpellScriptLoader
{
    public:
        spell_venoxis_toxic_link_aura() : SpellScriptLoader("spell_venoxis_toxic_link_aura") { }

        class spell_venoxis_toxic_link_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_venoxis_toxic_link_aura_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_TOXIC_EXPLOSION, true);
            }

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                Unit* target = GetTarget();
                if (!target || !GetCaster())
                    return;

                // The other linked player, found among the players of the map
                Player* partner = NULL;
                Map::PlayerList const& players = target->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    Player* player = itr->GetSource();
                    if (player && player != target && player->HasAura(SPELL_TOXIC_LINK_AURA))
                    {
                        partner = player;
                        break;
                    }
                }

                if (!partner)
                {
                    target->RemoveAura(SPELL_TOXIC_LINK_AURA);
                    return;
                }

                if (!target->IsWithinDistInMap(partner, 30.0f))
                {
                    target->RemoveAurasDueToSpell(SPELL_TOXIC_LINK_AURA);
                    partner->RemoveAurasDueToSpell(SPELL_TOXIC_LINK_AURA);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_venoxis_toxic_link_aura_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_venoxis_toxic_link_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_venoxis_toxic_link_aura_AuraScript();
        }
};

class ExactDistanceCheck
{
    public:
        ExactDistanceCheck(Unit* source, float dist) : _source(source), _dist(dist) { }

        bool operator()(WorldObject* unit) const
        {
            return _source->GetExactDist2d(unit) > _dist;
        }

    private:
        Unit* _source;
        float _dist;
};

// The pool grows with its scale, so only the targets inside the visible pool are hit
class spell_venoxis_pool_of_acid_tears_dmg : public SpellScriptLoader
{
    public:
        spell_venoxis_pool_of_acid_tears_dmg() : SpellScriptLoader("spell_venoxis_pool_of_acid_tears_dmg") { }

        class spell_venoxis_pool_of_acid_tears_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_venoxis_pool_of_acid_tears_dmg_SpellScript);

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                Unit* caster = GetCaster();
                targets.remove_if(ExactDistanceCheck(caster, 2.0f * caster->GetFloatValue(OBJECT_FIELD_SCALE)));
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_venoxis_pool_of_acid_tears_dmg_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_venoxis_pool_of_acid_tears_dmg_SpellScript();
        }
};

void AddSC_boss_venoxis()
{
    new boss_venoxis();
    new npc_venoxis_venomous_effusion_stalker();
    new npc_venoxis_venomous_effusion();
    new npc_venoxis_bloodvenom();
    new npc_venoxis_pool_of_acid_tears();
    new spell_venoxis_toxic_link();
    new spell_venoxis_toxic_link_aura();
    new spell_venoxis_pool_of_acid_tears_dmg();
}
