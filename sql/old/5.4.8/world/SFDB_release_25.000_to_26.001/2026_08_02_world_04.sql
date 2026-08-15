-- Tarantula
DELETE FROM `creature_loot_template` WHERE `entry`=442 AND `item`=58891;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(442, 58891, -80); -- Tarantula Eyes

-- Great Goretusk
DELETE FROM `creature_loot_template` WHERE `entry`=547 AND `item`=58893;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(547, 58893, -80); -- Goretusk Kidney

-- Dire Condor
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry`=428;

DELETE FROM `creature_loot_template` WHERE `entry`=428 AND `item`=58892;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(428, 58892, -80); -- Condor Giblets

-- Franks and Beans
UPDATE `quest_template` SET `RequestItemsText`='Have you gotten my ingredients?' WHERE `Id`=26506;
UPDATE `quest_template` SET `OfferRewardText`='PERFECT! I\'ll put these in the pot right away. Dinner should be ready in a few hours.$B$BThank you, darling!' WHERE `Id`=26506;

-- Lake Everstill Clean Up
UPDATE `quest_template` SET `OfferRewardText`='That ought to teach those murlocs a lesson. Hopefully the next time they decide to raid our town they\'ll think twice.$B$BWe both know that won\'t happen.' WHERE `Id`=26511;
