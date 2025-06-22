-- Riverpaw Scout
DELETE FROM `creature_loot_template` WHERE `entry`=500 AND `item`=57755;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(500, 57755, -20, 1, 0, 1, 1);

-- Riverpaw Gnoll
DELETE FROM `creature_loot_template` WHERE `entry`=117 AND `item`=57755;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(117, 57755, -20, 1, 0, 1, 1);

-- Murloc Raider
DELETE FROM `creature_loot_template` WHERE `entry`=515 AND `item`=57756;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(515, 57756, -20, 1, 0, 1, 1);

-- Murloc Coastrunner
DELETE FROM `creature_loot_template` WHERE `entry`=126 AND `item`=57756;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(126, 57756, -20, 1, 0, 1, 1);

-- First phase Lou Alive
DELETE FROM `spell_area` WHERE `spell`=78284 AND `area`=109 AND `quest_start`=26215 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(78284, 109, 26215, 26232, 0, 0, 2, 1, 66, 9);

-- Two-shoed Lou Spawn
DELETE FROM `creature` WHERE `guid`=100062;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(100062, 42560, 0, 1, 0, 0, 0, 0, -9855.68, 1277.57, 40.9636, 5.42797, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- Horatio Spawn
DELETE FROM `creature` WHERE `guid`=100063;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(100063, 42558, 0, 1, 0, 0, 0, 0, -9855.29, 1275.29, 40.9779, 1.41372, 120, 0, 0, 1, 0, 0, 0, 0, 0);

-- Two-shoed Lou Addon
DELETE FROM `creature_template_addon` WHERE `entry`=42560;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(42560, 0, 0, 3, 1, 0, '79343 79372 29266');

-- Horatio Addon
DELETE FROM `creature_template_addon` WHERE `entry`=42558;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(42558, 0, 0, 65544, 1, 0, '79343');

-- 2nd phase lou dead.
DELETE FROM `spell_area` WHERE `spell`=79343 AND `area`=109 AND `quest_start`=26232 AND `aura_spell`=0 AND `racemask`=0 AND `gender`=2;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(79343, 109, 26232, 0, 0, 0, 2, 1, 66, 11);

-- Lou dead flag disable powers
UPDATE `creature_template` SET `unit_flags`=536904448 WHERE `entry`=42560;

