-- These quest spells need a spell focus object near their target: Summon Aquementas (13978, focus 423); Quest Suicide (14050, focus 503).
-- 1 object(s) had no usable template and 2 of their spawns were missing, so the casts failed with a requirement error.
-- Based on Legends of Azeroth 5.4.8.

DELETE FROM `gameobject_template` WHERE `entry` IN (164799);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(164799, 8, 2770, 'Videre Elixir Spell Focus (DND)', 1, 503, 20, 1);

DELETE FROM `gameobject` WHERE `guid` BETWEEN 1061244 AND 1061245;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061244, 148507, 1, 1, 0, 0, -8172.58, -5077.78, 15.6239, 5.7652, 0, 0, 0.256107, -0.966648, 120, 0, 1),
(1061245, 164799, 1, 1, 0, 0, -7199.91, -3946.1, 9.64, 0, 0, 0, 0, 1, 300, 0, 1);
