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
    SPELL_BAINE_VISUALS     = 101624,

    SPELL_THROW_TOTEM       = 101615,
    SPELL_PULVERIZE         = 101626,
    SPELL_PULVERIZE_DMG     = 101627,
    SPELL_PULVERIZE_AOE     = 101625,
    SPELL_MOLTEN_AXE        = 101836,
    SPELL_MOLTEN_FIST       = 101866,
    SPELL_THROW_TOTEM_BACK  = 101602,
    SPELL_THROW_TOTEM_AURA  = 107837,
};

enum Events
{
    EVENT_PULVERIZE     = 1,
    EVENT_THROW_TOTEM   = 2,
    EVENT_CHECK_SELF    = 3,
};

enum Adds
{
    NPC_ROCK_ISLAND     = 54496,
    NPC_BAINES_TOTEM    = 54434,
};

class boss_echo_of_baine : public CreatureScript
{
    public:
        boss_echo_of_baine() : CreatureScript("boss_echo_of_baine") { }

        struct boss_echo_of_baineAI : public BossAI
        {
            boss_echo_of_baineAI(Creature* creature) : BossAI(creature, DATA_ECHO_OF_BAINE)
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
                me->setActive(true);
                bIntroDone = false;
            }

            bool bIntroDone;

            void Reset() OVERRIDE
            {
                _Reset();
                me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 5.0f);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 5.0f);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_PULVERIZE, 30000);
                events.ScheduleEvent(EVENT_THROW_TOTEM, 10000);
                events.ScheduleEvent(EVENT_CHECK_SELF, 1000);

                _EnterCombat();
            }

            void EnterEvadeMode() OVERRIDE
            {
                instance->SetData(DATA_PLATFORMS, NOT_STARTED);
                BossAI::EnterEvadeMode();
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (bIntroDone)
                    return;

                if (who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (!me->IsWithinDistInMap(who, 100.0f, false))
                    return;

                Talk(SAY_INTRO);
                bIntroDone = true;
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THROW_TOTEM_AURA);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THROW_TOTEM_BACK);

                // Quest
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                            if (me->GetDistance2d(player) <= 150.0f && player->GetQuestStatus(30097) == QUEST_STATUS_INCOMPLETE)
                                DoCast(player, SPELL_ARCHIVED_BAINE, true);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == EFFECT_MOTION_TYPE)
                    if (pointId == EVENT_JUMP)
                        DoCastAOE(SPELL_PULVERIZE_DMG);
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
                        case EVENT_CHECK_SELF:
                            if (me->IsInWater() && !me->HasAura(SPELL_MOLTEN_AXE))
                                DoCast(me, SPELL_MOLTEN_AXE);
                            if (me->GetPositionY() < 1398.0f)
                            {
                                EnterEvadeMode();
                                return;
                            }
                            events.ScheduleEvent(EVENT_CHECK_SELF, 1000);
                            break;
                        case EVENT_PULVERIZE:
                            Talk(SAY_SPELL);
                            DoCastAOE(SPELL_PULVERIZE_AOE);
                            events.ScheduleEvent(EVENT_PULVERIZE, 40000);
                            break;
                        case EVENT_THROW_TOTEM:
                        {
                            Unit* target = NULL;
                            target = SelectTarget(SELECT_TARGET_FARTHEST, 1, PositionSelector());
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, PositionSelector());
                            if (target)
                            {
                                DoCast(target, SPELL_THROW_TOTEM);
                                events.ScheduleEvent(EVENT_THROW_TOTEM, 26000);
                            }
                            else
                                events.ScheduleEvent(EVENT_THROW_TOTEM, 1000);
                            break;
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            struct PositionSelector
            {
                public:
                    //PositionSelector(bool b) : _b(b) { }

                    bool operator()(Unit const* target) const
                    {
                        if (target->GetTypeId() != TypeID::TYPEID_PLAYER)
                            return false;

                        if (target->GetAreaId() != AREA_OBSIDIAN)
                            return false;

                        if (target->IsInWater())
                            return false;

                        return true;
                    }
                private:
                    bool _b;
            };
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<boss_echo_of_baineAI>(creature);
        }
};

class npc_echo_of_baine_baines_totem : public CreatureScript
{
    public:
        npc_echo_of_baine_baines_totem() : CreatureScript("npc_echo_of_baine_baines_totem") { }

        struct npc_echo_of_baine_baines_totemAI : public ScriptedAI
        {
            npc_echo_of_baine_baines_totemAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                bDespawn = false;
                SetCombatMovement(false);
            }

            bool bDespawn;

            void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
            {
                if (!bDespawn)
                {
                    bDespawn = true;
                    me->DespawnOrUnsummon();
                    clicker->SetSheath(SHEATH_STATE_UNARMED);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_baine_baines_totemAI>(creature);
        }
};

class spell_echo_of_baine_pulverize_aoe : public SpellScriptLoader
{
    public:
        spell_echo_of_baine_pulverize_aoe() : SpellScriptLoader("spell_echo_of_baine_pulverize_aoe") { }

        class spell_echo_of_baine_pulverize_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_baine_pulverize_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster() || !GetCaster()->GetVictim())
                    return;

                std::list<WorldObject*> tempList;
                for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if ((*itr)->ToUnit() && !(*itr)->ToUnit()->IsInWater())
                        tempList.push_back((*itr));

                if (tempList.size() > 1)
                    tempList.remove(GetCaster()->GetVictim());

                targets.clear();
                if (!tempList.empty())
                    targets.push_back(Skyfire::Containers::SelectRandomContainerElement(tempList));
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_PULVERIZE, true);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_echo_of_baine_pulverize_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_echo_of_baine_pulverize_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_echo_of_baine_pulverize_aoe_SpellScript();
        }
};

void AddSC_boss_echo_of_baine()
{
    new boss_echo_of_baine();
    new npc_echo_of_baine_baines_totem();
    new spell_echo_of_baine_pulverize_aoe();
}
