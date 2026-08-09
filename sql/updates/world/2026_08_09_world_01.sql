-- Admiral Aubrey
UPDATE `creature_template` SET `gossip_menu_id`=11068 WHERE `entry`=38619;

UPDATE `creature_template` SET `npcflag`=3 WHERE `entry`=38619;

DELETE FROM `creature_text` WHERE `entry`=38619 AND `groupid`=1;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38619, 1, 0, 'Mister Blaine? Flag down a gryphon for $n if you please.', 12, 0, 100, 0, 0, 0, 'Admiral Aubrey SAY_1');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=11068 AND `ConditionValue1`=267225;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11068, 0, 0, 0, 48, 0, 267225, 0, 0, 0, 0, 0, '', 'Only allow gossip option to be visible if quest objective incomplete');

DELETE FROM `gossip_menu_option` WHERE `MenuID`=11068;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(11068, 0, 0, 'Admiral, can I get a ride to the Blackpool?', 0, 1, 1, 0, 0, 0, 0, NULL, 0, 18414);

DELETE FROM `smart_scripts` WHERE `entryorguid`=38619 AND `id`=1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38619, 0, 1, 0, 62, 0, 100, 0, 11068, 0, 0, 0, 0, 80, 3861901, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Admiral Aubrey - On Gossip Select - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=3861901;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3861901, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Admiral Aubrey - On Scipt - Close Gossip'),
(3861901, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Admiral Aubrey - On Script - Talk 1'),
(3861901, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 85, 72600, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Admiral Aubrey - On Script - Cast');

-- Captain Fisher <Master of the Blackpool>
UPDATE `creature_template` SET `gossip_menu_id`=11071 WHERE `entry`=38622;

UPDATE `creature_template` SET `npcflag`=3 WHERE `entry`=38622;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38622;

DELETE FROM `gossip_menu_option` WHERE `MenuID`=11071;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(11071, 0, 0, 'Captain, I need a ride back to Northwatch Hold.', 0, 1, 1, 0, 0, 0, 0, NULL, 0, 18414);

DELETE FROM `smart_scripts` WHERE `entryorguid`=38622;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38622, 0, 1, 0, 62, 0, 100, 0, 11071, 0, 0, 0, 0, 80, 3862200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captain Fisher - On Gossip Select - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=3862200;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3862200, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captain Fisher - On Scipt - Close Gossip'),
(3862200, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 85, 72610, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captain Fisher - On Script - Cast');

-- Northwatch Gryphon 1
UPDATE `creature_template` SET `VehicleId`=108 WHERE `entry`=38707;

UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry`=38707;

UPDATE `creature_template` SET `npcflag`=33554432 WHERE `entry`=38707;

UPDATE `creature_template` SET `speed_walk`=6 WHERE `entry`=38707;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38707;

DELETE FROM `waypoints` WHERE `entry`=38707;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(38707, 0, -2227.31, -3774.92, 142.144, 'Northwatch Gryphon - Path'),
(38707, 1, -2228.36, -3768.22, 141.755, 'Northwatch Gryphon - Path'),
(38707, 2, -2226.55, -3754.25, 140.505, 'Northwatch Gryphon - Path'),
(38707, 3, -2207.06, -3738.52, 134.255, 'Northwatch Gryphon - Path'),
(38707, 4, -2184.25, -3742.66, 126.437, 'Northwatch Gryphon - Path'),
(38707, 5, -2161.56, -3767.09, 115.172, 'Northwatch Gryphon - Path'),
(38707, 6, -2151.51, -3805.85, 102.086, 'Northwatch Gryphon - Path'),
(38707, 7, -2144.38, -3865.1, 82.5396, 'Northwatch Gryphon - Path'),
(38707, 8, -2150.32, -3910.4, 70.3986, 'Northwatch Gryphon - Path'),
(38707, 9, -2167.05, -3971.9, 59.0891, 'Northwatch Gryphon - Path'),
(38707, 10, -2186.56, -4021.22, 47.6923, 'Northwatch Gryphon - Path'),
(38707, 11, -2197.58, -4045.15, 41.6437, 'Northwatch Gryphon - Path'),
(38707, 12, -2200.43, -4068.22, 38.5375, 'Northwatch Gryphon - Path'),
(38707, 13, -2195.15, -4081.64, 35.7159, 'Northwatch Gryphon - Path'),
(38707, 14, -2186.58, -4087.07, 33.7828, 'Northwatch Gryphon - Path'),
(38707, 15, -2173.32, -4088.84, 29.1969, 'Northwatch Gryphon - Path'),
(38707, 16, -2161.21, -4083.64, 22.8661, 'Northwatch Gryphon - Path'),
(38707, 17, -2141.72, -4075.27, 44.0793, 'Northwatch Gryphon - Path');

DELETE FROM `smart_scripts` WHERE `entryorguid`=38707;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38707, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 38707, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Northwatch Gryphon - On Spawn - Start Waypoint Movement'),
(38707, 0, 1, 0, 40, 0, 100, 0, 16, 38707, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Northwatch Gryphon - On Reach WP - Despawn');

-- Northwatch Gryphon 2
UPDATE `creature_template` SET `VehicleId`=108 WHERE `entry`=38709;

UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry`=38709;

UPDATE `creature_template` SET `npcflag`=33554432 WHERE `entry`=38709;

UPDATE `creature_template` SET `speed_walk`=6 WHERE `entry`=38709;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38709;

DELETE FROM `waypoints` WHERE `entry`=38709;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(38709, 0, -2163.7466, -4085.2727, 28.29399, 'Northwatch Gryphon - Path'),
(38709, 1, -2206.6753, -4094.9202, 38.76632, 'Northwatch Gryphon - Path'),
(38709, 2, -2264.8767, -4084.441, 52.18295, 'Northwatch Gryphon - Path'),
(38709, 3, -2319.9167, -4016.2727, 56.76628, 'Northwatch Gryphon - Path'),
(38709, 4, -2358.2969, -3895.802, 71.84961, 'Northwatch Gryphon - Path'),
(38709, 5, -2356.059, -3835.3838, 81.87742, 'Northwatch Gryphon - Path'),
(38709, 6, -2323.4844, -3770.9114, 103.02383, 'Northwatch Gryphon - Path'),
(38709, 7, -2279.8923, -3746.731, 122.92632, 'Northwatch Gryphon - Path'),
(38709, 8, -2230.587, -3738.7395, 129.50717, 'Northwatch Gryphon - Path'),
(38709, 9, -2196.6736, -3758.6562, 141.53499, 'Northwatch Gryphon - Path'),
(38709, 10, -2195.0088, -3771.9758, 144.6185, 'Northwatch Gryphon - Path'),
(38709, 11, -2208.0764, -3780.111, 142.3961, 'Northwatch Gryphon - Path'),
(38709, 12, -2220.165, -3775.2327, 134.11206, 'Northwatch Gryphon - Path');

DELETE FROM `smart_scripts` WHERE `entryorguid`=38709;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38709, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 38709, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Northwatch Gryphon - On Spawn - Start Waypoint Movement'),
(38709, 0, 1, 0, 40, 0, 100, 0, 12, 38709, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Northwatch Gryphon - On Reach WP - Despawn');

-- Repel Boarders!
UPDATE `quest_template` SET `OfferRewardText`='You make a hell of a marine, $n.$B$BWe\'ll mop up the others. But it looks like the Horde fleet is disengaging - maybe to land more troops ashore?$B$BAdmiral Aubrey will know what to do. Talk to me when you\'re ready to fly back!' WHERE `Id`=24934;

-- The Guns of Northwatch
UPDATE `quest_template` SET `PrevQuestId`=24934 WHERE `Id`=24938;

