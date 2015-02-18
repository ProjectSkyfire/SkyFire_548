SET @GOBJECT := 41602; -- Bronze Gift of the Crane
SET @ITEM := 80938; -- Gift of the Great Crane
DELETE FROM `gameobject_loot_template` WHERE `entry`=@GOBJECT;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(@GOBJECT, @ITEM, -100);
