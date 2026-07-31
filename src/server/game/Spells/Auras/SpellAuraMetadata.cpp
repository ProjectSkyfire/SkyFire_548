/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SpellAuraMetadata.h"
#include "SpellAuraDefines.h"
#include "SpellValidation.h"

namespace Skyfire
{
namespace Spells
{
    namespace
    {
        struct SpellIdPair
        {
            uint32 AuraSpellId;
            uint32 TriggerSpellId;
        };

        struct ShapeshiftBoostSpellInfo
        {
            uint32 ShapeshiftForm;
            uint32 SpellIds[MAX_SHAPESHIFT_BOOST_SPELLS];
            uint8 SpellCount;
        };

        struct RaceGenderDisplayInfo
        {
            uint8 Race;
            uint32 MaleDisplayId;
            uint32 FemaleDisplayId;
        };

        struct TransformDisplayInfo
        {
            uint32 AuraSpellId;
            bool RequiresPlayerRace;
            RaceGenderDisplayInfo const* RaceDisplays;
            uint8 RaceDisplayCount;
            uint32 MaleDisplayId;
            uint32 FemaleDisplayId;
        };

        enum AuraImmunityAmountCondition
        {
            AURA_IMMUNITY_AMOUNT_ANY,
            AURA_IMMUNITY_AMOUNT_NONZERO,
            AURA_IMMUNITY_AMOUNT_ZERO,
            AURA_IMMUNITY_AMOUNT_ONE
        };

        struct AuraStateImmunityRuleInfo
        {
            AuraStateImmunityRule Rule;
            AuraImmunityAmountCondition AmountCondition;
        };

        struct GenderedAuraSpellInfo
        {
            uint32 AuraSpellId;
            uint32 FemaleSpellId;
            uint32 MaleSpellId;
        };

        struct AuraRemoveTriggerSpellInfo
        {
            uint32 AuraSpellId;
            uint32 NormalSpellId;
            uint32 HeroicSpellId;
        };

        struct LichKingIntroVoiceoverInfo
        {
            uint32 AuraSpellId;
            uint32 ApplySoundId;
            uint32 RemoveSoundId;
            uint32 ContinueSpellId;
        };

        enum ShapeshiftFormId
        {
            SHAPESHIFT_FORM_CAT = 0x01,
            SHAPESHIFT_FORM_TRAVEL = 0x03,
            SHAPESHIFT_FORM_AQUA = 0x04,
            SHAPESHIFT_FORM_BEAR = 0x05,
            SHAPESHIFT_FORM_BATTLE_STANCE = 0x11,
            SHAPESHIFT_FORM_DEFENSIVE_STANCE = 0x12,
            SHAPESHIFT_FORM_BERSERKER_STANCE = 0x13,
            SHAPESHIFT_FORM_METAMORPHOSIS = 0x16,
            SHAPESHIFT_FORM_FLIGHT_EPIC = 0x1B,
            SHAPESHIFT_FORM_SHADOW = 0x1C,
            SHAPESHIFT_FORM_FLIGHT = 0x1D,
            SHAPESHIFT_FORM_MOONKIN = 0x1F,
            SHAPESHIFT_FORM_SPIRIT_OF_REDEMPTION = 0x20
        };

        uint32 const SPELL_SHADOW_FOCUS_DUMMY_TALENT = 108209;
        uint32 const SPELL_SHADOW_FOCUS_AURA = 112942;
        uint8 const RACE_HUMAN_ID = 1;
        uint8 const RACE_ORC_ID = 2;
        uint8 const RACE_DWARF_ID = 3;
        uint8 const RACE_NIGHTELF_ID = 4;
        uint8 const RACE_UNDEAD_PLAYER_ID = 5;
        uint8 const RACE_TAUREN_ID = 6;
        uint8 const RACE_GNOME_ID = 7;
        uint8 const RACE_TROLL_ID = 8;
        uint8 const RACE_BLOODELF_ID = 10;
        uint8 const RACE_DRAENEI_ID = 11;
        uint8 const GENDER_MALE_ID = 0;
        uint8 const GENDER_FEMALE_ID = 1;
        uint32 const DISPLAY_UNKNOWN_TRANSFORM_FALLBACK = 16358;
        uint32 const SPELL_GLYPH_OF_THE_PENGUIN = 52648;
        uint32 const DISPLAY_GLYPH_OF_THE_PENGUIN = 26452;
        uint32 const SPELL_DRAGONMAW_ILLUSION = 42016;
        uint32 const DISPLAY_DRAGONMAW_ILLUSION_MOUNT = 16314;
        uint32 const SPELL_LEADER_OF_THE_PACK = 17007;
        uint32 const SPELL_LEADER_OF_THE_PACK_TRIGGERED = 24932;
        uint32 const SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT = 45839;
        uint32 const SPELL_NET_O_MATIC_SELF_ROOT = 13138;
        uint32 const SPELL_PERIODIC_DEATH_AND_DECAY_TRIGGER = 52212;
        uint32 const SPELL_PERIODIC_HYSTERIA = 49016;
        uint32 const SPELL_PERIODIC_RESTORATION_ENERGIZE = 23493;
        uint32 const SPELL_PERIODIC_FROST_BLAST_TRIGGER = 29879;
        uint32 const SPELL_PERIODIC_DOOM_TRIGGER = 31350;
        uint32 const NPC_SPELLCLOTH_ELEMENTAL = 17870;
        uint32 const SPELL_EYE_OF_GRILLOK = 38495;
        uint32 const SPELL_TOTEMIC_MASTERY_AURA = 38443;
        uint32 const SPELL_TOTEMIC_MASTERY_TRIGGER = 38437;
        uint32 const SPELL_DRAIN_SOUL_SHARD = 95810;
        uint32 const SPELL_TENACITY = 58549;
        uint32 const SPELL_WILD_GROWTH_T10_RESTORATION = 70658;
        uint32 const SPELL_DRUID_T8_RESTORATION = 64760;
        uint32 const SPELL_DRUID_CAT_FORM = 768;
        uint32 const SPELL_DRUID_BEAR_FORM = 5487;
        uint32 const SPELL_POLYMORPH_SHEEP_SOUND = 61634;
        uint32 const SPELL_POLYMORPH_PENGUIN_SOUND = 61635;
        uint32 const SPELL_GLYPH_OF_POWER_WORD_SHIELD = 55672;
        uint32 const SPELL_GLYPH_OF_POWER_WORD_SHIELD_HEAL = 56160;
        uint32 const SPELL_GLYPH_OF_BLURRED_SPEED = 58039;
        uint32 const SPELL_GLYPH_OF_BLURRED_SPEED_SPRINT = 61922;
        uint32 const SPELL_FORBEARANCE = 25771;
        uint32 const SPELL_SHARED_SUFFERING_TRIGGER = 72373;
        uint32 const SPELL_INVISIBILITY_FADE = 32612;
        uint32 const SPELL_RAPTURE_ENERGIZE = 47755;
        uint32 const SPELL_GLYPH_OF_FREEZING_TRAP = 56845;
        uint32 const SPELL_GLYPH_OF_FREEZING_TRAP_TRIGGER = 61394;
        uint32 const SPELL_PROWL = 5215;
        uint32 const SPELL_STEALTH = 1784;
        uint32 const SPELL_DRUID_T10_FERAL = 70726;
        uint32 const SPELL_DRUID_T10_FERAL_TRIGGER = 70725;
        uint32 const SPELL_BESTIAL_WRATH = 19574;
        uint32 const SPELL_THE_BEAST_WITHIN_TALENT = 34692;
        uint32 const SPELL_THE_BEAST_WITHIN_TRIGGER = 34471;
        uint32 const SPELL_DIVINE_FAVOR = 31842;
        uint32 const SPELL_PALADIN_T10_HOLY = 70755;
        uint32 const SPELL_PALADIN_T10_HOLY_TRIGGER = 71166;
        uint32 const NPC_TAGGED_BLACKSILT_SCOUT = 17654;
        uint32 const NPC_UNTAGGED_BLACKSILT_SCOUT = 17326;
        uint32 const NPC_FESTIVE_HOLIDAY_MOUNT_FLYING = 24906;
        uint32 const NPC_FESTIVE_HOLIDAY_MOUNT_GROUND = 15665;

        uint32 const CrowdControlMechanics[] =
        {
            MECHANIC_SNARE,
            MECHANIC_ROOT,
            MECHANIC_FEAR,
            MECHANIC_STUN,
            MECHANIC_SLEEP,
            MECHANIC_CHARM,
            MECHANIC_SAPPED,
            MECHANIC_HORROR,
            MECHANIC_POLYMORPH,
            MECHANIC_DISORIENTED,
            MECHANIC_FREEZE,
            MECHANIC_TURN
        };

        uint32 const StunMechanics[] =
        {
            MECHANIC_STUN
        };

        uint32 const LightControlMechanics[] =
        {
            MECHANIC_SNARE,
            MECHANIC_STUN,
            MECHANIC_DISORIENTED,
            MECHANIC_FREEZE
        };

        uint32 const MovementLossControlAppliedMechanics[] =
        {
            MECHANIC_CHARM,
            MECHANIC_DISORIENTED,
            MECHANIC_FEAR,
            MECHANIC_ROOT,
            MECHANIC_SLEEP,
            MECHANIC_SNARE,
            MECHANIC_STUN,
            MECHANIC_FREEZE,
            MECHANIC_KNOCKOUT,
            MECHANIC_POLYMORPH,
            MECHANIC_BANISH,
            MECHANIC_SHACKLE,
            MECHANIC_TURN,
            MECHANIC_HORROR,
            MECHANIC_DAZE,
            MECHANIC_SAPPED
        };

        uint32 const CrowdControlAuraTypes[] =
        {
            SPELL_AURA_MOD_STUN,
            SPELL_AURA_MOD_DECREASE_SPEED,
            SPELL_AURA_MOD_ROOT,
            SPELL_AURA_MOD_CONFUSE,
            SPELL_AURA_MOD_FEAR,
            SPELL_AURA_MOD_FEAR_2
        };

        uint32 const StunAuraTypes[] =
        {
            SPELL_AURA_MOD_STUN
        };

        uint32 const TauntAuraTypes[] =
        {
            SPELL_AURA_MOD_TAUNT
        };

        uint32 const LightControlAuraTypes[] =
        {
            SPELL_AURA_MOD_STUN,
            SPELL_AURA_MOD_DECREASE_SPEED
        };

        uint32 const AttackMeEffects[] =
        {
            SPELL_EFFECT_ATTACK_ME
        };

        uint32 const KnockbackEffects[] =
        {
            SPELL_EFFECT_KNOCK_BACK,
            SPELL_EFFECT_KNOCK_BACK_DEST
        };

        uint8 const CrowdControlMechanicCount = sizeof(CrowdControlMechanics) / sizeof(uint32);
        uint8 const StunMechanicCount = sizeof(StunMechanics) / sizeof(uint32);
        uint8 const LightControlMechanicCount = sizeof(LightControlMechanics) / sizeof(uint32);
        uint8 const MovementLossControlAppliedMechanicCount = sizeof(MovementLossControlAppliedMechanics) / sizeof(uint32);
        uint8 const CrowdControlAuraTypeCount = sizeof(CrowdControlAuraTypes) / sizeof(uint32);
        uint8 const StunAuraTypeCount = sizeof(StunAuraTypes) / sizeof(uint32);
        uint8 const TauntAuraTypeCount = sizeof(TauntAuraTypes) / sizeof(uint32);
        uint8 const LightControlAuraTypeCount = sizeof(LightControlAuraTypes) / sizeof(uint32);
        uint8 const AttackMeEffectCount = sizeof(AttackMeEffects) / sizeof(uint32);
        uint8 const KnockbackEffectCount = sizeof(KnockbackEffects) / sizeof(uint32);

        SpellIdPair const QuestTameFinalSpells[] =
        {
            { 19548, 19597 },
            { 19674, 19677 },
            { 19687, 19676 },
            { 19688, 19678 },
            { 19689, 19679 },
            { 19692, 19680 },
            { 19693, 19684 },
            { 19694, 19681 },
            { 19696, 19682 },
            { 19697, 19683 },
            { 19699, 19685 },
            { 19700, 19686 },
            { 30099, 30100 },
            { 30102, 30103 },
            { 30105, 30104 },
            { 30646, 30647 },
            { 30653, 30648 },
            { 30654, 30652 }
        };

        ChampioningAuraInfo const ChampioningAuras[] =
        {
            { 57819, 1106, 1 }, // Argent Champion - Argent Crusade
            { 57820, 1098, 1 }, // Ebon Champion - Knights of the Ebon Blade
            { 57821, 1090, 1 }, // Champion of the Kirin Tor
            { 57822, 1091, 1 }, // Wyrmrest Champion - The Wyrmrest Accord
            { 93337, 1173, 2 }, // Champion of Ramkahen
            { 93339, 1135, 2 }, // Champion of the Earthen Ring
            { 93341, 1158, 2 }, // Champion of the Guardians of Hyjal
            { 93347, 1171, 2 }, // Champion of Therazane
            { 93368, 1174, 2 }, // Champion of the Wildhammer Clan
            { 93795,   72, 4 }, // Stormwind Champion
            { 93805,   47, 4 }, // Ironforge Champion
            { 93806,   69, 4 }, // Darnassus Champion
            { 93811,  930, 4 }, // Exodar Champion
            { 93816, 1134, 4 }, // Gilneas Champion
            { 93821,   54, 4 }, // Gnomeregan Champion
            { 93825,   76, 4 }, // Orgrimmar Champion
            { 93827,  530, 4 }, // Darkspear Champion
            { 93828,  911, 4 }, // Silvermoon Champion
            { 93830, 1133, 4 }, // Bilgewater Champion
            { 94158, 1172, 2 }, // Champion of the Dragonmaw Clan
            { 94462,   68, 4 }, // Undercity Champion
            { 94463,   81, 4 }, // Thunder Bluff Champion
            { 126434, 1353, 4 }, // Tushui Champion
            { 126436, 1352, 4 }  // Huojin Champion
        };

        ShapeshiftBoostSpellInfo const ShapeshiftBoostSpells[] =
        {
            { SHAPESHIFT_FORM_CAT,                  {  3025, 48629, 106840, 113636 }, 4 },
            { SHAPESHIFT_FORM_TRAVEL,               {  5419,     0,      0,      0 }, 1 },
            { SHAPESHIFT_FORM_AQUA,                 {  5421,     0,      0,      0 }, 1 },
            { SHAPESHIFT_FORM_BEAR,                 {  1178, 21178, 106829, 106899 }, 4 },
            { SHAPESHIFT_FORM_BATTLE_STANCE,        { 21156,     0,      0,      0 }, 1 },
            { SHAPESHIFT_FORM_DEFENSIVE_STANCE,     {  7376,     0,      0,      0 }, 1 },
            { SHAPESHIFT_FORM_BERSERKER_STANCE,     {  7381,     0,      0,      0 }, 1 },
            { SHAPESHIFT_FORM_MOONKIN,              { 24905, 24907,      0,      0 }, 2 },
            { SHAPESHIFT_FORM_FLIGHT,               { 33948, 34764,      0,      0 }, 2 },
            { SHAPESHIFT_FORM_FLIGHT_EPIC,          { 40122, 40121,      0,      0 }, 2 },
            { SHAPESHIFT_FORM_METAMORPHOSIS,        { 54817, 54879,      0,      0 }, 2 },
            { SHAPESHIFT_FORM_SPIRIT_OF_REDEMPTION, { 27792, 27795,      0,      0 }, 2 },
            { SHAPESHIFT_FORM_SHADOW,               { 49868,     0,      0,      0 }, 1 }
        };

        SpellIdPair const NurturingInstinctTriggeredSpells[] =
        {
            { 33873, 47180 }
        };

        SpellIdPair const RaidProcChargeTriggeredSpells[] =
        {
            { 57949, 57952 }, // Shiver; animation spell 57951 has unknown use (see also 41637)
            { 59978, 59979 }, // Shiver
            { 43593, 43594 }  // Cold Stare
        };

        GenderedAuraSpellInfo const BloodElfIllusionDisguises[] =
        {
            { 37096, 37095, 37093 },
            { 46354, 46356, 46355 }
        };

        SpellIdPair const AuraDummyTriggeredSpells[] =
        {
            { 39850, 51581 }, // Rocket Blast backfire
            { 51701, 51699 }, // Honor Among Thieves
            { 71563, 71564 }  // Lucky Love Rocket
        };

        AuraRemoveTriggerSpellInfo const AuraRemoveTriggeredSpells[] =
        {
            { 36730, 36731,     0 }, // Flame Strike
            { 44191, 44190, 46163 }, // Flame Strike
            { 46308, 47287,     0 }, // Burning Winds
            { 91604, 58601,     0 }  // Restricted Flight Area
        };

        uint32 const DespawnOnRemoveAuras[] =
        {
            52172,
            60244,
            130067,
            130070,
            130108,
            130112,
            130119,
            130156,
            130484,
            148596
        };

        SpellIdPair const StackingAuraTriggeredSpells[] =
        {
            { 24658, 24659 }, // Unstable Power
            { 24661, 24662 }  // Restless Strength
        };

        LichKingIntroVoiceoverInfo const LichKingIntroVoiceovers[] =
        {
            { 58204, 14970,     0, 58205 },
            { 58205, 14971, 14972,     0 }
        };

        PeriodicBulletControllerRule const PeriodicBulletControllerRules[] =
        {
            { 66149, 66152, 66153, 6 },
            { 68396, 66152, 66153, 6 }
        };

        SpellIdPair const PeriodicTriggerOverrideSpells[] =
        {
            { 23170, 23171 }, // Brood Affliction: Bronze
            { 29917, 29916 }, // Feed Captured Animal
            { 30576, 30571 }, // Quake
            { 37027, 37029 }, // Remote Toy
            { SPELL_EYE_OF_GRILLOK, 38530 }, // Eye of Grillok
            { 39857, 39856 }, // Tear of Azzinoth Summon Channel
            { 46736, 46737 }  // Personalized Weather
        };

        uint32 const PeriodicRestorationAuras[] =
        {
            24379,
            23493
        };

        uint32 const ReagentConsumingPeriodicTriggerAuras[] =
        {
            24745,
            24747,
            24757,
            24759,
            24761,
            24762,
            24766,
            24769,
            24771,
            24773,
            24785,
            24787,
            24791,
            24792,
            24793
        };

        uint32 const FullHealthRemovePeriodicDamageAuras[] =
        {
            43093,
            31956,
            38801,
            35321,
            38363,
            39215
        };

        uint32 const UnboundPlaguePeriodicDamageAuras[] =
        {
            70911,
            72854,
            72855,
            72856
        };

        uint32 const UnsavableSpecialAuras[] =
        {
            44413,
            40075,
            55849
        };

        uint32 const UnsavableDruidEclipseAuras[] =
        {
            67483,
            67484,
            48517,
            48518
        };

        SpellIdPair const StonedAuraTriggeredSpells[] =
        {
            { 33572, 33652 },
            { 50836, 50812 }
        };

        SpellIdPair const ArcanePotencyTriggeredSpells[] =
        {
            { 31571, 57529 },
            { 31572, 57531 }
        };

        SpellIdPair const ImprovedFearTriggeredSpells[] =
        {
            { 53759, 60947 },
            { 53754, 60946 }
        };

        RaceGenderDisplayInfo const OrbOfDeceptionDisplays[] =
        {
            { RACE_BLOODELF_ID,       17829, 17830 },
            { RACE_ORC_ID,            10139, 10140 },
            { RACE_TROLL_ID,          10135, 10134 },
            { RACE_TAUREN_ID,         10136, 10147 },
            { RACE_UNDEAD_PLAYER_ID,  10146, 10145 },
            { RACE_DRAENEI_ID,        17827, 17828 },
            { RACE_DWARF_ID,          10141, 10142 },
            { RACE_GNOME_ID,          10148, 10149 },
            { RACE_HUMAN_ID,          10137, 10138 },
            { RACE_NIGHTELF_ID,       10143, 10144 }
        };

        RaceGenderDisplayInfo const CorsairDisplays[] =
        {
            { RACE_BLOODELF_ID,       25032, 25043 },
            { RACE_ORC_ID,            25039, 25050 },
            { RACE_TROLL_ID,          25041, 25052 },
            { RACE_TAUREN_ID,         25040, 25051 },
            { RACE_UNDEAD_PLAYER_ID,  25042, 25053 },
            { RACE_DRAENEI_ID,        25033, 25044 },
            { RACE_DWARF_ID,          25034, 25045 },
            { RACE_GNOME_ID,          25035, 25046 },
            { RACE_HUMAN_ID,          25037, 25048 },
            { RACE_NIGHTELF_ID,       25038, 25049 }
        };

        TransformDisplayInfo const TransformDisplays[] =
        {
            {  16739, true,  OrbOfDeceptionDisplays, 10,     0,     0 }, // Orb of Deception
            {  42365, false, NULL,                    0, 21723, 21723 }, // Murloc costume
            {  50517, true,  CorsairDisplays,        10,     0,     0 }, // Dread Corsair
            {  51926, true,  CorsairDisplays,        10,     0,     0 }, // Corsair Costume
            {  53806, false, NULL,                    0, 22512, 22512 }, // Pygmy Oil
            {  65386, false, NULL,                    0, 29203, 29204 }, // Honor the Dead
            {  65495, false, NULL,                    0, 29203, 29204 }, // Honor the Dead
            {  75532, false, NULL,                    0, 31737, 31738 }, // Darkspear Pride
            {  75531, false, NULL,                    0, 31654, 31655 }  // Gnomeregan Pride
        };

        TransformDisplayInfo const* GetTransformDisplayInfo(uint32 auraSpellId)
        {
            for (TransformDisplayInfo const& transformDisplay : TransformDisplays)
                if (transformDisplay.AuraSpellId == auraSpellId)
                    return &transformDisplay;

            return NULL;
        }

        bool MatchesAuraImmunityAmount(AuraImmunityAmountCondition amountCondition, int32 amount)
        {
            switch (amountCondition)
            {
                case AURA_IMMUNITY_AMOUNT_ANY:
                    return true;
                case AURA_IMMUNITY_AMOUNT_NONZERO:
                    return amount != 0;
                case AURA_IMMUNITY_AMOUNT_ZERO:
                    return amount == 0;
                case AURA_IMMUNITY_AMOUNT_ONE:
                    return amount == 1;
                default:
                    break;
            }

            return false;
        }

        AuraStateImmunityRuleInfo const AuraStateImmunityRules[] =
        {
            { {   96,     0, AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL, CrowdControlMechanics, CrowdControlMechanicCount, CrowdControlAuraTypes, CrowdControlAuraTypeCount, NULL, 0 }, AURA_IMMUNITY_AMOUNT_NONZERO },
            { { 1615,     0, AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL, CrowdControlMechanics, CrowdControlMechanicCount, CrowdControlAuraTypes, CrowdControlAuraTypeCount, NULL, 0 }, AURA_IMMUNITY_AMOUNT_NONZERO },
            { {  679, 57742, AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL, CrowdControlMechanics, CrowdControlMechanicCount, CrowdControlAuraTypes, CrowdControlAuraTypeCount, NULL, 0 }, AURA_IMMUNITY_AMOUNT_ANY },
            { { 1557, 64187, AURA_IMMUNITY_MECHANIC_MASK_STUN, StunMechanics, StunMechanicCount, StunAuraTypes, StunAuraTypeCount, NULL, 0 }, AURA_IMMUNITY_AMOUNT_ANY },
            { { 1557,     0, AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL, CrowdControlMechanics, CrowdControlMechanicCount, CrowdControlAuraTypes, CrowdControlAuraTypeCount, NULL, 0 }, AURA_IMMUNITY_AMOUNT_ANY },
            { { 1614,     0, AURA_IMMUNITY_MECHANIC_MASK_NONE, NULL, 0, TauntAuraTypes, TauntAuraTypeCount, AttackMeEffects, AttackMeEffectCount }, AURA_IMMUNITY_AMOUNT_ANY },
            { { 1694,     0, AURA_IMMUNITY_MECHANIC_MASK_NONE, NULL, 0, TauntAuraTypes, TauntAuraTypeCount, AttackMeEffects, AttackMeEffectCount }, AURA_IMMUNITY_AMOUNT_ANY },
            { { 1630,     0, AURA_IMMUNITY_MECHANIC_MASK_NONE, NULL, 0, TauntAuraTypes, TauntAuraTypeCount, AttackMeEffects, AttackMeEffectCount }, AURA_IMMUNITY_AMOUNT_ZERO },
            { { 1630,     0, AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL, CrowdControlMechanics, CrowdControlMechanicCount, CrowdControlAuraTypes, CrowdControlAuraTypeCount, NULL, 0 }, AURA_IMMUNITY_AMOUNT_NONZERO },
            { {  477,     0, AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL, CrowdControlMechanics, CrowdControlMechanicCount, CrowdControlAuraTypes, CrowdControlAuraTypeCount, KnockbackEffects, KnockbackEffectCount }, AURA_IMMUNITY_AMOUNT_ZERO },
            { { 1733,     0, AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL, CrowdControlMechanics, CrowdControlMechanicCount, CrowdControlAuraTypes, CrowdControlAuraTypeCount, KnockbackEffects, KnockbackEffectCount }, AURA_IMMUNITY_AMOUNT_ZERO },
            { {  878,     0, AURA_IMMUNITY_MECHANIC_MASK_LIGHT_CONTROL, LightControlMechanics, LightControlMechanicCount, LightControlAuraTypes, LightControlAuraTypeCount, NULL, 0 }, AURA_IMMUNITY_AMOUNT_ONE }
        };

        MechanicImmunitySpellRule const MechanicImmunitySpellRules[] =
        {
            { 34471, AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL, MovementLossControlAppliedMechanics, MovementLossControlAppliedMechanicCount },
            { 19574, AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL, MovementLossControlAppliedMechanics, MovementLossControlAppliedMechanicCount },
            { 42292, AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL, NULL, 0 },
            { 59752, AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL, NULL, 0 }
        };

        uint32 BuildMechanicMaskFromArray(uint32 const* mechanics, uint8 mechanicCount)
        {
            uint32 mechanicMask = 0;
            for (uint8 i = 0; i < mechanicCount; ++i)
                mechanicMask |= Skyfire::Spells::GetMechanicMask(mechanics[i]);

            return mechanicMask;
        }

        bool ContainsSpellId(uint32 const* spellIds, uint8 spellCount, uint32 spellId)
        {
            for (uint8 i = 0; i < spellCount; ++i)
                if (spellIds[i] == spellId)
                    return true;

            return false;
        }
    }

    uint32 GetQuestTameFinalSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& questTameSpell : QuestTameFinalSpells)
            if (questTameSpell.AuraSpellId == auraSpellId)
                return questTameSpell.TriggerSpellId;

        return 0;
    }

    ChampioningAuraInfo const* GetChampioningAuraInfo(uint32 auraSpellId)
    {
        for (ChampioningAuraInfo const& championingAura : ChampioningAuras)
            if (championingAura.AuraSpellId == auraSpellId)
                return &championingAura;

        return NULL;
    }

    uint8 GetShapeshiftBoostSpellIds(uint32 shapeshiftForm, uint32* spellIds, uint8 maxSpellIds)
    {
        if (!spellIds)
            return 0;

        for (ShapeshiftBoostSpellInfo const& shapeshiftBoost : ShapeshiftBoostSpells)
        {
            if (shapeshiftBoost.ShapeshiftForm != shapeshiftForm)
                continue;

            uint8 spellCount = shapeshiftBoost.SpellCount < maxSpellIds ? shapeshiftBoost.SpellCount : maxSpellIds;
            for (uint8 i = 0; i < spellCount; ++i)
                spellIds[i] = shapeshiftBoost.SpellIds[i];

            return spellCount;
        }

        return 0;
    }

    uint32 GetShadowFocusAuraSpellId()
    {
        return SPELL_SHADOW_FOCUS_DUMMY_TALENT;
    }

    uint32 GetShadowFocusTriggeredSpellId()
    {
        return SPELL_SHADOW_FOCUS_AURA;
    }

    uint32 GetNurturingInstinctTriggeredSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& nurturingInstinctSpell : NurturingInstinctTriggeredSpells)
            if (nurturingInstinctSpell.AuraSpellId == auraSpellId)
                return nurturingInstinctSpell.TriggerSpellId;

        return 0;
    }

    bool TransformDisplayRequiresPlayerRace(uint32 auraSpellId)
    {
        if (TransformDisplayInfo const* transformDisplay = GetTransformDisplayInfo(auraSpellId))
            return transformDisplay->RequiresPlayerRace;

        return false;
    }

    uint32 GetTransformDisplayId(uint32 auraSpellId, uint8 race, uint8 gender)
    {
        TransformDisplayInfo const* transformDisplay = GetTransformDisplayInfo(auraSpellId);
        if (!transformDisplay)
            return 0;

        if (!transformDisplay->RequiresPlayerRace)
            return gender == GENDER_MALE_ID ? transformDisplay->MaleDisplayId : transformDisplay->FemaleDisplayId;

        for (uint8 i = 0; i < transformDisplay->RaceDisplayCount; ++i)
        {
            RaceGenderDisplayInfo const& raceDisplay = transformDisplay->RaceDisplays[i];
            if (raceDisplay.Race == race)
                return gender == GENDER_MALE_ID ? raceDisplay.MaleDisplayId : raceDisplay.FemaleDisplayId;
        }

        return 0;
    }

    uint32 GetUnknownTransformFallbackDisplayId()
    {
        return DISPLAY_UNKNOWN_TRANSFORM_FALLBACK;
    }

    uint32 GetGlyphOfPenguinAuraSpellId()
    {
        return SPELL_GLYPH_OF_THE_PENGUIN;
    }

    uint32 GetGlyphOfPenguinDisplayId()
    {
        return DISPLAY_GLYPH_OF_THE_PENGUIN;
    }

    uint32 GetDragonmawIllusionAuraSpellId()
    {
        return SPELL_DRAGONMAW_ILLUSION;
    }

    uint32 GetDragonmawIllusionMountDisplayId()
    {
        return DISPLAY_DRAGONMAW_ILLUSION_MOUNT;
    }

    uint32 GetLeaderOfThePackSpellId()
    {
        return SPELL_LEADER_OF_THE_PACK;
    }

    uint32 GetLeaderOfThePackTriggeredSpellId()
    {
        return SPELL_LEADER_OF_THE_PACK_TRIGGERED;
    }

    bool IsNonAttackablePacifyAura(uint32 auraSpellId)
    {
        return auraSpellId == SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT;
    }

    uint32 GetRaidProcChargeTriggerSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& triggeredSpell : RaidProcChargeTriggeredSpells)
            if (triggeredSpell.AuraSpellId == auraSpellId)
                return triggeredSpell.TriggerSpellId;

        return 0;
    }

    bool ShouldBindStackingAuraToCaster(bool canStackWithDifferentCasters, bool casterIsCreature,
        bool casterIsSummon, bool ownerIsSummon)
    {
        if (!canStackWithDifferentCasters)
            return true;

        return !casterIsCreature || casterIsSummon || ownerIsSummon;
    }

    uint32 GetNetOMaticRootSelfSpellId()
    {
        return SPELL_NET_O_MATIC_SELF_ROOT;
    }

    uint32 GetBloodElfIllusionDisguiseSpellId(uint32 auraSpellId, uint8 gender)
    {
        for (GenderedAuraSpellInfo const& disguise : BloodElfIllusionDisguises)
        {
            if (disguise.AuraSpellId != auraSpellId)
                continue;

            if (gender == GENDER_MALE_ID)
                return disguise.MaleSpellId;
            if (gender == GENDER_FEMALE_ID)
                return disguise.FemaleSpellId;

            return 0;
        }

        return 0;
    }

    uint32 GetAuraDummyTriggeredSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& triggeredSpell : AuraDummyTriggeredSpells)
            if (triggeredSpell.AuraSpellId == auraSpellId)
                return triggeredSpell.TriggerSpellId;

        return 0;
    }

    uint32 GetAuraRemoveTriggeredSpellId(uint32 auraSpellId, bool heroic)
    {
        for (AuraRemoveTriggerSpellInfo const& triggeredSpell : AuraRemoveTriggeredSpells)
        {
            if (triggeredSpell.AuraSpellId != auraSpellId)
                continue;

            if (heroic && triggeredSpell.HeroicSpellId)
                return triggeredSpell.HeroicSpellId;

            return triggeredSpell.NormalSpellId;
        }

        return 0;
    }

    bool IsDespawnOnRemoveAura(uint32 auraSpellId)
    {
        return ContainsSpellId(DespawnOnRemoveAuras, sizeof(DespawnOnRemoveAuras) / sizeof(uint32), auraSpellId);
    }

    uint32 GetStackingAuraTriggerSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& triggeredSpell : StackingAuraTriggeredSpells)
            if (triggeredSpell.AuraSpellId == auraSpellId)
                return triggeredSpell.TriggerSpellId;

        return 0;
    }

    uint32 GetTaggedMurlocEntry(bool apply)
    {
        return apply ? NPC_TAGGED_BLACKSILT_SCOUT : NPC_UNTAGGED_BLACKSILT_SCOUT;
    }

    uint32 GetLichKingIntroApplySoundId(uint32 auraSpellId)
    {
        for (LichKingIntroVoiceoverInfo const& voiceover : LichKingIntroVoiceovers)
            if (voiceover.AuraSpellId == auraSpellId)
                return voiceover.ApplySoundId;

        return 0;
    }

    uint32 GetLichKingIntroRemoveSoundId(uint32 auraSpellId)
    {
        for (LichKingIntroVoiceoverInfo const& voiceover : LichKingIntroVoiceovers)
            if (voiceover.AuraSpellId == auraSpellId)
                return voiceover.RemoveSoundId;

        return 0;
    }

    uint32 GetLichKingIntroContinueSpellId(uint32 auraSpellId)
    {
        for (LichKingIntroVoiceoverInfo const& voiceover : LichKingIntroVoiceovers)
            if (voiceover.AuraSpellId == auraSpellId)
                return voiceover.ContinueSpellId;

        return 0;
    }

    uint32 GetFestiveHolidayMountCreatureEntry(bool flying)
    {
        return flying ? NPC_FESTIVE_HOLIDAY_MOUNT_FLYING : NPC_FESTIVE_HOLIDAY_MOUNT_GROUND;
    }

    PeriodicBulletControllerRule const* GetPeriodicBulletControllerRule(uint32 auraSpellId)
    {
        for (PeriodicBulletControllerRule const& rule : PeriodicBulletControllerRules)
            if (rule.AuraSpellId == auraSpellId)
                return &rule;

        return NULL;
    }

    uint32 GetPeriodicOverloadCircuitShutdownSpellId(uint32 auraSpellId)
    {
        return auraSpellId == 62399 ? 62475 : 0;
    }

    uint32 GetPeriodicFuseArmorTriggerSpellId(uint32 auraSpellId)
    {
        return auraSpellId == 64821 ? 64774 : 0;
    }

    uint32 GetKillingSpreePeriodicSpellId(uint8 index)
    {
        uint32 const spellIds[] = { 57840, 57841 };
        return index < 2 ? spellIds[index] : 0;
    }

    uint32 GetPeriodicDeathAndDecayTriggerSpellId()
    {
        return SPELL_PERIODIC_DEATH_AND_DECAY_TRIGGER;
    }

    uint32 GetPeriodicHysteriaAuraSpellId()
    {
        return SPELL_PERIODIC_HYSTERIA;
    }

    uint32 GetPeriodicTriggerOverrideSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& triggeredSpell : PeriodicTriggerOverrideSpells)
            if (triggeredSpell.AuraSpellId == auraSpellId)
                return triggeredSpell.TriggerSpellId;

        return 0;
    }

    bool IsPeriodicRestorationAura(uint32 auraSpellId)
    {
        return ContainsSpellId(PeriodicRestorationAuras, sizeof(PeriodicRestorationAuras) / sizeof(uint32), auraSpellId);
    }

    uint32 GetPeriodicRestorationEnergizeSpellId()
    {
        return SPELL_PERIODIC_RESTORATION_ENERGIZE;
    }

    uint32 GetPeriodicFrostBlastTriggerSpellId()
    {
        return SPELL_PERIODIC_FROST_BLAST_TRIGGER;
    }

    uint32 GetPeriodicDoomTriggerSpellId()
    {
        return SPELL_PERIODIC_DOOM_TRIGGER;
    }

    uint32 GetSpellclothSummonCreatureEntry()
    {
        return NPC_SPELLCLOTH_ELEMENTAL;
    }

    uint32 GetAbsorbEyeOfGrillokCasterSpellId()
    {
        return SPELL_EYE_OF_GRILLOK;
    }

    uint32 GetTotemicMasteryAuraSpellId()
    {
        return SPELL_TOTEMIC_MASTERY_AURA;
    }

    uint32 GetTotemicMasteryTriggerSpellId()
    {
        return SPELL_TOTEMIC_MASTERY_TRIGGER;
    }

    bool IsReagentConsumingPeriodicTriggerAura(uint32 auraSpellId)
    {
        return ContainsSpellId(ReagentConsumingPeriodicTriggerAuras,
            sizeof(ReagentConsumingPeriodicTriggerAuras) / sizeof(uint32), auraSpellId);
    }

    bool IsFullHealthRemovePeriodicDamageAura(uint32 auraSpellId)
    {
        return ContainsSpellId(FullHealthRemovePeriodicDamageAuras,
            sizeof(FullHealthRemovePeriodicDamageAuras) / sizeof(uint32), auraSpellId);
    }

    bool IsPctHealthRemovePeriodicDamageAura(uint32 auraSpellId)
    {
        return auraSpellId == 38772;
    }

    bool IsUnboundPlaguePeriodicDamageAura(uint32 auraSpellId)
    {
        return ContainsSpellId(UnboundPlaguePeriodicDamageAuras,
            sizeof(UnboundPlaguePeriodicDamageAuras) / sizeof(uint32), auraSpellId);
    }

    uint32 GetDrainSoulShardSpellId()
    {
        return SPELL_DRAIN_SOUL_SHARD;
    }

    uint32 GetTenacityAuraSpellId()
    {
        return SPELL_TENACITY;
    }

    uint32 GetWildGrowthTier10AuraSpellId()
    {
        return SPELL_WILD_GROWTH_T10_RESTORATION;
    }

    bool IsUnsavableSpecialAura(uint32 auraSpellId)
    {
        return ContainsSpellId(UnsavableSpecialAuras, sizeof(UnsavableSpecialAuras) / sizeof(uint32), auraSpellId);
    }

    bool IsUnsavableDruidEclipseAura(uint32 auraSpellId)
    {
        return ContainsSpellId(UnsavableDruidEclipseAuras, sizeof(UnsavableDruidEclipseAuras) / sizeof(uint32), auraSpellId);
    }

    uint32 GetStonedAuraTriggerSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& triggeredSpell : StonedAuraTriggeredSpells)
            if (triggeredSpell.AuraSpellId == auraSpellId)
                return triggeredSpell.TriggerSpellId;

        return 0;
    }

    uint32 GetDruidTier8RestorationAuraSpellId()
    {
        return SPELL_DRUID_T8_RESTORATION;
    }

    uint32 GetDruidCatFormSpellId()
    {
        return SPELL_DRUID_CAT_FORM;
    }

    uint32 GetDruidBearFormSpellId()
    {
        return SPELL_DRUID_BEAR_FORM;
    }

    uint32 GetPolymorphSheepSpellId()
    {
        return SPELL_POLYMORPH_SHEEP_SOUND;
    }

    uint32 GetPolymorphPenguinSpellId()
    {
        return SPELL_POLYMORPH_PENGUIN_SOUND;
    }

    uint32 GetArcanePotencyTriggerSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& triggeredSpell : ArcanePotencyTriggeredSpells)
            if (triggeredSpell.AuraSpellId == auraSpellId)
                return triggeredSpell.TriggerSpellId;

        return 0;
    }

    uint32 GetGlyphOfPowerWordShieldAuraSpellId()
    {
        return SPELL_GLYPH_OF_POWER_WORD_SHIELD;
    }

    uint32 GetGlyphOfPowerWordShieldHealSpellId()
    {
        return SPELL_GLYPH_OF_POWER_WORD_SHIELD_HEAL;
    }

    uint32 GetGlyphOfBlurredSpeedAuraSpellId()
    {
        return SPELL_GLYPH_OF_BLURRED_SPEED;
    }

    uint32 GetGlyphOfBlurredSpeedSprintSpellId()
    {
        return SPELL_GLYPH_OF_BLURRED_SPEED_SPRINT;
    }

    uint32 GetForbearanceAuraSpellId()
    {
        return SPELL_FORBEARANCE;
    }

    uint32 GetSharedSufferingTriggerSpellId()
    {
        return SPELL_SHARED_SUFFERING_TRIGGER;
    }

    uint32 GetInvisibilityFadeSpellId()
    {
        return SPELL_INVISIBILITY_FADE;
    }

    uint32 GetImprovedFearTriggerSpellId(uint32 auraSpellId)
    {
        for (SpellIdPair const& triggeredSpell : ImprovedFearTriggeredSpells)
            if (triggeredSpell.AuraSpellId == auraSpellId)
                return triggeredSpell.TriggerSpellId;

        return 0;
    }

    uint32 GetRaptureEnergizeSpellId()
    {
        return SPELL_RAPTURE_ENERGIZE;
    }

    uint32 GetGlyphOfFreezingTrapAuraSpellId()
    {
        return SPELL_GLYPH_OF_FREEZING_TRAP;
    }

    uint32 GetGlyphOfFreezingTrapTriggerSpellId()
    {
        return SPELL_GLYPH_OF_FREEZING_TRAP_TRIGGER;
    }

    uint32 GetProwlAuraSpellId()
    {
        return SPELL_PROWL;
    }

    uint32 GetStealthAuraSpellId()
    {
        return SPELL_STEALTH;
    }

    uint32 GetDruidFeralTier10BonusAuraSpellId()
    {
        return SPELL_DRUID_T10_FERAL;
    }

    uint32 GetDruidFeralTier10BonusTriggerSpellId()
    {
        return SPELL_DRUID_T10_FERAL_TRIGGER;
    }

    uint32 GetBestialWrathAuraSpellId()
    {
        return SPELL_BESTIAL_WRATH;
    }

    uint32 GetTheBeastWithinTalentSpellId()
    {
        return SPELL_THE_BEAST_WITHIN_TALENT;
    }

    uint32 GetTheBeastWithinTriggerSpellId()
    {
        return SPELL_THE_BEAST_WITHIN_TRIGGER;
    }

    uint32 GetDivineFavorAuraSpellId()
    {
        return SPELL_DIVINE_FAVOR;
    }

    uint32 GetPaladinTier10HolyBonusAuraSpellId()
    {
        return SPELL_PALADIN_T10_HOLY;
    }

    uint32 GetPaladinTier10HolyBonusTriggerSpellId()
    {
        return SPELL_PALADIN_T10_HOLY_TRIGGER;
    }

    AuraStateImmunityRule const* GetAuraStateImmunityRule(int32 miscValue, uint32 auraSpellId, int32 amount)
    {
        for (AuraStateImmunityRuleInfo const& ruleInfo : AuraStateImmunityRules)
        {
            AuraStateImmunityRule const& rule = ruleInfo.Rule;
            if (rule.MiscValue != miscValue)
                continue;

            if (rule.RequiredSpellId && rule.RequiredSpellId != auraSpellId)
                continue;

            if (!MatchesAuraImmunityAmount(ruleInfo.AmountCondition, amount))
                continue;

            return &rule;
        }

        return NULL;
    }

    uint8 GetDefaultAuraStateImmunityAuraTypes(int32 miscValue, uint32* auraTypes, uint8 maxAuraTypes)
    {
        if (!auraTypes)
            return 0;

        uint8 auraTypeCount = 0;

        if ((miscValue & (1 << 10)) && auraTypeCount < maxAuraTypes)
            auraTypes[auraTypeCount++] = SPELL_AURA_MOD_STUN;
        if ((miscValue & (1 << 1)) && auraTypeCount < maxAuraTypes)
            auraTypes[auraTypeCount++] = SPELL_AURA_TRANSFORM;
        if ((miscValue & (1 << 6)) && auraTypeCount < maxAuraTypes)
            auraTypes[auraTypeCount++] = SPELL_AURA_MOD_DECREASE_SPEED;
        if ((miscValue & (1 << 0)) && auraTypeCount < maxAuraTypes)
            auraTypes[auraTypeCount++] = SPELL_AURA_MOD_ROOT;
        if ((miscValue & (1 << 2)) && auraTypeCount < maxAuraTypes)
            auraTypes[auraTypeCount++] = SPELL_AURA_MOD_CONFUSE;
        if (miscValue & (1 << 9))
        {
            if (auraTypeCount < maxAuraTypes)
                auraTypes[auraTypeCount++] = SPELL_AURA_MOD_FEAR;
            if (auraTypeCount < maxAuraTypes)
                auraTypes[auraTypeCount++] = SPELL_AURA_MOD_FEAR_2;
        }
        if ((miscValue & (1 << 7)) && auraTypeCount < maxAuraTypes)
            auraTypes[auraTypeCount++] = SPELL_AURA_MOD_DISARM;

        return auraTypeCount;
    }

    MechanicImmunitySpellRule const* GetMechanicImmunitySpellRule(uint32 auraSpellId)
    {
        for (MechanicImmunitySpellRule const& rule : MechanicImmunitySpellRules)
            if (rule.AuraSpellId == auraSpellId)
                return &rule;

        return NULL;
    }

    uint32 GetAuraImmunityMechanicMask(AuraImmunityMechanicMask mechanicMask)
    {
        switch (mechanicMask)
        {
            case AURA_IMMUNITY_MECHANIC_MASK_CROWD_CONTROL:
                return BuildMechanicMaskFromArray(CrowdControlMechanics, CrowdControlMechanicCount);
            case AURA_IMMUNITY_MECHANIC_MASK_STUN:
                return BuildMechanicMaskFromArray(StunMechanics, StunMechanicCount);
            case AURA_IMMUNITY_MECHANIC_MASK_LIGHT_CONTROL:
                return BuildMechanicMaskFromArray(LightControlMechanics, LightControlMechanicCount);
            case AURA_IMMUNITY_MECHANIC_MASK_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL:
                return IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;
            case AURA_IMMUNITY_MECHANIC_MASK_NONE:
            default:
                break;
        }

        return 0;
    }
}
}
