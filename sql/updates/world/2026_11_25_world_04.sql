-- Stormwind Guard (1423): restore the Elwynn guards' missing pickpocket loot.
-- Wowhead MoP Classic, NPC 1423: 9 recorded pickpockets (2026-11-25).
-- Small Pocket Watch 6/9, Battered Junkbox 2/9, Moist Cornbread 1/9.
-- These are observed rates from a small sample, not documented server probabilities.
DELETE FROM `pickpocketing_loot_template` WHERE `entry` = 1423;
INSERT INTO `pickpocketing_loot_template`
(`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(1423, 5374, 66.6667, 1, 0, 1, 1),
(1423, 16882, 22.2222, 1, 0, 1, 1),
(1423, 4542, 11.1111, 1, 0, 1, 1);
UPDATE `creature_template` SET `pickpocketloot` = 1423 WHERE `entry` = 1423;
