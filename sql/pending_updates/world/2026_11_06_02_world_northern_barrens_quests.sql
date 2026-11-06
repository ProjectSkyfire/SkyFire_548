-- Northern Barrens quest fixes. Reference data: 4.3.4 for the race masks and 5.4.8 for the scripts, spawns and
-- quest credit NPCs that the 4.3.4 data does not have. Safe to run twice: every block deletes its own keys first
-- or sets absolute values.

-- Goblin players could not take seven Barrens quests because the race mask was the Horde without Goblin.
-- 4.3.4 has the Horde races with Goblin (946); 5.4.8 also adds the Pandaren Horde bit (33555378).
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `Id` IN (844, 869, 871, 872, 891, 1270, 5041);

-- Quest NPCs and targets that were never spawned, so their quests could not be started, finished or killed for
-- credit: Kala'ma (Waptor Twapping), Sludge Beast (Sludge Beast!), Balgor Whipshank (Mor'shan Caravan Rescue and
-- Demon Seed), Echeyakee (Echeyakee), Alicia Cuthbert (Mutiny, Mon!), Captured Razormane (By Hook Or By Crook)
-- and Brutusk (Got Wood?). Positions are the 5.4.8 spawns; the 4.3.4 data has none of them.
DELETE FROM `creature` WHERE `guid` BETWEEN 8311700 AND 8311706;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8311700, 34828, 1, 1, 0, 0, 0, 0, -638.365, -3381.51, 91.7473, 5.61996, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Kala'ma
(8311701, 52165, 1, 1, 0, 0, 0, 0, 1062.84, -3143.2, 68.1349, 2.77142, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Sludge Beast
(8311702, 52309, 1, 1, 0, 0, 0, 0, 308.556, -2157.91, 214.281, 4.01426, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Balgor Whipshank
(8311703, 3475, 1, 1, 0, 0, 0, 0, 29.8999, -2329.98, 94.0502, 3.8778, 60, 0, 0, 0, 0, 2, 0, 0, 0), -- Echeyakee
(8311704, 34782, 1, 1, 0, 0, 0, 0, -1642.11, -4383.71, 5.38813, 2.33874, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Alicia Cuthbert
(8311705, 34523, 1, 1, 0, 0, 0, 0, 283.477, -3050.65, 95.9371, 3.49066, 300, 5, 0, 0, 0, 1, 0, 0, 0), -- Captured Razormane
(8311706, 33374, 1, 1, 0, 0, 0, 0, 1195.28, -2258.38, 91.7586, 0.314159, 60, 3, 0, 0, 0, 1, 0, 0, 0); -- Brutusk

-- Echeyakee walks the short path he has in the 5.4.8 data.
DELETE FROM `creature_addon` WHERE `guid` = 8311703;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(8311703, 83117030, 0, 0, 0, 0, '');

DELETE FROM `waypoint_data` WHERE `id` = 83117030;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(83117030, 1, 47.1411, -2314.29, 91.7909, 0, 0, 0, 0, 100, 0),
(83117030, 2, 46.434, -2315, 91.7909, 0, 0, 0, 0, 100, 0),
(83117030, 3, -17.9184, -2373.31, 91.6676, 0, 0, 0, 0, 100, 0),
(83117030, 4, -17.9184, -2373.31, 91.6676, 0, 0, 0, 0, 100, 0);

-- Two quest objects were never spawned. Curious Pirate Landmark (Gazlowe's Fortune) also had no loot, so the
-- treasure chest could not be dug up; the 4.3.4 data gives it loot 27289 and one spawn near Ratchet (phase removed).
-- Krom'zar's Banner (Counterattack!) has its loot but no spawn; the spawn is the 5.4.8 one.
DELETE FROM `gameobject` WHERE `guid` IN (910990, 910991);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910990, 195206, 1, 1, 0, 0, -1161.73, -3605.03, 93.5951, -1.58825, 0, 0, -0.71325, 0.70091, 300, 100, 1), -- Curious Pirate Landmark
(910991, 164690, 1, 1, 0, 0, -282.274, -1882.83, 92.7252, 5.74213, 0, 0, -0.267242, 0.96363, 300, 100, 1); -- Krom'zar's Banner

UPDATE `gameobject_template` SET `data1` = 27289 WHERE `entry` = 195206;

DELETE FROM `gameobject_loot_template` WHERE `entry` = 27289;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(27289, 46833, -100, 1, 0, 1, 1); -- Gazlowe's Treasure Chest

-- Quest credit NPCs without their scripts. The 5.4.8 data gives each one a SmartAI that hands out the credit or the
-- quest item; the 4.3.4 data has no script for them, so these quests could not be finished. Baron Longshore
-- (The Baron's Demands) and Captured Razormane (By Hook Or By Crook) also need the gossip flag.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 3 WHERE `entry` = 3467;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 34754;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 1, `gossip_menu_id` = 10521 WHERE `entry` = 34523;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 16777216, `unit_flags` = 33536 WHERE `entry` = 44057;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 16777216 WHERE `entry` IN (52171, 33374);

-- Riverboat (Raging River Ride), Muddy Tracks (Sludge Investigation) and Brutusk (Got Wood?) are clicked by the
-- player. Spells from the 5.4.8 click data.
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (44057, 52171, 33374);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(33374, 46990, 1, 0),
(44057, 81265, 1, 0),
(52171, 96479, 1, 0);

-- Scripts: Baron Longshore (gossip credit), Chief Engineer Foote (gives the ship schematics), Captured Razormane
-- (the interrogation options and the timed action lists that give credit; the player casts the five interrogation
-- emotes on themselves, so the invoker casts them, as this core has no "target casts on itself" flag), Riverboat,
-- Muddy Tracks and Brutusk
-- (credit on click).
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (3467, 34754, 34523, 44057, 52171, 33374);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (3452301, 3452302, 3452303, 3452304);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3467, 0, 0, 0, 62, 0, 100, 0, 10567, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Baron Longshore'),
(3467, 0, 1, 2, 62, 0, 100, 0, 10567, 1, 0, 0, 0, 33, 34769, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Baron Longshore'),
(3467, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 15, 14046, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Baron Longshore'),
(3467, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Baron Longshore'),
(33374, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 33374, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Brutusk'),
(34523, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 34527, 10, 0, 0, 0, 0, 0, 'Captured Razormane - On Respawn - Set Data 0 1'),
(34523, 0, 1, 2, 62, 0, 100, 0, 10521, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Gossip Select - Close Gossip'),
(34523, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 65618, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 1 - Cast Question Me'),
(34523, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 2 - Say Line 1 1'),
(34523, 0, 4, 5, 62, 0, 100, 0, 10521, 1, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Gossip Select - Store Target List'),
(34523, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 4 - Close Gossip'),
(34523, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 65620, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 5 - Cast Punch Me'),
(34523, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65629, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 6 - Cast Nasty'),
(34523, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 87, 3452301, 3452302, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 7 - Run Random Script'),
(34523, 0, 9, 10, 62, 0, 100, 0, 10521, 2, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Gossip Select - Store Target List'),
(34523, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 9 - Close Gossip'),
(34523, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 65619, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 10 - Cast Kick Me'),
(34523, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65629, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 11 - Cast Nasty'),
(34523, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 87, 3452301, 3452302, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 12 - Run Random Script'),
(34523, 0, 14, 15, 62, 0, 100, 0, 10521, 3, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Gossip Select - Store Target List'),
(34523, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 14 - Close Gossip'),
(34523, 0, 16, 17, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 65623, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 15 - Cast Bow to Me'),
(34523, 0, 17, 18, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65628, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 16 - Cast Saintly'),
(34523, 0, 18, 19, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 17 - Say Line 2'),
(34523, 0, 19, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3452303, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - LInked To Id 18 - Run Script'),
(34523, 0, 20, 21, 62, 0, 100, 0, 10521, 4, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Gossip Select - Store Target List'),
(34523, 0, 21, 22, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 20 - Close Gossip'),
(34523, 0, 22, 23, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 65621, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 21 - Cast Tickle Me'),
(34523, 0, 23, 24, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65628, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - LInked To Id 22 - Cast Saintly'),
(34523, 0, 24, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3452304, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Linked To Id 23 - Run Script'),
(34754, 0, 0, 1, 62, 0, 100, 0, 10556, 0, 0, 0, 0, 56, 46827, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer Foote - Create Item Ship Schematics'),
(34754, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer Foote - Gossip Closed'),
(44057, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 44054, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Riverboat - On Spellclick - Give Kill Credit'),
(52171, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 52166, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Muddy Tracks - On Spellclick - Quest Credit'),
(52171, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Muddy Tracks - Linked To Id 0 - Say Line Random'),
(52171, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Muddy Tracks - Linked To Id 1 - Despawn'),
(3452301, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Say Line 0'),
(3452302, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Remove NpcFlag 1'),
(3452302, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Say Line 5'),
(3452302, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 34529, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Quest Credit'),
(3452302, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Say Line 6'),
(3452302, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Add NpcFlag 1'),
(3452303, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Remove NpcFlag 1'),
(3452303, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 71, 0, 0, 2202, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Change Equip'),
(3452303, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Say Line 3'),
(3452303, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 34529, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Quest Credit'),
(3452303, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Say Line 6'),
(3452303, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Add NpcFlag 1'),
(3452303, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 71, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Change Equip'),
(3452304, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Remove NpcFlag 1'),
(3452304, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - Say Line 4'),
(3452304, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 33, 34529, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Quest Credit'),
(3452304, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Say Line 6'),
(3452304, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Razormane - On Script - Add NpcFlag 1');

-- Lines the scripts above say. Muddy Tracks lines are shown as emotes (type 16).
DELETE FROM `creature_text` WHERE `entry` IN (34523, 52171);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34523, 0, 0, 'Ow! <Snort>', 12, 0, 100, 34, 0, 0, 'Captured Razormane'),
(34523, 0, 1, 'Unf. You learn nothing from me!', 12, 0, 100, 34, 0, 0, 'Captured Razormane'),
(34523, 0, 2, 'Unf - Not the face!', 12, 0, 100, 34, 0, 0, 'Captured Razormane'),
(34523, 0, 3, 'Oof! I not talking!', 12, 0, 100, 34, 0, 0, 'Captured Razormane'),
(34523, 0, 4, '<Snort!> That all you got?', 12, 0, 100, 34, 0, 0, 'Captured Razormane'),
(34523, 0, 5, 'Ow-oooh! I not talk for nothing!', 12, 0, 100, 34, 0, 0, 'Captured Razormane'),
(34523, 1, 0, '<Snort!>', 12, 0, 100, 0, 0, 0, 'Captured Razormane'),
(34523, 1, 1, 'I say nothing!', 12, 0, 100, 0, 0, 0, 'Captured Razormane'),
(34523, 1, 2, 'I not scared of you!', 12, 0, 100, 0, 0, 0, 'Captured Razormane'),
(34523, 1, 3, 'I say nothing! <snort>', 12, 0, 100, 0, 0, 0, 'Captured Razormane'),
(34523, 2, 0, 'The %s warily takes the offered food.', 16, 0, 100, 0, 0, 0, 'Captured Razormane'),
(34523, 3, 0, 'Mnn, <snort> why you cook this meat? Tortusk tells us meat taste best when stolen and raw.', 12, 0, 100, 0, 0, 0, 'Captured Razormane'),
(34523, 4, 0, 'Haha - <snort> ha - heee! <snort> Haha no stop haha <SNORT> <wheeze> Hee hee! Battlemaster Tortusk never prepared us for this!', 12, 0, 100, 0, 0, 1179, 'Captured Razormane'),
(34523, 5, 0, 'You waste your strength! Tortusk train us warriors never to talk.', 12, 0, 100, 0, 0, 0, 'Captured Razormane'),
(34523, 6, 0, 'Oh - I''ve said too much!', 12, 0, 100, 0, 0, 0, ''),
(52171, 0, 0, 'Grass, sand, and sludge mix together to form the vague resemblance of a footprint.', 16, 0, 100, 0, 0, 0, 'Muddy Tracks'),
(52171, 0, 1, 'The tracks are smeared, almost as if the creature drags its feet.', 16, 0, 100, 0, 0, 0, 'Muddy Tracks'),
(52171, 0, 2, 'Mixed in with the mud are traces of oily sludge from the nearby rig.', 16, 0, 100, 0, 0, 0, 'Muddy Tracks'),
(52171, 0, 3, 'This trail is bloody. You follow it to a nearby bush, where you find the remains of the creature''s last meal.', 16, 0, 100, 0, 0, 0, 'Muddy Tracks'),
(52171, 0, 4, 'These tracks are fresh, and they lead down into the water...', 16, 0, 100, 0, 0, 0, 'Muddy Tracks');

-- The interrogation options of Captured Razormane only show while By Hook Or By Crook is active (the food and
-- tickle options also need the target not to be under the "Nasty" effect).
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 10521;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 10521, 0, 0, 0, 9, 0, 13963, 0, 0, 0, 0, 0, '', 'Show gossip if player have quest 13963'),
(15, 10521, 1, 0, 0, 9, 0, 13963, 0, 0, 0, 0, 0, '', 'Show gossip if player have quest 13963'),
(15, 10521, 2, 0, 0, 9, 0, 13963, 0, 0, 0, 0, 0, '', 'Show gossip if player have quest 13963'),
(15, 10521, 3, 0, 0, 1, 0, 65629, 0, 0, 1, 0, 0, '', 'Show gossip if target not have aura Nasty'),
(15, 10521, 3, 0, 0, 9, 0, 13963, 0, 0, 0, 0, 0, '', 'Show gossip if player have quest 13963'),
(15, 10521, 4, 0, 0, 1, 0, 65629, 0, 0, 1, 0, 0, '', 'Show gossip if target not have aura Nasty'),
(15, 10521, 4, 0, 0, 9, 0, 13963, 0, 0, 0, 0, 0, '', 'Show gossip if player have quest 13963');

-- Savannah Prowler (72 spawns) and Kurak had no damage and no attack speed, so they never hurt anyone. Values follow
-- the other level 14 to 15 beasts of the Barrens (22 to 29 damage, 54 attack power) with cat attack speed 1500 for
-- the Prowler.
UPDATE `creature_template` SET `mindmg` = 22, `maxdmg` = 29, `attackpower` = 54, `baseattacktime` = 1500 WHERE `entry` = 3425;
UPDATE `creature_template` SET `mindmg` = 22, `maxdmg` = 29, `attackpower` = 54, `baseattacktime` = 2000 WHERE `entry` = 34635;
