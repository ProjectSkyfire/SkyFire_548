-- Halls of Origination, Earthrager Ptah: the Quicksand burst (89396) hits every unit in range with its second effect
-- (55 percent of maximum health, target type area by entry) because that effect had no target condition, so it killed
-- Ptah's own adds and made him reform early. The condition limits the effect to the creature it is meant for.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 2 AND `SourceEntry` = 89396;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 2, 89396, 0, 0, 31, 0, 3, 39443, 0, 0, 0, 0, '', 'Quicksand - Target Camel');
