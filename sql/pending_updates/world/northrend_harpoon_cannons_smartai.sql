-- 7th Legion Chain Gun (27714) and Jotunheim Rapid-Fire Harpoon (30337) are vehicle turrets bound to npc_generic_harpoon_cannon,
-- a script that was removed from the core when upstream moved the Valkyrion harpoon (30066) to SmartAI, which roots the turret
-- when it spawns. The same two rows root these two (event 11 on respawn, event 37 when the AI starts).
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` IN (27714, 30337);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (27714, 30337);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(27714, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '7th Legion Chain Gun - On AI init - Root'),
(27714, 0, 1, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '7th Legion Chain Gun - On respawn - Root'),
(30337, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jotunheim Rapid-Fire Harpoon - On AI init - Root'),
(30337, 0, 1, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jotunheim Rapid-Fire Harpoon - On respawn - Root');
