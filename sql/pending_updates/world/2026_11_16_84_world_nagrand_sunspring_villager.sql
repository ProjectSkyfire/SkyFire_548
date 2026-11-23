-- Nagrand: Stopping the Spread (9874) needs a credit when Liquid Fire hits a Sunspring Villager. The villager was bound
-- to the script npc_sunspring_villager, which does not exist, so the spell gave no credit. The 3.3.5 rows also cast
-- Ignite Corpse (32147), which is not in the 5.4.8 client data, so the corpse only despawns.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 18240;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 18240;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(18240, 0, 0, 1, 8, 0, 100, 1, 32146, 0, 0, 0, 0, 33, 18240, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sunspring Villager - On Spellhit ''Liquid Fire'' - Quest Credit ''Stopping the Spread'' (No Repeat)'),
(18240, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 7000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunspring Villager - On Spellhit ''Liquid Fire'' - Delayed Despawn');
