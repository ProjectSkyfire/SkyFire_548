-- Badlands: two quest objects had no spawn. The Trove of the Watchers (206374) ends "The Titans' Trove" (27912, 27913) and
-- starts "Return to Blam" (27794, 27888); Rhea's Final Note (206504) ends "Devastation" (27930). Without them the
-- Ancient Protectors chain and the end of the Rheastrasza chain could not be turned in.
-- Positions are as in Legends of Azeroth 5.4.8.

DELETE FROM `gameobject` WHERE `guid` IN (1068268, 1068269);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1068268, 206374, 0, 1, 0, 0, -6944.58, -3445.47, 197.649, 6.23083, 0, 0, -0.0261765, 0.999657, 300, 255, 1),
(1068269, 206504, 0, 1, 0, 0, -6473.92, -2480.95, 306.822, 1.74533, 0, 0, 0.766044, 0.642789, 300, 255, 1);
