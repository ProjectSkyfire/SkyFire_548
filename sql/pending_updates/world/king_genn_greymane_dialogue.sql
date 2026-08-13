DELETE FROM creature_text WHERE entry=35112;
INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(35112, 0, 0, 'Tell me, Godfrey.  Those that stayed in Gilneas City so that we could live.  Were they following protocol?', 0, 0, 100, 0, 0, 0, 'King Genn Greymane - ambient reflection'),
(35112, 1, 0, 'Fire!', 1, 0, 100, 0, 0, 0, 'King Genn Greymane - Old Divisions battle'),
(35112, 1, 1, 'Hold the barricades! Do not give them an inch!', 1, 0, 100, 0, 0, 0, 'King Genn Greymane - Old Divisions battle'),
(35112, 1, 2, 'Keep them back!', 1, 0, 100, 0, 0, 0, 'King Genn Greymane - Old Divisions battle'),
(35112, 1, 3, 'Enough! I\'ve made up my mind. We need Crowley on our side.', 0, 0, 100, 0, 0, 0, 'King Genn Greymane - Old Divisions battle');

UPDATE creature_template SET AIName='SmartAI' WHERE entry=35112;

DELETE FROM smart_scripts WHERE entryorguid=35112 AND source_type=0;
INSERT INTO smart_scripts
    (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags,
     event_param1, event_param2, event_param3, event_param4, event_param5,
     action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6,
     target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment)
VALUES
    (35112, 0, 0, 0, 1, 0, 100, 0,
     10000, 20000, 120000, 180000, 0,
     1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 'King Genn Greymane - periodic ambient line (group 0)');
-- Timers: 10-20s initial delay, then repeats every 2-3 minutes.
