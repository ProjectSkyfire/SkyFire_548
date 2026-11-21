-- Mulgore (reported from the Chief Squealer camp, ticket 152): Fledgling Braves and the quillboar around them stood
-- still. 54 of the 113 Fledgling Braves (36942) have a Bristleback Invader (36943) within 4 yd and 28 more within 30 yd,
-- spawned as fighting pairs; their factions (1935 and 2102) are hostile to each other, but stationary creatures never
-- get a line-of-sight check, so nobody starts the fight. Same fix as the Spitescale Cove battle: out of combat they
-- attack the nearest opponent. Chief Squealer Thornmantle (36712) and his Thornguards (43599) are left to the
-- players. No Razormane stands within 35 yd of any Brave, the quillboar here are Bristleback. Safe to run twice.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 36942 AND `id` >= 3;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 36943 AND `id` >= 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36942, 0, 3, 0, 1, 0, 100, 0, 1000, 1000, 5000, 5000, 0, 49, 0, 0, 0, 0, 0, 0, 11, 36943, 10, 0, 0, 0, 0, 0, 'Fledgling Brave - Out of combat - Attack the nearest Bristleback Invader within 10 yards'),
(36943, 0, 2, 0, 1, 0, 100, 0, 1000, 1000, 5000, 5000, 0, 49, 0, 0, 0, 0, 0, 0, 11, 36942, 10, 0, 0, 0, 0, 0, 'Bristleback Invader - Out of combat - Attack the nearest Fledgling Brave within 10 yards');
