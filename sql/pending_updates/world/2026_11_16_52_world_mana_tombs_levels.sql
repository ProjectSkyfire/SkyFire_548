-- Mana-Tombs: the normal-mode templates carried the heroic level as their maximum, so every normal
-- spawn rolled a random level up to 72. Level ranges follow TrinityCore 3.3.5; heroic rows are untouched.
UPDATE `creature_template` SET `maxlevel` = 64 WHERE `entry` = 19306 AND `minlevel` = 64 AND `maxlevel` = 70;
UPDATE `creature_template` SET `maxlevel` = 64 WHERE `entry` = 19307 AND `minlevel` = 64 AND `maxlevel` = 70;
