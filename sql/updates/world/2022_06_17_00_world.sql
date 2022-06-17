-- Creature (Entry: 33292) has wrong spell 63029 defined in `auras` field in `creature_template_addon`.
DELETE FROM `creature_template_addon` WHERE entry=33292;

-- Creature (Entry: 57797) has invalid emote (106562) defined in `creature_template_addon`.
-- Fix wrong collumn for Cosmetic - Sleep Zzz Breakable 
DELETE FROM `creature_template_addon` WHERE entry=57797;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(57797, 0, 0, 0, 1, 0, '106562');

-- Creature (GUID: 43491) has wrong spell 25900 defined in `auras` field in `creature_addon`.
-- Har'koa at Zim'Torga: Invisibility & Mod Scale
DELETE FROM `creature_addon` WHERE guid=43491;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(43491, 0, 0, 0, 1, 0, '52483');

-- Creature (GUID: 118591) has wrong spell 25900 defined in `auras` field in `creature_addon`.
-- Har'koa at the Altar of Har'koa: Invisibility
-- Altar of Har'koa: See Har'koa at the Altar of Har'koa Invisibility
DELETE FROM `creature_addon` WHERE guid=118591;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(118591, 0, 0, 0, 1, 0, '51666 52485');


