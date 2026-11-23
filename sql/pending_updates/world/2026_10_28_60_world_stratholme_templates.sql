-- Stratholme: creature template levels set to the 4.3.4 values (Cataclysm re-levelled the dungeon; these entries were still at their 3.3.5 levels).
-- Each statement is guarded by the current levels, so the file is safe to run twice.
-- Baron Rivendare 10440, Grand Crusader Dathrohan 10812 and Balnazzar 10813 are left alone (the ending is ported separately).

UPDATE `creature_template` SET `minlevel` = 40, `maxlevel` = 40 WHERE `entry` = 8477 AND `minlevel` = 51 AND `maxlevel` = 58; -- Skeletal Servant
UPDATE `creature_template` SET `minlevel` = 45, `maxlevel` = 46 WHERE `entry` = 10426 AND `minlevel` = 45 AND `maxlevel` = 45; -- Risen Inquisitor
UPDATE `creature_template` SET `minlevel` = 60, `maxlevel` = 60 WHERE `entry` = 16046 AND `minlevel` = 1 AND `maxlevel` = 1; -- Jarien and Sothos Trigger
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 10399 AND `ScriptName` = 'npc_thuzadin_acolyte'; -- Thuzadin Acolyte: no code registers this name (4.3.4 and 3.3.5 leave it empty)
