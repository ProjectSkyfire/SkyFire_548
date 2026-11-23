-- Terokkar Forest: the Digging Through Bones escort (Chief Archaeologist Letoll), the Mana Bomb of The Final Code
-- and the Sha'tari Torch credits of An Improper Burial.

-- Digging Through Bones
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (22458);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (2245800,2245801);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(22458,0,0,1,19,0,100,0,10922,0,0,0,0,53,0,22458,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Quest ''Digging Through Bones'' Taken - Start Waypoint'),
(22458,0,1,0,61,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Aggro - Say Line 0'),
(22458,0,2,3,40,0,100,0,1,22458,0,0,0,54,10000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 1 Reached - Pause Waypoint'),
(22458,0,3,10,61,0,100,0,0,0,0,0,0,80,2245800,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 1 Reached - Run Script'),
(22458,0,4,5,40,0,100,0,8,22458,0,0,0,17,233,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 8 Reached - Set Emote State 233'),
(22458,0,5,0,61,0,100,0,0,0,0,0,0,45,0,2,0,0,0,0,10,78837,22464,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 8 Reached - Set Data 0 2'),
(22458,0,6,7,40,0,100,0,9,22458,0,0,0,17,467,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 9 Reached - Set Emote State 467'),
(22458,0,7,8,61,0,100,0,0,0,0,0,0,45,0,3,0,0,0,0,10,78837,22464,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 9 Reached - Set Data 0 3'),
(22458,0,8,0,61,0,100,0,0,0,0,0,0,80,2245801,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 9 Reached - Run Script'),
(22458,0,9,0,40,0,100,0,7,22458,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 7 Reached - Say Line 3'),
(22458,0,10,0,61,0,100,0,0,0,0,0,0,81,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 9 Reached - Set Npc Flag '),
(22458,0,11,0,40,0,100,0,7,22458,0,0,0,54,3000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Waypoint 7 Reached - Pause Waypoint'),
(2245800,9,0,0,0,0,100,0,3000,3000,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 1'),
(2245800,9,1,0,0,0,100,0,7000,7000,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 2'),
(2245800,9,2,0,0,0,100,0,500,500,0,0,0,45,0,1,0,0,0,0,10,78837,22464,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Set Data 0 1'),
(2245801,9,0,0,0,0,100,0,0,0,0,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 4'),
(2245801,9,1,0,0,0,100,0,2000,2000,0,0,0,1,5,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 5'),
(2245801,9,2,0,0,0,100,0,5000,5000,0,0,0,1,6,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 6'),
(2245801,9,3,0,0,0,100,0,0,0,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Set Emote State 0'),
(2245801,9,4,0,0,0,100,0,22000,22000,0,0,0,1,7,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 7'),
(2245801,9,5,0,0,0,100,0,34000,34000,0,0,0,1,8,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 8'),
(2245801,9,6,0,0,0,100,0,7000,7000,0,0,0,1,9,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 9'),
(2245801,9,7,0,0,0,100,0,500,500,0,0,0,11,39217,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Cast ''Fumper Trap'''),
(2245801,9,8,0,0,0,100,0,8000,8000,0,0,0,1,10,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 10'),
(2245801,9,9,0,0,0,100,0,8000,8000,0,0,0,1,11,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Say Line 11'),
(2245801,9,10,0,0,0,100,0,6000,6000,0,0,0,15,10922,0,0,0,0,0,18,20,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Quest Credit ''Digging Through Bones'''),
(2245801,9,11,0,0,0,100,0,3000,3000,0,0,0,81,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Set Npc Flag Questgiver'),
(2245801,9,12,0,0,0,100,0,1000,1000,0,0,0,78,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Reset All Scripts'),
(2245801,9,13,0,0,0,100,0,16000,16000,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Chief Archaeologist Letoll - On Script - Despawn Instant');
DELETE FROM `creature_text` WHERE `entry` IN (22458);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(22458,0,0,'Aright, listen up! Form a circle around me and move out!',12,0,100,0,0,0,''),
(22458,1,0,'Aright, $r, just keep us safe from harm while we work. We''ll pay you when we return.',12,0,100,0,0,0,''),
(22458,2,0,'The dig site is just north of here.',12,0,100,0,0,0,''),
(22458,3,0,'We''re here! Start diggin''!',12,0,100,0,0,0,''),
(22458,4,0,'I think there''s somethin'' buried here, beneath the sand!',12,0,100,396,0,0,''),
(22458,5,0,'Almost got it!',12,0,100,396,0,0,''),
(22458,6,0,'By Brann''s brittle bananas! What is it!? It... It looks like a drum.',12,0,100,396,0,0,''),
(22458,7,0,'Have ye gone mad? You expect me to leave behind a drum without first beatin'' on it? Not this son of Ironforge! No sir!',12,0,100,0,0,0,''),
(22458,8,0,'Shut yer yaps! I''m gonna bang on this drum and that''s that!',12,0,100,0,0,0,''),
(22458,9,0,'IN YOUR FACE! I told you there was somethin'' here!',12,0,100,0,0,0,''),
(22458,10,0,'Don''t just stand there! Help $g him:her; out!',12,0,100,0,0,0,''),
(22458,11,0,'You''ve been a tremendous help, $r! Let''s get out of here before more of those things show up! I''ll let Dwarfowitz know you did the job asked of ya'' admirably.',12,0,100,0,0,0,'');
DELETE FROM `waypoints` WHERE `entry` IN (22458);
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(22458,1,-3728.1,5385.5,-3.41,''),
(22458,2,-3735.9,5392.8,-4.8,''),
(22458,3,-3729.7,5381,-4.9,''),
(22458,4,-3704.6,5372.74,-9.33,''),
(22458,5,-3704.8,5374.7,-8.9,''),
(22458,6,-3623.2,5403.2,-11.5,''),
(22458,7,-3554.6,5455.2,-11.3,''),
(22458,8,-3557.7,5468.9,-8.1,''),
(22458,9,-3548.9,5458.3,-12.3,'');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (22458) AND `AIName` = '' AND `ScriptName` = '';

-- The Final Code
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (20767,21039,21898);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (2103900);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(20767,0,0,1,38,0,100,0,1,1,0,0,0,45,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Explosion Trigger - On Data Set 1 1 - Set Data 1 0'),
(20767,0,1,0,61,0,100,0,0,0,0,0,0,11,35513,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Explosion Trigger - On Data Set 1 1 - Cast ''Mana Bomb Explosion'''),
(21039,0,0,1,38,0,100,0,1,1,0,0,0,45,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Data Set 1 1 - Set Data 1 0'),
(21039,0,1,2,61,0,100,0,0,0,0,0,0,1,0,3000,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Data Set 1 1 - Say Line 0'),
(21039,0,2,0,61,0,100,0,0,0,0,0,0,80,2103900,2,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Data Set 1 1 - Run Script'),
(21039,0,3,4,52,0,100,0,0,21039,0,0,0,1,1,3000,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 0 Over - Say Line 1'),
(21039,0,4,0,61,0,100,0,0,0,0,0,0,80,2103900,2,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 0 Over - Run Script'),
(21039,0,5,6,52,0,100,0,1,21039,0,0,0,1,2,3000,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 1 Over - Say Line 2'),
(21039,0,6,0,61,0,100,0,0,0,0,0,0,80,2103900,2,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 1 Over - Run Script'),
(21039,0,7,8,52,0,100,0,2,21039,0,0,0,1,3,3000,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 2 Over - Say Line 3'),
(21039,0,8,0,61,0,100,0,0,0,0,0,0,80,2103900,2,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 2 Over - Run Script'),
(21039,0,9,10,52,0,100,0,3,21039,0,0,0,1,4,3000,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 3 Over - Say Line 4'),
(21039,0,10,0,61,0,100,0,0,0,0,0,0,80,2103900,2,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 3 Over - Run Script'),
(21039,0,12,0,52,0,100,0,4,21039,0,0,0,45,1,1,0,0,0,0,9,20767,0,200,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Text 4 Over - Set Data 1 1'),
(21898,0,0,1,38,0,100,0,1,1,0,0,0,45,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mana Bomb Lightning Trigger - On Data Set 1 1 - Set Data 1 0'),
(21898,0,1,0,61,0,100,0,0,0,0,0,0,11,37843,0,0,0,0,0,9,21899,0,200,0,0,0,0,'Mana Bomb Lightning Trigger - On Data Set 1 1 - Cast ''Mana Bomb: Lightning'''),
(2103900,9,0,0,0,0,100,0,0,0,0,0,0,45,1,1,0,0,0,0,9,21898,0,200,0,0,0,0,'Mana Bomb Kill Credit Trigger - On Script - Set Data 1 1');
DELETE FROM `creature_text` WHERE `entry` IN (21039);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(21039,0,0,'5...',41,0,100,0,0,0,'Mana Bomb'),
(21039,1,0,'4...',41,0,100,0,0,0,'Mana Bomb'),
(21039,2,0,'3...',41,0,100,0,0,0,'Mana Bomb'),
(21039,3,0,'2...',41,0,100,0,0,0,'Mana Bomb'),
(21039,4,0,'1...',41,0,100,0,0,0,'Mana Bomb');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (20767,21039,21898) AND `AIName` = '' AND `ScriptName` = '';

-- Mana Bomb (The Final Code)
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` IN (184725);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(184725,1,0,1,70,0,100,0,2,0,0,0,0,45,1,1,0,0,0,0,10,73864,21039,0,0,0,0,0,'Mana Bomb - On Gameobject State Changed - Set Data 1 1'),
(184725,1,1,2,61,0,100,0,0,0,0,0,0,33,21039,0,0,0,0,0,16,0,0,0,0,0,0,0,'Mana Bomb - On Gameobject State Changed - Quest Credit ''The Final Code'''),
(184725,1,2,0,61,0,100,0,0,0,0,0,0,45,1,1,0,0,0,0,9,16769,0,50,0,0,0,0,'Mana Bomb - On Gameobject State Changed - Set Data 1 1');
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (184725) AND `AIName` = '' AND `ScriptName` = '';

-- An Improper Burial
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (21846,21859);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(21846,0,0,1,8,0,100,1,39189,0,0,0,0,11,59216,0,0,0,0,0,1,0,0,0,0,0,0,0,'Slain Auchenai Warrior - On Spellhit ''Sha''tari Torch'' - Cast ''Burning Corpse'' (No Repeat)'),
(21846,0,1,2,61,0,100,0,0,0,0,0,0,33,21846,0,0,0,0,0,7,0,0,0,0,0,0,0,'Slain Auchenai Warrior - On Spellhit ''Sha''tari Torch'' - Quest Credit ''An Improper Burial'' (No Repeat)'),
(21846,0,2,0,61,0,100,0,0,0,0,0,0,41,15000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Slain Auchenai Warrior - On Spellhit ''Sha''tari Torch'' - Despawn In 15000 ms (No Repeat)'),
(21859,0,0,1,8,0,100,1,39189,0,0,0,0,11,59216,0,0,0,0,0,1,0,0,0,0,0,0,0,'Slain Sha''tar Vindicator - On Spellhit ''Sha''tari Torch'' - Cast ''Burning Corpse'' (No Repeat)'),
(21859,0,1,2,61,0,100,0,0,0,0,0,0,33,21859,0,0,0,0,0,7,0,0,0,0,0,0,0,'Slain Sha''tar Vindicator - On Spellhit ''Sha''tari Torch'' - Quest Credit ''An Improper Burial'' (No Repeat)'),
(21859,0,2,3,61,0,100,0,0,0,0,0,0,11,37759,0,0,0,0,0,1,0,0,0,0,0,0,0,'Slain Sha''tar Vindicator - On Spellhit ''Sha''tari Torch'' - Cast ''Bone Wastes - Summon Draenei Guardian Spirit'' (No Repeat)'),
(21859,0,3,0,61,0,100,0,0,0,0,0,0,41,15000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Slain Sha''tar Vindicator - On Spellhit ''Sha''tari Torch'' - Despawn In 15000 ms (No Repeat)');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (21846,21859) AND `AIName` = '' AND `ScriptName` = '';
