/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "halls_of_stone.h"

enum Spells
{
    SPELL_LIGHTING_RING                               = 51849, // Periodic Trigger (interval 2s) spell = 50841
    SPELL_LIGHTING_RING_1                             = 50840, // Periodic Trigger (interval 2s) spell = 50841
    SPELL_STATIC_CHARGE                               = 50834, // Periodic Trigger 2s interval, spell =50835
    SPELL_CHAIN_LIGHTING                              = 50830,
    SPELL_LIGHTING_SHIELD                             = 50831,
    SPELL_FRENZY                                      = 28747,
    SPELL_TOXIC_VOLLEY                                = 50838,

    // Each periodic dummy aura summons its creature at a random pipe
    SPELL_SUMMON_IRON_DWARF_PERIODIC                  = 50789,
    SPELL_SUMMON_IRON_TROGG_PERIODIC                  = 50792,
    SPELL_SUMMON_IRON_TROGG_PERIODIC_H                = 59859,
    SPELL_SUMMON_MALFORMED_OOZE_PERIODIC              = 50801,
    SPELL_SUMMON_MALFORMED_OOZE_PERIODIC_H            = 59858,
    SPELL_SUMMON_EARTHEN_DWARF_PERIODIC               = 50824
};

enum Yells
{
    SAY_AGGRO                                         = 0,
    SAY_SLAY                                          = 1,
    SAY_DEATH                                         = 2,
    EMOTE_FRENZY                                      = 3
};

enum SjonnirCreatures
{
    NPC_FORGED_IRON_TROGG                             = 27979,
    NPC_MALFORMED_OOZE                                = 27981,
    NPC_FORGED_IRON_DWARF                             = 27982,
    NPC_IRON_SLUDGE                                   = 28165,
    NPC_EARTHEN_DWARF                                 = 27980
};

enum Misc
{
    ACTION_OOZE_DEAD                                  = 1,
    DATA_ABUSE_THE_OOZE                               = 2
};

enum Events
{
    EVENT_CHAIN_LIGHTNING                             = 1,
    EVENT_LIGHTNING_SHIELD,
    EVENT_STATIC_CHARGE,
    EVENT_LIGHTNING_RING,
    EVENT_LIGHTNING_RING_FRENZY,
    EVENT_FRENZY
};

Position const PipeLocations[] =
{
    { 1295.44f, 734.07f, 200.3f, 0.0f }, // left
    { 1297.7f,  595.6f,  199.9f, 0.0f }  // right
};

Position const CenterPoint = { 1295.21f, 667.157f, 189.691f, 0.0f };

class boss_sjonnir : public CreatureScript
{
    public:
        boss_sjonnir() : CreatureScript("boss_sjonnir") { }

        struct boss_sjonnirAI : public BossAI
        {
            boss_sjonnirAI(Creature* creature) : BossAI(creature, DATA_SJONNIR) { }

            void Reset() OVERRIDE
            {
                _Reset();
                abuseTheOoze = 0;
                summonsTroggs = false;
                summonsOozes = false;
                summonsDwarfs = false;
                frenzied = false;
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (!instance->CheckRequiredBosses(DATA_SJONNIR, who->ToPlayer()))
                {
                    EnterEvadeMode();
                    return;
                }

                _EnterCombat();
                Talk(SAY_AGGRO);
                DoCast(me, SPELL_SUMMON_IRON_DWARF_PERIODIC, true);

                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(3000, 8000));
                events.ScheduleEvent(EVENT_LIGHTNING_SHIELD, urand(5000, 15000));
                events.ScheduleEvent(EVENT_STATIC_CHARGE, urand(20000, 25000));
                events.ScheduleEvent(EVENT_LIGHTNING_RING, 30000);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                switch (summon->GetEntry())
                {
                    case NPC_FORGED_IRON_DWARF:
                    case NPC_FORGED_IRON_TROGG:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 0.0f, true))
                            summon->AI()->AttackStart(target);
                        break;
                    case NPC_EARTHEN_DWARF:
                        summon->AI()->AttackStart(me);
                        break;
                    default:
                        summon->GetMotionMaster()->MovePoint(0, CenterPoint);
                        break;
                }

                summons.Summon(summon);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (!summonsTroggs && me->HealthBelowPctDamaged(75, damage))
                {
                    summonsTroggs = true;
                    me->RemoveAurasDueToSpell(sSpellMgr->GetSpellIdForDifficulty(SPELL_SUMMON_IRON_DWARF_PERIODIC, me));
                    DoCast(me, SPELL_SUMMON_IRON_TROGG_PERIODIC, true);
                }

                if (!summonsOozes && me->HealthBelowPctDamaged(50, damage))
                {
                    summonsOozes = true;
                    me->RemoveAurasDueToSpell(sSpellMgr->GetSpellIdForDifficulty(SPELL_SUMMON_IRON_TROGG_PERIODIC, me));
                    DoCast(me, SPELL_SUMMON_MALFORMED_OOZE_PERIODIC, true);
                }

                if (!summonsDwarfs && me->HealthBelowPctDamaged(25, damage))
                {
                    summonsDwarfs = true;
                    me->RemoveAurasDueToSpell(sSpellMgr->GetSpellIdForDifficulty(SPELL_SUMMON_MALFORMED_OOZE_PERIODIC, me));
                    DoCast(me, SPELL_SUMMON_EARTHEN_DWARF_PERIODIC, true);
                }

                if (!frenzied && me->HealthBelowPctDamaged(20, damage))
                {
                    frenzied = true;
                    events.CancelEvent(EVENT_LIGHTNING_RING);
                    events.ScheduleEvent(EVENT_FRENZY, 0);
                    events.ScheduleEvent(EVENT_LIGHTNING_RING_FRENZY, 0);
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void KilledUnit(Unit* who) OVERRIDE
            {
                if (who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_OOZE_DEAD)
                    ++abuseTheOoze;
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type == DATA_ABUSE_THE_OOZE)
                    return abuseTheOoze;

                return 0;
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
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                                DoCast(target, SPELL_CHAIN_LIGHTING);
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(10000, 15000));
                            break;
                        case EVENT_LIGHTNING_SHIELD:
                            if (!me->HasAura(sSpellMgr->GetSpellIdForDifficulty(SPELL_LIGHTING_SHIELD, me)))
                                DoCast(me, SPELL_LIGHTING_SHIELD);
                            events.ScheduleEvent(EVENT_LIGHTNING_SHIELD, urand(5000, 15000));
                            break;
                        case EVENT_STATIC_CHARGE:
                            DoCastVictim(SPELL_STATIC_CHARGE);
                            events.ScheduleEvent(EVENT_STATIC_CHARGE, urand(20000, 25000));
                            break;
                        case EVENT_LIGHTNING_RING:
                            DoCast(me, SPELL_LIGHTING_RING_1);
                            events.ScheduleEvent(EVENT_LIGHTNING_RING, 50000);
                            break;
                        case EVENT_LIGHTNING_RING_FRENZY:
                            DoCast(me, SPELL_LIGHTING_RING);
                            events.ScheduleEvent(EVENT_LIGHTNING_RING_FRENZY, 15000);
                            break;
                        case EVENT_FRENZY:
                            Talk(EMOTE_FRENZY);
                            DoCast(me, SPELL_FRENZY, true);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            private:
                uint8 abuseTheOoze;
                bool summonsTroggs;
                bool summonsOozes;
                bool summonsDwarfs;
                bool frenzied;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHallsOfStoneAI<boss_sjonnirAI>(creature);
        }
};

class npc_malformed_ooze : public CreatureScript
{
    public:
        npc_malformed_ooze() : CreatureScript("npc_malformed_ooze") { }

        struct npc_malformed_oozeAI : public ScriptedAI
        {
            npc_malformed_oozeAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                _mergeTimer = 10000;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (_mergeTimer <= diff)
                {
                    if (Creature* temp = me->FindNearestCreature(NPC_MALFORMED_OOZE, 3.0f, true))
                    {
                        DoSpawnCreature(NPC_IRON_SLUDGE, 0, 0, 0, 0, TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
                        temp->DisappearAndDie();
                        me->DisappearAndDie();
                    }
                    _mergeTimer = 3000;
                }
                else
                    _mergeTimer -= diff;

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:
            uint32 _mergeTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHallsOfStoneAI<npc_malformed_oozeAI>(creature);
        }
};

class npc_iron_sludge : public CreatureScript
{
    public:
        npc_iron_sludge() : CreatureScript("npc_iron_sludge") { }

        struct npc_iron_sludgeAI : public ScriptedAI
        {
            npc_iron_sludgeAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;
            uint32 _toxicVolleyTimer;

            void Reset() OVERRIDE
            {
                _toxicVolleyTimer = urand(3000, 6000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (Creature* sjonnir = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SJONNIR)))
                    sjonnir->AI()->DoAction(ACTION_OOZE_DEAD);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (_toxicVolleyTimer <= diff)
                {
                    DoCast(me, SPELL_TOXIC_VOLLEY);
                    _toxicVolleyTimer = urand(3000, 6000);
                }
                else
                    _toxicVolleyTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHallsOfStoneAI<npc_iron_sludgeAI>(creature);
        }
};

class achievement_abuse_the_ooze : public AchievementCriteriaScript
{
    public:
        achievement_abuse_the_ooze() : AchievementCriteriaScript("achievement_abuse_the_ooze")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target) OVERRIDE
        {
            if (!target)
                return false;

            if (Creature* Sjonnir = target->ToCreature())
                if (Sjonnir->AI()->GetData(DATA_ABUSE_THE_OOZE) >= 5)
                    return true;

            return false;
        }
};

class spell_sjonnir_periodic_summon : public AuraScript
{
    PrepareAuraScript(spell_sjonnir_periodic_summon);

    void SummonAtPipe()
    {
        uint32 entry = 0;
        switch (GetId())
        {
            case SPELL_SUMMON_IRON_DWARF_PERIODIC:
                entry = NPC_FORGED_IRON_DWARF;
                break;
            case SPELL_SUMMON_IRON_TROGG_PERIODIC:
            case SPELL_SUMMON_IRON_TROGG_PERIODIC_H:
                entry = NPC_FORGED_IRON_TROGG;
                break;
            case SPELL_SUMMON_MALFORMED_OOZE_PERIODIC:
            case SPELL_SUMMON_MALFORMED_OOZE_PERIODIC_H:
                entry = NPC_MALFORMED_OOZE;
                break;
            case SPELL_SUMMON_EARTHEN_DWARF_PERIODIC:
                entry = NPC_EARTHEN_DWARF;
                break;
            default:
                return;
        }

        GetTarget()->SummonCreature(entry, PipeLocations[urand(0, 1)], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        SummonAtPipe();
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        SummonAtPipe();
    }

    void Register() OVERRIDE
    {
        AfterEffectApply += AuraEffectApplyFn(spell_sjonnir_periodic_summon::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_sjonnir_periodic_summon::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

void AddSC_boss_sjonnir()
{
    new boss_sjonnir();
    new npc_malformed_ooze();
    new npc_iron_sludge();
    new achievement_abuse_the_ooze();
    RegisterAuraScript(spell_sjonnir_periodic_summon);
}
