-- These quest spells need a spell focus object near their target: Shadow Vault Decree (31696, focus 1571); Drop Off Captured Crusader (56684, focus 1574); Neutralize Plague (59655, focus 1585).
-- 2 object(s) had no usable template and 3 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (192011, 193580);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(192011, 8, 327, 'Thane Ufrang the Mighty''s Spell Focus', 1, 1571, 5, 1),
(193580, 8, 5932, 'Mord''rethar Plague Cauldron', 1, 1585, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 911931 AND 911933;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911931, 192011, 571, 1, 0, 0, 8426.71, 2926.41, 606.26, 1.51712, 0, 0, 0.687878, 0.725827, 300, 0, 1),
(911932, 192523, 571, 1, 0, 0, 6219.46, 1.26644, 410.165, 1.11701, 0, 0, 0, 1, 300, 100, 1),
(911933, 193580, 571, 1, 0, 0, 6753.36, 1584.21, 396.044, 0.267029, 0, 0, 0.133118, 0.9911, 300, 0, 1);
