UPDATE creature_template SET AIName='SmartAI' WHERE  entry=34863;
DELETE FROM smart_scripts WHERE entryorguid=34863;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(34863, 0, 0, 0, 20, 0, 100, 0, 14078, 0, 0, 0, 44, 170, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lieutenant Walden: On quest reward - set player phaseId 170');
