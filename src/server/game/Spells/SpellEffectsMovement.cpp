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
#include "SpellMgr.h"
#include "SpellMovementMetadata.h"
#include "MovementTypedefs.h"
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

void Spell::EffectJump(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
        return;

    if (m_caster->IsInFlight())
        return;

    if (!unitTarget)
        return;

    float x, y, z;
    unitTarget->GetContactPoint(m_caster, x, y, z, CONTACT_DISTANCE);

    // Horizontal distance drives the leap arc; CalculateJumpSpeeds floors very short
    // jumps so near-vertical pounces still get a valid spline velocity.
    float speedXY, speedZ;
    CalculateJumpSpeeds(effIndex, m_caster->GetExactDist2d(x, y), speedXY, speedZ);
    uint32 arrivalSpellId = m_spellInfo->Effects[effIndex].TriggerSpell;
    if (!arrivalSpellId)
        arrivalSpellId = Skyfire::Spells::GetJumpArrivalSpellId(m_spellInfo->Id);
    m_caster->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ, EVENT_JUMP, arrivalSpellId);
}

void Spell::EffectJumpDest(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH)
        return;

    if (m_caster->IsInFlight())
        return;

    if (!m_targets.HasDst())
        return;

    // Init dest coordinates
    float x, y, z;
    destTarget->GetPosition(x, y, z);

    uint32 arrivalSpellId = m_spellInfo->Effects[effIndex].TriggerSpell;
    if (!arrivalSpellId)
        arrivalSpellId = Skyfire::Spells::GetJumpArrivalSpellId(m_spellInfo->Id);

    if (Skyfire::Spells::JumpDestOverride const* jumpDestOverride =
        Skyfire::Spells::GetJumpDestOverride(m_spellInfo->Id))
    {
        m_caster->GetMotionMaster()->MoveJump(x, y, z + jumpDestOverride->ZOffset,
            jumpDestOverride->SpeedXY, jumpDestOverride->SpeedZ, EVENT_JUMP, arrivalSpellId);
        return;
    }

    float speedXY, speedZ;
    CalculateJumpSpeeds(effIndex, m_caster->GetExactDist2d(x, y), speedXY, speedZ);
    m_caster->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ, EVENT_JUMP, arrivalSpellId);
}

void Spell::CalculateJumpSpeeds(uint8 i, float dist, float& speedXY, float& speedZ)
{
    // MiscValue / MiscValueB are min/max parabolic height (tenths of yards), not speedZ.
    // Treating MiscValue as speedZ made leaps like Heroic Leap look like a flat charge.
    float runSpeed = m_caster->IsControlledByPlayer() ? playerBaseMoveSpeed[MOVE_RUN] : baseMoveSpeed[MOVE_RUN];
    if (Creature* creature = m_caster->ToCreature())
        runSpeed *= creature->GetCreatureTemplate()->speed_run;

    float multiplier = m_spellInfo->Effects[i].ValueMultiplier;
    if (multiplier <= 0.0f)
        multiplier = 1.0f;

    speedXY = std::min(runSpeed * 3.0f * multiplier, std::max(28.0f, m_caster->GetSpeed(MOVE_RUN) * 4.0f));

    // Keep above the spline validation minimum (velocity > 0.1f) for very short jumps.
    if (speedXY < 2.0f)
        speedXY = 2.0f;

    if (dist < 0.1f)
        dist = 0.1f;

    float duration = dist / speedXY;
    float durationSqr = duration * duration;
    float minHeight = m_spellInfo->Effects[i].MiscValue ? m_spellInfo->Effects[i].MiscValue / 10.0f : 0.5f;
    float maxHeight = m_spellInfo->Effects[i].MiscValueB ? m_spellInfo->Effects[i].MiscValueB / 10.0f : 1000.0f;
    float height;
    if (durationSqr < minHeight * 8.0f / float(Movement::gravity))
        height = minHeight;
    else if (durationSqr > maxHeight * 8.0f / float(Movement::gravity))
        height = maxHeight;
    else
        height = float(Movement::gravity) * durationSqr / 8.0f;

    speedZ = std::sqrt(2.0f * float(Movement::gravity) * height);
}

void Spell::EffectTeleportUnits(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->IsInFlight())
        return;

    // If not exist data for dest location - return
    if (!m_targets.HasDst())
    {
        SF_LOG_ERROR("spells", "Spell::EffectTeleportUnits - does not have destination for spellId %u.", m_spellInfo->Id);
        return;
    }

    // Init dest coordinates
    uint32 mapid = destTarget->GetMapId();
    if (mapid == MAPID_INVALID)
        mapid = unitTarget->GetMapId();

    // Same-map spell teleports (Shadowstep, Blink, etc.) must not CombatStop —
    // that fires SMSG_CANCEL_COMBAT between SPELL_GO and MOVE_TELEPORT and drops
    // the departure smoke/visual for observers.
    uint32 options = 0;
    if (mapid == unitTarget->GetMapId())
        options |= TELE_TO_NOT_LEAVE_COMBAT;

    float x, y, z, orientation;
    destTarget->GetPosition(x, y, z, orientation);
    if (!orientation && m_targets.GetUnitTarget())
        orientation = m_targets.GetUnitTarget()->GetOrientation();
    SF_LOG_DEBUG("spells", "Spell::EffectTeleportUnits - teleport unit to %u %f %f %f %f\n", mapid, x, y, z, orientation);

    if (unitTarget->GetTypeId() == TypeID::TYPEID_PLAYER)
    {
        if (unitTarget == m_caster)
            options |= TELE_TO_SPELL;
        unitTarget->ToPlayer()->TeleportTo(mapid, x, y, z, orientation, options);
    }
    else if (mapid == unitTarget->GetMapId())
        unitTarget->NearTeleportTo(x, y, z, orientation, unitTarget == m_caster);
    else
    {
        SF_LOG_ERROR("spells", "Spell::EffectTeleportUnits - spellId %u attempted to teleport creature to a different map.", m_spellInfo->Id);
        return;
    }

    if (Skyfire::Spells::TeleportPostEffect const* teleportPostEffect =
        Skyfire::Spells::GetTeleportPostEffect(m_spellInfo->Id))
    {
        switch (teleportPostEffect->Kind)
        {
            case Skyfire::Spells::TELEPORT_POST_EFFECT_EVERLOOK_RIPPER:
            {
                uint32 spellId = Skyfire::Spells::GetEverlookRipperPostEffectSpellId(
                    std::rand() % teleportPostEffect->RandomEffectCount);
                if (spellId)
                    m_caster->CastSpell(m_caster, spellId, true);
                return;
            }
            case Skyfire::Spells::TELEPORT_POST_EFFECT_TOSHLEYS_TRANSPORTER:
            {
                if (roll_chance_i(teleportPostEffect->SuccessChancePct))
                {
                    uint32 randomEffect = std::rand() % teleportPostEffect->RandomEffectCount + 1;
                    uint32 spellId = 0;
                    if (randomEffect == 5)
                    {
                        if (Player* player = m_caster->ToPlayer())
                            spellId = Skyfire::Spells::GetToshleysTransporterPostEffectSpellId(
                                randomEffect, player->GetTeam());
                    }
                    else
                        spellId = Skyfire::Spells::GetToshleysTransporterPostEffectSpellId(randomEffect, 0);

                    if (spellId)
                        m_caster->CastSpell(m_caster, spellId, true);
                }
                return;
            }
            case Skyfire::Spells::TELEPORT_POST_EFFECT_AREA52_RIPPER:
            {
                if (roll_chance_i(teleportPostEffect->SuccessChancePct))
                {
                    uint32 randomEffect = std::rand() % teleportPostEffect->RandomEffectCount + 1;
                    uint32 spellId = 0;
                    if (randomEffect == 4)
                    {
                        if (Player* player = m_caster->ToPlayer())
                            spellId = Skyfire::Spells::GetArea52RipperPostEffectSpellId(
                                randomEffect, player->GetTeam());
                    }
                    else
                        spellId = Skyfire::Spells::GetArea52RipperPostEffectSpellId(randomEffect, 0);

                    if (spellId)
                        m_caster->CastSpell(m_caster, spellId, true);
                }
                return;
            }
            default:
                return;
        }
    }
}

void Spell::EffectPull(SpellEffIndex effIndex)
{
    /// @todo create a proper pull towards distract spell center for distract
    EffectNULL(effIndex);
}

void Spell::EffectDistract(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    // Check for possible target
    if (!unitTarget || unitTarget->IsInCombat())
        return;

    // target must be OK to do this
    if (unitTarget->HasUnitState(UNIT_STATE_CONFUSED | UNIT_STATE_STUNNED | UNIT_STATE_FLEEING))
        return;

    unitTarget->SetFacingTo(unitTarget->GetAngle(destTarget));
    unitTarget->ClearUnitState(UNIT_STATE_MOVING);

    if (unitTarget->GetTypeId() == TypeID::TYPEID_UNIT)
        unitTarget->GetMotionMaster()->MoveDistract(damage * IN_MILLISECONDS);
}

void Spell::EffectAddFarsight(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    float radius = m_spellInfo->Effects[effIndex].CalcRadius();
    int32 duration = m_spellInfo->GetDuration();
    // Caster not in world, might be spell triggered from aura removal
    if (!m_caster->IsInWorld())
        return;

    DynamicObject* dynObj = new DynamicObject(true);
    if (!dynObj->CreateDynamicObject(sObjectMgr->GenerateLowGuid(HIGHGUID_DYNAMICOBJECT), m_caster, m_spellInfo, *destTarget, radius, DYNAMIC_OBJECT_FARSIGHT_FOCUS))
    {
        delete dynObj;
        return;
    }

    dynObj->SetDuration(duration);
    dynObj->SetCasterViewpoint();
}

void Spell::EffectTeleUnitsFaceCaster(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (unitTarget->IsInFlight())
        return;

    float dis = m_spellInfo->Effects[effIndex].CalcRadius(m_caster);

    float fx, fy, fz;
    m_caster->GetClosePoint(fx, fy, fz, unitTarget->GetObjectSize(), dis);

    unitTarget->NearTeleportTo(fx, fy, fz, -m_caster->GetOrientation(), unitTarget == m_caster);
}

void Spell::EffectStuck(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CAST_UNSTUCK))
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    SF_LOG_DEBUG("spells", "Spell Effect: Stuck");
    SF_LOG_INFO("spells", "Player %s (guid %u) used auto-unstuck future at map %u (%f, %f, %f)", player->GetName().c_str(), player->GetGUIDLow(), player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());

    if (player->IsInFlight())
        return;

    player->TeleportTo(player->GetStartPosition(), TELE_TO_SPELL);
    // homebind location is loaded always
    // target->TeleportTo(target->m_homebindMapId, target->m_homebindX, target->m_homebindY, target->m_homebindZ, target->GetOrientation(), (m_caster == m_caster ? TELE_TO_SPELL : 0));

    // Stuck spell trigger Hearthstone cooldown
    SpellInfo const* spellInfo =
        sSpellMgr->GetSpellInfo(Skyfire::Spells::GetStuckHearthstoneCooldownSpellId());
    if (!spellInfo)
        return;
    Spell spell(player, spellInfo, TRIGGERED_FULL_MASK);
    spell.SendSpellCooldown();
}

void Spell::EffectLeap(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->IsInFlight())
        return;

    if (!m_targets.HasDst())
        return;

    Position pos;
    destTarget->GetPosition(&pos);
    unitTarget->GetFirstCollisionPosition(pos, unitTarget->GetDistance(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() + 2.0f), 0.0f);
    unitTarget->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), unitTarget == m_caster);
}

void Spell::EffectCharge(SpellEffIndex /*effIndex*/)
{
    if (!unitTarget)
        return;

    if (effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
    {
        // Spell is not using explicit target - no generated path
        if (m_preGeneratedPath.GetPathType() == PATHFIND_BLANK)
        {
            Position pos;
            unitTarget->GetContactPoint(m_caster, pos.m_positionX, pos.m_positionY, pos.m_positionZ);
            unitTarget->GetFirstCollisionPosition(pos, unitTarget->GetObjectSize(), unitTarget->GetRelativeAngle(m_caster));
            m_caster->GetMotionMaster()->MoveCharge(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
        }
        else
            m_caster->GetMotionMaster()->MoveCharge(m_preGeneratedPath);
    }

    if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT_TARGET)
    {
        // not all charge effects used in negative spells
        if (!m_spellInfo->IsPositive() && m_caster->GetTypeId() == TypeID::TYPEID_PLAYER)
            m_caster->Attack(unitTarget, true);
    }
}

void Spell::EffectChargeDest(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH)
        return;

    if (m_targets.HasDst())
    {
        Position pos;
        destTarget->GetPosition(&pos);
        float angle = m_caster->GetRelativeAngle(pos.GetPositionX(), pos.GetPositionY());
        float dist = m_caster->GetDistance(pos);
        m_caster->GetFirstCollisionPosition(pos, dist, angle);

        m_caster->GetMotionMaster()->MoveCharge(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    }
}

void Spell::EffectKnockBack(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (Creature* creatureTarget = unitTarget->ToCreature())
        if (creatureTarget->isWorldBoss() || creatureTarget->IsDungeonBoss())
            return;

    // Spells with SPELL_EFFECT_KNOCK_BACK (like Thunderstorm) can't knockback target if target has ROOT/STUN
    if (unitTarget->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
        return;

    // Instantly interrupt non melee spells being casted
    if (unitTarget->IsNonMeleeSpellCasted(true))
        unitTarget->InterruptNonMeleeSpells(true);

    float ratio = 0.1f;
    float speedxy = float(m_spellInfo->Effects[effIndex].MiscValue) * ratio;
    float speedz = float(damage) * ratio;
    if (speedxy < 0.1f && speedz < 0.1f)
        return;

    float x, y;
    if (m_spellInfo->Effects[effIndex].Effect == SPELL_EFFECT_KNOCK_BACK_DEST)
    {
        if (m_targets.HasDst())
            destTarget->GetPosition(x, y);
        else
            return;
    }
    else //if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_KNOCK_BACK)
    {
        m_caster->GetPosition(x, y);
    }

    unitTarget->KnockbackFrom(x, y, speedxy, speedz);
}

void Spell::EffectLeapBack(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
        return;

    if (!unitTarget)
        return;

    float speedxy = float(m_spellInfo->Effects[effIndex].MiscValue) / 10;
    float speedz = float(damage / 10);
    //1891: Disengage
    m_caster->JumpTo(speedxy, speedz, m_spellInfo->SpellIconID != 1891);
}

void Spell::EffectSendTaxi(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->ActivateTaxiPathTo(m_spellInfo->Effects[effIndex].MiscValue, m_spellInfo->Id);
}

void Spell::EffectPullTowards(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    float speedZ = (float)(m_spellInfo->Effects[effIndex].CalcValue() / 10);
    float speedXY = (float)(m_spellInfo->Effects[effIndex].MiscValue / 10);
    Position pos;
    if (m_spellInfo->Effects[effIndex].Effect == SPELL_EFFECT_PULL_TOWARDS_DEST)
    {
        if (m_targets.HasDst())
            pos.Relocate(*destTarget);
        else
            return;
    }
    else //if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_PULL_TOWARDS)
    {
        pos.Relocate(m_caster);
    }

    unitTarget->GetMotionMaster()->MoveJump(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), speedXY, speedZ);
}

void Spell::EffectDiscoverTaxi(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;
    uint32 nodeid = m_spellInfo->Effects[effIndex].MiscValue;
    if (sTaxiNodesStore.LookupEntry(nodeid))
        unitTarget->ToPlayer()->GetSession()->SendDiscoverNewTaxiNode(nodeid);
}

void Spell::EffectBind(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();

    WorldLocation homeLoc;
    uint32 areaId = player->GetAreaId();

    if (m_spellInfo->Effects[effIndex].MiscValue)
        areaId = m_spellInfo->Effects[effIndex].MiscValue;

    if (m_targets.HasDst())
        homeLoc.WorldRelocate(*destTarget);
    else
    {
        player->GetPosition(&homeLoc);
        homeLoc.m_mapId = player->GetMapId();
    }

    player->SetHomebind(homeLoc, areaId);

    // binding
    WorldPacket data(SMSG_BIND_POINT_UPDATE, 4 + 4 + 4 + 4 + 4);
    data << float(homeLoc.GetPositionX());
    data << float(homeLoc.GetPositionY());
    data << float(homeLoc.GetPositionZ());
    data << uint32(areaId);
    data << uint32(homeLoc.GetMapId());

    player->SendDirectMessage(&data);

    SF_LOG_DEBUG("spells", "EffectBind: New homebind X: %f, Y: %f, Z: %f, MapId: %u, AreaId: %u",
        homeLoc.GetPositionX(), homeLoc.GetPositionY(), homeLoc.GetPositionZ(), homeLoc.GetMapId(), areaId);

    ObjectGuid guid = m_caster->GetGUID();

    // zone update
    data.Initialize(SMSG_PLAYERBOUND, 1 + 8 + 4);
    data.WriteGuidMask(guid, 2, 4, 0, 3, 6, 7, 5, 1);
    data.WriteGuidMask(guid, 6, 1, 2, 3, 4, 5, 7, 0);
    data << uint32(areaId);
    player->SendDirectMessage(&data);
}
