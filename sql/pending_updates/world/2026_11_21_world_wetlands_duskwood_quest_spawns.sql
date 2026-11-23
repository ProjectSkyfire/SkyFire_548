-- Wetlands and Duskwood: quest NPCs that had no spawn. First Mate Fitzsimmons (The Third Fleet, Cursed to Roam),
-- Calamoth Ashbeard (The Battle of Thandol Span), Sven Yorgen (The Fate of Morbent Fel, The Lurking Lich, Morbent's Bane),
-- Stitches (The Embalmer's Revenge) and Stalvan Mistmantle (Mistmantle's Revenge) were not spawned. Based on Legends of
-- Azeroth 5.4.8; the positions match the spawn maps on Wowhead MoP Classic.

DELETE FROM `creature` WHERE `guid` BETWEEN 9310200 AND 9310204;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310200, 1239, 0, 1, 0, 0, 0, 0, -3794.61, -842.481, 1.1846, 6.0912, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310201, 41522, 0, 1, 0, 0, 0, 0, -2487.02, -2504.03, 80.51, 0.8207, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(9310202, 43861, 0, 1, 0, 0, 0, 0, -10761.7, 337.977, 37.8645, 5.2709, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310203, 43862, 0, 1, 0, 0, 0, 0, -10553.2, -1169.15, 27.8817, 1.7628, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(9310204, 315, 0, 1, 0, 0, 0, 0, -10371.7, -1251.92, 35.9934, 5.5327, 60, 3, 0, 0, 0, 1, 0, 0, 0);

-- Calamoth Ashbeard was a placeholder faction; Stalvan Mistmantle is level 23 in 5.4.8.
UPDATE `creature_template` SET `faction_A`=1801, `faction_H`=1801 WHERE `entry`=41522;
UPDATE `creature_template` SET `minlevel`=23, `maxlevel`=23 WHERE `entry`=315;
