-- Azshara: quest credits of Profitability Scouting and Beating the Market.
-- Based on Legends of Azeroth 5.4.8.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37143;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (37143);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37143,0,0,0,10,0,100,0,1,40,0,0,0,15,24452,0,0,0,0,0,21,40,0,0,0,0,0,0,'Heart of Arkkoroc Aura Bunny - OOC LoS - Quest Credit');

DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=42777 AND `id`>=2;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42777,0,2,0,64,0,100,0,0,0,0,0,0,2,14,0,0,0,0,0,1,0,0,0,0,0,0,0,'Bilgewater Foreman - On Gossip Hello - Set Faction Hostile'),
(42777,0,3,0,6,0,100,0,0,0,0,0,0,33,42798,0,0,0,0,0,1,0,0,0,0,0,0,0,'Bilgewater Foreman - On Just Died - Quest Credit (Bilgewater Foreman Quest Tracker)'),
(42777,0,4,0,6,0,100,0,0,0,0,0,0,70,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Bilgewater Foreman - On Just Died - Respawn Self');
