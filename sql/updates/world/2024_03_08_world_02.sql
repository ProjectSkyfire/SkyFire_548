UPDATE `creature_template` SET `gossip_menu_id`=14580, `npcflag`=1 WHERE `entry`=64043;

DELETE FROM `gossip_menu` WHERE `entry`=14580;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14580, 65082);

DELETE FROM `npc_text` WHERE `ID`=65082;
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES
(65082, 'Why was the tank nervous before the fight?$B$B...$B$BBecause he was a worrier!');
