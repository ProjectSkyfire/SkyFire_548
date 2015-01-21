-- Instructor Zhi Spawn
DELETE FROM creature WHERE guid=1868;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(1868, 61411, 860, 1, 1, 1441.255, 3372.109, 173.3791, 4.361386, 120, 0, 0);

-- Merchant Lorvo <Training Supplies> Spawn
DELETE FROM creature WHERE guid=2570;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(2570, 54943, 860, 1, 1, 1201.16, 3507.858, 85.9193, 3.403392, 120, 0, 0);

-- Merchant Lorvo <Training Supplies> Vendor Data
DELETE FROM npc_vendor WHERE entry=54943;
INSERT INTO `npc_vendor` (`entry`, `item`) VALUES
(54943, 3770),
(54943, 4538),
(54943, 4542),
(54943, 4593),
(54943, 4606),
(54943, 90660),
(54943, 2287),
(54943, 4541),
(54943, 4592),
(54943, 4605),
(54943, 90659),
(54943, 117),
(54943, 159),
(54943, 787),
(54943, 4536),
(54943, 4540),
(54943, 4604);

-- Merchant Lorvo <Training Supplies> Quest Ender
DELETE FROM creature_questender WHERE id=54943;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(54943, 29410),
(54943, 29419),
(54943, 29424);

-- Merchant Lorvo <Training Supplies> Quest Starter
DELETE FROM creature_queststarter WHERE id=54943;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(54943, 29419),
(54943, 29424);

-- Aysa Cloudsinger <Master of Tushui> Spawn
DELETE FROM creature WHERE guid=2571;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(2571, 54567, 860, 1, 1, 1168.944, 3498.039, 85.113, 3.01942, 120, 0, 0);

-- Aysa Cloudsinger <Master of Tushui> Quest Starter
DELETE FROM creature_queststarter WHERE id=54567;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(54567, 29414);

-- Lilly Pad Template
DELETE FROM `gameobject_template` WHERE `entry`=209078;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `size`, `unkInt32`, `WDBVerified`) VALUES
(209078, 5, 10720, 'Lilly Pad', '', '', '', 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 18291);

-- Lilly Pad Spawn
DELETE FROM `gameobject` WHERE `guid` IN (154, 155, 156);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(154, 209078, 860, 1, 1, 1168.913, 3498.093, 85.00146, 0, 0, 0, 0, 1, 120, 255, 1),
(155, 209078, 860, 1, 1, 1167.099, 3496.185, 85.00146, 5.497789, 0, 0, 0, 1, 120, 255, 1),
(156, 209078, 860, 1, 1, 1166.181, 3501.701, 85.00146, 0.6806767, 0, 0, 0, 1, 120, 255, 1);

-- Amberleaf Scamp Loot
DELETE FROM creature_loot_template WHERE entry=54130;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(54130, 72071, -100);

-- Corsac Fox Loot
DELETE FROM creature_loot_template WHERE entry=53704;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(53704, 62328, 75),
(53704, 3300, 25),
(53704, 805, 0.12);

-- Corsac Fox Loot
DELETE FROM creature_loot_template WHERE entry=57797;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(57797, 62328, 75),
(57797, 3300, 25);

