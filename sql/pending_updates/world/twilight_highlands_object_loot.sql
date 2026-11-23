-- Twilight Highlands: nine chests, herbs and fishing schools whose loot never dropped. The templates have no loot id (`data1` 0)
-- although their loot rows exist: the Sacred Ale Tap (206203, the quest item of "Anything But Water" 27647), the two Twilight Caravan
-- Chests (206674 and 206675, the Twilight's Hammer Gatestone of "Precious Goods" 28090 and 28104), the Twilight Jasmine herb (202751,
-- 18 spawns), the Sturdy Treasure Chest (207495, loot 207484) and the fishing schools Highland Guppy (202777), Algaefin Rockfish
-- (202781), Deepsea Sagefish (208311) and the Pool of Fire (207734). The 5.4.8 data points each of them at its loot rows. Safe to run
-- twice. Based on Legends of Azeroth 5.4.8.

UPDATE `gameobject_template` SET `data1` = 206203 WHERE `entry` = 206203;
UPDATE `gameobject_template` SET `data1` = 206674 WHERE `entry` = 206674;
UPDATE `gameobject_template` SET `data1` = 206675 WHERE `entry` = 206675;
UPDATE `gameobject_template` SET `data1` = 202751 WHERE `entry` = 202751;
UPDATE `gameobject_template` SET `data1` = 207484 WHERE `entry` = 207495;
UPDATE `gameobject_template` SET `data1` = 202777 WHERE `entry` = 202777;
UPDATE `gameobject_template` SET `data1` = 202781 WHERE `entry` = 202781;
UPDATE `gameobject_template` SET `data1` = 208311 WHERE `entry` = 208311;
UPDATE `gameobject_template` SET `data1` = 207734 WHERE `entry` = 207734;
