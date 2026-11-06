/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"

#include <list>
#include <vector>

#define ENTRANCE_ORIENTATION 4.723f

enum eProtectorsSpells
{
    SPELL_SHA_CORRUPTION                = 117052,
    SPELL_SHA_MASK                      = 118221,
    SPELL_PROTECTORS_CREDIT             = 123900,
    SPELL_PROTECTORS_BERSERK            = 144369,

    SPELL_TOUCH_OF_SHA                  = 117519,
    SPELL_DEFILED_GROUND_SUMMON         = 117986,
    SPELL_DEFILED_GROUND_VISUAL         = 117989,
    SPELL_DEFILED_GROUND_STACKS         = 118091,
    SPELL_EXPEL_CORRUPTION_SUMMON       = 117975,
    SPELL_EXPEL_CORRUPTION_VISUAL       = 117943,

    SPELL_LIGHTNING_BOLT                = 117187,
    SPELL_LIGHTNING_PRISON              = 111850,
    SPELL_LIGHTNING_PRISON_STUN         = 117436,
    SPELL_LIGHTNING_STORM               = 118077,
    SPELL_LIGHTNING_STORM_FIRST         = 118064,
    SPELL_LIGHTNING_STORM_SECOND        = 118040,
    SPELL_LIGHTNING_STORM_THIRD         = 118053,
    SPELL_LIGHTNING_STORM_FOURTH        = 118054,
    SPELL_LIGHTNING_STORM_FIFTH         = 118055,
    SPELL_LIGHTNING_STORM_FIRST_DMG     = 118003,
    SPELL_LIGHTNING_STORM_SECOND_DMG    = 118004,
    SPELL_LIGHTNING_STORM_THIRD_DMG     = 118005,
    SPELL_LIGHTNING_STORM_FOURTH_DMG    = 118007,
    SPELL_LIGHTNING_STORM_FIFTH_DMG     = 118008,
    SPELL_OVERWHELMING_CORRUPTION       = 117351,
    SPELL_OVERWHELMING_CORRUPTION_STACK = 117353,

    SPELL_WATER_BOLT                    = 118312,
    SPELL_CLEANSING_WATERS_SUMMON       = 117309,
    SPELL_CLEANSING_WATERS_VISUAL       = 117250,
    SPELL_CLEANSING_WATERS_REGEN        = 117283,
    SPELL_CORRUPTING_WATERS_SUMMON      = 117227,
    SPELL_CORRUPTING_WATERS_AURA        = 117217,
    SPELL_PURIFIED                      = 117235,

    SPELL_CORRUPTED_ESSENCE             = 118191,
    SPELL_ESSENCE_OF_FEAR               = 118198,
    SPELL_SUPERIOR_CORRUPTED_ESSENCE    = 117905,
    SPELL_SUPERIOR_ESSENCE_OF_FEAR      = 118186
};

enum eProtectorsActions
{
    ACTION_FIRST_PROTECTOR_DIED    = 0,
    ACTION_SECOND_PROTECTOR_DIED   = 1,
    ACTION_DESPAWN_SUMMONS         = 3,
    ACTION_INIT_MINION_CONTROLLER  = 4,
    ACTION_RESET_MINION_CONTROLLER = 5
};

enum eProtectorsEvents
{
    EVENT_TOUCH_OF_SHA             = 1,
    EVENT_DEFILED_GROUND           = 2,
    EVENT_EXPEL_CORRUPTION         = 3,

    EVENT_LIGHTNING_BOLT           = 4,
    EVENT_LIGHTNING_PRISON         = 5,
    EVENT_LIGHTNING_STORM          = 6,
    EVENT_OVERWHELMING_CORRUPTION  = 7,

    EVENT_WATER_BOLT               = 8,
    EVENT_CLEANSING_WATERS         = 9,
    EVENT_CORRUPTING_WATERS        = 10,

    EVENT_REFRESH_CLEANSING_WATERS = 11,
    EVENT_DESPAWN_CLEANSING_WATERS = 12,
    EVENT_SPAWN_MINION_OF_FEAR     = 13,
    EVENT_ESCORT                   = 14,

    EVENT_BERSERK                  = 15,
    EVENT_NOT_ALLOW_MOVE           = 16
};

enum eProtectorsSays
{
    TALK_INTRO,
    TALK_ASANI_AGGRO,
    TALK_REGAIL_AGGRO,
    TALK_KAOLAN_DIES_FIRST_ASANI,
    TALK_KAOLAN_DIES_FIRST_REGAIL,
    TALK_ASANI_DIES_FIRST_KAOLAN,
    TALK_ASANI_DIES_FIRST_REGAIL,
    TALK_CORRUPTED_WATERS,
    TALK_LIGHTNING_STORM,
    TALK_EXPEL_CORRUPTION,
    TALK_REGAIL_DIES_SECOND_ASANI,
    TALK_REGAIL_DIES_SECOND_KAOLAN,
    TALK_ASANI_DIES_SECOND_REGAIL,
    TALK_ASANI_DIES_SECOND_KAOLAN,
    TALK_ASANI_SLAY,
    TALK_REGAIL_SLAY,
    TALK_KAOLAN_SLAY,
    TALK_ASANI_DEATH,
    TALK_REGAIL_DEATH,
    TALK_KAOLAN_DEATH
};

enum eProtectorsEquipId
{
    ASANI_MH_ITEM  = 79832,
    KAOLAN_MH_ITEM = 81390,
    REGAIL_ITEMS   = 81389
};

enum Misc
{
    PROTECTORS_OF_THE_ENDLESS_LOOT_ID       = 60585,
    PROTECTORS_OF_THE_ENDLESS_ELITE_LOOT_ID = 60583,

    WORLD_STATE_KOALAN_DEFEATED_LAST = 12096,
    WORLD_STATE_REGAIL_DEFEATED_LAST = 12097,
    WORLD_STATE_ASANI_DEFEATED_LAST  = 12098
};

const std::vector<uint32> protectorsEntry =
{
    NPC_PROTECTOR_KAOLAN,
    NPC_ANCIENT_ASANI,
    NPC_ANCIENT_REGAIL
};

bool ProtectorsInCombat(Unit* me, uint32 entry, uint64 casterGUID)
{
    for (uint32 itr : protectorsEntry)
    {
        if (itr == entry)
            continue;

        if (Unit* caster = ObjectAccessor::GetUnit(*me, casterGUID))
        {
            InstanceScript* instance = caster->GetInstanceScript();
            if (Creature* protector = ObjectAccessor::GetCreature(*caster, instance ? instance->GetData64(itr) : 0))
            {
                if (!protector->IsInCombat() && protector->IsAlive())
                    return false;
            }
        }
    }

    return true;
}

void RespawnProtectors(InstanceScript* instance, Creature* me)
{
    if (!instance || !me)
        return;

    if (Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI)))
    {
        asani->Respawn();
        asani->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL)))
    {
        regail->Respawn();
        regail->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN)))
    {
        kaolan->Respawn();
        kaolan->GetMotionMaster()->MoveTargetedHome();
    }
}

void StartProtectors(InstanceScript* instance, Creature* /*me*/, Unit* /*target*/)
{
    if (!instance)
        return;

    if (instance->GetBossState(DATA_PROTECTORS) == IN_PROGRESS)
        return;

    instance->SetBossState(DATA_PROTECTORS, IN_PROGRESS);

    if (Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI)))
        asani->SetInCombatWithZone();

    if (Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL)))
        regail->SetInCombatWithZone();

    if (Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN)))
        kaolan->SetInCombatWithZone();
}

bool IntroDone(InstanceScript* instance, Creature* me)
{
    if (!instance || !me)
        return false;

    if (instance->GetData(INTRO_DONE) == DONE)
        return true;

    std::list<Creature*> fear;
    me->GetCreatureListWithEntryInGrid(fear, NPC_APPARITION_OF_FEAR, 100.0f);
    std::list<Creature*> terror;
    me->GetCreatureListWithEntryInGrid(fear, NPC_APPARITION_OF_TERROR, 100.0f);

    for (Creature* creature : fear)
    {
        if (creature->IsAlive())
            return false;
    }

    for (Creature* creature : terror)
    {
        if (creature->IsAlive())
            return false;
    }

    return true;
}

struct boss_protector_base : public BossAI
{
    boss_protector_base(Creature* creature, uint32 bossId) : BossAI(creature, bossId) { }

    void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
    {
        if (me->GetHealth() <= damage)
        {
            if (AliveProtectorsNumber() == 1)
            {
                uint32 worldState = 0;
                switch (me->GetEntry())
                {
                    case NPC_PROTECTOR_KAOLAN: worldState = WORLD_STATE_KOALAN_DEFEATED_LAST; break;
                    case NPC_ANCIENT_REGAIL:   worldState = WORLD_STATE_REGAIL_DEFEATED_LAST; break;
                    case NPC_ANCIENT_ASANI:    worldState = WORLD_STATE_ASANI_DEFEATED_LAST; break;
                    default: break;
                }

                if (worldState)
                    me->GetMap()->SetWorldState(worldState, 1);
            }
        }
    }

    uint8 AliveProtectorsNumber()
    {
        uint8 count = 0;
        if (!instance)
            return count;

        Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI));
        if (asani && asani->IsAlive())
            ++count;

        Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL));
        if (regail && regail->IsAlive())
            ++count;

        Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN));
        if (kaolan && kaolan->IsAlive())
            ++count;

        return count;
    }

    void FinishEncounter(Unit* killer)
    {
        instance->SetBossState(DATA_PROTECTORS, DONE);

        // The credit spell is not in the 5.4.8 client; its criteria name it as their spell target.
        instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_PROTECTORS_CREDIT);

        if (Player* player = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            me->GetMap()->ToInstanceMap()->PermBindAllPlayers(player);
            // LoA personal loot and currency rewards are not available in SkyFire.
        }

        instance->DoRemoveAurasDueToSpellOnPlayers(95809); // Insanity
        instance->DoRemoveAurasDueToSpellOnPlayers(80354); // Temporal Displacement
        instance->DoRemoveAurasDueToSpellOnPlayers(57723); // Exhaustion
        instance->DoRemoveAurasDueToSpellOnPlayers(57724); // Sated
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);
        _JustDied();

        if (Creature* minionController = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
            minionController->AI()->DoAction(ACTION_RESET_MINION_CONTROLLER);
    }
};

class boss_ancient_regail : public CreatureScript
{
    public:
        boss_ancient_regail() : CreatureScript("boss_ancient_regail") { }

        struct boss_ancient_regailAI : public boss_protector_base
        {
            boss_ancient_regailAI(Creature* creature) : boss_protector_base(creature, DATA_PROTECTORS) { }

            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
            bool combatDelayDone;
            uint32 delay;

            void Reset() OVERRIDE
            {
                SetEquipmentSlots(false, REGAIL_ITEMS, REGAIL_ITEMS, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);

                _Reset();

                events.Reset();
                summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;
                combatDelayDone = false;
                delay = 0;

                me->RemoveAura(SPELL_SHA_CORRUPTION);
                me->RemoveAura(SPELL_OVERWHELMING_CORRUPTION);
                me->RemoveAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);
                    instance->DoRemoveAurasDueToSpellOnPlayers(95809); // Insanity
                    instance->DoRemoveAurasDueToSpellOnPlayers(80354); // Temporal Displacement
                    instance->DoRemoveAurasDueToSpellOnPlayers(57723); // Exhaustion
                    instance->DoRemoveAurasDueToSpellOnPlayers(57724); // Sated

                    if (Creature* minionController = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
                        minionController->AI()->DoAction(ACTION_RESET_MINION_CONTROLLER);

                    RespawnProtectors(instance, me);
                }
            }

            void SpellInterrupted(uint32 /*spellId*/, uint32 /*unTimeMs*/) OVERRIDE
            {
                events.RescheduleEvent(EVENT_LIGHTNING_BOLT, 6 * IN_MILLISECONDS);

                if (Unit* victim = me->GetVictim())
                    me->GetMotionMaster()->MoveChase(victim);
            }

            void JustReachedHome() OVERRIDE
            {
                _JustReachedHome();

                if (instance)
                    instance->SetBossState(DATA_PROTECTORS, FAIL);
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (instance)
                {
                    StartProtectors(instance, me, who);
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                    Talk(TALK_REGAIL_AGGRO);

                    if (IsHeroic())
                        if (Creature* minionController = ObjectAccessor::GetCreature(*me, instance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
                            minionController->AI()->DoAction(ACTION_INIT_MINION_CONTROLLER);

                    events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 2000);
                    events.ScheduleEvent(EVENT_LIGHTNING_PRISON, 15000);
                    events.ScheduleEvent(EVENT_BERSERK, 8 * MINUTE * IN_MILLISECONDS + 10 * IN_MILLISECONDS);
                }

                delay = 0;
                me->m_Events.Schedule(delay += 3000, 20, [this]()
                {
                    combatDelayDone = true;
                });
            }

            void AttackStart(Unit* target) OVERRIDE
            {
                if (!target)
                    return;

                if (me->Attack(target, false))
                    DoStartNoMovement(target);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                summons.Despawn(summon);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(TALK_REGAIL_SLAY);
            }

            void JustDied(Unit* killer) OVERRIDE
            {
                Talk(TALK_REGAIL_DEATH);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI));
                    Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN));

                    switch (AliveProtectorsNumber())
                    {
                        case 2:
                        {
                            if (asani && asani->IsAlive())
                            {
                                asani->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                me->CastSpell(asani, SPELL_SHA_CORRUPTION, true);
                            }

                            if (kaolan && kaolan->IsAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                me->CastSpell(kaolan, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 1:
                        {
                            if (asani && asani->IsAlive())
                            {
                                asani->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                asani->AI()->Talk(TALK_REGAIL_DIES_SECOND_ASANI);

                                if (Aura* shaCorruption = asani->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(asani, SPELL_SHA_CORRUPTION, true);
                            }

                            if (kaolan && kaolan->IsAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                kaolan->AI()->Talk(TALK_REGAIL_DIES_SECOND_KAOLAN);

                                if (Aura* shaCorruption = kaolan->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(kaolan, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            FinishEncounter(killer);

                            if (kaolan)
                                kaolan->AI()->DoAction(ACTION_DESPAWN_SUMMONS);
                            if (asani)
                                asani->AI()->DoAction(ACTION_DESPAWN_SUMMONS);

                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_FIRST_PROTECTOR_DIED:
                        firstSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_LIGHTNING_STORM, 20 * IN_MILLISECONDS);
                        me->SetFullHealth();
                        break;
                    case ACTION_SECOND_PROTECTOR_DIED:
                        secondSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_OVERWHELMING_CORRUPTION, 5000);
                        me->SetFullHealth();
                        break;
                    case ACTION_INTRO_FINISHED:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_LOOTING);
                        me->SetFacingTo(ENTRANCE_ORIENTATION);
                        me->SetReactState(REACT_AGGRESSIVE);

                        if (GameObject* vortex = instance->instance->GetGameObject(instance->GetData64(GO_COUNCILS_VORTEX)))
                            vortex->SetGoState(GOState::GO_STATE_ACTIVE);
                        if (GameObject* wall = instance->instance->GetGameObject(instance->GetData64(GO_WALL_OF_COUNCILS_VORTEX)))
                            wall->SetGoState(GOState::GO_STATE_ACTIVE);

                        break;
                    case ACTION_DESPAWN_SUMMONS:
                        summons.DespawnAll();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!IntroDone(instance, me))
                {
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_LOOTING);

                    if (GameObject* vortex = instance->instance->GetGameObject(instance->GetData64(GO_COUNCILS_VORTEX)))
                    {
                        me->SetFacingToObject(vortex);
                        vortex->SetGoState(GOState::GO_STATE_READY);
                    }

                    if (GameObject* wall = instance->instance->GetGameObject(instance->GetData64(GO_WALL_OF_COUNCILS_VORTEX)))
                        wall->SetGoState(GOState::GO_STATE_READY);
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                // Misdirection exploit
                if (combatDelayDone && !ProtectorsInCombat(me, me->GetEntry(), me->GetGUID()))
                    EnterEvadeMode();

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LIGHTNING_BOLT:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            me->StopMoving();

                            if (Unit* victim = me->GetVictim())
                            {
                                if (me->GetExactDist2d(victim) > 39.0f)
                                    DoStartMovement(victim);
                                else
                                    DoStartNoMovement(victim);
                            }

                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                DoCast(target, SPELL_LIGHTNING_BOLT);
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 39.0f, true))
                                DoCast(target, SPELL_LIGHTNING_BOLT);

                            events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 1000);
                            break;
                        case EVENT_LIGHTNING_PRISON:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            DoCast(me, SPELL_LIGHTNING_PRISON);
                            events.ScheduleEvent(EVENT_LIGHTNING_PRISON, 25000);
                            break;
                        case EVENT_LIGHTNING_STORM:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            if (!firstSpecialEnabled)
                                break;

                            Talk(TALK_LIGHTNING_STORM);
                            me->CastSpell(me, SPELL_LIGHTNING_STORM, false);

                            if (!secondSpecialEnabled)
                                events.ScheduleEvent(EVENT_LIGHTNING_STORM, 40 * IN_MILLISECONDS);
                            else
                                events.ScheduleEvent(EVENT_LIGHTNING_STORM, 32000);
                            break;
                        case EVENT_OVERWHELMING_CORRUPTION:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            me->CastSpell(me, SPELL_OVERWHELMING_CORRUPTION, true);
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK, true);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            private:
                bool HandleRescheduleEventsIfCastAny(uint32 eventId)
                {
                    if (me->HasUnitState(UNIT_STATE_CASTING))
                    {
                        events.RescheduleEvent(eventId, urand(1 * IN_MILLISECONDS, 2 * IN_MILLISECONDS));
                        return true;
                    }

                    return false;
                }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_ancient_regailAI(creature);
        }
};

class boss_ancient_asani : public CreatureScript
{
    public:
        boss_ancient_asani() : CreatureScript("boss_ancient_asani") { }

        struct boss_ancient_asaniAI : public boss_protector_base
        {
            boss_ancient_asaniAI(Creature* creature) : boss_protector_base(creature, DATA_PROTECTORS) { }

            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
            bool combatDelayDone;
            uint32 delay;

            void Reset() OVERRIDE
            {
                SetEquipmentSlots(false, ASANI_MH_ITEM, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);

                _Reset();

                events.Reset();
                summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;
                combatDelayDone = false;
                delay = 0;

                me->RemoveAura(SPELL_SHA_CORRUPTION);
                me->RemoveAura(SPELL_OVERWHELMING_CORRUPTION);
                me->RemoveAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);

                    RespawnProtectors(instance, me);
                }
            }

            void JustReachedHome() OVERRIDE
            {
                _JustReachedHome();

                if (instance)
                    instance->SetBossState(DATA_PROTECTORS, FAIL);
            }

            void SpellInterrupted(uint32 /*spellId*/, uint32 /*unTimeMs*/) OVERRIDE
            {
                events.RescheduleEvent(EVENT_WATER_BOLT, 6 * IN_MILLISECONDS);

                if (Unit* victim = me->GetVictim())
                    me->GetMotionMaster()->MoveChase(victim);
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (instance)
                {
                    StartProtectors(instance, me, who);
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                    Talk(TALK_ASANI_AGGRO);
                }

                events.ScheduleEvent(EVENT_WATER_BOLT, 2000);
                events.ScheduleEvent(EVENT_CLEANSING_WATERS, 10 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_BERSERK, 8 * MINUTE * IN_MILLISECONDS + 10 * IN_MILLISECONDS);

                delay = 0;
                me->m_Events.Schedule(delay += 3000, 20, [this]()
                {
                    combatDelayDone = true;
                });
            }

            void AttackStart(Unit* target) OVERRIDE
            {
                if (!target)
                    return;

                if (me->Attack(target, false))
                    DoStartNoMovement(target);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                summons.Despawn(summon);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(TALK_ASANI_SLAY);
            }

            void JustDied(Unit* killer) OVERRIDE
            {
                Talk(TALK_ASANI_DEATH);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL));
                    Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN));

                    switch (AliveProtectorsNumber())
                    {
                        case 2:
                        {
                            if (regail && regail->IsAlive())
                            {
                                regail->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                regail->AI()->Talk(TALK_ASANI_DIES_FIRST_REGAIL);
                                me->CastSpell(regail, SPELL_SHA_CORRUPTION, true);
                            }

                            if (kaolan && kaolan->IsAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                kaolan->AI()->Talk(TALK_ASANI_DIES_FIRST_KAOLAN);
                                me->CastSpell(kaolan, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 1:
                        {
                            if (regail && regail->IsAlive())
                            {
                                regail->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                regail->AI()->Talk(TALK_ASANI_DIES_SECOND_REGAIL);

                                if (Aura* shaCorruption = regail->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(regail, SPELL_SHA_CORRUPTION, true);
                            }

                            if (kaolan && kaolan->IsAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                kaolan->AI()->Talk(TALK_ASANI_DIES_SECOND_KAOLAN);

                                if (Aura* shaCorruption = kaolan->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(kaolan, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            FinishEncounter(killer);

                            if (kaolan)
                                kaolan->AI()->DoAction(ACTION_DESPAWN_SUMMONS);
                            if (regail)
                                regail->AI()->DoAction(ACTION_DESPAWN_SUMMONS);

                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_FIRST_PROTECTOR_DIED:
                        firstSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_CORRUPTING_WATERS, 10000);
                        me->SetFullHealth();
                        break;
                    case ACTION_SECOND_PROTECTOR_DIED:
                        secondSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_OVERWHELMING_CORRUPTION, 5000);
                        me->SetFullHealth();
                        break;
                    case ACTION_INTRO_FINISHED:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_LOOTING);
                        me->SetFacingTo(ENTRANCE_ORIENTATION);
                        me->SetReactState(REACT_AGGRESSIVE);

                        if (GameObject* vortex = instance->instance->GetGameObject(instance->GetData64(GO_COUNCILS_VORTEX)))
                            vortex->SetGoState(GOState::GO_STATE_ACTIVE);
                        if (GameObject* wall = instance->instance->GetGameObject(instance->GetData64(GO_WALL_OF_COUNCILS_VORTEX)))
                            wall->SetGoState(GOState::GO_STATE_ACTIVE);

                        break;
                    case ACTION_DESPAWN_SUMMONS:
                        summons.DespawnAll();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!IntroDone(instance, me))
                {
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_LOOTING);

                    if (GameObject* vortex = instance->instance->GetGameObject(instance->GetData64(GO_COUNCILS_VORTEX)))
                    {
                        me->SetFacingToObject(vortex);
                        vortex->SetGoState(GOState::GO_STATE_READY);
                    }

                    if (GameObject* wall = instance->instance->GetGameObject(instance->GetData64(GO_WALL_OF_COUNCILS_VORTEX)))
                        wall->SetGoState(GOState::GO_STATE_READY);
                }

                if (!UpdateVictim())
                    return;

                // Misdirection exploit
                if (combatDelayDone && !ProtectorsInCombat(me, me->GetEntry(), me->GetGUID()))
                    EnterEvadeMode();

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WATER_BOLT:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            me->StopMoving();

                            if (Unit* victim = me->GetVictim())
                            {
                                if (me->GetExactDist2d(victim) > 59.0f)
                                    DoStartMovement(victim);
                                else
                                    DoStartNoMovement(victim);
                            }

                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                DoCast(target, SPELL_WATER_BOLT);
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 59.0f, true))
                                DoCast(target, SPELL_WATER_BOLT);

                            events.ScheduleEvent(EVENT_WATER_BOLT, 1000);
                            break;
                        case EVENT_CLEANSING_WATERS:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            if (Creature* lowestFriend = ObjectAccessor::GetCreature(*me, GetLowestFriendlyGUID()))
                                me->CastSpell(lowestFriend, SPELL_CLEANSING_WATERS_SUMMON, false);

                            events.ScheduleEvent(EVENT_CLEANSING_WATERS, 32500);
                            break;
                        case EVENT_CORRUPTING_WATERS:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            Talk(TALK_CORRUPTED_WATERS);
                            me->InterruptNonMeleeSpells(true);
                            me->CastSpell(me, SPELL_CORRUPTING_WATERS_SUMMON, false);
                            events.ScheduleEvent(EVENT_CORRUPTING_WATERS, 43500);
                            break;
                        case EVENT_OVERWHELMING_CORRUPTION:
                            if (HandleRescheduleEventsIfCastAny(eventId))
                                break;

                            me->CastSpell(me, SPELL_OVERWHELMING_CORRUPTION, true);
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK, true);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            private:
                uint64 GetLowestFriendlyGUID()
                {
                    std::list<Creature*> targets;

                    targets.push_back(me);
                    GetCreatureListWithEntryInGrid(targets, me, NPC_ANCIENT_REGAIL, 200.0f);
                    GetCreatureListWithEntryInGrid(targets, me, NPC_PROTECTOR_KAOLAN, 200.0f);
                    targets.remove_if([](Creature* target) { return !target->IsAlive(); });

                    if (targets.empty())
                        return 0;

                    targets.sort(Skyfire::HealthPctOrderPred());

                    if (Creature* lowestTarget = targets.front())
                        return lowestTarget->GetGUID();

                    return 0;
                }

                bool HandleRescheduleEventsIfCastAny(uint32 eventId)
                {
                    if (me->HasUnitState(UNIT_STATE_CASTING))
                    {
                        events.RescheduleEvent(eventId, urand(1 * IN_MILLISECONDS, 2 * IN_MILLISECONDS));
                        return true;
                    }

                    return false;
                }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_ancient_asaniAI(creature);
        }
};

class boss_protector_kaolan : public CreatureScript
{
    public:
        boss_protector_kaolan() : CreatureScript("boss_protector_kaolan") { }

        struct boss_protector_kaolanAI : public boss_protector_base
        {
            boss_protector_kaolanAI(Creature* creature) : boss_protector_base(creature, DATA_PROTECTORS) { }

            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
            bool combatDelayDone;
            uint32 delay;

            void Reset() OVERRIDE
            {
                SetEquipmentSlots(false, KAOLAN_MH_ITEM, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);

                _Reset();

                events.Reset();
                summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;
                combatDelayDone = false;
                delay = 0;

                me->RemoveAura(SPELL_SHA_CORRUPTION);
                me->RemoveAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);

                    RespawnProtectors(instance, me);
                }
            }

            void JustReachedHome() OVERRIDE
            {
                _JustReachedHome();

                if (instance)
                    instance->SetBossState(DATA_PROTECTORS, FAIL);
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (instance)
                {
                    StartProtectors(instance, me, who);
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                }

                events.ScheduleEvent(EVENT_TOUCH_OF_SHA, 35 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_BERSERK, 8 * MINUTE * IN_MILLISECONDS + 10 * IN_MILLISECONDS);

                delay = 0;
                me->m_Events.Schedule(delay += 3000, 20, [this]()
                {
                    combatDelayDone = true;
                });
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                summons.Despawn(summon);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(TALK_KAOLAN_SLAY);
            }

            void JustDied(Unit* killer) OVERRIDE
            {
                Talk(TALK_KAOLAN_DEATH);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);

                    Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL));
                    Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI));

                    switch (AliveProtectorsNumber())
                    {
                        case 2:
                        {
                            if (regail && regail->IsAlive())
                            {
                                regail->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                regail->AI()->Talk(TALK_KAOLAN_DIES_FIRST_REGAIL);
                                me->CastSpell(regail, SPELL_SHA_CORRUPTION, true);
                            }

                            if (asani && asani->IsAlive())
                            {
                                asani->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                asani->AI()->Talk(TALK_KAOLAN_DIES_FIRST_ASANI);
                                me->CastSpell(asani, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 1:
                        {
                            if (regail && regail->IsAlive())
                            {
                                regail->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);

                                if (Aura* shaCorruption = regail->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(regail, SPELL_SHA_CORRUPTION, true);
                            }

                            if (asani && asani->IsAlive())
                            {
                                asani->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);

                                if (Aura* shaCorruption = asani->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(asani, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            FinishEncounter(killer);

                            if (asani)
                                asani->AI()->DoAction(ACTION_DESPAWN_SUMMONS);
                            if (regail)
                                regail->AI()->DoAction(ACTION_DESPAWN_SUMMONS);

                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_FIRST_PROTECTOR_DIED:
                        firstSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_DEFILED_GROUND, 9 * IN_MILLISECONDS);
                        me->SetFullHealth();
                        break;
                    case ACTION_SECOND_PROTECTOR_DIED:
                        secondSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, 5 * IN_MILLISECONDS);
                        me->SetFullHealth();
                        break;
                    case ACTION_INTRO_FINISHED:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_LOOTING);
                        me->SetFacingTo(ENTRANCE_ORIENTATION);
                        me->SetReactState(REACT_AGGRESSIVE);

                        if (GameObject* vortex = instance->instance->GetGameObject(instance->GetData64(GO_COUNCILS_VORTEX)))
                            vortex->SetGoState(GOState::GO_STATE_ACTIVE);
                        if (GameObject* wall = instance->instance->GetGameObject(instance->GetData64(GO_WALL_OF_COUNCILS_VORTEX)))
                            wall->SetGoState(GOState::GO_STATE_ACTIVE);

                        break;
                    case ACTION_DESPAWN_SUMMONS:
                        summons.DespawnAll();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!IntroDone(instance, me))
                {
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_LOOTING);

                    if (GameObject* vortex = instance->instance->GetGameObject(instance->GetData64(GO_COUNCILS_VORTEX)))
                    {
                        me->SetFacingToObject(vortex);
                        vortex->SetGoState(GOState::GO_STATE_READY);
                    }

                    if (GameObject* wall = instance->instance->GetGameObject(instance->GetData64(GO_WALL_OF_COUNCILS_VORTEX)))
                        wall->SetGoState(GOState::GO_STATE_READY);
                }

                if (!UpdateVictim())
                    return;

                // Misdirection exploit
                if (combatDelayDone && !ProtectorsInCombat(me, me->GetEntry(), me->GetGUID()))
                    EnterEvadeMode();

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TOUCH_OF_SHA:
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, [this](Unit* candidate)
                            {
                                return NonTankTargetSelector(me)(candidate) &&
                                    !candidate->HasAura(SPELL_TOUCH_OF_SHA) &&
                                    !candidate->ToPlayer()->IsGameMaster();
                            });

                            if (target)
                                DoCast(target, SPELL_TOUCH_OF_SHA);
                            else if (Unit* fallback = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true, -SPELL_TOUCH_OF_SHA))
                                DoCast(fallback, SPELL_TOUCH_OF_SHA);

                            events.ScheduleEvent(EVENT_TOUCH_OF_SHA, 35 * IN_MILLISECONDS);
                            break;
                        }
                        case EVENT_DEFILED_GROUND:
                            if (!firstSpecialEnabled)
                                break;

                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                                me->CastSpell(target, SPELL_DEFILED_GROUND_SUMMON, true);

                            events.ScheduleEvent(EVENT_DEFILED_GROUND, 15 * IN_MILLISECONDS);
                            break;
                        case EVENT_EXPEL_CORRUPTION:
                            if (!secondSpecialEnabled)
                                break;

                            Talk(TALK_EXPEL_CORRUPTION);
                            me->CastSpell(me, SPELL_EXPEL_CORRUPTION_SUMMON, false);
                            events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, 38500);
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK, true);
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
            return new boss_protector_kaolanAI(creature);
        }
};

// Defiled Ground - 60906
class npc_defiled_ground : public CreatureScript
{
    public:
        npc_defiled_ground() : CreatureScript("npc_defiled_ground") { }

        struct npc_defiled_groundAI : public ScriptedAI
        {
            npc_defiled_groundAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->CastSpell(me, SPELL_DEFILED_GROUND_VISUAL, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_defiled_groundAI(creature);
        }
};

// Coalesced Corruption - 60886
class npc_coalesced_corruption : public CreatureScript
{
    public:
        npc_coalesced_corruption() : CreatureScript("npc_coalesced_corruption") { }

        struct npc_coalesced_corruptionAI : public ScriptedAI
        {
            npc_coalesced_corruptionAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->CastSpell(me, SPELL_EXPEL_CORRUPTION_VISUAL, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_coalesced_corruptionAI(creature);
        }
};

// Cleansing Water - 60646
class npc_cleansing_water : public CreatureScript
{
    public:
        npc_cleansing_water() : CreatureScript("npc_cleansing_water") { }

        struct npc_cleansing_waterAI : public ScriptedAI
        {
            npc_cleansing_waterAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                me->SetCorpseDelay(0);
                events.Reset();
                events.ScheduleEvent(EVENT_REFRESH_CLEANSING_WATERS, 1000);
                events.ScheduleEvent(EVENT_DESPAWN_CLEANSING_WATERS, 8000);

                me->CastSpell(me, SPELL_CLEANSING_WATERS_VISUAL, true);
                me->CastSpell(me, SPELL_CLEANSING_WATERS_REGEN, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_REFRESH_CLEANSING_WATERS:
                        me->CastSpell(me, SPELL_CLEANSING_WATERS_REGEN, true);
                        events.ScheduleEvent(EVENT_REFRESH_CLEANSING_WATERS, 1000);
                        break;
                    case EVENT_DESPAWN_CLEANSING_WATERS:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_cleansing_waterAI(creature);
        }
};

// Corrupting Waters - 60621
class npc_corrupting_waters : public CreatureScript
{
    public:
        npc_corrupting_waters() : CreatureScript("npc_corrupting_waters") { }

        struct npc_corrupting_watersAI : public ScriptedAI
        {
            npc_corrupting_watersAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->CastSpell(me, SPELL_CORRUPTING_WATERS_AURA, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->CastSpell(me, SPELL_PURIFIED, true);
                me->DespawnOrUnsummon(4500);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_corrupting_watersAI(creature);
        }
};

#define MOVE_POINT_PROTECTOR 9999

// Minion of Fear - 60885
class npc_minion_of_fear : public CreatureScript
{
    public:
        npc_minion_of_fear() : CreatureScript("npc_minion_of_fear") { }

        struct npc_minion_of_fearAI : public ScriptedAI
        {
            npc_minion_of_fearAI(Creature* creature) : ScriptedAI(creature) { }

            InstanceScript* instance;
            uint64 protectorTargetedGuid;
            uint32 wp;
            Creature* target;
            std::list<Creature*> targets;
            EventMap nonCombatEvents;

            void InitializeAI() OVERRIDE
            {
                instance = me->GetInstanceScript();
                protectorTargetedGuid = 0;
                wp = 0;
                target = NULL;
                me->SetReactState(REACT_PASSIVE);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                if (instance && !protectorTargetedGuid)
                {
                    if (Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI)))
                        if (asani->IsAlive())
                            targets.push_back(asani);
                    if (Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN)))
                        if (kaolan->IsAlive())
                            targets.push_back(kaolan);
                    if (Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL)))
                        if (regail->IsAlive())
                            targets.push_back(regail);

                    if (targets.empty())
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }

                    targets.sort(Skyfire::HealthPctOrderPred());
                    nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
                }

                me->AttackStop();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->CastSpell(me, SPELL_CORRUPTED_ESSENCE, true);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                ++wp;

                switch (pointId)
                {
                    case 1:
                        target = targets.front();
                        if (!target)
                        {
                            me->DespawnOrUnsummon();
                            return;
                        }

                        protectorTargetedGuid = target->GetGUID();
                        me->GetMotionMaster()->MoveFollow(target, 0.0f, me->GetAngle(target));
                        break;
                    default:
                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                if (uint32 eventId = nonCombatEvents.ExecuteEvent())
                    if (eventId == EVENT_ESCORT)
                    {
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->MovePoint(wp, MinionOfFearWaypoints[wp]);
                    }

                if (!protectorTargetedGuid)
                    return;

                if (Creature* protector = me->GetMap()->GetCreature(protectorTargetedGuid))
                {
                    if (protector->IsWithinDist(me, 2.0f, false))
                    {
                        if (Aura* superiorCorruptedEssence = protector->GetAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE))
                        {
                            superiorCorruptedEssence->ModStackAmount(1);
                            superiorCorruptedEssence->RefreshDuration();
                        }
                        else
                            me->AddAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE, protector);

                        me->DespawnOrUnsummon(50);
                        protectorTargetedGuid = 0;
                    }

                    if (target)
                        me->GetMotionMaster()->MoveFollow(target, 0.0f, me->GetAngle(target));
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_minion_of_fearAI(creature);
        }
};

// Minion of Fear Controller - 60957
class npc_minion_of_fear_controller : public CreatureScript
{
    public:
        npc_minion_of_fear_controller() : CreatureScript("npc_minion_of_fear_controller") { }

        struct npc_minion_of_fear_controllerAI : public ScriptedAI
        {
            npc_minion_of_fear_controllerAI(Creature* creature) : ScriptedAI(creature), summons(me) { }

            SummonList summons;
            EventMap events;
            bool started;

            void Reset() OVERRIDE
            {
                started = false;
                events.Reset();
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                summons.Despawn(summon);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_INIT_MINION_CONTROLLER:
                        started = true;
                        events.ScheduleEvent(EVENT_SPAWN_MINION_OF_FEAR, 15000);
                        break;
                    case ACTION_RESET_MINION_CONTROLLER:
                        started = false;
                        events.Reset();
                        summons.DespawnAll();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!started)
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_SPAWN_MINION_OF_FEAR)
                    {
                        Position pos;
                        me->GetPosition(&pos);

                        me->SummonCreature(NPC_MINION_OF_FEAR, pos);
                        events.ScheduleEvent(EVENT_SPAWN_MINION_OF_FEAR, 15000);
                    }
                    break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_minion_of_fear_controllerAI(creature);
        }
};

class spell_defiled_ground_damage : public SpellScriptLoader
{
    public:
        spell_defiled_ground_damage() : SpellScriptLoader("spell_defiled_ground_damage") { }

        class spell_defiled_ground_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_defiled_ground_damage_SpellScript);

            void DealDamage()
            {
                if (Unit* target = GetHitUnit())
                {
                    if (AuraEffect const* defiledGround = target->GetAuraEffect(SPELL_DEFILED_GROUND_STACKS, EFFECT_0))
                    {
                        uint32 damage = GetHitDamage();
                        AddPct(damage, defiledGround->GetAmount());
                        SetHitDamage(damage);
                    }
                }
            }

            void Register() OVERRIDE
            {
                OnHit += SpellHitFn(spell_defiled_ground_damage_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_defiled_ground_damage_SpellScript();
        }
};

// Expelled Corruption (triggered) - 117955
class spell_expelled_corruption : public SpellScriptLoader
{
    public:
        spell_expelled_corruption() : SpellScriptLoader("spell_expelled_corruption") { }

        class spell_expelled_corruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_expelled_corruption_SpellScript);

            void DealDamage()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                float distance = caster->GetExactDist2d(target);
                if (distance >= 0.0f && distance <= 30.0f + caster->GetFloatValue(UNIT_FIELD_BOUNDING_RADIUS))
                    SetHitDamage(GetHitDamage() * (1.0f - distance / 30.0f));
            }

            void Register() OVERRIDE
            {
                OnHit += SpellHitFn(spell_expelled_corruption_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_expelled_corruption_SpellScript();
        }
};

// Lightning Storm - 118064, 118040, 118053, 118054, 118055, 118077
class spell_lightning_storm_aura : public SpellScriptLoader
{
    public:
        spell_lightning_storm_aura() : SpellScriptLoader("spell_lightning_storm_aura") { }

        class spell_lightning_storm_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lightning_storm_aura_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetSpellInfo()->Id != SPELL_LIGHTNING_STORM)
                    return;

                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIRST, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    switch (GetSpellInfo()->Id)
                    {
                        case SPELL_LIGHTNING_STORM_FIRST:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIRST_DMG, true);
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_SECOND, true);
                            break;
                        case SPELL_LIGHTNING_STORM_SECOND:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_SECOND_DMG, true);
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_THIRD, true);
                            break;
                        case SPELL_LIGHTNING_STORM_THIRD:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_THIRD_DMG, true);
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FOURTH, true);
                            break;
                        case SPELL_LIGHTNING_STORM_FOURTH:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FOURTH_DMG, true);
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIFTH, true);
                            break;
                        case SPELL_LIGHTNING_STORM_FIFTH:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIFTH_DMG, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_lightning_storm_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_lightning_storm_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_lightning_storm_aura_AuraScript();
        }
};

// Lightning Storm (damage) - 118004, 118005, 118007, 118008
class spell_lightning_storm_damage : public SpellScriptLoader
{
    public:
        spell_lightning_storm_damage() : SpellScriptLoader("spell_lightning_storm_damage") { }

        class spell_lightning_storm_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lightning_storm_damage_SpellScript);

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                targets.clear();

                float maxDist = 0.0f;
                float minDist = 0.0f;

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_LIGHTNING_STORM_SECOND_DMG:
                        minDist = 10.0f;
                        maxDist = 22.0f;
                        break;
                    case SPELL_LIGHTNING_STORM_THIRD_DMG:
                        minDist = 30.0f;
                        maxDist = 42.0f;
                        break;
                    case SPELL_LIGHTNING_STORM_FOURTH_DMG:
                        minDist = 50.0f;
                        maxDist = 62.0f;
                        break;
                    case SPELL_LIGHTNING_STORM_FIFTH_DMG:
                        minDist = 70.0f;
                        maxDist = 82.0f;
                        break;
                    default:
                        break;
                }

                Map::PlayerList const& players = GetCaster()->GetMap()->GetPlayers();
                if (!players.isEmpty())
                {
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        if (Player* player = itr->GetSource())
                        {
                            float distance = player->GetExactDist2d(GetCaster()->GetPositionX(), GetCaster()->GetPositionY());
                            if (distance <= maxDist && distance >= minDist)
                                targets.push_back(player);
                        }
                    }
                }
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_lightning_storm_damage_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_lightning_storm_damage_SpellScript();
        }
};

// Lightning Prison - 111850
class spell_lightning_prison : public SpellScript
{
    PrepareSpellScript(spell_lightning_prison);

    void CorrectRange(std::list<WorldObject*>& targets)
    {
        if (!GetCaster())
            return;

        targets.remove_if([](WorldObject* target)
        {
            if (!target)
                return false;

            Player* player = target->ToPlayer();
            if (!player || player->IsGameMaster())
                return false;

            switch (player->GetTalentSpecialization(player->GetActiveSpec()))
            {
                case SPEC_MONK_BREWMASTER:
                case SPEC_DEATH_KNIGHT_BLOOD:
                case SPEC_WARRIOR_PROTECTION:
                case SPEC_DRUID_GUARDIAN:
                case SPEC_PALADIN_PROTECTION:
                    return true;
                default:
                    return false;
            }
        });

        uint32 maxTargets = GetCaster()->GetMap()->Is25ManRaid() ? 3 : 2;
        if (targets.size() > maxTargets)
            Skyfire::Containers::RandomResizeList(targets, maxTargets);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_lightning_prison::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class EssenceDistanceOrderPred
{
    public:
        EssenceDistanceOrderPred(WorldObject const* object, bool ascending = true)
            : m_object(object), m_ascending(ascending) { }

        bool operator()(WorldObject const* a, WorldObject const* b) const
        {
            return m_ascending ? a->GetDistance(m_object) < b->GetDistance(m_object) :
                a->GetDistance(m_object) > b->GetDistance(m_object);
        }

    private:
        WorldObject const* m_object;
        bool const m_ascending;
};

// Corrupted Essence - 118191
class spell_corrupted_essence : public SpellScript
{
    PrepareSpellScript(spell_corrupted_essence);

    void CorrectRange(std::list<WorldObject*>& targets)
    {
        if (Unit* caster = GetCaster())
        {
            uint32 maxVictims = caster->GetMap()->Is25ManRaid() ? 5 : 2;
            if (targets.empty())
                return;

            targets.sort(EssenceDistanceOrderPred(caster, true));
            if (targets.size() > maxVictims)
                targets.resize(maxVictims);
        }
    }

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
        {
            if (Aura* essence = target->GetAura(SPELL_CORRUPTED_ESSENCE))
            {
                if (essence->GetStackAmount() > 9)
                {
                    target->RemoveAurasDueToSpell(SPELL_CORRUPTED_ESSENCE);
                    target->CastSpell(target, SPELL_ESSENCE_OF_FEAR, true);
                }
            }
        }
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_corrupted_essence::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_corrupted_essence::HandleEffectHit, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

// Superior Corrupted Essence - 117905
class spell_superior_corrupted_essence : public SpellScriptLoader
{
    public:
        spell_superior_corrupted_essence() : SpellScriptLoader("spell_superior_corrupted_essence") { }

        class spell_superior_corrupted_essence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_superior_corrupted_essence_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (Aura* corruptedEssence = target->GetAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE))
                    {
                        if (corruptedEssence->GetStackAmount() >= 5)
                        {
                            target->RemoveAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE);
                            target->CastSpell(target, SPELL_SUPERIOR_ESSENCE_OF_FEAR, true);
                        }
                    }
                }
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_superior_corrupted_essence_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_superior_corrupted_essence_AuraScript();
        }
};

// Cleansing Waters - 117283
class spell_cleansing_waters_regen : public SpellScriptLoader
{
    public:
        spell_cleansing_waters_regen() : SpellScriptLoader("spell_cleansing_waters_regen") { }

        class spell_cleansing_waters_regen_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_cleansing_waters_regen_SpellScript);

            std::list<WorldObject*> m_targets;

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.remove_if([](WorldObject* target)
                {
                    Unit* unit = target->ToUnit();
                    return unit && unit->HasAura(SPELL_CLEANSING_WATERS_REGEN);
                });
                m_targets = targets;
            }

            void CopyTargets(std::list<WorldObject*>& targets)
            {
                targets = m_targets;
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_cleansing_waters_regen_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_cleansing_waters_regen_SpellScript::CopyTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_cleansing_waters_regen_SpellScript();
        }
};

// Corrupted Waters: Empowered - 117230
class spell_corrupted_waters_empowered : public SpellScript
{
    PrepareSpellScript(spell_corrupted_waters_empowered);

    void CorrectTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if([](WorldObject* target)
        {
            return target->GetEntry() != NPC_ANCIENT_ASANI &&
                target->GetEntry() != NPC_ANCIENT_REGAIL &&
                target->GetEntry() != NPC_PROTECTOR_KAOLAN;
        });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_corrupted_waters_empowered::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

void AddSC_boss_protectors_of_the_endless()
{
    new boss_ancient_regail();
    new boss_ancient_asani();
    new boss_protector_kaolan();
    new npc_defiled_ground();
    new npc_coalesced_corruption();
    new npc_cleansing_water();
    new npc_corrupting_waters();
    new npc_minion_of_fear();
    new npc_minion_of_fear_controller();

    new spell_defiled_ground_damage();
    new spell_expelled_corruption();
    new spell_lightning_storm_aura();
    new spell_lightning_storm_damage();
    RegisterSpellScript(spell_lightning_prison);
    RegisterSpellScript(spell_corrupted_essence);
    new spell_superior_corrupted_essence();
    new spell_cleansing_waters_regen();
    RegisterSpellScript(spell_corrupted_waters_empowered);
}
