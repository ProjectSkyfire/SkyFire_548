UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=54135;
DELETE FROM `smart_scripts` WHERE `entryorguid`=54135;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54135, 0, 0, 0, 19, 0, 100, 0, 29421, 0, 0, 0, 0, 44, 169, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - On quest accept - unset phaseid 169'),
(54135, 0, 1, 0, 19, 0, 100, 0, 29421, 0, 0, 0, 0, 44, 631, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - On quest accept - set phaseid 631'),
(54135, 0, 2, 0, 19, 0, 100, 0, 29422, 0, 0, 0, 0, 44, 631, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - On quest accept - unset phaseid 631'),
(54135, 0, 3, 0, 19, 0, 100, 0, 29422, 0, 0, 0, 0, 44, 632, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - On quest accept - set phaseid 632');

UPDATE `creature` SET `phaseId`=631 WHERE `guid`=224847;

UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88310;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88311;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88306;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88308;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88314;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88309;
UPDATE `gameobject` SET `phaseId`=631 WHERE `guid`=88312;

UPDATE `creature_template` SET `Health_mod`=36 WHERE `entry`=54734;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=54734;
DELETE FROM `smart_scripts` WHERE `entryorguid`=54734;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54734, 0, 0, 0, 0, 0, 0, 0, 4000, 4500, 10000, 10500, 0, 11, 108958, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - IC - Feet of Fury'),
(54734, 0, 1, 0, 0, 0, 0, 0, 5000, 6000, 11000, 12000, 0, 11, 108944, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - IC - Flying Shadow Kick'),
(54734, 0, 2, 0, 2, 0, 100, 0, 1, 20, 0, 0, 0, 33, 54734, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - at 20% health - give credit'),
(54734, 0, 3, 0, 2, 0, 100, 0, 1, 20, 0, 0, 0, 44, 631, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - at 20% health - unset phaseid 631'),
(54734, 0, 4, 0, 2, 0, 100, 0, 1, 20, 0, 0, 0, 44, 632, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - at 20% health - set phaseid 632'),
(54734, 0, 5, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Master Li Fei - on link - despawn');

DELETE FROM `creature` WHERE `guid`=257979;

SET @CGUID := 257982;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID-2 AND @CGUID-0;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID-0, 54135, 860, 1, 632, 0, 0, 0, 1331.78, 3942.8, 110.606, 6.00393, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-1, 57779, 860, 1, 632, 0, 0, 0, 1257.89, 3925.59, 127.856, 0.506145, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(@CGUID-2, 54787, 860, 1, 632, 0, 0, 0, 1259.8, 3926.66, 128.28, 0.453786, 120, 0, 0, 0, 0, 0, 0, 0, 0);

SET @OGUID := 100016;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-1 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID-0, 209369, 860, 1, 631, 0, 1333.55, 3945.18, 110.544, 0.488691, 0, 0, 0.241921, 0.970296, 120, 255, 1),
(@OGUID-1, 209369, 860, 1, 632, 0, 1333.55, 3945.18, 110.544, 0.488691, 0, 0, 0.241921, 0.970296, 120, 255, 1);

UPDATE `gameobject` SET `phaseId`=632 WHERE `guid`=88305;
UPDATE `gameobject` SET `phaseId`=632 WHERE `guid`=88313;
UPDATE `gameobject` SET `phaseId`=632 WHERE `guid`=88307;
