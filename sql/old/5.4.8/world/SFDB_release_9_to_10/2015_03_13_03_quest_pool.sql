SET @RDDQPOOL := 12003; -- Rik'kal the Dissector - Daily Quests
SET @QUESTID1 := 31234; -- Putting An Eye Out
SET @QUESTID2 := 31271; -- Bad Genes
SET @QUESTID3 := 31502; -- Wing Clip
SET @QUESTID4 := 31503; -- Shortcut to Ruin
SET @QUESTID5 := 31508; -- Specimen Request
SET @QUESTID6 := 31509; -- Fear Takes Root

DELETE FROM `pool_template` WHERE `entry`=@RDDQPOOL;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(@RDDQPOOL, 1, 'Rik''kal the Dissector - Daily Quests');

DELETE FROM `pool_quest` WHERE `pool_entry`=@RDDQPOOL;
INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
(@QUESTID1, @RDDQPOOL, 'Putting An Eye Out'),
(@QUESTID2, @RDDQPOOL, 'Bad Genes'),
(@QUESTID3, @RDDQPOOL, 'Wing Clip'),
(@QUESTID4, @RDDQPOOL, 'Shortcut to Ruin'),
(@QUESTID5, @RDDQPOOL, 'Specimen Request'),
(@QUESTID6, @RDDQPOOL, 'Fear Takes Root');
