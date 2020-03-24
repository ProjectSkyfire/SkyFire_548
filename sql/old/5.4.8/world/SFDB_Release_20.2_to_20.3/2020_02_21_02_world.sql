UPDATE `creature_template` SET `gossip_menu_id`='14988' WHERE `entry`=55054;

DELETE FROM `gossip_menu` WHERE `entry`= 14988;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14988, 21186);

DELETE FROM `gossip_menu_option` WHERE `menu_id` = 14988;
INSERT INTO `gossip_menu_option` (`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`box_coded`,`box_money`,`box_text`) VALUES
(14988, 0, 0, "I'm ready to go, General.", 1, 1, 0, 0, 0, 0, "");

SET @CGUID := 100051;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-1 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 55135, 870, 3176.74, -694.437, 321.444, 2.42254),
(@CGUID-1, 55054, 1, 1862.281, -5461.901, 443.814, 3.174382);
