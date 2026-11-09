-- Desolace: the rider leaves phase 170 whenever the Raging Vortex loses the passenger, not only when Calm the
-- Storm is used (dismount, vortex despawn or death). Safe to run twice.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35393 AND `source_type` = 0 AND `id` = 18;
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,
 `event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,
 `action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,
 `action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,
 `target_z`,`target_o`,`comment`)
VALUES
(35393,0,18,0,28,0,100,0,0,0,0,0,0,28,65141,0,0,0,0,0,7,0,0,0,0,0,0,0,
 'Raging Vortex - On Passenger Removed - Remove Aura Generic Phase Shift 1');
