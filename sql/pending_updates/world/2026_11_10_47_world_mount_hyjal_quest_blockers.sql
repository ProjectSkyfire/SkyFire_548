-- Mount Hyjal: three quest blockers

-- vehicle_template_accessory: Kristoff Manheim (quests 25328 and 25332) sits in his chain vehicle, which is spawned, but the
-- vehicle had no passenger; the Twilight Knight Rider and the Twilight Lancer ride the buzzard and the firebird of the
-- Hatchery flight (their waves are not given to players yet)
DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (39789, 39833, 40650);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(39789, 39640, 0, 0, '', 8, 0),
(39833, 39835, 0, 1, 'Twilight Knight Rider on Twilight Buzzard', 8, 0),
(40650, 40660, 0, 1, 'Twilight Lancer on Twilight Firebird', 8, 0);

-- event_scripts: Lycanthoth is summoned at his altar, where the 5.4.8 data stands him, for three minutes
UPDATE `event_scripts` SET `datalong2` = 180000, `x` = 5141.02, `y` = -2304.43, `z` = 1278.43, `o` = 0.66633 WHERE `id` = 24005 AND `command` = 10 AND `datalong` = 39446;

-- creature: Kalecgos, who ends quest 29234 (Delegation) in Borean Tundra, was not spawned
-- Provisional creature guid 9253300
DELETE FROM `creature` WHERE `guid` = 9253300;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9253300, 53009, 571, 1, 0, 0, 0, 0, 3546.56, 5882.51, 143.915, 1.18275, 300, 0, 0, 77490, 0, 0, 0, 0, 0);
