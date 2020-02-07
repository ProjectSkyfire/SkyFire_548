DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=170 AND `SourceEntry` IN (4714, 4762);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(26, 170, 4714, 0, 1, 8, 0, 14094, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4714 if q14094 rewarded'),
(26, 170, 4714, 0, 2, 28, 0, 14094, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4714 if q14094 complete'),
(26, 170, 4762, 0, 1, 8, 0, 14094, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4762 if q14094 rewarded'),
(26, 170, 4762, 0, 2, 28, 0, 14094, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4762 if q14094 complete');

DELETE FROM `phase_area` WHERE `AreaId` IN (4714, 4762) AND `PhaseId`=170;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(4714, 170, 'Gilneas Phase 170 - After Quest: 14094'),
(4762, 170, 'Gilneas Stoneward Prison - Phase 170 - After Quest: 14094');
