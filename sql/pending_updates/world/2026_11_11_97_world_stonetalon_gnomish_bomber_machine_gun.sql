-- The Gnomish Bombers over Krom'gar Fortress carry the Machine Gun aura (78298), which fires spell 78299 at the nearest
-- creature of a listed entry. No condition listed any entry, so the spell never had a target and the bombers never
-- attacked. Each entry needs its own else group, because conditions in one group must all be true.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 78299;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 78299, 0, 0, 31, 0, 3, 41550, 0, 0, 0, 0, '', 'Machine Gun - Target The Bomb TARGET'),
(13, 1, 78299, 0, 1, 31, 0, 3, 41677, 0, 0, 0, 0, '', 'Machine Gun - Target Krom''gar Grunt'),
(13, 1, 78299, 0, 2, 31, 0, 3, 41895, 0, 0, 0, 0, '', 'Machine Gun - Target Goblin Heavy Gun');
