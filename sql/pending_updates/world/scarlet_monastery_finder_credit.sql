-- Classic Whitemane (450) is marked as the last encounter of dungeon 164, which is the Scarlet Monastery. Clear it and
-- credit the encounters of the Scarlet Monastery on map 1004: Thalnos and Korloff, and Whitemane as the last one.

UPDATE `instance_encounters` SET `lastEncounterDungeon` = 0 WHERE `entry` = 450;

DELETE FROM `instance_encounters` WHERE `entry` IN (1423, 1424, 1425);
INSERT INTO `instance_encounters` (`entry`, `creditType`, `creditEntry`, `lastEncounterDungeon`, `comment`) VALUES
(1423, 0, 59789, 0, 'Thalnos the Soulrender'),
(1424, 0, 59223, 0, 'Brother Korloff'),
(1425, 0, 3977, 164, 'High Inquisitor Whitemane');
