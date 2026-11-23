-- The Corpsebeasts (27530) cannot be finished. The Argent Lightwell that the charm summons (45921) had no script, so
-- it never started its Lightwell's Glow barrage and was not rooted, and the three Corpsebeast entries ignored the
-- barrage. A Corpsebeast hit ten times by the glow of nearby lightwells now gives the Holy Overload kill credit
-- and goes away, and the lightwells that hit it vanish with it.
-- Rows are as in Legends of Azeroth 5.4.8; its counter action number (63) is this core's 116. The lightwell also
-- takes the source's flags (cannot move, immune to creatures).

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (45886, 45893, 45895, 45921) AND `ScriptName` = '';
UPDATE `creature_template` SET `unit_flags` = 516, `flags_extra` = 2 WHERE `entry` = 45921 AND `unit_flags` = 0 AND `flags_extra` = 0;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (45886, 45893, 45895, 45921) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45886, 0, 0, 0, 8, 0, 100, 0, 85593, 0, 0, 0, 0, 116, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - On Spellhit Light Barrage - Add Counter'),
(45886, 0, 1, 2, 77, 0, 100, 0, 1, 10, 0, 0, 0, 11, 85594, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - On Counter 10 - Cast Holy Overload'),
(45886, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - Linked To Id 1 - Despawn'),
(45893, 0, 0, 0, 8, 0, 100, 0, 85593, 0, 0, 0, 0, 116, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - On Spellhit Light Barrage - Add Counter'),
(45893, 0, 1, 2, 77, 0, 100, 0, 1, 10, 0, 0, 0, 11, 85594, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - On Counter 10 - Cast Holy Overload'),
(45893, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - Linked To Id 1 - Despawn'),
(45895, 0, 0, 0, 8, 0, 100, 0, 85593, 0, 0, 0, 0, 116, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - On Spellhit Light Barrage - Add Counter'),
(45895, 0, 1, 2, 77, 0, 100, 0, 1, 10, 0, 0, 0, 11, 85594, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - On Counter 10 - Cast Holy Overload'),
(45895, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corpsebeast - Linked To Id 1 - Despawn'),
(45921, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Argent Lightwell - On Summon - Set Root'),
(45921, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 85591, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Argent Lightwell - Linked To Id 0 - Cast Lightwell''s Glow'),
(45921, 0, 2, 3, 8, 0, 100, 0, 85594, 0, 0, 0, 0, 103, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Argent Lightwell - On Spellhit Holy Overload - Remove Root'),
(45921, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Argent Lightwell - Linked To Id 2 - Die');
