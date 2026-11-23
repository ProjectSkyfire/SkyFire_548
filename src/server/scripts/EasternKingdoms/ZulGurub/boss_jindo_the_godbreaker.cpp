/*
* This file is part of the Legends of Azeroth Pandaria Project. See THANKS file for Copyright information
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

enum JindoScriptTexts
{
    SAY_AGGRO                   = 0,
    SAY_PHASE_2                 = 1,
    EMOTE_SHADOWS_OF_HAKKAR     = 2,
    SAY_SHADOWS_OF_HAKKAR       = 3,
    SOUND_HAKKAR_BREAK_CHAINS   = 24245
};

enum HakkarScriptTexts
{
    SAY_PHASE   = 0,
    SAY_END_1   = 1
};

enum Spells
{
    SPELL_DRAIN_SPIRIT_ESSENCE  = 97321,
    SPELL_DEADZONE              = 97170,
    SPELL_SHADOWS_OF_HAKKAR     = 97172,
    SPELL_SHADOW_SPIKE_TARGET   = 97158,
    SPELL_SHADOW_SPIKE          = 97160,
    SPELL_HAKKAR_CHAINS         = 97022,
    SPELL_HAKKAR_CHAINS_VISUAL  = 97091,
    SPELL_SPIRIT_WORLD          = 98861,
    SPELL_SPIRIT_WORLD_AURA     = 96689,
    SPELL_SPIRIT_FORM           = 96568,
    SPELL_SPIRIT_FORM_CHAIN     = 96642,
    SPELL_SUMMON_SPIRIT_TARGET  = 97152,
    SPELL_SUMMON_SPIRIT         = 97123,
    SPELL_BODY_SLAM             = 97198,
    SPELL_SPIRIT_WARRIOR_GAZE   = 97597,
    SPELL_FRENZY                = 97088,
    SPELL_SUNDER_RIFT           = 96970,
    SPELL_SUNDER_RIFT_AURA      = 97320,
    SPELL_BRITTLE_BARRIER       = 97417,
};

enum Events
{
    EVENT_DEADZONE          = 1,
    EVENT_SHADOWS_OF_HAKKAR = 2,
    EVENT_PHASE_2           = 3,
    EVENT_SUMMON_SPIRIT     = 4,
    EVENT_BODY_SLAM         = 5,
    EVENT_SHADOW_SPIKE      = 6,
    EVENT_FRENZY            = 7,
    EVENT_END_1             = 8,
    EVENT_END_2             = 9,
    EVENT_END_3             = 10,
    EVENT_END_4             = 11,
    EVENT_SUMMON_GURUBASHI  = 12,
};

enum Adds
{
    NPC_TWISTED_SPIRIT      = 52624,
    NPC_SPIRIT_PORTAL       = 52532,
    NPC_HAKKAR_CHAINS       = 52430,
    NPC_SUNDERED_RIFT       = 52400,
    NPC_BROKEN_GROUND       = 52407,
    NPC_JINDO_THE_BROKEN    = 52154,
    NPC_SPIRIT_OF_HAKKAR    = 52222,
    NPC_GURUBASHI_SPIRIT    = 52730,
};

enum Points
{
    POINT_JINDO = 1,
};

enum Actions
{
    ACTION_TWISTED_SPIRIT,
};

Position const chainPos[3] =
{
    {-11801.4f, -1678.39f, 53.0471f, 0.0f},
    {-11761.5f, -1649.87f, 52.9657f, 0.0f},
    {-11772.7f, -1676.90f, 53.0477f, 0.0f}
};

Position const hakkarPos = {-11786.5f, -1690.92f, 53.0195f, 1.6057f};
Position const jindoPos = {-11787.84f, -1698.48f, 52.9600f, 1.539f};

Position const enterPos = {-11917.0f, -1230.14f, 92.29f, 4.78f};

class boss_jindo_the_godbreaker : public CreatureScript
{
    public:
        boss_jindo_the_godbreaker() : CreatureScript("boss_jindo_the_godbreaker") { }

        struct boss_jindo_the_godbreakerAI : public BossAI
        {
            boss_jindo_the_godbreakerAI(Creature* creature) : BossAI(creature, DATA_JINDO)
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
                me->setActive(true);
            }

            bool bTwoPhase;
            bool shadowsSaid;
            uint8 chains;

            void Reset() OVERRIDE
            {
                _Reset();
                bTwoPhase = false;
                shadowsSaid = false;
                chains = 0;
                me->SetReactState(REACT_AGGRESSIVE);
                if (instance->GetData(DATA_BOSSES) >= 2)
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->GetMap()->SetWorldState(WORLDSTATE_SPIRIT_TWISTER, 1); // by default, controlled by inner timer
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                // Two of the other bosses have to be dead before Jin'do can be fought
                if (instance->GetData(DATA_BOSSES) < 2)
                {
                    EnterEvadeMode();
                    TeleportPlayersToEntrance();
                    return;
                }

                _EnterCombat();
                Talk(SAY_AGGRO);
                bTwoPhase = false;
                chains = 0;
                events.ScheduleEvent(EVENT_DEADZONE, 12000);
                events.ScheduleEvent(EVENT_SHADOWS_OF_HAKKAR, 19000);
            }

            void TeleportPlayersToEntrance()
            {
                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->GetSource())
                        player->NearTeleportTo(enterPos.GetPositionX(), enterPos.GetPositionY(), enterPos.GetPositionZ(), enterPos.GetOrientation());
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
                if (me->IsInCombat() && (summon->GetEntry() != NPC_GURUBASHI_SPIRIT))
                    DoZoneInCombat(summon);

                if (summon->GetEntry() == NPC_GURUBASHI_SPIRIT ||
                    summon->GetEntry() == NPC_HAKKAR_CHAINS ||
                    summon->GetEntry() == NPC_SPIRIT_OF_HAKKAR)
                    summon->SetPhaseMask(2, true);
            }

            void SummonedCreatureDies(Creature* summon, Unit* killer) OVERRIDE
            {
                if (summon->GetEntry() == NPC_HAKKAR_CHAINS)
                {
                    chains++;
                    if (chains == 3)
                    {
                        events.Reset();
                        summons.DespawnEntry(NPC_TWISTED_SPIRIT);
                        summons.DespawnEntry(NPC_HAKKAR_CHAINS);
                        std::list<Creature*> creatures;
                        GetCreatureListWithEntryInGrid(creatures, me, NPC_GURUBASHI_SPIRIT, 100.0f);
                        if (!creatures.empty())
                            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                                (*iter)->DespawnOrUnsummon();

                        if (Player* realKiller = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
                            if (Creature* pHakkar = me->FindNearestCreature(NPC_SPIRIT_OF_HAKKAR, 100.0f))
                                pHakkar->PlayDirectSound(SOUND_HAKKAR_BREAK_CHAINS, realKiller);
                        events.ScheduleEvent(EVENT_END_1, 4000);
                    }
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE)
                {
                    if (pointId == POINT_JINDO)
                    {
                        DoCast(me, SPELL_SPIRIT_WORLD, true);
                        events.ScheduleEvent(EVENT_PHASE_2, 4000);
                        me->SetFacingTo(jindoPos.GetOrientation());
                        for (uint8 i = 0; i < 3; ++i)
                            me->SummonCreature(NPC_HAKKAR_CHAINS, chainPos[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);

                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HealthBelowPct(70) && !bTwoPhase)
                {
                    bTwoPhase = true;
                    me->AttackStop();
                    me->InterruptNonMeleeSpells(false);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    events.Reset();
                    Talk(SAY_PHASE_2);

                    if (Creature* pHakkar = me->SummonCreature(NPC_SPIRIT_OF_HAKKAR, hakkarPos))
                    {
                        pHakkar->CastSpell(pHakkar, SPELL_SPIRIT_FORM, true);
                        pHakkar->CastSpell(pHakkar, SPELL_HAKKAR_CHAINS_VISUAL, true);
                    }

                    me->GetMotionMaster()->MovePoint(POINT_JINDO, jindoPos);
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEADZONE:
                            DoCast(me, SPELL_DEADZONE);
                            events.ScheduleEvent(EVENT_DEADZONE, 19000);
                            break;
                        case EVENT_SHADOWS_OF_HAKKAR:
                            Talk(EMOTE_SHADOWS_OF_HAKKAR);
                            if (!shadowsSaid)
                            {
                                Talk(SAY_SHADOWS_OF_HAKKAR);
                                shadowsSaid = true;
                            }
                            DoCast(me, SPELL_SHADOWS_OF_HAKKAR);
                            events.ScheduleEvent(EVENT_SHADOWS_OF_HAKKAR, 18000);
                            break;
                        case EVENT_PHASE_2:
                            if (Creature* pHakkar = me->FindNearestCreature(NPC_SPIRIT_OF_HAKKAR, 100.0f))
                            {
                                pHakkar->AI()->Talk(SAY_PHASE);
                                                for (SummonList::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
                                    if (Creature* pSummon = Unit::GetCreature(*me, (*itr)))
                                        if (pSummon->GetEntry() == NPC_HAKKAR_CHAINS)
                                            pSummon->CastSpell(pHakkar, SPELL_HAKKAR_CHAINS);
                            }
                            me->SetCanFly(true);
                            me->GetMotionMaster()->MoveJump(jindoPos.GetPositionX(), jindoPos.GetPositionY(), jindoPos.GetPositionZ() + 15.0f, 20.0f, 40.0f);
                            events.ScheduleEvent(EVENT_SHADOW_SPIKE, 3000);
                            events.ScheduleEvent(EVENT_SUMMON_SPIRIT, 4000);
                            break;
                        case EVENT_SHADOW_SPIKE:
                            DoCast(me, SPELL_SHADOW_SPIKE_TARGET);
                            events.ScheduleEvent(EVENT_SHADOW_SPIKE, 5000);
                            break;
                        case EVENT_SUMMON_SPIRIT:
                            DoCast(me, SPELL_SUMMON_SPIRIT_TARGET);
                            events.ScheduleEvent(EVENT_SUMMON_SPIRIT, 6000);
                            break;
                        case EVENT_END_1:
                        {
                            if (Creature* pHakkar = me->FindNearestCreature(NPC_SPIRIT_OF_HAKKAR, 100.0f))
                                pHakkar->AI()->Talk(SAY_END_1);
                            events.ScheduleEvent(EVENT_END_2, 20000);
                            break;
                        }
                        case EVENT_END_2:
                            me->SetCanFly(false);
                            me->GetMotionMaster()->MoveJump(jindoPos.GetPositionX(), jindoPos.GetPositionY(), jindoPos.GetPositionZ(), 20.0f, 40.0f);
                            events.ScheduleEvent(EVENT_END_3, 5000);
                            break;
                        case EVENT_END_3:
                            events.ScheduleEvent(EVENT_END_4, 8000);
                            break;
                        case EVENT_END_4:
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                target->Kill(me);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<boss_jindo_the_godbreakerAI>(creature);
        }
};

class npc_jindo_gurubashi_spirit : public CreatureScript
{
    public:
        npc_jindo_gurubashi_spirit() : CreatureScript("npc_jindo_gurubashi_spirit") { }

        struct npc_jindo_gurubashi_spiritAI : public ScriptedAI
        {
            npc_jindo_gurubashi_spiritAI(Creature* creature) : ScriptedAI(creature)
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
            }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_BODY_SLAM, 12000);
                events.ScheduleEvent(EVENT_FRENZY, 7000);
                DoZoneInCombat(me, 100.0f);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (Creature* pJindo = me->FindNearestCreature(NPC_JINDO, 100.0f))
                    pJindo->AI()->JustSummoned(summon);
            }

            void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
            {
                if (pointId == EVENT_JUMP)
                    if (Creature* pChain = me->FindNearestCreature(NPC_HAKKAR_CHAINS, 5.0f))
                        pChain->RemoveAurasDueToSpell(SPELL_BRITTLE_BARRIER);
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
                        case EVENT_FRENZY:
                            DoCast(me, SPELL_FRENZY);
                            events.ScheduleEvent(EVENT_FRENZY, 10000);
                            break;
                        case EVENT_BODY_SLAM:
                            DoCast(me, SPELL_SPIRIT_WARRIOR_GAZE);
                            events.ScheduleEvent(EVENT_BODY_SLAM, 25000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_jindo_gurubashi_spiritAI>(creature);
        }
};

class npc_jindo_spirit_of_hakkar : public CreatureScript
{
    public:
        npc_jindo_spirit_of_hakkar() : CreatureScript("npc_jindo_spirit_of_hakkar") { }

        struct npc_jindo_spirit_of_hakkarAI : public ScriptedAI
        {
            npc_jindo_spirit_of_hakkarAI(Creature* creature) : ScriptedAI(creature)
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
                me->SetReactState(REACT_PASSIVE);
                SetCombatMovement(false);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_jindo_spirit_of_hakkarAI>(creature);
        }
};

class npc_jindo_chains_of_hakkar : public CreatureScript
{
    public:
        npc_jindo_chains_of_hakkar() : CreatureScript("npc_jindo_chains_of_hakkar") { }

        struct npc_jindo_chains_of_hakkarAI : public ScriptedAI
        {
            npc_jindo_chains_of_hakkarAI(Creature* creature) : ScriptedAI(creature)
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
                me->SetReactState(REACT_PASSIVE);
                SetCombatMovement(false);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                DoZoneInCombat(me, 300.0f);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_jindo_chains_of_hakkarAI>(creature);
        }
};

class npc_jindo_spirit_portal : public CreatureScript
{
    public:
        npc_jindo_spirit_portal() : CreatureScript("npc_jindo_spirit_portal") { }

        struct npc_jindo_spirit_portalAI : public ScriptedAI
        {
            npc_jindo_spirit_portalAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (Creature* pJindo = me->FindNearestCreature(NPC_JINDO, 300.0f))
                    pJindo->AI()->JustSummoned(summon);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_jindo_spirit_portalAI>(creature);
        }
};

class spell_jindo_shadow_spike_target : public SpellScriptLoader
{
    public:
        spell_jindo_shadow_spike_target() : SpellScriptLoader("spell_jindo_shadow_spike_target") { }

        class spell_jindo_shadow_spike_target_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jindo_shadow_spike_target_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHADOW_SPIKE, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_jindo_shadow_spike_target_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_jindo_shadow_spike_target_SpellScript();
        }
};

class SpiritPortalCheck
{
    public:
        SpiritPortalCheck(uint32 entry)
        {
            i_entry = entry;
        }

        bool operator()(WorldObject* obj) const
        {
            if (!obj->ToCreature())
                return true;

            return ((obj->ToCreature()->HasUnitState(UNIT_STATE_CASTING)) || (obj->ToCreature()->GetEntry() != i_entry) || !obj->ToCreature()->IsAlive());
        }
        uint32 i_entry;
};

class spell_jindo_summon_spirit_target : public SpellScriptLoader
{
    public:
        spell_jindo_summon_spirit_target() : SpellScriptLoader("spell_jindo_summon_spirit_target") { }

        class spell_jindo_summon_spirit_target_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jindo_summon_spirit_target_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(SpiritPortalCheck(NPC_SPIRIT_PORTAL));
                if (targets.size() > 1)
                    Skyfire::Containers::RandomResizeList(targets, 1);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetHitUnit(), SPELL_SUMMON_SPIRIT);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_jindo_summon_spirit_target_SpellScript::FilterTargets, EFFECT_0,TARGET_UNIT_SRC_AREA_ENTRY);
                OnEffectHitTarget += SpellEffectFn(spell_jindo_summon_spirit_target_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_jindo_summon_spirit_target_SpellScript();
        }
};

class spell_jindo_spirit_warrior_gaze_target : public SpellScriptLoader
{
    public:
        spell_jindo_spirit_warrior_gaze_target() : SpellScriptLoader("spell_jindo_spirit_warrior_gaze_target") { }

        class spell_jindo_spirit_warrior_gaze_target_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jindo_spirit_warrior_gaze_target_SpellScript);

            void HandleApplyAura(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_BODY_SLAM);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_jindo_spirit_warrior_gaze_target_SpellScript::HandleApplyAura, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_jindo_spirit_warrior_gaze_target_SpellScript();
        }
};

void AddSC_boss_jindo_the_godbreaker()
{
    new boss_jindo_the_godbreaker();
    new npc_jindo_gurubashi_spirit();
    new npc_jindo_spirit_of_hakkar();
    new npc_jindo_chains_of_hakkar();
    new npc_jindo_spirit_portal();

    new spell_jindo_shadow_spike_target();
    new spell_jindo_summon_spirit_target();
    new spell_jindo_spirit_warrior_gaze_target();
}
