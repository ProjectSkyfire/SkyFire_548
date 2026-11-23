-- To Win a War, You Gotta Become War (26651): Plant Seaforium (spell 82585, from the Bravo Company Field Kit) has to be
-- used on the Munitions Dump (43589) and on the Blackrock Tower (43590). The spell targets the nearest creature with a
-- spell target condition, which was missing, so it could not find the two markers and the charges could not be planted.
-- The credit rows of the two markers are already there. Condition as in Legends of Azeroth 5.4.8.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 82585;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 82585, 0, 0, 31, 0, 3, 43589, 0, 0, 0, 0, '', 'Plant Seaforium - Target is Munitions Dump'),
(13, 1, 82585, 0, 1, 31, 0, 3, 43590, 0, 0, 0, 0, '', 'Plant Seaforium - Target is Blackrock Tower');
