-- Hellfire Peninsula: quest credit scripts that were missing (Zeth'Gor Must Burn!, the Nether Modulator mission).

-- Quest credit scripts
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (19291,19292,20813,20814,20815,20816);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(19291,0,0,0,8,0,100,0,33655,0,0,0,0,33,19291,0,0,0,0,0,7,0,0,0,0,0,0,0,'Legion Transporter: Alpha - On Spellhit ''Dropping The Nether Modulator'' - Quest Credit ''Mission: Gateways Murketh and Shaadraz'' (No Repeat)'),
(19292,0,0,0,8,0,100,0,33655,0,0,0,0,33,19292,0,0,0,0,0,7,0,0,0,0,0,0,0,'Legion Transporter: Beta - On Spellhit ''Dropping The Nether Modulator'' - Quest Credit ''Mission: Gateways Murketh and Shaadraz'' (No Repeat)'),
(20813,0,0,0,8,0,100,0,35724,0,0,0,0,33,20813,0,0,0,0,0,7,0,0,0,0,0,0,0,'Zeth''Gor Quest Credit Marker, Barracks - On Spellhit ''Throw Torch'' - Quest Credit ''Zeth''Gor Must Burn!'' (No Repeat)'),
(20814,0,0,0,8,0,100,0,35724,0,0,0,0,33,20814,0,0,0,0,0,7,0,0,0,0,0,0,0,'Zeth''Gor Quest Credit Marker, Stable - On Spellhit ''Throw Torch'' - Quest Credit ''Zeth''Gor Must Burn!'' (No Repeat)'),
(20815,0,0,0,8,0,100,0,35724,0,0,0,0,33,20815,0,0,0,0,0,7,0,0,0,0,0,0,0,'Zeth''Gor Quest Credit Marker, East Hovel - On Spellhit ''Throw Torch'' - Quest Credit ''Zeth''Gor Must Burn!'' (No Repeat)'),
(20816,0,0,0,8,0,100,0,35724,0,0,0,0,33,20816,0,0,0,0,0,7,0,0,0,0,0,0,0,'Zeth''Gor Quest Credit Marker, West Hovel - On Spellhit ''Throw Torch'' - Quest Credit ''Zeth''Gor Must Burn!'' (No Repeat)');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (19291,19292,20813,20814,20815,20816) AND `AIName` = '' AND `ScriptName` = '';
