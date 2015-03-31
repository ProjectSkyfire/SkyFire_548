SET @SOURCETYPE := 1; -- Quest
SET @QUESTID    := 14; -- The People's Militia

DELETE FROM `disables` WHERE `sourceType`=@SOURCETYPE AND `entry`=@QUESTID;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(@SOURCETYPE, @QUESTID, 'Disable Quest 14: The People\'s Militia');
