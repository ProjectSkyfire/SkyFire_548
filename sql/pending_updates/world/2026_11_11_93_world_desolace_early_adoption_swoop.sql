-- Early Adoption (14246): the Defiant Swoop script (35752) also ran on the four wild swoops, so one standing next to a
-- placed egg could credit it. Only the swoop the egg summons sets the event phase the egg check needs.
-- Safe to run twice.
UPDATE `smart_scripts` SET `event_phase_mask` = 1 WHERE `entryorguid` = 35752 AND `source_type` = 0 AND `id` = 2;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 35752 AND `source_type` = 0 AND `id` = 6;
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,
 `event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,
 `action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,
 `action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,
 `target_z`,`target_o`,`comment`)
VALUES
(35752,0,6,0,54,0,100,0,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Defiant Swoop - On Summoned - Set Event Phase 1');
