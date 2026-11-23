-- Hodir, Thorim and Freya had no encounter credit rows, so their kills never counted for the instance
DELETE FROM `instance_encounters` WHERE `entry` IN (1133, 1135, 1141);
INSERT INTO `instance_encounters` (`entry`, `creditType`, `creditEntry`, `lastEncounterDungeon`, `comment`) VALUES
(1133, 0, 32906, 0, 'Freya'),
(1135, 0, 32845, 0, 'Hodir'),
(1141, 0, 32865, 0, 'Thorim');
