-- Fix Timberling herbalism: herb cursor on living mobs and broken corpse loot/despawn.
-- Timberlings use CREATURE_TYPEFLAGS_HERBLOOT; SKINNABLE must be set only on death by the core.
-- Restore skinloot entries and skinning_loot_template rows for Teldrassil timberling variants.

UPDATE `creature_template` SET `unit_flags` = 0, `skinloot` = 2022 WHERE `entry` = 2022;
UPDATE `creature_template` SET `skinloot` = 2025 WHERE `entry` = 2025;
UPDATE `creature_template` SET `skinloot` = 2027 WHERE `entry` = 2027;
UPDATE `creature_template` SET `skinloot` = 2029 WHERE `entry` = 2029;
UPDATE `creature_template` SET `skinloot` = 2030 WHERE `entry` = 2030;

DELETE FROM `skinning_loot_template` WHERE `entry` IN (2022, 2025, 2027, 2029, 2030);
INSERT INTO `skinning_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(2022, 765, 2.1034, 1, 0, 1, 1),
(2022, 785, 25.8413, 1, 0, 1, 1),
(2022, 2447, 2.2636, 1, 0, 1, 1),
(2022, 2449, 2.0833, 1, 0, 1, 1),
(2022, 23987, 25.0601, 1, 0, 1, 1),
(2022, 67357, 64.1123, 1, 0, 1, 1),
(2025, 765, 3.6415, 1, 0, 1, 1),
(2025, 785, 25.2101, 1, 0, 1, 1),
(2025, 2447, 1.9608, 1, 0, 1, 1),
(2025, 2449, 3.3613, 1, 0, 1, 1),
(2025, 23987, 24.93, 1, 0, 1, 1),
(2025, 67357, 65.3185, 1, 0, 1, 1),
(2027, 765, 3.7262, 1, 0, 1, 1),
(2027, 785, 23.3969, 1, 0, 1, 1),
(2027, 2447, 2.9463, 1, 0, 1, 1),
(2027, 2449, 2.0797, 1, 0, 1, 1),
(2027, 23987, 27.383, 1, 0, 1, 1),
(2027, 67357, 60.0907, 1, 0, 1, 1),
(2029, 765, 2.1195, 1, 0, 1, 1),
(2029, 785, 32.7553, 1, 0, 1, 1),
(2029, 2447, 2.569, 1, 0, 1, 1),
(2029, 2449, 1.8626, 1, 0, 1, 1),
(2029, 23987, 24.6628, 1, 0, 1, 1),
(2029, 67357, 64.7761, 1, 0, 1, 1),
(2030, 765, 2.2876, 1, 0, 1, 1),
(2030, 785, 25.3268, 1, 0, 1, 1),
(2030, 2447, 1.5523, 1, 0, 1, 1),
(2030, 2449, 1.5523, 1, 0, 1, 1),
(2030, 2450, 12.2549, 1, 0, 1, 1),
(2030, 2452, 2.451, 1, 0, 1, 1),
(2030, 23331, 43.4641, 1, 0, 1, 1),
(2030, 23987, 13.8889, 1, 0, 1, 1),
(2030, 54630, 2.9423, 1, 0, 1, 1),
(2030, 67357, 31.1581, 1, 0, 1, 1);
