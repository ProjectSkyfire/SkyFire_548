-- Southern Barrens quest fixes. Reference data: 4.3.4 for the Silithissues pitch bunny pattern, the Sabersnout and
-- High Road Scout scripts and the creature loot ids; 5.4.8 for the spawns, scripts, gossip conditions and gameobject
-- data that the 4.3.4 data does not have. Safe to run twice: every block deletes its own keys first or sets absolute
-- values.

-- Quest givers, enders and kill targets that were never spawned, so their quests could not be started, finished or
-- completed: Nura Pathfinder (Warchief's Command, Clear the High Road), Tunawa Stillwind, Holgom, Warlord Bloodhilt at
-- Desolation Hold, the four High Road Scouts, the four spirits of Honoring the Dead, and at Bael Modan Weezil
-- Slipshadow (corpse), Marley Twinbraid, the Wounded Bael'dun Officer and 34 Bael'dun Survivors. Positions are the
-- 5.4.8 spawns; the 4.3.4 data has none of them. Omusa's Spirit flies a path in 5.4.8; it stands still here.
DELETE FROM `creature` WHERE `guid` BETWEEN 8330600 AND 8330648;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8330600, 37135, 1, 1, 0, 0, 0, 0, -454.924, -972.305, 96.1874, 0.523599, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330601, 37137, 1, 1, 0, 0, 0, 0, -618.429, -1419.58, 156.591, 5.89959, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330602, 37153, 1, 1, 0, 0, 0, 0, -617.844, -1434.71, 156.657, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330603, 37837, 1, 1, 0, 0, 0, 0, -3246.13, -1730.26, 92.1956, 5.58505, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330604, 37159, 1, 1, 0, 0, 0, 0, -492.843, -1256.66, 149.845, 3.58609, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330605, 37159, 1, 1, 0, 0, 0, 0, -444.436, -1185.92, 176.732, 4.94269, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330606, 37159, 1, 1, 0, 0, 0, 0, -424.011, -1207.36, 180.748, 4.08817, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330607, 37159, 1, 1, 0, 0, 0, 0, -517.407, -1262.33, 154.162, 5.85924, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330608, 37747, 1, 1, 0, 0, 0, 0, -2310.22, -1930.83, 96.18, 2.85118, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330609, 37756, 1, 1, 0, 0, 0, 0, -2375.17, -1945.64, 96.2274, 5.6306, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330610, 37758, 1, 1, 0, 0, 0, 0, -2394.35, -1930.09, 96.8846, 1.19413, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330611, 37759, 1, 1, 0, 0, 0, 0, -2375.42, -1796.29, 238.136, 5.6486, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330612, 38152, 1, 1, 0, 0, 0, 0, -4098.5, -2369.88, 126.214, 2.23402, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330613, 39188, 1, 1, 0, 0, 0, 0, -4077.44, -2294.32, 125.455, 3.14159, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330614, 39220, 1, 1, 0, 0, 0, 0, -4103.25, -2323.3, 126.211, 1.36136, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330615, 39218, 1, 1, 0, 0, 0, 0, -4082.71, -2387.23, 135.938, 4.39823, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330616, 39218, 1, 1, 0, 0, 0, 0, -4072.1, -2419.81, 135.939, 1.27409, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330617, 39218, 1, 1, 0, 0, 0, 0, -4057.28, -2366.36, 135.938, 0.698132, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330618, 39218, 1, 1, 0, 0, 0, 0, -4097.62, -2373.37, 135.939, 0.558505, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330619, 39218, 1, 1, 0, 0, 0, 0, -4083.56, -2401.63, 135.939, 1.09956, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330620, 39218, 1, 1, 0, 0, 0, 0, -4046.86, -2376.68, 124.826, 1.29154, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330621, 39218, 1, 1, 0, 0, 0, 0, -4089.04, -2306.83, 124.997, 2.70526, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330622, 39218, 1, 1, 0, 0, 0, 0, -4087.91, -2385.22, 118.16, 6.05629, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330623, 39218, 1, 1, 0, 0, 0, 0, -4045.75, -2370.74, 135.94, 4.39823, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330624, 39218, 1, 1, 0, 0, 0, 0, -4079.82, -2384.75, 126.214, 1.55334, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330625, 39218, 1, 1, 0, 0, 0, 0, -4055.16, -2349.99, 135.939, 4.69494, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330626, 39218, 1, 1, 0, 0, 0, 0, -4051.85, -2369.68, 135.939, 6.07375, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330627, 39218, 1, 1, 0, 0, 0, 0, -4070.68, -2421.64, 126.216, 0.994838, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330628, 39218, 1, 1, 0, 0, 0, 0, -4047.6, -2368.5, 118.16, 2.96706, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330629, 39218, 1, 1, 0, 0, 0, 0, -4099.73, -2360.22, 127.228, 4.72984, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330630, 39218, 1, 1, 0, 0, 0, 0, -4076.81, -2391.98, 118.16, 1.23918, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330631, 39218, 1, 1, 0, 0, 0, 0, -4068.79, -2381.04, 110.381, 1.46608, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330632, 39218, 1, 1, 0, 0, 0, 0, -4086.51, -2392.77, 124.826, 4.79965, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330633, 39218, 1, 1, 0, 0, 0, 0, -4051.3, -2367.09, 118.159, 5.53269, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330634, 39218, 1, 1, 0, 0, 0, 0, -4083.68, -2393.74, 118.16, 2.63545, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330635, 39218, 1, 1, 0, 0, 0, 0, -4077.69, -2366.87, 108.992, 0.925025, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330636, 39218, 1, 1, 0, 0, 0, 0, -4084.03, -2361.9, 108.992, 5.72468, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330637, 39218, 1, 1, 0, 0, 0, 0, -4071.05, -2356.42, 108.992, 4.74729, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330638, 39218, 1, 1, 0, 0, 0, 0, -4051.37, -2363.85, 124.826, 5.95157, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330639, 39218, 1, 1, 0, 0, 0, 0, -4052, -2352.4, 126.214, 3.28122, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330640, 39218, 1, 1, 0, 0, 0, 0, -4061.05, -2371.44, 118.159, 4.39823, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330641, 39218, 1, 1, 0, 0, 0, 0, -4061.97, -2329.68, 127.603, 4.39823, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330642, 39218, 1, 1, 0, 0, 0, 0, -4040.64, -2372.64, 121.215, 2.94961, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330643, 39218, 1, 1, 0, 0, 0, 0, -4061.96, -2345.57, 126.469, 1.71042, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330644, 39218, 1, 1, 0, 0, 0, 0, -4071.35, -2371.33, 126.214, 2.89725, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330645, 39218, 1, 1, 0, 0, 0, 0, -4088.29, -2384.42, 124.826, 3.75246, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330646, 39218, 1, 1, 0, 0, 0, 0, -4100.85, -2376.66, 126.214, 5.72468, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330647, 39218, 1, 1, 0, 0, 0, 0, -4110.6, -2354.02, 127.603, 6.14356, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8330648, 39218, 1, 1, 0, 0, 0, 0, -4053.19, -2388.54, 126.214, 2.74017, 300, 0, 0, 0, 0, 0, 0, 0, 0);

-- 49 creature rows, guids 8330600 to 8330648.

-- Tunawa Stillwind and Holgom lacked the gossip flag and greeting menu (4.3.4 and 5.4.8: flag 3, menus 11003 and
-- 10890). The Stonetalon Prisoner (Warriors' Redemption) lacked the gossip flag, menu and script (5.4.8: flag 1,
-- menu 10882, SmartAI).
UPDATE `creature_template` SET `npcflag` = 3, `gossip_menu_id` = 11003 WHERE `entry` = 37137;
UPDATE `creature_template` SET `npcflag` = 3, `gossip_menu_id` = 10890 WHERE `entry` = 37153;
UPDATE `creature_template` SET `npcflag` = 1, `unit_flags` = 33280, `gossip_menu_id` = 10882, `AIName` = 'SmartAI' WHERE `entry` = 37167;

-- Four hostile kill targets dealt no damage (placeholder templates): High Road Scout, Sabersnout, Grasping Overgrowth
-- and Wildhammer Mercenary. The numbers follow home's neighbouring templates of the same level (level 31: 65 to 117,
-- attack power 108; level 32: 68 to 123, 112; level 33: 72 to 129, 114; the level 29 scout is interpolated at 59 to
-- 105, 102). They are not retail values, so the owner should feel-check them. High Road Scout and Sabersnout also get
-- their 4.3.4 SmartAI (a thrown weapon; Strike, Quilboar Rush, Fear and Enrage).
UPDATE `creature_template` SET `mindmg` = 59, `maxdmg` = 105, `attackpower` = 102, `AIName` = 'SmartAI' WHERE `entry` = 37159;
UPDATE `creature_template` SET `mindmg` = 68, `maxdmg` = 123, `attackpower` = 112, `AIName` = 'SmartAI' WHERE `entry` = 37513;
UPDATE `creature_template` SET `mindmg` = 65, `maxdmg` = 117, `attackpower` = 108 WHERE `entry` = 37548;
UPDATE `creature_template` SET `mindmg` = 72, `maxdmg` = 129, `attackpower` = 114 WHERE `entry` = 37977;


-- The Stonetalon Prisoner's gossip option only shows to a player who has Confiscated Arms and the quest (5.4.8).
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 10882;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 10882, 0, 0, 0, 2, 0, 49769, 1, 0, 0, 0, 0, '', 'Show gossip if player have item Confiscated Arms'),
(15, 10882, 0, 0, 0, 9, 0, 24512, 0, 0, 0, 0, 0, '', 'Show gossip if player have quest 24512');

-- Scripts. Stonetalon Prisoner: gossip option removes Confiscated Arms and gives the kill credit. Sabersnout and High
-- Road Scout: 4.3.4 combat spells. Deviate Terrortooth and Deviate Plainstrider: Soothing Seeds (Sowing a Solution)
-- gives the credit, then the creature turns friendly, enrages or explodes; the 5.4.8 combat stop action (203) is 27 in
-- this core and the self-cast flag 16 becomes 0. Silithid Mound Bunny: Bucket of Burning Pitch (Silithissues)
-- summons Silithid Swarmers. Frazzlecraz Explosives: kill credit when the object changes state.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (37167, 37513, 37159, 38055);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (37090, 37091) AND `id` = 2;
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` IN (202596);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (3709000, 3709001, 3709002, 3709100, 3709101, 3709102);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37167, 0, 0, 1, 62, 0, 100, 0, 10882, 0, 0, 0, 0, 57, 49769, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonetalon Prisoner - On Gossip Select - Remove Item Confiscated Arms'),
(37167, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonetalon Prisoner - Linked To Id 0 - Close Gossip'),
(37167, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 70010, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonetalon Prisoner - Linked To Id 1 - Cast Kill Credit'),
(37167, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 70011, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonetalon Prisoner - Linked To Id 2 - Cast Give Confiscated Arms'),
(37167, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 70012, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonetalon Prisoner - Linked To Id 3 - Cast Redeemed'),
(37167, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonetalon Prisoner - Linked To Id 4 - Despawn'),
(37159, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 11, 38557, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'High Road Scout - On Aggro - Cast ''Throw'' (No Repeat)'),
(37513, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sabersnout - On Aggro - Say Line 0 (No Repeat)'),
(37513, 0, 1, 0, 6, 0, 100, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sabersnout - On Just Died - Say Line 1 (No Repeat)'),
(37513, 0, 2, 0, 9, 0, 100, 0, 0, 5, 12000, 13000, 0, 11, 11976, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sabersnout - Within 0-5 Range - Cast ''Strike'''),
(37513, 0, 3, 0, 0, 0, 100, 0, 6000, 8000, 15000, 18000, 0, 11, 82610, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sabersnout - In Combat - Cast ''Quilboar Rush'''),
(37513, 0, 4, 0, 0, 0, 100, 0, 9000, 12000, 26000, 33000, 0, 11, 70171, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sabersnout - In Combat - Cast ''Fear'''),
(37513, 0, 5, 6, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sabersnout - Between 0-30% Health - Cast ''Enrage'' (No Repeat)'),
(37513, 0, 6, 0, 61, 0, 100, 1, 0, 30, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sabersnout - Between 0-30% Health - Say Line 2 (No Repeat)'),
(37090, 0, 2, 0, 8, 0, 100, 0, 70458, 0, 0, 0, 0, 87, 3709000, 3709001, 3709002, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On SpellHit Soothing Seeds - Run Random Script'),
(37091, 0, 2, 0, 8, 0, 100, 0, 70458, 0, 0, 0, 0, 87, 3709100, 3709101, 3709102, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On SpellHit Soothing Seeds - Run Random Script'),
(3709000, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 36, 37084, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Update Template On Terrortooth Scytheclaw'),
(3709000, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 37726, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Quest Credit'),
(3709000, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 52866, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Cast Transform Visual'),
(3709000, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Set Faction 35'),
(3709000, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Combat Stop'),
(3709000, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 19, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Remove UnitFlag 8'),
(3709000, 9, 6, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Despawn after 30 sec'),
(3709001, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 71205, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Cast Enrage'),
(3709001, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 37726, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Quest Credit'),
(3709001, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - React State Aggressive'),
(3709001, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Attack Start'),
(3709002, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 62987, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Cast Explosion'),
(3709002, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 37726, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Quest Credit'),
(3709002, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Set Invisible'),
(3709002, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Terrortooth - On Script - Despawn after 1 sec'),
(3709100, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 36, 37085, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Update Template On Towering Plainstrider'),
(3709100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 37726, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Quest Credit'),
(3709100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 52866, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Cast Transform Visual'),
(3709100, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Set Faction 35'),
(3709100, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Combat Stop'),
(3709100, 9, 5, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Despawn after 30 sec'),
(3709101, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 71205, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Cast Enrage'),
(3709101, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 37726, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Quest Credit'),
(3709101, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - React State Aggressive'),
(3709101, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Attack Start'),
(3709102, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 62987, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Cast Explosion'),
(3709102, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 37726, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Quest Credit'),
(3709102, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Set Invisible'),
(3709102, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deviate Plainstrider - On Script - Despawn after 1 sec'),
(38055, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 20, 201943, 3, 0, 0, 0, 0, 0, 'Silithid Mound Bunny - On Respawn - Ser Data 0 2'),
(38055, 0, 1, 2, 8, 0, 100, 0, 71030, 0, 0, 0, 0, 11, 71059, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Mound Bunny - On SpellHit Burning Pitch - Cast Summon Silithid Swarmer'),
(38055, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 71059, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Mound Bunny - Linked To Id 1 - Cast Summon Silithid Swarmer'),
(38055, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 71059, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Mound Bunny - Linked To Id 2 - Cast Summon Silithid Swarmer'),
(38055, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 20, 201943, 3, 0, 0, 0, 0, 0, 'Silithid Mound Bunny - Linked To Id 3 - Set Data 0 1'),
(38055, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Mound Bunny - Linked To Id 4 - Despawn'),
(202596, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 39315, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Frazzlecraz Explosives - On GO Change State - Quest Credit'),
(202596, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 8000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frazzlecraz Explosives - On GO Change State - Despawn After 8 Seconds');



-- The Frazzlecraz Explosives (...And Bury the Rest: the 40 explosives give a kill credit
-- and despawn when the plunger sets them off) need the gameobject script flag.
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (202596);

-- Horn of Challenge (Speaking Their Language) sends event 22771; nothing answered it, so Sabersnout was never
-- summoned. Event script from 5.4.8 (his spawn is disabled there, he is summoned for three minutes).
DELETE FROM `event_scripts` WHERE `id` = 22771;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(22771, 2, 10, 37513, 180000, 0, -1835.83, -1827.26, 90.5462, 3.29867);

-- Sabersnout's three lines (4.3.4): aggro yell, death say, enrage emote.
DELETE FROM `creature_text` WHERE `entry` = 37513;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(37513, 0, 0, 'You, <Snort> you are strong <Gurgle>. But... Strong enough for THE BOARPOCALYPSE?', 14, 0, 100, 0, 0, 0, 'on Aggro Text'),
(37513, 1, 0, 'You ... survive the boars? <Snort> So many boars! So sad! <Gurgle>', 12, 0, 100, 0, 0, 0, 'on Death Text'),
(37513, 2, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'combat Enrage');

-- Mutilated Remains (Parts-is-Parts: 15 Severed Extremities) had loot but no spawn. 5.4.8 has 17 spawns; only nine
-- object guids were reserved for this fix, so nine spread over the clusters are used (respawn 5 minutes).
-- Keg of Ol' Barkerstout (He Needs Ale!) was an unnamed placeholder with no loot or spawn; type, name, loot and the
-- spawn are the 5.4.8 ones.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 930030 AND 930039;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(930030, 201904, 1, 1, 0, 0, -3227.56, -2086.76, 114.423, 3.90947, 0, 0, -0.927197, 0.374575, 300, 100, 1),
(930031, 201904, 1, 1, 0, 0, -3227.99, -2087.66, 114.162, 3.90947, 0, 0, -0.927197, 0.374575, 300, 100, 1),
(930032, 201904, 1, 1, 0, 0, -3236.39, -2076.1, 116.137, 4.39839, 0, 0, -0.808971, 0.587848, 300, 100, 1),
(930033, 201904, 1, 1, 0, 0, -3199.43, -2091.72, 114.228, 1.04789, 0, 0, 0.500299, 0.865853, 300, 100, 1),
(930034, 201904, 1, 1, 0, 0, -3199.06, -2093.33, 113.798, 1.04789, 0, 0, 0.500299, 0.865853, 300, 100, 1),
(930035, 201904, 1, 1, 0, 0, -3130.58, -2082.25, 116.212, 5.86048, 0, 0, -0.209781, 0.977749, 300, 100, 1),
(930036, 201904, 1, 1, 0, 0, -3104.27, -2086.69, 116.249, 3.5835, 0, 0, -0.975689, 0.21916, 300, 100, 1),
(930037, 201904, 1, 1, 0, 0, -3090.01, -2077.07, 118.297, 3.77172, 0, 0, -0.950777, 0.309876, 300, 100, 1),
(930038, 201904, 1, 1, 0, 0, -3068.39, -2081.05, 115.31, 4.32777, 0, 0, -0.829218, 0.558925, 300, 100, 1),
(930039, 202567, 1, 1, 0, 0, -4039.06, -2384.59, 125.993, 2.00713, 0, 0, 0.843391, 0.5373, 300, 100, 1);
UPDATE `gameobject_template` SET `type` = 3, `name` = 'Keg of Ol\' Barkerstout', `data0` = 93, `data1` = 202567 WHERE `entry` = 202567;
DELETE FROM `gameobject_loot_template` WHERE `entry` = 202567;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(202567, 52491, -100, 1, 0, 1, 1);

-- Fresh Stormsnout Steak (Marching On Our Stomachs) had no source. 5.4.8 drops it from Stormsnout at 45.46 percent.
DELETE FROM `creature_loot_template` WHERE `entry` = 3240 AND `item` = 52198;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(3240, 52198, -45.4615, 1, 0, 1, 1);
