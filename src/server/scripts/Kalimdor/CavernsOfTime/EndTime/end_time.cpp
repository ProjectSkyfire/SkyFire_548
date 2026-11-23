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

enum ScriptTexts
{
    // Nozdormu
    SAY_INTRO_1             = 0, // Tyrande
    SAY_INTRO_2             = 1, // Baine
    SAY_INTRO_3             = 2, // Jaina
    SAY_INTRO_4             = 3, // Sylvanas
    SAY_INTRO_5             = 4, // Sylvanas
};

enum Spells
{
    // Infinite Warden
    SPELL_VOID_SHIELD       = 102599,
    SPELL_VOID_STRIKE       = 102598,

    // Infinite Suppressor
    SPELL_ARCANE_WAVE       = 102601,
    SPELL_TEMPORAL_VORTEX   = 102600,

    // Time-Twisted Breaker
    SPELL_RUPTURE_GROUND    = 102124,
    SPELL_BREAK_ARMOR       = 102132,

    // Time-Twisted Drake
    SPELL_ENRAGE            = 102134,
    SPELL_FLAME_BREATH      = 102135,

    // Time-Twisted Footman
    SPELL_SHIELD_BASH       = 101817,
    SPELL_SHIELD_WALL       = 101811,
    SPELL_THUNDERCLAP       = 101820,

    // Time-Twisted Geist
    SPELL_CADAVER_TOSS      = 109952,
    SPELL_FLESH_RIP         = 102066,
    SPELL_CANNIBALIZE_DUMMY = 109945,
    SPELL_CANNIBALIZE       = 109944,

    // Time-Twisted Priest
    SPELL_POWER_WORD_SHIELD = 102409,
    SPELL_FOUNTAIN_OF_LIGHT = 102405,
    SPELL_LIGHT_RAIN_AURA   = 102406,
    SPELL_LIGHT_RAIN_DMG    = 102407,

    // Time-Twisted Riffleman
    SPELL_MULTI_SHOT        = 102411,
    SPELL_SHOT              = 102410,

    // Time-Twisted Scourge
    SPELL_FACE_KICK         = 101888,
    SPELL_WAIL_OF_THE_DEAD  = 101891,
    SPELL_PUTRID_SPIT       = 102063,

    // Time-Twisted Seer
    SPELL_CALL_FLAMES       = 102156,
    SPELL_SEAR_FLESH        = 102158,

    // Time-Twisted Sorceress
    SPELL_BLINK             = 101812,
    SPELL_ARCANE_BLAST      = 101816,
};

enum Events
{
    // Image of Nozdormu
    EVENT_TALK              = 1,
    EVENT_TALK_1            = 2,

    // Infinite Warden
    EVENT_VOID_SHIELD       = 3,
    EVENT_VOID_STRIKE       = 4,

    // Infinite Suppressor
    EVENT_ARCANE_WAVE       = 5,
    EVENT_TEMPORAL_VORTEX   = 6,
};

enum Adds
{
    NPC_INFINITE_WARDEN         = 54923,
    NPC_INFINITE_SUPPRESSOR     = 54920,

    NPC_TIME_TWISTED_BREAKER    = 54552,
    NPC_RUPTURED_GROUND         = 54566, // temp
    NPC_TIME_TWISTED_DRAKE      = 54543,
    NPC_TIME_TWISTED_SEER       = 54553,
    NPC_CALL_FLAMES             = 54585, // temp
    NPC_UNDYING_FLAME           = 54550,

    NPC_TIME_TWISTED_FOOTMAN    = 54687,
    NPC_TIME_TWISTED_PRIEST     = 54690,
    NPC_FOUNTAIN_OF_LIGHT       = 54795, // temp
    NPC_TIME_TWISTED_RIFFLEMAN  = 54693,
    NPC_TIME_TWISTED_SORCERESS  = 54691,

    NPC_TIME_TWISTED_SCOURGE    = 54507,
    NPC_TIME_TWISTED_GEIST      = 54511,
};

class npc_end_time_image_of_nozdormu : public CreatureScript
{
    public:
        npc_end_time_image_of_nozdormu() : CreatureScript("npc_end_time_image_of_nozdormu") { }

        struct npc_end_time_image_of_nozdormuAI : public ScriptedAI
        {
            npc_end_time_image_of_nozdormuAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                id = 0;
                bTalk = false;
                instance = me->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_TALK_JAINA:
                        id = 2;
                        break;
                    case ACTION_TALK_BAINE:
                        id = 1;
                        break;
                    case ACTION_TALK_TYRANDE:
                        id = 0;
                        break;
                    case ACTION_TALK_SYLVANAS:
                        id = 3;
                        break;
                }
                if (instance)
                {
                    if (instance->GetData(11 + id) != IN_PROGRESS)
                        return;
                    else
                        instance->SetData(11 + id, DONE);
                }
                bTalk = true;
                events.ScheduleEvent(EVENT_TALK, 15000);
                if (id == 3)
                    events.ScheduleEvent(EVENT_TALK_1, 25000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!bTalk)
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TALK:
                            Talk(id, 0);
                            if (id != 3)
                                bTalk = false;
                            break;
                        case EVENT_TALK_1:
                            Talk(SAY_INTRO_5, 0);
                            bTalk = false;
                            break;
                    }
                }
            }

        private:
            bool bTalk;
            uint8 id;
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_end_time_image_of_nozdormuAI>(creature);
        }
};

class npc_end_time_infinite_warden : public CreatureScript
{
    public:
        npc_end_time_infinite_warden() : CreatureScript("npc_end_time_infinite_warden") { }

        struct npc_end_time_infinite_wardenAI : public ScriptedAI
        {
            npc_end_time_infinite_wardenAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_VOID_SHIELD, urand(10000, 20000));
                events.ScheduleEvent(EVENT_VOID_STRIKE, urand(5000, 10000));
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
                        case EVENT_VOID_SHIELD:
                            DoCast(me, SPELL_VOID_SHIELD);
                            break;
                        case EVENT_VOID_STRIKE:
                            DoCast(me->GetVictim(), SPELL_VOID_STRIKE);
                            events.ScheduleEvent(EVENT_VOID_STRIKE, urand(8000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_end_time_infinite_wardenAI>(creature);
        }
};

class npc_end_time_infinite_suppressor : public CreatureScript
{
    public:
        npc_end_time_infinite_suppressor() : CreatureScript("npc_end_time_infinite_suppressor") { }

        struct npc_end_time_infinite_suppressorAI : public ScriptedAI
        {
            npc_end_time_infinite_suppressorAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_ARCANE_WAVE, 1000);
                events.ScheduleEvent(EVENT_TEMPORAL_VORTEX, 5000);
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
                        case EVENT_ARCANE_WAVE:
                            if (me->HasUnitState(UNIT_STATE_CASTING) && me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            {
                                events.RescheduleEvent(EVENT_ARCANE_WAVE, 5 * IN_MILLISECONDS);
                                break;
                            }

                            if (Unit* vict = me->GetVictim())
                                DoCast(vict, SPELL_ARCANE_WAVE);

                            events.ScheduleEvent(EVENT_ARCANE_WAVE, 3000);
                            break;
                        case EVENT_TEMPORAL_VORTEX:
                            me->InterruptNonMeleeSpells(true);

                            if (Unit* vict = me->GetVictim())
                                DoCast(vict, SPELL_TEMPORAL_VORTEX);

                            events.ScheduleEvent(SPELL_TEMPORAL_VORTEX, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_end_time_infinite_suppressorAI>(creature);
        }
};

// Time-Twisted Rifleman 54693
class npc_end_time_time_twisted_rifleman : public CreatureScript
{
    public:
        npc_end_time_time_twisted_rifleman() : CreatureScript("npc_end_time_time_twisted_rifleman") { }

        enum iSpells
        {
            SPELL_SHOT       = 102410,
            SPELL_MULTY_SHOT = 102411,
        };

        enum iEvents
        {
            EVENT_MULTY_SHOT = 1,
        };

        struct npc_end_time_time_twisted_riflemanAI : public ScriptedAI
        {
            npc_end_time_time_twisted_riflemanAI(Creature* creature) : ScriptedAI(creature) { }

            bool hasMelee;

            void Reset() OVERRIDE
            {
                hasMelee = false;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_MULTY_SHOT, urand(5 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_MULTY_SHOT)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 120.0f, true))
                            DoCast(target, SPELL_MULTY_SHOT);

                        events.ScheduleEvent(EVENT_MULTY_SHOT, urand(12 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                    }
                    break;
                }

                if (Unit* victim = me->GetVictim())
                {
                    if (!me->IsWithinMeleeRange(victim) && !me->HasUnitState(UNIT_STATE_CASTING))
                        DoCast(victim, SPELL_SHOT);
                    else if (!hasMelee)
                    {
                        me->InterruptNonMeleeSpells(true);
                        hasMelee = true;
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->AI()->AttackStart(victim);
                        me->GetMotionMaster()->MoveChase(victim);
                    }
                }

                if (hasMelee)
                    DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_end_time_time_twisted_riflemanAI>(creature);
        }
};

// Time-Twisted Priest 54690
class npc_end_time_time_twisted_priest : public CreatureScript
{
    public:
        npc_end_time_time_twisted_priest() : CreatureScript("npc_end_time_time_twisted_priest") { }

        enum iSpells
        {
            SPELL_POWER_WORD_SHIELD = 102409,
            SPELL_LIGHTSPRING       = 102405,
            SPELL_LIGHT_RAIN        = 102406,
        };

        enum iEvents
        {
            EVENT_SHIELD      = 1,
            EVENT_LIGHTSPRING = 2,
        };

        struct npc_end_time_time_twisted_priestAI : public ScriptedAI
        {
            npc_end_time_time_twisted_priestAI(Creature* creature) : ScriptedAI(creature), summons(me) { }

            SummonList summons;

            void Reset() OVERRIDE
            {
                summons.DespawnAll();
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                DoCast(me, SPELL_POWER_WORD_SHIELD);
                events.ScheduleEvent(EVENT_SHIELD, urand(13 * IN_MILLISECONDS, 18 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_LIGHTSPRING, urand(12 * IN_MILLISECONDS, 26 * IN_MILLISECONDS));
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
                summon->SetInCombatWithZone();
                summon->CastSpell(summon, SPELL_LIGHT_RAIN, false);
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
                        case EVENT_SHIELD:
                            DoCast(me, SPELL_POWER_WORD_SHIELD);
                            events.ScheduleEvent(EVENT_SHIELD, urand(13 * IN_MILLISECONDS, 18 * IN_MILLISECONDS));
                            break;
                        case EVENT_LIGHTSPRING:
                            DoCast(me, SPELL_LIGHTSPRING);
                            events.ScheduleEvent(EVENT_LIGHTSPRING, urand(18 * IN_MILLISECONDS, 26 * IN_MILLISECONDS));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_end_time_time_twisted_priestAI>(creature);
        }
};

// Time-Twisted Footman 54687
class npc_end_time_time_twisted_footman : public CreatureScript
{
    public:
        npc_end_time_time_twisted_footman() : CreatureScript("npc_end_time_time_twisted_footman") { }

        enum iSpells
        {
            SPELL_THUNDERCLAP = 101820,
            SPELL_SHIELD_WALL = 101811,
            SPELL_SHIELD_BASH = 101817,
        };

        enum iEvents
        {
            EVENT_THUNDERCLAP = 1,
            EVENT_SHIELD_BASH = 2,
        };

        struct npc_end_time_time_twisted_footmanAI : public ScriptedAI
        {
            npc_end_time_time_twisted_footmanAI(Creature* creature) : ScriptedAI(creature) { }

            bool hasShieldWall;

            void Reset() OVERRIDE
            {
                hasShieldWall = false;
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_THUNDERCLAP, urand(3 * IN_MILLISECONDS, 6 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SHIELD_BASH, urand(8 * IN_MILLISECONDS, 17.5 * IN_MILLISECONDS));
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (HealthBelowPct(40) && !hasShieldWall)
                {
                    hasShieldWall = true;
                    DoCast(me, SPELL_SHIELD_WALL);
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
                        case EVENT_THUNDERCLAP:
                            DoCast(me, SPELL_THUNDERCLAP);
                            events.ScheduleEvent(EVENT_THUNDERCLAP, urand(10 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                            break;
                        case EVENT_SHIELD_BASH:
                            if (Unit* vict = me->GetVictim())
                                DoCast(vict, SPELL_SHIELD_BASH);

                            events.ScheduleEvent(EVENT_SHIELD_BASH, urand(8 * IN_MILLISECONDS, 17.5 * IN_MILLISECONDS));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_end_time_time_twisted_footmanAI>(creature);
        }
};

// Time-Twisted Sorceress 54691
class npc_end_time_time_twisted_sorceress : public CreatureScript
{
    public:
        npc_end_time_time_twisted_sorceress() : CreatureScript("npc_end_time_time_twisted_sorceress") { }

        enum iSpells
        {
            SPELL_ARCANE_BLAST     = 101816,
            SPELL_ARCANE_BLAST_EFF = 102442,
            SPELL_BLINK            = 101812,
        };

        enum iEvents
        {
            EVENT_BLINK = 1,
        };

        struct npc_end_time_time_twisted_sorceressAI : public ScriptedAI
        {
            npc_end_time_time_twisted_sorceressAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_BLINK, urand(8 * IN_MILLISECONDS, 39 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_BLINK)
                    {
                        me->InterruptNonMeleeSpells(true);
                        DoCast(me, SPELL_BLINK);
                        events.ScheduleEvent(EVENT_BLINK, urand(8 * IN_MILLISECONDS, 39 * IN_MILLISECONDS));
                    }
                    break;
                }

                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    if (!me->IsWithinMeleeRange(target) && !me->HasUnitState(UNIT_STATE_CASTING))
                        DoCast(target, SPELL_ARCANE_BLAST);
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_end_time_time_twisted_sorceressAI>(creature);
        }
};

class BlinkPredicate
{
    public:
        BlinkPredicate(Unit* const m_caster) : _caster(m_caster) { }

        bool operator()(WorldObject* object)
        {
            return object && object->GetExactDist2d(_caster) < 6.0f;
        }

    private:
        Unit const* _caster;
};

// Blink 101812
class spell_end_time_blink : public SpellScriptLoader
{
    public:
        spell_end_time_blink() : SpellScriptLoader("spell_end_time_blink") { }

        class spell_end_time_blink_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_end_time_blink_AuraScript);

            void OnAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetOwner()->ToUnit())
                {
                    std::list<Creature*> blinkTargets;
                    GetCreatureListWithEntryInGrid(blinkTargets, caster, NPC_BLINK_TARGET, 40.0f);
                    blinkTargets.remove_if(BlinkPredicate(caster));

                    if (!blinkTargets.empty())
                        if (Creature* sTarget = Skyfire::Containers::SelectRandomContainerElement(blinkTargets))
                            caster->NearTeleportTo(sTarget->GetPositionX(), sTarget->GetPositionY(), sTarget->GetPositionZ(), sTarget->GetOrientation());
                }
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_end_time_blink_AuraScript::OnAuraEffectApply, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_end_time_blink_AuraScript();
        }
};

void AddSC_end_time()
{
    new npc_end_time_image_of_nozdormu();
    new npc_end_time_infinite_warden();
    new npc_end_time_infinite_suppressor();
    new npc_end_time_time_twisted_rifleman();
    new npc_end_time_time_twisted_priest();
    new npc_end_time_time_twisted_footman();
    new npc_end_time_time_twisted_sorceress();
    new spell_end_time_blink();
}
