-- Zul'Drak: quest credit given by a link or a spell hit on creatures that had no SmartAI here.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (28029,28404,28719) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (28029,28404,28719);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(28029, 0, 0, 0, 9, 0, 100, 0, 0, 5, 5000, 8000, 0, 11, 14517, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Argent Crusader - Within 0-5 Range - Cast ''Crusader Strike'' (No Repeat)'),
(28029, 0, 1, 0, 0, 0, 100, 0, 7000, 11000, 20000, 24000, 0, 11, 10326, 1, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Argent Crusader - In Combat - Cast ''Turn Evil'' (No Repeat)'),
(28029, 0, 2, 3, 8, 0, 100, 1, 53031, 0, 0, 0, 0, 11, 53039, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Argent Crusader - On Spellhit ''Crusader Parachute'' - Cast ''Deploy Parachute'' (No Repeat)'),
(28029, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 29060, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Argent Crusader - On Spellhit ''Crusader Parachute'' - Quest Credit ''Parachutes for the Argent Crusade'' (No Repeat)'),
(28404, 0, 0, 0, 0, 0, 100, 0, 8000, 12000, 8000, 12000, 0, 11, 54604, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cursed Offspring of Har''koa - In Combat - Cast ''Curse of Blood'''),
(28404, 0, 1, 0, 8, 0, 100, 1, 51895, 0, 0, 0, 0, 11, 51891, 7, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cursed Offspring of Har''koa - On Spellhit ''Whisker of Har''koa'' - Cast ''But First My Offspring: Summon Released Offspring of Har''koa'''),
(28404, 0, 2, 0, 8, 0, 100, 1, 51895, 0, 0, 0, 0, 33, 28526, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cursed Offspring of Har''koa - On Spellhit ''Whisker of Har''koa'' - Quest Credit ''But First My Offspring'''),
(28404, 0, 3, 0, 8, 0, 100, 1, 51895, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cursed Offspring of Har''koa - On Spellhit ''Whisker of Har''koa'' - Despawn In 1000 ms'),
(28719, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 11, 52222, 64, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Soul Font Void Zone - On Respawn - Cast Soul Font Void'),
(28719, 0, 1, 0, 8, 0, 100, 0, 52242, 0, 0, 0, 0, 33, 28713, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Soul Font Void Zone - On SpellHit Soul Font Void Channel - Quest Credit');
