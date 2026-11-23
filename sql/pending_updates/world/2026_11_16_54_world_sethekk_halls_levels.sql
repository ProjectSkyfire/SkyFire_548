-- Sethekk Halls: the normal-mode templates carried the heroic level as their maximum, so every normal
-- spawn rolled a random level up to 72. Level ranges follow TrinityCore 3.3.5; heroic rows are untouched.
UPDATE `creature_template` SET `maxlevel` = 68 WHERE `entry` = 18319 AND `minlevel` = 67 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 69 WHERE `entry` = 18321 AND `minlevel` = 68 AND `maxlevel` = 72;
UPDATE `creature_template` SET `maxlevel` = 68 WHERE `entry` = 18323 AND `minlevel` = 67 AND `maxlevel` = 71;
UPDATE `creature_template` SET `maxlevel` = 69 WHERE `entry` = 18325 AND `minlevel` = 68 AND `maxlevel` = 72;
UPDATE `creature_template` SET `maxlevel` = 68 WHERE `entry` = 18328 AND `minlevel` = 67 AND `maxlevel` = 71;
