-- The Faction Champions encounter had no credit row; the champion controller now credits it when the last champion dies
DELETE FROM `instance_encounters` WHERE `entry`=1086;
INSERT INTO `instance_encounters` (`entry`, `creditType`, `creditEntry`, `lastEncounterDungeon`, `comment`) VALUES
(1086, 0, 34781, 0, 'Faction Champions');
