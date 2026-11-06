-- Teldrassil, Darnassus and Darkshore quest fixes (Alliance).
-- Sources: 4.3.4 data for the Nature's Reprisal credit rows and the Oakenscowl spawn; 5.4.8 data for the other spawns,
-- SmartAI rows, action lists and fishing loot (4.3.4 has none of them). Positions and rows are taken as they are in
-- those databases; the one change is that SmartAI rows on creatures that already have rows get new row ids (10 to 12).
-- 4.3.4 event flag 513 is written as 1 (the 0x200 bit does not exist in this core), the same as the Shadow Sprite row.
-- Safe to run twice: every block deletes its own keys first or sets absolute values.

-- Quest NPCs and kill targets that were never spawned. Oakenscowl (drops the Gargantuan Tumor of "Oakenscowl", 4.3.4
-- position), Genn Greymane (ends "The Howling Oak", starts "Breaking Waves of Change" for worgen, 5.4.8 position in
-- Darnassus), the four survivors of "The Last Wave of Survivors" (Gershala Nightwhisper, Cerellean Whiteclaw, Volcor,
-- Shaldyn) and the Whiteclaw and Volcor quest givers of "A Love Eternal", "A Lost Companion" and "Remembrance of
-- Auberdine", Malfurion Stormrage (starts "The Battle for Darkshore" and ends "The Offering to Azshara") and the Avatar
-- of Soggoth (its kill objective). All 5.4.8 positions, phase 0, spawn mask 1.
DELETE FROM `creature` WHERE `guid` BETWEEN 8332800 AND 8332809;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8332800, 2166, 1, 1, 0, 0, 0, 0, 10469.4, 1451.67, 1327.55, 0.128997, 300, 10, 0, 0, 0, 1, 0, 0, 0), -- Oakenscowl
(8332801, 48736, 1, 1, 0, 0, 0, 0, 10316.4, 2446.59, 1330.51, 2.94961, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Genn Greymane
(8332802, 32911, 1, 1, 0, 0, 0, 0, 7455.9, 164.987, 2.81793, 5.96903, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Gershala Nightwhisper
(8332803, 33093, 1, 1, 0, 0, 0, 0, 7365.81, 134.712, 11.7647, 2.96706, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Cerellean Whiteclaw (survivor form)
(8332804, 33094, 1, 1, 0, 0, 0, 0, 7439.48, 106.227, 0.362043, 5.96903, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Volcor (survivor form)
(8332805, 33095, 1, 1, 0, 0, 0, 0, 7295.92, 242.54, 1.79581, 4.97419, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Shaldyn
(8332806, 32959, 1, 1, 0, 0, 0, 0, 7384.1, -223.969, 8.78887, 4.50295, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Cerellean Whiteclaw
(8332807, 32960, 1, 1, 0, 0, 0, 0, 7445.88, -276.52, 13.7653, 1.58825, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Volcor
(8332808, 34422, 1, 1, 0, 0, 0, 0, 4558.18, 883.464, 33.5867, 4.50651, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Malfurion Stormrage
(8332809, 34485, 1, 1, 0, 0, 0, 0, 4586.9, 382.762, 18.5828, 6.05531, 60, 4, 0, 0, 0, 1, 0, 0, 0); -- Avatar of Soggoth

-- The four survivors and the supply credit bunny had no AI and no gossip menu in the template; the menus exist.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (32911, 33093, 33094, 33095, 33056) AND `AIName` = '';
UPDATE `creature_template` SET `gossip_menu_id` = 10296 WHERE `entry` = 32911 AND `gossip_menu_id` = 0;
UPDATE `creature_template` SET `gossip_menu_id` = 10293 WHERE `entry` = 33093 AND `gossip_menu_id` = 0;
UPDATE `creature_template` SET `gossip_menu_id` = 10294 WHERE `entry` = 33094 AND `gossip_menu_id` = 0;
UPDATE `creature_template` SET `gossip_menu_id` = 10295 WHERE `entry` = 33095 AND `gossip_menu_id` = 0;

-- "The Last Wave of Survivors" (talking to each survivor gives the credit) and "War Supplies" (the Sentinel Torch,
-- spell 62624, burns a supply stack and gives the credit).
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (32911, 33093, 33094, 33095, 33056);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(32911, 0, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 33, 32911, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gershala Nightwhisper - On Gossip Hello - Quest Credit'),
(33056, 0, 0, 1, 8, 0, 100, 1, 62624, 0, 0, 0, 0, 33, 33056, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spellhit 62624 - kill credit 33056'),
(33056, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 1, 20000, 20000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On spellhit 62624 - trigger timed event 1 after 20 seconds'),
(33056, 0, 2, 0, 59, 0, 100, 0, 1, 0, 0, 0, 0, 78, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On timed event 1 triggered - call script reset'),
(33093, 0, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 33, 33093, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cerellean Whiteclaw - On Gossip Hello - Quest Credit'),
(33094, 0, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 33, 33094, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Volcor - On Gossip Hello - Quest Credit'),
(33095, 0, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 33, 33095, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shaldyn - On Gossip Hello - Quest Credit');

-- "An Ounce of Prevention": the Lifebringer Sapling (spell 65188) on a corrupted bear, matriarch, sire or stag makes it
-- friendly, gives the credit and despawns it. New row ids 10 to 12 because the four animals already use ids 0 and 1.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (2071, 2165, 2237, 34318) AND `id` BETWEEN 10 AND 12;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2071, 0, 10, 11, 8, 0, 100, 0, 65188, 0, 10000, 10000, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - On SpellHit Darkshore Protection - Store Target List'),
(2071, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - Linked To Id 10 - Attack Start'),
(2071, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 207100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - Linked To Id 11 - Run Script'),
(2165, 0, 10, 11, 8, 0, 100, 0, 65188, 0, 10000, 10000, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - On SpellHit Darkshore Protection - Store Target List'),
(2165, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - Linked To Id 10 - Attack Start'),
(2165, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 216500, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - Linked To Id 11 - Run Script'),
(2237, 0, 10, 11, 8, 0, 100, 0, 65188, 0, 10000, 10000, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - On SpellHit Darkshore Protection - Store Target List'),
(2237, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - Linked To Id 10 - Attack Start'),
(2237, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 223700, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - Linked To Id 11 - Run Script'),
(34318, 0, 10, 11, 8, 0, 100, 0, 65188, 0, 10000, 10000, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - On SpellHit Darkshore Protection - Store Target List'),
(34318, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - Linked To Id 10 - Attack Start'),
(34318, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3431800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - Linked To Id 11 - Run Script');

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (207100, 216500, 223700, 3431800);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(207100, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - On Script - Set Faction 35'),
(207100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65193, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - On Script - Cast Protected from the Consumption'),
(207100, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - On Script - Evade'),
(207100, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34373, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - On Script - Quest Credit'),
(207100, 9, 4, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - On Script - Despawn after 10 sec'),
(216500, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - On Script - Set Faction 35'),
(216500, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65193, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - On Script - Cast Protected from the Consumption'),
(216500, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - On Script - Evade'),
(216500, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34373, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - On Script - Quest Credit'),
(216500, 9, 4, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - On Script - Despawn after 10 sec'),
(223700, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - On Script - Set Faction 35'),
(223700, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65193, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - On Script - Cast Protected from the Consumption'),
(223700, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - On Script - Evade'),
(223700, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34373, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - On Script - Quest Credit'),
(223700, 9, 4, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - On Script - Despawn after 10 sec'),
(3431800, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - On Script - Set Faction 35'),
(3431800, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65193, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - On Script - Cast Protected from the Consumption'),
(3431800, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - On Script - Evade'),
(3431800, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34373, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - On Script - Quest Credit'),
(3431800, 9, 4, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - On Script - Despawn after 10 sec');

-- "Nature's Reprisal": Ireroot Seeds (spell 65455) on a Rascal Sprite, Dark Sprite or Vicious Grell gave no credit;
-- only the Shadow Sprite had the row.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (2002, 2004, 2005) AND `id` = 1 AND `event_type` = 8;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2002, 0, 1, 0, 8, 0, 100, 1, 65455, 0, 0, 0, 0, 33, 34440, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Rascal Sprite - on spell hit (65455) - give quest credit (34440)'),
(2004, 0, 1, 0, 8, 0, 100, 1, 65455, 0, 0, 0, 0, 33, 34440, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dark Sprite - on spell hit (65455) - give quest credit (34440)'),
(2005, 0, 1, 0, 8, 0, 100, 1, 65455, 0, 0, 0, 0, 33, 34440, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vicious Grell - On spell hit - Kill credit');

-- "Stocking Up" and "An Old Favorite" (Darnassus fishing dailies) ask for Lake Whitefish and Kaldorei Herring, which no
-- loot gave. 5.4.8 fishing rows: the whitefish is a quest-only catch in the Darnassus zone, the herring a normal catch in
-- Teldrassil and a quest-only catch at Rut'theran Village (area 702). The Baby Octopus and Aquinne's Moon Pendant have
-- no source in any database and are left.
DELETE FROM `fishing_loot_template` WHERE (`entry` = 1657 AND `item` = 69912) OR (`entry` = 141 AND `item` = 69987) OR (`entry` = 702 AND `item` = 69987);
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(1657, 69912, -15.4109, 1, 0, 1, 1),
(141, 69987, 25.6432, 1, 0, 1, 1),
(702, 69987, -35, 1, 0, 1, 1);
