-- Tribunal of Ages: Brann casts the criteria marker 59046 when the event ends, which credits encounter bit 2
DELETE FROM `instance_encounters` WHERE `entry` IN (567, 568);
INSERT INTO `instance_encounters` (`entry`, `creditType`, `creditEntry`, `lastEncounterDungeon`, `comment`) VALUES
(567, 1, 59046, 0, 'Tribunal of Ages'),
(568, 1, 59046, 0, 'Tribunal of Ages');

DELETE FROM `spell_script_names` WHERE `spell_id` = 59046;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(59046, 'spell_gen_dungeon_credit');
