-- Darkmoon Faire cannon game scripts and scorer.

SET @DARKMOON_EVENT_ENTRY := 75;
SET @DARKMOON_CANNON_TARGET_GUID := 8200300;

START TRANSACTION;

DELETE FROM `creature_addon` WHERE `guid` = @DARKMOON_CANNON_TARGET_GUID;
DELETE FROM `game_event_creature` WHERE `guid` = @DARKMOON_CANNON_TARGET_GUID;
DELETE FROM `creature` WHERE `guid` = @DARKMOON_CANNON_TARGET_GUID;

INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@DARKMOON_CANNON_TARGET_GUID, 54224, 974, 1, 0, 0, 0, 0, -4440.18, 6211.33, 0.224782, 0, 300, 0, 0, 1, 0, 0, 0, 0, 0);

INSERT IGNORE INTO `game_event_creature` (`eventEntry`, `guid`) VALUES
(@DARKMOON_EVENT_ENTRY, @DARKMOON_CANNON_TARGET_GUID);

DELETE FROM `smart_scripts`
WHERE `source_type` = 0
  AND `entryorguid` IN (15303, 57850);

UPDATE `creature_template`
SET `AIName` = '',
    `ScriptName` = 'npc_darkmoon_maxima_blastenheimer',
    `gossip_menu_id` = 6575,
    `npcflag` = `npcflag` | 3
WHERE `entry` = 15303;

UPDATE `creature_template`
SET `AIName` = '',
    `ScriptName` = 'npc_darkmoon_cannon_target'
WHERE `entry` = 54224;

UPDATE `creature_template`
SET `AIName` = '',
    `ScriptName` = 'npc_darkmoon_fozlebub',
    `gossip_menu_id` = 13352,
    `npcflag` = `npcflag` | 1
WHERE `entry` = 57850;

DELETE FROM `spell_script_names`
WHERE `ScriptName` IN ('spell_darkmoon_cannon_preparation')
   OR (`spell_id` = 102112 AND `ScriptName` <> 'spell_darkmoon_cannon_preparation');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102112, 'spell_darkmoon_cannon_preparation');

COMMIT;
