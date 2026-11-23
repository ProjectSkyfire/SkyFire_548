-- Pilgrim's Bounty: retain 25 Wild Turkey spawns in each supported zone.
-- Elwynn Forest: 30 -> 25; Tirisfal Glades: 40 -> 25.
-- Remove the most redundant positions while retaining broad hunting coverage.
-- Surviving spawns keep event 26, random movement, loot and Turkinator credit.
-- Restrict cleanup to this event's reserved Wild Turkey spawn GUIDs.
DELETE `ge` FROM `game_event_creature` AS `ge`
 INNER JOIN `creature` AS `c` ON `c`.`guid`=`ge`.`guid`
 WHERE `ge`.`eventEntry`=26 AND `c`.`id`=32820
 AND `c`.`guid` IN (9001017, 9001020, 9001023, 9001026, 9001037, 9002154, 9002159, 9002160, 9002161, 9002162, 9002169, 9002170, 9002171, 9002172, 9002174, 9002177, 9002184, 9002186, 9002191, 9002192);
DELETE FROM `creature` WHERE `id`=32820
 AND `guid` IN (9001017, 9001020, 9001023, 9001026, 9001037, 9002154, 9002159, 9002160, 9002161, 9002162, 9002169, 9002170, 9002171, 9002172, 9002174, 9002177, 9002184, 9002186, 9002191, 9002192);

-- Redistribute five of Elwynn's 25 turkeys along the Stormwind approach and
-- feast grounds. Ground positions come from existing outdoor critter spawns.
-- Keep the event links and total population; do not add permanent turkeys.
UPDATE `creature` SET `position_x`=-9241.4, `position_y`=277.641, `position_z`=73.063, `orientation`=2.87783 WHERE `guid`=9001013 AND `id`=32820;
UPDATE `creature` SET `position_x`=-9221.74, `position_y`=336.121, `position_z`=73.4121, `orientation`=4.09794 WHERE `guid`=9001019 AND `id`=32820;
UPDATE `creature` SET `position_x`=-9199.59, `position_y`=343.454, `position_z`=74.0494, `orientation`=1.96619 WHERE `guid`=9001024 AND `id`=32820;
UPDATE `creature` SET `position_x`=-9147.32, `position_y`=420.592, `position_z`=94.3919, `orientation`=1.55111 WHERE `guid`=9001029 AND `id`=32820;
UPDATE `creature` SET `position_x`=-9180.7, `position_y`=417.456, `position_z`=90.611, `orientation`=4.91285 WHERE `guid`=9001033 AND `id`=32820;
