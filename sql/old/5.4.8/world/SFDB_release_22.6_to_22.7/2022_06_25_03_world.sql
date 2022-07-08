-- Wrathion gossip_menu_option
DELETE FROM `gossip_menu_option` WHERE `menu_id`=14936;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(14936, 0, 0, 'Who are you?', 1, 1, 0, 0, 0, 0, NULL),
(14936, 1, 0, 'I\'m ready for that drink now.', 1, 1, 0, 0, 0, 0, NULL);
