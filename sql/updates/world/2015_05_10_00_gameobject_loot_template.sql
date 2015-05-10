-- Quest: A Shade of Dread (31210)
SET @OLOOT :=42807;
SET @QITEM :=85573;

DELETE FROM `gameobject_loot_template` WHERE `entry`=@OLOOT AND `item`=@QITEM;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(@OLOOT, @QITEM, -100);
