/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "MoveSplineInit.h"
#include "Player.h"
#include "gate_of_the_setting_sun.h"

enum eSpells
{
    SPELL_PLANT_EXPLOSIVE         = 107187,
    SPELL_SABOTAGE                = 107268,
    SPELL_SABOTAGE_EXPLOSION      = 113645,
    SPELL_PLAYER_EXPLOSION        = 113654,
    SPELL_MUNITION_STABLE         = 109987,
    SPELL_MUNITION_EXPLOSION      = 107153,
    SPELL_MANTID_EXPLOSION        = 107215,
    SPELL_MUNITION_EXPLOSION_AURA = 107216
};

enum eEvents
{
    EVENT_EXPLOSIVES      = 1,
    EVENT_SABOTAGE        = 2,
    EVENT_INTRO_WAY       = 3,
    EVENT_WORLD_IN_FLAMES = 4,
};

enum eWorldInFlames
{
    WIF_NONE = 0,
    WIF_70   = 1,
    WIF_30   = 2,
};

enum eActions
{
    ACTION_INTRO   = 1,
    ACTION_WAY     = 2,
};

enum eTalks
{
    SAY_INTRO      = 0,
    SAY_AGGRO      = 1,
    SAY_SLAY       = 2,
    SAY_DEATH      = 3,
    SAY_SPECIAL    = 4,
};

class boss_saboteur_kiptilak : public CreatureScript
{
    public:
        boss_saboteur_kiptilak() : CreatureScript("boss_saboteur_kiptilak") { }

        struct boss_saboteur_kiptilakAI : public BossAI
        {
            boss_saboteur_kiptilakAI(Creature* creature) : BossAI(creature, DATA_KIPTILAK) { }

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                intro = false;

                Movement::MoveSplineInit init(me);
                for (Position const& itr : KiptilakInto)
                    init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                init.SetSmooth();
                init.SetCyclic();
                init.SetWalk(true);
                init.Launch();

                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                events.Reset();
                WorldInFlamesEvents = 0;
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_INTRO && !intro)
                {
                    intro = true;
                    Talk(SAY_INTRO);
                }
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                me->GetMotionMaster()->Clear();

                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_EXPLOSIVES, urand(7500, 10000));
                events.ScheduleEvent(EVENT_SABOTAGE, urand(22500, 30000));
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->SetBossState(DATA_KIPTILAK, FAIL);

                summons.DespawnAll();
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                float nextHealthPct = ((float(me->GetHealth()) - damage) / float(me->GetMaxHealth())) * 100;

                if (WorldInFlamesEvents < WIF_70 && nextHealthPct <= 70.0f)
                {
                    PrepareWorldInFlamesEvent();
                    ++WorldInFlamesEvents;
                }
                else if (WorldInFlamesEvents < WIF_30 && nextHealthPct <= 30.0f)
                {
                    PrepareWorldInFlamesEvent();
                    ++WorldInFlamesEvents;
                }
            }

            void PrepareWorldInFlamesEvent()
            {
                for (uint8 i = 0; i < 5; ++i)
                    me->CastSpell(frand(702, 740), frand(2292, 2320), 388.5f, SPELL_PLANT_EXPLOSIVE, true);

                events.ScheduleEvent(EVENT_WORLD_IN_FLAMES, 1 * IN_MILLISECONDS);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (summon->GetEntry() == NPC_STABLE_MUNITION)
                {
                    summon->AddAura(SPELL_MUNITION_STABLE, summon);
                    summon->SetReactState(REACT_PASSIVE);
                }

                summons.Summon(summon);
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
                        case EVENT_EXPLOSIVES:
                            for (uint8 i = 0; i < 3; ++i)
                                me->CastSpell(frand(702, 740), frand(2292, 2320), 388.5f, SPELL_PLANT_EXPLOSIVE, true);

                            events.ScheduleEvent(EVENT_EXPLOSIVES, urand(10, 11) * IN_MILLISECONDS);
                            break;
                        case EVENT_SABOTAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 15.0f, true))
                                me->CastSpell(target, SPELL_SABOTAGE, false);

                            events.ScheduleEvent(EVENT_SABOTAGE, urand(22500, 30000));
                            break;
                        case EVENT_WORLD_IN_FLAMES:
                        {
                            Talk(SAY_SPECIAL);

                            std::list<Creature*> munitionList;
                            GetCreatureListWithEntryInGrid(munitionList, me, NPC_STABLE_MUNITION, 100.0f);

                            for (Creature* munition : munitionList)
                            {
                                munition->RemoveAurasDueToSpell(SPELL_MUNITION_STABLE);
                                munition->CastSpell(munition, SPELL_MUNITION_EXPLOSION, true);
                                munition->DespawnOrUnsummon(2000);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
                EnterEvadeIfOutOfCombatArea(diff, 40.0f);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
                summons.DespawnAll();

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

        private:
            uint8 WorldInFlamesEvents;
            bool intro;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_saboteur_kiptilakAI(creature);
        }
};

class npc_munition_explosion_bunny : public CreatureScript
{
    public:
        npc_munition_explosion_bunny() : CreatureScript("npc_munition_explosion_bunny") { }

        enum eEvents
        {
            EVENT_CHECK = 1
        };

        struct npc_munition_explosion_bunnyAI : public ScriptedAI
        {
            npc_munition_explosion_bunnyAI(Creature* creature) : ScriptedAI(creature), orientation(0.0f), geoPosZ(0.0f), geoPosX(0.0f) { }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                orientation = 0.0f;
                geoPosZ = 0.0f;
                geoPosX = 0.0f;

                switch (me->GetEntry())
                {
                    case NPC_EXPLOSION_BUNNY_N_M:
                    case NPC_EXPLOSION_BUNNY_N_P:
                        orientation = 0.0f;
                        if (me->GetPositionY() < 2241.0f && summoner->GetEntry() != NPC_KIPTILAK) // geodata fix
                            geoPosX = 734.0f;
                        break;
                    case NPC_EXPLOSION_BUNNY_S_M:
                    case NPC_EXPLOSION_BUNNY_S_P:
                        orientation = M_PI;
                        if (me->GetPositionY() < 2241.0f && summoner->GetEntry() != NPC_KIPTILAK) // geodata fix
                            geoPosX = 709.7f;
                        break;
                    case NPC_EXPLOSION_BUNNY_E_M:
                    case NPC_EXPLOSION_BUNNY_E_P:
                        orientation = 4.71f;
                        if (summoner->GetEntry() != NPC_KIPTILAK)
                            geoPosZ = 402.56f;
                        break;
                    case NPC_EXPLOSION_BUNNY_W_M:
                    case NPC_EXPLOSION_BUNNY_W_P:
                        orientation = 1.57f;
                        break;
                    default:
                        break;
                }

                DetonateNearbyMunitions();

                events.ScheduleEvent(EVENT_CHECK, 500);
                float x = 0.0f;
                float y = 0.0f;
                GetPositionWithDistInOrientation(me, 40.0f, orientation, x, y);
                Movement::MoveSplineInit init(me);
                init.MoveTo(geoPosX ? geoPosX : x, y, geoPosZ ? geoPosZ : me->GetPositionZ());
                init.SetVelocity(10.0f);
                init.Launch();

                me->DespawnOrUnsummon(me->GetSplineDuration());
                me->CastSpell(me, SPELL_MUNITION_EXPLOSION_AURA, false);
            }

            void DetonateNearbyMunitions()
            {
                std::list<Creature*> munitionList;
                me->GetCreatureListWithEntryInGrid(munitionList, NPC_STABLE_MUNITION, 2.0f);

                for (Creature* munition : munitionList)
                {
                    if (munition->HasAura(SPELL_MUNITION_STABLE))
                    {
                        munition->RemoveAurasDueToSpell(SPELL_MUNITION_STABLE);
                        munition->CastSpell(munition, SPELL_MUNITION_EXPLOSION, true);
                        munition->DespawnOrUnsummon(2 * IN_MILLISECONDS);
                    }
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_CHECK)
                {
                    DetonateNearbyMunitions();

                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 2.0f);

                    for (Player* player : playerList)
                        me->CastSpell(player, SPELL_MANTID_EXPLOSION, false);

                    events.ScheduleEvent(EVENT_CHECK, 500);
                }
            }

        private:
            EventMap events;
            float orientation, geoPosZ, geoPosX;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_munition_explosion_bunnyAI(creature);
        }
};

class CheckMunitionExplosionPredicate
{
    public:
        CheckMunitionExplosionPredicate(Unit* caster) : _caster(caster) { }

        bool operator()(WorldObject* target)
        {
            if (!_caster || !target)
                return true;

            if (!_caster->ToTempSummon())
                return true;

            Unit* creator = _caster->ToTempSummon()->GetSummoner();

            if (!creator || creator == target)
                return true;

            uint32 entry = target->GetEntry();
            if (target->ToCreature() && (entry == NPC_KRITHIK_DEMOLISHER || entry == NPC_KRITHIK_WND_SHAPER || entry == NPC_KRITHIK_INFILTRATOR || entry == NPC_KIPTILAK))
                return true;

            return false;
        }

    private:
        Unit* _caster;
};

// Munition Explosion 120552
class spell_kiptilak_munitions_explosion : public SpellScriptLoader
{
    public:
        spell_kiptilak_munitions_explosion() : SpellScriptLoader("spell_kiptilak_munitions_explosion") { }

        class spell_kiptilak_munitions_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_kiptilak_munitions_explosion_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                if (Unit* caster = GetCaster())
                    unitList.remove_if(CheckMunitionExplosionPredicate(caster));
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_kiptilak_munitions_explosion_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_kiptilak_munitions_explosion_SpellScript();
        }
};

class spell_kiptilak_sabotage : public SpellScriptLoader
{
    public:
        spell_kiptilak_sabotage() : SpellScriptLoader("spell_kiptilak_sabotage") { }

        class spell_kiptilak_sabotage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_kiptilak_sabotage_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                if (!target)
                    return;

                target->CastSpell(target, SPELL_PLAYER_EXPLOSION, true);
                target->CastSpell(target, SPELL_SABOTAGE_EXPLOSION, true);
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_kiptilak_sabotage_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_kiptilak_sabotage_AuraScript();
        }
};

// 8359
class AreaTrigger_at_first_door : public AreaTriggerScript
{
    public:
        AreaTrigger_at_first_door() : AreaTriggerScript("at_first_door") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
            {
                if (instance->GetData(DATA_OPEN_FIRST_DOOR) != DONE)
                {
                    instance->SetData(DATA_OPEN_FIRST_DOOR, DONE);
                    if (Creature* kiptilak = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_KIPTILAK)))
                        if (kiptilak->IsAIEnabled)
                            kiptilak->AI()->DoAction(ACTION_INTRO);
                }
            }

            return false;
        }
};

class achievement_bomberman_gss : public AchievementCriteriaScript
{
    public:
        achievement_bomberman_gss() : AchievementCriteriaScript("achievement_bomberman_gss") { }

        bool OnCheck(Player* player, Unit* /*target*/) OVERRIDE
        {
            return player->GetMap()->IsHeroic();
        }
};

void AddSC_boss_saboteur_kiptilak()
{
    new boss_saboteur_kiptilak();
    new npc_munition_explosion_bunny();
    new spell_kiptilak_munitions_explosion();
    new spell_kiptilak_sabotage();
    new AreaTrigger_at_first_door();
    new achievement_bomberman_gss();
}
