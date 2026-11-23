-- Protecting the Shipment (309) cannot be finished, and Keep Your Hands Off The Goods! (13650) waits behind it. The quest
-- is an event: accepting it from Huldar sends Miran off with the powder kegs, and the Dark Iron Ambushers and then Saean
-- attack the cart; Huldar gives the quest credit when Saean dies. Huldar, Miran, Saean and the Ambushers have no AI, no
-- speech and Miran has no path. The script, the speech and the path are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (1379, 1380, 1981, 2057);

DELETE FROM `waypoints` WHERE `entry` = 1379;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(1379, 1, -5692.14, -3573.77, 309.397, 'Miran'),
(1379, 2, -5680.29, -3588.15, 310.965, 'Miran'),
(1379, 3, -5670.02, -3617.32, 311.512, 'Miran');

DELETE FROM `creature_text` WHERE `entry` IN (1379, 1380, 1981, 2057);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(1379, 0, 0, 'I''m off. Stay safe - I''ll be back as quickly as I''m able.', 12, 7, 100, 0, 0, 0, 'Miran'),
(1380, 0, 0, 'Sorry, Huldar... But it seems I''ve misled you.', 12, 7, 100, 0, 0, 0, 'Saean'),
(1981, 0, 0, 'We have need of your explosives. I''ll take pleasure in killing you for them.', 14, 7, 100, 0, 0, 0, 'Dark Iron Ambusher'),
(2057, 0, 0, 'I appreciate your help in this. We never would have suspected Saean as a sympathizer - we just weren''t prepared for this.', 12, 7, 100, 0, 0, 0, 'Huldar'),
(2057, 1, 0, 'Good work! I''m glad to see some revenge taken on that traitor. Go ahead and take the path Miran did to the east and let Prospector Ironband know we''re all set. I''m sure he''ll have some work for you as well.', 12, 7, 100, 0, 0, 0, 'Huldar');

DELETE FROM `smart_scripts` WHERE (`entryorguid` IN (1379, 1380, 1981, 2057) AND `source_type` = 0) OR (`entryorguid` = 205700 AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1379, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 0, 2000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Miran - On Data Set 0 1 - Say Line 0'),
(1379, 0, 1, 0, 52, 0, 100, 0, 0, 1379, 0, 0, 0, 53, 1, 1379, 0, 309, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Miran - On Text Over 0 - Waypoint Start'),
(1379, 0, 2, 0, 58, 0, 100, 0, 3, 1379, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Miran - On Waypoint Ended - Despawn after 2 sec'),
(1380, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Saean - On Summoned - Set Run 1'),
(1380, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5701.48, -3551.7, 306, 0, 'Saean - Linked To Id 0 - Move To Pos'),
(1380, 0, 2, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Saean - On Aggro - Say Line 0'),
(1380, 0, 3, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 2057, 20, 0, 0, 0, 0, 0, 'Saean - On Death - Set Data 0 1'),
(1981, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Ambusher - On Summoned - Set Run 1'),
(1981, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5701.48, -3551.7, 306, 0, 'Dark Iron Ambusher - Linked To Id 0 - Move To Pos'),
(1981, 0, 2, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Ambusher - On Data Set 0 1 - Say Line 0'),
(2057, 0, 0, 1, 19, 0, 100, 0, 309, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Huldar - On Accepted Quest - Store Target List'),
(2057, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 1379, 5, 0, 0, 0, 0, 0, 'Huldar - Linked To Id 0 - Set Data 0 1'),
(2057, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 205700, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huldar - Linked To Id 1 - Run Script'),
(2057, 0, 3, 4, 38, 0, 100, 0, 0, 1, 0, 0, 0, 15, 309, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Huldar - On Data Set 0 1 - Quest Credit'),
(2057, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Huldar - Linked To Id 3 - Set Orientation'),
(2057, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huldar - Linked To Id 4 - Say Line 1'),
(2057, 0, 6, 0, 52, 0, 100, 0, 1, 2057, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huldar - On Text Over 1 - Despawn'),
(205700, 9, 0, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Huldar - On Script - Set orientation'),
(205700, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huldar - On Script - Say Line 0'),
(205700, 9, 2, 0, 0, 0, 100, 0, 9000, 9000, 0, 0, 0, 12, 1981, 6, 10000, 1, 0, 0, 8, 0, 0, 0, -5731.4, -3562.37, 307.507, 0, 'Huldar - On Script - SSummon Creature Dark Iron Ambusher'),
(205700, 9, 3, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 1981, 100, 0, 0, 0, 0, 0, 'Huldar - On Script - Set Data 0 1'),
(205700, 9, 4, 0, 0, 0, 100, 0, 400, 400, 0, 0, 0, 12, 1981, 6, 10000, 1, 0, 0, 8, 0, 0, 0, -5744.27, -3558.1, 311.339, 0, 'Huldar - On Script - Summon Creature Dark Iron Ambusher'),
(205700, 9, 5, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, 12, 1981, 6, 10000, 1, 0, 0, 8, 0, 0, 0, -5747.23, -3586.86, 317.349, 0, 'Huldar - On Script - Summon Creature Dark Iron Ambusher'),
(205700, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 1380, 6, 10000, 1, 0, 0, 8, 0, 0, 0, -5744.54, -3586.57, 316.945, 0, 'Huldar - On Script - Summon Creature Saean');
