-- Displaced Threshadon (41137), level 20: restore regular loot and skinning.
-- Use the existing level 19-20 Young Threshadon (1224) distribution as a
-- level-appropriate reconstruction; these are not verified retail drop rates.
-- References 24104, 24732 and 44009 resolve to level-appropriate world drops.
-- Preserve Threshadon Chunk (55232), including its existing -94 quest-only chance.
-- https://github.com/ProjectSkyfire/SkyFire_548/issues/1534
DELETE FROM `creature_loot_template`
 WHERE `entry`=41137 AND `item` IN (2608,5516,5523,6361,24104,24732,44009);
INSERT INTO `creature_loot_template`
 (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(41137,2608,50,1,2,1,1),
(41137,5516,13,1,1,1,1),
(41137,5523,50,1,2,1,1),
(41137,6361,15,1,0,1,2),
(41137,24104,5,1,1,-24104,1),
(41137,24732,1,1,1,-24732,1),
(41137,44009,0.5,1,1,-44009,1);

-- Keep this creature's skinning distribution independent of other templates.
DELETE FROM `skinning_loot_template`
 WHERE `entry`=41137 AND `item` IN (783,2318,2319,4232);
INSERT INTO `skinning_loot_template`
 (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(41137,783,2.1164,1,1,1,1),
(41137,2318,78.836,1,1,1,2),
(41137,2319,17.4603,1,1,1,1),
(41137,4232,1.5873,1,1,1,1);

UPDATE `creature_template` SET `lootid`=41137, `skinloot`=41137 WHERE `entry`=41137;
