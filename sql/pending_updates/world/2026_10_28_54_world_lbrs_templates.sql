-- Lower Blackrock Spire: creature template levels set to the 4.3.4 values (Cataclysm re-levelled the dungeon; these entries were still at their 3.3.5 levels).
-- Each statement is guarded by the current levels, so the file is safe to run twice.

UPDATE `creature_template` SET `minlevel` = 58, `maxlevel` = 58 WHERE `entry` = 9219 AND `minlevel` = 57 AND `maxlevel` = 57; -- Spirestone Butcher
UPDATE `creature_template` SET `minlevel` = 56, `maxlevel` = 56 WHERE `entry` = 9708 AND `minlevel` = 54 AND `maxlevel` = 55; -- Burning Imp
UPDATE `creature_template` SET `minlevel` = 57, `maxlevel` = 57 WHERE `entry` = 10261 AND `minlevel` = 54 AND `maxlevel` = 55; -- Burning Felhound
UPDATE `creature_template` SET `minlevel` = 58, `maxlevel` = 58 WHERE `entry` = 10263 AND `minlevel` = 56 AND `maxlevel` = 57; -- Burning Felguard
UPDATE `creature_template` SET `minlevel` = 62, `maxlevel` = 62 WHERE `entry` = 10296 AND `minlevel` = 54 AND `maxlevel` = 55; -- Acride
