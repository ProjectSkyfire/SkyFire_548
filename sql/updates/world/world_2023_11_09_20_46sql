UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE  `entry`=54586;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=54587;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE  `entry`=65470;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=65471;

DELETE FROM `spell_area` WHERE `spell`=102403;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(102403, 5825, 29524, 29524, 0, 0, 2, 1, 74, 11),
(102403, 5834, 29524, 29524, 0, 0, 2, 1, 74, 11),
(102403, 5843, 29524, 29524, 0, 0, 2, 1, 74, 11);

DELETE FROM `smart_scripts` WHERE `entryorguid`=54586;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54586, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Respawn - Set Invincibility Hp 1'),
(54586, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 0 - Set React State Defensive'),
(54586, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 1 - Set Phase 0'),
(54586, 0, 3, 0, 1, 0, 100, 0, 2000, 4000, 2000, 4000, 87, 5458601, 5458602, 5458603, 5458604, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - OOC - Run Random Script'),
(54586, 0, 4, 0, 4, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Aggro - Set Phase 1'),
(54586, 0, 5, 6, 2, 1, 100, 1, 0, 1, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Between 0-1% Health - Combat Stop'),
(54586, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 5 - Set Faction 35'),
(54586, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 6 - Set Emote State 0'),
(54586, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 7 - Say Line Random'),
(54586, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 33, 54586, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 8 - Quest Credit'),
(54586, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 9 - Despawn after 3 sec');

DELETE FROM `smart_scripts` WHERE `entryorguid`=54587;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54587, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Respawn - Set Invincibility Hp 1'),
(54587, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 0 - Set React State Defensive'),
(54587, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 1 - Set Phase 0'),
(54587, 0, 3, 0, 38, 0, 100, 0, 0, 1, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 1 - Play Emote 507'),
(54587, 0, 4, 0, 38, 0, 100, 0, 0, 2, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 2 - Play Emote 508'),
(54587, 0, 5, 0, 38, 0, 100, 0, 0, 3, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 3 - Play Emote 509'),
(54587, 0, 6, 0, 4, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Aggro - Set Phase 1'),
(54587, 0, 7, 8, 2, 1, 100, 1, 0, 1, 0, 0, 27, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Between 0-1% Health - Combat Stop'),
(54587, 0, 8, 9, 61, 1, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 7 - Set Faction 35'),
(54587, 0, 9, 10, 61, 1, 100, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 8 - Set Emote State 0'),
(54587, 0, 10, 11, 61, 1, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 9 - Say Line Random'),
(54587, 0, 11, 12, 61, 1, 100, 0, 0, 0, 0, 0, 33, 54586, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 10 - Quest Credit'),
(54587, 0, 12, 0, 61, 1, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 11 - Despawn after 3 sec');

DELETE FROM `smart_scripts` WHERE `entryorguid`=65470;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(65470, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Respawn - Set Invincibility Hp 1'),
(65470, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 0 - Set React State Defensive'),
(65470, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 1 - Set Phase 0'),
(65470, 0, 3, 0, 1, 0, 100, 0, 2000, 4000, 2000, 4000, 87, 5458601, 5458602, 5458603, 5458604, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - OOC - Run Random Script'),
(65470, 0, 4, 0, 4, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Aggro - Set Phase 1'),
(65470, 0, 5, 6, 2, 1, 100, 1, 0, 1, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Between 0-1% Health - Combat Stop'),
(65470, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 5 - Set Faction 35'),
(65470, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 6 - Set Emote State 0'),
(65470, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 7 - Say Line Random'),
(65470, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 33, 54586, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 8 - Quest Credit'),
(65470, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - Linked To Id 9 - Despawn after 3 sec');

DELETE FROM `smart_scripts` WHERE `entryorguid`=65471;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(65471, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Respawn - Set Invincibility Hp 1'),
(65471, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 0 - Set React State Defensive'),
(65471, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 1 - Set Phase 0'),
(65471, 0, 3, 0, 38, 0, 100, 0, 0, 1, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 1 - Play Emote 507'),
(65471, 0, 4, 0, 38, 0, 100, 0, 0, 2, 0, 0, 5, 508, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 2 - Play Emote 508'),
(65471, 0, 5, 0, 38, 0, 100, 0, 0, 3, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Data Set 0 3 - Play Emote 509'),
(65471, 0, 6, 0, 4, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - On Aggro - Set Phase 1'),
(65471, 0, 7, 8, 2, 1, 100, 1, 0, 1, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Between 0-1% Health - Combat Stop'),
(65471, 0, 8, 9, 61, 1, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 7 - Set Faction 35'),
(65471, 0, 9, 10, 61, 1, 100, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 8 - Set Emote State 0'),
(65471, 0, 10, 11, 61, 1, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 9 - Say Line Random'),
(65471, 0, 11, 12, 61, 1, 100, 0, 0, 0, 0, 0, 33, 54586, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 10 - Quest Credit'),
(65471, 0, 12, 0, 61, 1, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tushui Trainee - Linked To Id 11 - Despawn after 3 sec');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458601;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458601, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 507, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Script - Play Emote 507');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458602;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458602, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 509, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Script - Play Emote 509');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458603;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458603, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 511, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Script - Play Emote 511');

DELETE FROM `smart_scripts` WHERE `entryorguid`=5458604;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5458604, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 5, 543, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huojin Trainee - On Script - Play Emote 543');

UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=54586;
UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=54587;
UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=65470;
UPDATE `creature_template` SET `unit_flags2`=2048 WHERE `entry`=65471;
