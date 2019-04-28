-- SmartScript for Quest: Paint it Black (26342).
SET @CRUSHCOG := 42291;
SET @PIBCREDIT := 42796;
SET @SIRENSPELL := 84152;
SET @QITEMSPELL := 79781;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@CRUSHCOG;
DELETE FROM smart_scripts WHERE entryorguid=@CRUSHCOG;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(@CRUSHCOG, 0, 0, 0, 9, 0, 100, 0, 0, 8, 15000, 25000, 11, @SIRENSPELL, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog Sentry-Bot - Cast Wailing Siren on Close'),
(@CRUSHCOG, 0, 1, 2, 8, 0, 100, 0, @QITEMSPELL, 0, 0, 0, 33, @PIBCREDIT, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Crushcog Sentry-Bot - Give Credit on Spell Hit'),
(@CRUSHCOG, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog Sentry-Bot - Despawn on Spell Hit');
