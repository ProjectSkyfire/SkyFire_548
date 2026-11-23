-- The View from Down Here (28868): the Viewpoint Equalizer (spell 93773, Equalize Viewpoint) shrinks a Frostmane Builder
-- (41251) the player has targeted, which the builder's script then credits. The spell can be cast on any target and had no
-- cast condition, so it could be used on other creatures and players. Condition as in Legends of Azeroth 5.4.8.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 93773;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 93773, 0, 0, 31, 1, 3, 41251, 0, 0, 0, 0, '', 'Equalize Viewpoint - Target is Frostmane Builder');
