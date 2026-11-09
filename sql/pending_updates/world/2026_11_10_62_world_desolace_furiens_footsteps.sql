-- Desolace: Furien's Footsteps (14191) and Elune's Gifts (14354). The three ruin bunnies cast nothing, and two of
-- the three ruins had a bunny of the wrong entry, so the visits could not be credited. Safe to run twice.
-- The three bunnies stand at the Handmaiden (35363), the Brazier (35367) and the Cup of Elune (35366).
UPDATE `creature` SET `id` = 35367 WHERE `guid` = 8288624;
UPDATE `creature` SET `id` = 35366 WHERE `guid` = 8288625;

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (35363,35366,35367);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (35363,35366,35367) AND `source_type` = 0;
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,
 `event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,
 `action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,
 `action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,
 `target_z`,`target_o`,`comment`)
VALUES
(35363,0,0,0,1,0,100,0,1000,1000,5000,5000,0,11,67342,2,0,0,0,0,18,30,0,0,0,0,0,0,
 'Elune Cathedral Bunny - Out Of Combat - Cast Elune Cathedral Kill Credit On Players Within 30 Yards'),
(35366,0,0,0,1,0,100,0,1000,1000,5000,5000,0,11,67344,2,0,0,0,0,18,30,0,0,0,0,0,0,
 'Elune Sanctuary Bunny - Out Of Combat - Cast Elune Sanctuary Kill Credit On Players Within 30 Yards'),
(35367,0,0,0,1,0,100,0,1000,1000,5000,5000,0,11,67343,2,0,0,0,0,18,30,0,0,0,0,0,0,
 'Elune Temple Bunny - Out Of Combat - Cast Elune Temple Kill Credit On Players Within 30 Yards');
