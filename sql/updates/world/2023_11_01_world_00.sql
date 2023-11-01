-- Defias Gunpowder
DELETE FROM `gameobject_loot_template` WHERE `entry`=2882;
INSERT INTO `gameobject_loot_template` (`entry`, `item`) VALUES
(2882, 17155);

-- Rattan Switch
DELETE FROM `gameobject_loot_template` WHERE `entry`=40371;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(40371, 72926, -100);

-- Vault of Forbidden Treasures
UPDATE `gameobject_template` SET `name`='Vault of Forbidden Treasures' WHERE `entry`=233029;

-- Ancient Dynastic Tablet
DELETE FROM `gameobject_loot_template` WHERE `entry`=41387;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(41387, 79713, -100);

-- Mutilated Remains
DELETE FROM `gameobject_loot_template` WHERE `entry`=27780;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(27780, 49945, -100);

-- Stolen Watermelon
DELETE FROM `gameobject_loot_template` WHERE `entry`=40518;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(40518, 76298, -100);

-- Gurthani Tablet
DELETE FROM `gameobject_loot_template` WHERE `entry`=42818;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(42818, 85634, -100);
