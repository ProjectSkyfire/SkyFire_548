-- Admiral Aubrey
DELETE FROM `creature_text` WHERE `entry`=38619;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`, `comment`) VALUES
(38619, 'Mister Blaine, signal the shore batteries and let Mr. Whessan know $n is on $g his:her; way. Handsomely, now.', 12, 100, 'Admiral Aubrey SAY_0');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38619;

DELETE FROM `smart_scripts` WHERE `entryorguid`=38619;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38619, 0, 0, 0, 19, 0, 100, 0, 24938, 0, 0, 0, 0, 80, 3861900, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Admiral Aubrey - On Quest Accept - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=3861900;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3861900, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Admiral Aubrey - On Script - Talk 0'),
(3861900, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 38697, 10, 0, 0, 0, 0, 0, 'Admiral Aubrey - On Script - Set Data 1 1');

-- Nathan Blaine
DELETE FROM `creature_text` WHERE `entry`=38697;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`, `comment`) VALUES
(38697, 'Aye aye sir! Immediately, sir!', 12, 100, 'Nathan Blaine SAY_0');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38697;

DELETE FROM `creature_template_addon` WHERE `entry`=38697;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(38697, 0, 0, 0, 1, 0, '72534');

DELETE FROM `smart_scripts` WHERE `entryorguid`=38697;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38697, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 80, 3869700, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nathan Blaine - On Data Set - Run Script'),
(38697, 0, 1, 0, 1, 0, 100, 0, 1000, 2000, 3000, 4000, 0, 10, 3, 25, 94, 463, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Every 3 - 4 seconds (1 - 2s initially) (OOC) - Self: Play random emote: 3, 25, 51, 463');

DELETE FROM `smart_scripts` WHERE `entryorguid`=3869700;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3869700, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nathan Blaine - On Script - Talk 0'),
(3869700, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nathan Blaine - On Script - Set Data 1 0');

-- The Guns of Northwatch
UPDATE `quest_template` SET `OfferRewardText`='Aubrey sent you? Good! I take it you know your way around a cannon?' WHERE `Id`=24938;
