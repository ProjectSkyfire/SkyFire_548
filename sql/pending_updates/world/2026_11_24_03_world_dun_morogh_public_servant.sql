-- The Public Servant (433): the Runes of Return (spell 77819, Give Rune of Return) free a Trapped Miner (41671), which its
-- script credits. The spell can be cast on any target and had no cast condition, so the rune was spent on other creatures
-- and players. Condition as in Legends of Azeroth 5.4.8.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 77819;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 77819, 0, 0, 31, 1, 3, 41671, 0, 0, 0, 0, '', 'Give Rune of Return - Target is Trapped Miner');
