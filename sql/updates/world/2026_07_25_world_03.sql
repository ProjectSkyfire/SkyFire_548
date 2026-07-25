-- Gato <Monk Trainer>
SET @CGUID := 100100;
DELETE FROM `creature` WHERE `guid`=@CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 63296, 1, -618.5833, -4209.587, 38.217278, 2.29400);

UPDATE `creature_template` SET `gossip_menu_id`=13895 WHERE `entry`=63296;

-- Calligraphed Parchment
DELETE FROM `creature_queststarter` WHERE `id`=3143 AND `quest`=31156;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(3143, 31156);

UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31156;
UPDATE `quest_template` SET `RequiredRaces`=2 WHERE  `Id`=31156;
UPDATE `quest_template` SET `RequestItemsText`='Ahh, you have come.' WHERE `Id`=31156;
UPDATE `quest_template` SET `OfferRewardText`='We will honor our respective peoples by learning from each other. You will become a great $c. If I have anything to say about it. And I will learn the ways of your people.' WHERE `Id`=31156;

-- Tiger Palm
UPDATE `quest_template` SET `RequiredClasses`=512 WHERE `Id`=31157;
UPDATE `quest_template` SET `RequiredRaces`=2 WHERE `Id`=31157;
UPDATE `quest_template` SET `OfferRewardText`='Good, good. Your first challenge surpassed. We will learn much from each other. I can see.' WHERE `Id`=31157;
