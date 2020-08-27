DELETE FROM `creature_template` WHERE `entry`=68986;
INSERT INTO `creature_template` (`entry`, `modelid1`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `type`, `WDBVerified`) VALUES
(68986, 47031, 'Li the Tamer', 'Stable Master', '', 5, 5, 4, 35, 35, 4194433, 2000, 2000, 1, 32768, 2048, 7, 18414);

DELETE FROM `creature_model_info` WHERE `modelid`=47031;
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES
(47031, 0.347222, 1.5, 0);
