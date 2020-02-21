DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=170 AND `SourceEntry`=4755;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=170 AND `SourceEntry`=4757;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(26, 170, 4755, 0, 1, 8, 0, 14094, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4755 if q14094 rewarded'),
(26, 170, 4755, 0, 2, 28, 0, 14094, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4755 if q14094 complete'),
(26, 170, 4757, 0, 1, 8, 0, 14094, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4757 if q14094 rewarded'),
(26, 170, 4757, 0, 2, 28, 0, 14094, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4757 if q14094 complete');

DELETE FROM `phase_area` WHERE `AreaId`=4755 AND `PhaseId`=170;
DELETE FROM `phase_area` WHERE `AreaId`=4757 AND `PhaseId`=170;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(4755, 170, 'Gilneas Phase 170 - After Quest: 14094'),
(4757, 170, 'Gilneas Phase 170 - After Quest: 14094');
