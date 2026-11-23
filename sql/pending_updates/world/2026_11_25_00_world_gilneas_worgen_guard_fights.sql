-- Gilneas City: the Bloodfang Worgen and the Gilnean Royal Guards stand side by side and only fight once a player
-- starts a fight. Creatures never pick a fight with each other on their own, so the worgen script looks for a Royal
-- Guard within 35 yards when it spawns and every 10 seconds while idle. The Rampaging Worgen and the Bloodfang Worgen
-- also enrage at low health (30 percent and 10 percent), with the emote text.
-- Based on Legends of Azeroth 5.4.8. The existing Spell Practice credit rows of 35118 (ids 0 to 6) stay as they are.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 34884;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 34884;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 35118 AND `id` IN (7, 8, 9, 10);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34884, 0, 0, 1, 2, 0, 100, 1, 0, 10, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rampaging Worgen - At 10% HP - Cast Enrage'),
(34884, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rampaging Worgen - At 10% HP - Say Text Line 0'),
(35118, 0, 7, 8, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - At 30% HP - Cast Enrage'),
(35118, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - At 30% HP - Say Text Line 0'),
(35118, 0, 9, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 19, 35232, 35, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Reset - Start Attack (Gilnean Royal Guard, 35 Yards)'),
(35118, 0, 10, 0, 1, 0, 100, 0, 10000, 10000, 10000, 10000, 0, 49, 0, 0, 0, 0, 0, 0, 19, 35232, 35, 0, 0, 0, 0, 0, 'Bloodfang Worgen - OOC (10000 - 10000) - Start Attack (Gilnean Royal Guard, 35 Yards)');

DELETE FROM `creature_text` WHERE `entry` IN (34884, 35118) AND `groupid` = 0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34884, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Rampaging Worgen - enrage'),
(35118, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Bloodfang Worgen - enrage');
