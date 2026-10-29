-- Mana Wyrm (15274) gives quest 8346 credit when hit by any class's Arcane Torrent (4.3.4 rows plus the monk version).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 15274 AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `entryorguid` = 15274 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(15274, 0, 0, 0, 8, 0, 100, 0, 28730, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mana Wyrm - On Spellhit Arcane Torrent (mana) - Cast Quest Credit 8346'),
(15274, 0, 1, 0, 8, 0, 100, 0, 25046, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mana Wyrm - On Spellhit Arcane Torrent (energy) - Cast Quest Credit 8346'),
(15274, 0, 2, 0, 8, 0, 100, 0, 50613, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mana Wyrm - On Spellhit Arcane Torrent (runic power) - Cast Quest Credit 8346'),
(15274, 0, 3, 0, 0, 0, 100, 0, 4000, 4000, 10000, 15000, 0, 11, 25602, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Mana Wyrm - In Combat - Cast Faerie Fire'),
(15274, 0, 4, 0, 8, 0, 100, 0, 69179, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mana Wyrm - On Spellhit Arcane Torrent (rage) - Cast Quest Credit 8346'),
(15274, 0, 5, 0, 8, 0, 100, 0, 80483, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mana Wyrm - On Spellhit Arcane Torrent (focus) - Cast Quest Credit 8346'),
(15274, 0, 6, 0, 8, 0, 100, 0, 129597, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mana Wyrm - On Spellhit Arcane Torrent (chi) - Cast Quest Credit 8346');
