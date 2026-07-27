-- DBErrors reward spell alignment for Argent Tournament daily quests.
-- Spell 64689 only exists as an old server-side spell placeholder in spell_dbc,
-- but current SpellInfo loading is DBC-backed and clears these casts at runtime.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `Id` IN (13847, 13851, 13852, 13854, 13855, 13856, 13857, 13858, 13859, 13860, 13861, 13862, 13863, 13864)
  AND `ZoneOrSort` = -241
  AND `RewardSpellCast` = 64689
  AND `RewardSpell` = 0
  AND (
      (`RewardItemId1` = 44987 AND `RewardItemCount1` = 1)
      OR (`RewardChoiceItemId1` = 46114 AND `RewardChoiceItemCount1` = 1 AND `RewardChoiceItemId2` = 45724 AND `RewardChoiceItemCount2` = 1)
  );
