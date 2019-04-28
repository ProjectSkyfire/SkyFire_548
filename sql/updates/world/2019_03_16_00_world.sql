-- SAI for Quest: "See to the Survivors (27671)"
SET @SURVIVOR := 46268;
SET @ATB := 86264;
SET @EMOTE := 26;

DELETE FROM conditions WHERE SourceEntry = @ATB;
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment) VALUES
(17, 0, @ATB, 0, 0, 31, 1, 3, @SURVIVOR, 0, 0, 0, 0, '', 'Item Teleport Beacon can only be used if target is Survivor');

DELETE FROM creature_template_addon WHERE entry= @ATB;
INSERT INTO creature_template_addon (entry, path_id, mount, bytes1, bytes2, emote, auras) VALUES
(@ATB, 0, 0, 0, 1, 431, NULL);

UPDATE creature_template SET AIName = "SmartAI" WHERE entry = @SURVIVOR;

DELETE FROM smart_scripts WHERE entryorguid = @SURVIVOR;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(@SURVIVOR, 0, 0, 0, 1, 0, 50, 0, 60000, 300000, 60000, 300000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Survivor - Say Text 0'),
(@SURVIVOR, 0, 1, 2, 8, 0, 100, 1, @ATB, 0, 0, 0, 17, @EMOTE, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Survivor - On Spell Hit "Activate Teleport Beacon" - Set Emote State (EMOTE_CONGRATULATE)'),
(@SURVIVOR, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 33, @SURVIVOR, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Survivor - linked - Give Killcredit (46268)'),
(@SURVIVOR, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Survivor - linked - Despawn after 5 seconds');

DELETE FROM creature_text WHERE entry = @SURVIVOR;
INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(@SURVIVOR, 0, 0, 'Don''t let them eat me!', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 1, 'Help! I''m too weak to make it on my own.', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 2, 'I don''t want to die down here!', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 3, 'I''m really going to die down here, aren''t I?', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 4, 'I''m too young to die!', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 5, 'Please help me!', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 6, 'Some help! Anyone!', 12, 0, 100, 0, 0, 0, 'Survivor');