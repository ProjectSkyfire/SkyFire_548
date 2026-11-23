-- Pushing Forward (25792): the Rune of Fire (spell 77314, Burn Constriction Totem) burns the Constriction Totems (41202)
-- of the Frostmane troll camps. The spell has a dummy effect on the nearest creature with an entry and had no spell target
-- condition, so it found no target and the totems could not be burned. Condition as in Legends of Azeroth 5.4.8.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 77314;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 77314, 0, 0, 31, 0, 3, 41202, 0, 0, 0, 0, '', 'Burn Constriction Totem - Target is Constriction Totem');
