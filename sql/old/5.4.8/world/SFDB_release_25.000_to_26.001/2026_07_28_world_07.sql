-- Yan
SET @CGUID := 110004;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 56773, 870, 214.42707, 300.23263, 153.87234, 5.96782);

-- The Golden Dream
UPDATE `quest_template` SET `RequestItemsText`='Yes, did you need something?' WHERE `Id`=29581;
UPDATE `quest_template` SET `OfferRewardText`='Ah... dreamleaf! So you want to see the Golden Dream, do you?$B$BCome to think of it, I haven\'t seen the Dream in years - not since I was a cub spending long hours playing in the pools. Long before I took up the culinary arts.$B$BLooks like you brought enough dreamleaf for two cups of tea... so why not?$B$BHere\'s to better days!' WHERE `Id`=29581;
