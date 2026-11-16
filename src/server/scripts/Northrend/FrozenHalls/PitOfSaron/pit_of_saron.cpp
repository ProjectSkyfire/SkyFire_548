/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "pit_of_saron.h"
#include "Vehicle.h"
#include "PassiveAI.h"
#include "ObjectAccessor.h"
#include "Player.h"

enum Spells
{
    SPELL_FIREBALL              = 69583, //Ymirjar Flamebearer
    SPELL_HELLFIRE              = 69586,
    SPELL_TACTICAL_BLINK        = 69584,
    SPELL_FROST_BREATH          = 69527, //Iceborn Proto-Drake
    SPELL_LEAPING_FACE_MAUL     = 69504, // Geist Ambusher
};

enum Events
{
    // Ymirjar Flamebearer
    EVENT_FIREBALL              = 1,
    EVENT_TACTICAL_BLINK        = 2,
};

bool ScheduledIcicleSummons::Execute(uint64 /*time*/, uint32 /*diff*/)
{
    if (roll_chance_i(12))
    {
        _trigger->CastSpell(_trigger, SPELL_ICICLE_SUMMON, true);
        _trigger->m_Events.AddEvent(new ScheduledIcicleSummons(_trigger), _trigger->m_Events.CalculateTime(urand(20000, 35000)));
    }
    else
        _trigger->m_Events.AddEvent(new ScheduledIcicleSummons(_trigger), _trigger->m_Events.CalculateTime(urand(1000, 20000)));

    return true;
}

class npc_ymirjar_flamebearer : public CreatureScript
{
    public:
        npc_ymirjar_flamebearer() : CreatureScript("npc_ymirjar_flamebearer") { }

        struct npc_ymirjar_flamebearerAI: public ScriptedAI
        {
            npc_ymirjar_flamebearerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset() OVERRIDE
            {
                _events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _events.ScheduleEvent(EVENT_FIREBALL, 4000);
                _events.ScheduleEvent(EVENT_TACTICAL_BLINK, 15000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FIREBALL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_FIREBALL);
                            _events.RescheduleEvent(EVENT_FIREBALL, 5000);
                            break;
                        case EVENT_TACTICAL_BLINK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_TACTICAL_BLINK);
                            DoCast(me, SPELL_HELLFIRE);
                            _events.RescheduleEvent(EVENT_TACTICAL_BLINK, 12000);
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
            return new npc_ymirjar_flamebearerAI(creature);
        }
};

class npc_iceborn_protodrake : public CreatureScript
{
    public:
        npc_iceborn_protodrake() : CreatureScript("npc_iceborn_protodrake") { }

        struct npc_iceborn_protodrakeAI: public ScriptedAI
        {
            npc_iceborn_protodrakeAI(Creature* creature) : ScriptedAI(creature), _vehicle(creature->GetVehicleKit())
            {
                ASSERT(_vehicle);
            }

            void Reset() OVERRIDE
            {
                _frostBreathCooldown = 5000;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _vehicle->RemoveAllPassengers();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (_frostBreathCooldown < diff)
                {
                    DoCastVictim(SPELL_FROST_BREATH);
                    _frostBreathCooldown = 10000;
                }
                else
                    _frostBreathCooldown -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            Vehicle* _vehicle;
            uint32 _frostBreathCooldown;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_iceborn_protodrakeAI(creature);
        }
};

class npc_geist_ambusher : public CreatureScript
{
    public:
        npc_geist_ambusher() : CreatureScript("npc_geist_ambusher") { }

        struct npc_geist_ambusherAI: public ScriptedAI
        {
            npc_geist_ambusherAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset() OVERRIDE
            {
                _leapingFaceMaulCooldown = 9000;
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                // the max range is determined by aggro range
                if (me->GetDistance(who) > 5.0f)
                    DoCast(who, SPELL_LEAPING_FACE_MAUL);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (_leapingFaceMaulCooldown < diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 5.0f, true))
                        DoCast(target, SPELL_LEAPING_FACE_MAUL);
                    _leapingFaceMaulCooldown = urand(9000, 14000);
                }
                else
                    _leapingFaceMaulCooldown -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            uint32 _leapingFaceMaulCooldown;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_geist_ambusherAI(creature);
        }
};

class spell_trash_npc_glacial_strike : public SpellScriptLoader
{
    public:
        spell_trash_npc_glacial_strike() : SpellScriptLoader("spell_trash_npc_glacial_strike") { }

        class spell_trash_npc_glacial_strike_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_trash_npc_glacial_strike_AuraScript);

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                if (GetTarget()->IsFullHealth())
                {
                    GetTarget()->RemoveAura(GetId(), 0, 0, AURA_REMOVE_BY_ENEMY_SPELL);
                    PreventDefaultAction();
                }
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_trash_npc_glacial_strike_AuraScript::PeriodicTick, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_trash_npc_glacial_strike_AuraScript();
        }
};

class npc_pit_of_saron_icicle : public CreatureScript
{
    public:
        npc_pit_of_saron_icicle() : CreatureScript("npc_pit_of_saron_icicle") { }

        struct npc_pit_of_saron_icicleAI : public PassiveAI
        {
            npc_pit_of_saron_icicleAI(Creature* creature) : PassiveAI(creature), _fallTimer(0), _summonerGUID(0)
            {
                me->SetDisplayId(me->GetCreatureTemplate()->Modelid1);
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                _summonerGUID = summoner->GetGUID();
                _fallTimer = 3650;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!_fallTimer)
                    return;

                if (_fallTimer > diff)
                {
                    _fallTimer -= diff;
                    return;
                }

                _fallTimer = 0;
                DoCast(me, SPELL_ICICLE_FALL_TRIGGER, true);
                DoCast(me, SPELL_ICICLE_FALL_VISUAL);

                if (Unit* caster = ObjectAccessor::GetUnit(*me, _summonerGUID))
                    caster->RemoveDynObject(SPELL_ICICLE_SUMMON);
            }

        private:
            uint32 _fallTimer;
            uint64 _summonerGUID;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pit_of_saron_icicleAI(creature);
        }
};

enum TyrannusEventCavernEmote
{
    SAY_TYRANNUS_CAVERN_ENTRANCE = 3
};

class at_pit_cavern_entrance : public AreaTriggerScript
{
    public:
        at_pit_cavern_entrance() : AreaTriggerScript("at_pit_cavern_entrance") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
            {
                if (instance->GetData(DATA_CAVERN_ACTIVE))
                    return true;

                instance->SetData(DATA_CAVERN_ACTIVE, 1);

                if (Creature* tyrannus = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_TYRANNUS_EVENT)))
                    tyrannus->AI()->Talk(SAY_TYRANNUS_CAVERN_ENTRANCE);
            }

            return true;
        }
};

class at_pit_cavern_end : public AreaTriggerScript
{
    public:
        at_pit_cavern_end() : AreaTriggerScript("at_pit_cavern_end") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                instance->SetData(DATA_CAVERN_ACTIVE, 0);

            return true;
        }
};

void AddSC_pit_of_saron()
{
    new npc_ymirjar_flamebearer();
    new npc_iceborn_protodrake();
    new npc_geist_ambusher();
    new spell_trash_npc_glacial_strike();
    new npc_pit_of_saron_icicle();
    new at_pit_cavern_entrance();
    new at_pit_cavern_end();
}
