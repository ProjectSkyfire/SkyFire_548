-- DBErrors reward spell alignment for Anglers fishing daily quests.
-- Spell 125991 is absent from current spell data; the affected rows already carry
-- their fishing skill, faction, currency, and item rewards through explicit fields.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `RewardSpellCast` = 125991
  AND `RewardSpell` = 0
  AND `ZoneOrSort` = -101
  AND `RewardSkillId` = 356;
