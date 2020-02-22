DELETE FROM `gossip_menu_option` WHERE `menu_id` = 14971;
INSERT INTO `gossip_menu_option` (`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`box_coded`,`box_money`,`box_text`) VALUES
(14971, 0, 0, "I am ready to depart.", 1, 1, 0, 0, 0, 0, ""),
(14971, 1, 0, "You really have it in for the Horde, don't you?", 1, 1, 0, 0, 0, 0, "");

UPDATE `creature_template` SET `gossip_menu_id`=14971, `npcflag`=`npcflag`|1|2, `AIName`='SmartAI' WHERE `entry`=66292;
DELETE FROM `smart_scripts` WHERE `entryorguid`=66292;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(66292, 0, 0, 1, 62, 0, 100, 0, 14971, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sky Admiral Rogers - On gossip select - Close gossip'),
(66292, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 33, 66292, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sky Admiral Rogers - On linked - Quest credit'),
(66292, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 85, 130321, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sky Admiral Rogers - On linked - Player Cast 130321 on Self');
