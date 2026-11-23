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

#include "well_of_eternity.h"

enum ScriptTexts
{
    SAY_AGGRO              = 0,
    SAY_DEATH              = 1,
    SAY_RAGE               = 2,
    SAY_EVENT_1            = 3, // near entrance
    SAY_EVENT_2            = 4, // near entrance
    SAY_EVENT_3            = 5, // near entrance
    SAY_FOUND              = 6, // when eyes have found a player
    SAY_ACHIEVE            = 7,
    SAY_HIDING             = 8,
    SAY_INTRO              = 9,
    SAY_SPELL_1            = 10,
    SAY_SPELL_2            = 11,

    SAY_ANNOUNCE_HIDE      = 12,
    SAY_ANNOUNCE_HUNTING   = 13,
    SAY_ANNOUNCE_EASY_PREY = 14,
};

enum Spells
{
    SPELL_CORRUPTING_TOUCH          = 104939,
    SPELL_CORRUPTING_TOUCH_DMG      = 108101,
    SPELL_FEL_FLAMES                = 108141,
    SPELL_FEL_FLAMES_SUMMON         = 108193,
    SPELL_FEL_FLAMES_DMG            = 109824,
    SPELL_FEL_FLAMES_AURA           = 108214,
    SPELL_FEL_FLAMES_AURA_DMG       = 108217,
    SPELL_FEL_DECAY                 = 105544,
    SPELL_FEL_DECAY_AURA            = 108124,
    SPELL_FEL_SURGE_DMG             = 108128,
    SPELL_DRAIN_ESSENCE             = 104905,
    SPELL_FEL_ADLED                 = 105545,
    SPELL_CAMOUFLAGE                = 105341,
    SPELL_CAMOUFLAGE_REMOVE         = 105541,

    SPELL_HUNTING_CHANNEL           = 105353,

    SPELL_TRACKED_LOCK_ON_PLAYER    = 105496,
    SPELL_EASY_PREY                 = 105493,
    SPELL_ATTACK_ME_PEROTHARN       = 105509,
    SPELL_ENFEEBLED                 = 105442,
    SPELL_ENDLESS_FRENZY            = 105521,

    SPELL_PUNISHING_FLAMES          = 107532,
    SPELL_PUNISHING_FLAMES_DMG      = 107536, // 80m

    SPELL_AGGRO_ILLIDAN             = 104683,
    SPELL_END_EXHAUSTED             = 105548,

    //illidan
    SPELL_ABSORB_FEL_ENERGY         = 105543,
    SPELL_RETURN_TO_THE_SHADOWS     = 105635,
    SPELL_CONSUME_ESSENCE           = 104738,
    SPELL_REGENERATION              = 105547,
};

enum Adds
{
    NPC_FEL_FLAMES              = 57329,
    NPC_HUNTING_SUMMON_STALKER  = 56248,
    NPC_HUNTING_SUMMON_CIRCLE   = 56182,
    NPC_HUNTING_STALKERS        = 56189,
    NPC_EYE_OF_PEROTHARN_1      = 55868,
    NPC_EYE_OF_PEROTHARN_2      = 55879,
    NPC_EASY_PREY_STALKER       = 56308,
};

enum Events
{
    EVENT_ILLIDAN_AGGRO     = 1,
    EVENT_CORRUPTING_TOUCH  = 2,
    EVENT_FEL_FLAMES        = 3,
    EVENT_FEL_DECAY         = 4,
    EVENT_ILLIDAN_BREAK     = 5,
    EVENT_CAMOUFLAGE        = 6,
    EVENT_ILLIDAN_HIDE      = 7,
    EVENT_HUNTING           = 8,
    EVENT_NEXT_MOVE         = 9,
    EVENT_EASY_PREY         = 10,
    EVENT_ILLIDAN_OUTRO_1   = 11,
    EVENT_ILLIDAN_OUTRO_2   = 12,
    EVENT_END_HUNT          = 13,
    EVENT_HIDING            = 14,
    EVENT_ILLIDAN_RECOVERY  = 15,
};

enum Actions
{
    ACTION_START            = 1,
    ACTION_PREEVENT_APPEAR  = 2,
    ACTION_TELEPORT_APPEAR  = 3,
    ACTION_TELEPORT_CENTER  = 4,
    ACTION_EASY_PREY        = 5,
};

enum Points
{
    POINT_EYE       = 1,
};

const Position centerPos = {3335.07f, -4891.54f, 181.16f, 0.0f};

class boss_perotharn : public CreatureScript
{
    public:
        boss_perotharn() : CreatureScript("boss_perotharn") { }

        struct boss_perotharnAI : public BossAI
        {
            boss_perotharnAI(Creature* creature) : BossAI(creature, DATA_PEROTHARN)
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
                updatePositionTimer = 0;
                preeventDisapperTimer = 0;
                phase = 0;
                hasPreeventAppeared = false;

                me->SetReactState(REACT_PASSIVE);
                me->GetMap()->SetWorldState(WORLDSTATE_LAZY_EYE, 0);
            }

            void Reset() OVERRIDE
            {
                _Reset();

                phase = 0;
                targetGUID = 0;
                me->GetMap()->SetWorldState(WORLDSTATE_LAZY_EYE, 0);

                if (instance->GetData(DATA_EVENT_DEMON) == DONE)
                {
                    if (instance->GetData(DATA_EVENT_ILLIDAN_1) == DONE)
                    {
                        hasPreeventAppeared = true;
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        me->RemoveAura(SPELL_CAMOUFLAGE);
                        updatePositionTimer = 5000;
                    }
                    else
                    {
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        DoCast(me, SPELL_CAMOUFLAGE, true);
                    }
                }
                else
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_START)
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    DoCast(me, SPELL_CAMOUFLAGE_REMOVE, true);
                }
                else if (action == ACTION_PREEVENT_APPEAR)
                {
                    if (hasPreeventAppeared)
                        return;
                    hasPreeventAppeared = true;
                    me->SetReactState(REACT_PASSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    DoCastAOE(SPELL_CAMOUFLAGE_REMOVE);
                    Talk(SAY_INTRO);
                    preeventDisapperTimer = 7000;
                }
                else if (action == ACTION_TELEPORT_APPEAR)
                {
                    me->NearTeleportTo(perotharnPosPreeventAppear.GetPositionX(), perotharnPosPreeventAppear.GetPositionY(), perotharnPosPreeventAppear.GetPositionZ(), perotharnPosPreeventAppear.GetOrientation());
                }
                else if (action == ACTION_TELEPORT_CENTER)
                {
                    me->NearTeleportTo(perotharnPosCenter.GetPositionX(), perotharnPosCenter.GetPositionY(), perotharnPosCenter.GetPositionZ(), perotharnPosCenter.GetOrientation());
                }
                else if (action == ACTION_EASY_PREY)
                {
                    summons.DespawnEntry(NPC_EYE_OF_PEROTHARN_1);
                    summons.DespawnEntry(NPC_HUNTING_SUMMON_CIRCLE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    Talk(SAY_FOUND);
                    me->RemoveAura(SPELL_HUNTING_CHANNEL);
                    events.ScheduleEvent(EVENT_EASY_PREY, 2000);
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);

                updatePositionTimer = 0;
                events.ScheduleEvent(EVENT_ILLIDAN_AGGRO, 5000);
                events.ScheduleEvent(EVENT_CORRUPTING_TOUCH, 5000);
                events.ScheduleEvent(EVENT_FEL_FLAMES, urand(10000, 12000));
                events.ScheduleEvent(EVENT_FEL_DECAY, urand(12000, 15000));

                if (Creature* pStalker = me->SummonCreature(NPC_HUNTING_SUMMON_STALKER, centerPos))
                {
                    pStalker->SetReactState(REACT_PASSIVE);
                    pStalker->CastSpell(pStalker, SPELL_PUNISHING_FLAMES, true);
                }

                DoZoneInCombat();
                instance->SetBossState(DATA_PEROTHARN, IN_PROGRESS);

                DoCastAOE(SPELL_AGGRO_ILLIDAN);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
                if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_1, 100.0f))
                    pIllidan->AI()->DoAction(1); // ACTION_PEROTHARN_DEAD

                WoEKilledMonsterCredit(instance, QUEST_IN_UNENDING_NUMBERS, 58239);
                WoEKilledMonsterCredit(instance, QUEST_IN_UNENDING_NUMBERS, 58240);
                WoEKilledMonsterCredit(instance, QUEST_IN_UNENDING_NUMBERS, 58241);
            }

            void KilledUnit(Unit* victim) OVERRIDE { }

            void SetGUID(uint64 guid, int32 /*type*/) OVERRIDE
            {
                targetGUID = guid;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                // Hack. Clients may not be notified about his position change if they are on a loading screen.
                // So we repeat it multiple times before the encounter starts.
                // Should happen only if players entered a newly created instance map while having a lockout with Illidan event done.
                if (updatePositionTimer)
                {
                    if (updatePositionTimer <= diff)
                    {
                        updatePositionTimer = 5000;
                        me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                    }
                    else updatePositionTimer -= diff;
                }

                if (preeventDisapperTimer)
                {
                    if (preeventDisapperTimer <= diff)
                    {
                        preeventDisapperTimer = 0;
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        DoCastAOE(SPELL_CAMOUFLAGE);
                    }
                    else preeventDisapperTimer -= diff;
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) && phase != 3)
                    return;

                if (me->HealthBelowPct(60) && phase == 0)
                {
                    phase = 1;
                    me->SetReactState(REACT_PASSIVE);
                    events.CancelEvent(EVENT_FEL_FLAMES);
                    events.CancelEvent(EVENT_FEL_DECAY);
                    events.CancelEvent(EVENT_CORRUPTING_TOUCH);
                    me->AttackStop();
                    me->RemoveAllAuras();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    Talk(SAY_SPELL_1);
                    DoCastAOE(SPELL_DRAIN_ESSENCE);
                    events.ScheduleEvent(EVENT_ILLIDAN_BREAK, 4000);
                    return;
                }
                if (me->HealthBelowPct(20) && !me->HasAura(SPELL_ENDLESS_FRENZY))
                    DoCastAOE(SPELL_ENDLESS_FRENZY);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ILLIDAN_AGGRO:
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_1, 100.0f))
                                pIllidan->AI()->Talk(1); // SAY_ILLIDAN_1_BOSS_AGGRO
                            break;
                        case EVENT_CORRUPTING_TOUCH:
                            DoCastVictim(SPELL_CORRUPTING_TOUCH_DMG);
                            events.ScheduleEvent(EVENT_CORRUPTING_TOUCH, urand(13000, 17000));
                            break;
                        case EVENT_FEL_FLAMES:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_FEL_FLAMES);
                            events.ScheduleEvent(EVENT_FEL_FLAMES, urand(20000, 30000));
                            break;
                        case EVENT_FEL_DECAY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_FEL_DECAY);
                            events.ScheduleEvent(EVENT_FEL_DECAY, urand(20000, 30000));
                            break;
                        case EVENT_ILLIDAN_BREAK:
                            DoCastAOE(SPELL_FEL_ADLED, true);
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_1, 100.0f))
                            {
                                pIllidan->AI()->Talk(5); // SAY_ILLIDAN_1_BOSS_SPELL
                                pIllidan->CastSpell(pIllidan, SPELL_CONSUME_ESSENCE);
                                pIllidan->ClearUnitState(UNIT_STATE_CASTING);
                                pIllidan->CastSpell(me, SPELL_ABSORB_FEL_ENERGY);
                            }
                            events.ScheduleEvent(EVENT_CAMOUFLAGE, 6000);
                            break;
                        case EVENT_CAMOUFLAGE:
                            phase = 3;
                            Talk(SAY_SPELL_2);
                            Talk(SAY_ANNOUNCE_HIDE);
                            DoCast(me, SPELL_CAMOUFLAGE);
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_1, 100.0f))
                                pIllidan->CastSpell(pIllidan, SPELL_REGENERATION);
                            events.ScheduleEvent(EVENT_ILLIDAN_HIDE, 2000);
                            events.ScheduleEvent(EVENT_HIDING, 5000);
                            break;
                        case EVENT_ILLIDAN_HIDE:
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_1, 100.0f))
                                pIllidan->AI()->Talk(0); // SAY_ILLIDAN_1_BOSS_HIDES
                            events.ScheduleEvent(EVENT_HUNTING, 2000);
                            break;
                        case EVENT_HIDING:
                            Talk(SAY_HIDING);
                            events.ScheduleEvent(EVENT_HIDING, 15000);
                            break;
                        case EVENT_HUNTING:
                            Talk(SAY_ANNOUNCE_HUNTING);
                            events.ScheduleEvent(EVENT_END_HUNT, 45000);
                            for (float i = 0.0f; i < 2 * M_PI; i += (M_PI / 4))
                            {
                                Position pos = me->GetNearPosition(5.0f, i);
                                me->SummonCreature(NPC_HUNTING_SUMMON_CIRCLE, pos);
                                me->SummonCreature(NPC_EYE_OF_PEROTHARN_1, pos);
                            }
                            DoCast(me, SPELL_HUNTING_CHANNEL);
                            break;
                        case EVENT_EASY_PREY:
                            phase = 4;
                            events.CancelEvent(EVENT_END_HUNT);
                            events.CancelEvent(EVENT_HIDING);
                            me->SetReactState(REACT_AGGRESSIVE);
                            DoCast(me, SPELL_CAMOUFLAGE_REMOVE, true);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                            me->InterruptNonMeleeSpells(false);
                            if (targetGUID)
                                if (Player* player = ObjectAccessor::GetPlayer(*me, targetGUID))
                                {
                                    Talk(SAY_ANNOUNCE_EASY_PREY);
                                    player->CastSpell(player, SPELL_ATTACK_ME_PEROTHARN, true);
                                    DoCast(player, SPELL_TRACKED_LOCK_ON_PLAYER);
                                }
                            events.ScheduleEvent(EVENT_ILLIDAN_RECOVERY, 5000);
                            events.ScheduleEvent(EVENT_CORRUPTING_TOUCH, 5000);
                            events.ScheduleEvent(EVENT_FEL_FLAMES, urand(10000, 12000));
                            events.ScheduleEvent(EVENT_FEL_DECAY, urand(12000, 15000));
                            break;
                        case EVENT_END_HUNT:
                            events.CancelEvent(EVENT_EASY_PREY);
                            events.CancelEvent(EVENT_HIDING);
                            phase = 4;
                            summons.DespawnEntry(NPC_EYE_OF_PEROTHARN_1);
                            summons.DespawnEntry(NPC_HUNTING_SUMMON_CIRCLE);
                            me->SetReactState(REACT_AGGRESSIVE);
                            DoCast(me, SPELL_CAMOUFLAGE_REMOVE, true);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                            Talk(SAY_ACHIEVE);
                            me->GetMap()->SetWorldState(WORLDSTATE_LAZY_EYE, 1);
                            DoCast(me, SPELL_ENFEEBLED, true);
                            events.ScheduleEvent(EVENT_ILLIDAN_RECOVERY, 5000);
                            events.ScheduleEvent(EVENT_CORRUPTING_TOUCH, 20000);
                            events.ScheduleEvent(EVENT_FEL_FLAMES, urand(25000, 27000));
                            events.ScheduleEvent(EVENT_FEL_DECAY, urand(27000, 30000));
                            break;
                        case EVENT_ILLIDAN_RECOVERY:
                            DoCastAOE(SPELL_END_EXHAUSTED);
                            if (Creature* pIllidan = me->FindNearestCreature(NPC_ILLIDAN_1, 100.0f))
                                pIllidan->AI()->Talk(31); // SAY_ILLIDAN_1_BOSS_RECOVERY
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint32 updatePositionTimer;
            uint32 preeventDisapperTimer;
            uint8 phase;
            uint64 targetGUID;
            bool hasPreeventAppeared;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<boss_perotharnAI>(creature);
        }
};

class npc_perotharn_eye_of_perotharn : public CreatureScript
{
    public:
        npc_perotharn_eye_of_perotharn() : CreatureScript("npc_perotharn_eye_of_perotharn") { }

        struct npc_perotharn_eye_of_perotharnAI : public ScriptedAI
        {
            npc_perotharn_eye_of_perotharnAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                SetCombatMovement(false);
                bDespawn = false;
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                me->SetSpeed(MOVE_RUN, 0.8f, true);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_NEXT_MOVE, urand(500, 2000));
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE)
                    if (pointId == POINT_EYE)
                        events.ScheduleEvent(EVENT_NEXT_MOVE, 500);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (!bDespawn)
                {
                    if (Player* target = me->FindNearestPlayer(1.0f))
                    {
                        bDespawn = true;
                        if (Creature* pPerotharn = me->FindNearestCreature(NPC_PEROTHARN, 300.0f))
                        {
                            target->CastSpell(target, SPELL_EASY_PREY, true);
                            pPerotharn->AI()->SetGUID(target->GetGUID());
                            pPerotharn->AI()->DoAction(ACTION_EASY_PREY);
                        }
                    }
                }

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_NEXT_MOVE:
                            me->GetMotionMaster()->MovementExpired();
                            if (urand(0, 1))
                            {
                                if (Creature* pStalker = me->FindNearestCreature(NPC_HUNTING_STALKERS, 100.0f))
                                    me->GetMotionMaster()->MovePoint(POINT_EYE, pStalker->GetPositionX(), pStalker->GetPositionY(), pStalker->GetPositionZ());
                            }
                            else
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                    me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f);
                            }
                            events.ScheduleEvent(EVENT_NEXT_MOVE, urand(7000, 10000));
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            bool bDespawn;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_perotharn_eye_of_perotharnAI>(creature);
        }
};

class spell_perotharn_drain_essence : public SpellScriptLoader
{
    public:
        spell_perotharn_drain_essence() : SpellScriptLoader("spell_perotharn_drain_essence") { }

        class spell_perotharn_drain_essence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_perotharn_drain_essence_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetUnitOwner())
                    GetUnitOwner()->SetControlled(true, UNIT_STATE_STUNNED);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetUnitOwner())
                    GetUnitOwner()->SetControlled(false, UNIT_STATE_STUNNED);
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_perotharn_drain_essence_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_PACIFY_SILENCE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_perotharn_drain_essence_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_PACIFY_SILENCE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_perotharn_drain_essence_AuraScript();
        }
};

class spell_perotharn_drain_essence_dmg : public SpellScriptLoader
{
    public:
        spell_perotharn_drain_essence_dmg() : SpellScriptLoader("spell_perotharn_drain_essence_dmg") { }

        class spell_perotharn_drain_essence_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_perotharn_drain_essence_dmg_SpellScript);

            void RecalculateDamage()
            {
                SetHitDamage(int32(GetHitUnit()->CountPctFromCurHealth(10)));
            }

            void Register() OVERRIDE
            {
                OnHit += SpellHitFn(spell_perotharn_drain_essence_dmg_SpellScript::RecalculateDamage);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_perotharn_drain_essence_dmg_SpellScript();
        }
};

class spell_perotharn_punishing_flames_dmg : public SpellScriptLoader
{
    public:
        spell_perotharn_punishing_flames_dmg() : SpellScriptLoader("spell_perotharn_punishing_flames_dmg") { }

        class spell_perotharn_punishing_flames_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_perotharn_punishing_flames_dmg_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (!targets.empty())
                    targets.remove_if(DistanceCheck(GetCaster()));
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_perotharn_punishing_flames_dmg_SpellScript::FilterTargets, EFFECT_0,TARGET_UNIT_SRC_AREA_ENEMY);
            }

        private:
            class DistanceCheck
            {
                public:
                    DistanceCheck(Unit* searcher) : _searcher(searcher) { }

                    bool operator()(WorldObject* unit)
                    {
                        return (!unit->ToUnit() || _searcher->GetDistance(unit) < 80.0f);
                    }

                private:
                    Unit* _searcher;
            };
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_perotharn_punishing_flames_dmg_SpellScript();
        }
};

void AddSC_boss_perotharn()
{
    new boss_perotharn();
    new npc_perotharn_eye_of_perotharn();
    new spell_perotharn_drain_essence();
    new spell_perotharn_drain_essence_dmg();
    new spell_perotharn_punishing_flames_dmg();
}
