-- These quest spells need a spell focus object near their target: Summon Ekkorash (34992, focus 1413); Detonate Teleporter (38920, focus 1445).
-- 1 object(s) had no usable template and 4 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (185215);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(185215, 8, 2770, 'Gehenna Teleporter Focus', 1, 1445, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911610 AND 911613;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911610, 184143, 530, 1, 0, 0, 2760.63, 3204.53, 151.376, 3.93823, 0, 0, 0.921715, -0.387867, 600, 0, 1),
(911611, 185215, 530, 1, 0, 0, 4734.71, 3193.67, 161.48, 0.386745, 0, 0, 0.19217, 0.981362, 300, 0, 1),
(911612, 185215, 530, 1, 0, 0, 4673.28, 3126.67, 166.725, 5.42979, 0, 0, 0.413869, -0.910336, 300, 0, 1),
(911613, 185215, 530, 1, 0, 0, 4697.5, 3298.68, 178.892, 2.02823, 0, 0, 0.849013, 0.528373, 600, 0, 1);
