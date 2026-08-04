/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AccountMgr.h"
#include "AreaTrigger.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlePetMgr.h"
#include "CellImpl.h"
#include "Common.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "DatabaseEnv.h"
#include "DynamicObject.h"
#include "Formulas.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "GossipDef.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "InstanceScript.h"
#include "Language.h"
#include "Log.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "OutdoorPvPMgr.h"
#include "PathGenerator.h"
#include "Pet.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "SkillDiscovery.h"
#include "SkillExtraItems.h"
#include "SocialMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellEffectMetadata.h"
#include "SpellMgr.h"
#include "SpellValidation.h"
#include "TemporarySummon.h"
#include "Totem.h"
#include "Unit.h"
#include "UpdateData.h"
#include "UpdateMask.h"
#include "Util.h"
#include "Vehicle.h"
#include "VMapFactory.h"
#include "World.h"
#include "WorldPacket.h"

void Spell::EffectTriggerSpell(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET
        && effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH)
        return;

    uint32 triggered_spell_id = m_spellInfo->Effects[effIndex].TriggerSpell;

    /// @todo move those to spell scripts
    if (m_spellInfo->Effects[effIndex].Effect == SPELL_EFFECT_TRIGGER_SPELL
        && effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
    {
        if (Skyfire::Spells::TriggerSpellRule const* triggerRule =
            Skyfire::Spells::GetTriggerSpellRule(triggered_spell_id))
        {
            switch (triggerRule->Kind)
            {
                case Skyfire::Spells::TRIGGER_SPELL_RULE_VANISH:
                {
                    unitTarget->RemoveMovementImpairingAuras();
                    unitTarget->RemoveAurasByType(SPELL_AURA_MOD_STALKED);

                    // Interrupt spells currently targeting the vanishing unit
                    {
                        UnitList targets;
                        Skyfire::AnyUnfriendlyUnitInObjectRangeCheck u_check(unitTarget, unitTarget, unitTarget->GetMap()->GetVisibilityRange());
                        Skyfire::UnitListSearcher<Skyfire::AnyUnfriendlyUnitInObjectRangeCheck> searcher(unitTarget, targets, u_check);
                        unitTarget->VisitNearbyObject(unitTarget->GetMap()->GetVisibilityRange(), searcher);
                        for (UnitList::iterator iter = targets.begin(); iter != targets.end(); ++iter)
                        {
                            if (!(*iter)->HasUnitState(UNIT_STATE_CASTING))
                                continue;

                            for (uint32 i = CURRENT_FIRST_NON_MELEE_SPELL; i < CURRENT_MAX_SPELL; ++i)
                            {
                                if ((*iter)->GetCurrentSpell(i)
                                    && (*iter)->GetCurrentSpell(i)->m_targets.GetUnitTargetGUID() == unitTarget->GetGUID())
                                    (*iter)->InterruptSpell(CurrentSpellTypes(i), false);
                            }
                        }
                    }

                    // If this spell is given to an NPC, it must handle the rest using its own AI
                    if (unitTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    uint32 const subterfugeTalent = 108208;
                    uint32 const subterfugeVanish = 115193;
                    uint32 vanishAuraId = unitTarget->HasSpell(subterfugeTalent) ? subterfugeVanish : triggerRule->CastSpellId;

                    // Already in this Vanish state
                    if (unitTarget->HasAura(vanishAuraId))
                        return;

                    // Drop normal stealth so Vanish aura can replace it
                    unitTarget->RemoveAurasDueToSpell(1784);
                    unitTarget->RemoveAurasDueToSpell(115191);

                    if (!unitTarget->GetInstanceScript() || !unitTarget->GetInstanceScript()->IsEncounterInProgress())
                    {
                        unitTarget->CombatStop();
                        unitTarget->getHostileRefManager().deleteReferences();
                    }

                    unitTarget->CastSpell(unitTarget, vanishAuraId, true);
                    return;
                }
                case Skyfire::Spells::TRIGGER_SPELL_RULE_DEMONIC_EMPOWERMENT_SUCCUBUS:
                {
                    unitTarget->RemoveMovementImpairingAuras();
                    unitTarget->RemoveAurasByType(SPELL_AURA_MOD_STALKED);
                    unitTarget->RemoveAurasByType(SPELL_AURA_MOD_STUN);

                    unitTarget->CastSpell(unitTarget, triggerRule->CastSpellId, true);
                    return;
                }
                case Skyfire::Spells::TRIGGER_SPELL_RULE_STACK_TRIGGER:
                {
                    SpellInfo const* spell = sSpellMgr->GetSpellInfo(triggerRule->StackSpellId);
                    if (!spell)
                        return;

                    for (uint32 j = 0; j < spell->StackAmount; ++j)
                        m_caster->CastSpell(unitTarget, spell->Id, true);
                    return;
                }
                case Skyfire::Spells::TRIGGER_SPELL_RULE_CLOAK_OF_SHADOWS:
                {
                    uint32 dispelMask = SpellInfo::GetDispelMask(DISPEL_ALL);
                    Unit::AuraApplicationMap& Auras = unitTarget->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::iterator iter = Auras.begin(); iter != Auras.end();)
                    {
                        // remove all harmful spells on you...
                        SpellInfo const* spell = iter->second->GetBase()->GetSpellInfo();
                        if ((spell->DmgClass == SPELL_DAMAGE_CLASS_MAGIC // only affect magic spells
                            || ((spell->GetDispelMask()) & dispelMask))
                            // ignore positive and passive auras
                            && !iter->second->IsPositive() && !iter->second->GetBase()->IsPassive())
                        {
                            m_caster->RemoveAura(iter);
                        }
                        else
                            ++iter;
                    }
                    return;
                }
                default:
                    break;
            }
        }
    }

    // normal case
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(triggered_spell_id);
    if (!spellInfo)
    {
        SF_LOG_DEBUG("spells", "Spell::EffectTriggerSpell spell %u tried to trigger unknown spell %u", m_spellInfo->Id, triggered_spell_id);
        return;
    }

    SpellCastTargets targets;
    if (effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
    {
        if (!spellInfo->NeedsToBeTriggeredByCaster(m_spellInfo))
            return;
        targets.SetUnitTarget(unitTarget);
    }
    else //if (effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH)
    {
        if (spellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) && (m_spellInfo->Effects[effIndex].GetProvidedTargetMask() & TARGET_FLAG_UNIT_MASK))
            return;

        if (spellInfo->GetExplicitTargetMask() & TARGET_FLAG_DEST_LOCATION)
            targets.SetDst(m_targets);

        targets.SetUnitTarget(m_caster);
    }

    CustomSpellValues values;
    // set basepoints for trigger with value effect
    if (m_spellInfo->Effects[effIndex].Effect == SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE)
    {
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, damage);
    }

    // Remove spell cooldown (not category) if spell triggering spell with cooldown and same category
    if (m_caster->GetTypeId() == TypeID::TYPEID_PLAYER && m_spellInfo->CategoryRecoveryTime && spellInfo->CategoryRecoveryTime
        && m_spellInfo->GetCategory() == spellInfo->GetCategory())
        m_caster->ToPlayer()->RemoveSpellCooldown(spellInfo->Id);

    // original caster guid only for GO cast
    m_caster->CastSpell(targets, spellInfo, &values, TRIGGERED_FULL_MASK, NULL, NULL, m_originalCasterGUID);
}

void Spell::EffectTriggerMissileSpell(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET
        && effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    uint32 triggered_spell_id = m_spellInfo->Effects[effIndex].TriggerSpell;

    // normal case
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(triggered_spell_id);
    if (!spellInfo)
    {
        SF_LOG_DEBUG("spells", "Spell::EffectTriggerMissileSpell spell %u tried to trigger unknown spell %u", m_spellInfo->Id, triggered_spell_id);
        return;
    }

    SpellCastTargets targets;
    if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT_TARGET)
    {
        if (!spellInfo->NeedsToBeTriggeredByCaster(m_spellInfo))
            return;
        targets.SetUnitTarget(unitTarget);
    }
    else //if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT)
    {
        if (spellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) && (m_spellInfo->Effects[effIndex].GetProvidedTargetMask() & TARGET_FLAG_UNIT_MASK))
            return;

        if (spellInfo->GetExplicitTargetMask() & TARGET_FLAG_DEST_LOCATION)
            targets.SetDst(m_targets);

        targets.SetUnitTarget(m_caster);
    }

    CustomSpellValues values;
    // set basepoints for trigger with value effect
    if (m_spellInfo->Effects[effIndex].Effect == SPELL_EFFECT_TRIGGER_MISSILE_SPELL_WITH_VALUE)
    {
        // maybe need to set value only when basepoints == 0?
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, damage);
    }

    // Remove spell cooldown (not category) if spell triggering spell with cooldown and same category
    if (m_caster->GetTypeId() == TypeID::TYPEID_PLAYER && m_spellInfo->CategoryRecoveryTime && spellInfo->CategoryRecoveryTime
        && m_spellInfo->GetCategory() == spellInfo->GetCategory())
        m_caster->ToPlayer()->RemoveSpellCooldown(spellInfo->Id);

    // original caster guid only for GO cast
    m_caster->CastSpell(targets, spellInfo, &values, TRIGGERED_FULL_MASK, NULL, NULL, m_originalCasterGUID);
}

void Spell::EffectForceCast(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    uint32 triggered_spell_id = m_spellInfo->Effects[effIndex].TriggerSpell;

    // normal case
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(triggered_spell_id);

    if (!spellInfo)
    {
        SF_LOG_ERROR("spells", "Spell::EffectForceCast of spell %u: triggering unknown spell id %i", m_spellInfo->Id, triggered_spell_id);
        return;
    }

    if (m_spellInfo->Effects[effIndex].Effect == SPELL_EFFECT_FORCE_CAST && damage)
    {
        if (Skyfire::Spells::ForceCastDamageRule const* forceCastRule =
            Skyfire::Spells::GetForceCastDamageRule(m_spellInfo->Id))
        {
            switch (forceCastRule->Kind)
            {
                case Skyfire::Spells::FORCE_CAST_DAMAGE_RULE_REMOVE_AURA:
                    unitTarget->RemoveAura(damage);
                    break;
                case Skyfire::Spells::FORCE_CAST_DAMAGE_RULE_CUSTOM_BASEPOINTS:
                    unitTarget->CastCustomSpell(unitTarget, spellInfo->Id, &damage, NULL, NULL, true, NULL, NULL, m_originalCasterGUID);
                    return;
                case Skyfire::Spells::FORCE_CAST_DAMAGE_RULE_TRIGGERED_ORIGINAL_CASTER:
                    unitTarget->CastSpell(unitTarget, spellInfo->Id, true, NULL, NULL, m_originalCasterGUID);
                    return;
                default:
                    break;
            }
        }
    }

    CustomSpellValues values;
    // set basepoints for trigger with value effect
    if (m_spellInfo->Effects[effIndex].Effect == SPELL_EFFECT_FORCE_CAST_WITH_VALUE)
    {
        // maybe need to set value only when basepoints == 0?
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, damage);
    }

    SpellCastTargets targets;
    targets.SetUnitTarget(m_caster);

    unitTarget->CastSpell(targets, spellInfo, &values, TRIGGERED_FULL_MASK);
}

void Spell::EffectSendEvent(SpellEffIndex effIndex)
{
    // we do not handle a flag dropping or clicking on flag in battleground by sendevent system
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET
        && effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    WorldObject* target = NULL;

    // call events for object target if present
    if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT_TARGET)
    {
        if (unitTarget)
            target = unitTarget;
        else if (gameObjTarget)
            target = gameObjTarget;
    }
    else // if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT)
    {
        // let's prevent executing effect handler twice in case when spell effect is capable of targeting an object
        // this check was requested by scripters, but it has some downsides:
        // now it's impossible to script (using sEventScripts) a cast which misses all targets
        // or to have an ability to script the moment spell hits dest (in a case when there are object targets present)
        if (m_spellInfo->Effects[effIndex].GetProvidedTargetMask() & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_GAMEOBJECT_MASK))
            return;
        // some spells have no target entries in dbc and they use focus target
        if (focusObject)
            target = focusObject;
        /// @todo there should be a possibility to pass dest target to event script
    }

    SF_LOG_DEBUG("spells", "Spell ScriptStart %u for spellid %u in EffectSendEvent ", m_spellInfo->Effects[effIndex].MiscValue, m_spellInfo->Id);

    if (ZoneScript* zoneScript = m_caster->GetZoneScript())
        zoneScript->ProcessEvent(target, m_spellInfo->Effects[effIndex].MiscValue);
    else if (InstanceScript* instanceScript = m_caster->GetInstanceScript())    // needed in case Player is the caster
        instanceScript->ProcessEvent(target, m_spellInfo->Effects[effIndex].MiscValue);

    m_caster->GetMap()->ScriptsStart(sEventScripts, m_spellInfo->Effects[effIndex].MiscValue, m_caster, target);
}
