-- Benjamin Foxworthy (46983) and the other hunter trainers using menu 11817.
-- https://www.wowhead.com/mop-classic/npc=46983/benjamin-foxworthy
-- CONDITION_CLASS uses a class mask: hunter (class 3) = 4.
-- Restrict options, leaving the greeting and any quest interactions available.
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11817, 0, 0, 0, 15, 0, 4, 0, 0, 0, 0, 0, '', 'Hunter trainer menu 11817: training requires hunter'),
(15, 11817, 1, 0, 0, 15, 0, 4, 0, 0, 0, 0, 0, '', 'Hunter trainer menu 11817: talent reset requires hunter'),
(15, 11817, 2, 0, 0, 15, 0, 4, 0, 0, 0, 0, 0, '', 'Hunter trainer menu 11817: dual specialization requires hunter'),
(15, 11817, 3, 0, 0, 15, 0, 4, 0, 0, 0, 0, 0, '', 'Hunter trainer menu 11817: specialization reset requires hunter')
ON DUPLICATE KEY UPDATE `NegativeCondition` = 0;
