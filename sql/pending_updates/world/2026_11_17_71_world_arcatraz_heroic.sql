-- Heroic fixes for The Arcatraz. Six trash creatures had no AI at all, so neither the normal nor the heroic abilities were
-- cast; the rows are the TrinityCore 3.3.5 ones with their normal and heroic variants. Hell Rain was missing from the
-- Sargeron Hellcaller, the Unbound Devastator cast Transporter Malfunction instead of Deafening Roar in normal mode, and
-- three heroic templates had levels above the retail ones.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (20859, 20864, 20879, 20883, 20898, 20901) AND `AIName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (20859, 20864, 20879, 20883, 20898, 20901);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` BETWEEN 2090100 AND 2090105;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(20859, 0, 0, 0, 1, 0, 100, 0, 0, 20000, 5000, 20000, 0, 11, 36327, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arcatraz Warder - Out of Combat - Cast ''Shoot Arcane Explosion Arrow'''),
(20859, 0, 1, 0, 1, 0, 100, 0, 20000, 25000, 20000, 25000, 0, 11, 36293, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arcatraz Warder - Out of Combat - Cast ''Arcane Shot'''),
(20859, 0, 2, 0, 0, 0, 100, 4, 0, 0, 2000, 3000, 0, 11, 15620, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arcatraz Warder - In Combat CMC - Cast ''Shoot'' (Normal Dungeon)'),
(20859, 0, 3, 0, 0, 0, 100, 8, 0, 0, 2000, 3000, 0, 11, 22907, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arcatraz Warder - In Combat CMC - Cast ''Shoot'' (Heroic Dungeon)'),
(20859, 0, 4, 0, 0, 0, 100, 4, 5000, 10000, 7000, 10000, 0, 11, 36609, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arcatraz Warder - In Combat CMC - Cast ''Arcane Shoot'' (Normal Dungeon)'),
(20859, 0, 5, 0, 0, 0, 100, 8, 5000, 10000, 7000, 10000, 0, 11, 38807, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arcatraz Warder - In Combat CMC - Cast ''Arcane Shoot'' (Heroic Dungeon)'),
(20864, 0, 0, 0, 0, 0, 100, 4, 16900, 17600, 19300, 26500, 0, 11, 36617, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Protean Nightmare - In Combat - Cast ''Gaping Maw'' (Normal Dungeon)'),
(20864, 0, 1, 0, 0, 0, 100, 8, 16900, 17600, 19300, 26500, 0, 11, 38810, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Protean Nightmare - In Combat - Cast ''Gaping Maw'' (Heroic Dungeon)'),
(20864, 0, 2, 0, 0, 0, 100, 4, 22900, 26300, 10800, 22900, 0, 11, 36619, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Protean Nightmare - In Combat - Cast ''Infectious Poison (Normal Dungeon)'),
(20864, 0, 3, 0, 0, 0, 100, 8, 22900, 26300, 10800, 22900, 0, 11, 38811, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Protean Nightmare - In Combat - Cast ''Infectious Poison (Heroic Dungeon)'),
(20864, 0, 4, 0, 0, 0, 100, 0, 15200, 21700, 10900, 22900, 0, 11, 36622, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Protean Nightmare - In Combat - Cast ''Incubation'''),
(20879, 0, 0, 0, 0, 0, 100, 4, 9000, 12000, 15000, 17000, 0, 11, 36786, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Eredar Soul-Eater - In Combat - Cast ''Soul Chill'' (Normal Dungeon)'),
(20879, 0, 1, 0, 0, 0, 100, 8, 9000, 12000, 15000, 17000, 0, 11, 38843, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Eredar Soul-Eater - In Combat - Cast ''Soul Chill'' (Heroic Dungeon)'),
(20879, 0, 2, 0, 0, 0, 100, 0, 15000, 17000, 14000, 21000, 0, 11, 36778, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Eredar Soul-Eater - In Combat - Cast ''Soul Steal'''),
(20879, 0, 3, 0, 31, 0, 100, 0, 36778, 0, 0, 0, 0, 11, 36782, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Eredar Soul-Eater - On Target Spellhit ''Soul Steal'' - Cast ''Soul Steal'''),
(20883, 0, 0, 0, 0, 0, 100, 0, 10000, 15000, 17000, 25000, 0, 11, 36866, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Spiteful Temptress - In Combat - Cast ''Domination'''),
(20883, 0, 1, 0, 0, 0, 100, 0, 6000, 11000, 14000, 17000, 0, 11, 36886, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Spiteful Temptress - In Combat - Cast ''Spiteful Fury'''),
(20883, 0, 2, 0, 0, 0, 100, 4, 8000, 15000, 10000, 15000, 0, 11, 36868, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spiteful Temptress - In Combat - Cast ''Shadow Bolt'' (Normal Dungeon)'),
(20883, 0, 3, 0, 0, 0, 100, 8, 8000, 15000, 10000, 15000, 0, 11, 38892, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spiteful Temptress - In Combat - Cast ''Shadow Bolt'' (Heroic Dungeon)'),
(20898, 0, 0, 0, 0, 0, 100, 4, 4000, 6000, 10000, 12000, 0, 11, 38855, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gargantuan Abyssal - In Combat - Cast ''Fire Shield'' (Normal Dungeon)'),
(20898, 0, 1, 0, 0, 0, 100, 8, 4000, 6000, 10000, 12000, 0, 11, 38901, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gargantuan Abyssal - In Combat - Cast ''Fire Shield'' (Heroic Dungeon)'),
(20898, 0, 2, 0, 0, 0, 100, 4, 7000, 9000, 12000, 17000, 0, 11, 36837, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Gargantuan Abyssal - In Combat - Cast ''Meteor'' (Normal Dungeon)'),
(20898, 0, 3, 0, 0, 0, 100, 8, 7000, 9000, 12000, 17000, 0, 11, 38903, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Gargantuan Abyssal - In Combat - Cast ''Meteor'' (Heroic Dungeon)'),
(20901, 0, 0, 0, 0, 0, 100, 4, 0, 0, 2400, 4800, 0, 11, 22907, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - In Combat CMC - Cast ''Shoot'' (Normal Dungeon)'),
(20901, 0, 1, 0, 0, 0, 100, 8, 0, 0, 2400, 4800, 0, 11, 38940, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - In Combat CMC - Cast ''Shoot'' (Heroic Dungeon)'),
(20901, 0, 2, 0, 0, 0, 100, 4, 12000, 18000, 15000, 20000, 0, 11, 36827, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - In Combat - Cast ''Hooked Net'' (Normal Dungeon)'),
(20901, 0, 3, 0, 0, 0, 100, 8, 12000, 18000, 15000, 20000, 0, 11, 38912, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - In Combat - Cast ''Hooked Net'' (Heroic Dungeon)'),
(20901, 0, 4, 0, 0, 0, 100, 4, 12000, 15000, 17000, 23000, 0, 87, 2090100, 2090102, 2090104, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - In Combat - Run Random Script (Normal Dungeon)'),
(20901, 0, 5, 0, 0, 0, 100, 8, 12000, 15000, 17000, 23000, 0, 87, 2090101, 2090103, 2090105, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - In Combat - Run Random Script (Heroic Dungeon)'),
(20901, 0, 6, 0, 0, 0, 100, 0, 13000, 16000, 18000, 22000, 0, 11, 23601, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - In Combat - Cast ''Scatter Shot'''),
(20901, 0, 7, 0, 2, 0, 100, 0, 0, 30, 23000, 30000, 0, 11, 36828, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - Between 0-30% Health - Cast ''Rapid Fire'''),
(2090100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 35964, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - On Script - Cast ''Frost Arrow'' (Normal Dungeon)'),
(2090101, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 38942, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - On Script - Cast ''Frost Arrow'' (Heroic Dungeon)'),
(2090102, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 35932, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - On Script - Cast ''Immolation Arrow'' (Normal Dungeon)'),
(2090103, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 38943, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - On Script - Cast ''Immolation Arrow'' (Heroic Dungeon)'),
(2090104, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 36984, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - On Script - Cast ''Serpent Sting'' (Normal Dungeon)'),
(2090105, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 38914, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Archer - On Script - Cast ''Serpent Sting'' (Heroic Dungeon)');

UPDATE `smart_scripts` SET `action_param1` = 36887 WHERE `entryorguid` = 20881 AND `source_type` = 0 AND `id` = 0 AND `action_param1` = 36897;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 20902 AND `source_type` = 0 AND `id` IN (3, 4);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(20902, 0, 3, 0, 0, 0, 100, 4, 7000, 10000, 13000, 20000, 0, 11, 36829, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Hellcaller - In Combat - Cast ''Hell Rain'' (Normal Dungeon)'),
(20902, 0, 4, 0, 0, 0, 100, 8, 7000, 10000, 13000, 20000, 0, 11, 38917, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Sargeron Hellcaller - In Combat - Cast ''Hell Rain'' (Heroic Dungeon)');

UPDATE `creature_template` SET `minlevel` = 68, `maxlevel` = 69 WHERE `entry` = 21585; -- Arcatraz Defender heroic, was 70-71
UPDATE `creature_template` SET `minlevel` = 68, `maxlevel` = 69 WHERE `entry` = 21587; -- Arcatraz Warder heroic, was 70-71
UPDATE `creature_template` SET `minlevel` = 68, `maxlevel` = 69 WHERE `entry` = 21607; -- Protean Horror heroic, was 70-70
