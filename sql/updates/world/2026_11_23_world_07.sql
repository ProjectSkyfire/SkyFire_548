-- Flood Elemental (41132), level 20: restore its missing normal loot table.
-- The 74/26 vendor-item split is reconstructed from the Cataclysm references
-- reported in issue #1533, not independently verified retail drop rates.
-- Use the existing level 19-20 world-drop references and weights from 1224.
-- Their nested references resolve to items requiring level 20 or below.
-- https://github.com/ProjectSkyfire/SkyFire_548/issues/1533
DELETE FROM `creature_loot_template`
 WHERE `entry`=41132 AND `item` IN (55974,55982,24104,24732,44009);
INSERT INTO `creature_loot_template`
 (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(41132,55974,74,1,2,1,1),
(41132,55982,26,1,2,1,1),
(41132,24104,5,1,1,-24104,1),
(41132,24732,1,1,1,-24732,1),
(41132,44009,0.5,1,1,-44009,1);

UPDATE `creature_template` SET `lootid`=41132 WHERE `entry`=41132;
