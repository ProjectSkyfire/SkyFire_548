-- Auchenai Crypts: the normal-mode templates carried the heroic level as their maximum, so every normal
-- spawn rolled a random level up to 72. Level ranges follow TrinityCore 3.3.5; heroic rows are untouched.
UPDATE `creature_template` SET `maxlevel` = 67 WHERE `entry` = 18373 AND `minlevel` = 67 AND `maxlevel` = 72;
UPDATE `creature_template` SET `maxlevel` = 66 WHERE `entry` = 18524 AND `minlevel` = 66 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 66 WHERE `entry` = 18558 AND `minlevel` = 65 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 66 WHERE `entry` = 18559 AND `minlevel` = 65 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 67 WHERE `entry` = 18700 AND `minlevel` = 66 AND `maxlevel` = 72;
UPDATE `creature_template` SET `maxlevel` = 67 WHERE `entry` = 18702 AND `minlevel` = 66 AND `maxlevel` = 72;
UPDATE `creature_template` SET `maxlevel` = 66 WHERE `entry` = 18778 AND `minlevel` = 65 AND `maxlevel` = 71;
