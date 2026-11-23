-- Wetlands: the Dragonmaw Centurion (1036), Grunt (2102) and Scout (2103) and the four Dark Iron dwarves (1051 to 1054) had
-- no combat rows (the Centurion had SmartAI without rows, the others no AI), so they fought in melee only and never
-- spoke. Rows and speech are as in Legends of Azeroth 5.4.8: the Centurion's Defensive Stance and Improved Blocking, the
-- dwarves' Gift of Ragnaros, the Tunneler's Sunder Armor and Defensive Stance, the Demolitionist's Bomb, the
-- Saboteur's Sapper Explode at low health, the Grunt's Shield Slam and the Scout's Shoot, and the flee at low health. The
-- Centurion's speech exists already.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (1051, 1052, 1053, 1054, 2102, 2103) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (1036, 1051, 1052, 1053, 1054, 2102, 2103);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1036, 0, 0, 0, 4, 0, 15, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Centurion - On Aggro - Say Line 0 (No Repeat)'),
(1036, 0, 1, 0, 0, 0, 90, 0, 1000, 2000, 10000, 15000, 0, 11, 7164, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Centurion - In Combat - Cast ''Defensive Stance'' (No Repeat)'),
(1036, 0, 4, 0, 0, 0, 70, 0, 5200, 12800, 22600, 28600, 0, 11, 3419, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Centurion - In Combat - Cast ''Improved Blocking'' (No Repeat)'),
(1036, 0, 5, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Centurion - Between 0-15% Health - Flee For Assist (No Repeat)'),
(1051, 0, 0, 0, 4, 0, 15, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Dwarf - On Aggro - Say Line 0 (No Repeat)'),
(1051, 0, 1, 0, 0, 0, 100, 0, 2000, 8300, 18000, 34200, 0, 11, 7891, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Dwarf - In Combat - Cast ''Gift of Ragnaros'' (No Repeat)'),
(1051, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Dwarf - Between 0-15% Health - Flee For Assist (No Repeat)'),
(1052, 0, 0, 0, 0, 0, 100, 0, 2100, 7300, 18000, 26000, 0, 11, 7891, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Saboteur - In Combat - Cast ''Gift of Ragnaros'' (No Repeat)'),
(1052, 0, 1, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Saboteur - Between 0-15% Health - Say Line 0 (No Repeat)'),
(1052, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Saboteur - Between 0-15% Health - Say Line 1 (No Repeat)'),
(1052, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 11, 3204, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Saboteur - Between 0-15% Health - Cast ''Sapper Explode'' (No Repeat)'),
(1053, 0, 0, 0, 4, 0, 15, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Tunneler - On Aggro - Say Line 0 (No Repeat)'),
(1053, 0, 1, 0, 0, 0, 100, 0, 500, 1000, 1300, 5000, 0, 11, 7164, 33, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Tunneler - In Combat - Cast ''Defensive Stance'' (No Repeat)'),
(1053, 0, 2, 0, 0, 0, 100, 0, 2000, 8500, 18000, 22000, 0, 11, 7891, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Tunneler - In Combat - Cast ''Gift of Ragnaros'' (No Repeat)'),
(1053, 0, 3, 0, 0, 0, 100, 0, 3100, 12400, 8900, 15900, 0, 11, 11971, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Tunneler - In Combat - Cast ''Sunder Armor'' (No Repeat)'),
(1053, 0, 4, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Tunneler - Between 0-15% Health - Flee For Assist (No Repeat)'),
(1054, 0, 0, 0, 4, 0, 15, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Demolitionist - On Aggro - Say Line 0'),
(1054, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3600, 3800, 0, 11, 8858, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Demolitionist - In Combat CMC - Cast ''Bomb'''),
(1054, 0, 2, 0, 0, 0, 100, 0, 1200, 8300, 8200, 20100, 0, 11, 7891, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Demolitionist - In Combat - Cast ''Gift of Ragnaros'''),
(1054, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Iron Demolitionist - Between 0-15% Health - Flee For Assist (No Repeat)'),
(2102, 0, 0, 0, 4, 0, 15, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Grunt - On Aggro - Say Line 0'),
(2102, 0, 1, 0, 0, 0, 100, 0, 5200, 11100, 7300, 20900, 0, 11, 8242, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Grunt - In Combat - Cast ''Shield Slam'''),
(2102, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Grunt - Between 0-15% Health - Flee For Assist'),
(2103, 0, 0, 0, 4, 0, 15, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Scout - On Aggro - Say Line 0'),
(2103, 0, 1, 0, 0, 0, 100, 0, 0, 0, 2400, 3600, 0, 11, 6660, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Scout - In Combat CMC - Cast ''Shoot'''),
(2103, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dragonmaw Scout - Between 0-15% Health - Flee For Assist (No Repeat)');

DELETE FROM `creature_text` WHERE `entry` IN (1051, 1052, 1053, 1054, 2102, 2103);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(1051, 0, 0, 'Never cross a Dark Iron, $C.', 12, 0, 100, 0, 0, 0, ''),
(1051, 0, 1, 'Time to die, $C.', 12, 0, 100, 0, 0, 0, ''),
(1051, 0, 2, 'Feel the power of the Dark Iron Dwarves!', 12, 0, 100, 0, 0, 0, ''),
(1052, 0, 0, 'Wahehe! I''m taking you down with me!', 12, 7, 100, 0, 0, 0, ''),
(1052, 1, 0, '%s''s eyes glow red as he lights his dynamite and begins to cackle madly!', 16, 7, 100, 0, 0, 0, ''),
(1053, 0, 0, 'Never cross a Dark Iron, $C.', 12, 0, 100, 0, 0, 0, ''),
(1053, 0, 1, 'Time to die, $C.', 12, 0, 100, 0, 0, 0, ''),
(1053, 0, 2, 'Feel the power of the Dark Iron Dwarves!', 12, 0, 100, 0, 0, 0, ''),
(1054, 0, 0, 'Never cross a Dark Iron, $C.', 12, 0, 100, 0, 0, 0, ''),
(1054, 0, 1, 'Time to die, $C.', 12, 0, 100, 0, 0, 0, ''),
(1054, 0, 2, 'Feel the power of the Dark Iron Dwarves!', 12, 0, 100, 0, 0, 0, ''),
(2102, 0, 0, 'Your bones will break under my boot, $r!', 12, 1, 100, 0, 0, 0, ''),
(2102, 0, 1, 'Long live the Dragonmaw! Die you worthless $r!', 12, 1, 100, 0, 0, 0, ''),
(2102, 0, 2, 'For the Dragonmaw!', 12, 1, 100, 0, 0, 0, ''),
(2103, 0, 0, 'Your bones will break under my boot, $r!', 12, 1, 100, 0, 0, 0, ''),
(2103, 0, 1, 'Long live the Dragonmaw! Die you worthless $r!', 12, 1, 100, 0, 0, 0, ''),
(2103, 0, 2, 'For the Dragonmaw!', 12, 1, 100, 0, 0, 0, '');
