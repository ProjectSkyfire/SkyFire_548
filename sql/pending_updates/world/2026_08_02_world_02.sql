-- Tuning the Gnomecorder
UPDATE `quest_template` SET `OfferRewardText`='<The gnomecorder crackles and pops.>$B$BCan you hear me, $n? Is this thing on? Ah, yes, I see you there now.$B$BLet\'s get to work!' WHERE `Id`=26512;

-- Redridge Mystic
DELETE FROM `creature_loot_template` WHERE `entry`=430 AND `item`=58897;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(430, 58897, -80);

-- Redridge Alpha
DELETE FROM `creature_loot_template` WHERE `entry`=445 AND `item`=58897;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(445, 58897, -80);

-- Redridge Basher
DELETE FROM `creature_loot_template` WHERE `entry`=446 AND `item`=58897;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(446, 58897, -80);

-- Redridge Drudger
DELETE FROM `creature_loot_template` WHERE `entry`=580 AND `item`=58897;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(580, 58897, -80);

-- Canyon Romp
UPDATE `quest_template` SET `RequestItemsText`='Did you recover those collars?' WHERE `Id`=26514;
UPDATE `quest_template` SET `OfferRewardText`='<The gnomecorder crackles and pops.>$B$BGood work, $n. I\'ve just been handed some important information. It would appear that our most hated enemy, the Blackrock orcs, have wised up...' WHERE `Id`=26514;
