/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Battleground.h"
#include "CellImpl.h"
#include "Common.h"
#include "Formulas.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "OutdoorPvPMgr.h"
#include "Pet.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuraMetadata.h"
#include "SpellMgr.h"
#include "SpellValidation.h"
#include "Unit.h"
#include "Util.h"
#include "Vehicle.h"
#include "WeatherMgr.h"
#include "WorldPacket.h"

class Aura;

void AuraEffect::HandlePeriodicDummyAuraTick(Unit* target, Unit* caster) const
{
    switch (GetSpellInfo()->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
            switch (GetId())
            {
                case 66149: // Bullet Controller Periodic - 10 Man
                case 68396: // Bullet Controller Periodic - 25 Man
                {
                    if (!caster)
                        break;

                    Skyfire::Spells::PeriodicBulletControllerRule const* bulletController =
                        Skyfire::Spells::GetPeriodicBulletControllerRule(GetId());
                    if (!bulletController)
                        break;

                    caster->CastCustomSpell(bulletController->FirstSpellId, SPELLVALUE_MAX_TARGETS,
                        std::rand() % bulletController->MaxTargetRoll + 1, target, true);
                    caster->CastCustomSpell(bulletController->SecondSpellId, SPELLVALUE_MAX_TARGETS,
                        std::rand() % bulletController->MaxTargetRoll + 1, target, true);
                    break;
                }
                case 62292: // Blaze (Pool of Tar)
                    // should we use custom damage?
                    target->CastSpell((Unit*)NULL, m_spellInfo->Effects[m_effIndex].TriggerSpell, true);
                    break;
                case 62399: // Overload Circuit
                    if (target->GetMap()->IsDungeon() && int(target->GetAppliedAuras().count(62399)) >= (target->GetMap()->IsHeroic() ? 4 : 2))
                    {
                        uint32 shutdownSpellId = Skyfire::Spells::GetPeriodicOverloadCircuitShutdownSpellId(GetId());
                        target->CastSpell(target, shutdownSpellId, true); // System Shutdown
                        if (Unit* veh = target->GetVehicleBase())
                            veh->CastSpell(target, shutdownSpellId, true);
                    }
                    break;
                case 64821: // Fuse Armor (Razorscale)
                    if (GetBase()->GetStackAmount() == GetSpellInfo()->StackAmount)
                    {
                        target->CastSpell(target, Skyfire::Spells::GetPeriodicFuseArmorTriggerSpellId(GetId()),
                            true, NULL, NULL, GetCasterGUID());
                        target->RemoveAura(64821);
                    }
                    break;
            }
            break;
        case SPELLFAMILY_MAGE:
        {
            // Mirror Image
            if (GetId() == 55342)
                // Set name of summons to name of caster
                target->CastSpell((Unit*)NULL, m_spellInfo->Effects[m_effIndex].TriggerSpell, true);
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            switch (GetSpellInfo()->Id)
            {
                // Killing Spree
                case 51690:
                {
                    /// @todo this should use effect[1] of 51690
                    UnitList targets;
                    {
                        // eff_radius == 0
                        float radius = GetSpellInfo()->GetMaxRange(false);

                        CellCoord p(Skyfire::ComputeCellCoord(target->GetPositionX(), target->GetPositionY()));
                        Cell cell(p);

                        Skyfire::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck u_check(target, radius);
                        Skyfire::UnitListSearcher<Skyfire::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck> checker(target, targets, u_check);

                        TypeContainerVisitor<Skyfire::UnitListSearcher<Skyfire::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck>, GridTypeMapContainer > grid_object_checker(checker);
                        TypeContainerVisitor<Skyfire::UnitListSearcher<Skyfire::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck>, WorldTypeMapContainer > world_object_checker(checker);

                        cell.Visit(p, grid_object_checker, *GetBase()->GetOwner()->GetMap(), *target, radius);
                        cell.Visit(p, world_object_checker, *GetBase()->GetOwner()->GetMap(), *target, radius);
                    }

                    if (targets.empty())
                        return;

                    Unit* spellTarget = Skyfire::Containers::SelectRandomContainerElement(targets);

                    target->CastSpell(spellTarget, Skyfire::Spells::GetKillingSpreePeriodicSpellId(0), true);
                    target->CastSpell(spellTarget, Skyfire::Spells::GetKillingSpreePeriodicSpellId(1), true);
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
            switch (GetId())
            {
                case 49016: // Hysteria
                    uint32 damage = uint32(target->CountPctFromMaxHealth(1));
                    target->DealDamage(target, damage, NULL, NODAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    break;
            }
            // Death and Decay
            if (GetSpellInfo()->SpellFamilyFlags[0] & 0x20)
            {
                if (caster)
                    caster->CastCustomSpell(target, Skyfire::Spells::GetPeriodicDeathAndDecayTriggerSpellId(),
                        &m_amount, NULL, NULL, true, 0, this);
                break;
            }
            // Blood of the North
            // Reaping
            // Death Rune Mastery
            if (GetSpellInfo()->SpellIconID == 3041 || GetSpellInfo()->SpellIconID == 22 || GetSpellInfo()->SpellIconID == 2622)
            {
                if (target->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;
                if (target->ToPlayer()->getClass() != CLASS_DEATH_KNIGHT)
                    return;

                // timer expired - remove death runes
                target->ToPlayer()->RemoveRunesByAuraEffect(this);
            }
            break;
        default:
            break;
    }
}

void AuraEffect::HandlePeriodicTriggerSpellAuraTick(Unit* target, Unit* caster) const
{
    // generic casting code with custom spells and target/caster customs
    uint32 triggerSpellId = GetSpellInfo()->Effects[GetEffIndex()].TriggerSpell;

    SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId);
    SpellInfo const* auraSpellInfo = GetSpellInfo();
    uint32 auraId = auraSpellInfo->Id;

    // specific code for cases with no trigger spell provided in field
    if (triggeredSpellInfo == NULL)
    {
        switch (auraSpellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                switch (auraId)
                {
                    // Brood Affliction: Bronze
                    case 23170:
                        triggerSpellId = Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(auraId);
                        break;
                    // Restoration
                    case 24379:
                    case 23493:
                    {
                        if (caster)
                        {
                            int32 heal = caster->CountPctFromMaxHealth(10);
                            caster->HealBySpell(target, auraSpellInfo, heal);

                            if (int32 mana = caster->GetMaxPower(POWER_MANA))
                            {
                                mana /= 10;
                                caster->EnergizeBySpell(caster,
                                    Skyfire::Spells::GetPeriodicRestorationEnergizeSpellId(), mana, POWER_MANA);
                            }
                        }
                        return;
                    }
                    // Nitrous Boost
                    case 27746:
                        if (caster && target->GetPower(POWER_MANA) >= 10)
                        {
                            target->ModifyPower(POWER_MANA, -10);
                            target->SendEnergizeSpellLog(caster, auraId, 10, POWER_MANA);
                        }
                        else
                            target->RemoveAurasDueToSpell(auraId);
                        return;
                        // Frost Blast
                    case 27808:
                        if (caster)
                            caster->CastCustomSpell(Skyfire::Spells::GetPeriodicFrostBlastTriggerSpellId(),
                                SPELLVALUE_BASE_POINT0, int32(target->CountPctFromMaxHealth(21)), target, true, NULL, this);
                        return;
                        // Inoculate Nestlewood Owlkin
                    case 29528:
                        if (target->GetTypeId() != TypeID::TYPEID_UNIT) // prevent error reports in case ignored player target
                            return;
                        break;
                        // Feed Captured Animal
                    case 29917:
                        triggerSpellId = Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(auraId);
                        break;
                        // Extract Gas
                    case 30427:
                    {
                        // move loot to player inventory and despawn target
                        if (caster && caster->GetTypeId() == TypeID::TYPEID_PLAYER &&
                            target->GetTypeId() == TypeID::TYPEID_UNIT &&
                            target->ToCreature()->GetCreatureTemplate()->type == CREATURE_TYPE_GAS_CLOUD)
                        {
                            Player* player = caster->ToPlayer();
                            Creature* creature = target->ToCreature();
                            // missing lootid has been reported on startup - just return
                            if (!creature->GetCreatureTemplate()->SkinLootId)
                                return;

                            player->AutoStoreLoot(creature->GetCreatureTemplate()->SkinLootId, LootTemplates_Skinning, true);

                            creature->DespawnOrUnsummon();
                        }
                        return;
                    }
                    // Quake
                    case 30576:
                        triggerSpellId = Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(auraId);
                        break;
                        // Doom
                        /// @todo effect trigger spell may be independant on spell targets, and executed in spell finish phase
                        // so instakill will be naturally done before trigger spell
                    case 31347:
                    {
                        target->CastSpell(target, Skyfire::Spells::GetPeriodicDoomTriggerSpellId(), true, NULL, this);
                        target->Kill(target);
                        return;
                    }
                    // Spellcloth
                    case 31373:
                    {
                        // Summon Elemental after create item
                        target->SummonCreature(Skyfire::Spells::GetSpellclothSummonCreatureEntry(), 0, 0, 0,
                            target->GetOrientation(), TempSummonType::TEMPSUMMON_DEAD_DESPAWN, 0);
                        return;
                    }
                    // Flame Quills
                    case 34229:
                    {
                        // cast 24 spells 34269-34289, 34314-34316
                        for (uint32 spell_id = 34269; spell_id != 34290; ++spell_id)
                            target->CastSpell(target, spell_id, true, NULL, this);
                        for (uint32 spell_id = 34314; spell_id != 34317; ++spell_id)
                            target->CastSpell(target, spell_id, true, NULL, this);
                        return;
                    }
                    // Remote Toy
                    case 37027:
                        triggerSpellId = Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(auraId);
                        break;
                        // Eye of Grillok
                    case 38495:
                        triggerSpellId = Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(auraId);
                        break;
                        // Absorb Eye of Grillok (Zezzak's Shard)
                    case 38554:
                    {
                        if (!caster || target->GetTypeId() != TypeID::TYPEID_UNIT)
                            return;

                        caster->CastSpell(caster, Skyfire::Spells::GetAbsorbEyeOfGrillokCasterSpellId(), true, NULL, this);

                        Creature* creatureTarget = target->ToCreature();

                        creatureTarget->DespawnOrUnsummon();
                        return;
                    }
                    // Tear of Azzinoth Summon Channel - it's not really supposed to do anything, and this only prevents the console spam
                    case 39857:
                        triggerSpellId = Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(auraId);
                        break;
                        // Personalized Weather
                    case 46736:
                        triggerSpellId = Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(auraId);
                        break;
                }
                break;
            }
            case SPELLFAMILY_SHAMAN:
            {
                switch (auraId)
                {
                    // Lightning Shield (The Earthshatterer set trigger after cast Lighting Shield)
                    case 28820:
                    {
                        // Need remove self if Lightning Shield not active
                        if (!target->GetAuraEffect(SPELL_AURA_PROC_TRIGGER_SPELL, SPELLFAMILY_SHAMAN, 0x400))
                            target->RemoveAurasDueToSpell(28820);
                        return;
                    }
                    // Totemic Mastery (Skyshatter Regalia (Shaman Tier 6) - bonus)
                    case 38443:
                    {
                        bool all = true;
                        for (int i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
                        {
                            if (!target->m_SummonSlot[i])
                            {
                                all = false;
                                break;
                            }
                        }

                        if (all)
                            target->CastSpell(target, Skyfire::Spells::GetTotemicMasteryTriggerSpellId(), true, NULL, this);
                        else
                            target->RemoveAurasDueToSpell(Skyfire::Spells::GetTotemicMasteryTriggerSpellId());
                        return;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        // Spell exist but require custom code
        switch (auraId)
        {
            // Summon Tarindrella Aura
            case 92237:
            {
                if (caster->FindNearestCreature(49480, 15.0f, true))
                    return;
            }
            // Pursuing Spikes (Anub'arak)
            case 65920:
            case 65922:
            case 65923:
            {
                Unit* permafrostCaster = NULL;
                Aura* permafrostAura = target->GetAura(66193);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67855);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67856);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67857);

                if (permafrostAura)
                    permafrostCaster = permafrostAura->GetCaster();

                if (permafrostCaster)
                {
                    if (Creature* permafrostCasterCreature = permafrostCaster->ToCreature())
                        permafrostCasterCreature->DespawnOrUnsummon(3000);

                    target->CastSpell(target, 66181, false);
                    target->RemoveAllAuras();
                    if (Creature* targetCreature = target->ToCreature())
                        targetCreature->DisappearAndDie();
                }
                break;
            }
            // Mana Tide
            case 16191:
                target->CastCustomSpell(target, triggerSpellId, &m_amount, NULL, NULL, true, NULL, this);
                return;
                // Negative Energy Periodic
            case 46284:
                target->CastCustomSpell(triggerSpellId, SPELLVALUE_MAX_TARGETS, m_tickNumber / 10 + 1, NULL, true, NULL, this);
                return;
                // Poison (Grobbulus)
            case 28158:
            case 54362:
                // Slime Pool (Dreadscale & Acidmaw)
            case 66882:
                target->CastCustomSpell(triggerSpellId, SPELLVALUE_RADIUS_MOD, (int32)((((float)m_tickNumber / 60) * 0.9f + 0.1f) * 10000 * 2 / 3), NULL, true, NULL, this);
                return;
                // Slime Spray - temporary here until preventing default effect works again
                // added on 9.10.2010
            case 69508:
            {
                if (caster)
                    caster->CastSpell(target, triggerSpellId, true, NULL, NULL, caster->GetGUID());
                return;
            }
            case 24745: // Summon Templar, Trigger
            case 24747: // Summon Templar Fire, Trigger
            case 24757: // Summon Templar Air, Trigger
            case 24759: // Summon Templar Earth, Trigger
            case 24761: // Summon Templar Water, Trigger
            case 24762: // Summon Duke, Trigger
            case 24766: // Summon Duke Fire, Trigger
            case 24769: // Summon Duke Air, Trigger
            case 24771: // Summon Duke Earth, Trigger
            case 24773: // Summon Duke Water, Trigger
            case 24785: // Summon Royal, Trigger
            case 24787: // Summon Royal Fire, Trigger
            case 24791: // Summon Royal Air, Trigger
            case 24792: // Summon Royal Earth, Trigger
            case 24793: // Summon Royal Water, Trigger
            {
                // All this spells trigger a spell that requires reagents; if the
                // triggered spell is cast as "triggered", reagents are not consumed
                if (caster && Skyfire::Spells::IsReagentConsumingPeriodicTriggerAura(auraId))
                    caster->CastSpell(target, triggerSpellId, false);
                return;
            }
            // Emblazon Runeblade
            case 51769:
            {
                caster->CastSpell(target, triggerSpellId, false);
                return;
            }
        }
    }

    // Reget trigger spell proto
    triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId);

    if (triggeredSpellInfo)
    {
        if (Unit* triggerCaster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? caster : target)
        {
            triggerCaster->CastSpell(target, triggeredSpellInfo, true, NULL, this);
            SF_LOG_DEBUG("spells", "AuraEffect::HandlePeriodicTriggerSpellAuraTick: Spell %u Trigger %u", GetId(), triggeredSpellInfo->Id);
        }
    }
    else
    {
        Creature* c = target->ToCreature();
        if (!c || !caster || !sScriptMgr->OnDummyEffect(caster, GetId(), SpellEffIndex(GetEffIndex()), target->ToCreature()) ||
            !c->AI()->sOnDummyEffect(caster, GetId(), SpellEffIndex(GetEffIndex())))
            SF_LOG_DEBUG("spells", "AuraEffect::HandlePeriodicTriggerSpellAuraTick: Spell %u has non-existent spell %u in EffectTriggered[%d] and is therefor not triggered.", GetId(), triggerSpellId, GetEffIndex());
    }
}

void AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick(Unit* target, Unit* caster) const
{
    uint32 triggerSpellId = GetSpellInfo()->Effects[m_effIndex].TriggerSpell;
    if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
    {
        if (Unit* triggerCaster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? caster : target)
        {
            int32 basepoints = GetAmount();
            triggerCaster->CastCustomSpell(target, triggerSpellId, &basepoints, &basepoints, &basepoints, true, 0, this);
            SF_LOG_DEBUG("spells", "AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick: Spell %u Trigger %u", GetId(), triggeredSpellInfo->Id);
        }
    }
    else
        SF_LOG_DEBUG("spells", "AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick: Spell %u has non-existent spell %u in EffectTriggered[%d] and is therefor not triggered.", GetId(), triggerSpellId, GetEffIndex());
}

void AuraEffect::HandlePeriodicDamageAurasTick(Unit* target, Unit* caster) const
{
    if (!caster || !target->IsAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    // Consecrate ticks can miss and will not show up in the combat log
    if (GetSpellInfo()->Effects[GetEffIndex()].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        caster->SpellHitResult(target, GetSpellInfo(), false) != SPELL_MISS_NONE)
        return;

    // some auras remove at specific health level or more
    if (GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE)
    {
        switch (GetSpellInfo()->Id)
        {
            case 43093: case 31956: case 38801:  // Grievous Wound
            case 35321: case 38363: case 39215:  // Gushing Wound
                if (target->IsFullHealth())
                {
                    if (Skyfire::Spells::IsFullHealthRemovePeriodicDamageAura(GetSpellInfo()->Id))
                        target->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                    return;
                }
                break;
            case 38772: // Grievous Wound
            {
                uint32 percent = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);
                if (!target->HealthBelowPct(percent))
                {
                    if (Skyfire::Spells::IsPctHealthRemovePeriodicDamageAura(GetSpellInfo()->Id))
                        target->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                    return;
                }
                break;
            }
        }
    }

    uint32 absorb = 0;
    uint32 resist = 0;
    CleanDamage cleanDamage = CleanDamage(0, 0, WeaponAttackType::BASE_ATTACK, MeleeHitOutcome::MELEE_HIT_NORMAL);

    // ignore non positive values (can be result apply spellmods to aura damage
    uint32 damage = std::max(GetAmount(), 0);

    // Script Hook For HandlePeriodicDamageAurasTick -- Allow scripts to change the Damage pre class mitigation calculations
    sScriptMgr->ModifyPeriodicDamageAurasTick(target, caster, damage);

    if (GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE)
    {
        damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), damage, DOT, GetBase()->GetStackAmount());
        damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), damage, DOT, GetBase()->GetStackAmount());

        // Calculate armor mitigation
        if (Unit::IsDamageReducedByArmor(GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), GetEffIndex()))
        {
            uint32 damageReductedArmor = caster->CalcArmorReducedDamage(target, damage, GetSpellInfo());
            cleanDamage.mitigated_damage += damage - damageReductedArmor;
            damage = damageReductedArmor;
        }

        // Curse of Agony damage-per-tick calculation
        if (GetSpellInfo()->SpellFamilyName == SPELLFAMILY_WARLOCK && (GetSpellInfo()->SpellFamilyFlags[0] & 0x400) && GetSpellInfo()->SpellIconID == 544)
        {
            uint32 totalTick = GetTotalTicks();
            // 1..4 ticks, 1/2 from normal tick damage
            if (m_tickNumber <= totalTick / 3)
                damage = damage / 2;
            // 9..12 ticks, 3/2 from normal tick damage
            else if (m_tickNumber > totalTick * 2 / 3)
                damage += (damage + 1) / 2;           // +1 prevent 0.5 damage possible lost at 1..4 ticks
            // 5..8 ticks have normal tick damage
        }
        // There is a Chance to make a Soul Shard when Drain soul does damage
        if (GetSpellInfo()->SpellFamilyName == SPELLFAMILY_WARLOCK && (GetSpellInfo()->SpellFamilyFlags[0] & 0x00004000))
        {
            if (caster->GetTypeId() == TypeID::TYPEID_PLAYER && caster->ToPlayer()->isHonorOrXPTarget(target))
                caster->CastSpell(caster, Skyfire::Spells::GetDrainSoulShardSpellId(), true, 0, this);
        }
        if (GetSpellInfo()->SpellFamilyName == SPELLFAMILY_GENERIC)
        {
            switch (GetId())
            {
                case 70911: // Unbound Plague
                case 72854: // Unbound Plague
                case 72855: // Unbound Plague
                case 72856: // Unbound Plague
                    if (Skyfire::Spells::IsUnboundPlaguePeriodicDamageAura(GetId()))
                        damage *= uint32(pow(1.25f, int32(m_tickNumber)));
                    break;
                default:
                    break;
            }
        }
    }
    else
        damage = uint32(target->CountPctFromMaxHealth(damage));

    bool crit = IsPeriodicTickCrit(target, caster);
    if (crit)
        damage = caster->SpellCriticalDamageBonus(m_spellInfo, damage, target);

    int32 dmg = damage;
    caster->ApplyResilience(target, &dmg, crit);
    damage = dmg;

    caster->CalcAbsorbResist(target, GetSpellInfo()->GetSchoolMask(), DOT, damage, &absorb, &resist, GetSpellInfo());

    SF_LOG_INFO("spells", "PeriodicTick: %u (TypeId: %u) attacked %u (TypeId: %u) for %u dmg inflicted by %u abs is %u",
        GUID_LOPART(GetCasterGUID()), uint8(GuidHigh2TypeId(GUID_HIPART(GetCasterGUID()))), target->GetGUIDLow(), uint8(target->GetTypeId()), damage, GetId(), absorb);

    caster->DealDamageMods(target, damage, &absorb);

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DONE_PERIODIC;
    uint32 procVictim = PROC_FLAG_TAKEN_PERIODIC;
    uint32 procEx = (crit ? PROC_EX_CRITICAL_HIT : PROC_EX_NORMAL_HIT) | PROC_EX_INTERNAL_DOT;
    damage = (damage <= absorb + resist) ? 0 : (damage - absorb - resist);
    if (damage)
        procVictim |= PROC_FLAG_TAKEN_DAMAGE;

    int32 overkill = damage - target->GetHealth();
    if (overkill < 0)
        overkill = 0;

    SpellPeriodicAuraLogInfo pInfo(this, damage, overkill, absorb, resist, GetSpellInfo()->GetSchoolMask(), 0.0f, crit);
    target->SendPeriodicAuraLog(&pInfo);

    caster->ProcDamageAndSpell(target, procAttacker, procVictim, procEx, damage, WeaponAttackType::BASE_ATTACK, GetSpellInfo());

    caster->DealDamage(target, damage, &cleanDamage, DOT, GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), true);
}

void AuraEffect::HandlePeriodicHealthLeechAuraTick(Unit* target, Unit* caster) const
{
    if (!caster || !target->IsAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    if (GetSpellInfo()->Effects[GetEffIndex()].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        caster->SpellHitResult(target, GetSpellInfo(), false) != SPELL_MISS_NONE)
        return;

    uint32 absorb = 0;
    uint32 resist = 0;
    CleanDamage cleanDamage = CleanDamage(0, 0, WeaponAttackType::BASE_ATTACK, MeleeHitOutcome::MELEE_HIT_NORMAL);

    uint32 damage = std::max(GetAmount(), 0);

    damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), damage, DOT, GetBase()->GetStackAmount());
    damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), damage, DOT, GetBase()->GetStackAmount());

    bool crit = IsPeriodicTickCrit(target, caster);
    if (crit)
        damage = caster->SpellCriticalDamageBonus(m_spellInfo, damage, target);

    // Calculate armor mitigation
    if (Unit::IsDamageReducedByArmor(GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), m_effIndex))
    {
        uint32 damageReductedArmor = caster->CalcArmorReducedDamage(target, damage, GetSpellInfo());
        cleanDamage.mitigated_damage += damage - damageReductedArmor;
        damage = damageReductedArmor;
    }

    int32 dmg = damage;
    caster->ApplyResilience(target, &dmg, crit);
    damage = dmg;

    caster->CalcAbsorbResist(target, GetSpellInfo()->GetSchoolMask(), DOT, damage, &absorb, &resist, m_spellInfo);

    if (target->GetHealth() < damage)
        damage = uint32(target->GetHealth());

    SF_LOG_INFO("spells", "PeriodicTick: %u (TypeId: %u) health leech of %u (TypeId: %u) for %u dmg inflicted by %u abs is %u",
        GUID_LOPART(GetCasterGUID()), uint8(GuidHigh2TypeId(GUID_HIPART(GetCasterGUID()))), target->GetGUIDLow(), uint8(target->GetTypeId()), damage, GetId(), absorb);

    caster->SendSpellNonMeleeDamageLog(target, GetId(), damage, GetSpellInfo()->GetSchoolMask(), absorb, resist, false, 0, crit);

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DONE_PERIODIC;
    uint32 procVictim = PROC_FLAG_TAKEN_PERIODIC;
    uint32 procEx = (crit ? PROC_EX_CRITICAL_HIT : PROC_EX_NORMAL_HIT) | PROC_EX_INTERNAL_DOT;
    damage = (damage <= absorb + resist) ? 0 : (damage - absorb - resist);
    if (damage)
        procVictim |= PROC_FLAG_TAKEN_DAMAGE;
    if (caster->IsAlive())
        caster->ProcDamageAndSpell(target, procAttacker, procVictim, procEx, damage, WeaponAttackType::BASE_ATTACK, GetSpellInfo());
    int32 new_damage = caster->DealDamage(target, damage, &cleanDamage, DOT, GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), false);
    if (caster->IsAlive())
    {
        float gainMultiplier = GetSpellInfo()->Effects[GetEffIndex()].CalcValueMultiplier(caster);

        uint32 heal = uint32(caster->SpellHealingBonusDone(caster, GetSpellInfo(), uint32(new_damage * gainMultiplier), DOT, GetBase()->GetStackAmount()));
        heal = uint32(caster->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, DOT, GetBase()->GetStackAmount()));

        int32 gain = caster->HealBySpell(caster, GetSpellInfo(), heal);
        caster->getHostileRefManager().threatAssist(caster, gain * 0.5f, GetSpellInfo());
    }
}

void AuraEffect::HandlePeriodicHealthFunnelAuraTick(Unit* target, Unit* caster) const
{
    if (!caster || !caster->IsAlive() || !target->IsAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    uint32 damage = std::max(GetAmount(), 0);
    // do not kill health donator
    if (caster->GetHealth() < damage)
        damage = caster->GetHealth() - 1;
    if (!damage)
        return;

    caster->ModifyHealth(-(int32)damage);
    SF_LOG_DEBUG("spells", "PeriodicTick: donator %u target %u damage %u.", caster->GetEntry(), target->GetEntry(), damage);

    float gainMultiplier = GetSpellInfo()->Effects[GetEffIndex()].CalcValueMultiplier(caster);

    damage = int32(damage * gainMultiplier);

    caster->HealBySpell(target, GetSpellInfo(), damage);
}

void AuraEffect::HandlePeriodicHealAurasTick(Unit* target, Unit* caster) const
{
    if (!caster || !target->IsAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // heal for caster damage (must be alive)
    if (target != caster && GetSpellInfo()->AttributesEx2 & SPELL_ATTR2_HEALTH_FUNNEL && !caster->IsAlive())
        return;

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->IsFullHealth())
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 damage = std::max(m_amount, 0);

    if (GetAuraType() == SPELL_AURA_OBS_MOD_HEALTH)
    {
        // Taken mods
        float TakenTotalMod = 1.0f;

        // Tenacity increase healing % taken
        if (AuraEffect const* Tenacity = target->GetAuraEffect(Skyfire::Spells::GetTenacityAuraSpellId(), 0))
            AddPct(TakenTotalMod, Tenacity->GetAmount());

        // Healing taken percent
        float minval = (float)target->GetMaxNegativeAuraModifier(SPELL_AURA_MOD_HEALING_PCT);
        if (minval)
            AddPct(TakenTotalMod, minval);

        float maxval = (float)target->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_HEALING_PCT);
        if (maxval)
            AddPct(TakenTotalMod, maxval);

        TakenTotalMod = std::max(TakenTotalMod, 0.0f);

        damage = uint32(target->CountPctFromMaxHealth(damage));
        damage = uint32(damage * TakenTotalMod);
    }
    else
    {
        // Wild Growth = amount + (6 - 2*doneTicks) * ticks* amount / 100
        if (m_spellInfo->SpellFamilyName == SPELLFAMILY_DRUID && m_spellInfo->SpellIconID == 2864)
        {
            int32 addition = int32(float(damage * GetTotalTicks()) * ((6 - float(2 * (GetTickNumber() - 1))) / 100));

            // Item - Druid T10 Restoration 2P Bonus
            if (AuraEffect* aurEff = caster->GetAuraEffect(Skyfire::Spells::GetWildGrowthTier10AuraSpellId(), 0))
                // divided by 50 instead of 100 because calculated as for every 2 tick
                addition += abs(int32((addition * aurEff->GetAmount()) / 50));

            damage += addition;
        }

        damage = caster->SpellHealingBonusDone(target, GetSpellInfo(), damage, DOT, GetBase()->GetStackAmount());
        damage = target->SpellHealingBonusTaken(caster, GetSpellInfo(), damage, DOT, GetBase()->GetStackAmount());
    }

    bool crit = IsPeriodicTickCrit(target, caster);
    if (crit)
        damage = caster->SpellCriticalHealingBonus(m_spellInfo, damage, target);

    SF_LOG_INFO("spells", "PeriodicTick: %u (TypeId: %u) heal of %u (TypeId: %u) for %u health inflicted by %u",
        GUID_LOPART(GetCasterGUID()), uint8(GuidHigh2TypeId(GUID_HIPART(GetCasterGUID()))), target->GetGUIDLow(), uint8(target->GetTypeId()), damage, GetId());

    uint32 absorb = 0;
    uint32 heal = uint32(damage);
    caster->CalcHealAbsorb(target, GetSpellInfo(), heal, absorb);
    int32 gain = caster->DealHeal(target, heal);

    SpellPeriodicAuraLogInfo pInfo(this, heal, heal - gain, absorb, 0, GetSpellInfo()->GetSchoolMask(), 0.0f, crit);
    target->SendPeriodicAuraLog(&pInfo);

    target->getHostileRefManager().threatAssist(caster, float(gain) * 0.5f, GetSpellInfo());

    bool haveCastItem = GetBase()->GetCastItemGUID() != 0;

    // Health Funnel
    // damage caster for heal amount
    if (target != caster && GetSpellInfo()->AttributesEx2 & SPELL_ATTR2_HEALTH_FUNNEL && GetSpellInfo()->Id != 755)
    {
        uint32 funnelDamage = gain;
        uint32 funnelAbsorb = 0;
        caster->DealDamageMods(caster, funnelDamage, &funnelAbsorb);
        caster->SendSpellNonMeleeDamageLog(caster, GetId(), funnelDamage, GetSpellInfo()->GetSchoolMask(), funnelAbsorb, 0, false, 0, false);

        CleanDamage cleanDamage = CleanDamage(0, 0, WeaponAttackType::BASE_ATTACK, MeleeHitOutcome::MELEE_HIT_NORMAL);
        caster->DealDamage(caster, funnelDamage, &cleanDamage, NODAMAGE, GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), true);
    }

    uint32 procAttacker = PROC_FLAG_DONE_PERIODIC;
    uint32 procVictim = PROC_FLAG_TAKEN_PERIODIC;
    uint32 procEx = (crit ? PROC_EX_CRITICAL_HIT : PROC_EX_NORMAL_HIT) | PROC_EX_INTERNAL_HOT;
    // ignore item heals
    if (!haveCastItem)
        caster->ProcDamageAndSpell(target, procAttacker, procVictim, procEx, damage, WeaponAttackType::BASE_ATTACK, GetSpellInfo());
}

void AuraEffect::HandlePeriodicManaLeechAuraTick(Unit* target, Unit* caster) const
{
    Powers powerType = Powers(GetMiscValue());

    if (!caster || !caster->IsAlive() || !target->IsAlive() || target->getPowerType() != powerType)
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    if (GetSpellInfo()->Effects[GetEffIndex()].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        caster->SpellHitResult(target, GetSpellInfo(), false) != SPELL_MISS_NONE)
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 drainAmount = std::max(m_amount, 0);

    SF_LOG_INFO("spells", "PeriodicTick: %u (TypeId: %u) power leech of %u (TypeId: %u) for %u dmg inflicted by %u",
        GUID_LOPART(GetCasterGUID()), uint8(GuidHigh2TypeId(GUID_HIPART(GetCasterGUID()))), target->GetGUIDLow(), uint8(target->GetTypeId()), drainAmount, GetId());

    int32 drainedAmount = -target->ModifyPower(powerType, -drainAmount);

    float gainMultiplier = GetSpellInfo()->Effects[GetEffIndex()].CalcValueMultiplier(caster);

    SpellPeriodicAuraLogInfo pInfo(this, drainedAmount, 0, 0, 0, GetMiscValue(), gainMultiplier, false);
    target->SendPeriodicAuraLog(&pInfo);

    int32 gainAmount = int32(drainedAmount * gainMultiplier);
    int32 gainedAmount = 0;
    if (gainAmount)
    {
        gainedAmount = caster->ModifyPower(powerType, gainAmount);
        target->AddThreat(caster, float(gainedAmount) * 0.5f, GetSpellInfo()->GetSchoolMask(), GetSpellInfo());
    }
}

void AuraEffect::HandleObsModPowerAuraTick(Unit* target, Unit* caster) const
{
    Powers powerType;
    if (GetMiscValue() == POWER_ALL)
        powerType = target->getPowerType();
    else
        powerType = Powers(GetMiscValue());

    if (!target->IsAlive() || !target->GetMaxPower(powerType))
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->GetPower(powerType) == target->GetMaxPower(powerType))
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    uint32 amount = std::max(m_amount, 0) * target->GetMaxPower(powerType) / 100;
    SF_LOG_INFO("spells", "PeriodicTick: %u (TypeId: %u) energize %u (TypeId: %u) for %u dmg inflicted by %u",
        GUID_LOPART(GetCasterGUID()), uint8(GuidHigh2TypeId(GUID_HIPART(GetCasterGUID()))), target->GetGUIDLow(), uint8(target->GetTypeId()), amount, GetId());

    SpellPeriodicAuraLogInfo pInfo(this, amount, 0, 0, 0, GetMiscValue(), 0.0f, false);
    target->SendPeriodicAuraLog(&pInfo);

    int32 gain = target->ModifyPower(powerType, amount);

    if (caster)
        target->getHostileRefManager().threatAssist(caster, float(gain) * 0.5f, GetSpellInfo());
}

void AuraEffect::HandlePeriodicEnergizeAuraTick(Unit* target, Unit* caster) const
{
    Powers powerType = Powers(GetMiscValue());

    if (!target->IsAlive() || !target->GetMaxPower(powerType))
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->GetPower(powerType) == target->GetMaxPower(powerType))
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 amount = std::max(m_amount, 0);

    SpellPeriodicAuraLogInfo pInfo(this, amount, 0, 0, 0, GetMiscValue(), 0.0f, false);
    target->SendPeriodicAuraLog(&pInfo);

    SF_LOG_INFO("spells", "PeriodicTick: %u (TypeId: %u) energize %u (TypeId: %u) for %u dmg inflicted by %u",
        GUID_LOPART(GetCasterGUID()), uint8(GuidHigh2TypeId(GUID_HIPART(GetCasterGUID()))), target->GetGUIDLow(), uint8(target->GetTypeId()), amount, GetId());

    int32 gain = target->ModifyPower(powerType, amount);

    if (caster)
        target->getHostileRefManager().threatAssist(caster, float(gain) * 0.5f, GetSpellInfo());
}

void AuraEffect::HandlePeriodicPowerBurnAuraTick(Unit* target, Unit* caster) const
{
    Powers powerType = Powers(GetMiscValue());

    if (!caster || !target->IsAlive() || target->getPowerType() != powerType)
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    // ignore negative values (can be result apply spellmods to aura damage
    int32 damage = std::max(m_amount, 0);

    uint32 gain = uint32(-target->ModifyPower(powerType, -damage));

    float dmgMultiplier = GetSpellInfo()->Effects[GetEffIndex()].CalcValueMultiplier(caster);

    SpellInfo const* spellProto = GetSpellInfo();
    // maybe has to be sent different to client, but not by SMSG_SPELL_PERIODIC_AURA_LOG
    SpellNonMeleeDamage damageInfo(caster, target, spellProto->Id, spellProto->SchoolMask);
    // no SpellDamageBonus for burn mana
    caster->CalculateSpellDamageTaken(&damageInfo, int32(gain * dmgMultiplier), spellProto);

    caster->DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);

    caster->SendSpellNonMeleeDamageLog(&damageInfo);

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DONE_PERIODIC;
    uint32 procVictim = PROC_FLAG_TAKEN_PERIODIC;
    uint32 procEx = createProcExtendMask(&damageInfo, SPELL_MISS_NONE) | PROC_EX_INTERNAL_DOT;
    if (damageInfo.damage)
        procVictim |= PROC_FLAG_TAKEN_DAMAGE;

    caster->ProcDamageAndSpell(damageInfo.target, procAttacker, procVictim, procEx, damageInfo.damage, WeaponAttackType::BASE_ATTACK, spellProto);

    caster->DealSpellDamage(&damageInfo, true);
}
