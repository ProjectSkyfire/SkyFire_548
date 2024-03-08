-- Tinza Silvermug
DELETE FROM `gossip_menu_option` WHERE `menu_id`=12040;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(12040, 0, 5, 'Make this inn your home.', 8, 65536, 0, 0, 0, 0, ''),
(12040, 1, 1, 'Let me browse your goods.', 3, 128, 0, 0, 0, 0, '');

-- Delaniel
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=6620;
DELETE FROM `smart_scripts` WHERE `entryorguid`=15433;
UPDATE `creature_template` SET `npcflag`=65667 WHERE `entry`=15433;
DELETE FROM `gossip_menu_option` WHERE `menu_id`=6620;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(6620, 0, 5, 'Make this inn your home.', 8, 65536, 0, 0, 0, 0, ''),
(6620, 1, 1, 'Let me browse your goods.', 3, 128, 0, 0, 0, 0, '');
