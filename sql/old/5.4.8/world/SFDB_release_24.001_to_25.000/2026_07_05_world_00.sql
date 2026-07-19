-- Fix 1: Add loot for Jitter's Bugs quest (NPC 45582, Item 60207)
UPDATE `creature_template` SET `lootid` = 45582 WHERE `entry` = 45582;
DELETE FROM `creature_loot_template` WHERE `entry` = 45582 AND `item` = 60207;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45582, 60207, -100, 1, 0, 1, 1);

-- Fix 2: Prevent Jitters and Oliver Harris from disappearing after turning in quest 26721
-- By removing the quest_end condition, the phase aura remains permanently after completing quest 26717
UPDATE `spell_area` SET `quest_end` = 0, `quest_end_status` = 0 WHERE `spell` = 49416 AND `quest_start` = 26717;
