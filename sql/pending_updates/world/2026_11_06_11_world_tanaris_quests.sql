-- Tanaris quest fixes (Horde side first, then neutral and Alliance).
-- Sources: 5.4.8 data for the spawns, SmartAI rows, action lists, creature text, gossip and condition rows, template
-- fields and loot chances of the Centipaar rows; 4.3.4 for the quest item loot chances of the Hyena Chunk, Fistful
-- of Blood and Turtle-Digested Key rows (its quest-only 75 and 90 percent rows, written here with the quest-only sign
-- of this table). Where 5.4.8 SmartAI uses action 203 (combat stop) it is written as 27 in this core.
-- Safe to run twice: every block deletes its own keys first or sets absolute values.

-- Quest NPCs that were never spawned, 5.4.8 positions: Megs Dreadshredder and the two Kelsey Steelspark entries
-- (Lost Rigger Cove and Land's End Beach: they give and end the Lost Rigger Cove and Land's End quests, and Megs ends
-- "Momentum"), the Antechamber Guardian (the kill objective of "What Lies Within", with its 5.4.8 patrol path),
-- Williden Marshal (ends "Tropical Paradise Beckons", Un'Goro Crater) and Mazoga's Spirit (ends "Into Zul'Farrak",
-- inside the dungeon map with the spawn mask the other spawns of that map use).
DELETE FROM `creature` WHERE `guid` BETWEEN 8331600 AND 8331605;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8331600, 38703, 1, 1, 0, 0, 0, 0, -7954.87, -5245.22, 1.33514, 5.06145, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Megs Dreadshredder
(8331601, 38704, 1, 1, 0, 0, 0, 0, -7945.07, -5270.22, 0.852259, 1.85005, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Kelsey Steelspark
(8331602, 39059, 1, 1, 0, 0, 0, 0, -9758.17, -3672.51, 10.865, 1.36335, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Kelsey Steelspark
(8331603, 39077, 1, 1, 0, 0, 0, 0, -9773.95, -2790.01, 9.62328, 3.28833, 60, 0, 0, 0, 0, 2, 0, 0, 0), -- Antechamber Guardian
(8331604, 9270, 1, 1, 0, 0, 0, 0, -7501.73, -1504.57, -266.16, 1.36136, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Williden Marshal
(8331605, 40712, 209, 2, 0, 0, 0, 0, 1229.18, 839.988, 9.01394, 3.01942, 7200, 0, 0, 0, 0, 0, 0, 0, 0); -- Mazoga's Spirit

DELETE FROM `creature_addon` WHERE `guid` = 8331603;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(8331603, 83316030, 0, 0, 0, 0, NULL);

DELETE FROM `waypoint_data` WHERE `id` = 83316030;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(83316030, 1, -9771.15, -2789.68, 9.62311, 0, 0, 0, 0, 100, 0),
(83316030, 2, -9772.15, -2789.75, 9.62311, 0, 0, 0, 0, 100, 0),
(83316030, 3, -9775.54, -2790.25, 9.62343, 0, 0, 0, 0, 100, 0),
(83316030, 4, -9792.99, -2792.83, 9.62144, 0, 0, 0, 0, 100, 0),
(83316030, 5, -9795.73, -2793.23, 10.3907, 0, 0, 0, 0, 100, 0),
(83316030, 6, -9803.48, -2794.38, 10.3907, 0, 0, 0, 0, 100, 0),
(83316030, 7, -9805.12, -2794.62, 10.3907, 0, 0, 0, 0, 100, 0),
(83316030, 8, -9808.11, -2795.06, 10.3445, 0, 0, 0, 0, 100, 0),
(83316030, 9, -9808.11, -2795.06, 10.3445, 0, 0, 0, 0, 100, 0);

-- The three Lost Rigger Cove and Land's End quest NPCs had no gossip menu in the template; the menus exist.
UPDATE `creature_template` SET `gossip_menu_id` = 11176 WHERE `entry` = 38703 AND `gossip_menu_id` = 0;
UPDATE `creature_template` SET `gossip_menu_id` = 11141 WHERE `entry` IN (38704, 39059) AND `gossip_menu_id` = 0;

-- Quest items with no source: Hyena Chunk ("Chicken of the Desert"), Fistful of Blood ("Blood to Thrive"),
-- Turtle-Digested Key ("Terrapination") from 4.3.4 (quest-only drops), Centipaarts ("Get The Centipaarty Started")
-- from 5.4.8. Negative chance is the quest-only sign of this table.
DELETE FROM `creature_loot_template` WHERE (`entry`, `item`) IN ((5427, 51778), (5645, 52064), (5646, 52064), (5647, 52064), (5431, 52282), (5455, 52041), (5458, 52041), (5459, 52041), (5460, 52041));
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(5427, 51778, -75, 1, 0, 1, 1),
(5645, 52064, -90, 1, 0, 1, 1),
(5646, 52064, -90, 1, 0, 1, 1),
(5647, 52064, -90, 1, 0, 1, 1),
(5431, 52282, -75, 1, 0, 1, 1),
(5455, 52041, -89.5178, 1, 0, 1, 1),
(5458, 52041, -83.4869, 1, 0, 1, 1),
(5459, 52041, -43.4437, 1, 0, 1, 1),
(5460, 52041, -80.3412, 1, 0, 1, 1);

-- "Butcherbot", "Scavengers Scavenged" and "Blisterpaw Butchery": the Butcherbot Control Gizmo (spell 74175) hits a
-- basilisk, hyena or fire roc, which summons the Butcherbot, gives the harvest credit and despawns the animal. The
-- three animals had only their combat row.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (5419, 5426, 5429) AND `id` BETWEEN 1 AND 3;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5419, 0, 1, 2, 8, 0, 100, 0, 74175, 0, 0, 0, 0, 11, 74176, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Glasshide Basilisk'),
(5419, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39702, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Glasshide Basilisk'),
(5419, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Glasshide Basilisk');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5426, 0, 1, 2, 8, 0, 100, 0, 74175, 0, 0, 0, 0, 11, 74176, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blisterpaw Hyena'),
(5426, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 40509, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blisterpaw Hyena'),
(5426, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blisterpaw Hyena');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5429, 0, 1, 2, 8, 0, 100, 0, 74175, 0, 0, 0, 0, 11, 74176, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fire Roc'),
(5429, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 40507, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fire Roc'),
(5429, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fire Roc');

-- "To The Ground!" (both factions): the Burning Rum (spell 72520) hits the three building bunnies, which give the
-- objective credit and light themselves. The bunnies had no script.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (38660, 38662, 38665) AND `AIName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (38660, 38662, 38665) AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38660, 0, 0, 1, 8, 0, 100, 0, 72520, 0, 25000, 25000, 0, 33, 38660, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lost Rigger Building 01 Bunny - On SpellHit Throw Burning Rum - Quest Credit'),
(38660, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 75, 42346, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lost Rigger Building 01 Bunny - Linked To Id 0 - Add Aura Cosmetic - Flame Patch 2.0');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38662, 0, 0, 1, 8, 0, 100, 0, 72520, 0, 25000, 25000, 0, 33, 38662, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lost Rigger Building 02 Bunny - On SpellHit Throw Burning Rum - Quest Credit'),
(38662, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 75, 42346, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lost Rigger Building 02 Bunny - Linked To Id 0 - Add Aura Cosmetic - Flame Patch 2.0');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38665, 0, 0, 1, 8, 0, 100, 0, 72520, 0, 25000, 25000, 0, 33, 38665, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lost Rigger Building 03 Bunny - On SpellHit Throw Burning Rum - Quest Credit'),
(38665, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 75, 42346, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lost Rigger Building 03 Bunny - Linked To Id 0 - Add Aura Cosmetic - Flame Patch 2.0');

-- "Un-Chartered" (sign the charter, gossip option on the Dunemaul ogres) and "Maul 'Em With Kindness" (the morsel,
-- spell 73068, makes an ogre friendly and gives the credit): the five Dunemaul templates had no gossip flag, no gossip
-- menu and no script rows for either quest. The gossip option needs the quest taken.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 11093 WHERE `entry` IN (5471, 5472, 5473, 5474, 5475) AND `gossip_menu_id` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 11093 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11093, 0, 0, 0, 9, 0, 24955, 0, 0, 0, 0, 0, '', 'Dunemaul gossip option needs quest 24955 taken');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` = 5471 AND `id` BETWEEN 2 AND 14) OR (`entryorguid` = 5472 AND `id` BETWEEN 1 AND 13) OR (`entryorguid` = 5473 AND `id` BETWEEN 4 AND 16) OR (`entryorguid` = 5474 AND `id` BETWEEN 1 AND 13) OR (`entryorguid` = 5475 AND `id` BETWEEN 3 AND 15));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5471, 0, 2, 3, 8, 0, 100, 0, 73068, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - On SpellHit Throw Morsel - Set Faction 35'),
(5471, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39073, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 2 - Quest Credit'),
(5471, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 3 - Combat Stop'),
(5471, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 4 - Play Emote 7'),
(5471, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 5471, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 5 - Create Timed Event'),
(5471, 0, 7, 8, 59, 0, 100, 0, 5471, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - On Timed Event Triggered - Say Line 1'),
(5471, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 7 - Despawn after 3 sec'),
(5471, 0, 9, 10, 62, 0, 100, 0, 11093, 0, 0, 0, 0, 33, 38848, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - On Gossip Select - Quest Credit'),
(5471, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 9 - Close Gossip'),
(5471, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 10 - Remove NpcFlag 1'),
(5471, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 11 - Set Orientation'),
(5471, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 12 - Say Line Random'),
(5471, 0, 14, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre - Linked To Id 13 - Despawn after 3 sec');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5472, 0, 1, 2, 8, 0, 100, 0, 73068, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - On SpellHit Throw Morsel - Set Faction 35'),
(5472, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39073, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 1 - Quest Credit'),
(5472, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 3 - Combat Stop'),
(5472, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 4 - Play Emote 7'),
(5472, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 5472, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 4 - Create Timed Event'),
(5472, 0, 6, 7, 59, 0, 100, 0, 5472, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - On Timed Event Triggered - Say Line 1'),
(5472, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 6 - Despawn after 3 sec'),
(5472, 0, 8, 9, 62, 0, 100, 0, 11093, 0, 0, 0, 0, 33, 38848, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - On Gossip Select - Quest Credit'),
(5472, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 8 - Close Gossip'),
(5472, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 9 - Remove NpcFlag 1'),
(5472, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 10 - Set Orientation'),
(5472, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 11 - Say Line Random'),
(5472, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Enforcer - Linked To Id 12 - Despawn after 5 sec');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5473, 0, 4, 5, 8, 0, 100, 0, 73068, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - On SpellHit Throw Morsel - Set Faction 35'),
(5473, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39073, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 4 - Quest Credit'),
(5473, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 5 - Combat Stop'),
(5473, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 6 - Play Emote 7'),
(5473, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 5473, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 7 - Create Timed Event'),
(5473, 0, 9, 10, 59, 0, 100, 0, 5473, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - On Timed Event Triggered - Say Line 1'),
(5473, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 9 - Despawn after 3 sec'),
(5473, 0, 11, 12, 62, 0, 100, 0, 11093, 0, 0, 0, 0, 33, 38848, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - On Gossip Select - Quest Credit'),
(5473, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 11 - Close Gossip'),
(5473, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 12 - Remove NpcFlag 1'),
(5473, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 13 - Set Orientation'),
(5473, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 14 - Say Line Random'),
(5473, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Ogre Mage - Linked To Id 15 - Despawn after 5 sec');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5474, 0, 1, 2, 8, 0, 100, 0, 73068, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - On SpellHit Throw Morsel - Set Faction 35'),
(5474, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39073, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 1 - Quest Credit'),
(5474, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 2 - Combat Stop'),
(5474, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 3 - Play Emote 7'),
(5474, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 5474, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 4 - Create Timed Event'),
(5474, 0, 6, 7, 59, 0, 100, 0, 5474, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - On Timed Event Triggered - Say Line 1'),
(5474, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 6 - Despawn after 3 sec'),
(5474, 0, 8, 9, 62, 0, 100, 0, 11093, 0, 0, 0, 0, 33, 38848, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - On Gossip Select - Quest Credit'),
(5474, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 8 - Close Gossip'),
(5474, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 9 - Remove NpcFlag 1'),
(5474, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 10 - Set Orientation'),
(5474, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 11 - Say Line Random'),
(5474, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Brute - Linked To Id 12 - Despawn after 5 sec');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5475, 0, 3, 4, 8, 0, 100, 0, 73068, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - On SpellHit Throw Morsel - Set Faction 35'),
(5475, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39073, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 3 - Quest Credit'),
(5475, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 4 - Combat Stop'),
(5475, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 5 - Play Emote 7'),
(5475, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 5475, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 6 - Create Timed Event'),
(5475, 0, 8, 9, 59, 0, 100, 0, 5475, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - On Timed Event Triggered - Say Line 1'),
(5475, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 8 - Despawn after 3 sec'),
(5475, 0, 10, 11, 62, 0, 100, 0, 11093, 0, 0, 0, 0, 33, 38848, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - On Gossip Select - Quest Credit'),
(5475, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 10 - Close Gossip'),
(5475, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 11 - Remove NpcFlag 1'),
(5475, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 12 - Set Orientation'),
(5475, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 13 - Say Line Random'),
(5475, 0, 15, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dunemaul Warlock - Linked To Id 14 - Despawn after 5 sec');
-- Creature text of the five ogres: group 1 (thanks for the morsel), group 2 (random lines after signing); group 0 is the
-- aggro line the existing rows of the Ogre Mage and the Warlock already call.
DELETE FROM `creature_text` WHERE `entry` IN (5471, 5472, 5473, 5474, 5475) AND (`groupid` IN (1, 2) OR (`entry` IN (5473, 5475) AND `groupid` = 0));
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(5471, 1, 0, 'Me sorry! No hurt!', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5471, 2, 0, 'Yay!', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5471, 2, 1, 'Thank you, funny ogre!', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5471, 2, 2, 'Me not understand.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5471, 2, 3, 'Read make head hurt.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5471, 2, 4, '...me forget what you said.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5471, 2, 5, 'Okay. Me sign.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5471, 2, 6, 'Me still hungry.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5471, 2, 7, 'We get tabard?', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre'),
(5472, 1, 0, 'Me sorry! No hurt!', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5472, 2, 0, 'Yay!', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5472, 2, 1, 'Thank you, funny ogre!', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5472, 2, 2, 'Me not understand.', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5472, 2, 3, 'Read make head hurt.', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5472, 2, 4, '...me forget what you said.', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5472, 2, 5, 'Okay. Me sign.', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5472, 2, 6, 'Me still hungry.', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5472, 2, 7, 'We get tabard?', 12, 0, 100, 0, 0, 0, 'Dunemaul Enforcer'),
(5473, 0, 0, 'I''ll crush you!', 12, 0, 50, 0, 0, 0, 'on Aggro Text'),
(5473, 0, 1, 'Me smash! You die!', 12, 0, 50, 0, 0, 0, 'on Aggro Text'),
(5473, 0, 2, 'Raaar!!! Me smash $r!', 12, 0, 50, 0, 0, 0, 'on Aggro Text'),
(5473, 1, 0, 'Me sorry! No hurt!', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5473, 2, 0, 'Yay!', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5473, 2, 1, 'Thank you, funny ogre!', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5473, 2, 2, 'Me not understand.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5473, 2, 3, 'Read make head hurt.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5473, 2, 4, '...me forget what you said.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5473, 2, 5, 'Okay. Me sign.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5473, 2, 6, 'Me still hungry.', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5473, 2, 7, 'We get tabard?', 12, 0, 100, 0, 0, 0, 'Dunemaul Ogre Mage'),
(5474, 1, 0, 'Me sorry! No hurt!', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5474, 2, 0, 'Yay!', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5474, 2, 1, 'Thank you, funny ogre!', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5474, 2, 2, 'Me not understand.', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5474, 2, 3, 'Read make head hurt.', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5474, 2, 4, '...me forget what you said.', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5474, 2, 5, 'Okay. Me sign.', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5474, 2, 6, 'Me still hungry.', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5474, 2, 7, 'We get tabard?', 12, 0, 100, 0, 0, 0, 'Dunemaul Brute'),
(5475, 0, 0, 'I''ll crush you!', 12, 0, 50, 0, 0, 0, 'on Aggro Text'),
(5475, 0, 1, 'Me smash! You die!', 12, 0, 50, 0, 0, 0, 'on Aggro Text'),
(5475, 0, 2, 'Raaar!!! Me smash $r!', 12, 0, 50, 0, 0, 0, 'on Aggro Text'),
(5475, 1, 0, 'Me sorry! No hurt!', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock'),
(5475, 2, 0, 'Yay!', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock'),
(5475, 2, 1, 'Thank you, funny ogre!', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock'),
(5475, 2, 2, 'Me not understand.', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock'),
(5475, 2, 3, 'Read make head hurt.', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock'),
(5475, 2, 4, '...me forget what you said.', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock'),
(5475, 2, 5, 'Okay. Me sign.', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock'),
(5475, 2, 6, 'Me still hungry.', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock'),
(5475, 2, 7, 'We get tabard?', 12, 0, 100, 0, 0, 0, 'Dunemaul Warlock');

-- "A Great Idea": the Bootlegger Bug Bait (spell 72659) summons the Silithid Bait, which sends the player to the
-- nearest Hazzali Swarmer; the swarmer becomes friendly, turns into the wrangled model and follows the player, and gives
-- the credit when it gets within 10 yards of Zeke Bootscuff. The bait had no script and the swarmer only its two combat
-- rows. The follow-up quest "Just Trying to Kill Some Bugs" is not fixed here.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `faction_A` = 1375, `faction_H` = 1375, `unit_flags` = 256 WHERE `entry` = 38718;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 5451 AND `id` BETWEEN 2 AND 5;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 38718 AND `id` BETWEEN 0 AND 2;
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 3871800 AND `id` BETWEEN 0 AND 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5451, 0, 2, 3, 38, 0, 100, 1, 1, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hazzali Swarmer'),
(5451, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 3, 38739, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hazzali Swarmer'),
(5451, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 38706, 38742, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Hazzali Swarmer');
-- The 5.4.8 distance row that also gave this credit is left out: the follow above already credits the player when the
-- swarmer reaches Zeke, and the extra row credited twice.
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38718, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Bait'),
(38718, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Silithid Bait'),
(38718, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3871800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Bait');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3871800, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 100, 1, 0, 0, 0, 0, 0, 19, 5451, 10, 0, 0, 0, 0, 0, 'Silithid Bait'),
(3871800, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 19, 5451, 10, 0, 0, 0, 0, 0, 'Silithid Bait'),
(3871800, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silithid Bait');
