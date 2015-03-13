SET @GREENTEALEAF      := 72234; -- Green Tea Leaf
SET @GREENTEALEAFREF   := 11919; -- Green Tea Leaf Ref ID
SET @SNOWLILY          := 79010; -- Snow Lily
SET @SNOWLILYREF       := 11920; -- Snow Lily Ref ID
SET @FOOLCAP           := 79011; -- Fool's Cap
SET @FOOLCAPREF        := 11921; -- Fool's Cap Ref ID
SET @SILKWEED          := 72235; -- Silkweed
SET @SILKWEEDREF       := 11922; -- Silkweed Ref ID
SET @RAINPOPPY         := 72237; -- Rain Poppy
SET @RAINPOPPYREF      := 11923; -- Rain Poppy Ref ID
SET @DESECRATEDHERB    := 89639; -- Desecrated Herb
SET @DESECRATEDHERBREF := 11924; -- Desecrated Herb Ref ID

SET @SHADOWPIGMENT     := 79251; -- Shadow Pigment
SET @MISTYPIGMENT      := 79253; -- Misty Pigment

DELETE FROM `milling_loot_template` WHERE `entry`=@GREENTEALEAF;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@GREENTEALEAF, @GREENTEALEAFREF, -@GREENTEALEAFREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@GREENTEALEAFREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@GREENTEALEAFREF, @SHADOWPIGMENT, 100, 2, 3),
(@GREENTEALEAFREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@SNOWLILY;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@SNOWLILY, @SNOWLILYREF, -@SNOWLILYREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@SNOWLILYREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@SNOWLILYREF, @SHADOWPIGMENT, 100, 2, 3),
(@SNOWLILYREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@FOOLCAP;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@FOOLCAP, @FOOLCAPREF, -@FOOLCAPREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@FOOLCAPREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@FOOLCAPREF, @SHADOWPIGMENT, 100, 2, 3),
(@FOOLCAPREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@SILKWEED;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@SILKWEED, @SILKWEEDREF, -@SILKWEEDREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@SILKWEEDREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@SILKWEEDREF, @SHADOWPIGMENT, 100, 2, 3),
(@SILKWEEDREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@RAINPOPPY;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@RAINPOPPY, @RAINPOPPYREF, -@RAINPOPPYREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@RAINPOPPYREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@RAINPOPPYREF, @SHADOWPIGMENT, 100, 2, 3),
(@RAINPOPPYREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@DESECRATEDHERB;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@DESECRATEDHERB, @DESECRATEDHERBREF, -@DESECRATEDHERBREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@DESECRATEDHERBREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@DESECRATEDHERBREF, @SHADOWPIGMENT, 100, 2, 3),
(@DESECRATEDHERBREF, @MISTYPIGMENT, 25, 1, 3);
