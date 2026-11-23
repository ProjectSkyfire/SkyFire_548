-- For Peat's Sake (25939): the Blessed Floodlily (spell 77571, Water Blast) puts out the Marsh Fires, which are the Marsh
-- Fire Bunnies (41628) the credit script already listens to. The spell hits everything in 13 yards with a dummy effect and had
-- no spell target condition, so it was not limited to the fires. Condition as in Legends of Azeroth 5.4.8.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 77571;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 77571, 0, 0, 31, 0, 3, 41628, 0, 0, 0, 0, '', 'Water Blast - Target is Marsh Fire Bunny');
