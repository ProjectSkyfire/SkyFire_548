SET @ISDQPOOL := 12002; -- Instructor Skythorn - Daily Quests
SET @QUESTID1 := 30155; -- Restoring the Balance
SET @QUESTID2 := 30158; -- Disarming the Enemy
SET @QUESTID3 := 31698; -- Thinning The Pack
SET @QUESTID4 := 31706; -- Weeping Widows
SET @QUESTID5 := 31707; -- A Tangled Web
SET @QUESTID6 := 31712; -- Monkey Mischief

DELETE FROM `pool_template` WHERE `entry`=@ISDQPOOL;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(@ISDQPOOL, 1, 'Instructor Skythorn - Daily Quests');

DELETE FROM `pool_quest` WHERE `pool_entry`=@ISDQPOOL;
INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
(@QUESTID1, @ISDQPOOL, 'Restoring the Balance'),
(@QUESTID2, @ISDQPOOL, 'Disarming the Enemy'),
(@QUESTID3, @ISDQPOOL, 'Thinning The Pack'),
(@QUESTID4, @ISDQPOOL, 'Weeping Widows'),
(@QUESTID5, @ISDQPOOL, 'A Tangled Web'),
(@QUESTID6, @ISDQPOOL, 'Monkey Mischief');
