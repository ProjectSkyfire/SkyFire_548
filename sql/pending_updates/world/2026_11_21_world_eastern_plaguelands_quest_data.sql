-- Eastern Plaguelands: spawns, drops and chests of the Cataclysm quests. Based on Legends of Azeroth 5.4.8, checked
-- against the spawn maps on Wowhead MoP Classic.
--
-- Gidwin Goldbraids, Tarenar Sunstrike and Vex'tul (the Northdale quest line: 27369, 27449, 27455, 27463, 27466,
-- 27481 to 27489, 27522 to 27526) and Lord Raymond George ("Argent Call: The Trial of the Crypt") had no spawn, nor had
-- the Mossflayer Rogues, the Scarlet commanders of 27613 to 27620 and the credit of "Into the Flames". Nine quest
-- items (Crypt Bile, Plaguehound Blood, the Death Cultist clothes, the Mossflayer Eye, the Body of Warlord Thresh'jin,
-- the Stinking Skull and the four Scarlet Salvage items) had no drop rows.

DELETE FROM `creature` WHERE `guid` BETWEEN 8316008 AND 8316022;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8316008, 45431, 0, 1, 0, 0, 0, 0, 2262.44, -5308.36, 81.9178, 0.2466, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8316009, 45431, 0, 1, 0, 0, 0, 1, 2736.42, -2454.52, 68.7811, 3.1765, 180, 0, 0, 0, 0, 0, 0, 0, 0),
(8316010, 45730, 0, 1, 0, 0, 0, 0, 2301.97, -5260.52, 82.2781, 0.733, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8316011, 45574, 0, 1, 0, 0, 0, 0, 2287.06, -4428.35, 111.781, 3.0976, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8316012, 45729, 0, 1, 0, 0, 0, 0, 2306.87, -5262.5, 82.5345, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8316013, 45579, 0, 1, 0, 0, 0, 0, 2316.31, -4416.9, 95.4475, 0.1789, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8316014, 45579, 0, 1, 0, 0, 0, 0, 2312.85, -4411.04, 95.7862, 5.6765, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8316015, 45579, 0, 1, 0, 0, 0, 0, 2315.12, -4423.61, 95.2216, 0.9641, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8316016, 45707, 0, 1, 0, 0, 0, 0, 2323.28, -5385.66, 76.9932, 4.6426, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8316017, 45738, 0, 1, 0, 0, 0, 0, 3462.09, -5004.08, 202.471, 5.5979, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(8316018, 46092, 0, 1, 0, 0, 0, 0, 1603.47, -5300.02, 91.1053, 5.7556, 300, 5, 0, 0, 0, 1, 0, 0, 0),
(8316019, 46093, 0, 1, 0, 0, 0, 0, 1597.88, -5308.08, 68.7318, 0.4496, 300, 5, 0, 0, 0, 1, 0, 0, 0),
(8316020, 46094, 0, 1, 0, 0, 0, 0, 1618.84, -5292.09, 99.9683, 5.8626, 300, 5, 0, 0, 0, 1, 0, 0, 0),
(8316021, 46095, 0, 1, 0, 0, 0, 0, 1752.99, -5425.66, 113.527, 1.4078, 300, 5, 0, 0, 0, 1, 0, 0, 0),
(8316022, 46096, 0, 1, 0, 0, 0, 0, 1570.07, -5606.6, 114.19, 1.157, 300, 0, 0, 0, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `faction_A`=14, `faction_H`=14 WHERE `entry` IN (46092, 46093, 46094, 46095, 46096);

DELETE FROM `creature_loot_template` WHERE (`item`=60983 AND `entry` IN (8555, 8556, 8557, 8558)) OR (`item`=60986 AND `entry` IN (8596, 8598))
 OR (`item` IN (61280, 61281) AND `entry` IN (8548, 8551)) OR (`item`=61313 AND `entry` IN (8560, 8561, 8562, 10822, 12261))
 OR (`item`=61315 AND `entry`=10822) OR (`item`=61372 AND `entry` IN (8523, 8524));
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(8555, 60983, -73.8597, 1, 0, 1, 1),
(8556, 60983, -74.4514, 1, 0, 1, 1),
(8557, 60983, -77.7324, 1, 0, 1, 1),
(8558, 60983, -76.569, 1, 0, 1, 1),
(8596, 60986, -58.0017, 1, 0, 1, 1),
(8598, 60986, -0.9448, 1, 0, 1, 1),
(8548, 61280, -28.1484, 1, 0, 1, 1),
(8551, 61280, -28.6079, 1, 0, 1, 1),
(8548, 61281, -28.6097, 1, 0, 1, 1),
(8551, 61281, -28.2831, 1, 0, 1, 1),
(8560, 61313, -70.7269, 1, 0, 1, 1),
(8561, 61313, -68.8504, 1, 0, 1, 1),
(8562, 61313, -70.9781, 1, 0, 1, 1),
(10822, 61313, -32.4795, 1, 0, 1, 1),
(12261, 61313, -36.3242, 1, 0, 1, 1),
(10822, 61315, -100, 1, 0, 1, 1),
(8523, 61372, -86.3126, 1, 0, 1, 1),
(8524, 61372, -86.1156, 1, 0, 1, 1);

-- "Scarlet Salvage": the four Battered Chests drop the Crimson Boar, Lihanna's Strand, Shroud of Uther and Gavinrad's
-- Sigil items.
UPDATE `gameobject_template` SET `data1`=`entry` WHERE `entry` IN (205878, 205879, 205880, 205881);
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (205878, 205879, 205880, 205881);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(205878, 61959, -100, 1, 0, 1, 1),
(205879, 61960, -100, 1, 0, 1, 1),
(205880, 61961, -100, 1, 0, 1, 1),
(205881, 61962, -100, 1, 0, 1, 1);
