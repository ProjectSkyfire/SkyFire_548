-- Achievement: Hozen in the Mist, Item: Hozen in the Mist
SET @ACHIEVEMENT := 6850;
SET @ITEM        := 83770;
SET @SENDER      := 61962;
DELETE FROM `achievement_reward` WHERE `entry`=@ACHIEVEMENT;
INSERT INTO `achievement_reward` (`entry`, `item`, `sender`, `subject`, `text`) VALUES
(@ACHIEVEMENT, @ITEM, @SENDER, 'Studying the Hozen', 'Greetings $N.$B$BI understand you have been doing some research into the nature of the hozen. You are very brave to do so - they are an extremely unruly people. Come! Join me at the Seat of Knowledge and we will compare what we have learned.$B$BIncluded should be a map to our location. I look forward to our next meeting.$B$BSincerely,$BLorewalker Cho');
