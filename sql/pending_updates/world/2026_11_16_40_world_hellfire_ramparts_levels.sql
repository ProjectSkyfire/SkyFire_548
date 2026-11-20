-- Hellfire Ramparts: the normal-mode templates carried the heroic level as their maximum
-- (60-71 for a level 60 trash pack), so every normal spawn rolled a random level up to 71.
-- Level ranges follow TrinityCore 3.3.5; the heroic rows (difficulty_entry_1) are untouched.
UPDATE `creature_template` SET `maxlevel` = 61 WHERE `entry` = 17259 AND `minlevel` = 60 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 61 WHERE `entry` = 17269 AND `minlevel` = 60 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 61 WHERE `entry` = 17270 AND `minlevel` = 60 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 61 WHERE `entry` = 17271 AND `minlevel` = 60 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 61 WHERE `entry` = 17281 AND `minlevel` = 61 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 62 WHERE `entry` = 17307 AND `minlevel` = 62 AND `maxlevel` = 72;
UPDATE `creature_template` SET `maxlevel` = 62 WHERE `entry` = 17308 AND `minlevel` = 62 AND `maxlevel` = 72;
UPDATE `creature_template` SET `maxlevel` = 59 WHERE `entry` = 17309 AND `minlevel` = 59 AND `maxlevel` = 69;
UPDATE `creature_template` SET `maxlevel` = 60 WHERE `entry` = 17455 AND `minlevel` = 60 AND `maxlevel` = 70;
UPDATE `creature_template` SET `maxlevel` = 61 WHERE `entry` = 17478 AND `minlevel` = 60 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 58 WHERE `entry` = 17517 AND `minlevel` = 57 AND `maxlevel` = 69;
