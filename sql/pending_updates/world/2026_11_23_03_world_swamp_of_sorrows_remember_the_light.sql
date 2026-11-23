-- Remember the Light (24913) cannot be finished. Accepting it from Anchorite Avuun (17127) starts a scene at the
-- Harborage: a second Avuun (47001) and Prophet Velen (46986) are summoned and talk with Avuun and Magtoor (1776), and at
-- the end the quest gets its credit (47541). Anchorite Avuun had no SmartAI, and neither speaker had speech, so nothing
-- happened after accepting. Script, speech and Avuun's greeting (menu 12161) are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI', `gossip_menu_id` = 12161 WHERE `entry` = 17127 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` = 17127 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17127, 0, 0, 0, 19, 0, 100, 0, 24913, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anchorite Avuun - On Quest Accepted - Set Data 1'),
(17127, 0, 1, 0, 38, 0, 100, 0, 1, 0, 55000, 55000, 0, 80, 1712700, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anchorite Avuun - On Data Set 1 - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 1712700 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1712700, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Store Target'),
(1712700, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 47001, 3, 50000, 0, 0, 0, 8, 0, 0, 0, -10096.9, -2754.94, 25.4567, 4.55531, 'Anchorite Avuun - Script - Summon Anchorite Avuun'),
(1712700, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 46986, 3, 50000, 0, 0, 0, 8, 0, 0, 0, -10101.9, -2756.57, 25.3471, 6.24392, 'Anchorite Avuun - Script - Summon Prophet Velen'),
(1712700, 9, 3, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 47001, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 0'),
(1712700, 9, 4, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 46986, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 0'),
(1712700, 9, 5, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 47001, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 1'),
(1712700, 9, 6, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 46986, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 1'),
(1712700, 9, 7, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 46986, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 2'),
(1712700, 9, 8, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 19, 46986, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 3'),
(1712700, 9, 9, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 1776, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 0'),
(1712700, 9, 10, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 19, 46986, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 4'),
(1712700, 9, 11, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 19, 46986, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 5'),
(1712700, 9, 12, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 47001, 20, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Say Line 2'),
(1712700, 9, 13, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 33, 47541, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Anchorite Avuun - Script - Killcredit');

DELETE FROM `creature_text` WHERE `entry` IN (1776, 46986, 47001);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(1776, 0, 0, 'Yes...', 12, 0, 100, 0, 0, 0, ''),
(46986, 0, 0, 'Some have spent decades meditating upon this mystery, Avuun. I do not wish the same fate upon you.', 12, 0, 100, 0, 0, 0, ''),
(46986, 1, 0, 'The Light wishes suffering on none, my child. But it does not reign unopposed in our realm.', 12, 0, 100, 396, 0, 0, ''),
(46986, 2, 0, 'Even now, the true battle between the forces of Light and Darkness approaches. We will all be called to join, and in the face of this conflict, all mortal suffering will be meaningless.', 12, 0, 100, 396, 0, 0, ''),
(46986, 3, 0, 'Magtoor, you have endured your plight bravely. Are you ready to return to the embrace of the Light?', 12, 0, 100, 396, 0, 0, ''),
(46986, 4, 0, 'Then may the Light save you and raise you up.', 12, 0, 100, 0, 0, 0, ''),
(46986, 5, 0, 'Remember, Avuun. Not all who wander are lost.', 12, 0, 100, 396, 0, 0, ''),
(47001, 0, 0, 'Even now, nothing. Why? Why would the Light forsake the innocent?', 12, 0, 100, 6, 0, 0, ''),
(47001, 1, 0, 'Prophet Velen!', 12, 0, 100, 5, 0, 0, ''),
(47001, 2, 0, '...thank you, Prophet. You have given both of us great comfort. I will continue my work, in faith and memory of Magtoor.', 12, 0, 100, 273, 0, 0, '');
