-- Winter Veil npc's for stormwind
SET @CGUID:=2997;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+4;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID+0, 13436, 0, 1, 1, -8810.5, 635.925, 94.59804, 4.223697, 120, 0, 0),
(@CGUID+1, 13435, 0, 1, 1, -8807.18, 631.804, 94.50333, 3.612832, 120, 0, 0),
(@CGUID+2, 15732, 0, 1, 1, -8823.45, 544.083, 96.76003, 2.897247, 120, 0, 0),
(@CGUID+3, 15760, 0, 1, 1, -8866.352, 676.3616, 97.98639, 0.03490658, 120, 0, 0),
(@CGUID+4, 15760, 0, 1, 1, -8864.067, 675.9958, 97.98639, 2.879793, 120, 0, 0);

SET @CGUID:=2997;
DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+4;
INSERT INTO `creature_addon` (`guid`, `mount`, `bytes1`, `bytes2`, `auras`) VALUES
(@CGUID+0, 0, 0x0, 0x101, ''),
(@CGUID+1, 0, 0x0, 0x101, ''),
(@CGUID+2, 0, 0x0, 0x101, ''),
(@CGUID+3, 0, 0x0, 0x101, ''),
(@CGUID+4, 0, 0x0, 0x101, '');

DELETE FROM `creature_equip_template` WHERE `entry` in (15732, 13435, 13436, 15760);
INSERT INTO `creature_equip_template` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(15732, 1, 0x0, 0x200, 0x800),
(13436, 1, 0x0, 0x200, 0x800),
(13435, 1, 0x0, 0x200, 0x800),
(15760, 1, 0x0, 0x200, 0x800);

SET @CGUID:=2997;
SET @Event:=2;
DELETE FROM `game_event_creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+4 AND `eventEntry`=@Event;
INSERT INTO `game_event_creature` SELECT @Event, creature.guid FROM `creature` WHERE creature.guid BETWEEN @CGUID+0 AND @CGUID+4;