-- The Four Horsemen encounter was credited through spell 59450, which the 5.4.8 client does not have; the last Horseman
-- to die now credits it through a kill credit row on Highlord Mograine, who is not spawned in the raid
DELETE FROM `instance_encounters` WHERE `entry` = 1121;
INSERT INTO `instance_encounters` (`entry`, `creditType`, `creditEntry`, `lastEncounterDungeon`, `comment`) VALUES
(1121, 0, 16062, 0, 'The Four Horsemen');
