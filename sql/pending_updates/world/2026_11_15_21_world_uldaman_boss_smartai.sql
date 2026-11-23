-- Uldaman (map 70): Revelosh, Grimlok and Galgann Firehammer move from their own C++ scripts to SmartAI. The scripts
-- used timers of their own and cast the casters' spells from melee range; the SmartAI rows are the ones the 4.3.4
-- data has (casters stay at range, Grimlok's Tremor Totem, Shrink below 50% and Bloodlust below 30% health, Revelosh
-- flees at 15%).
-- Grimlok's kill line moves from text group 1 to group 2, where the SmartAI row reads it.
-- Idempotent: absolute UPDATEs, rows deleted by key before they are inserted.

UPDATE `creature_template` SET `ScriptName` = '', `AIName` = 'SmartAI' WHERE `entry` IN (4854, 6910, 7291);
UPDATE `creature_text` SET `groupid` = 2 WHERE `entry` = 4854 AND `groupid` = 1;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (4854,6910,7291);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4854, 0, 0, 0, 4, 0, 100, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grimlok - On Aggro - Say Line 0 (Normal Dungeon)'),
(4854, 0, 1, 0, 0, 0, 100, 4, 0, 0, 2400, 3800, 0, 11, 9532, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grimlok - In Combat CMC - Cast ''Lightning Bolt'' (Normal Dungeon)'),
(4854, 0, 2, 0, 0, 0, 100, 4, 8000, 12000, 21000, 31000, 0, 11, 8292, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grimlok - In Combat - Cast ''Chain Bolt'' (Normal Dungeon)'),
(4854, 0, 3, 0, 0, 0, 100, 5, 5000, 7000, 0, 0, 0, 11, 8143, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grimlok - In Combat - Cast ''Tremor Totem'' (No Repeat) (Normal Dungeon)'),
(4854, 0, 4, 0, 2, 0, 100, 4, 0, 50, 25000, 35000, 0, 11, 11892, 33, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Grimlok - Between 0-50% Health - Cast ''Shrink'' (Normal Dungeon)'),
(4854, 0, 5, 0, 2, 0, 100, 4, 0, 30, 30000, 35000, 0, 11, 6742, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grimlok - Between 0-30% Health - Cast ''Bloodlust'' (Normal Dungeon)'),
(4854, 0, 6, 0, 5, 0, 100, 5, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grimlok - On Killed Unit - Say Line 2 (No Repeat) (Normal Dungeon)'),
(6910, 0, 0, 0, 0, 0, 100, 4, 0, 0, 2400, 3800, 0, 11, 15801, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Revelosh - In Combat CMC - Cast ''Lightning Bolt'' (Normal Dungeon)'),
(6910, 0, 1, 0, 0, 0, 100, 4, 8000, 13000, 18000, 25000, 0, 11, 16006, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Revelosh - In Combat - Cast ''Chain Lightning'' (Normal Dungeon)'),
(6910, 0, 2, 0, 2, 0, 100, 5, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Revelosh - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(7291, 0, 0, 0, 4, 0, 100, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Galgann Firehammer - On Aggro - Say Line 0 (No Repeat) (Normal Dungeon)'),
(7291, 0, 2, 0, 0, 0, 100, 4, 9000, 11000, 13000, 16000, 0, 11, 11969, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Galgann Firehammer - In Combat - Cast ''Fire Nova'' (No Repeat) (Normal Dungeon)'),
(7291, 0, 3, 0, 0, 0, 100, 4, 2000, 5000, 13000, 18000, 0, 11, 3356, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Galgann Firehammer - In Combat - Cast ''Flame Lash'' (No Repeat) (Normal Dungeon)'),
(7291, 0, 4, 0, 0, 0, 100, 4, 5000, 6000, 14000, 19000, 0, 11, 9482, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Galgann Firehammer - In Combat - Cast ''Amplify Flames'' (No Repeat) (Normal Dungeon)');
