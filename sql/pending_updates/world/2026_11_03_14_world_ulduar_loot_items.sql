-- Ulduar loot: wrong item ids and missing rows, as in TrinityCore 3.3.5.

-- Steelbreaker: Belt of the Crystal Tree (45455) was entered as Phosphorescent Globe (25455).
UPDATE `creature_loot_template` SET `item` = 45455 WHERE `entry` = 32867 AND `item` = 25455;

-- Archivum Data Disc: Runemaster Molgeim dropped it only in hard mode, and the 25-player Assembly dropped the
-- 10-player disc (45506, quest 13604) instead of the 25-player one (45857, quest 13817).
UPDATE `creature_loot_template` SET `lootmode` = 1 WHERE `entry` = 32927 AND `item` = 45506;
UPDATE `creature_loot_template` SET `item` = 45857 WHERE `entry` IN (33692, 33693) AND `item` = 45506;

-- Fragment of Val'anyr: the Cache of Winter, the Rare Cache of Winter and the Freya's Gift chests had Enchanted
-- Bridle (45083) in its place; General Vezax and Yogg-Saron in the 25-player raid had no row for it.
UPDATE `gameobject_loot_template` SET `item` = 45038 WHERE `entry` IN (26946, 26950, 26960, 26962, 27079, 27081) AND `item` = 45083;
DELETE FROM `creature_loot_template` WHERE `entry` IN (33449, 33955) AND `item` = 45038;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(33449, 45038, 9, 1, 0, 1, 1),
(33955, 45038, 100, 1, 0, 1, 1);

-- Gift of the Observer in the 25-player raid gives the 25-player Reply-Code Alpha (46053).
UPDATE `gameobject_loot_template` SET `item` = 46053 WHERE `entry` = 26974 AND `item` = 46052;
