-- Stormwind Royal Guard (42218): nine-member road patrol to the Eastvale bridge.
-- https://www.wowhead.com/mop-classic/npc=42218/stormwind-royal-guard
-- Requires npc_stormwind_royal_patrol for the coordinated arrival/respawn cycle.
SET @LEADER := 167605;
SET @PATH := @LEADER * 10;

UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_stormwind_royal_patrol' WHERE `entry` = 42218;

-- Remove the apparent duplicate road squad: same entry, phase, equipment, with no
-- quest, event, pool, formation or script references in the base database.
-- Keep the six Northshire guards: their role is not established by this patrol.
DELETE FROM `creature` WHERE `guid` BETWEEN 167468 AND 167476 AND `id` = 42218 AND `map` = 0;

UPDATE `creature` SET `position_x`=-9125.449000, `position_y`=389.961900, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=2, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167605 AND `id`=42218 AND `map`=0;
UPDATE `creature` SET `position_x`=-9127.197501, `position_y`=392.399675, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=0, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167606 AND `id`=42218 AND `map`=0;
UPDATE `creature` SET `position_x`=-9123.700499, `position_y`=387.524125, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=0, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167607 AND `id`=42218 AND `map`=0;
UPDATE `creature` SET `position_x`=-9124.759726, `position_y`=394.148175, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=0, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167608 AND `id`=42218 AND `map`=0;
UPDATE `creature` SET `position_x`=-9123.011225, `position_y`=391.710401, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=0, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167609 AND `id`=42218 AND `map`=0;
UPDATE `creature` SET `position_x`=-9121.262725, `position_y`=389.272626, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=0, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167610 AND `id`=42218 AND `map`=0;
UPDATE `creature` SET `position_x`=-9122.321952, `position_y`=395.896676, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=0, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167611 AND `id`=42218 AND `map`=0;
UPDATE `creature` SET `position_x`=-9120.573451, `position_y`=393.458902, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=0, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167612 AND `id`=42218 AND `map`=0;
UPDATE `creature` SET `position_x`=-9118.824950, `position_y`=391.021127, `position_z`=91.57055, `orientation`=3.763804, `MovementType`=0, `spawndist`=0, `currentwaypoint`=0, `spawntimesecs`=60 WHERE `guid`=167613 AND `id`=42218 AND `map`=0;

INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
SELECT c.`guid`, IF(c.`guid` = @LEADER, @PATH, 0), COALESCE(a.`mount`, 0), COALESCE(a.`bytes1`, 0), COALESCE(a.`bytes2`, 257), COALESCE(a.`emote`, 0), a.`auras`
FROM `creature` c LEFT JOIN `creature_template_addon` a ON a.`entry`=c.`id`
WHERE c.`guid` BETWEEN 167605 AND 167613 AND c.`id`=42218 AND c.`map`=0
ON DUPLICATE KEY UPDATE `path_id`=VALUES(`path_id`);

DELETE FROM `creature_formations` WHERE `leaderGUID`=@LEADER OR `memberGUID` BETWEEN 167605 AND 167613;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`, `point_1`, `point_2`) VALUES
(@LEADER,167605,0.000000,0.000000,2,0,0),
(@LEADER,167606,3.000000,90.000000,2,0,0),
(@LEADER,167607,3.000000,270.000000,2,0,0),
(@LEADER,167608,4.242641,45.000000,2,0,0),
(@LEADER,167609,3.000000,0.000000,2,0,0),
(@LEADER,167610,4.242641,315.000000,2,0,0),
(@LEADER,167611,6.708204,26.565051,2,0,0),
(@LEADER,167612,6.000000,0.000000,2,0,0),
(@LEADER,167613,6.708204,333.434949,2,0,0);

-- Reconstruct the road route from existing road waypoints and the Eastvale bridge approach.
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-9125.449,389.9619,91.57055,0,15000,0,0,100,0),
(@PATH,2,-9144.014,376.6461,90.75526,0,0,0,0,100,0),
(@PATH,3,-9150.359,370.6424,90.63026,0,0,0,0,100,0),
(@PATH,4,-9159.769,358.9712,89.09229,0,0,0,0,100,0),
(@PATH,5,-9169.689,347.3052,86.06768,0,0,0,0,100,0),
(@PATH,6,-9176.393,336.1326,83.34795,0,0,0,0,100,0),
(@PATH,7,-9179.039,324.0334,81.12244,0,0,0,0,100,0),
(@PATH,8,-9181.582,310.6828,79.00208,0,0,0,0,100,0),
(@PATH,9,-9182.38,299.1464,77.95396,0,0,0,0,100,0),
(@PATH,10,-9183.653,286.0146,76.42735,0,0,0,0,100,0),
(@PATH,11,-9184.186,259.5755,74.10345,0,0,0,0,100,0),
(@PATH,12,-9187.476,239.7987,72.50726,0,0,0,0,100,0),
(@PATH,13,-9193.939,228.3685,71.82954,0,0,0,0,100,0),
(@PATH,14,-9217.061,204.2547,69.10202,0,0,0,0,100,0),
(@PATH,15,-9229.925,191.6477,68.03234,0,0,0,0,100,0),
(@PATH,16,-9239.118,181.4567,67.84704,0,0,0,0,100,0),
(@PATH,17,-9252.867,160.5058,67.88796,0,0,0,0,100,0),
(@PATH,18,-9270.517,152.484,67.12913,0,0,0,0,100,0),
(@PATH,19,-9281.984,150.6615,66.75413,0,0,0,0,100,0),
(@PATH,20,-9303.524,142.9477,65.81363,0,0,0,0,100,0),
(@PATH,21,-9326.493,133.6549,64.28152,0,0,0,0,100,0),
(@PATH,22,-9338.177,126.1314,63.79213,0,0,0,0,100,0),
(@PATH,23,-9357.066,114.8676,62.03004,0,0,0,0,100,0),
(@PATH,24,-9376.01,105.7821,60.5753,0,0,0,0,100,0),
(@PATH,25,-9396.483,100.0127,58.82322,0,0,0,0,100,0),
(@PATH,26,-9415.729,90.13653,57.42837,0,0,0,0,100,0),
(@PATH,27,-9430.02,79.53779,56.53139,0,0,0,0,100,0),
(@PATH,28,-9441.903,69.07547,56.12108,0,0,0,0,100,0),
(@PATH,29,-9459.128,61.95774,55.895,0,0,0,0,100,0),
(@PATH,30,-9471.797,63.62576,56.15252,0,0,0,0,100,0),
(@PATH,31,-9492.84,58.88786,55.85588,0,0,0,0,100,0),
(@PATH,32,-9495.036,54.66113,55.81353,0,0,0,0,100,0),
(@PATH,33,-9505.715,31.62967,56.48589,0,0,0,0,100,0),
(@PATH,34,-9518.505,4.58865,56.11089,0,0,0,0,100,0),
(@PATH,35,-9531.666,-17.71886,56.00526,0,0,0,0,100,0),
(@PATH,36,-9542.052,-43.97417,56.69081,0,0,0,0,100,0),
(@PATH,37,-9546.561,-64.21647,57.19081,0,0,0,0,100,0),
(@PATH,38,-9548.437,-83.4119,57.64455,0,0,0,0,100,0),
(@PATH,39,-9549.182,-112.0365,57.55236,0,0,0,0,100,0),
(@PATH,40,-9553.459,-136.2681,57.28034,0,0,0,0,100,0),
(@PATH,41,-9567.966,-150.9942,57.1911,0,0,0,0,100,0),
(@PATH,42,-9578.535,-171.4949,57.50352,0,0,0,0,100,0),
(@PATH,43,-9588.345,-202.6797,57.81245,0,0,0,0,100,0),
(@PATH,44,-9598.686,-228.6033,57.3935,0,0,0,0,100,0),
(@PATH,45,-9608.691,-255.3432,57.32165,0,0,0,0,100,0),
(@PATH,46,-9612.653,-276.4153,57.76284,0,0,0,0,100,0),
(@PATH,47,-9618.642,-297.6285,57.18179,0,0,0,0,100,0),
(@PATH,48,-9620.884,-320.5441,57.52703,0,0,0,0,100,0),
(@PATH,49,-9620.904,-344.161,57.28194,0,0,0,0,100,0),
(@PATH,50,-9619.874,-371.5169,57.58074,0,0,0,0,100,0),
(@PATH,51,-9617.868,-395.8977,57.95574,0,0,0,0,100,0),
(@PATH,52,-9614.505,-416.2884,57.66486,0,0,0,0,100,0),
(@PATH,53,-9608.497,-436.1051,57.36327,0,0,0,0,100,0),
(@PATH,54,-9600.497,-453.0271,57.78099,0,0,0,0,100,0),
(@PATH,55,-9590.743,-469.7306,57.8166,0,0,0,0,100,0),
(@PATH,56,-9589.494,-487.5282,57.76851,0,0,0,0,100,0),
(@PATH,57,-9593.436,-504.3423,57.52607,0,0,0,0,100,0),
(@PATH,58,-9602.272,-517.8843,56.76878,0,0,0,0,100,0),
(@PATH,59,-9611.551,-532.0573,54.7783,0,0,0,0,100,0),
(@PATH,60,-9617.313,-551.6019,54.49004,0,0,0,0,100,0),
(@PATH,61,-9619.694,-574.7324,54.19883,0,0,0,0,100,0),
(@PATH,62,-9622.42,-603.7773,52.84998,0,0,0,0,100,0),
(@PATH,63,-9628.837,-641.6657,51.14138,0,0,0,0,100,0),
(@PATH,64,-9643.346,-663.4222,49.36496,0,0,0,0,100,0),
(@PATH,65,-9649.04,-701.8206,46.00813,0,0,0,0,100,0),
(@PATH,66,-9656.645,-723.0509,44.66413,0,0,0,0,100,0),
(@PATH,67,-9656.33,-740.673,44.61954,0,0,0,0,100,0),
(@PATH,68,-9651.457,-762.5749,44.47965,0,0,0,0,100,0),
(@PATH,69,-9651.212,-783.6508,44.32621,0,0,0,0,100,0),
(@PATH,70,-9646.747,-797.5594,43.59379,0,0,0,0,100,0),
(@PATH,71,-9635.435,-813.6964,43.84379,0,0,0,0,100,0),
(@PATH,72,-9603.133,-848.3824,43.8069,0,0,0,0,100,0),
(@PATH,73,-9587.052,-870.2479,43.77868,0,0,0,0,100,0),
(@PATH,74,-9582.131,-892.7952,43.52868,0,0,0,0,100,0),
(@PATH,75,-9581.84,-917.2405,43.88842,0,0,0,0,100,0),
(@PATH,76,-9589.601,-935.541,43.71594,0,0,0,0,100,0),
(@PATH,77,-9605.688,-952.9254,43.84697,0,0,0,0,100,0),
(@PATH,78,-9616.812,-971.4866,43.64319,0,0,0,0,100,0),
(@PATH,79,-9622.979,-993.5161,42.51941,0,0,0,0,100,0),
(@PATH,80,-9621.585,-1009.805,41.4601,0,0,0,0,100,0),
(@PATH,81,-9618.49,-1038.765,39.85565,0,0,0,0,100,0),
(@PATH,82,-9614.81,-1070.425,39.68115,0,60000,0,0,100,0);
