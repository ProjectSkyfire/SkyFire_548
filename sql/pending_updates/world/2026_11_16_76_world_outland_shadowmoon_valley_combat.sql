-- Shadowmoon Valley: combat SmartAI for 1 creatures that had no spells and no AI here. The rows are the 5.4.8 ones; the
-- spells they cast agree with the 3.3.5 SmartAI of the same creatures.

-- Shadowmoon Valley combat abilities
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (21207);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(21207,0,0,1,1,0,100,0,0,0,5000,5000,0,11,36382,0,0,0,0,0,1,0,0,0,0,0,0,0,'Summoner Skartax - On Spawn - Cast Skartax Self Aura I'),
(21207,0,1,0,61,0,100,0,0,0,0,0,0,11,36431,0,0,0,0,0,1,0,0,0,0,0,0,0,'Summoner Skartax - On Spawn - Cast Test Channel'),
(21207,0,2,0,0,0,100,0,0,0,3000,5000,0,11,12471,64,0,0,0,0,2,0,0,0,0,0,0,0,'Summoner Skartax - IC - Cast Shadowbolt'),
(21207,0,3,0,0,0,100,0,0,0,2500,4500,0,11,38401,64,0,0,0,0,2,0,0,0,0,0,0,0,'Summoner Skartax - IC - Cast Incinerate');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (21207) AND `AIName` = '' AND `ScriptName` = '';
