-- Captain Tharran
DELETE FROM `creature_text` WHERE `entry`=40950;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`, `comment`) VALUES
(40950, 'There\'s no substitute for live observers, but I need those mountaineers here. We\'ll have to rely on those little gnomish observer bots for now...', 12, 100, 'Mountaineer Dunstan - SAY_0');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=40950;

DELETE FROM `smart_scripts` WHERE `entryorguid`=40950 AND `event_param1`=313;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40950, 0, 0, 0, 19, 0, 100, 0, 313, 0, 0, 0, 0, 80, 4095000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captain Tharran - On Quest Accept - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4095000;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4095000, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captain Tharran - On Scipt - Talk 0');

-- Mountaineer Dunstan
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`= 15 AND `SourceGroup`=11455 AND `ConditionValue1`=252811;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11455, 0, 0, 0, 48, 0, 252811, 0, 0, 0, 0, 0, '', 'Only allow gossip option to be visible if quest objective incomplete');

UPDATE `gossip_menu_option` SET `OptionType`=1 WHERE `MenuID`=11455;
UPDATE `gossip_menu_option` SET `OptionNpcflag`=1 WHERE `MenuID`=11455;

DELETE FROM `creature_text` WHERE `entry`=40991;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`, `emote`, `comment`) VALUES
(40991, 'Tell Captain Tharran that I\'ll be back in Kharanos as soon as I\'ve verified that the bot is working correctly.', 12, 100, 66, 'Mountaineer Dunstan - SAY_0');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=40991;

DELETE FROM `smart_scripts` WHERE `entryorguid`=40991;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40991, 0, 0, 0, 62, 0, 100, 0, 11455, 0, 0, 0, 0, 80, 4099100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Dunstan - On Gossip Select - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4099100;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4099100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Dunstan - On Scipt - Close Gossip'),
(4099100, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Dunstan - On Script - Talk 0'),
(4099100, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 12, 41052, 1, 30000, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Dunstan - On Script - Summon Remote Observation Bot'),
(4099100, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 33, 40991, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Dunstan - On Script - Give Credit');

-- Mountaineer Lewin
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`= 15 AND `SourceGroup`=11456 AND `ConditionValue1`=252812;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11456, 0, 0, 0, 48, 0, 252812, 0, 0, 0, 0, 0, '', 'Only allow gossip option to be visible if quest objective incomplete');

UPDATE `gossip_menu_option` SET `OptionType`=1 WHERE `MenuID`=11456;
UPDATE `gossip_menu_option` SET `OptionNpcflag`=1 WHERE `MenuID`=11456;

DELETE FROM `creature_text` WHERE `entry`=40994;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`, `emote`, `comment`) VALUES
(40994, 'I can\'t wait to get out there and help in the fight against those trolls.', 12, 100, 66, 'Mountaineer Lewin - SAY_0');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=40994;

DELETE FROM `smart_scripts` WHERE `entryorguid`=40994;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40994, 0, 0, 0, 62, 0, 100, 0, 11456, 0, 0, 0, 0, 80, 4099400, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Lewin - On Gossip Select - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4099400;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4099400, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Lewin - On Scipt - Close Gossip'),
(4099400, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Lewin - On Script - Talk 0'),
(4099400, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 12, 41052, 1, 30000, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Lewin - On Script - Summon Remote Observation Bot'),
(4099400, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 33, 40994, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Lewin - On Script - Give Credit');


-- Mountaineer Valgrum
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`= 15 AND `SourceGroup`=11457 AND `ConditionValue1`=252813;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11457, 0, 0, 0, 48, 0, 252813, 0, 0, 0, 0, 0, '', 'Only allow gossip option to be visible if quest objective incomplete');

UPDATE `gossip_menu_option` SET `OptionType`=1 WHERE `MenuID`=11457;
UPDATE `gossip_menu_option` SET `OptionNpcflag`=1 WHERE `MenuID`=11457;

DELETE FROM `creature_text` WHERE `entry`=41056;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`, `emote`, `comment`) VALUES
(41056, 'We\'ve been fighting nonstop since the cataclysm. It\'ll be nice to get a rest, if a brief one.', 12, 100, 66, 'Mountaineer Valgrum - SAY_0');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41056;

DELETE FROM `smart_scripts` WHERE `entryorguid`=41056;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41056, 0, 0, 0, 62, 0, 100, 0, 11457, 0, 0, 0, 0, 80, 4105600, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Valgrum - On Gossip Select - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4105600;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4105600, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Valgrum - On Scipt - Close Gossip'),
(4105600, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Valgrum - On Script - Talk 0'),
(4105600, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 12, 41052, 1, 30000, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Valgrum - On Script - Summon Remote Observation Bot'),
(4105600, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 33, 41056, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Valgrum - On Script - Give Credit');

-- Remote Observation Bot
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41052;

DELETE FROM `smart_scripts` WHERE `entryorguid`=41052;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41052, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 89, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Remote Observation Bot - On Respawn - Set Random Movement');
