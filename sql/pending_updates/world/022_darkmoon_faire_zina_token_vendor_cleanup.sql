-- Darkmoon Faire Zina Sharpworth token vendor cleanup.

SET @ZINA_SHARPWORTH := 55266;
SET @DARKMOON_GAME_TOKEN := 71083;
SET @POUCH_O_TOKENS := 78897;
SET @SACK_O_TOKENS := 78898;

START TRANSACTION;

DELETE FROM `npc_vendor`
WHERE `entry` = @ZINA_SHARPWORTH;

INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(@ZINA_SHARPWORTH, 0, @POUCH_O_TOKENS, 0, 0, 0, 1),
(@ZINA_SHARPWORTH, 1, @SACK_O_TOKENS, 0, 0, 0, 1);

DELETE FROM `item_loot_template`
WHERE `entry` IN (@POUCH_O_TOKENS, @SACK_O_TOKENS);

INSERT INTO `item_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@POUCH_O_TOKENS, @DARKMOON_GAME_TOKEN, 100, 1, 0, 5, 5),
(@SACK_O_TOKENS, @DARKMOON_GAME_TOKEN, 100, 1, 0, 20, 20);

COMMIT;
