-- Darkmoon Faire Tonk Challenge script binding and duplicate booth cleanup.

START TRANSACTION;

-- Remove low-GUID static copies that overlap the event-managed Darkmoon booth spawns.
DELETE `game_event_creature`
FROM `game_event_creature`
JOIN `creature` ON `creature`.`guid` = `game_event_creature`.`guid`
WHERE `creature`.`guid` IN (733, 734, 735, 737)
  AND `creature`.`id` IN (67382, 67383, 67384, 68561, 74019, 73422, 74010, 74012, 68993);

DELETE `creature_addon`
FROM `creature_addon`
JOIN `creature` ON `creature`.`guid` = `creature_addon`.`guid`
WHERE `creature`.`guid` IN (733, 734, 735, 737)
  AND `creature`.`id` IN (67382, 67383, 67384, 68561, 74019, 73422, 74010, 74012, 68993);

DELETE FROM `creature`
WHERE `guid` IN (733, 734, 735, 737)
  AND `id` IN (67382, 67383, 67384, 68561, 74019, 73422, 74010, 74012, 68993);

-- Remove duplicate Darkmoon Faire gameobject spawns that predate the reserved Darkmoon GUID range.
DELETE FROM `game_event_gameobject`
WHERE `guid` IN (71957, 71958, 71964, 71968, 71972, 71973, 71974);

DELETE FROM `gameobject`
WHERE `guid` IN (71957, 71958, 71964, 71968, 71972, 71973, 71974);

-- C++ now owns the Tonk Challenge start flow, target waves, hit credit, and cleanup.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 22
  AND ((`SourceId` = 0 AND `SourceEntry` IN (33081, 54605))
    OR (`SourceId` = 9 AND `SourceEntry` = 5460500));

DELETE FROM `smart_scripts`
WHERE (`source_type` = 0 AND `entryorguid` IN (33081, 54605))
   OR (`source_type` = 9 AND `entryorguid` = 5460500);

UPDATE `creature_template`
SET `AIName` = '',
    `ScriptName` = 'npc_darkmoon_finlay_coolshot'
WHERE `entry` = 54605;

UPDATE `creature_template`
SET `AIName` = '',
    `ScriptName` = 'npc_darkmoon_tonk_cannon_target'
WHERE `entry` = 33081;

UPDATE `creature_template`
SET `unit_flags` = 16809984,
    `unit_flags2` = 2048,
    `VehicleId` = 1734,
    `spell1` = 102292,
    `spell2` = 102297,
    `AIName` = '',
    `ScriptName` = 'npc_darkmoon_steam_tonk'
WHERE `entry` = 54588;

DELETE FROM `spell_script_names`
WHERE `spell_id` = 100752
   OR `ScriptName` = 'spell_darkmoon_tonk_controller';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(100752, 'spell_darkmoon_tonk_controller');

COMMIT;
