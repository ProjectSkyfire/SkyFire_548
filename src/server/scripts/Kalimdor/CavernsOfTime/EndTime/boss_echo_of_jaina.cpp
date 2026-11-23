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

#include "end_time.h"

enum Yells
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_INTRO   = 2,
    SAY_KILL    = 3,
    SAY_SPELL   = 4,
};

enum Spells
{
    SPELL_BLINK                 = 101812,
    SPELL_FLARECORE_MISSILE     = 101927,
    SPELL_UNSTABLE_FLARE        = 101980,
    SPELL_TIME_EXPIRE_FLARE     = 101587,
    SPELL_CHECK_PLAYER_DIST     = 101588,
    SPELL_FLARE_UP              = 101589,

    SPELL_FROSTBOLT_VOLLEY      = 101810,
    SPELL_PYROBLAST             = 101809,

    SPELL_FROST_BLADES_SUMMON   = 101339,
};

enum Events
{
    EVENT_FLARECORE         = 1,
    EVENT_BLINK             = 2,
    EVENT_FROSTBOLT_VOLLEY  = 3,
    EVENT_PYROBLAST         = 4,
    EVENT_FROST_BLADES      = 5,
    EVENT_CHECK_PLAYER      = 6,
    EVENT_EXPLODE           = 7,
    EVENT_FLARE_UP          = 8,
};

enum Creatures
{
    NPC_FLARECORE       = 54446,
    NPC_FROSTBLADES     = 54494,
    NPC_ARCANE_CIRCLE   = 54639,
    NPC_JAINA           = 54641,
};

enum Others
{
    ACTION_FRAGMENTS = 1,
};

uint32 FragmentsCount = 1;


static const Position jainaPos = {3004.780029f, 515.729004f, 21.55f, 3.12f};

class boss_echo_of_jaina : public CreatureScript
{
    public:
        boss_echo_of_jaina() : CreatureScript("boss_echo_of_jaina") { }

        struct boss_echo_of_jainaAI : public BossAI
        {
            boss_echo_of_jainaAI(Creature* creature) : BossAI(creature, DATA_ECHO_OF_JAINA)
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
                me->SetReactState(REACT_PASSIVE);
                me->setActive(true);
                me->SetVisible(false);
                bIntro = false;
                uiVolleyCount = 0;
                uiFragments = 0;
            }

            uint32 uiVolleyCount;
            uint8 uiFragments;
            bool bIntro;

            void Reset() OVERRIDE
            {
                _Reset();
                uiVolleyCount = 0;
                if (instance->GetData(DATA_JAINA_EVENT) == DONE)
                {
                    uiFragments = MAX_FRAGMENTS_COUNT;
                    me->SetVisible(true);
                    me->SetReactState(REACT_AGGRESSIVE);
                }
                else
                    me->SummonCreature(NPC_ARCANE_CIRCLE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_FRAGMENTS)
                {
                    if (uiFragments >= MAX_FRAGMENTS_COUNT)
                        return;

                    if (!bIntro)
                    {
                        bIntro = true;
                        Talk(SAY_INTRO);
                    }
                    uiFragments++;

                    if (uiFragments >= MAX_FRAGMENTS_COUNT)
                    {
                        instance->SetData(DATA_JAINA_EVENT, DONE);
                        me->SetVisible(true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        summons.DespawnEntry(NPC_ARCANE_CIRCLE);
                    }

                    instance->SetData(DATA_FRAGMENTS, uiFragments);
                    instance->DoUpdateWorldState(WORLDSTATE_SHOW_FRAGMENTS, 1);
                    instance->DoUpdateWorldState(WORLDSTATE_FRAGMENTS_COLLECTED, uiFragments);
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);

                // Quest
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                            if (me->GetDistance2d(player) <= 150.0f && player->GetQuestStatus(30097) == QUEST_STATUS_INCOMPLETE)
                                DoCast(player, SPELL_ARCHIVED_JAINA, true);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                BossAI::JustSummoned(summon);

                switch (summon->GetEntry())
                {
                    case NPC_FROSTBLADES:
                    {
                        summon->SetReactState(REACT_PASSIVE);
                        float x, y, z;
                        summon->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 75.0f);
                        summon->GetMotionMaster()->MovePoint(1, x, y, z);
                        summon->DespawnOrUnsummon(10000);
                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_BLINK, 30000);
                events.ScheduleEvent(EVENT_PYROBLAST, 1000);
                events.ScheduleEvent(EVENT_FLARECORE, urand(10000, 20000));

                instance->SetBossState(DATA_ECHO_OF_JAINA, IN_PROGRESS);
                DoZoneInCombat();
            }

            void AttackStart(Unit* target) OVERRIDE
            {
                if (target)
                    me->Attack(target, false);
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
                        case EVENT_PYROBLAST:
                            DoCast(me->GetVictim(), SPELL_PYROBLAST);
                            events.ScheduleEvent(EVENT_PYROBLAST, urand(3500, 4500));
                            break;
                        case EVENT_FLARECORE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_FLARECORE_MISSILE);
                            events.ScheduleEvent(EVENT_FLARECORE, urand(19000, 21000));
                            break;
                        case EVENT_BLINK:
                        {
                            std::list<Creature*> creatures;
                            me->GetCreatureListWithEntryInGrid(creatures, NPC_BLINK_TARGET, 40.0f);
                            if (!creatures.empty())
                            {
                                Skyfire::Containers::RandomResizeList(creatures, 1);
                                DoCast(me, SPELL_BLINK, true);
                                if (Creature* target = creatures.front())
                                    me->NearTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0f, true);
                            }
                            me->UpdateObjectVisibility();
                            events.CancelEvent(EVENT_PYROBLAST);
                            events.CancelEvent(EVENT_FLARECORE);
                            events.ScheduleEvent(EVENT_FROST_BLADES, 1500);
                            events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 2000);
                            events.ScheduleEvent(EVENT_BLINK, 31500);
                            break;
                        }
                        case EVENT_FROSTBOLT_VOLLEY:
                            if (uiVolleyCount < 3)
                            {
                                DoCast(me, SPELL_FROSTBOLT_VOLLEY);
                                events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 2200);
                                uiVolleyCount++;
                            }
                            else
                            {
                                events.CancelEvent(EVENT_FROSTBOLT_VOLLEY);
                                uiVolleyCount = 0;
                                events.ScheduleEvent(EVENT_PYROBLAST, urand(3000, 3500));
                                events.ScheduleEvent(EVENT_FLARECORE, urand (7500, 8500));
                            }
                            break;
                        case EVENT_FROST_BLADES:
                            Talk(SAY_SPELL);
                            me->SummonCreature(NPC_FROSTBLADES, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation() - (M_PI / 4.0f), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30000);
                            me->SummonCreature(NPC_FROSTBLADES, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30000);
                            me->SummonCreature(NPC_FROSTBLADES, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation() + (M_PI / 4.0f), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30000);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<boss_echo_of_jainaAI>(creature);
        }
};

class npc_echo_of_jaina_blink_target : public CreatureScript
{
    public:
        npc_echo_of_jaina_blink_target() : CreatureScript("npc_echo_of_jaina_blink_target") { }

        struct npc_echo_of_jaina_blink_targetAI : public ScriptedAI
        {
            npc_echo_of_jaina_blink_targetAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                SetCombatMovement(false);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_jaina_blink_targetAI>(creature);
        }
};

class npc_echo_of_jaina_flarecore : public CreatureScript
{
    public:
        npc_echo_of_jaina_flarecore() : CreatureScript("npc_echo_of_jaina_flarecore") { }

        struct npc_echo_of_jaina_flarecoreAI : public ScriptedAI
        {
            npc_echo_of_jaina_flarecoreAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                SetCombatMovement(false);
            }

            EventMap events;

            // The flare is passive and never enters combat, so its timers start when it appears
            void Reset() OVERRIDE
            {
                if (!me->HasAura(SPELL_CHECK_PLAYER_DIST))
                    me->AddAura(SPELL_CHECK_PLAYER_DIST, me);

                events.Reset();
                events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                events.ScheduleEvent(EVENT_EXPLODE, 10000);
                events.ScheduleEvent(EVENT_FLARE_UP, 1000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_PLAYER:
                            if (me->SelectNearestPlayer(3.0f))
                            {
                                DoCast(me, SPELL_UNSTABLE_FLARE);
                                me->DespawnOrUnsummon(100);
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                            break;
                        case EVENT_EXPLODE:
                            me->RemoveAurasDueToSpell(SPELL_CHECK_PLAYER_DIST);
                            DoCast(me, SPELL_TIME_EXPIRE_FLARE);
                            me->DespawnOrUnsummon(10000);
                            events.CancelEvent(EVENT_CHECK_PLAYER);
                            events.CancelEvent(EVENT_FLARE_UP);
                            break;
                        case EVENT_FLARE_UP:
                            DoCast(me, SPELL_FLARE_UP, true);
                            events.ScheduleEvent(EVENT_FLARE_UP, 1000);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_jaina_flarecoreAI>(creature);
        }
};

class go_echo_of_jaina_jaina_staff_fragment : public GameObjectScript
{
    public:
        go_echo_of_jaina_jaina_staff_fragment() : GameObjectScript("go_echo_of_jaina_jaina_staff_fragment") { }

        bool OnGossipHello(Player* /*player*/, GameObject* go) OVERRIDE
        {
            if (go->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE))
                return true;

            InstanceScript* instance = go->GetInstanceScript();
            if (!instance)
                return true;

            if (Creature* pJaina = ObjectAccessor::GetCreature(*go, instance->GetData64(DATA_ECHO_OF_JAINA)))
                pJaina->AI()->DoAction(ACTION_FRAGMENTS);

            go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
            go->SetPhaseMask(256, true);
            go->SetGoState(GOState::GO_STATE_ACTIVE);
            return true;
        }
};

class spell_echo_of_jaina_frost_blades : public SpellScriptLoader
{
    public:
        spell_echo_of_jaina_frost_blades() : SpellScriptLoader("spell_echo_of_jaina_frost_blades") { }

        class spell_echo_of_jaina_frost_blades_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_jaina_frost_blades_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (GetCaster())
                    targets.remove_if(DistanceCheck(GetCaster()));
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_echo_of_jaina_frost_blades_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }

        private:
            struct DistanceCheck
            {
                DistanceCheck(Unit* caster) : _caster(caster) { }

                bool operator()(WorldObject* target)
                {
                    float dist = _caster->GetExactDist2d(target);
                    if (dist > 4)
                        return true;

                    float angle = _caster->GetRelativeAngle(target);
                    if (angle <= -float(M_PI) || angle > float(M_PI))
                    {
                        angle = fmod(angle, 2.0f * float(M_PI));
                        if (angle <= -float(M_PI))
                            angle += 2.0f * float(M_PI);
                        else if (angle > float(M_PI))
                            angle -= 2.0f * float(M_PI);
                    }

                    return dist > 4 * (fabs(fabs(angle) / M_PI - 0.5f) + 0.5f);
                }

                Unit* _caster;
            };
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_echo_of_jaina_frost_blades_SpellScript();
        }
};

void AddSC_boss_echo_of_jaina()
{
    new boss_echo_of_jaina();
    new npc_echo_of_jaina_flarecore();
    new npc_echo_of_jaina_blink_target();
    new go_echo_of_jaina_jaina_staff_fragment();
    new spell_echo_of_jaina_frost_blades();
}
