-- Heroic levels of Old Hillsbrad Foothills: the heroic templates below had levels that differ from the retail ones.

UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 71 WHERE `entry` = 20526; -- Durnholde Rifleman heroic, was 71-71
UPDATE `creature_template` SET `minlevel` = 71, `maxlevel` = 71 WHERE `entry` = 20548; -- Thrall heroic, was 67-67
UPDATE `creature_template` SET `minlevel` = 71, `maxlevel` = 72 WHERE `entry` = 20546; -- Tarren Mill Lookout heroic, was 72-72
UPDATE `creature_template` SET `minlevel` = 68, `maxlevel` = 69 WHERE `entry` = 20541; -- Orc Prisoner heroic, was 65-65
UPDATE `creature_template` SET `minlevel` = 69, `maxlevel` = 69 WHERE `entry` = 20524; -- Durnholde Cook heroic, was 70-71
