DELETE FROM `spell_loot_template` WHERE `entry` IN
(53831, 53832, 53843, 53844, 53845, 53852, 53856);
INSERT INTO `spell_loot_template` (`entry`, `item`) VALUES
(53831, 39900), -- Bold Bloodstone
(53832, 39905), -- Delicate Bloodstone
(53843, 39907), -- Subtle Sun Crystal
(53844, 39908), -- Flashing Bloodstone
(53845, 39909), -- Smooth Sun Crystal
(53852, 39912), -- Brilliant Bloodstone
(53856, 39918); -- Quick Sun Crystal
