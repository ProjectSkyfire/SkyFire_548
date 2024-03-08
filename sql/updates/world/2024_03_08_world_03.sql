UPDATE `creature_template` SET `gossip_menu_id`=15719 WHERE `entry`=66998;

DELETE FROM `gossip_menu` WHERE `entry`=15719;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(15719, 74212);

DELETE FROM `npc_text` WHERE `ID`=74212;
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES
(74212, 'Hello friend.');
