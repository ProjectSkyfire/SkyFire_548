SET @WULI := 63994;
SET @POOL := 12001;
SET @QUEST1 := 31519;
SET @QUEST2 := 31520;
SET @QUEST3 := 31522;
SET @QUEST4 := 31523;
SET @QUEST5 := 31524;
SET @QUEST6 := 31525;
SET @QUEST7 := 31526;
SET @QUEST8 := 31527;
SET @QUEST9 := 31528;

DELETE FROM `creature_queststarter` WHERE `id`=@WULI;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(@WULI, @QUEST1),
(@WULI, @QUEST2),
(@WULI, @QUEST3),
(@WULI, @QUEST4),
(@WULI, @QUEST5),
(@WULI, @QUEST6),
(@WULI, @QUEST7),
(@WULI, @QUEST8),
(@WULI, @QUEST9);

DELETE FROM `creature_questender` WHERE `id`=@WULI;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(@WULI, @QUEST1),
(@WULI, @QUEST2),
(@WULI, @QUEST3),
(@WULI, @QUEST4),
(@WULI, @QUEST5),
(@WULI, @QUEST6),
(@WULI, @QUEST7),
(@WULI, @QUEST8),
(@WULI, @QUEST9);

DELETE FROM `pool_template` WHERE `entry`=@POOL;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(@POOL, 1, 'Challenger Wuli - Daily Quests');

DELETE FROM `pool_quest` WHERE `pool_entry`=@POOL;
INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
(@QUEST1, @POOL, 'A Worthy Challenge: Yan-zhu the Uncasked'),
(@QUEST2, @POOL, 'A Worthy Challenge: Sha of Doubt'),
(@QUEST3, @POOL, 'A Worthy Challenge: Sha of Hatred'),
(@QUEST4, @POOL, 'A Worthy Challenge: Xin the Weaponmaster'),
(@QUEST5, @POOL, 'A Worthy Challenge: Raigonn'),
(@QUEST6, @POOL, 'A Worthy Challenge: Wing Leader Ner''onok'),
(@QUEST7, @POOL, 'A Worthy Challenge: Durand'),
(@QUEST8, @POOL, 'A Worthy Challenge: Flameweaver Koegler'),
(@QUEST9, @POOL, 'A Worthy Challenge: Darkmaster Gandling');
