DELETE FROM `gossip_menu` WHERE `entry` = 11777;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES (11777, 16509);

DELETE FROM `gossip_menu_option` WHERE `menu_id` = 11777;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES (11777, 0, 2, 'Show me where I can fly.', 4, 8192, 0, 0, 0, 0, '');
