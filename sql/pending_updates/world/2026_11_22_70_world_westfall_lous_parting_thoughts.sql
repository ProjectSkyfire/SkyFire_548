-- Lou's Parting Thoughts (26232) cannot be finished. Walking into the area trigger (5988) is meant
-- to start the conversation of the four thugs, which the player overhears for the kill credit (42417) after the thugs
-- attack and die. The thugs (42387) and the trigger (42562) are spawned, but neither has a script, there is no script
-- for the area trigger and no creature text, so nothing happens.
-- Area trigger, scripts and text are as in the Cataclysm Preservation Project 4.3.4; the text rows are the same in
-- Legends of Azeroth 5.4.8. The thugs stay the static spawns they already are, so only the script names are set.

UPDATE `creature_template` SET `ScriptName` = 'npc_westfall_thug' WHERE `entry` = 42387;
UPDATE `creature_template` SET `ScriptName` = 'npc_westfall_lous_parting_thoughts_trigger' WHERE `entry` = 42562;

DELETE FROM `areatrigger_scripts` WHERE `entry` = 5988;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(5988, 'at_westfall_two_shoed_lou_thugs');

DELETE FROM `creature_text` WHERE `entry` IN (42387, 42562);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42387, 0, 0, 'Did you... Did you meet her?', 12, 7, 100, 6, 0, 0, 'Thug to Player'),
(42387, 1, 0, 'Yep. She''s for real.', 12, 7, 100, 396, 0, 0, 'Thug to Player'),
(42387, 2, 0, 'She wanted me to tell you lugs that she appreciates the job that we did for her on the Furlbrows. Gave me a pile o'' gold to split with you all.', 12, 7, 100, 396, 0, 0, 'Thug to Player'),
(42387, 3, 0, 'See her face? Is it really...', 12, 0, 100, 6, 0, 0, 'Thug to Player'),
(42387, 4, 0, 'Whoa, what do we have here? Looks like we have ourselves an eavesdropper, boys.', 12, 0, 100, 5, 0, 0, 'Thug to Player'),
(42387, 5, 0, 'Only one thing to do with a lousy, good-for-nothin eavesdropper.', 12, 0, 100, 397, 0, 0, 'Thug to Player'),
(42387, 6, 0, 'DIE!', 12, 0, 100, 53, 0, 0, 'Thug to Player'),
(42562, 0, 0, 'Hurry back to the Furlbrow''s Cottage!', 42, 0, 100, 0, 0, 0, 'Lou''s Parting Thoughts Trigger to Player');
