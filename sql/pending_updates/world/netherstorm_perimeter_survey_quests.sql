-- Netherstorm: the ender of Against the Legion and the starter of Ethereum Prisoner I.D. Catalogue, and the credit
-- scripts of Building a Perimeter (Kirin'Var runes) and Surveying the Ruins.

-- Against the Legion is handed in to Exarch Orelis; Ethereum Prisoner I.D. Catalogue is given by the same NPC that takes it
DELETE FROM `creature_queststarter` WHERE (`id` = 20448 AND `quest` = 10972);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(20448, 10972);
DELETE FROM `creature_questender` WHERE (`id` = 19466 AND `quest` = 10404);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(19466, 10404);

-- Building a Perimeter
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (19866,19867,19868);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(19866,0,0,1,8,0,100,0,34646,0,60000,60000,0,33,19866,0,0,0,0,0,7,0,0,0,0,0,0,0,'Invis East KV Rune - On SpellHit Activate Kirin''Var Rune - Quest Credit'),
(19866,0,1,0,61,0,100,0,0,0,0,0,0,50,183948,50,0,0,0,0,1,0,0,0,0,0,0,0,'Invis East KV Rune - Linked To Id 0 - Summon Object Ward Effect'),
(19867,0,0,1,8,0,100,0,34646,0,60000,60000,0,33,19867,0,0,0,0,0,7,0,0,0,0,0,0,0,'Invis NE KV Rune - On SpellHit Activate Kirin''Var Rune - Quest Credit'),
(19867,0,1,0,61,0,100,0,0,0,0,0,0,50,183948,50,0,0,0,0,1,0,0,0,0,0,0,0,'Invis NE KV Rune - Linked To Id 0 - Summon Object Ward Effect'),
(19868,0,0,1,8,0,100,0,34646,0,60000,60000,0,33,19868,0,0,0,0,0,7,0,0,0,0,0,0,0,'Invis West KV Rune - On SpellHit Activate Kirin''Var Rune - Quest Credit'),
(19868,0,1,0,61,0,100,0,0,0,0,0,0,50,183948,50,0,0,0,0,1,0,0,0,0,0,0,0,'Invis West KV Rune - Linked To Id 0 - Summon Object Ward Effect');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (19866,19867,19868) AND `AIName` = '' AND `ScriptName` = '';

-- Surveying the Ruins
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (20473,20475,20476);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(20473,0,0,0,8,0,100,0,35246,0,0,0,0,33,20473,0,0,0,0,0,7,0,0,0,0,0,0,0,'Surveying Marker One - On Spellhit ''Place Surveying Marker'' - Quest Credit ''Surveying the Ruins'' (Phase 1) (No Repeat)'),
(20475,0,0,0,8,0,100,0,35246,0,0,0,0,33,20475,0,0,0,0,0,7,0,0,0,0,0,0,0,'Surveying Marker Two - On Spellhit ''Place Surveying Marker'' - Quest Credit ''Surveying the Ruins'' (Phase 1) (No Repeat)'),
(20476,0,0,0,8,0,100,0,35246,0,0,0,0,33,20476,0,0,0,0,0,7,0,0,0,0,0,0,0,'Surveying Marker Three - On Spellhit ''Place Surveying Marker'' - Quest Credit ''Surveying the Ruins'' (Phase 1) (No Repeat)');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (20473,20475,20476) AND `AIName` = '' AND `ScriptName` = '';
