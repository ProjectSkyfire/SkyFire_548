-- Show the Stair of Destiny intro flight options while the delivery quest is active.

UPDATE `conditions`
SET `ConditionTypeOrReference` = 9,
    `Comment` = 'Show gossip option 1 if player has quest Journey to Thrallmar active'
WHERE `SourceTypeOrReferenceId` = 15
  AND `SourceGroup` = 7938
  AND `SourceEntry` = 1
  AND `ConditionTypeOrReference` = 28
  AND `ConditionValue1` = 10289;

UPDATE `conditions`
SET `ConditionTypeOrReference` = 9,
    `Comment` = 'Show gossip option 1 if player has quest Journey to Honor Hold active'
WHERE `SourceTypeOrReferenceId` = 15
  AND `SourceGroup` = 7939
  AND `SourceEntry` = 1
  AND `ConditionTypeOrReference` = 28
  AND `ConditionValue1` = 10140;
