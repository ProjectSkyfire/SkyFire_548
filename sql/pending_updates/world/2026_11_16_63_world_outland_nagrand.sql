-- Nagrand: the credit scripts of Standards and Practices (Burning Blade pyres) and Forge Camp: Annihilated (Fel
-- Cannon: Hate and Fear). Without them neither quest could be completed.

-- Standards and Practices
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (18305,18306,18307);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(18305,0,0,0,8,0,100,0,32205,0,0,0,0,33,18305,0,0,0,0,0,7,0,0,0,0,0,0,0,'Burning Blade Pyre (01) - On Spellhit ''Place Mag''har Battle Standard'' - Quest Credit ''Standards and Practices'' (Phase 1) (No Repeat)'),
(18306,0,0,0,8,0,100,0,32205,0,0,0,0,33,18306,0,0,0,0,0,7,0,0,0,0,0,0,0,'Burning Blade Pyre (02) - On Spellhit ''Place Mag''har Battle Standard'' - Quest Credit ''Standards and Practices'' (Phase 1) (No Repeat)'),
(18307,0,0,0,8,0,100,0,32205,0,0,0,0,33,18307,0,0,0,0,0,7,0,0,0,0,0,0,0,'Burning Blade Pyre (03) - On Spellhit ''Place Mag''har Battle Standard'' - Quest Credit ''Standards and Practices'' (Phase 1) (No Repeat)');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (18305,18306,18307) AND `AIName` = '' AND `ScriptName` = '';

-- Forge Camp: Annihilated
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (19067,19210);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(19067,0,0,0,8,0,100,0,33531,0,0,0,0,33,19067,0,0,0,0,0,16,0,0,0,0,0,0,0,'Fel Cannon: Hate - On Spellhit ''Fel Cannon: Hate'' - Quest Credit ''Forge Camp: Annihilated'''),
(19210,0,0,0,8,0,100,0,33532,0,0,0,0,33,19210,0,0,0,0,0,16,0,0,0,0,0,0,0,'Fel Cannon: Fear - On Spellhit ''Fel Cannon: Fear'' - Quest Credit ''Forge Camp: Annihilated'' (No Repeat)');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (19067,19210) AND `AIName` = '' AND `ScriptName` = '';
