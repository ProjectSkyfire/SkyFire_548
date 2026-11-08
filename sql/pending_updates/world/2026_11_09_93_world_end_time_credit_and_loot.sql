-- End Time: Murozond's encounter credit (he fades instead of dying), the Dungeon Finder entry and loot.
-- Idempotent: every INSERT is preceded by a DELETE of its keys.

DELETE FROM `spell_dbc` WHERE `Id`=110158;
DELETE FROM `spellmisc_dbc` WHERE `Id`=310158;
INSERT INTO `spell_dbc` (`Id`, `SpellAuraOptionsId`, `SpellCastingRequirementsId`, `SpellCategoriesId`, `SpellClassOptionsId`, `SpellEquippedItemsId`, `SpellInterruptsId`, `SpellLevelsId`, `SpellTargetRestrictionsId`, `SpellMiscId`, `Comment`) VALUES
(110158, 0, 0, 0, 0, 0, 0, 0, 0, 310158, 'Murozond credit marker');
INSERT INTO `spellmisc_dbc` (`Id`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`, `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `AttributesEx8`, `AttributesEx9`, `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `SchoolMask`, `Comment`) VALUES
(310158, 8388992, 0, 5, 269484288, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 'Murozond credit marker');

DELETE FROM `instance_encounters` WHERE `entry`=1271;
INSERT INTO `instance_encounters` (`entry`, `creditType`, `creditEntry`, `lastEncounterDungeon`, `comment`) VALUES
(1271, 1, 110158, 435, 'Murozond');

DELETE FROM `lfg_dungeon_template` WHERE `dungeonId`=435;
INSERT INTO `lfg_dungeon_template` (`dungeonId`, `position_x`, `position_y`, `position_z`, `orientation`, `requiredItemLevel`) VALUES
(435, 3726.54, -400.181, 110.834, 1.85005, 353);

DELETE FROM `creature_loot_template` WHERE `entry`=54123;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(54123, 44731, 0, 1, 1, 1, 1),
(54123, 72798, 0, 1, 1, 1, 1),
(54123, 72799, 0, 1, 1, 1, 1),
(54123, 72800, 0, 1, 1, 1, 1),
(54123, 72801, 0, 1, 1, 1, 1),
(54123, 72802, 0, 1, 1, 1, 1),
(54123, 72803, 0, 1, 1, 1, 1),
(54123, 72804, 0, 1, 1, 1, 1),
(54123, 72805, 0, 1, 1, 1, 1),
(54123, 72806, 0, 1, 1, 1, 1),
(54123, 72807, 0, 1, 1, 1, 1),
(54123, 72810, 0, 1, 1, 1, 1),
(54123, 72811, 0, 1, 1, 1, 1);
UPDATE `creature_template` SET `lootid`=54123 WHERE `entry`=54123;

DELETE FROM `gameobject_loot_template` WHERE `entry`=209547;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(209547, 52078, 100, 1, 0, 1, 1),
(209547, 71636, 30, 1, 0, 1, 1),
(209547, 71715, 10, 1, 0, 1, 1),
(209547, 72816, 0, 1, 1, 1, 1),
(209547, 72817, 0, 1, 2, 1, 1),
(209547, 72818, 0, 1, 1, 1, 1),
(209547, 72819, 0, 1, 2, 1, 1),
(209547, 72820, 0, 1, 1, 1, 1),
(209547, 72821, 0, 1, 2, 1, 1),
(209547, 72822, 0, 1, 1, 1, 1),
(209547, 72823, 0, 1, 2, 1, 1),
(209547, 72824, 0, 1, 1, 1, 1),
(209547, 72825, 0, 1, 2, 1, 1),
(209547, 72826, 0, 1, 1, 1, 1),
(209547, 72897, 0, 1, 2, 1, 1);
