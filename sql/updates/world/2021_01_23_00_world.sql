-- Recalculate drop for small pouches.
-- small blue pouch 828
-- small red pouch 805
-- small green pouch 5572
-- small brown pouch 4496
-- small black pouch 5571

UPDATE creature_loot_template SET ChanceOrQuestChance = "0.04" WHERE item IN (805, 828, 4496, 5571, 5572);