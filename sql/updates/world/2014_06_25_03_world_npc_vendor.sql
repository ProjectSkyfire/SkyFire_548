-- Gentle Dari
DELETE FROM npc_vendor WHERE entry=64062;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(64062, 1, 44693, 0, 0, 0, 1), -- Wound Dressing
(64062, 2, 44694, 0, 0, 0, 1); -- Antiseptic-Soaked Dressing

-- Stephen Wong
DELETE FROM npc_vendor WHERE entry=64126;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(64126, 1, 74659, 0, 0, 0, 1), -- Farm Chicken
(64126, 2, 74660, 0, 0, 0, 1), -- Pandaren Peach
(64126, 3, 74832, 0, 0, 0, 1), -- Barley
(64126, 4, 74845, 0, 0, 0, 1), -- Ginseng
(64126, 5, 74851, 0, 0, 0, 1), -- Rice
(64126, 6, 74852, 0, 0, 0, 1), -- Yak Milk
(64126, 7, 74854, 0, 0, 0, 1), -- Instant Noodles
(64126, 8, 85583, 0, 0, 0, 1), -- Needle Mushrooms
(64126, 9, 85584, 0, 0, 0, 1), -- Silkworm Pupa
(64126, 10, 85585, 0, 0, 0, 1), -- Red Beans
(64126, 11, 21219, 0, 0, 0, 1), -- Recipe: Sagefish Delight
(64126, 12, 21099, 0, 0, 0, 1), -- Recipe: Smoked Sagefish
(64126, 13, 2678, 0, 0, 0, 1), -- Mild Spices
(64126, 14, 30817, 0, 0, 0, 1), -- Simple Flour
(64126, 15, 159, 0, 0, 0, 1); -- Refreshing Spring Water

-- Jinho the Wind Breaker
DELETE FROM npc_vendor WHERE entry=66998;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(66998, 1, 81414, 0, 0, 0, 1), -- Pearl Milk Tea
(66998, 2, 81415, 0, 0, 0, 1), -- Pandaren Plum Wine
(66998, 3, 74636, 0, 0, 0, 1), -- Golden Carp Consomme
(66998, 4, 81406, 0, 0, 0, 1), -- Roasted Barley Tea
(66998, 5, 81407, 0, 0, 0, 1); -- Four Wind Soju

-- Reeler Uko <Fishing Supplies>
DELETE FROM npc_vendor WHERE entry=64585;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost, type) VALUES
(64585, 1, 6532, 0, 0, 0, 1), -- Bright Baubles
(64585, 2, 6533, 0, 0, 0, 1), -- Aquadynamic Fish Attractor
(64585, 3, 6530, 0, 0, 0, 1), -- Nightcrawlers
(64585, 4, 6365, 1, 7200, 0, 1), -- Strong Fishing Pole
(64585, 5, 6529, 0, 0, 0, 1), -- Shiny Bauble
(64585, 6, 6256, 0, 0, 0, 1); -- Fishing Pole