-- Reinforcements for Loch Modan
UPDATE `quest_template` SET `OfferRewardText`='You don\'t know how glad I am to see fresh faces from Ironforge. Perhaps sending Mountaineer Barleybrew wasn\'t a waste after all.' WHERE `Id`=26131;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=4360;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 4360, 1, 0, 0, 28, 0, 26131, 0, 0, 0, 0, 0, '', 'Show gossip if quest "Reinforcements for Loch Modan" complete');

DELETE FROM `gossip_menu_option` WHERE `MenuID`=4360;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(4360, 0, 2, 'I need a ride.', 3409, 4, 8192, 0, 0, 0, 0, NULL, 0, 15595),
(4360, 1, 0, 'Mountaineer Barleybrew said you could help me get to Loch Modan.', 42071, 1, 1, 0, 0, 0, 0, NULL, 0, 14545);

-- Gryth Thurden
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=1573;

DELETE FROM `smart_scripts` WHERE `entryorguid`=1573;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1573, 0, 0, 0, 62, 0, 100, 0, 4360, 1, 0, 0, 0, 80, 157300, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gryth Thurden - On Gossip Select - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=157300;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(157300, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gryth Thurden - On Script - Close Gossip'),
(157300, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 52, 2206, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gryth Thurden - On Script - Activate Taxi');