-- Shadowmoon Valley: the Spirit of Ar'tor (same spot in both 3.3.5 databases), the starter and ender of Akama's
-- Promise and the Replace Lost Spectrecles gossip of Hildagard and Zorus.

-- Akama's Promise: starter and ender
DELETE FROM `creature_queststarter` WHERE (`id` = 21700 AND `quest` = 10708);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(21700, 10708);
DELETE FROM `creature_questender` WHERE (`id` = 18481 AND `quest` = 10708);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(18481, 10708);

-- Commander Hobb, Xi'ri and the Spirit of Ar'tor, same spots in both 3.3.5 databases
DELETE FROM `creature` WHERE `guid` IN (8317010);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8317010, 21318, 530, 1, 0, 0, 0, 0, -3800.38000, 2601.10000, 90.14300, 5.481630, 300, 0, 0, 0, 0, 0, 0, 0, 0);

-- Replace Lost Spectrecles
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (21772,21774);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(21772,0,0,2,62,0,100,0,8383,0,0,0,0,85,37602,2,0,0,0,0,7,0,0,0,0,0,0,0,'Chief Apothecary Hildagard - On Gossip Option 0 Selected - Invoker cast Replace Lost Spectrecles T1'),
(21772,0,1,2,62,0,100,0,8383,1,0,0,0,85,37700,2,0,0,0,0,7,0,0,0,0,0,0,0,'Chief Apothecary Hildagard - On Gossip Option 1 Selected - Invoker cast Replace Lost Spectrecles T2'),
(21772,0,2,0,61,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Chief Apothecary Hildagard - Link - Close Gossip'),
(21774,0,0,2,62,0,100,0,8392,0,0,0,0,85,37602,2,0,0,0,0,7,0,0,0,0,0,0,0,'Zorus the Judicator - On Gossip Option 0 Selected - Invoker cast Replace Lost Spectrecles T1'),
(21774,0,1,2,62,0,100,0,8392,1,0,0,0,85,37700,2,0,0,0,0,7,0,0,0,0,0,0,0,'Zorus the Judicator - On Gossip Option 1 Selected - Invoker cast Replace Lost Spectrecles T2'),
(21774,0,2,0,61,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Zorus the Judicator - Link - Close Gossip');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (21772,21774) AND `AIName` = '' AND `ScriptName` = '';
