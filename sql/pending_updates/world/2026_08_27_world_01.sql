-- Gnarlpine Necklace
SET @NPC := 7234; -- Ferocitas the Dream Eater
SET @ITEM := 8049; -- Gnarlpine Necklace
SET @QOID := 256737; -- Tallonkai's Jewel quest objective

DELETE FROM `creature_loot_template` WHERE `entry`=@NPC AND `item`=@ITEM;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@NPC, @ITEM, 100, 1, 0, 1, 1);

DELETE FROM `conditions` WHERE `SourceGroup`=@NPC AND `SourceEntry`=@ITEM;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(1, @NPC, @ITEM, 0, 0, 48, 0, @QOID, 0, 0, 0, 0, 0, '', 'Gnarlpine Necklace drops if Tallonkai\'s Jewel objective incomplete');
