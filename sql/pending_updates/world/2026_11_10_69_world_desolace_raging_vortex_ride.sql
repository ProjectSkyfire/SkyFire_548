-- Desolace: Rider on the Storm (14198). The cast of Generic Phase Shift 1 by the rider failed with an incorrect area
-- result, because spells with `spell_area` rows can only be cast in those areas, and Calm the Storm and the exit
-- button did not end the ride in the same way. Safe to run twice.
-- Allow the spell in Desolace, it is only cast by the vortex.
DELETE FROM `spell_area` WHERE `spell` = 65141 AND `area` = 405;
INSERT INTO `spell_area`
(`spell`,`area`,`quest_start`,`quest_end`,`aura_spell`,`racemask`,`gender`,`autocast`,`quest_start_status`,`quest_end_status`)
VALUES
(65141,405,0,0,0,0,2,0,64,11);

-- Every end of the ride goes through the passenger leaving the vortex: Calm the Storm only removes the ride aura.
-- The rider loses the phase at once, is taken to Furien's Post a moment later and the vortex despawns.
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 35393 AND `source_type` = 0 AND `id` = 14;
UPDATE `smart_scripts` SET `comment` = 'Raging Vortex - On SpellHit Calm the Storm - Remove Aura Ride Raging Vortex'
WHERE `entryorguid` = 35393 AND `source_type` = 0 AND `id` = 14;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35393 AND `source_type` = 0 AND `id` IN (15,16,17,18,19);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3539301 AND `source_type` = 9;
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,
 `event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,
 `action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,
 `action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,
 `target_z`,`target_o`,`comment`)
VALUES
(35393,0,18,19,28,0,100,0,0,0,0,0,0,28,65141,0,0,0,0,0,7,0,0,0,0,0,0,0,
 'Raging Vortex - On Passenger Removed - Remove Aura Generic Phase Shift 1'),
(35393,0,19,0,61,0,100,0,0,0,0,0,0,80,3539301,2,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 18 - Run Script'),
(3539301,9,0,0,0,0,100,0,1000,1000,0,0,0,62,1,0,0,0,0,0,12,1,0,0,-444.859,2230.79,89.739,6.03501,
 'Raging Vortex - On Script - Teleport Rider'),
(3539301,9,1,0,0,0,100,0,500,500,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - On Script - Despawn');
