-- DBErrors reward spell alignment for quests carrying stale RewardSpellCast 81040.
-- 81040 is not present in current spell data, and affected rows have no display reward spell.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `RewardSpellCast` = 81040
  AND `RewardSpell` = 0;
