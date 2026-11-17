-- Vashj'ir: quest objects that give nothing when used. Eight gameobject templates have no loot id, so the quest items of
-- Fibrous Vine, Captain Taylor's and Nazgrim's flare guns, Rusty Harpoon, Coil of Kvaldir Rope, Deepfin Plunder, Horde
-- Survival Kit and Scalding Shroom (quests 25970, 26096 among others) could never be looted.
UPDATE `gameobject_template` SET `data1` = `entry` WHERE `entry` IN (202876, 203017, 203172, 203312, 203387, 203396, 203409, 203453) AND `data1` = 0;
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (202876, 203017, 203172, 203312, 203387, 203396, 203409, 203453);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(202876, 54459, 100, 1, 0, 1, 1),
(203017, 54846, 100, 1, 0, 1, 1),
(203172, 55214, -100, 1, 0, 1, 1),
(203312, 56183, -100, 1, 0, 1, 1),
(203387, 56235, -100, 1, 0, 1, 1),
(203396, 56249, 100, 1, 0, 1, 1),
(203409, 56251, -100, 1, 0, 1, 1),
(203453, 56810, -100, 1, 0, 1, 1);
