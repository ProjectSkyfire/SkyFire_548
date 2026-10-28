-- Maraudon: creature template levels set to the 4.3.4 values (Cataclysm re-levelled the dungeon; these entries were still at their 3.3.5 levels).
-- Each statement is guarded by the current levels, so the file is safe to run twice.

UPDATE `creature_template` SET `minlevel` = 35, `maxlevel` = 36 WHERE `entry` = 12223 AND `minlevel` = 36 AND `maxlevel` = 36; -- Cavern Lurker
UPDATE `creature_template` SET `minlevel` = 36, `maxlevel` = 36 WHERE `entry` = 12224 AND `minlevel` = 45 AND `maxlevel` = 45; -- Cavern Shambler
UPDATE `creature_template` SET `minlevel` = 37, `maxlevel` = 37 WHERE `entry` = 12225 AND `minlevel` = 46 AND `maxlevel` = 46; -- Celebras the Cursed
UPDATE `creature_template` SET `minlevel` = 37, `maxlevel` = 37 WHERE `entry` = 12237 AND `minlevel` = 46 AND `maxlevel` = 46; -- Meshlok the Harvester
UPDATE `creature_template` SET `minlevel` = 33, `maxlevel` = 33 WHERE `entry` = 13743 AND `minlevel` = 42 AND `maxlevel` = 46; -- Corrupt Force of Nature
