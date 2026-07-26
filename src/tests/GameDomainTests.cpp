/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Cell.h"
#include "CurrencyFormulas.h"
#include "AreaTableUtils.h"
#include "DBCEnums.h"
#include "GridDefines.h"
#include "LegacyTransportSupport.h"
#include "MapLifecycle.h"
#include "ObjectAccessorLifecycle.h"
#include "PlayerRestState.h"
#include "RuntimeMetrics.h"
#include "SpellCalculations.h"
#include "SpellAuraMetadata.h"
#include "SpellAuraDefines.h"
#include "SpellCombatMetadata.h"
#include "SpellEffectMetadata.h"
#include "SpellItemMetadata.h"
#include "SpellMovementMetadata.h"
#include "SpellSummonMetadata.h"
#include "SpellTargeting.h"
#include "SpellValidation.h"
#include "ThreatCalcHelper.h"
#include "WorldShutdownLifecycle.h"
#include "WorldPacket.h"
#include "WorldStateBuilder.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool ExpectNear(float actual, float expected, float epsilon, char const* message)
    {
        return Expect(std::fabs(actual - expected) <= epsilon, message);
    }

    bool TestCurrencyFormulaBoundaries()
    {
        bool passed = true;

        passed &= Expect(Skyfire::Currency::ConquestRatingCalculator(0) == 1350,
            "Conquest cap below rated threshold should use the base cap");
        passed &= Expect(Skyfire::Currency::ConquestRatingCalculator(1500) == 1350,
            "Conquest cap at rated threshold should use the base cap");
        passed &= Expect(Skyfire::Currency::ConquestRatingCalculator(2000) > 1350,
            "Conquest cap above rated threshold should increase");
        passed &= Expect(Skyfire::Currency::ConquestRatingCalculator(3100) == Skyfire::Currency::ConquestRatingCalculator(3000),
            "Conquest cap should clamp ratings above 3000");
        passed &= Expect(Skyfire::Currency::BgConquestRatingCalculator(1500) == 1650,
            "Battleground conquest cap should apply the 22.2 percent bonus");

        return passed;
    }

    bool TestWorldPacketContainerBehavior()
    {
        bool passed = true;

        WorldPacket packet(SMSG_PONG, 4);
        packet << uint32(0x11223344);

        passed &= Expect(packet.GetOpcode() == SMSG_PONG, "WorldPacket should preserve its opcode");
        passed &= Expect(packet.size() == sizeof(uint32), "WorldPacket should store appended payload bytes");

        packet.rpos(0);
        uint32 value = 0;
        packet >> value;
        passed &= Expect(value == 0x11223344, "WorldPacket should round-trip uint32 payloads");

        packet.SetReceivedOpcode(0x1234);
        WorldPacket copy(packet);
        passed &= Expect(copy.GetOpcode() == SMSG_PONG, "WorldPacket copy should preserve opcode");
        passed &= Expect(copy.GetReceivedOpcode() == 0, "WorldPacket copy should reset received opcode tracking");

        packet.Initialize(CMSG_PING, 1);
        passed &= Expect(packet.GetOpcode() == CMSG_PING, "WorldPacket Initialize should replace opcode");
        passed &= Expect(packet.size() == 0 && packet.rpos() == 0 && packet.wpos() == 0,
            "WorldPacket Initialize should clear payload and positions");

        WorldPacket bitPacket(SMSG_PONG, 1);
        bitPacket.WriteBit(1);
        bitPacket.WriteBit(0);
        bitPacket.WriteBit(1);
        bitPacket.FlushBits();

        passed &= Expect(bitPacket.size() == 1, "WorldPacket bit writes should flush into one byte");
        passed &= Expect(bitPacket.contents()[0] == 0xA0, "WorldPacket bit writes should use high-bit-first packing");

        bitPacket.rpos(0);
        passed &= Expect(bitPacket.ReadBit() == true, "WorldPacket should read first written bit");
        passed &= Expect(bitPacket.ReadBit() == false, "WorldPacket should read second written bit");
        passed &= Expect(bitPacket.ReadBit() == true, "WorldPacket should read third written bit");

        return passed;
    }

    bool TestClientOpcodeTableAcceptsGuildAchievementTracking()
    {
        bool passed = true;

        clientOpcodeTable.InitializeClientTable();
        Opcodes opcode = clientOpcodeTable.GetOpcodeByNumber(0x0CF0);
        OpcodeHandler const* handler = clientOpcodeTable[opcode];

        passed &= Expect(opcode == CMSG_GUILD_SET_ACHIEVEMENT_TRACKING,
            "0x0CF0 should map to CMSG_GUILD_SET_ACHIEVEMENT_TRACKING");
        passed &= Expect(handler != NULL,
            "CMSG_GUILD_SET_ACHIEVEMENT_TRACKING should have an opcode table entry");
        passed &= Expect(handler && handler->Status == STATUS_LOGGEDIN,
            "CMSG_GUILD_SET_ACHIEVEMENT_TRACKING should be accepted while logged in");
        passed &= Expect(handler && handler->Handler != NULL,
            "CMSG_GUILD_SET_ACHIEVEMENT_TRACKING should have an executable handler");

        return passed;
    }

    bool TestServerOpcodeTableEnablesDisplayGameError()
    {
        bool passed = true;

        serverOpcodeTable.InitializeServerTable();
        OpcodeHandler const* handler = serverOpcodeTable[SMSG_DISPLAY_GAME_ERROR];

        passed &= Expect(handler != NULL,
            "SMSG_DISPLAY_GAME_ERROR should have an opcode table entry");
        passed &= Expect(handler && handler->OpcodeNumber == 0x181F,
            "SMSG_DISPLAY_GAME_ERROR should use the 5.4.8 opcode number");
        passed &= Expect(handler && handler->Status == STATUS_NEVER,
            "SMSG_DISPLAY_GAME_ERROR should be enabled as a server-only opcode");

        return passed;
    }

    bool TestWorldStateBuilderPacketLayout()
    {
        bool passed = true;

        WorldStateBuilder builder(870, 4384, 607);
        builder.AppendState(123, 456);
        WorldPacket packet = builder.BuildPacket();

        uint32 mapId = 0;
        uint32 areaId = 0;
        uint32 zoneId = 0;
        packet.rpos(0);
        packet >> mapId >> areaId >> zoneId;

        passed &= Expect(packet.GetOpcode() == SMSG_INIT_WORLD_STATES,
            "WorldStateBuilder should create SMSG_INIT_WORLD_STATES packets");
        passed &= Expect(mapId == 870, "WorldStateBuilder should write map id first");
        passed &= Expect(areaId == 4384, "WorldStateBuilder should write area id before zone id");
        passed &= Expect(zoneId == 607, "WorldStateBuilder should write zone id after area id");

        uint32 stateCount = packet.ReadBits(21);
        passed &= Expect(stateCount == 1, "WorldStateBuilder should write a 21-bit state count");

        uint32 value = 0;
        uint32 state = 0;
        packet >> value >> state;
        passed &= Expect(value == 456, "WorldStateBuilder should write world-state value before id");
        passed &= Expect(state == 123, "WorldStateBuilder should write world-state id after value");

        return passed;
    }

    bool TestThreatSpellModifierRules()
    {
        bool passed = true;

        ThreatCalcHelper::SpellThreatCalculation defaultCalculation =
            ThreatCalcHelper::ApplySpellThreatModifiers(100.0f, 1.0f, false);
        passed &= Expect(defaultCalculation.threat == 100.0f,
            "Threat spell modifier default multiplier should leave threat unchanged");
        passed &= Expect(!defaultCalculation.ignoresUnitModifiers,
            "Non-energize threat should still allow unit threat modifiers");

        ThreatCalcHelper::SpellThreatCalculation percentCalculation =
            ThreatCalcHelper::ApplySpellThreatModifiers(100.0f, 1.25f, false);
        passed &= Expect(percentCalculation.threat == 125.0f,
            "Threat spell modifier percent should scale threat");
        passed &= Expect(!percentCalculation.ignoresUnitModifiers,
            "Percent-only threat should still allow unit threat modifiers");

        ThreatCalcHelper::SpellThreatCalculation energizeCalculation =
            ThreatCalcHelper::ApplySpellThreatModifiers(100.0f, 0.5f, true);
        passed &= Expect(energizeCalculation.threat == 50.0f,
            "Energize threat should still apply spell threat percent modifiers");
        passed &= Expect(energizeCalculation.ignoresUnitModifiers,
            "Energize threat should bypass later unit threat modifiers");

        return passed;
    }

    bool TestSpellValidationMasks()
    {
        bool passed = true;

        passed &= Expect(Skyfire::Spells::IsMechanicRepresentable(MECHANIC_STUN),
            "Stun should be representable in a 32-bit spell mechanic mask");
        passed &= Expect(!Skyfire::Spells::IsMechanicRepresentable(MECHANIC_NONE),
            "Mechanic none should not set a mechanic mask bit");
        passed &= Expect(!Skyfire::Spells::IsMechanicRepresentable(MECHANIC_WOUNDED),
            "Mechanics outside the 32-bit mask should be rejected");

        uint32 stunMask = Skyfire::Spells::GetMechanicMask(MECHANIC_STUN);
        passed &= Expect(stunMask == (1u << MECHANIC_STUN),
            "Mechanic mask should set the mechanic bit");
        passed &= Expect(Skyfire::Spells::GetMechanicMask(MECHANIC_NONE) == 0,
            "Mechanic none should build an empty mask");
        passed &= Expect(Skyfire::Spells::GetMechanicMask(MECHANIC_WOUNDED) == 0,
            "Out-of-range mechanics should build an empty mask");
        passed &= Expect(Skyfire::Spells::HasMechanic(stunMask, MECHANIC_STUN),
            "Mechanic mask lookup should find an active mechanic");
        passed &= Expect(!Skyfire::Spells::HasMechanic(stunMask, MECHANIC_FEAR),
            "Mechanic mask lookup should reject inactive mechanics");
        passed &= Expect(Skyfire::Spells::BuildMechanicMask({ MECHANIC_SNARE, MECHANIC_ROOT, MECHANIC_WOUNDED }) ==
            ((1u << MECHANIC_SNARE) | (1u << MECHANIC_ROOT)),
            "Mechanic mask builder should combine representable mechanics only");
        passed &= Expect(Skyfire::Spells::HasAnyMechanic(stunMask, { MECHANIC_FEAR, MECHANIC_STUN }),
            "Mechanic mask lookup should find any requested active mechanic");

        passed &= Expect(Skyfire::Spells::GetEffectIndexMask(0) == 1u,
            "Effect index zero should map to the lowest mask bit");
        passed &= Expect(Skyfire::Spells::GetEffectIndexMask(31) == 0x80000000u,
            "Effect index thirty-one should map to the highest mask bit");
        passed &= Expect(Skyfire::Spells::GetEffectIndexMask(32) == 0,
            "Effect indexes outside the 32-bit mask should be rejected");

        passed &= Expect(Skyfire::Spells::GetDispelMask(DISPEL_MAGIC) == (1u << DISPEL_MAGIC),
            "Specific dispel types should map to their dispel bit");
        passed &= Expect(Skyfire::Spells::GetDispelMask(DISPEL_ALL) == DISPEL_ALL_MASK,
            "Dispel all should expand to all removable dispel types");
        passed &= Expect(Skyfire::Spells::GetDispelMask(DispelType(32)) == 0,
            "Out-of-range dispel types should build an empty mask");

        passed &= Expect(Skyfire::Spells::ClassifySpellDispatchHandler(Skyfire::Spells::SPELL_DISPATCH_HANDLER_REAL) ==
            Skyfire::Spells::SPELL_DISPATCH_SUPPORTED,
            "Real dispatch handlers should be classified as supported");
        passed &= Expect(Skyfire::Spells::ClassifySpellDispatchHandler(Skyfire::Spells::SPELL_DISPATCH_HANDLER_UNUSED) ==
            Skyfire::Spells::SPELL_DISPATCH_INTENTIONALLY_UNUSED,
            "Unused dispatch handlers should be classified as intentionally unused");
        passed &= Expect(Skyfire::Spells::ClassifySpellDispatchHandler(Skyfire::Spells::SPELL_DISPATCH_HANDLER_NO_IMMEDIATE_EFFECT) ==
            Skyfire::Spells::SPELL_DISPATCH_NO_IMMEDIATE_EFFECT,
            "No-immediate-effect aura handlers should be classified separately from missing handlers");
        passed &= Expect(Skyfire::Spells::ClassifySpellDispatchHandler(Skyfire::Spells::SPELL_DISPATCH_HANDLER_NULL) ==
            Skyfire::Spells::SPELL_DISPATCH_MISSING_OR_UNSUPPORTED,
            "Null dispatch handlers should be classified as missing or unsupported");
        passed &= Expect(Skyfire::Spells::GetSpellDispatchValidationStatusName(Skyfire::Spells::SPELL_DISPATCH_SUPPORTED) == std::string("supported"),
            "Supported dispatch status should have a stable diagnostics label");
        passed &= Expect(Skyfire::Spells::GetSpellDispatchValidationStatusName(Skyfire::Spells::SPELL_DISPATCH_INTENTIONALLY_UNUSED) == std::string("intentionally unused"),
            "Intentionally unused dispatch status should have a stable diagnostics label");
        passed &= Expect(Skyfire::Spells::GetSpellDispatchValidationStatusName(Skyfire::Spells::SPELL_DISPATCH_NO_IMMEDIATE_EFFECT) == std::string("no immediate effect"),
            "No-immediate-effect dispatch status should have a stable diagnostics label");
        passed &= Expect(Skyfire::Spells::GetSpellDispatchValidationStatusName(Skyfire::Spells::SPELL_DISPATCH_MISSING_OR_UNSUPPORTED) == std::string("missing or unsupported"),
            "Missing dispatch status should have a stable diagnostics label");

        return passed;
    }

    bool TestSpellTargetingRules()
    {
        bool passed = true;

        passed &= Expect(GetTargetFlagMask(TARGET_OBJECT_TYPE_NONE) == TARGET_FLAG_NONE,
            "Empty target object type should not require an explicit target flag");
        passed &= Expect(GetTargetFlagMask(TARGET_OBJECT_TYPE_UNIT_AND_DEST) ==
            (TARGET_FLAG_DEST_LOCATION | TARGET_FLAG_UNIT),
            "Unit-and-destination object type should require both unit and destination flags");
        passed &= Expect(GetTargetFlagMask(TARGET_OBJECT_TYPE_CORPSE) ==
            (TARGET_FLAG_CORPSE_ALLY | TARGET_FLAG_CORPSE_ENEMY),
            "Generic corpse object type should allow both corpse target flags");
        passed &= Expect(GetTargetFlagMask(TARGET_OBJECT_TYPE_GOBJ_ITEM) == TARGET_FLAG_GAMEOBJECT_ITEM,
            "Gameobject item object type should map to the gameobject item flag");

        SpellImplicitTargetInfo enemyTarget(TARGET_UNIT_TARGET_ENEMY);
        passed &= Expect(!enemyTarget.IsArea(), "Single enemy unit target should not be area-targeted");
        passed &= Expect(enemyTarget.GetSelectionCategory() == TARGET_SELECT_CATEGORY_DEFAULT,
            "Single enemy unit target should use default target selection");
        passed &= Expect(enemyTarget.GetReferenceType() == TARGET_REFERENCE_TYPE_TARGET,
            "Single enemy unit target should reference the selected target");
        passed &= Expect(enemyTarget.GetObjectType() == TARGET_OBJECT_TYPE_UNIT,
            "Single enemy unit target should use unit object type");
        passed &= Expect(enemyTarget.GetCheckType() == TARGET_CHECK_ENEMY,
            "Single enemy unit target should require enemy target checks");

        bool srcSet = false;
        bool dstSet = false;
        passed &= Expect(enemyTarget.GetExplicitTargetMask(srcSet, dstSet) == TARGET_FLAG_UNIT_ENEMY,
            "Single enemy unit target should request the enemy-unit explicit target flag");
        passed &= Expect(!srcSet && !dstSet,
            "Single enemy unit target should not mark source or destination locations as consumed");

        SpellImplicitTargetInfo coneTarget(TARGET_UNIT_CONE_ENEMY_54);
        passed &= Expect(coneTarget.IsArea(), "Cone enemy target should be area-targeted");
        passed &= Expect(coneTarget.GetDirectionType() == TARGET_DIR_FRONT,
            "Cone enemy target should face forward");
        passed &= ExpectNear(coneTarget.CalcDirectionAngle(), 0.0f, 0.001f,
            "Forward direction target should have zero angle");

        SpellImplicitTargetInfo rightDest(TARGET_DEST_CASTER_RIGHT);
        passed &= ExpectNear(rightDest.CalcDirectionAngle(), static_cast<float>(-M_PI / 2), 0.001f,
            "Right caster destination should resolve to negative half-pi");

        SpellImplicitTargetInfo frontLeftDest(TARGET_DEST_CASTER_FRONT_LEFT);
        passed &= ExpectNear(frontLeftDest.CalcDirectionAngle(), static_cast<float>(M_PI / 4), 0.001f,
            "Front-left caster destination should resolve to quarter-pi");

        SpellImplicitTargetInfo randomDest(TARGET_DEST_CASTER_RANDOM);
        passed &= ExpectNear(randomDest.CalcDirectionAngle(), static_cast<float>(M_PI / 2), 0.001f,
            "Random caster destination should scale the normalized random value across a full circle");

        SpellImplicitTargetInfo sourceMarker(TARGET_SRC_CASTER);
        srcSet = false;
        dstSet = false;
        passed &= Expect(sourceMarker.GetExplicitTargetMask(srcSet, dstSet) == TARGET_FLAG_NONE,
            "Caster source marker should not request an explicit target flag");
        passed &= Expect(srcSet && !dstSet,
            "Caster source marker should mark the source location as consumed");

        SpellImplicitTargetInfo trajectoryTarget(TARGET_DEST_TRAJ);
        srcSet = false;
        dstSet = false;
        passed &= Expect(trajectoryTarget.GetExplicitTargetMask(srcSet, dstSet) ==
            (TARGET_FLAG_SOURCE_LOCATION | TARGET_FLAG_DEST_LOCATION),
            "Trajectory target should request both source and destination when neither is present");
        passed &= Expect(!srcSet && dstSet,
            "Trajectory target should mark destination consumption through its object type");

        srcSet = true;
        dstSet = false;
        passed &= Expect(trajectoryTarget.GetExplicitTargetMask(srcSet, dstSet) == TARGET_FLAG_DEST_LOCATION,
            "Trajectory target should only request destination when source is already present");

        return passed;
    }

    bool TestSpellCalculationRules()
    {
        bool passed = true;

        passed &= Expect(Skyfire::Spells::CalculateBaseCastTime(0, 1000, 3000, 5, 1500) == 1500,
            "Cast time without a level should use the cast-time entry fallback");
        passed &= Expect(Skyfire::Spells::CalculateBaseCastTime(3, 1000, 3000, 5, 1500) == 2000,
            "Cast time below max level should interpolate between min and max");
        passed &= Expect(Skyfire::Spells::CalculateBaseCastTime(5, 1000, 3000, 5, 1500) == 3000,
            "Cast time at max level should use the max cast time");
        passed &= Expect(Skyfire::Spells::CalculateBaseCastTime(8, 1000, 3000, 5, 1500) == 3000,
            "Cast time above max level should use the max cast time");
        passed &= Expect(Skyfire::Spells::CalculateBaseCastTime(1, 0, 0, 0, 0) == 0,
            "Missing cast-time data should produce no cast time");

        passed &= Expect(Skyfire::Spells::FinalizeCastTime(1500, false) == 1500,
            "Final cast time should preserve positive cast time without ammo delay");
        passed &= Expect(Skyfire::Spells::FinalizeCastTime(1500, true) == 2000,
            "Final cast time should add ranged ammo delay when requested");
        passed &= Expect(Skyfire::Spells::FinalizeCastTime(-50, false) == 0,
            "Final cast time should clamp negative cast time to zero");

        passed &= Expect(Skyfire::Spells::NormalizeDurationValue(-1) == -1,
            "Permanent spell duration should remain permanent");
        passed &= Expect(Skyfire::Spells::NormalizeDurationValue(-5000) == 5000,
            "Negative non-permanent spell duration should be made positive");
        passed &= Expect(Skyfire::Spells::CalculateDuration(false, 12000) == 0,
            "Missing spell duration entry should produce zero duration");
        passed &= Expect(Skyfire::Spells::CalculateDuration(true, -12000) == 12000,
            "Present spell duration entry should normalize duration values");

        Skyfire::Spells::SpellTickEffectInfo noPeriodicEffects[] =
        {
            { SPELL_EFFECT_SCHOOL_DAMAGE, 0, 0 }
        };
        passed &= Expect(Skyfire::Spells::CalculateMaxTicks(0, noPeriodicEffects, 1) == 1,
            "Zero-duration spell should report a single max tick");
        passed &= Expect(Skyfire::Spells::CalculateMaxTicks(12000, noPeriodicEffects, 1) == 6,
            "Spell without counted periodic aura effects should use the default max tick count");

        Skyfire::Spells::SpellTickEffectInfo periodicEffects[] =
        {
            { SPELL_EFFECT_APPLY_AURA, SPELL_AURA_PERIODIC_DAMAGE, 3000 }
        };
        passed &= Expect(Skyfire::Spells::CalculateMaxTicks(12000, periodicEffects, 1) == 4,
            "Periodic damage aura should derive max ticks from duration and tick count");
        passed &= Expect(Skyfire::Spells::CalculateMaxTicks(45000, periodicEffects, 1) == 10,
            "Long periodic spell duration should clamp to thirty seconds for max ticks");

        Skyfire::Spells::SpellTickEffectInfo zeroTickPeriodicEffects[] =
        {
            { SPELL_EFFECT_APPLY_AURA, SPELL_AURA_PERIODIC_HEAL, 0 }
        };
        passed &= Expect(Skyfire::Spells::CalculateMaxTicks(12000, zeroTickPeriodicEffects, 1) == 6,
            "Periodic aura without tick count should fall back to the default max tick count");

        passed &= Expect(Skyfire::Spells::CalculateEffectBaseValue(42, 0) == 42,
            "Effect base value should stay unchanged without die sides");
        passed &= Expect(Skyfire::Spells::CalculateEffectBaseValue(42, 1) == 41,
            "Effect base value should subtract one when die sides are present");
        passed &= Expect(Skyfire::Spells::CalculateEffectBaseValue(42, -5) == 41,
            "Effect base value should treat any non-zero die sides as randomized");

        Skyfire::Spells::SpellRadiusCalculationData noRadius = { false, 0.0f, 0.0f, 0.0f, false, 0.0f, false, 0 };
        passed &= ExpectNear(Skyfire::Spells::CalculateRadius(noRadius), 0.0f, 0.001f,
            "Missing radius data should produce zero radius");

        Skyfire::Spells::SpellRadiusCalculationData maxRadiusOnly = { false, 0.0f, 0.0f, 0.0f, true, 12.5f, false, 0 };
        passed &= ExpectNear(Skyfire::Spells::CalculateRadius(maxRadiusOnly), 12.5f, 0.001f,
            "Missing radius with max-radius data should use max radius minimum");

        Skyfire::Spells::SpellRadiusCalculationData radiusWithoutCaster = { true, 8.0f, 2.0f, 20.0f, false, 0.0f, false, 0 };
        passed &= ExpectNear(Skyfire::Spells::CalculateRadius(radiusWithoutCaster), 8.0f, 0.001f,
            "Radius without caster should use minimum radius");

        Skyfire::Spells::SpellRadiusCalculationData radiusWithCaster = { true, 8.0f, 2.0f, 20.0f, false, 0.0f, true, 5 };
        passed &= ExpectNear(Skyfire::Spells::CalculateRadius(radiusWithCaster), 18.0f, 0.001f,
            "Radius with caster should scale by caster level");

        Skyfire::Spells::SpellRadiusCalculationData clampedRadius = { true, 8.0f, 4.0f, 20.0f, false, 0.0f, true, 5 };
        passed &= ExpectNear(Skyfire::Spells::CalculateRadius(clampedRadius), 20.0f, 0.001f,
            "Radius with caster should clamp at radius maximum");

        passed &= ExpectNear(Skyfire::Spells::SelectSpellRange(false, 5.0f, 30.0f, true), 0.0f, 0.001f,
            "Missing range data should produce zero range");
        passed &= ExpectNear(Skyfire::Spells::SelectSpellRange(true, 5.0f, 30.0f, true), 5.0f, 0.001f,
            "Positive range selection should use friendly range");
        passed &= ExpectNear(Skyfire::Spells::SelectSpellRange(true, 5.0f, 30.0f, false), 30.0f, 0.001f,
            "Hostile range selection should use hostile range");

        passed &= Expect(Skyfire::Spells::CalculateRecoveryTime(1000, 500) == 1000,
            "Recovery time should prefer direct recovery when larger");
        passed &= Expect(Skyfire::Spells::CalculateRecoveryTime(100, 500) == 500,
            "Recovery time should prefer category recovery when larger");

        Skyfire::Spells::SpellPowerCostCalculationData fixedPowerCost =
        {
            100,
            5,
            0.0f,
            0.0f,
            POWER_MANA,
            5000,
            2000
        };
        Skyfire::Spells::SpellPowerCostCalculationResult powerCostResult = Skyfire::Spells::CalculateSpellPowerCosts(fixedPowerCost);
        passed &= Expect(powerCostResult.Status == Skyfire::Spells::SPELL_POWER_COST_OK,
            "Fixed spell power cost should be accepted");
        passed &= Expect(powerCostResult.PowerCost == 100,
            "Fixed spell power cost should preserve base cost");
        passed &= Expect(powerCostResult.PeriodicPowerCost == 5,
            "Fixed spell power cost should preserve base periodic cost");

        Skyfire::Spells::SpellPowerCostCalculationData manaPercentCost =
        {
            100,
            5,
            10.0f,
            5.0f,
            POWER_MANA,
            5000,
            2000
        };
        powerCostResult = Skyfire::Spells::CalculateSpellPowerCosts(manaPercentCost);
        passed &= Expect(powerCostResult.PowerCost == 300,
            "Mana percentage spell power cost should use max power");
        passed &= Expect(powerCostResult.PeriodicPowerCost == 105,
            "Channel percentage spell power cost should use max power");

        Skyfire::Spells::SpellPowerCostCalculationData healthPercentCost =
        {
            50,
            0,
            20.0f,
            0.0f,
            POWER_HEALTH,
            5000,
            2000
        };
        powerCostResult = Skyfire::Spells::CalculateSpellPowerCosts(healthPercentCost);
        passed &= Expect(powerCostResult.PowerCost == 1050,
            "Health spell power cost should use created health for percentage cost");

        Skyfire::Spells::SpellPowerCostCalculationData demonicFuryCost = manaPercentCost;
        demonicFuryCost.PowerType = POWER_DEMONIC_FURY;
        powerCostResult = Skyfire::Spells::CalculateSpellPowerCosts(demonicFuryCost);
        passed &= Expect(powerCostResult.PowerCost == 300,
            "Demonic fury percentage spell power cost should use max power");

        Skyfire::Spells::SpellPowerCostCalculationData unsupportedRunicCost = manaPercentCost;
        unsupportedRunicCost.PowerType = POWER_RUNIC_POWER;
        powerCostResult = Skyfire::Spells::CalculateSpellPowerCosts(unsupportedRunicCost);
        passed &= Expect(powerCostResult.Status == Skyfire::Spells::SPELL_POWER_COST_UNSUPPORTED_POWER_TYPE,
            "Runic power percentage cost should report unsupported status");
        passed &= Expect(powerCostResult.PowerCost == 100,
            "Unsupported runic power percentage cost should leave base cost unchanged");

        Skyfire::Spells::SpellPowerCostCalculationData unknownPowerCost = manaPercentCost;
        unknownPowerCost.PowerType = MAX_POWERS + 7;
        powerCostResult = Skyfire::Spells::CalculateSpellPowerCosts(unknownPowerCost);
        passed &= Expect(powerCostResult.Status == Skyfire::Spells::SPELL_POWER_COST_UNKNOWN_POWER_TYPE,
            "Unknown percentage power type should report unknown status");
        passed &= Expect(powerCostResult.PowerCost == 0,
            "Unknown percentage power type should zero the base cost");

        return passed;
    }

    bool TestSpellAuraMetadataRules()
    {
        bool passed = true;

        passed &= Expect(Skyfire::Spells::GetQuestTameFinalSpellId(19548) == 19597,
            "Quest tame aura should map to its final credit spell");
        passed &= Expect(Skyfire::Spells::GetQuestTameFinalSpellId(30654) == 30652,
            "Later quest tame aura should map to its final credit spell");
        passed &= Expect(Skyfire::Spells::GetQuestTameFinalSpellId(30102) == 30103,
            "Blood elf quest tame aura should map to its final credit spell");
        passed &= Expect(Skyfire::Spells::GetQuestTameFinalSpellId(999999) == 0,
            "Unknown quest tame aura should not resolve to a final credit spell");

        Skyfire::Spells::ChampioningAuraInfo const* argentChampion =
            Skyfire::Spells::GetChampioningAuraInfo(57819);
        passed &= Expect(argentChampion && argentChampion->FactionId == 1106 && argentChampion->ChampioningType == 1,
            "Argent Champion aura should resolve to Argent Crusade championing metadata");

        Skyfire::Spells::ChampioningAuraInfo const* wildhammerChampion =
            Skyfire::Spells::GetChampioningAuraInfo(93368);
        passed &= Expect(wildhammerChampion && wildhammerChampion->FactionId == 1174 && wildhammerChampion->ChampioningType == 2,
            "Wildhammer champion aura should resolve to Cataclysm faction championing metadata");

        Skyfire::Spells::ChampioningAuraInfo const* stormwindChampion =
            Skyfire::Spells::GetChampioningAuraInfo(93795);
        passed &= Expect(stormwindChampion && stormwindChampion->FactionId == 72 && stormwindChampion->ChampioningType == 4,
            "Stormwind champion aura should resolve to capital city championing metadata");

        passed &= Expect(!Skyfire::Spells::GetChampioningAuraInfo(999999),
            "Unknown championing aura should not resolve metadata");

        uint32 shapeshiftSpells[Skyfire::Spells::MAX_SHAPESHIFT_BOOST_SPELLS] = { 0, 0, 0, 0 };
        uint8 catSpellCount = Skyfire::Spells::GetShapeshiftBoostSpellIds(1, shapeshiftSpells,
            Skyfire::Spells::MAX_SHAPESHIFT_BOOST_SPELLS);
        passed &= Expect(catSpellCount == 4,
            "Cat Form should resolve four shapeshift boost spells");
        passed &= Expect(shapeshiftSpells[0] == 3025 && shapeshiftSpells[1] == 48629 &&
            shapeshiftSpells[2] == 106840 && shapeshiftSpells[3] == 113636,
            "Cat Form shapeshift boost spells should match the current metadata");

        shapeshiftSpells[0] = shapeshiftSpells[1] = shapeshiftSpells[2] = shapeshiftSpells[3] = 0;
        uint8 spiritSpellCount = Skyfire::Spells::GetShapeshiftBoostSpellIds(32, shapeshiftSpells,
            Skyfire::Spells::MAX_SHAPESHIFT_BOOST_SPELLS);
        passed &= Expect(spiritSpellCount == 2,
            "Spirit of Redemption should resolve two ordered shapeshift boost spells");
        passed &= Expect(shapeshiftSpells[0] == 27792 && shapeshiftSpells[1] == 27795,
            "Spirit of Redemption shapeshift boost spells should preserve removal order");

        shapeshiftSpells[0] = shapeshiftSpells[1] = shapeshiftSpells[2] = shapeshiftSpells[3] = 77;
        uint8 unknownFormSpellCount = Skyfire::Spells::GetShapeshiftBoostSpellIds(250, shapeshiftSpells,
            Skyfire::Spells::MAX_SHAPESHIFT_BOOST_SPELLS);
        passed &= Expect(unknownFormSpellCount == 0,
            "Unknown shapeshift form should not resolve boost spells");
        passed &= Expect(shapeshiftSpells[0] == 77 && shapeshiftSpells[1] == 77 &&
            shapeshiftSpells[2] == 77 && shapeshiftSpells[3] == 77,
            "Unknown shapeshift form should leave the output buffer unchanged");

        passed &= Expect(Skyfire::Spells::GetNurturingInstinctTriggeredSpellId(33873) == 47180,
            "Nurturing Instinct aura should resolve its triggered spell");
        passed &= Expect(Skyfire::Spells::GetNurturingInstinctTriggeredSpellId(999999) == 0,
            "Unknown Nurturing Instinct aura should not resolve a triggered spell");

        passed &= Expect(Skyfire::Spells::TransformDisplayRequiresPlayerRace(16739),
            "Orb of Deception transform display should require player race metadata");
        passed &= Expect(Skyfire::Spells::GetTransformDisplayId(16739, 1, 0) == 10137,
            "Orb of Deception should map male human targets to the configured display");
        passed &= Expect(Skyfire::Spells::GetTransformDisplayId(16739, 10, 1) == 17830,
            "Orb of Deception should map female blood elf targets to the configured display");
        passed &= Expect(Skyfire::Spells::GetTransformDisplayId(50517, 2, 0) == 25039,
            "Dread Corsair should map male orc targets to the configured display");
        passed &= Expect(Skyfire::Spells::GetTransformDisplayId(51926, 4, 1) == 25049,
            "Corsair Costume should map female night elf targets to the configured display");
        passed &= Expect(!Skyfire::Spells::TransformDisplayRequiresPlayerRace(42365),
            "Murloc costume transform display should not require player race metadata");
        passed &= Expect(Skyfire::Spells::GetTransformDisplayId(42365, 0, 0) == 21723,
            "Murloc costume should resolve its display id without race metadata");
        passed &= Expect(Skyfire::Spells::GetTransformDisplayId(65386, 0, 1) == 29204,
            "Honor the Dead should resolve its female display id");
        passed &= Expect(Skyfire::Spells::GetTransformDisplayId(999999, 1, 0) == 0,
            "Unknown transform aura should not resolve a display id");
        passed &= Expect(Skyfire::Spells::GetUnknownTransformFallbackDisplayId() == 16358,
            "Unknown transform fallback display id should match the current pink pig display");
        passed &= Expect(Skyfire::Spells::GetGlyphOfPenguinAuraSpellId() == 52648,
            "Glyph of the Penguin aura spell id should be named in aura metadata");
        passed &= Expect(Skyfire::Spells::GetGlyphOfPenguinDisplayId() == 26452,
            "Glyph of the Penguin display id should be named in aura metadata");
        passed &= Expect(Skyfire::Spells::GetDragonmawIllusionAuraSpellId() == 42016,
            "Dragonmaw Illusion aura spell id should be named in aura metadata");
        passed &= Expect(Skyfire::Spells::GetDragonmawIllusionMountDisplayId() == 16314,
            "Dragonmaw Illusion mount display id should be named in aura metadata");
        passed &= Expect(Skyfire::Spells::GetLeaderOfThePackSpellId() == 17007,
            "Leader of the Pack spell should be named in aura metadata");
        passed &= Expect(Skyfire::Spells::GetLeaderOfThePackTriggeredSpellId() == 24932,
            "Leader of the Pack triggered spell should be named in aura metadata");
        passed &= Expect(Skyfire::Spells::IsNonAttackablePacifyAura(45839),
            "Vengeance of the Blue Flight should be named as the non-attackable pacify aura");
        passed &= Expect(!Skyfire::Spells::IsNonAttackablePacifyAura(999999),
            "Unknown pacify aura should not force non-attackable state");

        passed &= Expect(Skyfire::Spells::GetRaidProcChargeTriggerSpellId(57949) == 57952,
            "Shiver raid proc aura should resolve its trigger spell");
        passed &= Expect(Skyfire::Spells::GetRaidProcChargeTriggerSpellId(59978) == 59979,
            "Second Shiver raid proc aura should resolve its trigger spell");
        passed &= Expect(Skyfire::Spells::GetRaidProcChargeTriggerSpellId(43593) == 43594,
            "Cold Stare raid proc aura should resolve its trigger spell");
        passed &= Expect(Skyfire::Spells::GetRaidProcChargeTriggerSpellId(999999) == 0,
            "Unknown raid proc charge aura should not resolve a trigger spell");
        passed &= Expect(Skyfire::Spells::ShouldBindStackingAuraToCaster(false, true, false, false),
            "Non-shared stack auras should stay caster-bound");
        passed &= Expect(Skyfire::Spells::ShouldBindStackingAuraToCaster(true, false, false, false),
            "Player-cast stack auras should stay caster-bound");
        passed &= Expect(Skyfire::Spells::ShouldBindStackingAuraToCaster(true, true, true, false),
            "Summon-cast stack auras should stay caster-bound");
        passed &= Expect(Skyfire::Spells::ShouldBindStackingAuraToCaster(true, true, false, true),
            "Stack auras owned by summons should stay caster-bound");
        passed &= Expect(!Skyfire::Spells::ShouldBindStackingAuraToCaster(true, true, false, false),
            "Creature encounter stack auras should still be allowed to share a target stack");

        passed &= Expect(Skyfire::Spells::GetNetOMaticRootSelfSpellId() == 13138,
            "Net-o-matic should name its self-root spell");
        passed &= Expect(Skyfire::Spells::GetKillCommandOwnerAuraSpellId() == 34027 &&
            Skyfire::Spells::GetKillCommandPetAuraSpellId() == 58914,
            "Kill Command should name owner and pet aura metadata");
        passed &= Expect(Skyfire::Spells::GetBloodElfIllusionDisguiseSpellId(37096, GENDER_FEMALE) == 37095 &&
            Skyfire::Spells::GetBloodElfIllusionDisguiseSpellId(37096, GENDER_MALE) == 37093,
            "Blood Elf Illusion should resolve gendered disguise spell metadata");
        passed &= Expect(Skyfire::Spells::GetBloodElfIllusionDisguiseSpellId(46354, GENDER_FEMALE) == 46356 &&
            Skyfire::Spells::GetBloodElfIllusionDisguiseSpellId(46354, GENDER_MALE) == 46355,
            "Second Blood Elf Illusion should resolve gendered disguise spell metadata");
        passed &= Expect(Skyfire::Spells::GetBloodElfIllusionDisguiseSpellId(999999, GENDER_MALE) == 0,
            "Unknown Blood Elf Illusion aura should not resolve disguise metadata");
        passed &= Expect(Skyfire::Spells::GetBloodElfIllusionDisguiseSpellId(37096, GENDER_NONE) == 0,
            "Blood Elf Illusion should not resolve a disguise for unknown gender metadata");
        passed &= Expect(Skyfire::Spells::GetAuraDummyTriggeredSpellId(39850) == 51581 &&
            Skyfire::Spells::GetAuraDummyTriggeredSpellId(51701) == 51699 &&
            Skyfire::Spells::GetAuraDummyTriggeredSpellId(71563) == 71564,
            "Aura dummy apply rules should resolve their triggered spell metadata");
        passed &= Expect(Skyfire::Spells::GetAuraRemoveTriggeredSpellId(36730, false) == 36731 &&
            Skyfire::Spells::GetAuraRemoveTriggeredSpellId(44191, false) == 44190 &&
            Skyfire::Spells::GetAuraRemoveTriggeredSpellId(44191, true) == 46163 &&
            Skyfire::Spells::GetAuraRemoveTriggeredSpellId(46308, false) == 47287 &&
            Skyfire::Spells::GetAuraRemoveTriggeredSpellId(91604, false) == 58601,
            "Aura remove rules should resolve trigger spell metadata");
        passed &= Expect(Skyfire::Spells::IsDespawnOnRemoveAura(52172) &&
            Skyfire::Spells::IsDespawnOnRemoveAura(148596) &&
            !Skyfire::Spells::IsDespawnOnRemoveAura(999999),
            "Despawn-on-remove aura metadata should identify configured despawn auras only");
        passed &= Expect(Skyfire::Spells::GetStackingAuraTriggerSpellId(24658) == 24659 &&
            Skyfire::Spells::GetStackingAuraTriggerSpellId(24661) == 24662 &&
            Skyfire::Spells::GetStackingAuraTriggerSpellId(999999) == 0,
            "Stacking aura metadata should resolve triggered stack spell ids");
        passed &= Expect(Skyfire::Spells::GetTaggedMurlocEntry(true) == 17654 &&
            Skyfire::Spells::GetTaggedMurlocEntry(false) == 17326,
            "Tag Murloc metadata should name apply and remove entries");
        passed &= Expect(Skyfire::Spells::GetLichKingIntroApplySoundId(58204) == 14970 &&
            Skyfire::Spells::GetLichKingIntroContinueSpellId(58204) == 58205 &&
            Skyfire::Spells::GetLichKingIntroApplySoundId(58205) == 14971 &&
            Skyfire::Spells::GetLichKingIntroRemoveSoundId(58205) == 14972,
            "Lich King intro metadata should resolve sound and continuation ids");
        passed &= Expect(Skyfire::Spells::GetFestiveHolidayMountCreatureEntry(true) == 24906 &&
            Skyfire::Spells::GetFestiveHolidayMountCreatureEntry(false) == 15665,
            "Festive Holiday Mount metadata should resolve flying and ground creature entries");

        Skyfire::Spells::PeriodicBulletControllerRule const* bulletRule =
            Skyfire::Spells::GetPeriodicBulletControllerRule(66149);
        passed &= Expect(bulletRule && bulletRule->FirstSpellId == 66152 &&
            bulletRule->SecondSpellId == 66153 && bulletRule->MaxTargetRoll == 6,
            "Bullet controller periodic aura should resolve target spell metadata");
        passed &= Expect(Skyfire::Spells::GetPeriodicOverloadCircuitShutdownSpellId(62399) == 62475,
            "Overload Circuit should resolve System Shutdown metadata");
        passed &= Expect(Skyfire::Spells::GetPeriodicFuseArmorTriggerSpellId(64821) == 64774,
            "Fuse Armor should resolve its final trigger spell metadata");
        passed &= Expect(Skyfire::Spells::GetPeriodicFrenziedRegenerationSpellId() == 22845,
            "Frenzied Regeneration should name its heal trigger spell");
        passed &= Expect(Skyfire::Spells::GetKillingSpreePeriodicSpellId(0) == 57840 &&
            Skyfire::Spells::GetKillingSpreePeriodicSpellId(1) == 57841 &&
            Skyfire::Spells::GetKillingSpreePeriodicSpellId(2) == 0,
            "Killing Spree periodic metadata should expose its two trigger spells");
        passed &= Expect(Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(23170) == 23171 &&
            Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(29917) == 29916 &&
            Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(46736) == 46737 &&
            Skyfire::Spells::GetPeriodicTriggerOverrideSpellId(999999) == 0,
            "Periodic trigger override metadata should resolve configured trigger spells");
        passed &= Expect(Skyfire::Spells::IsPeriodicRestorationAura(24379) &&
            Skyfire::Spells::IsPeriodicRestorationAura(23493) &&
            Skyfire::Spells::GetPeriodicRestorationEnergizeSpellId() == 23493,
            "Restoration periodic metadata should identify both aura ids and its energize spell");
        passed &= Expect(Skyfire::Spells::GetPeriodicFrostBlastTriggerSpellId() == 29879 &&
            Skyfire::Spells::GetPeriodicDoomTriggerSpellId() == 31350 &&
            Skyfire::Spells::GetSpellclothSummonCreatureEntry() == 17870,
            "Periodic special-case metadata should name Frost Blast, Doom, and Spellcloth ids");
        passed &= Expect(Skyfire::Spells::GetAbsorbEyeOfGrillokCasterSpellId() == 38495,
            "Absorb Eye of Grillok should name the caster spell metadata");
        passed &= Expect(Skyfire::Spells::GetTotemicMasteryAuraSpellId() == 38443 &&
            Skyfire::Spells::GetTotemicMasteryTriggerSpellId() == 38437,
            "Totemic Mastery metadata should name the source aura and trigger spell");
        passed &= Expect(Skyfire::Spells::IsReagentConsumingPeriodicTriggerAura(24745) &&
            Skyfire::Spells::IsReagentConsumingPeriodicTriggerAura(24793) &&
            !Skyfire::Spells::IsReagentConsumingPeriodicTriggerAura(999999),
            "Reagent-consuming periodic trigger metadata should identify only configured auras");
        passed &= Expect(Skyfire::Spells::IsFullHealthRemovePeriodicDamageAura(43093) &&
            Skyfire::Spells::IsFullHealthRemovePeriodicDamageAura(39215) &&
            !Skyfire::Spells::IsFullHealthRemovePeriodicDamageAura(999999),
            "Full-health periodic damage removal metadata should identify configured auras");
        passed &= Expect(Skyfire::Spells::IsUnboundPlaguePeriodicDamageAura(70911) &&
            Skyfire::Spells::IsUnboundPlaguePeriodicDamageAura(72856) &&
            !Skyfire::Spells::IsUnboundPlaguePeriodicDamageAura(999999),
            "Unbound Plague metadata should identify configured damage auras");
        passed &= Expect(Skyfire::Spells::GetDrainSoulShardSpellId() == 95810 &&
            Skyfire::Spells::GetManaFeedTriggerSpellId() == 32554,
            "Periodic warlock metadata should name Drain Soul and Mana Feed triggers");

        passed &= Expect(Skyfire::Spells::IsUnsavableSpecialAura(44413) &&
            Skyfire::Spells::IsUnsavableSpecialAura(55849) &&
            !Skyfire::Spells::IsUnsavableSpecialAura(999999),
            "Unsavable special aura metadata should identify configured auras");
        passed &= Expect(Skyfire::Spells::IsUnsavableDruidEclipseAura(67483) &&
            Skyfire::Spells::IsUnsavableDruidEclipseAura(48518) &&
            !Skyfire::Spells::IsUnsavableDruidEclipseAura(999999),
            "Unsavable druid eclipse aura metadata should identify configured auras");
        passed &= Expect(Skyfire::Spells::GetStonedAuraTriggerSpellId(33572) == 33652 &&
            Skyfire::Spells::GetStonedAuraTriggerSpellId(50836) == 50812,
            "Stoned aura metadata should resolve final trigger spell ids");
        passed &= Expect(Skyfire::Spells::GetHeroicFuryCooldownSpellId() == 20252,
            "Heroic Fury metadata should name the cooldown spell to remove");
        passed &= Expect(Skyfire::Spells::GetArcanePotencyTriggerSpellId(31571) == 57529 &&
            Skyfire::Spells::GetArcanePotencyTriggerSpellId(31572) == 57531,
            "Arcane Potency metadata should resolve rank trigger spells");
        passed &= Expect(Skyfire::Spells::GetImprovedFearTriggerSpellId(53759) == 60947 &&
            Skyfire::Spells::GetImprovedFearTriggerSpellId(53754) == 60946,
            "Improved Fear metadata should resolve rank trigger spells");
        passed &= Expect(Skyfire::Spells::GetDruidFeralTier10BonusAuraSpellId() == 70726 &&
            Skyfire::Spells::GetDruidFeralTier10BonusTriggerSpellId() == 70725,
            "Druid T10 feral metadata should name source and trigger spells");
        passed &= Expect(Skyfire::Spells::GetStealthAuraSpellId() == 1784,
            "Stealth aura metadata should name the base stealth spell");

        Skyfire::Spells::AuraStateImmunityRule const* crowdControlRule =
            Skyfire::Spells::GetAuraStateImmunityRule(96, 0, 1);
        passed &= Expect(crowdControlRule &&
            crowdControlRule->MechanicMask == Skyfire::Spells::AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL &&
            crowdControlRule->MechanicCount == 12 && crowdControlRule->AuraTypeCount == 6,
            "State immunity misc 96 with amount should resolve crowd-control immunity metadata");
        passed &= Expect(crowdControlRule && crowdControlRule->Mechanics[0] == MECHANIC_SNARE &&
            crowdControlRule->Mechanics[11] == MECHANIC_TURN,
            "Crowd-control state immunity metadata should preserve mechanic order");
        passed &= Expect(!Skyfire::Spells::GetAuraStateImmunityRule(96, 0, 0),
            "State immunity misc 96 without amount should fall back to misc-flag decoding");

        Skyfire::Spells::AuraStateImmunityRule const* triggeredCrowdControlRule =
            Skyfire::Spells::GetAuraStateImmunityRule(679, 57742, 0);
        passed &= Expect(triggeredCrowdControlRule &&
            triggeredCrowdControlRule->MechanicMask == Skyfire::Spells::AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL,
            "State immunity misc 679 should require spell 57742 for crowd-control metadata");
        passed &= Expect(!Skyfire::Spells::GetAuraStateImmunityRule(679, 1, 0),
            "State immunity misc 679 should not resolve for unrelated spells");

        Skyfire::Spells::AuraStateImmunityRule const* stunRule =
            Skyfire::Spells::GetAuraStateImmunityRule(1557, 64187, 5);
        passed &= Expect(stunRule &&
            stunRule->MechanicMask == Skyfire::Spells::AURA_IMMUNITY_MECHANIC_MASK_STUN &&
            stunRule->MechanicCount == 1 && stunRule->Mechanics[0] == MECHANIC_STUN &&
            stunRule->AuraTypeCount == 1 && stunRule->AuraTypes[0] == SPELL_AURA_MOD_STUN,
            "State immunity misc 1557 with spell 64187 should resolve stun-only metadata");

        Skyfire::Spells::AuraStateImmunityRule const* tauntRule =
            Skyfire::Spells::GetAuraStateImmunityRule(1630, 0, 0);
        passed &= Expect(tauntRule && tauntRule->EffectCount == 1 &&
            tauntRule->Effects[0] == SPELL_EFFECT_ATTACK_ME &&
            tauntRule->AuraTypeCount == 1 && tauntRule->AuraTypes[0] == SPELL_AURA_MOD_TAUNT,
            "State immunity misc 1630 without amount should resolve taunt immunity metadata");

        Skyfire::Spells::AuraStateImmunityRule const* knockbackRule =
            Skyfire::Spells::GetAuraStateImmunityRule(477, 0, 0);
        passed &= Expect(knockbackRule && knockbackRule->EffectCount == 2 &&
            knockbackRule->Effects[0] == SPELL_EFFECT_KNOCK_BACK &&
            knockbackRule->Effects[1] == SPELL_EFFECT_KNOCK_BACK_DEST,
            "State immunity misc 477 without amount should include knockback effect immunities");

        Skyfire::Spells::AuraStateImmunityRule const* lightControlRule =
            Skyfire::Spells::GetAuraStateImmunityRule(878, 0, 1);
        passed &= Expect(lightControlRule &&
            lightControlRule->MechanicMask == Skyfire::Spells::AURA_IMMUNITY_MECHANIC_MASK_LIGHT_CONTROL &&
            lightControlRule->MechanicCount == 4 && lightControlRule->AuraTypeCount == 2,
            "State immunity misc 878 with amount 1 should resolve light-control immunity metadata");
        passed &= Expect(Skyfire::Spells::GetAuraImmunityMechanicMask(Skyfire::Spells::AURA_IMMUNITY_MECHANIC_MASK_LIGHT_CONTROL) ==
            Skyfire::Spells::BuildMechanicMask({ MECHANIC_SNARE, MECHANIC_STUN, MECHANIC_DISORIENTED, MECHANIC_FREEZE }),
            "Light-control immunity metadata should build the expected mechanic mask");

        uint32 defaultAuraTypes[Skyfire::Spells::MAX_AURA_IMMUNITY_AURA_TYPES] = {};
        uint8 defaultAuraCount = Skyfire::Spells::GetDefaultAuraStateImmunityAuraTypes(
            (1 << 10) | (1 << 9) | (1 << 7), defaultAuraTypes,
            Skyfire::Spells::MAX_AURA_IMMUNITY_AURA_TYPES);
        passed &= Expect(defaultAuraCount == 4 &&
            defaultAuraTypes[0] == SPELL_AURA_MOD_STUN &&
            defaultAuraTypes[1] == SPELL_AURA_MOD_FEAR &&
            defaultAuraTypes[2] == SPELL_AURA_MOD_FEAR_2 &&
            defaultAuraTypes[3] == SPELL_AURA_MOD_DISARM,
            "Default state immunity misc flags should decode aura types in handler order");

        Skyfire::Spells::MechanicImmunitySpellRule const* beastWithinRule =
            Skyfire::Spells::GetMechanicImmunitySpellRule(34471);
        passed &= Expect(beastWithinRule &&
            beastWithinRule->MechanicMask == Skyfire::Spells::AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL &&
            beastWithinRule->MechanicCount == 16 && beastWithinRule->Mechanics[0] == MECHANIC_CHARM &&
            beastWithinRule->Mechanics[15] == MECHANIC_SAPPED,
            "The Beast Within should resolve movement/loss-control mechanic immunity metadata");
        passed &= Expect(Skyfire::Spells::GetAuraImmunityMechanicMask(Skyfire::Spells::AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL) ==
            IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK,
            "Movement/loss-control immunity metadata should preserve the existing removal mask");

        Skyfire::Spells::MechanicImmunitySpellRule const* pvpTrinketRule =
            Skyfire::Spells::GetMechanicImmunitySpellRule(42292);
        passed &= Expect(pvpTrinketRule &&
            pvpTrinketRule->MechanicMask == Skyfire::Spells::AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL &&
            pvpTrinketRule->MechanicCount == 0,
            "PvP trinket should resolve removal-mask metadata without applying mechanic immunities");
        passed &= Expect(!Skyfire::Spells::GetMechanicImmunitySpellRule(999999),
            "Unknown mechanic immunity spell should not resolve metadata");

        return passed;
    }

    bool TestSpellMovementMetadataRules()
    {
        bool passed = true;

        Skyfire::Spells::JumpDestOverride const* shadowstepOverride =
            Skyfire::Spells::GetJumpDestOverride(108938);
        passed &= Expect(shadowstepOverride && shadowstepOverride->SpellId == 108938,
            "Movement metadata should resolve the Shadowstep destination jump override");
        passed &= Expect(shadowstepOverride &&
            ExpectNear(shadowstepOverride->ZOffset, 10.0f, 0.001f,
                "Shadowstep destination jump override should preserve its custom z offset") &&
            ExpectNear(shadowstepOverride->SpeedXY, 20.0f, 0.001f,
                "Shadowstep destination jump override should preserve its horizontal speed") &&
            ExpectNear(shadowstepOverride->SpeedZ, 20.0f, 0.001f,
                "Shadowstep destination jump override should preserve its vertical speed"),
            "Shadowstep destination jump override should preserve its custom arc");
        passed &= Expect(!Skyfire::Spells::GetJumpDestOverride(999999),
            "Unknown spell should not resolve a destination jump override");

        Skyfire::Spells::TeleportPostEffect const* everlookEffect =
            Skyfire::Spells::GetTeleportPostEffect(23442);
        passed &= Expect(everlookEffect &&
            everlookEffect->Kind == Skyfire::Spells::TELEPORT_POST_EFFECT_EVERLOOK_RIPPER &&
            everlookEffect->RandomEffectCount == 119,
            "Everlook dimensional ripper should resolve its post-teleport metadata");
        passed &= Expect(Skyfire::Spells::GetEverlookRipperPostEffectSpellId(69) == 0,
            "Everlook ripper rolls below 70 should not trigger a post effect");
        passed &= Expect(Skyfire::Spells::GetEverlookRipperPostEffectSpellId(70) == 23445,
            "Everlook ripper roll 70 should trigger the evil twin spell");
        passed &= Expect(Skyfire::Spells::GetEverlookRipperPostEffectSpellId(99) == 23445,
            "Everlook ripper rolls below 100 should trigger the evil twin spell");
        passed &= Expect(Skyfire::Spells::GetEverlookRipperPostEffectSpellId(100) == 23449,
            "Everlook ripper roll 100 should trigger the fire spell");

        Skyfire::Spells::TeleportPostEffect const* toshleysEffect =
            Skyfire::Spells::GetTeleportPostEffect(36941);
        passed &= Expect(toshleysEffect &&
            toshleysEffect->Kind == Skyfire::Spells::TELEPORT_POST_EFFECT_TOSHLEYS_TRANSPORTER &&
            toshleysEffect->SuccessChancePct == 50 && toshleysEffect->RandomEffectCount == 7,
            "Toshley's transporter should resolve its chance and random effect metadata");
        passed &= Expect(Skyfire::Spells::GetToshleysTransporterPostEffectSpellId(1, ALLIANCE) == 36900,
            "Toshley's transporter effect 1 should trigger soul split evil");
        passed &= Expect(Skyfire::Spells::GetToshleysTransporterPostEffectSpellId(4, ALLIANCE) == 36893,
            "Toshley's transporter effect 4 should trigger decrease size");
        passed &= Expect(Skyfire::Spells::GetToshleysTransporterPostEffectSpellId(5, ALLIANCE) == 36897,
            "Toshley's transporter alliance transform should use the alliance spell");
        passed &= Expect(Skyfire::Spells::GetToshleysTransporterPostEffectSpellId(5, HORDE) == 36899,
            "Toshley's transporter horde transform should use the horde spell");
        passed &= Expect(Skyfire::Spells::GetToshleysTransporterPostEffectSpellId(7, HORDE) == 23445,
            "Toshley's transporter effect 7 should trigger evil twin");
        passed &= Expect(Skyfire::Spells::GetToshleysTransporterPostEffectSpellId(8, HORDE) == 0,
            "Unknown Toshley's transporter effect should not trigger a spell");

        Skyfire::Spells::TeleportPostEffect const* area52Effect =
            Skyfire::Spells::GetTeleportPostEffect(36890);
        passed &= Expect(area52Effect &&
            area52Effect->Kind == Skyfire::Spells::TELEPORT_POST_EFFECT_AREA52_RIPPER &&
            area52Effect->SuccessChancePct == 50 && area52Effect->RandomEffectCount == 4,
            "Area 52 ripper should resolve its chance and random effect metadata");
        passed &= Expect(Skyfire::Spells::GetArea52RipperPostEffectSpellId(1, ALLIANCE) == 36900,
            "Area 52 ripper effect 1 should trigger soul split evil");
        passed &= Expect(Skyfire::Spells::GetArea52RipperPostEffectSpellId(4, ALLIANCE) == 36897,
            "Area 52 ripper alliance transform should use the alliance spell");
        passed &= Expect(Skyfire::Spells::GetArea52RipperPostEffectSpellId(4, HORDE) == 36899,
            "Area 52 ripper horde transform should use the horde spell");
        passed &= Expect(Skyfire::Spells::GetArea52RipperPostEffectSpellId(5, HORDE) == 0,
            "Unknown Area 52 ripper effect should not trigger a spell");
        passed &= Expect(!Skyfire::Spells::GetTeleportPostEffect(999999),
            "Unknown spell should not resolve teleport post-effect metadata");

        passed &= Expect(Skyfire::Spells::GetStuckHearthstoneCooldownSpellId() == 8690,
            "Stuck movement metadata should name the hearthstone cooldown spell");

        return passed;
    }

    bool TestSpellItemMetadataRules()
    {
        bool passed = true;

        passed &= Expect(Skyfire::Spells::ShouldDeactivateOwnedTrapOnOpenLock(1842, GAMEOBJECT_TYPE_TRAP, true),
            "Disarm Trap should deactivate owned trap gameobjects when opening locks");
        passed &= Expect(!Skyfire::Spells::ShouldDeactivateOwnedTrapOnOpenLock(1842, GAMEOBJECT_TYPE_TRAP, false),
            "Disarm Trap should require an owned trap before deactivating the gameobject");
        passed &= Expect(!Skyfire::Spells::ShouldDeactivateOwnedTrapOnOpenLock(1842, GAMEOBJECT_TYPE_GOOBER, true),
            "Disarm Trap should not deactivate non-trap gameobjects");
        passed &= Expect(!Skyfire::Spells::ShouldDeactivateOwnedTrapOnOpenLock(999999, GAMEOBJECT_TYPE_TRAP, true),
            "Unknown open-lock spells should not use the disarm trap exception");

        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(38615, SPELLFAMILY_ROGUE, 999) == 1800,
            "Rogue temporary enchant spell 38615 should preserve its 30 minute exception");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(12345, SPELLFAMILY_ROGUE, 999) == 3600,
            "Other rogue temporary enchants should last one hour");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(12345, SPELLFAMILY_SHAMAN, 999) == 1800,
            "Shaman temporary enchants should last 30 minutes");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(12345, SPELLFAMILY_GENERIC, 215) == 1800,
            "Temporary enchants with visual 215 should last 30 minutes");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(12345, SPELLFAMILY_GENERIC, 563) == 600,
            "Fishing pole temporary enchants with visual 563 should last 10 minutes");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(64401, SPELLFAMILY_GENERIC, 563) == 3600,
            "Glow Worm should be excluded from the visual 563 10 minute rule");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(12345, SPELLFAMILY_GENERIC, 0) == 1800,
            "Temporary enchants with visual 0 should last 30 minutes");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(29702, SPELLFAMILY_GENERIC, 999) == 300,
            "Temporary enchant spell 29702 should last 5 minutes");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(37360, SPELLFAMILY_GENERIC, 999) == 300,
            "Temporary enchant spell 37360 should last 5 minutes");
        passed &= Expect(Skyfire::Spells::GetTemporaryItemEnchantDurationSeconds(12345, SPELLFAMILY_GENERIC, 999) == 3600,
            "Default temporary enchants should last one hour");

        return passed;
    }

    bool TestSpellCombatMetadataRules()
    {
        bool passed = true;

        passed &= Expect(Skyfire::Spells::GetGenericSchoolDamageRule(28865) ==
            Skyfire::Spells::GENERIC_SCHOOL_DAMAGE_CONSUMPTION,
            "Consumption should resolve its generic school damage rule");
        passed &= Expect(Skyfire::Spells::GetConsumptionDamage(DIFFICULTY_NONE) == 2750,
            "Consumption should preserve normal difficulty damage");
        passed &= Expect(Skyfire::Spells::GetConsumptionDamage(DIFFICULTY_HEROIC) == 4250,
            "Consumption should preserve heroic difficulty damage");
        passed &= Expect(Skyfire::Spells::GetGenericSchoolDamageRule(25599) ==
            Skyfire::Spells::GENERIC_SCHOOL_DAMAGE_THUNDERCRASH,
            "Thundercrash should resolve its generic school damage rule");
        passed &= Expect(Skyfire::Spells::GetThundercrashMinimumDamage() == 200,
            "Thundercrash should preserve its minimum damage");
        passed &= Expect(Skyfire::Spells::GetGenericSchoolDamageRule(45072) ==
            Skyfire::Spells::GENERIC_SCHOOL_DAMAGE_ARCANE_CHARGE,
            "Arcane Charge should resolve its generic school damage rule");
        passed &= Expect(Skyfire::Spells::IsArcaneChargeAllowedCreatureType(CREATURE_TYPE_DEMON),
            "Arcane Charge should affect demons");
        passed &= Expect(Skyfire::Spells::IsArcaneChargeAllowedCreatureType(CREATURE_TYPE_UNDEAD),
            "Arcane Charge should affect undead");
        passed &= Expect(!Skyfire::Spells::IsArcaneChargeAllowedCreatureType(CREATURE_TYPE_HUMANOID),
            "Arcane Charge should reject unrelated creature types");
        passed &= Expect(Skyfire::Spells::GetGargoyleStrikeDamageForLevel(85) == 160,
            "Gargoyle Strike should preserve its level scaling formula");
        passed &= Expect(Skyfire::Spells::GetGargoyleStrikeDamageForLevel(55) == 40,
            "Gargoyle Strike should preserve signed level scaling below level 60");
        passed &= Expect(Skyfire::Spells::GetGenericSchoolDamageRule(999999) ==
            Skyfire::Spells::GENERIC_SCHOOL_DAMAGE_NONE,
            "Unknown generic school damage spell should not resolve a rule");

        passed &= Expect(Skyfire::Spells::IsVictoryRushDamageSpell(34428),
            "Victory Rush should be named in combat metadata");
        passed &= Expect(Skyfire::Spells::IsShockwaveDamageSpell(46968),
            "Shockwave should be named in combat metadata");
        passed &= Expect(Skyfire::Spells::IsManaBurnSpell(8129),
            "Mana Burn should be named in combat metadata");
        passed &= Expect(Skyfire::Spells::IsVesselOfTheNaaruHealSpell(45064) &&
            Skyfire::Spells::GetVesselOfTheNaaruStackAuraSpellId() == 45062,
            "Vessel of the Naaru should resolve its stack aura metadata");
        passed &= Expect(Skyfire::Spells::IsRunicHealingInjectorSpell(67489) &&
            Skyfire::Spells::GetEngineeringHealingBonusPct() == 25,
            "Runic Healing Injector should preserve the engineering heal bonus");

        Skyfire::Spells::EnergizeLevelScalingRule const* restoreEnergy =
            Skyfire::Spells::GetEnergizeLevelScalingRule(9512);
        passed &= Expect(restoreEnergy && restoreEnergy->BaseLevel == 40 &&
            restoreEnergy->LevelMultiplier == 2,
            "Restore Energy should preserve its level scaling metadata");
        Skyfire::Spells::EnergizeLevelScalingRule const* bloodFury =
            Skyfire::Spells::GetEnergizeLevelScalingRule(24571);
        passed &= Expect(bloodFury && bloodFury->BaseLevel == 60 &&
            bloodFury->LevelMultiplier == 10,
            "Blood Fury should preserve its level scaling metadata");
        Skyfire::Spells::EnergizeLevelScalingRule const* burstOfEnergy =
            Skyfire::Spells::GetEnergizeLevelScalingRule(24532);
        passed &= Expect(burstOfEnergy && burstOfEnergy->BaseLevel == 60 &&
            burstOfEnergy->LevelMultiplier == 4,
            "Burst of Energy should preserve its level scaling metadata");
        passed &= Expect(!Skyfire::Spells::GetEnergizeLevelScalingRule(999999),
            "Unknown energize spell should not resolve level scaling metadata");
        passed &= Expect(Skyfire::Spells::IsPrimalWisdomEnergizeSpell(63375),
            "Primal Wisdom should be named in combat metadata");
        passed &= Expect(Skyfire::Spells::IsRunicManaInjectorSpell(67490),
            "Runic Mana Injector should be named in combat metadata");
        passed &= Expect(Skyfire::Spells::IsJabEnergizeSpell(100780) &&
            Skyfire::Spells::GetJabBaseChi() == 1 &&
            Skyfire::Spells::GetStanceOfTheFierceTigerAuraSpellId() == 103985,
            "Jab should preserve its base chi and stance aura metadata");
        passed &= Expect(Skyfire::Spells::IsMadAlchemistsPotionSpell(45051),
            "Mad Alchemist's Potion should be named in combat metadata");
        passed &= Expect(Skyfire::Spells::IsBloodTapRuneSpell(45529),
            "Blood Tap should be named in combat metadata");
        passed &= Expect(Skyfire::Spells::IsEmpowerRuneWeaponSpell(47568),
            "Empower Rune Weapon should be named in combat metadata");

        return passed;
    }

    bool TestSpellEffectMetadataRules()
    {
        bool passed = true;

        Skyfire::Spells::TriggerSpellRule const* vanishRule =
            Skyfire::Spells::GetTriggerSpellRule(18461);
        passed &= Expect(vanishRule &&
            vanishRule->Kind == Skyfire::Spells::TRIGGER_SPELL_RULE_VANISH &&
            vanishRule->CastSpellId == 1784,
            "Vanish trigger should resolve its stealth spell metadata");

        Skyfire::Spells::TriggerSpellRule const* succubusRule =
            Skyfire::Spells::GetTriggerSpellRule(54437);
        passed &= Expect(succubusRule &&
            succubusRule->Kind == Skyfire::Spells::TRIGGER_SPELL_RULE_DEMONIC_EMPOWERMENT_SUCCUBUS &&
            succubusRule->CastSpellId == 7870,
            "Succubus demonic empowerment should resolve Lesser Invisibility metadata");

        Skyfire::Spells::TriggerSpellRule const* brittleArmorRule =
            Skyfire::Spells::GetTriggerSpellRule(29284);
        passed &= Expect(brittleArmorRule &&
            brittleArmorRule->Kind == Skyfire::Spells::TRIGGER_SPELL_RULE_STACK_TRIGGER &&
            brittleArmorRule->StackSpellId == 24575,
            "Brittle Armor should resolve stack trigger metadata");

        Skyfire::Spells::TriggerSpellRule const* mercurialShieldRule =
            Skyfire::Spells::GetTriggerSpellRule(29286);
        passed &= Expect(mercurialShieldRule &&
            mercurialShieldRule->Kind == Skyfire::Spells::TRIGGER_SPELL_RULE_STACK_TRIGGER &&
            mercurialShieldRule->StackSpellId == 26464,
            "Mercurial Shield should resolve stack trigger metadata");

        Skyfire::Spells::TriggerSpellRule const* cloakRule =
            Skyfire::Spells::GetTriggerSpellRule(35729);
        passed &= Expect(cloakRule &&
            cloakRule->Kind == Skyfire::Spells::TRIGGER_SPELL_RULE_CLOAK_OF_SHADOWS,
            "Cloak of Shadows should resolve trigger metadata");
        passed &= Expect(!Skyfire::Spells::GetTriggerSpellRule(999999),
            "Unknown trigger spell should not resolve special trigger metadata");

        passed &= Expect(Skyfire::Spells::ShouldSendNoFlyZoneNotification(91604),
            "Restricted Flight Area should send the no-fly notification");
        passed &= Expect(!Skyfire::Spells::ShouldSendNoFlyZoneNotification(999999),
            "Unknown media spell should not send the no-fly notification");
        passed &= Expect(Skyfire::Spells::GetKillCreditFallbackCreatureEntry(42793) == 24008,
            "Burn Body should resolve Fallen Combatant fallback kill credit");
        passed &= Expect(Skyfire::Spells::GetKillCreditFallbackCreatureEntry(999999) == 0,
            "Unknown quest spell should not resolve fallback kill credit");

        Skyfire::Spells::ForceCastDamageRule const* skeletalGryphonRule =
            Skyfire::Spells::GetForceCastDamageRule(52588);
        passed &= Expect(skeletalGryphonRule &&
            skeletalGryphonRule->Kind == Skyfire::Spells::FORCE_CAST_DAMAGE_RULE_REMOVE_AURA,
            "Skeletal Gryphon Escape should remove the aura identified by damage");
        Skyfire::Spells::ForceCastDamageRule const* flamebringerRule =
            Skyfire::Spells::GetForceCastDamageRule(48598);
        passed &= Expect(flamebringerRule &&
            flamebringerRule->Kind == Skyfire::Spells::FORCE_CAST_DAMAGE_RULE_REMOVE_AURA,
            "Ride Flamebringer Cue should remove the aura identified by damage");
        Skyfire::Spells::ForceCastDamageRule const* mineCarRule =
            Skyfire::Spells::GetForceCastDamageRule(52463);
        passed &= Expect(mineCarRule &&
            mineCarRule->Kind == Skyfire::Spells::FORCE_CAST_DAMAGE_RULE_CUSTOM_BASEPOINTS,
            "Hide In Mine Car should force cast with damage as custom basepoints");
        Skyfire::Spells::ForceCastDamageRule const* overtakeRule =
            Skyfire::Spells::GetForceCastDamageRule(52349);
        passed &= Expect(overtakeRule &&
            overtakeRule->Kind == Skyfire::Spells::FORCE_CAST_DAMAGE_RULE_CUSTOM_BASEPOINTS,
            "Overtake should force cast with damage as custom basepoints");
        Skyfire::Spells::ForceCastDamageRule const* gooRule =
            Skyfire::Spells::GetForceCastDamageRule(72299);
        passed &= Expect(gooRule &&
            gooRule->Kind == Skyfire::Spells::FORCE_CAST_DAMAGE_RULE_TRIGGERED_ORIGINAL_CASTER,
            "Malleable Goo Summon Trigger should force cast with the original caster guid");
        passed &= Expect(!Skyfire::Spells::GetForceCastDamageRule(999999),
            "Unknown force-cast spell should not resolve damage special handling");

        passed &= Expect(Skyfire::Spells::ShouldLearnSpellFromEffectDamage(483),
            "Learning spell 483 should use effect damage as the learned spell id");
        passed &= Expect(Skyfire::Spells::ShouldLearnSpellFromEffectDamage(55884),
            "Learning spell 55884 should use effect damage as the learned spell id");
        passed &= Expect(!Skyfire::Spells::ShouldLearnSpellFromEffectDamage(999999),
            "Unknown learning spell should use the trigger spell id");

        passed &= Expect(Skyfire::Spells::GetBendingShinboneTriggerSpellId(true) == 8854 &&
            Skyfire::Spells::GetBendingShinboneTriggerSpellId(false) == 8855,
            "Bending Shinbone should name both random trigger spells");
        passed &= Expect(Skyfire::Spells::GetScriptEffectStackRemovalAuraSpellId(24590) == 24575 &&
            Skyfire::Spells::GetScriptEffectStackRemovalAuraSpellId(26465) == 26464 &&
            Skyfire::Spells::GetScriptEffectStackRemovalAuraSpellId(999999) == 0,
            "Script stack-removal metadata should resolve removable aura ids");
        Skyfire::Spells::ShadowFlameScriptRule const* shadowFlameRule =
            Skyfire::Spells::GetShadowFlameScriptRule(22985);
        passed &= Expect(shadowFlameRule &&
            shadowFlameRule->ProtectionAuraSpellId == 22683 &&
            shadowFlameRule->TriggerSpellId == 22682,
            "Shadow Flame metadata should resolve protection and trigger spell ids");
        passed &= Expect(Skyfire::Spells::GetDecimateScriptTriggerSpellId(28374) == 28375 &&
            Skyfire::Spells::GetDecimateScriptTriggerSpellId(54426) == 28375,
            "Decimate script metadata should resolve the damage trigger spell");
        passed &= Expect(Skyfire::Spells::GetMirrensDrinkingHatItemId(1) == 23584 &&
            Skyfire::Spells::GetMirrensDrinkingHatItemId(4) == 23585 &&
            Skyfire::Spells::GetMirrensDrinkingHatItemId(6) == 23586 &&
            Skyfire::Spells::GetMirrensDrinkingHatItemId(7) == 0,
            "Mirren's Drinking Hat metadata should map rolls to item ids");
        passed &= Expect(Skyfire::Spells::GetGoblinWeatherMachineSpellId(0) == 46740 &&
            Skyfire::Spells::GetGoblinWeatherMachineSpellId(3) == 46736 &&
            Skyfire::Spells::GetGoblinWeatherMachineSpellId(4) == 0,
            "Goblin Weather Machine metadata should map rolls to weather spells");
        passed &= Expect(Skyfire::Spells::GetDeathKnightInitiateVisualSpellId(25369) == 51552 &&
            Skyfire::Spells::GetDeathKnightInitiateVisualSpellId(25365) == 51546 &&
            Skyfire::Spells::GetDeathKnightInitiateVisualSpellId(999999) == 0,
            "Death Knight Initiate visual metadata should map display ids to trigger spells");
        passed &= Expect(Skyfire::Spells::GetBigBlizzardBearMountSpellId(149) == 58997 &&
            Skyfire::Spells::GetBigBlizzardBearMountSpellId(150) == 58999,
            "Big Blizzard Bear metadata should choose mount spells from riding skill");
        passed &= Expect(Skyfire::Spells::GetTeleportingScriptSpellId(4637) == 59316 &&
            Skyfire::Spells::GetTeleportingScriptSpellId(1) == 59314,
            "Teleporting script metadata should choose return or atop spell ids");
        passed &= Expect(Skyfire::Spells::GetLightwellHealSpellId(31897) == 7001 &&
            Skyfire::Spells::GetLightwellHealSpellId(31883) == 48085 &&
            Skyfire::Spells::GetLightwellHealSpellId(999999) == 0,
            "Lightwell metadata should map summon entries to heal spells");
        Skyfire::Spells::ProtoTypicalShorteningBlasterSpells const* blasterSpells =
            Skyfire::Spells::GetProtoTypicalShorteningBlasterSpells(4);
        passed &= Expect(blasterSpells &&
            blasterSpells->CasterSpellId == 45684 &&
            blasterSpells->TargetSpellId == 45683,
            "Proto-Typical Shortening Blaster metadata should map random effects to caster and target spells");
        passed &= Expect(Skyfire::Spells::GetJudgementSealTriggerSpellId(53407) == 20184 &&
            Skyfire::Spells::GetJudgementSealTriggerSpellId(57774) == 20185 &&
            Skyfire::Spells::GetJudgementSealTriggerSpellId(53408) == 20186 &&
            Skyfire::Spells::GetJudgementSealTriggerSpellId(999999) == 0,
            "Judgement metadata should resolve seal trigger spell ids");
        passed &= Expect(Skyfire::Spells::GetJudgementFallbackSealTriggerSpellId(20165) == 54158 &&
            Skyfire::Spells::GetJudgementFallbackSealTriggerSpellId(20164) == 54158 &&
            Skyfire::Spells::GetJudgementFallbackSealTriggerSpellId(999999) == 0,
            "Judgement fallback metadata should resolve default seal trigger spell ids");
        passed &= Expect(Skyfire::Spells::GetCreateHealthstoneTriggeredSpellId(6201) == 23517 &&
            Skyfire::Spells::GetCreateHealthstoneTriggeredSpellId(34130) == 23517 &&
            Skyfire::Spells::GetCreateHealthstoneTriggeredSpellId(999999) == 0,
            "Create Healthstone metadata should resolve its triggered spell id");

        return passed;
    }

    bool TestSpellSummonMetadataRules()
    {
        bool passed = true;

        uint32 damageBasedProperties[] =
        {
            64,
            61,
            1101,
            66,
            648,
            2301,
            1061,
            1261,
            629,
            181,
            715,
            1562,
            833,
            1161
        };

        for (uint32 propertyId : damageBasedProperties)
            passed &= Expect(Skyfire::Spells::ShouldUseDamageAsSummonCount(propertyId),
                "Summon property should use effect damage as summon count");

        passed &= Expect(!Skyfire::Spells::ShouldUseDamageAsSummonCount(999999),
            "Unknown summon property should default to one summon");
        passed &= Expect(Skyfire::Spells::GetSummonCountForProperty(64, 5) == 5,
            "Damage-based summon properties should use positive damage as the summon count");
        passed &= Expect(Skyfire::Spells::GetSummonCountForProperty(64, 0) == 1,
            "Damage-based summon properties should default to one summon for zero damage");
        passed &= Expect(Skyfire::Spells::GetSummonCountForProperty(64, -3) == 1,
            "Damage-based summon properties should default to one summon for negative damage");
        passed &= Expect(Skyfire::Spells::GetSummonCountForProperty(999999, 5) == 1,
            "Unknown summon properties should ignore effect damage for summon count");

        passed &= Expect(Skyfire::Spells::GetTotemSummonHealthPct(115313) == 30,
            "Monk statue summon should use 30 percent caster max health");
        passed &= Expect(Skyfire::Spells::GetTotemSummonHealthPct(115315) == 30,
            "Second monk statue summon should use 30 percent caster max health");
        passed &= Expect(Skyfire::Spells::GetTotemSummonHealthPct(126135) == 30,
            "Third monk statue summon should use 30 percent caster max health");
        passed &= Expect(Skyfire::Spells::GetTotemSummonHealthPct(16190) == 10,
            "Mana Tide Totem should use 10 percent caster max health");
        passed &= Expect(Skyfire::Spells::GetTotemSummonHealthPct(999999) == 0,
            "Unknown totem summon should not override DB health");

        return passed;
    }

    bool TestGridAndCellPrimitives()
    {
        bool passed = true;

        GridCoord grid = Skyfire::ComputeGridCoord(0.0f, 0.0f);
        passed &= Expect(grid.x_coord == CENTER_GRID_ID && grid.y_coord == CENTER_GRID_ID,
            "World origin should map to the center grid");

        CellCoord cell = Skyfire::ComputeCellCoord(0.0f, 0.0f);
        passed &= Expect(cell.x_coord == CENTER_GRID_CELL_ID && cell.y_coord == CENTER_GRID_CELL_ID,
            "World origin should map to the center cell");

        CellCoord low(10, 11);
        CellCoord high(12, 13);
        CellArea area(low, high);
        CellCoord begin;
        CellCoord end;
        area.ResizeBorders(begin, end);
        passed &= Expect(begin == low && end == high, "CellArea should return configured borders");
        passed &= Expect(!CellArea(low, low), "CellArea should be empty when low and high bounds match");
        passed &= Expect(!!area, "CellArea should be non-empty when bounds differ");

        CellCoord invalid(TOTAL_NUMBER_OF_CELLS_PER_MAP + 5, TOTAL_NUMBER_OF_CELLS_PER_MAP + 7);
        invalid.normalize();
        passed &= Expect(invalid.x_coord == TOTAL_NUMBER_OF_CELLS_PER_MAP - 1 &&
            invalid.y_coord == TOTAL_NUMBER_OF_CELLS_PER_MAP - 1,
            "CellCoord normalize should clamp to the valid maximum");

        float highCoord = MAP_HALFSIZE + 100.0f;
        Skyfire::NormalizeMapCoord(highCoord);
        passed &= Expect(std::fabs(highCoord - (MAP_HALFSIZE - 0.5f)) < 0.001f,
            "NormalizeMapCoord should clamp high map coordinates");

        float lowCoord = -MAP_HALFSIZE - 100.0f;
        Skyfire::NormalizeMapCoord(lowCoord);
        passed &= Expect(std::fabs(lowCoord + (MAP_HALFSIZE - 0.5f)) < 0.001f,
            "NormalizeMapCoord should clamp low map coordinates");

        passed &= Expect(Skyfire::IsValidMapCoord(MAP_HALFSIZE - 1.0f),
            "Coordinate inside map bounds should be valid");
        passed &= Expect(!Skyfire::IsValidMapCoord(std::numeric_limits<float>::infinity()),
            "Infinite coordinate should be invalid");

        return passed;
    }

    bool TestMapMoveQueueLifecycleRules()
    {
        bool passed = true;

        using Skyfire::Maps::MAP_MOVE_QUEUE_ADD_APPEND;
        using Skyfire::Maps::MAP_MOVE_QUEUE_ADD_REFRESH;
        using Skyfire::Maps::MAP_MOVE_QUEUE_ADD_SKIPPED_LOCKED;
        using Skyfire::Maps::MAP_MOVE_QUEUE_REMOVE_ALREADY_CLEAR;
        using Skyfire::Maps::MAP_MOVE_QUEUE_REMOVE_MARK_INACTIVE;
        using Skyfire::Maps::MAP_MOVE_QUEUE_REMOVE_SKIPPED_LOCKED;

        passed &= Expect(Skyfire::Maps::GetMoveQueueAddAction(MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_NONE, false) ==
            MAP_MOVE_QUEUE_ADD_APPEND,
            "Objects that are not queued should be appended to the map move queue");
        passed &= Expect(Skyfire::Maps::GetMoveQueueAddAction(MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_ACTIVE, false) ==
            MAP_MOVE_QUEUE_ADD_REFRESH,
            "Objects already queued for movement should refresh their target position without duplicating the queue entry");
        passed &= Expect(Skyfire::Maps::GetMoveQueueAddAction(MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_INACTIVE, false) ==
            MAP_MOVE_QUEUE_ADD_REFRESH,
            "Inactive queued objects should be reactivated without duplicating the queue entry");
        passed &= Expect(Skyfire::Maps::GetMoveQueueAddAction(MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_NONE, true) ==
            MAP_MOVE_QUEUE_ADD_SKIPPED_LOCKED,
            "Move queue additions during queue drain should be skipped");

        MapObjectCellMoveState state = MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_ACTIVE;
        passed &= Expect(Skyfire::Maps::MarkMoveQueueEntryInactive(state, false) == MAP_MOVE_QUEUE_REMOVE_MARK_INACTIVE,
            "Active queued objects should be marked inactive when removed before queue drain");
        passed &= Expect(state == MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_INACTIVE,
            "Removing an active queued object should leave an inactive queue marker");

        state = MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_NONE;
        passed &= Expect(Skyfire::Maps::MarkMoveQueueEntryInactive(state, false) == MAP_MOVE_QUEUE_REMOVE_ALREADY_CLEAR,
            "Objects outside the move queue should not change when removed from the queue");
        passed &= Expect(state == MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_NONE,
            "Removing an object outside the move queue should keep the queue state clear");

        state = MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_ACTIVE;
        passed &= Expect(Skyfire::Maps::MarkMoveQueueEntryInactive(state, true) == MAP_MOVE_QUEUE_REMOVE_SKIPPED_LOCKED,
            "Move queue removals during queue drain should be skipped");
        passed &= Expect(state == MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_ACTIVE,
            "Skipped removals during queue drain should preserve the active state");

        state = MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_ACTIVE;
        passed &= Expect(Skyfire::Maps::ConsumeMoveQueueEntry(state),
            "Active queued objects should be consumed for relocation");
        passed &= Expect(state == MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_NONE,
            "Consumed active queue entries should reset to the clear state");

        state = MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_INACTIVE;
        passed &= Expect(!Skyfire::Maps::ConsumeMoveQueueEntry(state),
            "Inactive queued objects should be skipped during queue drain");
        passed &= Expect(state == MapObjectCellMoveState::MAP_OBJECT_CELL_MOVE_NONE,
            "Consumed inactive queue entries should reset to the clear state");

        return passed;
    }

    bool TestMapDelayedListLifecycleRules()
    {
        bool passed = true;

        using Skyfire::Maps::MAP_ADD_OBJECT_ADD_TO_GRID;
        using Skyfire::Maps::MAP_ADD_OBJECT_REFRESH_EXISTING;
        using Skyfire::Maps::MAP_ADD_OBJECT_REJECT_INVALID_COORDS;
        using Skyfire::Maps::MAP_REMOVE_LIST_ADD_ALREADY_QUEUED;
        using Skyfire::Maps::MAP_REMOVE_LIST_ADD_INSERT;
        using Skyfire::Maps::MAP_SWITCH_LIST_ERASE_OPPOSITE;
        using Skyfire::Maps::MAP_SWITCH_LIST_IGNORE_UNSUPPORTED_TYPE;
        using Skyfire::Maps::MAP_SWITCH_LIST_INSERT;
        using Skyfire::Maps::MAP_SWITCH_LIST_REJECT_DUPLICATE;

        passed &= Expect(Skyfire::Maps::GetAddObjectAction(true, true) == MAP_ADD_OBJECT_REFRESH_EXISTING,
            "Objects already in world should preserve the legacy AddToMap visibility-refresh path");
        passed &= Expect(Skyfire::Maps::GetAddObjectAction(false, false) == MAP_ADD_OBJECT_REJECT_INVALID_COORDS,
            "Objects outside valid map coordinates should be rejected before grid insertion");
        passed &= Expect(Skyfire::Maps::GetAddObjectAction(false, true) == MAP_ADD_OBJECT_ADD_TO_GRID,
            "Fresh objects with valid coordinates should be added to the map grid");

        passed &= Expect(Skyfire::Maps::GetRemoveListAddAction(false) == MAP_REMOVE_LIST_ADD_INSERT,
            "Objects not already queued for delayed removal should be inserted into the remove list");
        passed &= Expect(Skyfire::Maps::GetRemoveListAddAction(true) == MAP_REMOVE_LIST_ADD_ALREADY_QUEUED,
            "Objects already queued for delayed removal should be classified as duplicate remove-list entries");

        passed &= Expect(Skyfire::Maps::GetSwitchListAction(false, false, false, true) ==
            MAP_SWITCH_LIST_IGNORE_UNSUPPORTED_TYPE,
            "Unsupported object types should be ignored by the delayed switch list");
        passed &= Expect(Skyfire::Maps::GetSwitchListAction(true, false, false, true) ==
            MAP_SWITCH_LIST_INSERT,
            "Supported objects missing from the switch list should be inserted");
        passed &= Expect(Skyfire::Maps::GetSwitchListAction(true, true, true, false) ==
            MAP_SWITCH_LIST_ERASE_OPPOSITE,
            "Opposite switch-list requests should cancel the existing queued transition");
        passed &= Expect(Skyfire::Maps::GetSwitchListAction(true, true, false, true) ==
            MAP_SWITCH_LIST_ERASE_OPPOSITE,
            "Opposite switch-list requests should cancel queued off-to-on transitions");
        passed &= Expect(Skyfire::Maps::GetSwitchListAction(true, true, true, true) ==
            MAP_SWITCH_LIST_REJECT_DUPLICATE,
            "Duplicate switch-list requests should be rejected as an invariant violation");

        return passed;
    }

    bool TestObjectAccessorLifecycleRules()
    {
        bool passed = true;

        using Skyfire::ObjectAccess::OBJECT_UPDATE_QUEUE_ADD_ALREADY_PRESENT;
        using Skyfire::ObjectAccess::OBJECT_UPDATE_QUEUE_ADD_INSERT;
        using Skyfire::ObjectAccess::OBJECT_UPDATE_QUEUE_REMOVE_ERASE;
        using Skyfire::ObjectAccess::OBJECT_UPDATE_QUEUE_REMOVE_MISSING;
        using Skyfire::ObjectAccess::OBJECT_UPDATE_QUEUE_DRAIN_EMPTY;
        using Skyfire::ObjectAccess::OBJECT_UPDATE_QUEUE_DRAIN_POP;
        using Skyfire::ObjectAccess::OBJECT_CORPSE_OWNER_ADD_ALREADY_MAPPED;
        using Skyfire::ObjectAccess::OBJECT_CORPSE_OWNER_ADD_INSERT;
        using Skyfire::ObjectAccess::OBJECT_CORPSE_OWNER_REMOVE_ERASE;
        using Skyfire::ObjectAccess::OBJECT_CORPSE_OWNER_REMOVE_MISSING;
        using Skyfire::ObjectAccess::OBJECT_CORPSE_STORAGE_UNLOAD_DRAIN;
        using Skyfire::ObjectAccess::OBJECT_CORPSE_STORAGE_UNLOAD_EMPTY;

        passed &= Expect(Skyfire::ObjectAccess::GetUpdateObjectQueueAddAction(false) ==
            OBJECT_UPDATE_QUEUE_ADD_INSERT,
            "ObjectAccessor should insert objects that are not already queued for update");
        passed &= Expect(Skyfire::ObjectAccess::GetUpdateObjectQueueAddAction(true) ==
            OBJECT_UPDATE_QUEUE_ADD_ALREADY_PRESENT,
            "ObjectAccessor should treat duplicate update-object adds as a no-op");

        passed &= Expect(Skyfire::ObjectAccess::GetUpdateObjectQueueRemoveAction(true) ==
            OBJECT_UPDATE_QUEUE_REMOVE_ERASE,
            "ObjectAccessor should erase objects that are present in the update queue");
        passed &= Expect(Skyfire::ObjectAccess::GetUpdateObjectQueueRemoveAction(false) ==
            OBJECT_UPDATE_QUEUE_REMOVE_MISSING,
            "ObjectAccessor should treat missing update-object removes as a no-op");

        passed &= Expect(Skyfire::ObjectAccess::CanBuildUpdateForQueuedObject(true, true),
            "Queued update objects should be buildable only when the object exists and is in world");
        passed &= Expect(!Skyfire::ObjectAccess::CanBuildUpdateForQueuedObject(false, true),
            "ObjectAccessor should not build an update for a missing queued object");
        passed &= Expect(!Skyfire::ObjectAccess::CanBuildUpdateForQueuedObject(true, false),
            "ObjectAccessor should not build an update for an object that is out of world");

        passed &= Expect(Skyfire::ObjectAccess::GetUpdateObjectQueueDrainAction(false) ==
            OBJECT_UPDATE_QUEUE_DRAIN_POP,
            "ObjectAccessor should pop one queued update object while the queue is not empty");
        passed &= Expect(Skyfire::ObjectAccess::GetUpdateObjectQueueDrainAction(true) ==
            OBJECT_UPDATE_QUEUE_DRAIN_EMPTY,
            "ObjectAccessor should stop draining update objects when the queue is empty");

        passed &= Expect(Skyfire::ObjectAccess::GetCorpseOwnerMappingRemoveAction(true) ==
            OBJECT_CORPSE_OWNER_REMOVE_ERASE,
            "ObjectAccessor should erase tracked corpse owner mappings");
        passed &= Expect(Skyfire::ObjectAccess::GetCorpseOwnerMappingRemoveAction(false) ==
            OBJECT_CORPSE_OWNER_REMOVE_MISSING,
            "ObjectAccessor should classify missing corpse owner mappings for diagnostics");

        passed &= Expect(Skyfire::ObjectAccess::GetCorpseOwnerMappingAddAction(false) ==
            OBJECT_CORPSE_OWNER_ADD_INSERT,
            "ObjectAccessor should insert corpse owner mappings that are not already tracked");
        passed &= Expect(Skyfire::ObjectAccess::GetCorpseOwnerMappingAddAction(true) ==
            OBJECT_CORPSE_OWNER_ADD_ALREADY_MAPPED,
            "ObjectAccessor should classify duplicate corpse owner mappings as an invariant violation");

        passed &= Expect(Skyfire::ObjectAccess::GetCorpseStorageUnloadAction(false) ==
            OBJECT_CORPSE_STORAGE_UNLOAD_DRAIN,
            "ObjectAccessor shutdown should drain corpse storage when tracked corpses remain");
        passed &= Expect(Skyfire::ObjectAccess::GetCorpseStorageUnloadAction(true) ==
            OBJECT_CORPSE_STORAGE_UNLOAD_EMPTY,
            "ObjectAccessor shutdown should skip corpse storage drain when storage is empty");

        return passed;
    }

    bool TestWorldShutdownLifecycleRules()
    {
        bool passed = true;

        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_START;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_SCRIPT_SHUTDOWN;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_KICK_PLAYERS;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_UPDATE_SESSIONS;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_DELETE_BATTLEGROUNDS;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_STOP_NETWORK;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_UNLOAD_MAPS;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_UNLOAD_OBJECT_ACCESSOR;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_UNLOAD_SCRIPTS;
        using Skyfire::WorldShutdown::WORLD_SHUTDOWN_OUTDOOR_PVP_DIE;

        passed &= Expect(Skyfire::WorldShutdown::IsExpectedShutdownTransition(WORLD_SHUTDOWN_START, WORLD_SHUTDOWN_SCRIPT_SHUTDOWN),
            "World shutdown should begin with script shutdown hooks");
        passed &= Expect(Skyfire::WorldShutdown::IsExpectedShutdownTransition(WORLD_SHUTDOWN_KICK_PLAYERS, WORLD_SHUTDOWN_UPDATE_SESSIONS),
            "World shutdown should update sessions immediately after kicking players");
        passed &= Expect(Skyfire::WorldShutdown::IsExpectedShutdownTransition(WORLD_SHUTDOWN_STOP_NETWORK, WORLD_SHUTDOWN_UNLOAD_MAPS),
            "World shutdown should stop the network before unloading maps");
        passed &= Expect(Skyfire::WorldShutdown::IsExpectedShutdownTransition(WORLD_SHUTDOWN_UNLOAD_MAPS, WORLD_SHUTDOWN_UNLOAD_OBJECT_ACCESSOR),
            "World shutdown should unload maps before ObjectAccessor corpse storage");

        passed &= Expect(!Skyfire::WorldShutdown::IsExpectedShutdownTransition(WORLD_SHUTDOWN_START, WORLD_SHUTDOWN_UNLOAD_MAPS),
            "World shutdown should reject jumping directly from start to map unload");
        passed &= Expect(!Skyfire::WorldShutdown::IsExpectedShutdownTransition(WORLD_SHUTDOWN_UNLOAD_OBJECT_ACCESSOR, WORLD_SHUTDOWN_UNLOAD_MAPS),
            "World shutdown should reject unloading maps after ObjectAccessor teardown");

        passed &= Expect(Skyfire::WorldShutdown::IsShutdownStepBefore(WORLD_SHUTDOWN_KICK_PLAYERS, WORLD_SHUTDOWN_UPDATE_SESSIONS),
            "KickAll must happen before the session update pass");
        passed &= Expect(Skyfire::WorldShutdown::IsShutdownStepBefore(WORLD_SHUTDOWN_UPDATE_SESSIONS, WORLD_SHUTDOWN_UNLOAD_MAPS),
            "The session update pass must happen before map unload");
        passed &= Expect(Skyfire::WorldShutdown::IsShutdownStepBefore(WORLD_SHUTDOWN_UNLOAD_MAPS, WORLD_SHUTDOWN_UNLOAD_OBJECT_ACCESSOR),
            "Map unload must happen before ObjectAccessor unload");
        passed &= Expect(!Skyfire::WorldShutdown::IsShutdownStepBefore(WORLD_SHUTDOWN_UNLOAD_SCRIPTS, WORLD_SHUTDOWN_DELETE_BATTLEGROUNDS),
            "Script unload should not be classified before battleground deletion");

        passed &= Expect(Skyfire::WorldShutdown::GetShutdownStepName(WORLD_SHUTDOWN_DELETE_BATTLEGROUNDS) ==
            std::string("delete battlegrounds"),
            "Shutdown lifecycle steps should have stable diagnostics labels");
        passed &= Expect(Skyfire::WorldShutdown::GetShutdownStepName(WORLD_SHUTDOWN_OUTDOOR_PVP_DIE) ==
            std::string("outdoor pvp cleanup"),
            "Final shutdown lifecycle step should have a stable diagnostics label");

        return passed;
    }

    bool TestRuntimeMetricsRules()
    {
        bool passed = true;

        Skyfire::Diagnostics::RuntimeMetrics metrics;

        Skyfire::Diagnostics::RuntimeMetricsSnapshot emptySnapshot = metrics.Snapshot();
        passed &= Expect(emptySnapshot.WorldUpdate.SampleCount == 0,
            "Runtime metrics should start with no world update samples");

        metrics.RecordWorldUpdate(12);
        metrics.RecordWorldUpdate(28);
        metrics.RecordWorldUpdate(250);

        Skyfire::Diagnostics::RuntimeMetricsSnapshot worldSnapshot = metrics.Snapshot();
        passed &= Expect(worldSnapshot.WorldUpdate.SampleCount == 3,
            "Runtime metrics should count world update samples");
        passed &= Expect(worldSnapshot.WorldUpdate.Last == 250,
            "Runtime metrics should remember the latest world update diff");
        passed &= Expect(worldSnapshot.WorldUpdate.Average == 96,
            "Runtime metrics should average world update samples");
        passed &= Expect(worldSnapshot.WorldUpdate.Maximum == 250,
            "Runtime metrics should keep the maximum world update sample");
        passed &= Expect(worldSnapshot.WorldUpdate.SlowSamples == 1,
            "Runtime metrics should count slow world update samples");

        metrics.RecordMapUpdatePass(3);
        metrics.RecordMapUpdatePass(5);
        metrics.RecordMapUpdateScheduled(1);
        metrics.RecordMapUpdateScheduled(2);
        metrics.RecordMapUpdateCompleted(1);
        metrics.RecordMapUpdateScheduleFailed(1);
        metrics.RecordMapUpdateWait(7);
        metrics.RecordMapUpdateWait(11);
        metrics.RecordMapUpdateWait(125);

        Skyfire::Diagnostics::RuntimeMetricsSnapshot mapSnapshot = metrics.Snapshot();
        passed &= Expect(mapSnapshot.MapUpdatePasses.SampleCount == 2,
            "Runtime metrics should count map update passes");
        passed &= Expect(mapSnapshot.MapUpdatePasses.Last == 5,
            "Runtime metrics should remember the latest map update pass size");
        passed &= Expect(mapSnapshot.MapUpdatePasses.Average == 4,
            "Runtime metrics should average map update pass sizes");
        passed &= Expect(mapSnapshot.MapUpdatePasses.Maximum == 5,
            "Runtime metrics should keep the largest map update pass size");
        passed &= Expect(mapSnapshot.MapUpdater.Scheduled == 2,
            "Runtime metrics should count scheduled map updates");
        passed &= Expect(mapSnapshot.MapUpdater.Completed == 1,
            "Runtime metrics should count completed map updates");
        passed &= Expect(mapSnapshot.MapUpdater.ScheduleFailures == 1,
            "Runtime metrics should count failed map update schedules");
        passed &= Expect(mapSnapshot.MapUpdater.Pending == 1,
            "Runtime metrics should report current pending map updates");
        passed &= Expect(mapSnapshot.MapUpdater.PendingHighWater == 2,
            "Runtime metrics should keep pending map update high-water");
        passed &= Expect(mapSnapshot.MapUpdater.Wait.SampleCount == 3,
            "Runtime metrics should count map updater wait samples");
        passed &= Expect(mapSnapshot.MapUpdater.Wait.Average == 47,
            "Runtime metrics should average map updater wait samples");
        passed &= Expect(mapSnapshot.MapUpdater.Wait.Maximum == 125,
            "Runtime metrics should keep the largest map updater wait sample");
        passed &= Expect(mapSnapshot.MapUpdater.Wait.SlowSamples == 1,
            "Runtime metrics should count slow map updater waits");

        metrics.RecordWorldSessionPacketQueued(1);
        metrics.RecordWorldSessionPacketQueued(3);
        metrics.RecordWorldSessionPacketQueued(2);
        metrics.RecordWorldSessionPacketProcessed(1);
        metrics.RecordWorldSessionPacketProcessed(0);

        Skyfire::Diagnostics::RuntimeMetricsSnapshot packetSnapshot = metrics.Snapshot();
        passed &= Expect(packetSnapshot.WorldSession.PacketsQueued == 3,
            "Runtime metrics should count queued world session packets");
        passed &= Expect(packetSnapshot.WorldSession.PacketsProcessed == 2,
            "Runtime metrics should count processed world session packets");
        passed &= Expect(packetSnapshot.WorldSession.QueueDepth == 0,
            "Runtime metrics should remember current world session packet queue depth");
        passed &= Expect(packetSnapshot.WorldSession.QueueDepthHighWater == 3,
            "Runtime metrics should keep world session packet queue high-water");
        passed &= Expect(packetSnapshot.WorldSession.QueueDepthHighWaterEvents == 2,
            "Runtime metrics should count world session packet queue high-water events");

        metrics.RecordSpellCastFailure(133, 24, 0, 1234);
        metrics.RecordSpellCastFailure(116, 46, 7, 5678);

        Skyfire::Diagnostics::RuntimeMetricsSnapshot spellSnapshot = metrics.Snapshot();
        passed &= Expect(spellSnapshot.SpellCast.Failures == 2,
            "Runtime metrics should count reported spell cast failures");
        passed &= Expect(spellSnapshot.SpellCast.LastSpellId == 116,
            "Runtime metrics should remember the latest failed spell id");
        passed &= Expect(spellSnapshot.SpellCast.LastFailure == 46,
            "Runtime metrics should remember the latest spell failure reason");
        passed &= Expect(spellSnapshot.SpellCast.LastCustomError == 7,
            "Runtime metrics should remember the latest spell custom error");
        passed &= Expect(spellSnapshot.SpellCast.LastOpcode == 5678,
            "Runtime metrics should remember the latest spell failure opcode");

        std::vector<std::string> lines = Skyfire::Diagnostics::FormatRuntimeMetricLines(spellSnapshot);
        passed &= Expect(lines.size() == 4,
            "Runtime metrics formatting should produce four server info lines");
        passed &= Expect(lines[0].find("World update: samples 3") != std::string::npos,
            "Runtime metrics world line should include sample count");
        passed &= Expect(lines[0].find("slow 1") != std::string::npos,
            "Runtime metrics world line should include slow sample count");
        passed &= Expect(lines[1].find("Map updater: scheduled 2") != std::string::npos,
            "Runtime metrics map line should include scheduled count");
        passed &= Expect(lines[1].find("slow waits 1") != std::string::npos,
            "Runtime metrics map line should include slow wait count");
        passed &= Expect(lines[2].find("Packet queue: queued 3") != std::string::npos,
            "Runtime metrics packet line should include queued packet count");
        passed &= Expect(lines[2].find("high-water events 2") != std::string::npos,
            "Runtime metrics packet line should include queue high-water event count");
        passed &= Expect(lines[3].find("Spell cast failures: count 2") != std::string::npos,
            "Runtime metrics spell line should include failure count");

        metrics.Reset();

        Skyfire::Diagnostics::RuntimeMetricsSnapshot resetSnapshot = metrics.Snapshot();
        passed &= Expect(resetSnapshot.WorldUpdate.SampleCount == 0,
            "Runtime metrics reset should clear world update samples");
        passed &= Expect(resetSnapshot.WorldUpdate.SlowSamples == 0,
            "Runtime metrics reset should clear slow world update samples");
        passed &= Expect(resetSnapshot.MapUpdater.Pending == 0,
            "Runtime metrics reset should clear pending map updates");
        passed &= Expect(resetSnapshot.WorldSession.QueueDepthHighWater == 0,
            "Runtime metrics reset should clear packet queue high-water");
        passed &= Expect(resetSnapshot.WorldSession.QueueDepthHighWaterEvents == 0,
            "Runtime metrics reset should clear packet queue high-water events");
        passed &= Expect(resetSnapshot.SpellCast.Failures == 0,
            "Runtime metrics reset should clear spell cast failure counts");

        return passed;
    }

    bool TestInnAreaBoundsRules()
    {
        bool passed = true;

        Skyfire::Rest::InnAreaBounds lionsPride;
        lionsPride.MapId = 0;
        lionsPride.X = -9467.07f;
        lionsPride.Y = 23.8406f;
        lionsPride.Z = 56.34f;
        lionsPride.BoxX = 60.0f;
        lionsPride.BoxY = 48.0f;
        lionsPride.BoxZ = 20.0f;

        passed &= Expect(Skyfire::Rest::HasInnAreaBounds(lionsPride),
            "Lion's Pride override should be recognized as bounded");
        passed &= Expect(Skyfire::Rest::IsInsideInnArea(lionsPride, 0, -9467.07f, 23.8406f, 56.34f),
            "Lion's Pride center note should be inside the corrected inn footprint");
        passed &= Expect(!Skyfire::Rest::IsInsideInnArea(lionsPride, 0, -9458.70f, 54.82137f, 56.15725f),
            "Lion's Pride threshold trigger point should not be treated as inside the inn");
        passed &= Expect(!Skyfire::Rest::IsInsideInnArea(lionsPride, 0, -9462.162f, 50.40414f, 56.49729f),
            "Lion's Pride outside trigger point should not be treated as inside the inn");

        Skyfire::Rest::InnAreaBounds lakeshire;
        lakeshire.MapId = 0;
        lakeshire.X = -9219.37f;
        lakeshire.Y = -2149.94f;
        lakeshire.Z = 70.606f;
        lakeshire.Radius = 30.0f;

        passed &= Expect(Skyfire::Rest::IsInsideInnArea(lakeshire, 0, -9219.37f, -2149.94f, 70.606f),
            "Radius based tavern footprints should include their center point");
        passed &= Expect(!Skyfire::Rest::IsInsideInnArea(lakeshire, 1, -9219.37f, -2149.94f, 70.606f),
            "Inn footprints should reject positions on a different map");

        Skyfire::Rest::InnAreaBounds empty;
        passed &= Expect(!Skyfire::Rest::HasInnAreaBounds(empty),
            "Empty inn bounds should not be considered a footprint");
        passed &= Expect(!Skyfire::Rest::IsInsideInnArea(empty, 0, 0.0f, 0.0f, 0.0f),
            "Empty inn bounds should not contain positions");

        std::string formattedBounds = Skyfire::Rest::FormatInnAreaBounds(lionsPride);
        passed &= Expect(formattedBounds.find("map=0") != std::string::npos,
            "Formatted inn bounds should include map id");
        passed &= Expect(formattedBounds.find("box=(60.00,48.00,20.00,0.00)") != std::string::npos,
            "Formatted inn bounds should include box dimensions");

        return passed;
    }

    bool TestLegacyTransportVisibilityPreservationRules()
    {
        bool passed = true;

        passed &= Expect(LegacyTransport::ShouldPreservePassengerGameObjectVisibility(218203),
            "Deeprun Tram client entry 218203 should preserve station gameobjects while riding");
        passed &= Expect(LegacyTransport::ShouldPreservePassengerGameObjectVisibility(218208),
            "Deeprun Tram client entry 218208 should preserve station gameobjects while riding");
        passed &= Expect(!LegacyTransport::ShouldPreservePassengerGameObjectVisibility(176080),
            "Database-only Deeprun Tram entry should not be used for passenger visibility preservation");
        passed &= Expect(!LegacyTransport::ShouldPreservePassengerGameObjectVisibility(1),
            "Unrelated transports should not preserve passenger gameobject visibility");

        return passed;
    }

    bool TestAreaTableRootZoneResolutionRules()
    {
        bool passed = true;

        auto parentLookup = [](uint32 areaId) -> uint32
        {
            switch (areaId)
            {
                case 3431: return 6455;
                case 6455: return 3430;
                case 3430: return 0;
                default: return 0;
            }
        };

        passed &= Expect(Skyfire::AreaTable::ResolveRootZoneId(3431, parentLookup) == 3430,
            "Nested AreaTable parents should resolve to the top-level zone");
        passed &= Expect(Skyfire::AreaTable::ResolveRootZoneId(6455, parentLookup) == 3430,
            "Intermediate AreaTable parents should resolve to the top-level zone");
        passed &= Expect(Skyfire::AreaTable::ResolveRootZoneId(3430, parentLookup) == 3430,
            "Root zones should resolve to themselves");

        return passed;
    }

}

int main()
{
    bool passed = true;

    passed &= TestCurrencyFormulaBoundaries();
    passed &= TestWorldPacketContainerBehavior();
    passed &= TestClientOpcodeTableAcceptsGuildAchievementTracking();
    passed &= TestServerOpcodeTableEnablesDisplayGameError();
    passed &= TestWorldStateBuilderPacketLayout();
    passed &= TestThreatSpellModifierRules();
    passed &= TestSpellValidationMasks();
    passed &= TestSpellTargetingRules();
    passed &= TestSpellCalculationRules();
    passed &= TestSpellAuraMetadataRules();
    passed &= TestSpellMovementMetadataRules();
    passed &= TestSpellItemMetadataRules();
    passed &= TestSpellCombatMetadataRules();
    passed &= TestSpellEffectMetadataRules();
    passed &= TestSpellSummonMetadataRules();
    passed &= TestGridAndCellPrimitives();
    passed &= TestMapMoveQueueLifecycleRules();
    passed &= TestMapDelayedListLifecycleRules();
    passed &= TestObjectAccessorLifecycleRules();
    passed &= TestWorldShutdownLifecycleRules();
    passed &= TestRuntimeMetricsRules();
    passed &= TestInnAreaBoundsRules();
    passed &= TestLegacyTransportVisibilityPreservationRules();
    passed &= TestAreaTableRootZoneResolutionRules();

    return passed ? 0 : 1;
}
