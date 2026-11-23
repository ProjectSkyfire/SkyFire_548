-- Goods from Silvermoon City (9130) is handed in to Skymaster Brightdawn (44036) and Fly to Silvermoon City (9133)
-- is given by him, but he was not spawned, and his template had no gossip menu, so he could not give the flight either.
-- As in Legends of Azeroth 5.4.8 he stands about 75 yards from Sathiel (16261) and uses the gossip menu 6944
-- ("Show me where I can fly."), which already exists here.

UPDATE `creature_template` SET `gossip_menu_id` = 6944, `npcflag` = 8195 WHERE `entry` = 44036;

DELETE FROM `creature` WHERE `guid` = 9311547;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9311547, 44036, 530, 1, 0, 0, 0, 0, 8743.85, -6651.69, 70.2161, 5.41052, 300, 0, 0, 0, 0, 0, 0, 0, 0);
