-- The Shattered Halls: the Shattered Hand Savages (SkyFire had none, TrinityCore 3.3.5 sniffed 13 in build
-- 14545) with the SmartAI and text of TrinityCore (Deathblow in normal 36023 and heroic 36054, Slice and
-- Dice, Enrage below 30 percent).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 16523 AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `entryorguid` = 16523 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(16523, 0, 0, 0, 0, 0, 100, 4, 1500, 1500, 12000, 14500, 0, 11, 36023, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shattered Hand Savage - In Combat - Cast Deathblow (Normal Dungeon)'),
(16523, 0, 1, 0, 0, 0, 100, 8, 1500, 1500, 12000, 14500, 0, 11, 36054, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shattered Hand Savage - In Combat - Cast Deathblow (Heroic Dungeon)'),
(16523, 0, 2, 0, 0, 0, 100, 12, 4000, 4000, 8000, 14000, 0, 11, 30470, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shattered Hand Savage - In Combat - Cast Slice and Dice'),
(16523, 0, 3, 4, 2, 0, 100, 12, 0, 30, 60, 60, 0, 11, 30485, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shattered Hand Savage - Between 0-30% Health - Cast Enrage'),
(16523, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shattered Hand Savage - Between 0-30% Health - Say Line 0');

DELETE FROM `creature_text` WHERE `entry` = 16523;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(16523, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Shattered Hand Savage');

DELETE FROM `creature` WHERE `guid` BETWEEN 8319140 AND 8319155;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8319140, 16523, 540, 6, 0, 0, 0, 0, 52.12560, 7.03777, -13.21110, 3.559010, 7200, 0, 0, 0, 0, 2, 0, 0, 0),
(8319141, 16523, 540, 6, 0, 0, 0, 0, 64.49470, 7.92366, -13.21290, 4.465340, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319142, 16523, 540, 6, 0, 0, 0, 0, 60.28710, 14.17210, -13.21840, 3.656390, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319143, 16523, 540, 6, 0, 0, 0, 0, 69.60130, 39.09190, -13.22980, 4.928810, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319144, 16523, 540, 6, 0, 0, 0, 0, 83.66960, 57.30760, -13.12300, 2.924040, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319145, 16523, 540, 6, 0, 0, 0, 0, 56.92240, 57.60810, -13.12200, 5.776030, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319146, 16523, 540, 6, 0, 0, 0, 0, 65.68930, 12.66020, -13.21540, 4.465340, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319147, 16523, 540, 6, 0, 0, 0, 0, 512.33900, 172.70700, 1.94192, 1.515260, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319148, 16523, 540, 6, 0, 0, 0, 0, 525.18400, 171.85900, 1.94192, 1.515260, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319149, 16523, 540, 6, 0, 0, 0, 0, 524.73600, 120.87000, 1.99425, 1.515260, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319150, 16523, 540, 6, 0, 0, 0, 0, 514.60100, 120.46000, 1.99257, 1.515260, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8319151, 16523, 540, 6, 0, 0, 0, 0, 61.30350, -0.40230, -13.20680, 2.896550, 7200, 0, 0, 0, 0, 0, 0, 0, 0);
