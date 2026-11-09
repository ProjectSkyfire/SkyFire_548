-- Lost in the Floods: Thonk's Spyglass summons a telescope vehicle that flies over each lost scout and gives the credit.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `spell_script_names` WHERE `spell_id` = 73817;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(73817, 'spell_thonks_spyglass');

UPDATE `creature_template` SET `unit_flags` = 33554944, `speed_run` = 6, `VehicleId` = 686, `AIName` = 'SmartAI', `mechanic_immune_mask` = 16384 WHERE `entry` IN (39320, 39345, 39346, 39347);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (39320, 39345, 39346, 39347);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(39320, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 21, 30, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 01 (Raggaran) - On Summoned - Store Target List'),
(39320, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 39320, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 01 (Raggaran) - Linked To Id 0 - Waypoint Start'),
(39320, 0, 2, 0, 58, 0, 100, 0, 4, 0, 0, 0, 0, 67, 39320, 10000, 10000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 01 (Raggaran) - On Waypoint Ended - Create Timed Event'),
(39320, 0, 3, 4, 59, 0, 100, 0, 39320, 0, 0, 0, 0, 62, 1, 0, 0, 0, 0, 0, 12, 1, 0, 0, 393.527, -4581.75, 76.592, 1.05974, 'Durotar Watershed Telescope 01 (Raggaran) - On Timed Event Triggered - Teleport Player'),
(39320, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39357, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 01 (Raggaran) - Linked To Id 3 - Quest Credit'),
(39320, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 01 (Raggaran) - Linked To Id 4 - Despawn'),
(39345, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 21, 30, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 02 (Tekla) - Store Target List'),
(39345, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 39345, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 02 (Tekla) - Linked To Id 0 - Waypoint Start'),
(39345, 0, 2, 0, 58, 0, 100, 0, 4, 0, 0, 0, 0, 67, 39345, 10000, 10000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 02 (Tekla) - On Waypoint Ended - Create Timed Event'),
(39345, 0, 3, 4, 59, 0, 100, 0, 39345, 0, 0, 0, 0, 62, 1, 0, 0, 0, 0, 0, 12, 1, 0, 0, 393.527, -4581.75, 76.592, 1.05974, 'Durotar Watershed Telescope 02 (Tekla) - On Timed Event Triggered - Teleport Player'),
(39345, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39358, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 02 (Tekla) - Linked To Id 3 - Quest Credit'),
(39345, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 02 (Tekla) - Linked To Id 4 - Despawn'),
(39346, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 21, 30, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 03 (Misha) - On Summoned - Store Target List'),
(39346, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 39346, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 03 (Misha) - Linked To Id 0 - Waypoint Start'),
(39346, 0, 2, 0, 58, 0, 100, 0, 4, 0, 0, 0, 0, 67, 39346, 10000, 10000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 03 (Misha) - On Waypoint Ended - Create Timed Event'),
(39346, 0, 3, 4, 59, 0, 100, 0, 39346, 0, 0, 0, 0, 62, 1, 0, 0, 0, 0, 0, 12, 1, 0, 0, 393.527, -4581.75, 76.592, 1.05974, 'Durotar Watershed Telescope 03 (Misha) - On Timed Event Triggered - Teleport Creature'),
(39346, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39359, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 03 (Misha) - Linked To Id 3 - Quest Credit'),
(39346, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 03 (Misha) - Linked To Id 4 - Despawn'),
(39347, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 21, 30, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 04 (Zen\'Taji) - On Summoned - Store Target List'),
(39347, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 39347, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 04 (Zen\'Taji) - Linked To Id 0 - Waypoint Start'),
(39347, 0, 2, 0, 58, 0, 100, 0, 3, 0, 0, 0, 0, 67, 39347, 10000, 10000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 04 (Zen\'Taji) - On Waypoint Ended - Create Timed Event'),
(39347, 0, 3, 4, 59, 0, 100, 0, 39347, 0, 0, 0, 0, 62, 1, 0, 0, 0, 0, 0, 12, 1, 0, 0, 393.527, -4581.75, 76.592, 1.05974, 'Durotar Watershed Telescope 04 (Zen\'Taji) - On Timed Event Triggered - Teleport Player'),
(39347, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39360, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 04 (Zen\'Taji) - Linked To Id 3 - Quest Credit'),
(39347, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Durotar Watershed Telescope 04 (Zen\'Taji) - Linked To Id 4 - Despawn');

DELETE FROM `waypoints` WHERE `entry` IN (39320, 39345, 39346, 39347);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(39320, 1, 419.651, -4583.51, 75.2948, 'Durotar Watershed Telescope 01 (Raggaran)'),
(39320, 2, 385.281, -4565.53, 76.6682, 'Durotar Watershed Telescope 01 (Raggaran)'),
(39320, 3, 350.911, -4547.55, 78.0417, 'Durotar Watershed Telescope 01 (Raggaran)'),
(39320, 4, 60.7847, -4232.33, 38.4023, 'Durotar Watershed Telescope 01 (Raggaran)'),
(39345, 1, 358.496, -4602.83, 75.2948, 'Durotar Watershed Telescope 02 (Tekla)'),
(39345, 2, 385.281, -4565.53, 76.6682, 'Durotar Watershed Telescope 02 (Tekla)'),
(39345, 3, 412.066, -4528.23, 78.0417, 'Durotar Watershed Telescope 02 (Tekla)'),
(39345, 4, 555.582, -4146.9, 28.3467, 'Durotar Watershed Telescope 02 (Tekla)'),
(39346, 1, 329.949, -4592.83, 81.2394, 'Durotar Watershed Telescope 03 (Misha)'),
(39346, 2, 385.281, -4565.53, 76.6682, 'Durotar Watershed Telescope 03 (Misha)'),
(39346, 3, 440.613, -4538.23, 72.097, 'Durotar Watershed Telescope 03 (Misha)'),
(39346, 4, 721.934, -4260.48, 23.069, 'Durotar Watershed Telescope 03 (Misha)'),
(39347, 1, 385.281, -4565.53, 76.6682, 'Durotar Watershed Telescope 04 (Zen\'Taji)'),
(39347, 2, 385.194, -4497.49, 75.3749, 'Durotar Watershed Telescope 04 (Zen\'Taji)'),
(39347, 3, 349.146, -3869.01, 25.069, 'Durotar Watershed Telescope 04 (Zen\'Taji)');
