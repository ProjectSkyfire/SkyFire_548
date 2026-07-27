-- Feed Silkworm Larva
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=57403;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`) VALUES
(57403, 108554, 1);

-- Hungry Silkworm
UPDATE `creature_template` SET `npcflag`=16777216 WHERE `entry`=57403;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=57403;

DELETE FROM `smart_scripts` WHERE `entryorguid`=57403;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(57403, 0, 0, 0, 8, 0, 100, 0, 108554, 0, 0, 0, 0, 33, 57403, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hungry Silkworm - On Hit - Give kill credit to invoker');

SET @CGUID := 180373;
DELETE FROM `creature` WHERE `guid`=@CGUID-24 AND @CGUID-0;
DELETE FROM `creature` WHERE `id`=57403;
INSERT INTO `creature` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(@CGUID-0, 57403, 870, -503.7182, 275.24588, 154.25813, 0.98878),
(@CGUID-1, 57403, 870, -383.08731, 229.08865, 166.01066, 6.10009),
(@CGUID-2, 57403, 870, -388.26388, 244.77951, 167.31138, 0),
(@CGUID-3, 57403, 870, -493.31130, 268.50851, 155.40328, 1.44512),
(@CGUID-4, 57403, 870, -389.4479, 246.11285, 166.16284, 1.98967),
(@CGUID-5, 57403, 870, -489.81161, 275.48001, 155.13557, 4.03832),
(@CGUID-6, 57403, 870, -501.89062, 267.16146, 154.41186, 1.36441),
(@CGUID-7, 57403, 870, -391.96438, 233.12373, 166.01066, 1.53608),
(@CGUID-8, 57403, 870, -499.03082, 276.18182, 154.26878, 3.57853),
(@CGUID-9, 57403, 870, -388.93923, 246.46528, 166.24938, 0.66322),
(@CGUID-10, 57403, 870, -499.78881, 278.97851, 154.31732, 5.38187),
(@CGUID-11, 57403, 870, -387.30551, 237.22354, 166.01066, 0.53624),
(@CGUID-12, 57403, 870, -508.02282, 277.70529, 154.67094, 0.85453),
(@CGUID-13, 57403, 870, -500.59262, 271.47842, 154.34693, 5.61501),
(@CGUID-14, 57403, 870, -509.08334, 272.21527, 154.51141, 1.18976),
(@CGUID-15, 57403, 870, -491.27862, 272.25076, 155.61302, 5.16160),
(@CGUID-16, 57403, 870, -494.89337, 272.57861, 155.20654, 5.42433),
(@CGUID-17, 57403, 870, -495.738, 269.69836, 154.82676, 0.05813),
(@CGUID-18, 57403, 870, -504.33325, 270.65789, 154.29522, 4.26745),
(@CGUID-19, 57403, 870, -495.7272, 275.69229, 155.20046, 4.08204),
(@CGUID-20, 57403, 870, -510.54519, 276.92807, 154.42344, 5.95026),
(@CGUID-21, 57403, 870, -508.10015, 270.47174, 154.2798, 2.38867),
(@CGUID-22, 57403, 870, -443.63369, 259.46008, 165.61538, 6.23082),
(@CGUID-23, 57403, 870, -374.99584, 224.41232, 166.01066, 0.01662),
(@CGUID-24, 57403, 870, -379.08, 220.01171, 166.01066, 5.11616);

-- Silk Cocoon Bucket
SET @OGUID := 150080;
DELETE FROM `gameobject` WHERE `guid`=@OGUID-0;
DELETE FROM `gameobject` WHERE `id`=210088;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 210088, 870, -539.36114, 231.24826, 162.43194, 0, 0, 0, 0, 1);

UPDATE `gameobject` SET `state`=1 WHERE `id`=210088;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=210088;
UPDATE `gameobject_template` SET `data1`=30072 WHERE `entry`=210088;

-- Raw Silk
DELETE FROM `spell_loot_template` WHERE `entry`=108448;
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(108448, 77456, -100);

-- Mulberry Barrel
SET @OGUID := 150131;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-2 AND @OGUID-0;
DELETE FROM `gameobject` WHERE `id`=210080;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`) VALUES
(@OGUID-0, 210080, 870, -386.06423, 283.52432, 167.46884, 0, 0, 0, 0, 1),
(@OGUID-1, 210080, 870, -348.39932, 227, 167.50093, 0, 0, 0, 0, 1),
(@OGUID-2, 210080, 870, -361.91146, 265.44790, 167.66621, 1.53588, 0, 0, 0.69465, 0.71933);

UPDATE `gameobject` SET `state`=1 WHERE `id`=210080;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=210080;

-- Mulberry Leaves
DELETE FROM `gameobject_loot_template` WHERE `entry`=40884;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(40884, 77455, -100);
