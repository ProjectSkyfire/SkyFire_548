-- The core reads ConditionValue1 to 3 as uint32, but the columns were MEDIUMINT UNSIGNED (at most 16777215). Race
-- masks that include the Pandaren races (bits 24 and 25) were clamped to 16777215, so such a condition matched every
-- race below Pandaren instead of one faction.
ALTER TABLE `conditions`
  MODIFY `ConditionValue1` INT UNSIGNED NOT NULL DEFAULT 0,
  MODIFY `ConditionValue2` INT UNSIGNED NOT NULL DEFAULT 0,
  MODIFY `ConditionValue3` INT UNSIGNED NOT NULL DEFAULT 0;
