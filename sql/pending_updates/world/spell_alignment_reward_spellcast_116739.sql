-- DBErrors reward spell alignment for the Cradle of Chi-Ji duel quest chain.
-- Spell 116739 is absent from current spell data, and these quests have no visible reward spell.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `Id` BETWEEN 30725 AND 30740
  AND `ZoneOrSort` = 6134
  AND `RewardSpellCast` = 116739
  AND `RewardSpell` = 0;
