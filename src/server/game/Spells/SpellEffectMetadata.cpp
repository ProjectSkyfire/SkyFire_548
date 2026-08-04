/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SpellEffectMetadata.h"

namespace Skyfire
{
namespace Spells
{
    namespace
    {
        struct SpellIdPair
        {
            uint32 SpellId;
            uint32 RelatedSpellId;
        };

        struct UInt32Pair
        {
            uint32 Key;
            uint32 Value;
        };

        struct DisplaySpellInfo
        {
            uint32 DisplayId;
            uint32 SpellId;
        };

        uint32 const SPELL_VANISH_AURA = 11327;
        uint32 const SPELL_LESSER_INVISIBILITY = 7870;
        uint32 const SPELL_BRITTLE_ARMOR_STACK = 24575;
        uint32 const SPELL_MERCURIAL_SHIELD_STACK = 26464;
        uint32 const SPELL_RESTRICTED_FLIGHT_AREA = 91604;
        uint32 const SPELL_BURN_BODY = 42793;
        uint32 const NPC_FALLEN_COMBATANT = 24008;
        uint32 const SPELL_BENDING_SHINBONE_STRONG = 8854;
        uint32 const SPELL_BENDING_SHINBONE_WEAK = 8855;
        uint32 const SPELL_SHADOW_FLAME_PROTECTION = 22683;
        uint32 const SPELL_SHADOW_FLAME_TRIGGER = 22682;
        uint32 const SPELL_DECIMATE_TRIGGER = 28375;
        uint32 const EVENT_WARMAUL_OGRE_BANNER = 18388;
        uint32 const ITEM_MUG_TRANSFORMATION = 38587;
        uint32 const SPELL_BREWFEST_SAMPLE = 42518;
        uint32 const SPELL_BRUTALLUS_BURN_RANGE = 46394;
        uint32 const COPPER_5000_GOLD = 5000;
        uint32 const SPELL_EMBLAZON_RUNEBLADE_TRIGGER = 51771;
        uint32 const SPELL_DEATHBOLT_WARD = 51845;
        uint32 const SPELL_DEATHBOLT_REFLECTED = 51856;
        uint32 const SPELL_DEATHBOLT_DIRECT = 51855;
        uint32 const SPELL_SUMMON_GHOULS_ON_SCARLET_CRUSADE = 54522;
        uint32 const SPELL_GIFT_OF_THE_HARVESTER_FALLBACK = 52505;
        uint32 const WINTERGRASP_RPGG_DROP_ITEM_COUNT = 5;
        uint32 const SPELL_BIG_BLIZZARD_BEAR_EPIC = 58999;
        uint32 const SPELL_BIG_BLIZZARD_BEAR_NORMAL = 58997;
        uint32 const AREA_TELEPORTING_TOP = 4637;
        uint32 const SPELL_TELEPORTING_RETURN = 59316;
        uint32 const SPELL_TELEPORTING_ATOP = 59314;
        uint32 const SPELL_GRAB_CRATE_PARENT = 62496;
        uint32 const SPELL_LIGHTWELL_CHARGES = 59907;
        uint32 const SPELL_JUDGEMENT_FALLBACK_SEAL_TRIGGER = 54158;
        uint32 const SPELL_PESTILENCE_BLOOD_PLAGUE = 55078;
        uint32 const SPELL_PESTILENCE_FROST_FEVER = 55095;
        uint32 const SPELL_CREATE_HEALTHSTONE_TRIGGER = 23517;

        TriggerSpellRule const TriggerSpellRules[] =
        {
            // 1856 triggers missing Spell.dbc entry 18461; rule key stays 18461 and applies Vanish aura 11327
            { 18461, TRIGGER_SPELL_RULE_VANISH, SPELL_VANISH_AURA, 0 },
            { 54437, TRIGGER_SPELL_RULE_DEMONIC_EMPOWERMENT_SUCCUBUS, SPELL_LESSER_INVISIBILITY, 0 },
            { 29284, TRIGGER_SPELL_RULE_STACK_TRIGGER, 0, SPELL_BRITTLE_ARMOR_STACK },
            { 29286, TRIGGER_SPELL_RULE_STACK_TRIGGER, 0, SPELL_MERCURIAL_SHIELD_STACK },
            { 35729, TRIGGER_SPELL_RULE_CLOAK_OF_SHADOWS, 0, 0 }
        };

        ForceCastDamageRule const ForceCastDamageRules[] =
        {
            { 52588, FORCE_CAST_DAMAGE_RULE_REMOVE_AURA },
            { 48598, FORCE_CAST_DAMAGE_RULE_REMOVE_AURA },
            { 52463, FORCE_CAST_DAMAGE_RULE_CUSTOM_BASEPOINTS },
            { 52349, FORCE_CAST_DAMAGE_RULE_CUSTOM_BASEPOINTS },
            { 72299, FORCE_CAST_DAMAGE_RULE_TRIGGERED_ORIGINAL_CASTER }
        };

        uint32 const LearnSpellFromEffectDamageSpells[] =
        {
            483,
            55884
        };

        SpellIdPair const ScriptEffectStackRemovalAuras[] =
        {
            { 24590, SPELL_BRITTLE_ARMOR_STACK },
            { 26465, SPELL_MERCURIAL_SHIELD_STACK }
        };

        uint32 const ShadowFlameScriptSpells[] =
        {
            22539,
            22972,
            22975,
            22976,
            22977,
            22978,
            22979,
            22980,
            22981,
            22982,
            22983,
            22984,
            22985
        };

        uint32 const DecimateScriptSpells[] =
        {
            28374,
            54426
        };

        UInt32Pair const MirrensDrinkingHatItems[] =
        {
            { 1, 23584 },
            { 2, 23584 },
            { 3, 23584 },
            { 4, 23585 },
            { 5, 23585 },
            { 6, 23586 }
        };

        UInt32Pair const GoblinWeatherMachineSpells[] =
        {
            { 0, 46740 },
            { 1, 46739 },
            { 2, 46738 },
            { 3, 46736 }
        };

        DisplaySpellInfo const DeathKnightInitiateVisualSpells[] =
        {
            { 25369, 51552 },
            { 25373, 51551 },
            { 25363, 51542 },
            { 25357, 51541 },
            { 25361, 51537 },
            { 25356, 51538 },
            { 25372, 51550 },
            { 25367, 51549 },
            { 25362, 51540 },
            { 25359, 51539 },
            { 25355, 51534 },
            { 25354, 51520 },
            { 25360, 51536 },
            { 25358, 51535 },
            { 25368, 51544 },
            { 25364, 51543 },
            { 25371, 51548 },
            { 25366, 51547 },
            { 25370, 51545 },
            { 25365, 51546 }
        };

        uint32 const ScriptDespawnSpells[] =
        {
            52173,
            60243
        };

        SpellIdPair const LightwellHealSpells[] =
        {
            { 31897, 7001 },
            { 31896, 27873 },
            { 31895, 27874 },
            { 31894, 28276 },
            { 31893, 48084 },
            { 31883, 48085 }
        };

        ProtoTypicalShorteningBlasterSpells const ProtoTypicalShorteningBlasterSpellPairs[] =
        {
            { 45674, 45673 },
            { 45675, 45672 },
            { 45678, 45677 },
            { 45682, 45681 },
            { 45684, 45683 }
        };

        SpellIdPair const JudgementSealTriggerSpells[] =
        {
            { 53407, 20184 },
            { 20271, 20185 },
            { 57774, 20185 },
            { 53408, 20186 }
        };

        uint32 const JudgementFallbackSealSpells[] =
        {
            20165,
            20166,
            20164
        };

        uint32 const CreateHealthstoneSpells[] =
        {
            6201,
            34130
        };

        bool ContainsSpellId(uint32 const* spellIds, uint8 spellCount, uint32 spellId)
        {
            for (uint8 i = 0; i < spellCount; ++i)
                if (spellIds[i] == spellId)
                    return true;

            return false;
        }
    }

    TriggerSpellRule const* GetTriggerSpellRule(uint32 triggerSpellId)
    {
        for (TriggerSpellRule const& rule : TriggerSpellRules)
            if (rule.TriggerSpellId == triggerSpellId)
                return &rule;

        return nullptr;
    }

    ForceCastDamageRule const* GetForceCastDamageRule(uint32 spellId)
    {
        for (ForceCastDamageRule const& rule : ForceCastDamageRules)
            if (rule.SpellId == spellId)
                return &rule;

        return nullptr;
    }

    bool ShouldSendNoFlyZoneNotification(uint32 spellId)
    {
        return spellId == SPELL_RESTRICTED_FLIGHT_AREA;
    }

    uint32 GetKillCreditFallbackCreatureEntry(uint32 spellId)
    {
        return spellId == SPELL_BURN_BODY ? NPC_FALLEN_COMBATANT : 0;
    }

    bool ShouldLearnSpellFromEffectDamage(uint32 spellId)
    {
        for (uint32 const damageSpellId : LearnSpellFromEffectDamageSpells)
            if (damageSpellId == spellId)
                return true;

        return false;
    }

    uint32 GetBendingShinboneTriggerSpellId(bool strongHit)
    {
        return strongHit ? SPELL_BENDING_SHINBONE_STRONG : SPELL_BENDING_SHINBONE_WEAK;
    }

    uint32 GetScriptEffectStackRemovalAuraSpellId(uint32 scriptSpellId)
    {
        for (SpellIdPair const& stackRemoval : ScriptEffectStackRemovalAuras)
            if (stackRemoval.SpellId == scriptSpellId)
                return stackRemoval.RelatedSpellId;

        return 0;
    }

    ShadowFlameScriptRule const* GetShadowFlameScriptRule(uint32 scriptSpellId)
    {
        if (!ContainsSpellId(ShadowFlameScriptSpells, sizeof(ShadowFlameScriptSpells) / sizeof(uint32), scriptSpellId))
            return nullptr;

        static ShadowFlameScriptRule const rule =
        {
            SPELL_SHADOW_FLAME_PROTECTION,
            SPELL_SHADOW_FLAME_TRIGGER
        };
        return &rule;
    }

    uint32 GetDecimateScriptTriggerSpellId(uint32 scriptSpellId)
    {
        return ContainsSpellId(DecimateScriptSpells, sizeof(DecimateScriptSpells) / sizeof(uint32), scriptSpellId) ?
            SPELL_DECIMATE_TRIGGER : 0;
    }

    uint32 GetMirrensDrinkingHatItemId(uint32 roll)
    {
        for (UInt32Pair const& item : MirrensDrinkingHatItems)
            if (item.Key == roll)
                return item.Value;

        return 0;
    }

    uint32 GetPlantWarmaulOgreBannerEventId()
    {
        return EVENT_WARMAUL_OGRE_BANNER;
    }

    uint32 GetMugTransformationItemId()
    {
        return ITEM_MUG_TRANSFORMATION;
    }

    uint32 GetBrewfestSampleSpellId()
    {
        return SPELL_BREWFEST_SAMPLE;
    }

    uint32 GetBrutallusBurnRangeAuraSpellId()
    {
        return SPELL_BRUTALLUS_BURN_RANGE;
    }

    uint32 GetGoblinWeatherMachineSpellId(uint32 roll)
    {
        for (UInt32Pair const& spell : GoblinWeatherMachineSpells)
            if (spell.Key == roll)
                return spell.Value;

        return 0;
    }

    uint32 GetGoldScriptAmount()
    {
        return COPPER_5000_GOLD;
    }

    uint32 GetEmblazonRunebladeTriggerSpellId()
    {
        return SPELL_EMBLAZON_RUNEBLADE_TRIGGER;
    }

    uint32 GetDeathKnightInitiateVisualSpellId(uint32 displayId)
    {
        for (DisplaySpellInfo const& visualSpell : DeathKnightInitiateVisualSpells)
            if (visualSpell.DisplayId == displayId)
                return visualSpell.SpellId;

        return 0;
    }

    uint32 GetDeathboltWardAuraSpellId()
    {
        return SPELL_DEATHBOLT_WARD;
    }

    uint32 GetDeathboltReflectedSpellId()
    {
        return SPELL_DEATHBOLT_REFLECTED;
    }

    uint32 GetDeathboltDirectSpellId()
    {
        return SPELL_DEATHBOLT_DIRECT;
    }

    uint32 GetSummonGhoulsOnScarletCrusadeSpellId()
    {
        return SPELL_SUMMON_GHOULS_ON_SCARLET_CRUSADE;
    }

    bool IsScriptDespawnSpell(uint32 scriptSpellId)
    {
        return ContainsSpellId(ScriptDespawnSpells, sizeof(ScriptDespawnSpells) / sizeof(uint32), scriptSpellId);
    }

    uint32 GetGiftOfTheHarvesterFallbackSpellId()
    {
        return SPELL_GIFT_OF_THE_HARVESTER_FALLBACK;
    }

    uint32 GetWintergraspRpggDropItemCount()
    {
        return WINTERGRASP_RPGG_DROP_ITEM_COUNT;
    }

    uint32 GetBigBlizzardBearMountSpellId(uint16 ridingSkill)
    {
        return ridingSkill >= 150 ? SPELL_BIG_BLIZZARD_BEAR_EPIC : SPELL_BIG_BLIZZARD_BEAR_NORMAL;
    }

    uint32 GetTeleportingScriptSpellId(uint32 areaId)
    {
        return areaId == AREA_TELEPORTING_TOP ? SPELL_TELEPORTING_RETURN : SPELL_TELEPORTING_ATOP;
    }

    uint32 GetGrabCrateParentSpellId()
    {
        return SPELL_GRAB_CRATE_PARENT;
    }

    uint32 GetLightwellHealSpellId(uint32 creatureEntry)
    {
        for (SpellIdPair const& healSpell : LightwellHealSpells)
            if (healSpell.SpellId == creatureEntry)
                return healSpell.RelatedSpellId;

        return 0;
    }

    uint32 GetLightwellChargesAuraSpellId()
    {
        return SPELL_LIGHTWELL_CHARGES;
    }

    ProtoTypicalShorteningBlasterSpells const* GetProtoTypicalShorteningBlasterSpells(uint32 roll)
    {
        if (roll >= sizeof(ProtoTypicalShorteningBlasterSpellPairs) / sizeof(ProtoTypicalShorteningBlasterSpells))
            return nullptr;

        return &ProtoTypicalShorteningBlasterSpellPairs[roll];
    }

    uint32 GetJudgementSealTriggerSpellId(uint32 judgementSpellId)
    {
        for (SpellIdPair const& sealTrigger : JudgementSealTriggerSpells)
            if (sealTrigger.SpellId == judgementSpellId)
                return sealTrigger.RelatedSpellId;

        return 0;
    }

    uint32 GetJudgementFallbackSealTriggerSpellId(uint32 sealSpellId)
    {
        return ContainsSpellId(JudgementFallbackSealSpells, sizeof(JudgementFallbackSealSpells) / sizeof(uint32),
            sealSpellId) ? SPELL_JUDGEMENT_FALLBACK_SEAL_TRIGGER : 0;
    }

    uint32 GetPestilenceBloodPlagueSpellId()
    {
        return SPELL_PESTILENCE_BLOOD_PLAGUE;
    }

    uint32 GetPestilenceFrostFeverSpellId()
    {
        return SPELL_PESTILENCE_FROST_FEVER;
    }

    uint32 GetCreateHealthstoneTriggeredSpellId(uint32 createHealthstoneSpellId)
    {
        return ContainsSpellId(CreateHealthstoneSpells, sizeof(CreateHealthstoneSpells) / sizeof(uint32),
            createHealthstoneSpellId) ? SPELL_CREATE_HEALTHSTONE_TRIGGER : 0;
    }
}
}
