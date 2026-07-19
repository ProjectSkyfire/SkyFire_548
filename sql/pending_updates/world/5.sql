-- Wind-Reaver's Dagger of Quick Strikes
DELETE FROM `gameobject_loot_template` WHERE `entry`=42949;
INSERT INTO `gameobject_loot_template` (`entry`, `item`) VALUES
(42949, 86519);

-- Onyx Egg
DELETE FROM `gameobject_loot_template` WHERE `entry`=43460;
INSERT INTO `gameobject_loot_template` (`entry`, `item`) VALUES
(43460, 89155);

-- Vor'thik Egg
DELETE FROM `gameobject_loot_template` WHERE `entry`=43115;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(43115, 86598, -100);
