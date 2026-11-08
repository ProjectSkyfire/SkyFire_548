-- The Culling of Stratholme: credit the Mal'Ganis kill for the dungeon reward.
-- Mal'Ganis casts spell 58630 on death as the encounter credit, but nothing listened for it: the encounter
-- rows and the script binding that the other dungeons with a cast credit have were missing.
-- Idempotent: every INSERT is preceded by a DELETE of its keys.

DELETE FROM `spell_script_names` WHERE `spell_id`=58630;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(58630, 'spell_gen_dungeon_credit');

DELETE FROM `instance_encounters` WHERE `entry` IN (296, 300);
INSERT INTO `instance_encounters` (`entry`, `creditType`, `creditEntry`, `lastEncounterDungeon`, `comment`) VALUES
(296, 1, 58630, 209, 'Mal''ganis'),
(300, 1, 58630, 210, 'Mal''ganis');
