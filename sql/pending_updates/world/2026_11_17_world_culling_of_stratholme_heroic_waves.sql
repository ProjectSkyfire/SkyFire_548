-- The Culling of Stratholme heroic waves summon the Acolyte, which had no abilities. The spells are the normal and
-- heroic pairs of Cone of Cold, Fire Blast, Shadow Shock, Corruption and Curse of Agony, picked by the event flags
-- 2 and 4.
-- Safe to run twice: the script rows are deleted by entry before they are inserted.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=27731 AND `ScriptName`='';
DELETE FROM `smart_scripts` WHERE `entryorguid`=27731 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(27731, 0, 0, 0, 9, 0, 100, 4, 0, 8, 14000, 19000, 0, 11, 20828, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - Within 0-8 Range - Cast ''Cone of Cold'' (Phase 1)'),
(27731, 0, 1, 0, 9, 0, 100, 8, 0, 8, 14000, 19000, 0, 11, 15244, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - Within 0-8 Range - Cast ''Cone of Cold'' (Phase 1)'),
(27731, 0, 2, 0, 9, 0, 100, 4, 0, 20, 8000, 13000, 0, 11, 20832, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - Within 0-20 Range - Cast ''Fire Blast'' (Phase 1)'),
(27731, 0, 3, 0, 9, 0, 100, 8, 0, 20, 8000, 13000, 0, 11, 14145, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - Within 0-20 Range - Cast ''Fire Blast'' (Phase 1)'),
(27731, 0, 4, 0, 0, 0, 100, 4, 4000, 8000, 7000, 10000, 0, 11, 17439, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - In Combat - Cast ''Shadow Shock'' (Phase 1)'),
(27731, 0, 5, 0, 0, 0, 100, 8, 4000, 8000, 7000, 10000, 0, 11, 17234, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - In Combat - Cast ''Shadow Shock'' (Phase 1)'),
(27731, 0, 6, 0, 0, 0, 100, 4, 6000, 10000, 20000, 30000, 0, 11, 39621, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - In Combat - Cast ''Corruption'' (Phase 1)'),
(27731, 0, 7, 0, 0, 0, 100, 8, 6000, 10000, 20000, 30000, 0, 11, 58811, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - In Combat - Cast ''Corruption'' (Phase 1)'),
(27731, 0, 8, 0, 0, 0, 100, 4, 11000, 15000, 35000, 45000, 0, 11, 14875, 1, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - In Combat - Cast ''Curse of Agony'' (Phase 1)'),
(27731, 0, 9, 0, 0, 0, 100, 8, 11000, 15000, 35000, 45000, 0, 11, 46190, 1, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Acolyte - In Combat - Cast ''Curse of Agony'' (Phase 1)');
