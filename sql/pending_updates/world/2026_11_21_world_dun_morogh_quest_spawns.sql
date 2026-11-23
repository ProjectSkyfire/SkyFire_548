-- Dun Morogh: "Entombed in Ice" (25978), "Dark Iron Scheming" (25997) and "Protecting the Herd" (314). The Icy Tombs that
-- enclose the Frozen Mountaineers had no spawn, were friendly and had no script, Captain Beld (the Dark Iron Attack
-- Plans) and Rudra Amberstill were not spawned. Based on Legends of Azeroth 5.4.8; the positions match the spawn maps on
-- Wowhead MoP Classic.

DELETE FROM `creature` WHERE `guid` BETWEEN 9310000 AND 9310022;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310000, 41768, 0, 1, 0, 0, 0, 0, -5775.43, -1991.7, 399.854, 3.6827, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310001, 41768, 0, 1, 0, 0, 0, 0, -5715.08, -1960.67, 395.539, 5.6374, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310002, 41768, 0, 1, 0, 0, 0, 0, -5744.9, -1994.26, 395.539, 0.2094, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310003, 41768, 0, 1, 0, 0, 0, 0, -5659.23, -1885.82, 395.539, 4.1888, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310004, 41768, 0, 1, 0, 0, 0, 0, -5656.17, -1934.24, 395.278, 1.7279, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310005, 41768, 0, 1, 0, 0, 0, 0, -5687.05, -1927.1, 395.539, 5.1313, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310006, 41768, 0, 1, 0, 0, 0, 0, -5664.72, -1957.09, 395.337, 1.5184, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310007, 41768, 0, 1, 0, 0, 0, 0, -5654.29, -1995.44, 395.26, 2.7053, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310008, 41768, 0, 1, 0, 0, 0, 0, -5655.28, -2087.21, 399.725, 1.3788, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310009, 41768, 0, 1, 0, 0, 0, 0, -5736.01, -2054.17, 395.539, 1.0996, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310010, 41768, 0, 1, 0, 0, 0, 0, -5700.1, -1999.36, 395.539, 2.2166, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310011, 41768, 0, 1, 0, 0, 0, 0, -5598.48, -2018.34, 395.392, 5.2709, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310012, 41768, 0, 1, 0, 0, 0, 0, -5666.14, -2033.83, 395.454, 4.4331, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310013, 41768, 0, 1, 0, 0, 0, 0, -5618.07, -2052.23, 395.539, 0.384, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310014, 41768, 0, 1, 0, 0, 0, 0, -5636.21, -2033.33, 395.365, 4.8346, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310015, 41768, 0, 1, 0, 0, 0, 0, -5567.8, -1975.57, 395.539, 2.4435, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310016, 41768, 0, 1, 0, 0, 0, 0, -5604, -1969.36, 395.292, 5.3756, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310017, 41768, 0, 1, 0, 0, 0, 0, -5563.93, -2025.25, 395.539, 2.234, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310018, 41768, 0, 1, 0, 0, 0, 0, -5627.28, -1906.04, 395.539, 3.8223, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310019, 41768, 0, 1, 0, 0, 0, 0, -5614.44, -1932.03, 395.374, 5.969, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(9310020, 6124, 0, 1, 0, 0, 0, 0, -5927.52, -2033.28, 401.582, 1.683, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(9310021, 6124, 0, 1, 0, 0, 0, 0, -5920.45, -2034.19, 400.833, 3.2987, 300, 3, 0, 0, 0, 1, 0, 0, 0),
(9310022, 1265, 0, 1, 0, 0, 0, 0, -5513.79, -1304.71, 404.609, 3.5081, 300, 0, 0, 0, 0, 0, 0, 0, 0);

-- The tomb breaks on death and frees its mountaineer, who thanks the player, runs off and despawns.
UPDATE `creature_template` SET `faction_A`=14, `faction_H`=14 WHERE `entry`=41768;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (41763, 41768);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41763, 0, 0, 1, 38, 0, 100, 1, 0, 1, 0, 0, 0, 28, 77910, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Data Set - Remove Aura'),
(41763, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Data Set - Say Line 0'),
(41763, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 5, 20, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Data Set - Play Emote Beg'),
(41763, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 1, 2500, 2500, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Data Set - Start Timed Event 1'),
(41763, 0, 4, 5, 59, 0, 100, 0, 1, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Timed Event 1 - Set Run'),
(41763, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 114, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 15, 0, 0, 'Frozen Mountaineer - On Timed Event 1 - Move Forward'),
(41763, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 4000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Timed Event 1 - Despawn'),
(41768, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Icy Tomb - On Respawn - Passive'),
(41768, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 41763, 3, 0, 0, 0, 0, 0, 'Icy Tomb - On Death - Set Data On Mountaineer');

DELETE FROM `creature_loot_template` WHERE `entry`=6124 AND `item`=56264;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(6124, 56264, -100, 1, 0, 1, 1);
