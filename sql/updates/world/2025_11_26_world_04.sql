-- Fix "Pie for Billy" quest - Fix Tender Boar Meat (item 769) drop from Stonetusk Boar (creature 113)
-- Quest items require negative ChanceOrQuestChance value in creature_loot_template
-- Negative values mean the item only drops when player has the quest active

-- Update existing entry to be a quest item (negative value = quest item)
-- Change from regular drop (75%) to quest item (-100 means guaranteed drop when quest is active)
UPDATE `creature_loot_template` 
SET `ChanceOrQuestChance` = -100 
WHERE `entry` = 113 AND `item` = 769;

-- If the entry doesn't exist, insert it
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
SELECT 113, 769, -100, 1, 0, 1, 1
WHERE NOT EXISTS (
    SELECT 1 FROM `creature_loot_template` WHERE `entry` = 113 AND `item` = 769
);

