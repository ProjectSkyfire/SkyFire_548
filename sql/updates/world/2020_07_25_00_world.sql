-- Gameobject Loot for Quest Materia Medica (30081)
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (41035, 41037);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(41035, 78877, -100), -- Slick Mudfish
(41037, 78881, -100); -- Chunk of Honeycomb
