-- Mount Hyjal: the burnt terrain (swap map 719) shows only until Aessina's Miracle (quest 25372) is rewarded; afterwards the
-- player sees the base map with the restored Sanctuary of Malorne and its trees, and the matching world map
-- A creature never meets a quest condition, so with the condition negated creatures keep the burnt terrain as before.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 25 AND `SourceGroup` = 0 AND `SourceEntry` = 719;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(25, 0, 719, 0, 0, 8, 0, 25372, 0, 0, 1, 0, 0, '', 'Burnt Mount Hyjal terrain swap until Aessina''s Miracle is rewarded');
