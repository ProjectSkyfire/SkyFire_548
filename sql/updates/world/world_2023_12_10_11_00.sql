DELETE FROM `gossip_menu` WHERE `entry` = 13046;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES (13046, 18321);

DELETE FROM `gossip_menu_option` WHERE `menu_id` = 13046;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES (13046, 0, 2, 'I\'d like to travel by air.', 4, 8192, 0, 0, 0, 0, NULL);