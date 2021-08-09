/*
 * Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2021 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2021 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

#ifndef SKYFIRE_DBCSFRM_H
#define SKYFIRE_DBCSFRM_H

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - bool, p - field present in sql dbc, a - field absent in sql dbc

char const Achievementfmt[] = "niixsxxixixxiii";
//const std::string CustomAchievementfmt = "pppaaaapapaappa";
//const std::string CustomAchievementIndex = "ID";
//char const AchievementCriteriafmt[] = "niiiixiiiisiiiiixxiiiii";
char const AreaTableEntryfmt[] = "iiinixxxxxxxisiiiiifxxxxxxxxxx";
char const AreaGroupEntryfmt[] = "niiiiiii";
//char const AreaPOIEntryfmt[] = "niiiiiiiiiiiffixixxixxx";
char const AreaTriggerEntryfmt[] = "nifffxxxfffffxxx";
char const ArmorLocationfmt[] = "dfffff";
char const AuctionHouseEntryfmt[] = "niiix";
char const BankBagSlotPricesEntryfmt[] = "di";
char const BannedAddOnsfmt[] = "nxxxxxxxxxx";
char const BarberShopStyleEntryfmt[] = "dixxxiii";
char const BattlemasterListEntryfmt[] = "niiiiiiiiiiiiiiiiixsiixxxxxxxx";
char const CharStartOutfitEntryfmt[] = "dbbbXiiiiiiiiiiiiiiiiiiiiiiiixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const CharTitlesEntryfmt[] = "dxsxix";
char const ChatChannelsEntryfmt[] = "nixsx";
char const ChrClassesEntryfmt[] = "dixsxxxixiiiixxxxx";
char const ChrRacesEntryfmt[] = "dxixiixxxxxxiisxxxxxxxxxxxxxxxxxxxxx";
char const ChrClassesXPowerTypesfmt[] = "dii";
char const ChrSpecializationfmt[] = "nxiixiixxxxxxx";
char const CinematicSequencesEntryfmt[] = "nxxxxxxxxx";
char const CreatureDisplayInfofmt[] = "dixxfxxxxxxxxxxxxxxx";
char const CreatureModelDatafmt[] = "dxxxxxxxxxxxxxxffxxxxxxxxxxxxxxxxx";
char const CreatureFamilyfmt[] = "nfifiiiiixsx";
char const CreatureSpellDatafmt[] = "diiiixxxx";
char const CreatureTypefmt[] = "nxx";
char const Criteriafmt[] = "niiiiixxixxx";
char const CriteriaTreefmt[] = "niixxixx";
char const CurrencyTypesfmt[] = "nixxxxxiiixx";
char const DestructibleModelDatafmt[] = "dixxxixxxxixxxxxxxxxxxXX";
char const Difficultyfmt[] = "niixxxixxxxx";
char const DungeonEncounterfmt[] = "diixisxxx";
char const DurabilityCostsfmt[] = "diiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const DurabilityQualityfmt[] = "df";
char const EmotesEntryfmt[] = "nxxxxxxx";
char const EmotesTextEntryfmt[] = "dxixxxxxxxxxxxxxxxx";
char const FactionEntryfmt[] = "niiiiiiiiiiiiiiiiiiffixsxxxx";
char const FactionTemplateEntryfmt[] = "niiiiiiiiiiiii";
char const GameObjectDisplayInfofmt[] = "dxxxxxxxxxxxffffffxxx";
char const GemPropertiesEntryfmt[] = "dixxix";
char const GlyphPropertiesfmt[] = "diix";
char const GlyphSlotfmt[] = "nix";
char const GtBarberShopCostBasefmt[] = "xf";
char const GtCombatRatingsfmt[] = "xf";
char const GtOCTHpPerStaminafmt[] = "df";
char const GtChanceToMeleeCritBasefmt[] = "xf";
char const GtChanceToMeleeCritfmt[] = "xf";
char const GtChanceToSpellCritBasefmt[] = "xf";
char const GtChanceToSpellCritfmt[] = "xf";
char const GtNPCManaCostScalerfmt[] = "xf";
char const GtOCTClassCombatRatingScalarfmt[] = "df";
char const GtRegenMPPerSptfmt[] = "xf";
char const GtSpellScalingfmt[] = "df";
char const GtOCTBaseHPByClassfmt[] = "df";
char const GtOCTBaseMPByClassfmt[] = "df";
char const GuildPerkSpellsfmt[] = "dii";
char const Holidaysfmt[] = "nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const ImportPriceArmorfmt[] = "dffff";
char const ImportPriceQualityfmt[] = "df";
char const ImportPriceShieldfmt[] = "df";
char const ImportPriceWeaponfmt[] = "df";
char const ItemPriceBasefmt[] = "dxff";
char const ItemReforgefmt[] = "difif";
char const ItemArmorQualityfmt[] = "dfffffffx";
char const ItemArmorShieldfmt[] = "dxfffffff";
char const ItemArmorTotalfmt[] = "dxffff";
char const ItemClassfmt[] = "dxfx";
char const ItemDamagefmt[] = "dfffffffx";
//char const ItemBagFamilyfmt[] = "nx";
//char const ItemDisplayTemplateEntryfmt[] = "nxxxxxxxxxxixxxxxxxxxxx";
char const ItemDisenchantLootfmt[] = "nixiiii";
char const ItemLimitCategoryEntryfmt[] = "dxii";
char const ItemRandomPropertiesfmt[] = "nxiiixxs";
char const ItemRandomSuffixfmt[] = "nsxiiiiiiiiii";
char const ItemSetEntryfmt[] = "dsxxxxxxxxxxxxxxxxxiiiiiiiiiiiiiiiiii";
char const LFGDungeonEntryfmt[] = "nsiiixxiiiixxixixxxxxxxxxxxxx";
char const LiquidTypefmt[] = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const LockEntryfmt[] = "diiiiiiiiiiiiiiiiiiiiiiiixxxxxxxx";
char const MailTemplateEntryfmt[] = "dxs";
char const MapEntryfmt[] = "nxixxsixxxxiffxixxi";
char const MapDifficultyEntryfmt[] = "diisiix";
char const ModifierTreefmt[] = "niiixxi";
char const MountCapabilityfmt[] = "niiiiiii";
char const MountTypefmt[] = "diiiiiiiiiiiiiiiiiiiiiiii";
char const MovieEntryfmt[] = "nxxxx";
char const NameGenfmt[] = "dsii";
char const OverrideSpellDatafmt[] = "diiiiiiiiiixx";
char const PhaseEntryfmt[] = "nxx";
char const PhaseGroupfmt[] = "dii";
char const PvPDifficultyfmt[] = "diiiii";
char const QuestFactionRewardfmt[] = "diiiiiiiiii";
char const QuestPOIPointfmt[] = "nxxx";
char const QuestSortEntryfmt[] = "nx";
char const QuestXPfmt[] = "diiiiiiiiii";
char const RandomPropertiesPointsfmt[] = "diiiiiiiiiiiiiii";
//char const ResearchBranchfmt [] = "nxxxxx";
//char const ResearchProjectfmt [] = "nxxiixxxi";
//char const ResearchSitefmt [] = "niixx";
char const ScalingStatDistributionfmt[] = "diiiiiiiiiiiiiiiiiiiixi";
char const ScalingStatValuesfmt[] = "xdxiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const SkillLinefmt[] = "nisxxxixx";
char const SkillLineAbilityfmt[] = "diiiiiiiiixxx";
char const SoundEntriesfmt[] = "nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const SpecializationSpellsfmt[] = "diiix";

//
char const SpellCastTimefmt[] = "nixx";
char const SpellCategoriesEntryfmt[] = "dxxiiiiiix";
char const SpellCategoryfmt[] = "nixxxx";
char const SpellDifficultyfmt[] = "niiii";
const std::string CustomSpellDifficultyfmt = "ppppp";
const std::string CustomSpellDifficultyIndex = "id";
char const SpellDurationfmt[] = "niii";
//                                             0         10        20    26
char const SpellEffectEntryfmt[] =            "nxifiiiffiiiiiifiifiiiiixiiiix";
const std::string CustomSpellEffectEntryfmt = "ppppppppppapppappppppppppp";
const std::string CustomSpellEffectEntryIndex = "Id";
char const SpellEntryfmt[] =            "nssxxixxfiiiiiiiiiiiiiiii";
//const std::string CustomSpellEntryfmt = "ppppppppppppppapaaaaaaaaapaaaaaapapppaapppaaapa";
//const std::string CustomSpellEntryIndex = "Id";
char const SpellMiscfmt[] = "nxiiiiiiiiiiiiiiiiiifiiiii";
char const SpellEffectScalingfmt[] = "nfffxi";
char const SpellFocusObjectfmt[] = "nx";
char const SpellItemEnchantmentfmt[] = "nxiiiiiiiiisiiiiiiiixxxxxx";
char const SpellItemEnchantmentConditionfmt[] = "nbbbbbxxxxxbbbbbbbbbbiiiiiXXXXX";
char const SpellRadiusfmt[] = "nffxf";
char const SpellRangefmt[] = "nffffixx";
char const SpellScalingEntryfmt[] = "diiiifixx";
char const SpellTotemsEntryfmt[] = "niiii";
char const SpellTargetRestrictionsEntryfmt[] = "nxxxxiiii";
char const SpellPowerEntryfmt[] = "dixiiiiixffix";
char const SpellInterruptsEntryfmt[] = "dxxixixi";
char const SpellEquippedItemsEntryfmt[] = "dxxiii";
char const SpellAuraOptionsEntryfmt[] = "nxxiiiixx";
char const SpellAuraRestrictionsEntryfmt[] = "dxxiiiiiiii";
char const SpellCastingRequirementsEntryfmt[] = "dixxixi";
char const SpellClassOptionsEntryfmt[] = "dxiiiii";
char const SpellCooldownsEntryfmt[] = "dxxiii";
char const SpellLevelsEntryfmt[] = "dxxiii";
char const SpellRuneCostfmt[] = "niiiii";
char const SpellShapeshiftEntryfmt[] = "nixixx";
char const SpellShapeshiftFormfmt[] = "nxxiixiiixxiiiiiiiiix";

char const SummonPropertiesfmt[] = "niiiii";
char const TalentEntryfmt[] = "nxixixxxiix";
char const TaxiNodesEntryfmt[] = "difffsiixxxx";
char const TaxiPathEntryfmt[] = "niii";
char const TaxiPathNodeEntryfmt[] = "diiifffiiii";
char const TotemCategoryEntryfmt[] = "dxii";
char const UnitPowerBarfmt[] = "dxixxxxxxxxxxxxxxxxxxxxxxxx";
char const TransportAnimationfmt[] = "diixxxx";
char const TransportRotationfmt[] = "diiffff";
char const VehicleEntryfmt[] = "nixxxxxiiiiiiiixxxxxxxxxxxxxxxxxxxxxxxixx";
char const VehicleSeatEntryfmt[] = "nixfffxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxixxxxxxxxxxxxxxxxxxxxx";
char const WMOAreaTableEntryfmt[] = "diiixxxxxiixxxx";
char const WorldMapAreaEntryfmt[] = "xidxffffixxxxx";
char const WorldMapOverlayEntryfmt[] = "dxiiiixxxxxxxxxx";
char const WorldSafeLocsEntryfmt[] = "nifffxx";

#endif
