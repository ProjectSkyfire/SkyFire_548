-- Keep Stair of Destiny flight masters usable outside the intro delivery quest state.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 15
  AND `SourceGroup` = 7938
  AND `SourceEntry` = 0
  AND `ConditionTypeOrReference` = 8
  AND `ConditionValue1` = 10289;

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 15
  AND `SourceGroup` = 7939
  AND `SourceEntry` = 0
  AND `ConditionTypeOrReference` = 8
  AND `ConditionValue1` = 10140;
