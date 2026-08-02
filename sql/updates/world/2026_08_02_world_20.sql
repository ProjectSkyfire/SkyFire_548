-- Darkmoon Faire Ring Toss.

SET @DARKMOON_EVENT_ENTRY := 75;
SET @RING_TOSS_ORIGINAL_CREATURE_GUID := 8200050;
SET @RING_TOSS_ORIGINAL_PATH_ID := 303666;
SET @RING_TOSS_CREATURE_GUID_START := 8202000;
SET @RING_TOSS_CREATURE_GUID_COUNT := 2;
SET @RING_TOSS_EXISTING_CREATURE_GUID := 8981;
SET @RING_TOSS_EXISTING_PATH_ID := 8981;

START TRANSACTION;

DELETE FROM `creature_addon`
WHERE `guid` = 8200042;

DELETE FROM `game_event_creature`
WHERE `guid` = 8200042;

DELETE FROM `creature`
WHERE `guid` = 8200042
  AND `id` = 55278
  AND `map` = 974;

DELETE FROM `smart_scripts`
WHERE (`source_type` = 0 AND `entryorguid` IN (54485, 54490))
   OR (`source_type` = 9 AND `entryorguid` IN (5448500));

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_darkmoon_jessica_rogers'
WHERE `entry` = 54485;

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = ''
WHERE `entry` = 54490;

UPDATE `gameobject_template`
SET `data0` = 1729,
    `data1` = 80
WHERE `entry` = 210173
  AND `type` = 5;

INSERT IGNORE INTO `game_event_gameobject` (`eventEntry`, `guid`)
SELECT @DARKMOON_EVENT_ENTRY, `guid`
FROM `gameobject`
WHERE `id` = 210173
  AND `map` = 974;

DELETE FROM `spell_script_names`
WHERE `spell_id` IN (101695, 102058)
   OR `ScriptName` IN ('spell_darkmoon_ring_toss', 'spell_darkmoon_ring_toss_throw', 'spell_darkmoon_ring_toss_action');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101695, 'spell_darkmoon_ring_toss_throw'),
(102058, 'spell_darkmoon_ring_toss_action');

UPDATE `quest_template`
SET `RewardSpell` = 0,
    `RewardSpellCast` = 0
WHERE `Id` = 29455;

DELETE FROM `creature_addon`
WHERE `guid` BETWEEN @RING_TOSS_CREATURE_GUID_START AND (@RING_TOSS_CREATURE_GUID_START + @RING_TOSS_CREATURE_GUID_COUNT - 1);

DELETE FROM `game_event_creature`
WHERE `guid` BETWEEN @RING_TOSS_CREATURE_GUID_START AND (@RING_TOSS_CREATURE_GUID_START + @RING_TOSS_CREATURE_GUID_COUNT - 1);

DELETE FROM `creature`
WHERE `guid` BETWEEN @RING_TOSS_CREATURE_GUID_START AND (@RING_TOSS_CREATURE_GUID_START + @RING_TOSS_CREATURE_GUID_COUNT - 1);

DELETE FROM `waypoint_data`
WHERE `id` BETWEEN @RING_TOSS_CREATURE_GUID_START AND (@RING_TOSS_CREATURE_GUID_START + @RING_TOSS_CREATURE_GUID_COUNT - 1);

INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@RING_TOSS_CREATURE_GUID_START + 0, 54490, 974, 1, 0, 0, 39038, 0, -4290.30, 6301.60, 13.118, 3.60, 300, 0, 0, 1, 0, 2, 0, 0, 0),
(@RING_TOSS_CREATURE_GUID_START + 1, 54490, 974, 1, 0, 0, 39038, 0, -4299.40, 6289.40, 13.118, 1.00, 300, 0, 0, 1, 0, 2, 0, 0, 0);

UPDATE `creature`
SET `position_x` = -4302.40,
    `position_y` = 6299.20,
    `position_z` = 13.118,
    `orientation` = 5.10,
    `spawndist` = 0,
    `currentwaypoint` = 0,
    `MovementType` = 2
WHERE `guid` = @RING_TOSS_ORIGINAL_CREATURE_GUID
  AND `id` = 54490
  AND `map` = 974;

UPDATE `creature`
SET `position_x` = -4294.20,
    `position_y` = 6302.80,
    `position_z` = 13.118,
    `orientation` = 3.80,
    `spawndist` = 0,
    `currentwaypoint` = 0,
    `MovementType` = 2
WHERE `guid` = @RING_TOSS_EXISTING_CREATURE_GUID
  AND `id` = 54490
  AND `map` = 974;

DELETE FROM `creature_addon`
WHERE `guid` IN (@RING_TOSS_ORIGINAL_CREATURE_GUID, @RING_TOSS_CREATURE_GUID_START, @RING_TOSS_CREATURE_GUID_START + 1, @RING_TOSS_EXISTING_CREATURE_GUID);

INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
SELECT @RING_TOSS_ORIGINAL_CREATURE_GUID, @RING_TOSS_ORIGINAL_PATH_ID, 0, 0, 0, 0, '101696'
FROM `creature`
WHERE `guid` = @RING_TOSS_ORIGINAL_CREATURE_GUID
  AND `id` = 54490
  AND `map` = 974
UNION ALL
SELECT @RING_TOSS_CREATURE_GUID_START, @RING_TOSS_CREATURE_GUID_START, 0, 0, 0, 0, '101696'
FROM `creature`
WHERE `guid` = @RING_TOSS_CREATURE_GUID_START
  AND `id` = 54490
  AND `map` = 974
UNION ALL
SELECT @RING_TOSS_CREATURE_GUID_START + 1, @RING_TOSS_CREATURE_GUID_START + 1, 0, 0, 0, 0, '101696'
FROM `creature`
WHERE `guid` = @RING_TOSS_CREATURE_GUID_START + 1
  AND `id` = 54490
  AND `map` = 974
UNION ALL
SELECT @RING_TOSS_EXISTING_CREATURE_GUID, @RING_TOSS_EXISTING_PATH_ID, 0, 0, 0, 0, '101696'
FROM `creature`
WHERE `guid` = @RING_TOSS_EXISTING_CREATURE_GUID
  AND `id` = 54490
  AND `map` = 974;

DELETE FROM `waypoint_data`
WHERE `id` IN (@RING_TOSS_ORIGINAL_PATH_ID, @RING_TOSS_CREATURE_GUID_START, @RING_TOSS_CREATURE_GUID_START + 1, @RING_TOSS_EXISTING_PATH_ID);

INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(@RING_TOSS_ORIGINAL_PATH_ID, 1, -4302.40, 6299.20, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_ORIGINAL_PATH_ID, 2, -4305.80, 6294.00, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_ORIGINAL_PATH_ID, 3, -4303.00, 6286.20, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_ORIGINAL_PATH_ID, 4, -4295.00, 6284.00, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_ORIGINAL_PATH_ID, 5, -4290.50, 6292.00, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_ORIGINAL_PATH_ID, 6, -4293.60, 6301.20, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START, 1, -4290.30, 6301.60, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START, 2, -4286.80, 6295.50, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START, 3, -4288.60, 6287.20, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START, 4, -4294.50, 6282.80, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START, 5, -4300.40, 6287.50, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START, 6, -4298.20, 6297.60, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START + 1, 1, -4299.40, 6289.40, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START + 1, 2, -4306.40, 6291.80, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START + 1, 3, -4305.70, 6298.60, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START + 1, 4, -4300.80, 6303.40, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START + 1, 5, -4293.30, 6303.10, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START + 1, 6, -4289.00, 6296.00, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_CREATURE_GUID_START + 1, 7, -4292.80, 6287.00, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_EXISTING_PATH_ID, 1, -4294.20, 6302.80, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_EXISTING_PATH_ID, 2, -4289.60, 6298.20, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_EXISTING_PATH_ID, 3, -4287.80, 6291.00, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_EXISTING_PATH_ID, 4, -4292.40, 6284.80, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_EXISTING_PATH_ID, 5, -4299.90, 6284.80, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_EXISTING_PATH_ID, 6, -4304.20, 6291.80, 13.118, 0, 0, 0, 0, 100, 0),
(@RING_TOSS_EXISTING_PATH_ID, 7, -4301.80, 6300.50, 13.118, 0, 0, 0, 0, 100, 0);

INSERT IGNORE INTO `game_event_creature` (`eventEntry`, `guid`)
SELECT @DARKMOON_EVENT_ENTRY, `guid`
FROM `creature`
WHERE `guid` IN (@RING_TOSS_ORIGINAL_CREATURE_GUID, @RING_TOSS_CREATURE_GUID_START, @RING_TOSS_CREATURE_GUID_START + 1, @RING_TOSS_EXISTING_CREATURE_GUID)
  AND `id` = 54490
  AND `map` = 974;

COMMIT;
