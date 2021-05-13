-- Rigid Lapis Lazuli
DELETE FROM `spell_loot_template` WHERE `entry`=107617;
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(107617, 76502, 75, 1, 1, 1, 1),
(107617, 76570, 25, 1, 1, 1, 1);

-- Solid Lapis Lazuli
DELETE FROM `spell_loot_template` WHERE `entry`=107619;
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(107619, 76506, 75, 1, 1, 1, 1),
(107619, 76573, 25, 1, 1, 1, 1);

-- Sparkling Lapis Lazuli
DELETE FROM `spell_loot_template` WHERE `entry`=107620;
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(107620, 76505, 75, 1, 1, 1, 1),
(107620, 76572, 25, 1, 1, 1, 1);

-- Stormy Lapis Lazuli
DELETE FROM `spell_loot_template` WHERE `entry`=107621;
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(107621, 76504, 75, 1, 1, 1, 1),
(107621, 76571, 25, 1, 1, 1, 1);
