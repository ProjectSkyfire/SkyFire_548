-- Remove the legacy unconditional Tarindrella summon rule in Shadowthread Cave.
-- Preserve the quest-gated rule for The Woodland Protector through Signs of Things to Come.
DELETE FROM `spell_area`
WHERE `spell` = 92237 AND `area` = 257
  AND `quest_start` = 0 AND `quest_end` = 0 AND `aura_spell` = 0
  AND `racemask` = 0 AND `gender` = 2 AND `autocast` = 1
  AND `quest_start_status` = 64 AND `quest_end_status` = 11;
