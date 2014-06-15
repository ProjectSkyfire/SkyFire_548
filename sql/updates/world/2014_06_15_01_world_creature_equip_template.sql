DELETE FROM creature_equip_template WHERE entry IN (67014, 64044, 68981, 67013);
INSERT INTO creature_equip_template (entry, itemEntry1, itemEntry2, itemEntry3) VALUES
(67014, 71739, 71741, 0), -- Warpweaver Shafiee <Transmogrifier>
(64044, 71732, 0, 0), -- Thaumaturge Anjool <Arcane Reforger>
(68981, 29417, 71741, 0), -- Voidbinder Shadzor <Item Upgrade>
(67013, 0, 71736, 0); -- Vaultkeeper Meelad <Void Storage>