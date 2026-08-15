-- DBErrors spell source alignment for quest 1149 (Test of Faith).
-- Spell 6716 is absent from current spell data; the loader already clears this stale value at runtime.
UPDATE `quest_template`
SET `SourceSpellId` = 0
WHERE `Id` = 1149
  AND `SourceSpellId` = 6716;
