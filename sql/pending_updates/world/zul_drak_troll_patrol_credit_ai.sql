-- Zul'Drak: quest credit given by a spell hit on creatures that had no SmartAI here.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (28064,28304,28305,28352) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (28064,28304,28305,28352);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(28064, 0, 0, 1, 8, 0, 100, 0, 51247, 0, 0, 0, 0, 33, 28064, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Pedestal 01 - On SpellHit Place Banner - Quest Credit'),
(28064, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 51249, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Pedestal 01 - Linked To Id 0 - Cast Summon Agent Crusade Banner'),
(28304, 0, 0, 1, 8, 0, 100, 0, 51247, 0, 0, 0, 0, 33, 28304, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Pedestal 02 - On SpellHit Place Banner - Quest Credit'),
(28304, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 51249, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Pedestal 02 - Linked To Id 0 - Cast Summon Agent Crusade Banner'),
(28305, 0, 0, 1, 8, 0, 100, 0, 51247, 0, 0, 0, 0, 33, 28305, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Pedestal 03 - On SpellHit Place Banner - Quest Credit'),
(28305, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 51249, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drakkari Pedestal 03 - Linked To Id 0 - Cast Summon Agent Crusade Banner'),
(28352, 0, 0, 0, 8, 0, 100, 0, 51381, 0, 180000, 180000, 0, 33, 28352, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Nethurbian Crater KC Bunny - On Spellhit ''Toss Grenade'' - Quest Credit ''Troll Patrol: Throwing Down''');
