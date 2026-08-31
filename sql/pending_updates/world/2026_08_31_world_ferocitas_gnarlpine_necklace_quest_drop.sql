-- Restore Gnarlpine Necklace as a quest drop for Ferocitas the Dream Eater.
-- With HasQuestForItem() checking RequiredSourceItemId even when the quest has
-- item objectives, ChanceOrQuestChance=-100 works again. Remove the temporary
-- 100% drop + objective-progress condition workaround.

SET @NPC := 7234;  -- Ferocitas the Dream Eater
SET @ITEM := 8049; -- Gnarlpine Necklace

DELETE FROM `creature_loot_template` WHERE `entry`=@NPC AND `item`=@ITEM;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@NPC, @ITEM, -100, 1, 0, 1, 1);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=@NPC AND `SourceEntry`=@ITEM;
