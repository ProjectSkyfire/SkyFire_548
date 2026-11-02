-- Restore loot for the Cataclysm Dragonmaw Scout and Grunt templates.
-- Reuse the original creatures' existing loot tables instead of duplicating them.
-- https://github.com/ProjectSkyfire/SkyFire_548/issues/1516
UPDATE `creature_template` SET `lootid` = 2103 WHERE `entry` = 41080;
UPDATE `creature_template` SET `lootid` = 2102 WHERE `entry` IN (41072, 42107);
