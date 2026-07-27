-- DBErrors reward spell alignment for repeated stale quest reward casts.
-- These spell IDs are absent from current spell data and are already cleared by
-- the quest loader at runtime. The guarded updates preserve the actual visible
-- quest rewards that are stored in RewardSpell, item, faction, and currency fields.

-- Hunter taming quests keep the visible reward spell 23356.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `Id` IN (6082, 6085, 6088, 6102, 9485, 9593, 25078)
  AND `ZoneOrSort` = -261
  AND `RewardSpellCast` = 1579
  AND `RewardSpell` = 23356;

-- Hunter beast training quests keep the visible reward spell 23357.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `Id` IN (6081, 6086, 6089, 6103, 9673, 9675, 25083)
  AND `ZoneOrSort` = -261
  AND `RewardSpellCast` = 5300
  AND `RewardSpell` = 23357;

-- Level 3 class training quests already grant their racial faction reward.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `Id` IN (24964, 24965, 24966, 24967, 24968, 24969, 31147)
  AND `Level` = 3
  AND `RewardSpellCast` = 73421
  AND `RewardSpell` = 0
  AND `RewardFactionId1` = 68
  AND `RewardFactionValueId1` = 3;

-- Level 3 class training quests already grant their item and faction rewards.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `Id` IN (25139, 25141, 25143, 25145, 25147, 25149, 31157)
  AND `Level` = 3
  AND `RewardSpellCast` = 73663
  AND `RewardSpell` = 0
  AND `RewardItemId1` IN (53356, 53357, 53363, 53365, 53376, 53385, 87265)
  AND `RewardItemCount1` = 1
  AND `RewardFactionValueId1` = 3;

-- Goblin level 3 class training quests already grant Bilgewater Cartel reputation.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `Id` IN (14007, 14008, 14009, 14010, 14011, 14012, 14013)
  AND `Level` = 3
  AND `RewardSpellCast` = 90062
  AND `RewardSpell` = 0
  AND `RewardFactionId1` = 1133
  AND `RewardFactionValueId1` = 3;

-- Klaxxi daily quests already grant reputation and Elder Charm currency.
UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE `Id` IN (31049, 31061, 31062, 31117, 31120, 31203, 31204)
  AND `ZoneOrSort` = 5842
  AND `Level` = 90
  AND `MinLevel` = 90
  AND `RewardSpellCast` = 131432
  AND `RewardSpell` = 0
  AND `RewardFactionId1` = 1270
  AND `RewardFactionValueId1` = 5
  AND `RewardCurrencyId1` = 396
  AND `RewardCurrencyCount1` = 5;
