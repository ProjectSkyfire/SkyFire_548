-- Desolace: Firestarter (14196) and Rider on the Storm (14198).
-- The Wispy Vortex had no script, so it never grew and gave no credit. The Raging Vortex had no vehicle, script or
-- path, so the second quest could not be started, and the Nijel's Point Defenders and Merchants stood in the default
-- phase where the guards of the outpost attack the rider. Safe to run twice.

-- Wispy Vortex: grows with every Disciple of Elune killed nearby (the Disciple's script sets data 1 on it).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 35386;

-- Raging Vortex: the flying vehicle summoned by the Raging Vortex Gem. Its path is in `waypoints`, 29 to 34 are
-- move points in the script.
UPDATE `creature_template`
SET `faction_A` = 2, `faction_H` = 2, `npcflag` = 16777216, `speed_run` = 2, `unit_flags` = 33554432,
    `unit_flags2` = 2099200, `type_flags` = 1074790400, `spell1` = 67523, `spell4` = 67692, `VehicleId` = 475,
    `AIName` = 'SmartAI', `InhabitType` = 4
WHERE `entry` = 35393;

DELETE FROM `creature_template_addon` WHERE `entry` = 35393;
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`)
VALUES (35393,0,0,0,1,0,'67505 67707');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 35393;
INSERT INTO `npc_spellclick_spells` (`npc_entry`,`spell_id`,`cast_flags`,`user_type`)
VALUES (35393,67510,1,0);

-- The gem's spell summons the vehicle.
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 67501 AND `spell_effect` = 67504;
INSERT INTO `spell_linked_spell` (`spell_trigger`,`spell_effect`,`type`,`comment`)
VALUES (67501,67504,0,'Summon Raging Vortex');

DELETE FROM `waypoints` WHERE `entry` = 35393;
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`)
VALUES
(35393,1,132.939,1197.83,165.958,'Raging Vortex'),
(35393,2,144.363,1176.09,166.441,'Raging Vortex'),
(35393,3,159.998,1170.64,166.764,'Raging Vortex'),
(35393,4,173.828,1182.29,166.764,'Raging Vortex'),
(35393,5,173.203,1199.88,166.764,'Raging Vortex'),
(35393,6,161.757,1219.91,166.764,'Raging Vortex'),
(35393,7,144.818,1229.75,166.764,'Raging Vortex'),
(35393,8,132.08,1236.66,166.764,'Raging Vortex'),
(35393,9,123.998,1252.98,166.764,'Raging Vortex'),
(35393,10,132.745,1267.93,173.53,'Raging Vortex'),
(35393,11,141.097,1280.12,180.38,'Raging Vortex'),
(35393,12,142.757,1297.99,188.907,'Raging Vortex'),
(35393,13,156.974,1308.19,190.628,'Raging Vortex'),
(35393,14,192.458,1315.05,191.619,'Raging Vortex'),
(35393,15,216.465,1297.23,191.202,'Raging Vortex'),
(35393,16,226.599,1290.67,191.063,'Raging Vortex'),
(35393,17,238.993,1292.18,190.897,'Raging Vortex'),
(35393,18,244.342,1299.48,191.859,'Raging Vortex'),
(35393,19,241.337,1309.6,191.747,'Raging Vortex'),
(35393,20,231.691,1316.21,191.831,'Raging Vortex'),
(35393,21,196.396,1320.33,191.692,'Raging Vortex'),
(35393,22,174.661,1314.27,191.053,'Raging Vortex'),
(35393,23,160.477,1309.74,191.053,'Raging Vortex'),
(35393,24,146.618,1300.57,191.053,'Raging Vortex'),
(35393,25,140.649,1288.23,191.053,'Raging Vortex'),
(35393,26,137.79,1271.47,176.359,'Raging Vortex'),
(35393,27,128.898,1249.79,167.859,'Raging Vortex'),
(35393,28,114.892,1227.87,167.859,'Raging Vortex');

-- The rider and the vortex are in phase 170 (the aura of spell 65141) while the Defenders and Merchants are
-- only visible there.
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (35386,35393) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3539300 AND `source_type` = 9;
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,
 `event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,
 `action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,
 `action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,
 `target_z`,`target_o`,`comment`)
VALUES
(35386,0,0,1,54,0,100,0,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,
 'Wispy Vortex - On Summoned - Store Target List'),
(35386,0,1,2,61,0,100,0,0,0,0,0,0,18,33587712,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 0 - Set Unit Flags'),
(35386,0,2,3,61,0,100,0,0,0,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 1 - Set Run'),
(35386,0,3,4,61,0,100,0,0,0,0,0,0,29,0,0,0,0,0,0,12,1,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 2 - Follow Summoner'),
(35386,0,4,0,61,0,100,0,0,0,0,0,0,11,67409,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 3 - Cast Wispy Vortex'),
(35386,0,5,0,38,0,100,0,1,0,1000,1000,0,11,58177,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - On Data Set 1 0 - Cast Stack Aura'),
(35386,0,6,7,23,0,100,1,58177,1,0,0,0,28,67409,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - On 1 Stack - Remove Wispy Vortex'),
(35386,0,7,0,61,0,100,0,0,0,0,0,0,11,67412,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 6 - Cast Gusty Vortex'),
(35386,0,8,9,23,0,100,1,58177,2,0,0,0,28,67412,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - On 2 Stacks - Remove Gusty Vortex'),
(35386,0,9,0,61,0,100,0,0,0,0,0,0,11,67421,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 8 - Cast Stormy Vortex'),
(35386,0,10,11,23,0,100,1,58177,3,0,0,0,28,67421,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - On 3 Stacks - Remove Stormy Vortex'),
(35386,0,11,0,61,0,100,0,0,0,0,0,0,11,67422,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 10 - Cast Blustering Vortex'),
(35386,0,12,13,23,0,100,1,58177,4,0,0,0,28,67422,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - On 4 Stacks - Remove Blustering Vortex'),
(35386,0,13,0,61,0,100,0,0,0,0,0,0,11,67423,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 12 - Cast Surging Vortex'),
(35386,0,14,15,23,0,100,1,58177,5,0,0,0,28,67423,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - On 5 Stacks - Remove Surging Vortex'),
(35386,0,15,16,61,0,100,0,0,0,0,0,0,11,67425,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 14 - Cast Seething Vortex'),
(35386,0,16,17,61,0,100,0,0,0,0,0,0,33,35392,0,0,0,0,0,12,1,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 15 - Give Kill Credit'),
(35386,0,17,18,61,0,100,0,0,0,0,0,0,28,67425,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 16 - Remove Seething Vortex'),
(35386,0,18,0,61,0,100,0,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Wispy Vortex - Linked To Id 17 - Despawn'),
(35393,0,0,1,54,0,100,0,0,0,0,0,0,2,2,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - On Summoned - Set Faction 2'),
(35393,0,1,2,61,0,100,0,0,0,0,0,0,81,16777216,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 0 - Set NpcFlag 16777216'),
(35393,0,2,3,61,0,100,0,0,0,0,0,0,44,170,1,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 1 - Set Ingame Phase 170'),
(35393,0,3,4,61,0,100,0,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 2 - Store Target List'),
(35393,0,4,0,61,0,100,0,0,0,0,0,0,67,35393,1000,1000,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 3 - Create Timed Event'),
(35393,0,5,6,59,0,100,0,35393,0,0,0,0,86,67510,2,12,1,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - On Timed Event Triggered - Cross Cast Ride Raging Vortex'),
(35393,0,6,7,61,0,100,0,0,0,0,0,0,86,65141,2,12,1,0,0,12,1,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 5 - Cross Cast Generic Phase Shift 1'),
(35393,0,7,0,61,0,100,0,0,0,0,0,0,80,3539300,2,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 6 - Run Script'),
(35393,0,8,0,34,0,100,0,0,29,0,0,0,69,30,0,0,0,0,0,1,0,0,0,15.4358,1218.38,130.122,0,
 'Raging Vortex - On Reached Point 29 - Move To Pos'),
(35393,0,9,0,34,0,100,0,0,30,0,0,0,69,31,0,0,0,0,0,1,0,0,0,35.3438,1217.64,141.031,0,
 'Raging Vortex - On Reached Point 30 - Move To Pos'),
(35393,0,10,0,34,0,100,0,0,31,0,0,0,69,32,0,0,0,0,0,1,0,0,0,58.9028,1219.53,152.895,0,
 'Raging Vortex - On Reached Point 31 - Move To Pos'),
(35393,0,11,0,34,0,100,0,0,32,0,0,0,69,33,0,0,0,0,0,1,0,0,0,87.4601,1222.84,162.029,0,
 'Raging Vortex - On Reached Point 32 - Move To Pos'),
(35393,0,12,0,34,0,100,0,0,33,0,0,0,69,34,0,0,0,0,0,1,0,0,0,118.674,1218.4,163.277,0,
 'Raging Vortex - On Reached Point 33 - Move To Pos'),
(35393,0,13,0,34,0,100,0,0,34,0,0,0,53,1,35393,1,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - On Reached Point 34 - Waypoint Start'),
(35393,0,14,15,8,0,100,0,67692,0,0,0,0,28,67510,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - On SpellHit Calm the Storm - Remove Aura Ride Raging Vortex'),
(35393,0,15,16,61,0,100,0,0,0,0,0,0,28,65141,0,0,0,0,0,12,1,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 14 - Remove Aura Generic Phase Shift 1'),
(35393,0,16,17,61,0,100,0,0,0,0,0,0,62,1,0,0,0,0,0,12,1,0,0,-444.859,2230.79,89.739,6.03501,
 'Raging Vortex - Linked To Id 15 - Teleport Player'),
(35393,0,17,0,61,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - Linked To Id 16 - Despawn'),
(3539300,9,0,0,0,0,100,0,2000,2000,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Raging Vortex - On Script - Set Run 1'),
(3539300,9,1,0,0,0,100,0,0,0,0,0,0,69,29,0,0,0,0,0,1,0,0,0,-0.663194,1212.13,118.687,0,
 'Raging Vortex - On Script - Move To Pos');

UPDATE `creature` SET `phaseId` = 170 WHERE `id` IN (35506,35510);
