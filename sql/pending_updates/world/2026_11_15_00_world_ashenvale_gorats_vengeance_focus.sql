-- Gorat's Vengeance (13621): Gorat's Imbued Blood (62772) requires spell focus 1598 (Gorat's Corpse) within 10 yards,
-- and no object with that focus existed, so the spell failed with "requires spell focus" and the Spirit of Gorat, the
-- bowshot and Captain Elendilad never appeared. Spell focus object at Gorat's corpse, where Gorat (33294) is spawned.

DELETE FROM `gameobject_template` WHERE `entry` = 194290;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`, `data3`) VALUES
(194290, 8, 327, 'Spell Focus: Gorat''s Corpse', 1, 1598, 10, 1);

DELETE FROM `gameobject` WHERE `guid` = 1061220;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1061220, 194290, 1, 1, 0, 0, 1424.91, -1999.62, 96.412, 2.11261, 0, 0, 0.870542, 0.492094, 300, 100, 1);
