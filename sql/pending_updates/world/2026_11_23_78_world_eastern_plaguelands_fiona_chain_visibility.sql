-- The Northdale quest chain (Gidwin Goldbraids, Tarenar Sunstrike and Fiona, quests 27367 to 27373 and 27448) cannot be
-- followed. The travel companions stand in several places and appear one after the other through quest invisibility
-- (auras 49414, 49415 and 60921 on the NPC, detection auras 49416, 49417 and 60922 that an area grants by quest
-- state). Here only the invisibility existed and nothing granted the detection, so Fiona and Tarenar were invisible
-- to every player (Gidwin and Rimblat carried a different invisibility, also without detection), the two Gidwin
-- entries stood on top of each other at Northdale, and the later spots of Fiona, Tarenar and Gidwin had no spawn.
-- Added as in Legends of Azeroth 5.4.8: the six area auras, the later spots of Fiona (two), Tarenar and Gidwin, and
-- the invisibility of Tarenar's spawns (49414), of the Northdale Gidwin entry (60921) and of Gidwin Goldbraids
-- (49415). Rimblat Earthshatter's invisibility and Tarenar's other sniffed auras are removed, as the source has
-- neither. New spawn guids 9310650 to 9310653.

UPDATE `creature_template_addon` SET `auras` = '49415' WHERE `entry` = 45428 AND `auras` = '80852';
UPDATE `creature_template_addon` SET `auras` = NULL WHERE `entry` = 16134 AND `auras` = '80852';
UPDATE `creature_template_addon` SET `auras` = NULL WHERE `entry` = 45429 AND `auras` = '83305 79976 79977 79972';

DELETE FROM `creature` WHERE `guid` BETWEEN 9310650 AND 9310653;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310650, 45417, 0, 1, 0, 0, 0, 0, 1844.85, -3692.72, 158.708, 3.98634, 180, 0, 0, 0, 0, 0, 0, 0, 0),
(9310651, 45417, 0, 1, 0, 0, 0, 0, 2277.73, -4422.13, 111.741, 2.76431, 180, 0, 0, 0, 0, 0, 0, 0, 0),
(9310652, 45429, 0, 1, 0, 0, 0, 1, 1853.58, -3708.91, 160.586, 1.30816, 180, 0, 0, 0, 0, 0, 0, 0, 0),
(9310653, 45431, 0, 1, 0, 0, 0, 1, 1855.71, -3694.71, 158.502, 6.00091, 180, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_addon` WHERE `guid` IN (192964, 8316009, 9310652, 9310653);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(192964, 0, 0, 65536, 1, 0, '79976 49414'),
(8316009, 0, 0, 0, 0, 0, '60921'),
(9310652, 0, 0, 65536, 1, 0, '79976 49414'),
(9310653, 0, 0, 0, 0, 0, '49414');

DELETE FROM `spell_area` WHERE (`spell` IN (49416, 49417, 60922) AND `area` IN (2263, 2619, 5446, 5448));
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(49416, 2263, 27373, 27448, 0, 0, 2, 1, 66, 9),
(49416, 2619, 0, 27373, 0, 0, 2, 1, 64, 9),
(49416, 5446, 0, 27372, 0, 0, 2, 1, 64, 11),
(49416, 5448, 27448, 0, 0, 0, 2, 1, 66, 11),
(49417, 2619, 0, 27368, 0, 0, 2, 1, 64, 11),
(60922, 2619, 27368, 27369, 0, 0, 2, 1, 64, 11);
