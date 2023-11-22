UPDATE `creature_template` SET `gossip_menu_id`=14672 WHERE `entry`=54567;
DELETE FROM `npc_text` WHERE `ID`=66068;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(66068, '', '<Aysa appears to be deeply lost in her excercises.>');
DELETE FROM `gossip_menu` WHERE `entry`=14672;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14672, 66068);

UPDATE `creature_template` SET `gossip_menu_id`=14616 WHERE `entry`=64929;
DELETE FROM `npc_text` WHERE `ID`=65901;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(65901, 'Whoever said they\'d rather light a candle than curse the darkness...$b$bWell, they probably haven\'t lit as many candles as me.', '');
DELETE FROM `gossip_menu` WHERE `entry`=14616;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14616, 65901);
