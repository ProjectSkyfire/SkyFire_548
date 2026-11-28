-- Keep the Oracle Glade history narration from starting a second instance.
-- The sequence lasts 51 seconds, followed by five seconds of despawn cleanup.
-- Check eligibility before starting the cooldown so unrelated visitors do not delay it.
UPDATE `smart_scripts`
SET `event_param3` = 65000, `event_param4` = 65000
WHERE `entryorguid` = -140238 AND `source_type` = 0 AND `id` = 2 AND `event_type` = 10;

DELETE FROM `conditions`
WHERE (`SourceTypeOrReferenceId` = 22 AND `SourceEntry` = -140238 AND `SourceId` = 0 AND `SourceGroup` = 3)
   OR (`SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 65680 AND `ConditionTypeOrReference` = 2 AND `ConditionValue1` = 18151);
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,3,-140238,0,0,9,0,7383,0,0,0,0,0,'','Oracle history - invoker has the active quest'),
(22,3,-140238,0,0,2,0,18151,1,0,1,0,0,'','Oracle history - invoker has not filled the Amethyst Phial'),
(22,3,-140238,0,0,29,1,34584,100,0,1,0,0,'','Oracle history - no shade near the moonwell controller'),
(17,0,65680,0,0,2,1,18151,1,0,1,0,0,'','Oracle history - target has not filled the Amethyst Phial');
