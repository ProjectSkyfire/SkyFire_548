-- Gilneas, Last Stand (14222) and the stalkers of Sacrifices (14212) and By Blood and Ash (14218):
--  * Lord Darius Crowley respawns the Frenzied Stalkers (35627) within 100 yards when Last Stand is accepted, so the
--    eight kills are available again for the next player.
--  * The Frenzied Stalkers and the Bloodfang Stalkers (35229) enrage at 30 percent health, with the emote text. The
--    credit row of the Bloodfang Stalker (id 0, Throw Torch) stays as it is.
-- Based on Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (35566, 35627);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (35566, 35627);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 35229 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35566, 0, 0, 0, 19, 0, 100, 0, 14222, 0, 0, 0, 0, 70, 0, 0, 0, 0, 0, 0, 9, 35627, 0, 100, 0, 0, 0, 0, 'Lord Darius Crowley - On Quest Accepted (Last Stand) - Respawn Frenzied Stalkers'),
(35627, 0, 0, 1, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frenzied Stalker - At 30% HP - Cast Enrage'),
(35627, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frenzied Stalker - At 30% HP - Say Text Line 0'),
(35229, 0, 1, 2, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Stalker - At 30% HP - Cast Enrage'),
(35229, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Stalker - At 30% HP - Say Text Line 0');

DELETE FROM `creature_text` WHERE `entry` IN (35229, 35627) AND `groupid` = 0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(35229, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Bloodfang Stalker - enrage'),
(35627, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Frenzied Stalker - enrage');
