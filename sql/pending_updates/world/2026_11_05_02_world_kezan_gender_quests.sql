-- Life of the Party and Off to the Bank have a version for each gender, and the Kezan Partygoers, Candy Cane and Chip
-- Endale offered both versions to everyone.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` = 0 AND `SourceEntry` IN (14113, 14153, 26711, 26712) AND `ConditionTypeOrReference` = 20;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19, 0, 14113, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, '', 'Life of the Party - male only'),
(19, 0, 14153, 0, 0, 20, 0, 1, 0, 0, 0, 0, 0, '', 'Life of the Party - female only'),
(19, 0, 26711, 0, 0, 20, 0, 1, 0, 0, 0, 0, 0, '', 'Off to the Bank - female only'),
(19, 0, 26712, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, '', 'Off to the Bank - male only');
