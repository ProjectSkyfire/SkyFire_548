-- Teldrassil and Darnassus quest fixes (Alliance). The Oakenscowl, Darkshore survivor, Malfurion and Soggoth spawns and
-- the "An Ounce of Prevention" and "Nature's Reprisal" scripts that this file used to carry are in the main branch now.
-- Safe to run twice: every block deletes its own keys first.

-- Genn Greymane ends "The Howling Oak" and starts "Breaking Waves of Change" for worgen and was never spawned.
-- 5.4.8 position in Darnassus, phase 0, spawn mask 1.
DELETE FROM `creature` WHERE `guid` = 8332801;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8332801, 48736, 1, 1, 0, 0, 0, 0, 10316.4, 2446.59, 1330.51, 2.94961, 300, 0, 0, 0, 0, 0, 0, 0, 0); -- Genn Greymane

-- "Stocking Up" and "An Old Favorite" (Darnassus fishing dailies) ask for Lake Whitefish and Kaldorei Herring, which no
-- loot gave. 5.4.8 fishing rows: the whitefish is a quest-only catch in the Darnassus zone, the herring a normal catch in
-- Teldrassil and a quest-only catch at Rut'theran Village (area 702). The Baby Octopus and Aquinne's Moon Pendant have
-- no source in any database and are left.
DELETE FROM `fishing_loot_template` WHERE (`entry` = 1657 AND `item` = 69912) OR (`entry` = 141 AND `item` = 69987) OR (`entry` = 702 AND `item` = 69987);
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(1657, 69912, -15.4109, 1, 0, 1, 1),
(141, 69987, 25.6432, 1, 0, 1, 1),
(702, 69987, -35, 1, 0, 1, 1);
