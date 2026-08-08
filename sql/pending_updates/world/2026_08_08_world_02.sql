-- Smuggled Brewfather Statue
UPDATE `gameobject_template` SET `flags`=4, `data1`=42854, `data3`=1, `data20`=1 WHERE `entry`=213516;

-- Evil Monkey Idol
UPDATE `gameobject_template` SET `flags`=4, `data1`=42854, `data3`=1, `data20`=1 WHERE `entry`=213517;

-- Smuggled Cannon
UPDATE `gameobject_template` SET `flags`=4, `data1`=42854, `data3`=1, `data20`=1 WHERE `entry`=213518;

-- Cursed Hozen Totem
UPDATE `gameobject_template` SET `flags`=4, `data1`=42854, `data3`=1, `data20`=1 WHERE `entry`=213519;

-- Priceless Mogu Artifact
UPDATE `gameobject_template` SET `flags`=4, `data1`=42854, `data3`=1, `data20`=1 WHERE `entry`=213520;

DELETE FROM `gameobject_loot_template` WHERE `entry`=42854;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(42854, 85981, -100, 1, 0, 1, 1);

-- Robbing Robbers of Robbers
UPDATE `quest_template` SET `RequestItemsText`='Saurok are seldom this bold.' WHERE `Id`=31286;
UPDATE `quest_template` SET `OfferRewardText`='Ah, our merchandise. Madam Goya will be pleased. You have the gratitude of her ladyship.' WHERE `Id`=31286;
