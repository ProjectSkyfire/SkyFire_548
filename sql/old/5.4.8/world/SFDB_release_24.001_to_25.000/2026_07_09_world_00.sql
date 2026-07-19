-- Fix 204811 Gameobject coordinates and size
DELETE FROM gameobject_template WHERE entry = 204811;
INSERT INTO gameobject_template (entry, type, displayId, name, IconName, castBarCaption, unk1, size, data0, AIName, ScriptName) VALUES (204811, 8, 5811, 'Manor Mistmantle Spell Focus', '', '', '', 0.01, 1677, '', '');
DELETE FROM gameobject WHERE id = 204811;
INSERT INTO gameobject (id, map, spawnMask, phaseId, phaseGroup, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES (204811, 0, 1, 0, 0, -10368.0, -1255.883, 35.91, 0, 0, 0, 0, 0, 120, 100, 1);

-- Stalvan Mistmantle RP texts
DELETE FROM creature_text WHERE entry = 315;
INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES 
(315, 0, 0, 'My ring... Who holds my family ring... Tilloa, is that you?', 12, 0, 100, 0, 0, 0, 'Stalvan RP 0'),
(315, 1, 0, 'Tobias...', 12, 0, 100, 0, 0, 0, 'Stalvan RP 1'),
(315, 2, 0, 'It''s all true, brother. Every word. You doubted it?', 12, 0, 100, 0, 0, 0, 'Stalvan RP 2'),
(315, 3, 0, 'You know why!', 12, 0, 100, 0, 0, 0, 'Stalvan RP 3'),
(315, 4, 0, 'Surely you''ve felt anger. Anger so foul and vicious that it makes you want to tear someone to shreds...', 12, 0, 100, 0, 0, 0, 'Stalvan RP 4'),
(315, 5, 0, 'Aren''t you feeling it right now?', 12, 0, 100, 0, 0, 0, 'Stalvan RP 5'),
(315, 6, 0, 'You see, brother... we''re not so different...', 12, 0, 100, 0, 0, 0, 'Stalvan RP 6');

-- Tobias Mistmantle RP texts
DELETE FROM creature_text WHERE entry = 35124;
INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES 
(35124, 0, 0, 'Brother!', 12, 0, 100, 0, 0, 0, 'Tobias RP 0'),
(35124, 1, 0, 'Tell me it''s not true, brother. Tell me you didn''t die a murderer!', 12, 0, 100, 0, 0, 0, 'Tobias RP 1'),
(35124, 2, 0, 'But why? How could you?', 12, 0, 100, 0, 0, 0, 'Tobias RP 2'),
(35124, 3, 0, 'No... NO! STOP IT!', 14, 0, 100, 0, 0, 0, 'Tobias RP 3'),
(35124, 4, 0, 'No...', 12, 0, 100, 0, 0, 0, 'Tobias RP 4');

-- Register spell script
DELETE FROM spell_script_names WHERE spell_id = 82029;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (82029, 'spell_q26674_call_stalvan');

-- Assign SmartAI to creatures
UPDATE creature_template SET ScriptName = '', AIName = 'SmartAI' WHERE entry IN (315, 35124);

-- Delete old smart scripts
DELETE FROM smart_scripts WHERE entryorguid IN (315, 31500, 35124, 351240, 351241, 351242);

-- Stalvan (315) SmartAI
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(315, 0, 0, 0, 63, 0, 100, 0, 0, 0, 0, 0, 80, 31500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Just Created - Start Timed ActionList 31500'),
(315, 0, 1, 0, 8, 0, 100, 0, 0, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Death - Talk 6'),
(315, 0, 2, 0, 8, 0, 100, 0, 0, 0, 0, 0, 45, 9, 9, 0, 0, 0, 0, 10, 35124, 100, 0, 0, 0, 0, 0, 'On Death - Trigger Tobias Data 9');

-- Stalvan (315) Timed ActionList (31500)
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(31500, 9, 0, 0, 59, 0, 100, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Stalvan Invincibility HP Level'),
(31500, 9, 1, 0, 59, 0, 100, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10369.74, -1253.70, 35.91, 0, '0ms - Stalvan Walk'),
(31500, 9, 2, 0, 59, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Stalvan Talk 0'),
(31500, 9, 3, 0, 59, 0, 100, 0, 4000, 4000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '4000ms wait - Stalvan Talk 1'),
(31500, 9, 4, 0, 59, 0, 100, 0, 4000, 4000, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '4000ms wait - Stalvan Talk 2'),
(31500, 9, 5, 0, 59, 0, 100, 0, 5000, 5000, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '5000ms wait - Stalvan Talk 3'),
(31500, 9, 6, 0, 59, 0, 100, 0, 2000, 2000, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '2000ms wait - Stalvan Talk 4'),
(31500, 9, 7, 0, 59, 0, 100, 0, 4000, 4000, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '4000ms wait - Stalvan Talk 5'),
(31500, 9, 8, 0, 59, 0, 100, 0, 2000, 2000, 0, 0, 19, 770, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '2000ms wait - Stalvan Remove Unit Flags'),
(31500, 9, 9, 0, 59, 0, 100, 0, 1000, 1000, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '1000ms wait - Stalvan React State Aggressive'),
(31500, 9, 10, 0, 59, 0, 100, 0, 500, 500, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 35124, 20, 0, 0, 0, 0, 0, '500ms wait - Stalvan Attack Tobias');

-- Tobias (35124) SmartAI
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(35124, 0, 0, 0, 63, 0, 100, 0, 0, 0, 0, 0, 80, 351240, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Just Created - Start Timed ActionList 351240'),
(35124, 0, 1, 0, 38, 0, 100, 0, 9, 9, 0, 0, 80, 351242, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Data 9 - Start Timed ActionList 351242');

-- Tobias (35124) Timed ActionList 351240
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(351240, 9, 0, 0, 59, 0, 100, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Tobias Invincibility HP Level'),
(351240, 9, 1, 0, 59, 0, 100, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10365.75, -1256.18, 35.91, 0, '0ms - Tobias Walk'),
(351240, 9, 2, 0, 59, 0, 100, 0, 2000, 2000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '2000ms wait - Tobias Talk 0'),
(351240, 9, 3, 0, 59, 0, 100, 0, 3000, 3000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '3000ms wait - Tobias Talk 1'),
(351240, 9, 4, 0, 59, 0, 100, 0, 6000, 6000, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '6000ms wait - Tobias Talk 2'),
(351240, 9, 5, 0, 59, 0, 100, 0, 9000, 9000, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '9000ms wait - Tobias Talk 3'),
(351240, 9, 6, 0, 59, 0, 100, 0, 1000, 1000, 0, 0, 11, 68996, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '1000ms wait - Tobias Cast Two Forms'),
(351240, 9, 7, 0, 59, 0, 100, 0, 0, 0, 0, 0, 3, 0, 33508, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms wait - Tobias Morph'),
(351240, 9, 8, 0, 59, 0, 100, 0, 0, 0, 0, 0, 19, 770, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms wait - Tobias Remove Unit Flags'),
(351240, 9, 9, 0, 59, 0, 100, 0, 1000, 1000, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '1000ms wait - Tobias React State Aggressive'),
(351240, 9, 10, 0, 59, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 315, 20, 0, 0, 0, 0, 0, '0ms wait - Tobias Attack Stalvan');

-- Delete old smart scripts
DELETE FROM smart_scripts WHERE entryorguid IN (315, 31500, 35124, 351240, 351241, 351242);

-- Stalvan (315) SmartAI
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(315, 0, 0, 0, 63, 0, 100, 0, 0, 0, 0, 0, 80, 31500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Just Created - Start Timed ActionList 31500'),
(315, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Death - Talk 6'),
(315, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 45, 9, 9, 0, 0, 0, 0, 11, 35124, 100, 0, 0, 0, 0, 0, 'On Death - Trigger Tobias Data 9');

-- Stalvan (315) Timed ActionList (31500)
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(31500, 9, 0, 0, 59, 0, 100, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Stalvan Invincibility HP Level'),
(31500, 9, 1, 0, 59, 0, 100, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10369.74, -1253.70, 35.91, 0, '0ms - Stalvan Walk'),
(31500, 9, 2, 0, 59, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Stalvan Talk 0'),
(31500, 9, 3, 0, 59, 0, 100, 0, 4000, 4000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '4000ms wait - Stalvan Talk 1'),
(31500, 9, 4, 0, 59, 0, 100, 0, 4000, 4000, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '4000ms wait - Stalvan Talk 2'),
(31500, 9, 5, 0, 59, 0, 100, 0, 5000, 5000, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '5000ms wait - Stalvan Talk 3'),
(31500, 9, 6, 0, 59, 0, 100, 0, 2000, 2000, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '2000ms wait - Stalvan Talk 4'),
(31500, 9, 7, 0, 59, 0, 100, 0, 4000, 4000, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '4000ms wait - Stalvan Talk 5'),
(31500, 9, 8, 0, 59, 0, 100, 0, 2000, 2000, 0, 0, 19, 770, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '2000ms wait - Stalvan Remove Unit Flags'),
(31500, 9, 9, 0, 59, 0, 100, 0, 1000, 1000, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '1000ms wait - Stalvan React State Aggressive'),
(31500, 9, 10, 0, 59, 0, 100, 0, 500, 500, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 35124, 20, 0, 0, 0, 0, 0, '500ms wait - Stalvan Attack Tobias');

-- Tobias (35124) SmartAI
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(35124, 0, 0, 0, 63, 0, 100, 0, 0, 0, 0, 0, 80, 351240, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Just Created - Start Timed ActionList 351240'),
(35124, 0, 1, 0, 38, 0, 100, 0, 9, 9, 0, 0, 80, 351242, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Data 9 - Start Timed ActionList 351242');

-- Tobias (35124) Timed ActionList 351240
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(351240, 9, 0, 0, 59, 0, 100, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Tobias Invincibility HP Level'),
(351240, 9, 1, 0, 59, 0, 100, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10365.75, -1256.18, 35.91, 0, '0ms - Tobias Walk'),
(351240, 9, 2, 0, 59, 0, 100, 0, 2000, 2000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '2000ms wait - Tobias Talk 0'),
(351240, 9, 3, 0, 59, 0, 100, 0, 3000, 3000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '3000ms wait - Tobias Talk 1'),
(351240, 9, 4, 0, 59, 0, 100, 0, 6000, 6000, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '6000ms wait - Tobias Talk 2'),
(351240, 9, 5, 0, 59, 0, 100, 0, 9000, 9000, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '9000ms wait - Tobias Talk 3'),
(351240, 9, 6, 0, 59, 0, 100, 0, 1000, 1000, 0, 0, 11, 68996, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '1000ms wait - Tobias Cast Two Forms'),
(351240, 9, 7, 0, 59, 0, 100, 0, 0, 0, 0, 0, 3, 0, 33508, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms wait - Tobias Morph'),
(351240, 9, 8, 0, 59, 0, 100, 0, 0, 0, 0, 0, 19, 770, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms wait - Tobias Remove Unit Flags'),
(351240, 9, 9, 0, 59, 0, 100, 0, 1000, 1000, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '1000ms wait - Tobias React State Aggressive'),
(351240, 9, 10, 0, 59, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 315, 20, 0, 0, 0, 0, 0, '0ms wait - Tobias Attack Stalvan');

-- Tobias (35124) Timed ActionList 351242
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(351242, 9, 0, 0, 59, 0, 100, 0, 0, 0, 0, 0, 18, 770, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Tobias Set Unit Flags'),
(351242, 9, 1, 0, 59, 0, 100, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Tobias Combat Stop'),
(351242, 9, 2, 0, 59, 0, 100, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Tobias React Passive'),
(351242, 9, 3, 0, 59, 0, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Tobias Set Faction Friendly'),
(351242, 9, 4, 0, 59, 0, 100, 0, 1000, 1000, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '1000ms - Talk 4'),
(351242, 9, 5, 0, 59, 0, 100, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '0ms - Tobias Set Run True'),
(351242, 9, 6, 0, 59, 0, 100, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -10385.0, -1250.0, 35.91, 0, '0ms - Move outside'),
(351242, 9, 7, 0, 59, 0, 100, 0, 5000, 5000, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '5000ms - Despawn');
