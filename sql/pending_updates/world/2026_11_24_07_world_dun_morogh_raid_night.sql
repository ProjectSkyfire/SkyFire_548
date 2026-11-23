-- It's Raid Night Every Night (25932): after the quest is accepted, Sergeant Flinthammer (41578) at Amberstill Ranch calls
-- Frostmane Raiders (41603) and Amberstill Mountaineers (41611) to defend the ranch with him, and the first raider that dies
-- credits the "Defend Amberstill Ranch" objective (42169) to the players around. Flinthammer had no script and the Raider was a
-- level 1 friendly placeholder, so the event never started (upstream issue 1415). The scripts of Flinthammer and the Raider's
-- death, the Raider's level and faction, and the Mountaineers' despawn are as in Legends of Azeroth 5.4.8; the Raider's
-- Throw Javelin and Net are the rows of the other Frostmane Raider (41544), whose spell list is the same, and its damage
-- values are that raider's.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 41578 AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 8, `maxlevel` = 9, `faction_A` = 2264, `faction_H` = 2264, `mindmg` = 16, `maxdmg` = 33, `attackpower` = 28, `baseattacktime` = 2000, `rangeattacktime` = 2000, `AIName` = 'SmartAI' WHERE `entry` = 41603 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (41578);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41578, 0, 0, 0, 19, 0, 100, 0, 25932, 0, 0, 0, 0, 80, 4157800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sergeant Flinthammer - On Quest Accept - Run Script'),
(41578, 0, 1, 0, 38, 0, 100, 0, 1, 0, 0, 0, 0, 33, 42169, 0, 0, 0, 0, 0, 17, 0, 50, 0, 0, 0, 0, 0, 'Sergeant Flinthammer - On Data Set - Give Quest Credit');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41603;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41603, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 9, 41578, 0, 50, 0, 0, 0, 0, 'Frostmane Raider - On Death - Set Data On Sergeant Flinthammer'),
(41603, 0, 1, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 77663, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Raider - In Combat - Cast ''Throw Javelin'''),
(41603, 0, 2, 0, 9, 0, 100, 0, 0, 20, 15000, 16000, 0, 11, 31290, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Frostmane Raider - Within 0-20 Range - Cast ''Net''');

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (4157800);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4157800, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 12, 41603, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5554.65, -1313.99, 398.673, 0, 'Sergeant Flinthammer - On Script - Summon Frostmane Raider'),
(4157800, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41603, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5554.62, -1308.67, 398.539, 0, 'Sergeant Flinthammer - On Script - Summon Frostmane Raider'),
(4157800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41603, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5551.63, -1303.61, 398.613, 0, 'Sergeant Flinthammer - On Script - Summon Frostmane Raider'),
(4157800, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41603, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5548.73, -1301.51, 399.473, 0, 'Sergeant Flinthammer - On Script - Summon Frostmane Raider'),
(4157800, 9, 4, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 41611, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5546.29, -1304.64, 399.016, 0, 'Sergeant Flinthammer - On Script - Summon Amberstill Mountaineer'),
(4157800, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41611, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5547.21, -1309.21, 398.072, 0, 'Sergeant Flinthammer - On Script - Summon Amberstill Mountaineer'),
(4157800, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41611, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5549.98, -1314.47, 398.508, 0, 'Sergeant Flinthammer - On Script - Summon Amberstill Mountaineer'),
(4157800, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41611, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5553.76, -1316.54, 398.663, 0, 'Sergeant Flinthammer - On Script - Summon Amberstill Mountaineer');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41611 AND `id` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41611, 0, 2, 0, 21, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Amberstill Mountaineer - On Reached Home - Despawn');
