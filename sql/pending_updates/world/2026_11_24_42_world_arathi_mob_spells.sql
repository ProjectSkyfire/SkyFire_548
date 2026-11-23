-- Arathi Highlands: combat spells of the kill targets of Call to Arms (26027) and Drowned Sorrows (26055) and other mobs.
-- The creatures have the SmartAI name but no rows, so they only melee. Added as in Legends of Azeroth 5.4.8: the
-- Boulderfist Shaman (Lightning Bolt, Healing Wave, a line on aggro), the Daggerspine Sorceress (Lightning Bolt), the
-- Drywhisker Surveyor (Frost Armor, Frostbolt), the Drywhisker Digger (Defensive Stance) and the Cresting Exile (Frost
-- Armor), all fleeing for help at low health.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 2570 AND `source_type` = 0 AND `id` IN (0, 1, 3);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2573 AND `source_type` = 0 AND `id` IN (0, 1, 3);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2574 AND `source_type` = 0 AND `id` IN (0, 2);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2596 AND `source_type` = 0 AND `id` IN (0, 2);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2761 AND `source_type` = 0 AND `id` IN (0);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2570, 0, 0, 0, 4, 0, 15, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderfist Shaman - On Aggro - Say Line 0'),
(2570, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3500, 6400, 0, 11, 9532, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Boulderfist Shaman - In Combat CMC - Cast ''Lightning Bolt'''),
(2570, 0, 3, 0, 2, 0, 100, 1, 0, 40, 0, 0, 0, 11, 11986, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderfist Shaman - Between 0-45% Health - Cast ''Healing Wave'' (No Repeat)'),
(2573, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 1800000, 1800000, 0, 11, 12544, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Drywhisker Surveyor - Out of Combat - Cast ''Frost Armor'''),
(2573, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3500, 6400, 0, 11, 20822, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Drywhisker Surveyor - In Combat CMC - Cast ''Frostbolt'''),
(2573, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Drywhisker Surveyor - Between 0-15% Health - Flee For Assist (No Repeat)'),
(2574, 0, 0, 0, 1, 0, 100, 0, 2500, 6100, 179800, 180400, 0, 11, 7164, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Drywhisker Digger - Out of Combat - Cast ''Defensive Stance'' (No Repeat)'),
(2574, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drywhisker Digger - Between 0-15% Health - Flee For Assist (No Repeat)'),
(2596, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3400, 4700, 0, 11, 20824, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Daggerspine Sorceress - In Combat CMC - Cast ''Lightning Bolt'''),
(2596, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Daggerspine Sorceress - Between 0-15% Health - Flee For Assist (No Repeat)'),
(2761, 0, 0, 0, 1, 0, 100, 1, 1000, 1000, 1800000, 1800000, 0, 11, 12544, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cresting Exile - Out of Combat - Cast ''Frost Armor''');
