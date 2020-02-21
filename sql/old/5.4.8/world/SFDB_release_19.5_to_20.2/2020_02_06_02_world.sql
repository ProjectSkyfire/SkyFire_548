DELETE FROM `creature_template_addon` WHERE `entry`=34913;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(34913, 0, 2410, 0, 1, 0, NULL);

UPDATE `creature_template` SET flags_extra=flags_extra|2 WHERE entry IN (34913, 34936);
