DELETE FROM `spell_script_names`
WHERE `spell_id` = 102522 AND `ScriptName` = 'spell_item_huo_offering';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102522, 'spell_item_huo_offering');

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 13
  AND `SourceGroup` = 2
  AND `SourceEntry` = 102522;
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 2, 102522, 0, 0, 31, 0, 3, 54787, 0, 0, 0, 0, '', 'Huo''s Offerings targets Huo for quest 29422');
