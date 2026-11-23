-- It's Goat Time, Baby (27776): the Billy Goat Blaster (item 62397, spell 86425) gives the credit (46696) to its caster when
-- it is fired, but nothing limited its target, so it could be fired at any creature. Only Billy Goats (46393) are valid
-- targets in Legends of Azeroth 5.4.8, whose spell target condition is added.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 86425;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 86425, 0, 0, 31, 1, 3, 46393, 0, 0, 0, 0, '', 'Billy Goat Blaster - Target is a Billy Goat');
