-- Twilight Highlands: the spell bars of the quest vehicles. The world has empty spell columns for 10 vehicles that the 5.4.8 data gives
-- spells (the Obsidian Pyrewing, Darunga, Jora's Wildhammer mount, the Stagecoach, the Vermillion Skyscreamer, the four Tentacles of
-- Iso'rath and the Wildhammer Stormtalon) and three of the four abilities of the Krazz Cannon: the player rides them in the quests
-- and gets their abilities on the vehicle bar. Based on Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `spell1` = 88986, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 46141; -- Obsidian Pyrewing
UPDATE `creature_template` SET `spell1` = 93738, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 46624; -- Darunga
UPDATE `creature_template` SET `spell1` = 88530, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 47482; -- Jora "Nobbly" Wildwing
UPDATE `creature_template` SET `spell1` = 88622, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 47523; -- Stagecoach Carriage
UPDATE `creature_template` SET `spell1` = 85401, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 47780; -- Vermillion Skyscreamer
UPDATE `creature_template` SET `spell1` = 89918, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 48739; -- Tentacle of Iso'rath
UPDATE `creature_template` SET `spell1` = 89918, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 48790; -- Tentacle of Iso'rath
UPDATE `creature_template` SET `spell1` = 89918, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 48794; -- Tentacle of Iso'rath
UPDATE `creature_template` SET `spell1` = 89918, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 48796; -- Tentacle of Iso'rath
UPDATE `creature_template` SET `spell1` = 92733, `spell2` = 92743, `spell3` = 92745, `spell4` = 92742, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 49680; -- Krazz Cannon
UPDATE `creature_template` SET `spell1` = 16576, `spell2` = 3147, `spell3` = 83005, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0 WHERE `entry` = 49683; -- Wildhammer Stormtalon
