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

char const GlyphPropertiesfmt[] = "niii";
char const GlyphSlotfmt[] = "nii";
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
char const Holidaysfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiixxsiix";
char const ImportPriceArmorfmt[] = "nffff";
char const ImportPriceQualityfmt[] = "nf";
char const ImportPriceShieldfmt[] = "nf";
char const ImportPriceWeaponfmt[] = "nf";
char const ItemPriceBasefmt[] = "diff";
char const ItemReforgefmt[] = "nifif";
//char const ItemBagFamilyfmt[] = "nx";
char const ItemArmorQualityfmt[] = "nfffffffi";
char const ItemArmorShieldfmt[] = "nifffffff";
char const ItemArmorTotalfmt[] = "niffff";
char const ItemClassfmt[] = "difx";
char const ItemDamagefmt[] = "nfffffffi";
char const ItemDisenchantLootfmt[] = "niiiiii";
//char const ItemDisplayTemplateEntryfmt[] = "nxxxxxxxxxxixxxxxxxxxxx";
char const ItemLimitCategoryEntryfmt[] = "nxii";
char const ItemRandomPropertiesfmt[] = "nxiiixxs";
char const ItemRandomSuffixfmt[] = "nsxiiiiiiiiii";
char const ItemSetEntryfmt[] = "dsiiiiiiiiiixxxxxxxiiiiiiiiiiiiiiiiii";
char const LFGDungeonEntryfmt[] = "nsiiiiiiiiixxixixixxxxxxxxxxx";
char const LiquidTypefmt[] = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const LockEntryfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiixxxxxxxx";
char const PhaseEntryfmt[] = "nsi";
char const PhaseGroupfmt[] = "nii";
char const MailTemplateEntryfmt[] = "nxs";
char const MapEntryfmt[] = "nxixxsixxixiffxiixi";
char const MapDifficultyEntryfmt[] = "diisiix";
char const ModifierTreefmt[] = "niiiixi";
char const MovieEntryfmt[] = "nxxxx";
char const MountCapabilityfmt[] = "niiiiiii";
char const MountTypefmt[] = "niiiiiiiiiiiiiiiiiiiiiiii";
char const NameGenfmt[] = "dsii";
char const NumTalentsAtLevelfmt[] = "df";
char const OverrideSpellDatafmt[] = "niiiiiiiiiixx";
char const QuestFactionRewardfmt[] = "niiiiiiiiii";
char const QuestSortEntryfmt[] = "nx";
char const QuestXPfmt[] = "niiiiiiiiii";
char const QuestPOIPointfmt [] = "diii";
char const PvPDifficultyfmt[] = "diiiii";
char const RandomPropertiesPointsfmt[] = "niiiiiiiiiiiiiii";
//char const ResearchBranchfmt [] = "nxxxxx";
//char const ResearchProjectfmt [] = "nxxiixxxi";
//char const ResearchSitefmt [] = "niixx";
char const ScalingStatDistributionfmt[] = "niiiiiiiiiiiiiiiiiiiixi";
char const ScalingStatValuesfmt[] = "iniiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const SkillLinefmt[] = "nisxixixx";
char const SkillLineAbilityfmt[] = "niiiiiiiiiixx";
char const SoundEntriesfmt[] = "nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const SpecializationSpellsfmt[] = "niiix";
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
char const TalentEntryfmt[] = "nxiiixxxiix";
char const TaxiNodesEntryfmt[] = "nifffsiixixx";
char const TaxiPathEntryfmt[] = "niii";
char const TaxiPathNodeEntryfmt[] = "diiifffiiii";
char const TotemCategoryEntryfmt[] = "nxii";
char const UnitPowerBarfmt[] = "niixxxxxxxxxxxxxxxxxxxxxxxx";
char const TransportAnimationfmt[] = "diifffx";
char const TransportRotationfmt[] = "diiffff";
char const VehicleEntryfmt[] = "nixffffiiiiiiiifffffffffffffffssssfifiixx";
char const VehicleSeatEntryfmt[] = "niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiixxxxxxxxxxxxxxxxxxxxx";
char const WMOAreaTableEntryfmt[] = "niiixxxxxiixxxx";
char const WorldMapAreaEntryfmt[] = "xinxffffixxxxx";
char const WorldMapOverlayEntryfmt[] = "nxiiiixxxxxxxxxx";
char const WorldSafeLocsEntryfmt[] = "nifffxx";

#endif
