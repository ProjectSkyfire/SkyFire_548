-- Restore Connor Rivers' supplies and the five cooking-daily sugar vendors.
-- https://www.wowhead.com/mop-classic/npc=5081/connor-rivers
-- https://www.wowhead.com/mop-classic/item=57246/confectioners-sugar
-- Two sugar units per vendor; use the reference database's one-hour restock.
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`)
VALUES
(5081, 0, 159, 0, 0, 0, 1),
(5081, 0, 2678, 0, 0, 0, 1),
(5081, 0, 30817, 0, 0, 0, 1),
(5081, 0, 57246, 2, 3600, 0, 1),
(1285, 0, 57246, 2, 3600, 0, 1),
(5483, 0, 57246, 2, 3600, 0, 1),
(49893, 0, 57246, 2, 3600, 0, 1),
(43034, 0, 57246, 2, 3600, 0, 1)
ON DUPLICATE KEY UPDATE `maxcount` = VALUES(`maxcount`), `incrtime` = VALUES(`incrtime`);
