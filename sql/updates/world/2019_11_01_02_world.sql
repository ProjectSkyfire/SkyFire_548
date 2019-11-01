DELETE FROM phase_area WHERE AreaId=4756 AND PhaseId=170;
INSERT INTO phase_area (AreaId, PhaseId, Comment) VALUES (4756, 170, 'Gilneas Phase 170 - After Quest: Lockdown!');

DELETE FROM conditions WHERE SourceTypeOrReferenceId=26 AND SourceGroup=170 AND ConditionValue1=14078;
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment) VALUES
(26, 170, 4756, 0, 1, 8, 0, 14078, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4756 if q14078 rewarded');
(26, 170, 4756, 0, 2, 28, 0, 14078, 0, 0, 0, 0, 0, '', 'Set Phase 170 in area 4756 if q14078 complete');

SET @CGUID := 237139;
DELETE FROM creature WHERE guid=@CGUID-0;
INSERT INTO creature (guid, id, map, phaseId, phasegroup, position_x, position_y, position_z, orientation, spawntimesecs, spawndist) VALUES
(@CGUID-0, 34863, 654, 170, 0, -1405.49, 1447.42, 35.6393, 0, 120, 0);
