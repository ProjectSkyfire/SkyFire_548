-- Grizzly Hills: quest credit given by a spell hit or by sight on creatures that had no SmartAI here.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (26261,26468,26469,26470,26855,26856,26857) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (26261,26468,26469,26470,26855,26856,26857);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(26261, 0, 0, 0, 8, 0, 100, 0, 47394, 0, 60000, 60000, 0, 33, 26261, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grizzly Hills Giant - On SpellHit - Kill Credit'),
(26468, 0, 0, 0, 10, 0, 100, 0, 1, 10, 2000, 2000, 0, 33, 26468, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'North Building - Within 1-10 Range Out of Combat LoS - Quest Credit ''Uncovering the Tunnels'' (Phase 1)'),
(26469, 0, 0, 0, 10, 0, 100, 0, 1, 10, 2000, 2000, 0, 33, 26469, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'South Building - Within 1-10 Range Out of Combat LoS - Quest Credit ''Uncovering the Tunnels'' (Phase 1)'),
(26470, 0, 0, 0, 10, 0, 100, 0, 1, 10, 2000, 2000, 0, 33, 26470, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'East Building - Within 1-10 Range Out of Combat LoS - Quest Credit ''Uncovering the Tunnels'' (Phase 1)'),
(26855, 0, 0, 0, 8, 0, 100, 0, 47542, 0, 0, 0, 0, 33, 26855, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'East Stone - On Spell Hit - Give Quest Credit'),
(26856, 0, 0, 0, 8, 0, 100, 0, 47542, 0, 0, 0, 0, 33, 26856, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'North Stone - On Spell Hit - Give Quest Credit'),
(26857, 0, 0, 0, 8, 0, 100, 0, 47542, 0, 0, 0, 0, 33, 26857, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'South Stone - On Spell Hit - Give Quest Credit');
