UPDATE `gossip_menu_option` SET `option_id`=1, `npc_option_npcflag`=1 WHERE `menu_id`=12125 AND `id`=0;
UPDATE `creature_template` SET `npcflag`=1, `AIName`='SmartAI' WHERE `entry`=15188;

DELETE FROM `smart_scripts` WHERE `entryorguid`=15188;
INSERT INTO `smart_scripts` (`entryorguid`, `event_type`, `event_param1`, `event_param2`, `action_type`, `action_param1`, `action_param2`, `target_type`, `comment`) VALUES
(15188, 62, 12125, 0, 85, 86565, 2, 7, 'Cenarion Emissary Blackhoof - On gossip select - Player Cast 86565 on Self');

DELETE FROM `spell_target_position` WHERE `id`=86565;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(86565, 1, 7827.74, -2423.115, 488.893, 3.22);
