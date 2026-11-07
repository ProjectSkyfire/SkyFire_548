-- Schnottz's Landing: make the four chapter phases exclusive, and move Adarrah out of the Uldum spawn range (this server already ran the earlier versions).
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` IN (313, 315, 324, 325) AND `SourceEntry` IN (5602, 5700);

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(26, 313, 5602, 0, 0, 1, 0, 88111, 0, 0, 1, 0, 0, '', 'Set Phase 313 in area 5602 if the player has no aura Phase Player'),
(26, 313, 5602, 0, 0, 8, 0, 27950, 0, 0, 1, 0, 0, '', 'and q27950 is not rewarded'),
(26, 313, 5700, 0, 0, 1, 0, 88111, 0, 0, 1, 0, 0, '', 'Set Phase 313 in area 5700 if the player has no aura Phase Player'),
(26, 313, 5700, 0, 0, 8, 0, 27950, 0, 0, 1, 0, 0, '', 'and q27950 is not rewarded'),
(26, 315, 5602, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'Set Phase 315 in area 5602 if q27950 is rewarded'),
(26, 315, 5602, 0, 0, 8, 0, 28002, 0, 0, 1, 0, 0, '', 'and q28002 is not rewarded'),
(26, 315, 5700, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'Set Phase 315 in area 5700 if q27950 is rewarded'),
(26, 315, 5700, 0, 0, 8, 0, 28002, 0, 0, 1, 0, 0, '', 'and q28002 is not rewarded'),
(26, 324, 5602, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'and q27950 is rewarded'),
(26, 324, 5602, 0, 0, 8, 0, 28002, 0, 0, 0, 0, 0, '', 'Set Phase 324 in area 5602 if q28002 is rewarded'),
(26, 324, 5602, 0, 0, 47, 0, 27990, 66, 0, 1, 0, 0, '', 'and q27990 is not complete or rewarded'),
(26, 324, 5700, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'and q27950 is rewarded'),
(26, 324, 5700, 0, 0, 8, 0, 28002, 0, 0, 0, 0, 0, '', 'Set Phase 324 in area 5700 if q28002 is rewarded'),
(26, 324, 5700, 0, 0, 47, 0, 27990, 66, 0, 1, 0, 0, '', 'and q27990 is not complete or rewarded'),
(26, 325, 5602, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'and q27950 is rewarded'),
(26, 325, 5602, 0, 0, 8, 0, 28002, 0, 0, 0, 0, 0, '', 'and q28002 is rewarded'),
(26, 325, 5602, 0, 0, 47, 0, 27990, 66, 0, 0, 0, 0, '', 'Set Phase 325 in area 5602 if q27990 is complete or rewarded'),
(26, 325, 5700, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'and q27950 is rewarded'),
(26, 325, 5700, 0, 0, 8, 0, 28002, 0, 0, 0, 0, 0, '', 'and q28002 is rewarded'),
(26, 325, 5700, 0, 0, 47, 0, 27990, 66, 0, 0, 0, 0, '', 'Set Phase 325 in area 5700 if q27990 is complete or rewarded');

DELETE FROM `creature` WHERE `guid` = 8311803 AND `id` = 44833;
UPDATE `creature` SET `guid` = 8311803 WHERE `guid` = 9126040 AND `id` = 44833;
