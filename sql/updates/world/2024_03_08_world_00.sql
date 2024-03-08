-- Brewmaster Skye
UPDATE `creature_template` SET `gossip_menu_id`=14224 WHERE `entry`=63008;

DELETE FROM `gossip_menu` WHERE `entry`=14224
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14224, 62445);

DELETE FROM `npc_text` WHERE `ID`=62445;
INSERT INTO `npc_text` (`ID`, `text0_1`) VALUES
(62445, 'Should try the special.');

DELETE FROM `gossip_menu_option` WHERE `menu_id`=14224;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(14224, 0, 5, 'Make this inn your home.', 8, 65536, 0, 0, 0, 0, ''),
(14224, 1, 1, 'Let me browse your goods.', 3, 128, 0, 0, 0, 0, '');
