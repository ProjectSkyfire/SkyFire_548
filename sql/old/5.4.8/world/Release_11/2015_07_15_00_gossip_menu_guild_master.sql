DELETE FROM `gossip_menu_option` WHERE `menu_id`= 708;

INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES (708, 0, 7, 'How do I form a guild?', 10, 524288, 0, 0, 0, 0, '');
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES (708, 1, 8, 'I want to create a guild crest.', 11, 262144, 0, 0, 0, 0, '');
