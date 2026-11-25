-- Match Eastvale Logging Camp vendor stock to Wowhead MoP Classic.
-- Preserve existing ammunition and legacy items.
DELETE FROM npc_vendor
WHERE entry = 1250 AND item IN (2946,2947,3111,3131,4471,20815,39505,85663)
  AND ExtendedCost = 0 AND type = 1;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(1250,0,2946,0,0,0,1), -- Broken Balanced Throwing Dagger
(1250,0,2947,0,0,0,1), -- Broken Small Throwing Knife
(1250,0,3111,0,0,0,1), -- Broken Crude Throwing Axe
(1250,0,3131,0,0,0,1), -- Broken Weighted Throwing Axe
(1250,0,4471,0,0,0,1), -- Flint and Tinder
(1250,0,20815,0,0,0,1), -- Jeweler's Kit
(1250,0,39505,0,0,0,1), -- Virtuoso Inking Set
(1250,0,85663,0,0,0,1); -- Herbalist's Spade

-- Keep the existing restock interval; only correct the limited-stock quantity.
UPDATE npc_vendor SET maxcount = 2
WHERE entry = 1198 AND item = 11303 AND ExtendedCost = 0 AND type = 1;
