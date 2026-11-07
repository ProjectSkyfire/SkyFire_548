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
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "GridNotifiers.h"
#include "halls_of_origination.h"

#include <list>

enum ScriptTexts
{
    SAY_DEATH       = 0,
    SAY_AGGRO       = 1,
    SAY_SUPERNOVA   = 2,
    SAY_KILL        = 3,
};

enum Spells
{
    SPELL_SUPERNOVA             = 74136,
    SPELL_ASTRAL_RAIN1          = 74134,
    SPELL_ASTRAL_RAIN2          = 74365,
    SPELL_ASTRAL_RAIN3          = 74371,
    SPELL_CELESTIAL_CALL1       = 74362,
    SPELL_CELESTIAL_CALL2       = 74355,
    SPELL_CELESTIAL_CALL3       = 74364,
    SPELL_VEIL_OF_SKY1          = 74133,
    SPELL_VEIL_OF_SKY2          = 74372,
    SPELL_VEIL_OF_SKY3          = 74373,
    SPELL_ARCANE_BARRAGE        = 74374,
    SPELL_MIRROR_IMAGE_VS       = 74261, // summon veil of sky
    SPELL_MIRROR_IMAGE_AR       = 74262, // summon astral rain
    SPELL_MIRROR_IMAGE_CC       = 74263, // summon celestial call
};

enum Events
{
    EVENT_SUPERNOVA         = 1,
    EVENT_ASTRAL_RAIN       = 2,
    EVENT_CELESTIAL_CALL    = 3,
    EVENT_VEIL_OF_SKY       = 4,
    EVENT_ENTER_COMBAT      = 5, // for images
    EVENT_ARCANE_BARRAGE    = 7,
};

enum Adds
{
    NPC_ASTRAL_RAIN             = 39720, // 74265
    NPC_CELESTIAL_CALL          = 39721, // 74289
    NPC_VEIL_OF_SKY             = 39722,
    NPC_ASTRAL_FAMILIAR         = 39795,
    NPC_SPATIAL_FLUX            = 48707, // spawnmask=0
    NPC_ISISET_ADD_CONTROLLER   = 42382,
};

class boss_isiset : public CreatureScript
{
    public:
        boss_isiset() : CreatureScript("boss_isiset") { }

        struct boss_isisetAI : public BossAI
        {
            boss_isisetAI(Creature* creature) : BossAI(creature, DATA_ISISET)
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
            }

            uint8 phase;
            bool AstralRain, VeilOfSky, CelestialCall;

            void Reset() OVERRIDE
            {
                phase = 0;
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                DespawnCreatures(NPC_ASTRAL_FAMILIAR);
                AstralRain = true;
                VeilOfSky = true;
                CelestialCall = true;

                me->SetVisible(true);
                me->SetReactState(REACT_AGGRESSIVE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_SUPERNOVA, urand(7000, 10000));
                events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(9000, 15000));
                events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(10000, 17000));
                events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(6000, 10000));
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                phase = 0;
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Talk(SAY_DEATH);
                DespawnCreatures(NPC_ASTRAL_FAMILIAR);
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                BossAI::SummonedCreatureDespawn(summon);

                if (!summon->IsAlive() && (phase == 1 || phase == 3))
                {
                    switch (summon->GetEntry())
                    {
                        case NPC_ASTRAL_RAIN:
                            AstralRain = false;
                            phase++;
                            summons.DespawnEntry(NPC_CELESTIAL_CALL);
                            summons.DespawnEntry(NPC_VEIL_OF_SKY);
                            me->SetVisible(true);
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_SUPERNOVA, urand(7000, 10000));
                            events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(9000, 15000));
                            events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(10000, 17000));
                            events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(6000, 10000));
                            break;
                        case NPC_CELESTIAL_CALL:
                            CelestialCall = false;
                            phase++;
                            summons.DespawnEntry(NPC_ASTRAL_RAIN);
                            summons.DespawnEntry(NPC_VEIL_OF_SKY);
                            me->SetVisible(true);
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_SUPERNOVA, urand(7000, 10000));
                            events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(9000, 15000));
                            events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(10000, 17000));
                            events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(6000, 10000));
                            break;
                        case NPC_VEIL_OF_SKY:
                            VeilOfSky = false;
                            phase++;
                            summons.DespawnEntry(NPC_CELESTIAL_CALL);
                            summons.DespawnEntry(NPC_ASTRAL_RAIN);
                            me->SetVisible(true);
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_SUPERNOVA, urand(7000, 10000));
                            events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(9000, 15000));
                            events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(10000, 17000));
                            events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(6000, 10000));
                            break;
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if ((me->HealthBelowPct(67) && phase == 0) ||
                    (me->HealthBelowPct(34) && phase == 2))
                {
                    phase++;
                    events.Reset();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    if (CelestialCall)
                         DoCast(me, SPELL_MIRROR_IMAGE_CC, true);
                    if (AstralRain)
                        DoCast(me, SPELL_MIRROR_IMAGE_AR, true);
                    if (VeilOfSky)
                        DoCast(me, SPELL_MIRROR_IMAGE_VS, true);
                    me->SetVisible(false);
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUPERNOVA:
                            Talk(SAY_SUPERNOVA);
                            DoCastAOE(SPELL_SUPERNOVA);
                            events.ScheduleEvent(EVENT_SUPERNOVA, urand(20000, 25000));
                            break;
                        case EVENT_ASTRAL_RAIN:
                            if (AstralRain)
                            {
                                if (phase == 0) DoCastAOE(SPELL_ASTRAL_RAIN1);
                                else if (phase == 2) DoCastAOE(SPELL_ASTRAL_RAIN2);
                                else if (phase == 4) DoCastAOE(SPELL_ASTRAL_RAIN3);
                                events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(15000, 22000));
                            }
                            break;
                        case EVENT_CELESTIAL_CALL:
                            if (CelestialCall)
                            {
                                if (phase == 0) DoCast(me, SPELL_CELESTIAL_CALL1);
                                else if (phase == 2) DoCast(me, SPELL_CELESTIAL_CALL2);
                                else if (phase == 4) DoCast(me, SPELL_CELESTIAL_CALL3);
                                events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(19000, 24000));
                            }
                            break;
                        case EVENT_VEIL_OF_SKY:
                            if (VeilOfSky)
                            {
                                if (phase == 0) DoCast(me, SPELL_VEIL_OF_SKY1);
                                else if (phase == 2) DoCast(me, SPELL_VEIL_OF_SKY2);
                                else if (phase == 4) DoCast(me, SPELL_VEIL_OF_SKY3);
                                events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(25000, 27000));
                            }
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            void DespawnCreatures(uint32 entry)
            {
                std::list<Creature*> creatures;
                GetCreatureListWithEntryInGrid(creatures, me, entry, 1000.0f);

                if (creatures.empty())
                   return;

                for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                     (*iter)->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_isisetAI(creature);
        }
};

class npc_isiset_astral_rain : public CreatureScript
{
    public:
        npc_isiset_astral_rain() : CreatureScript("npc_isiset_astral_rain") { }

        struct npc_isiset_astral_rainAI : public ScriptedAI
        {
            npc_isiset_astral_rainAI(Creature* creature) : ScriptedAI(creature)
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
            }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_ASTRAL_RAIN, urand(5000, 15000));
                events.ScheduleEvent(EVENT_ENTER_COMBAT, 2000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
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
                        case EVENT_ENTER_COMBAT:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->GetVictim());
                            break;
                        case EVENT_ASTRAL_RAIN:
                            DoCast(me, SPELL_ASTRAL_RAIN1);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_isiset_astral_rainAI(creature);
        }
};

class npc_isiset_celestial_call : public CreatureScript
{
    public:
        npc_isiset_celestial_call() : CreatureScript("npc_isiset_celestial_call") { }

        struct npc_isiset_celestial_callAI : public ScriptedAI
        {
            npc_isiset_celestial_callAI(Creature* creature) : ScriptedAI(creature)
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
            }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                events.ScheduleEvent(EVENT_CELESTIAL_CALL, urand(5000, 15000));
                events.ScheduleEvent(EVENT_ENTER_COMBAT, 2000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
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
                        case EVENT_ENTER_COMBAT:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->GetVictim());
                            break;
                        case EVENT_CELESTIAL_CALL:
                            DoCast(me, SPELL_CELESTIAL_CALL1);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_isiset_celestial_callAI(creature);
        }
};

class npc_isiset_veil_of_sky : public CreatureScript
{
    public:
        npc_isiset_veil_of_sky() : CreatureScript("npc_isiset_veil_of_sky") { }

        struct npc_isiset_veil_of_skyAI : public ScriptedAI
        {
            npc_isiset_veil_of_skyAI(Creature* creature) : ScriptedAI(creature)
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
            }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                events.ScheduleEvent(EVENT_VEIL_OF_SKY, urand(5000, 15000));
                events.ScheduleEvent(EVENT_ENTER_COMBAT, 2000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
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
                        case EVENT_ENTER_COMBAT:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->GetVictim());
                            break;
                        case EVENT_VEIL_OF_SKY:
                            DoCast(me, SPELL_VEIL_OF_SKY1);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_isiset_veil_of_skyAI(creature);
        }
};

class npc_isiset_astral_familiar : public CreatureScript
{
    public:
        npc_isiset_astral_familiar() : CreatureScript("npc_isiset_astral_familiar") { }

        struct npc_isiset_astral_familiarAI : public ScriptedAI
        {
            npc_isiset_astral_familiarAI(Creature* creature) : ScriptedAI(creature)
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
            }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                events.ScheduleEvent(EVENT_ARCANE_BARRAGE, urand(3000, 7000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
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
                        case EVENT_ARCANE_BARRAGE:
                            DoCastAOE(SPELL_ARCANE_BARRAGE);
                            events.ScheduleEvent(EVENT_ARCANE_BARRAGE, urand(8000, 15000));
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_isiset_astral_familiarAI(creature);
        }
};

class OrientationCheck
{
    public:
        explicit OrientationCheck(WorldObject* _caster) : caster(_caster) { }

        bool operator() (WorldObject* unit)
        {
            return !unit->isInFront(caster, 2.5f);
        }

    private:
        WorldObject* caster;
};

class spell_isiset_supernova_dis : public SpellScriptLoader
{
    public:
        spell_isiset_supernova_dis() : SpellScriptLoader("spell_isiset_supernova_dis") { }

        class spell_isiset_supernova_dis_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_isiset_supernova_dis_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                if (Unit* caster = GetCaster())
                    unitList.remove_if(OrientationCheck(caster));
                else
                    unitList.clear();
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_isiset_supernova_dis_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_isiset_supernova_dis_SpellScript();
        }
};

class spell_isiset_supernova_dmg : public SpellScriptLoader
{
    public:
        spell_isiset_supernova_dmg() : SpellScriptLoader("spell_isiset_supernova_dmg") { }

        class spell_isiset_supernova_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_isiset_supernova_dmg_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                if (Unit* caster = GetCaster())
                    unitList.remove_if(OrientationCheck(caster));
                else
                    unitList.clear();
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_isiset_supernova_dmg_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_isiset_supernova_dmg_SpellScript();
        }
};

void AddSC_boss_isiset()
{
    new boss_isiset();
    new npc_isiset_astral_rain();
    new npc_isiset_celestial_call();
    new npc_isiset_veil_of_sky();
    new npc_isiset_astral_familiar();
    new spell_isiset_supernova_dis();
    new spell_isiset_supernova_dmg();
}
