-- Guild Masters: restore petitioner/tabard gossip and assign missing menus.

-- Lysheana, Christopher Drakul, Jondor Steelbrow, Droha had no gossip_menu_id.
UPDATE `creature_template` SET `npcflag` = 786433, `gossip_menu_id` = 7487 WHERE `entry` = 4161;
UPDATE `creature_template` SET `npcflag` = 786433, `gossip_menu_id` = 5108 WHERE `entry` = 4613;
UPDATE `creature_template` SET `npcflag` = 786433, `gossip_menu_id` = 5462 WHERE `entry` = 5130;
UPDATE `creature_template` SET `npcflag` = 786433, `gossip_menu_id` = 7484 WHERE `entry` = 52277;

DELETE FROM `gossip_menu` WHERE `MenuID` IN (708, 1626, 5108, 5461, 5462, 7440, 7484, 7487, 9838);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`, `VerifiedBuild`) VALUES
(708, 1260, 0),
(1626, 2280, 0),
(5108, 6159, 0),
(5461, 6513, 0),
(5462, 6514, 0),
(7440, 9008, 0),
(7484, 9071, 0),
(7487, 9073, 0),
(9838, 13349, 0);

DELETE FROM `gossip_menu_option` WHERE `MenuID` IN (708, 1626, 5108, 5461, 5462, 7440, 7484, 7487, 9838);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(708, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(708, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0),
(1626, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(1626, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0),
(5108, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(5108, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0),
(5461, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(5461, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0),
(5462, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(5462, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0),
(7440, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(7440, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0),
(7484, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(7484, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0),
(7487, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(7487, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0),
(9838, 0, 7, 'How do I form a guild?', 3413, 10, 262144, 0, 0, 0, 0, NULL, 0, 0),
(9838, 1, 8, 'I want to create a guild crest.', 3415, 11, 524288, 0, 0, 0, 0, NULL, 0, 0);
