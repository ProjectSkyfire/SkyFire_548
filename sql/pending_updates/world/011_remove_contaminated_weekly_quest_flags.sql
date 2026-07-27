-- Guard for databases that already applied the earlier weekly flag cleanup attempt.

UPDATE `quest_template`
SET `Flags` = `Flags` - 4096
WHERE `Id` IN (28617, 28733, 28744, 28747, 28748)
AND (`Flags` & 4096) <> 0;

UPDATE `quest_template`
SET `Flags` = `Flags` - 32768
WHERE `Id` IN (28617, 28733, 28744, 28747, 28748)
AND (`Flags` & 32768) <> 0;

UPDATE `quest_template`
SET `SpecialFlags` = `SpecialFlags` - 1
WHERE `Id` IN (28617, 28733, 28744, 28747, 28748)
AND (`SpecialFlags` & 1) <> 0;

UPDATE `quest_template`
SET `RewardChoiceItemId2` = 0,
    `RewardChoiceItemCount2` = 0
WHERE `Id` = 28748
AND `RewardChoiceItemId2` = 65666;
