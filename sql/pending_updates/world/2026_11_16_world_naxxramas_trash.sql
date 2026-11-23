-- Naxxramas: SmartAI for the Crypt Reaver, Plague Slime, Infectious Ghoul and Plagued Ghoul
-- Ported from TrinityCore 3.3.5. TrinityCore's 10 and 25 player difficulty bits are moved to the 10 and 25 player normal bits.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (15978,16243,16244,16447) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (15978,16243,16244,16447);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(15978, 0, 0, 0, 0, 0, 100, 24, 2000, 5000, 6000, 9000, 0, 11, 40504, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Crypt Reaver - In Combat - Cast ''Cleave'' (No Repeat)'),
(15978, 0, 1, 2, 2, 0, 100, 24, 0, 30, 120000, 120000, 0, 11, 56625, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crypt Reaver - Between 0-30% Health - Cast ''Frenzy'' (Dungeon)'),
(15978, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crypt Reaver - Between 0-30% Health - Say Line 0 (Dungeon)'),
(16243, 0, 0, 0, 0, 0, 100, 25, 0, 0, 0, 0, 0, 31, 1, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Slime - In Combat - Set Phase Random Between 1-4'),
(16243, 0, 1, 0, 0, 2, 100, 25, 1000, 1000, 0, 0, 0, 11, 28987, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Slime - In Combat - Cast ''Plague Slime (Black)'''),
(16243, 0, 2, 0, 0, 4, 100, 25, 1000, 1000, 0, 0, 0, 11, 28988, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Slime - In Combat - Cast ''Plague Slime (Blue)'''),
(16243, 0, 3, 0, 0, 8, 100, 25, 1000, 1000, 0, 0, 0, 11, 28989, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Slime - In Combat - Cast ''Plague Slime (Green)'''),
(16243, 0, 4, 0, 0, 0, 100, 25, 1000, 1000, 0, 0, 0, 11, 28990, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Slime - In Combat - Cast ''Plague Slime (Red)'''),
(16244, 0, 0, 0, 0, 0, 100, 8, 3000, 6000, 9000, 12000, 0, 11, 29915, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Infectious Ghoul - In Combat - Cast ''Flesh Rot'''),
(16244, 0, 1, 0, 0, 0, 100, 16, 3000, 6000, 9000, 12000, 0, 11, 54709, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Infectious Ghoul - In Combat - Cast ''Flesh Rot'''),
(16244, 0, 2, 0, 0, 0, 100, 8, 8000, 14000, 19000, 24000, 0, 11, 54703, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Infectious Ghoul - In Combat - Cast ''Rend'''),
(16244, 0, 3, 0, 0, 0, 100, 16, 8000, 14000, 19000, 24000, 0, 11, 54708, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Infectious Ghoul - In Combat - Cast ''Rend'''),
(16244, 0, 4, 5, 2, 0, 100, 24, 0, 30, 90000, 90000, 0, 11, 54701, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infectious Ghoul - Between 0-30% Health - Cast ''Frenzy'' (Dungeon)'),
(16244, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infectious Ghoul - Between 0-30% Health - Say Line 0 (Dungeon)'),
(16447, 0, 0, 0, 0, 0, 100, 24, 2000, 5000, 12000, 16000, 0, 11, 29915, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Plagued Ghoul - In Combat - Cast ''Flesh Rot'' (No Repeat) (Normal Dungeon)'),
(16447, 0, 1, 0, 0, 0, 100, 24, 4000, 8000, 15000, 19000, 0, 11, 13738, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Plagued Ghoul - In Combat - Cast ''Rend'' (No Repeat) (Normal Dungeon)'),
(16447, 0, 2, 3, 2, 0, 100, 25, 0, 30, 0, 0, 0, 11, 24318, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plagued Ghoul - Between 0-30% Health - Cast ''Frenzy'' (No Repeat) (Dungeon)'),
(16447, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plagued Ghoul - Between 0-30% Health - Say Line 0 (No Repeat) (Dungeon)');

INSERT IGNORE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(15978, 0, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Crypt Reaver'),
(16244, 0, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Infectious Ghoul'),
(16447, 0, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Plagued Ghoul');
