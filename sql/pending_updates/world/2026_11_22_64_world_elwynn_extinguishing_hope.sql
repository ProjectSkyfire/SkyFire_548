-- Extinguishing Hope (26391) cannot be finished. Spray Water (80208, fired by Milly's Fire Extinguisher) selects its
-- targets by entry in a cone, but no condition names the Northshire Vineyards Fire Trigger (42940), so the fires are
-- never hit and never give the kill credit. The condition is the same in the Cataclysm Preservation Project 4.3.4 and
-- Legends of Azeroth 5.4.8.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 80208;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 80208, 0, 0, 31, 0, 3, 42940, 0, 0, 0, 0, '', 'Spray Water - Northshire Vineyards Fire Trigger');
