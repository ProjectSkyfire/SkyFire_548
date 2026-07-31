/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_SPELL_AURA_METADATA_H
#define SF_SPELL_AURA_METADATA_H

#include "Define.h"

namespace Skyfire
{
namespace Spells
{
    struct ChampioningAuraInfo
    {
        uint32 AuraSpellId;
        uint32 FactionId;
        uint8 ChampioningType;
    };

    enum AuraImmunityMechanicMask
    {
        AURA_IMMUNITY_MECHANIC_MASK_NONE,
        AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL,
        AURA_IMMUNITY_MECHANIC_MASK_STUN,
        AURA_IMMUNITY_MECHANIC_MASK_LIGHT_CONTROL,
        AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL
    };

    struct AuraStateImmunityRule
    {
        int32 MiscValue;
        uint32 RequiredSpellId;
        AuraImmunityMechanicMask MechanicMask;
        uint32 const* Mechanics;
        uint8 MechanicCount;
        uint32 const* AuraTypes;
        uint8 AuraTypeCount;
        uint32 const* Effects;
        uint8 EffectCount;
    };

    struct MechanicImmunitySpellRule
    {
        uint32 AuraSpellId;
        AuraImmunityMechanicMask MechanicMask;
        uint32 const* Mechanics;
        uint8 MechanicCount;
    };

    struct PeriodicBulletControllerRule
    {
        uint32 AuraSpellId;
        uint32 FirstSpellId;
        uint32 SecondSpellId;
        uint8 MaxTargetRoll;
    };

    uint8 const MAX_SHAPESHIFT_BOOST_SPELLS = 4;
    uint8 const MAX_AURA_IMMUNITY_AURA_TYPES = 8;

    uint32 GetQuestTameFinalSpellId(uint32 auraSpellId);
    ChampioningAuraInfo const* GetChampioningAuraInfo(uint32 auraSpellId);
    uint8 GetShapeshiftBoostSpellIds(uint32 shapeshiftForm, uint32* spellIds, uint8 maxSpellIds);
    uint32 GetShadowFocusAuraSpellId();
    uint32 GetShadowFocusTriggeredSpellId();
    uint32 GetNurturingInstinctTriggeredSpellId(uint32 auraSpellId);
    bool TransformDisplayRequiresPlayerRace(uint32 auraSpellId);
    uint32 GetTransformDisplayId(uint32 auraSpellId, uint8 race, uint8 gender);
    uint32 GetUnknownTransformFallbackDisplayId();
    uint32 GetGlyphOfPenguinAuraSpellId();
    uint32 GetGlyphOfPenguinDisplayId();
    uint32 GetDragonmawIllusionAuraSpellId();
    uint32 GetDragonmawIllusionMountDisplayId();
    uint32 GetLeaderOfThePackSpellId();
    uint32 GetLeaderOfThePackTriggeredSpellId();
    bool IsNonAttackablePacifyAura(uint32 auraSpellId);
    uint32 GetRaidProcChargeTriggerSpellId(uint32 auraSpellId);
    bool ShouldBindStackingAuraToCaster(bool canStackWithDifferentCasters, bool casterIsCreature,
        bool casterIsSummon, bool ownerIsSummon);
    uint32 GetNetOMaticRootSelfSpellId();
    uint32 GetKillCommandPetAuraSpellId();
    uint32 GetBloodElfIllusionDisguiseSpellId(uint32 auraSpellId, uint8 gender);
    uint32 GetAuraDummyTriggeredSpellId(uint32 auraSpellId);
    uint32 GetAuraRemoveTriggeredSpellId(uint32 auraSpellId, bool heroic);
    bool IsDespawnOnRemoveAura(uint32 auraSpellId);
    uint32 GetStackingAuraTriggerSpellId(uint32 auraSpellId);
    uint32 GetTaggedMurlocEntry(bool apply);
    uint32 GetLichKingIntroApplySoundId(uint32 auraSpellId);
    uint32 GetLichKingIntroRemoveSoundId(uint32 auraSpellId);
    uint32 GetLichKingIntroContinueSpellId(uint32 auraSpellId);
    uint32 GetFestiveHolidayMountCreatureEntry(bool flying);
    PeriodicBulletControllerRule const* GetPeriodicBulletControllerRule(uint32 auraSpellId);
    uint32 GetPeriodicOverloadCircuitShutdownSpellId(uint32 auraSpellId);
    uint32 GetPeriodicFuseArmorTriggerSpellId(uint32 auraSpellId);
    uint32 GetKillingSpreePeriodicSpellId(uint8 index);
    uint32 GetPeriodicDeathAndDecayTriggerSpellId();
    uint32 GetPeriodicHysteriaAuraSpellId();
    uint32 GetPeriodicTriggerOverrideSpellId(uint32 auraSpellId);
    bool IsPeriodicRestorationAura(uint32 auraSpellId);
    uint32 GetPeriodicRestorationEnergizeSpellId();
    uint32 GetPeriodicFrostBlastTriggerSpellId();
    uint32 GetPeriodicDoomTriggerSpellId();
    uint32 GetSpellclothSummonCreatureEntry();
    uint32 GetAbsorbEyeOfGrillokCasterSpellId();
    uint32 GetTotemicMasteryAuraSpellId();
    uint32 GetTotemicMasteryTriggerSpellId();
    bool IsReagentConsumingPeriodicTriggerAura(uint32 auraSpellId);
    bool IsFullHealthRemovePeriodicDamageAura(uint32 auraSpellId);
    bool IsPctHealthRemovePeriodicDamageAura(uint32 auraSpellId);
    bool IsUnboundPlaguePeriodicDamageAura(uint32 auraSpellId);
    uint32 GetDrainSoulShardSpellId();
    uint32 GetTenacityAuraSpellId();
    uint32 GetWildGrowthTier10AuraSpellId();
    bool IsUnsavableSpecialAura(uint32 auraSpellId);
    bool IsUnsavableDruidEclipseAura(uint32 auraSpellId);
    uint32 GetStonedAuraTriggerSpellId(uint32 auraSpellId);
    uint32 GetDruidTier8RestorationAuraSpellId();
    uint32 GetDruidCatFormSpellId();
    uint32 GetDruidBearFormSpellId();
    uint32 GetPolymorphSheepSpellId();
    uint32 GetPolymorphPenguinSpellId();
    uint32 GetArcanePotencyTriggerSpellId(uint32 auraSpellId);
    uint32 GetGlyphOfPowerWordShieldAuraSpellId();
    uint32 GetGlyphOfPowerWordShieldHealSpellId();
    uint32 GetGlyphOfBlurredSpeedAuraSpellId();
    uint32 GetGlyphOfBlurredSpeedSprintSpellId();
    uint32 GetForbearanceAuraSpellId();
    uint32 GetSharedSufferingTriggerSpellId();
    uint32 GetInvisibilityFadeSpellId();
    uint32 GetImprovedFearTriggerSpellId(uint32 auraSpellId);
    uint32 GetRaptureEnergizeSpellId();
    uint32 GetGlyphOfFreezingTrapAuraSpellId();
    uint32 GetGlyphOfFreezingTrapTriggerSpellId();
    uint32 GetProwlAuraSpellId();
    uint32 GetStealthAuraSpellId();
    uint32 GetDruidFeralTier10BonusAuraSpellId();
    uint32 GetDruidFeralTier10BonusTriggerSpellId();
    uint32 GetBestialWrathAuraSpellId();
    uint32 GetTheBeastWithinTalentSpellId();
    uint32 GetTheBeastWithinTriggerSpellId();
    uint32 GetDivineFavorAuraSpellId();
    uint32 GetPaladinTier10HolyBonusAuraSpellId();
    uint32 GetPaladinTier10HolyBonusTriggerSpellId();
    AuraStateImmunityRule const* GetAuraStateImmunityRule(int32 miscValue, uint32 auraSpellId, int32 amount);
    uint8 GetDefaultAuraStateImmunityAuraTypes(int32 miscValue, uint32* auraTypes, uint8 maxAuraTypes);
    MechanicImmunitySpellRule const* GetMechanicImmunitySpellRule(uint32 auraSpellId);
    uint32 GetAuraImmunityMechanicMask(AuraImmunityMechanicMask mechanicMask);
}
}

#endif
