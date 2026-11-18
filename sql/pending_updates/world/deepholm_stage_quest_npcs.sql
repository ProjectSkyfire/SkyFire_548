-- Deepholm, Temple of Earth: the quest givers that the quest stages hid. A stage replaces the default phase here, so while
-- "Temple of Earth Finale, Stage 1" (phase 253) and, after "The Binding", "Temple of Earth Finale Complete" (phase 257, for
-- good) are active in the temple (area 5303), the NPCs that stand only in the default phase are gone: Seer Kormo (quests
-- 26244, 26259, 27100, 27101, 27102, 27136), Earthcaller Yevaa (26409, 26413, 26484, 27938, 27952, 27953), Windspeaker
-- Lorvarius and Caretaker Nuunwa. Copies of those spawns are added to the two stages at the same spots. Not retail data: the
-- 4.3.4 stages show the default spawns alongside the stage ones, this core shows only the stage.
-- Guids 9270300 to 9270309 (inside the Deepholm range 9270000 to 9276999). Safe to run twice.

DELETE FROM `creature` WHERE `guid` BETWEEN 9270300 AND 9270309;

INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9270300, 43397, 646, 1, 253, 0, 0, 0, 1024.06, 514.127, -49.3348, 4.12151, 120, 10, 0, 0, 0, 1, 0, 0, 0),
(9270301, 43397, 646, 1, 253, 0, 0, 0, 996.09, 518.243, -49.2515, 0.314159, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(9270302, 42573, 646, 1, 253, 0, 0, 0, 981.939, 528.03, -49.2515, 3.9619, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(9270303, 42732, 646, 1, 253, 0, 0, 0, 981.076, 507.977, -49.2515, 3.92699, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(9270304, 43397, 646, 1, 257, 0, 0, 0, 1024.06, 514.127, -49.3348, 4.12151, 120, 10, 0, 0, 0, 1, 0, 0, 0),
(9270305, 43397, 646, 1, 257, 0, 0, 0, 996.09, 518.243, -49.2515, 0.314159, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(9270306, 42573, 646, 1, 257, 0, 0, 0, 981.939, 528.03, -49.2515, 3.9619, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(9270307, 42732, 646, 1, 257, 0, 0, 0, 981.076, 507.977, -49.2515, 3.92699, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(9270308, 45300, 646, 1, 257, 0, 0, 0, 1032.42, 542.142, -49.3207, 3.9968, 120, 10, 0, 0, 0, 1, 0, 0, 0);
