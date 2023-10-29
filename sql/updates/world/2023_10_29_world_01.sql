
DELETE FROM `disenchant_loot_template` WHERE `entry` IN (134, 137, 138, 139, 140, 141, 142, 143, 144, 145);
INSERT INTO `disenchant_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(134, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 364 - 377 UNCOMMON ARMOR
(134, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 364 - 377 UNCOMMON ARMOR
(137, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 378 - 390 UNCOMMON ARMOR
(137, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 378 - 390 UNCOMMON ARMOR
(138, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 391 - 403 UNCOMMON ARMOR
(138, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 391 - 403 UNCOMMON ARMOR
(139, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 404 - 416 UNCOMMON ARMOR
(139, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 404 - 416 UNCOMMON ARMOR
(140, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 417 - 900 UNCOMMON ARMOR
(140, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 417 - 900 UNCOMMON ARMOR
(141, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 364 - 377 UNCOMMON WEAPONS
(141, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 364 - 377 UNCOMMON WEAPONS
(142, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 378 - 390 UNCOMMON WEAPONS
(142, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 378 - 390 UNCOMMON WEAPONS
(143, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 391 - 403 UNCOMMON WEAPONS
(143, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 391 - 403 UNCOMMON WEAPONS
(144, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 404 - 416 UNCOMMON WEAPONS
(144, 74250, 25, 1, 1, 1, 2), -- Mysterious Essence - 404 - 416 UNCOMMON WEAPONS
(145, 74249, 75, 1, 1, 1, 6), -- Spirit Dust - 417 - 900 UNCOMMON WEAPONS
(145, 74250, 25, 1, 1, 1, 2); -- Mysterious Essence - 417 - 900 UNCOMMON WEAPONS


DELETE FROM `disenchant_loot_template` WHERE `entry` IN (129, 130, 131, 132);
INSERT INTO `disenchant_loot_template` (`entry`, `item`, `groupid`, `maxcount`) VALUES
(129, 87517, 1, 2), -- Ethereal Shard - RARE ARMOR 378 - 424
(130, 87517, 1, 2), -- Ethereal Shard - RARE ARMOR 425 - 900
(131, 87517, 1, 2), -- Ethereal Shard - RARE WEAPONS 378 - 424
(132, 87517, 1, 2); -- Ethereal Shard - RARE WEAPONS 425 - 900

DELETE FROM `disenchant_loot_template` WHERE `entry` IN (135, 136);
INSERT INTO `disenchant_loot_template` (`entry`, `item`, `groupid`) VALUES
(135, 74248, 1), -- Sha Crystal - EPIC ARMOR
(136, 74248, 1); -- Sha Crystal - EPIC WEAPONS
