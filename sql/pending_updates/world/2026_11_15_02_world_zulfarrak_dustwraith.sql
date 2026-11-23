-- Zul'Farrak: spawn the rare Dustwraith. Its template was level 46, friendly (faction 35), not attackable and flagged
-- as dead; it is now level 48, hostile and attackable, with its Sinister Strike and Virulent Poison SmartAI rows. It
-- sits alone in a pool and appears in 20 of 100 runs.
UPDATE `creature_template` SET `minlevel` = 48, `maxlevel` = 48, `faction_A` = 14, `faction_H` = 14, `unit_flags` = 0, `dynamicflags` = 0, `spell1` = 15667, `AIName` = 'SmartAI' WHERE `entry` = 10081 AND `ScriptName` = '';

DELETE FROM `creature` WHERE `guid` = 9257504;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9257504, 10081, 209, 2, 0, 0, 0, 0, 1627.47, 1186.85, 8.87693, 0.647954, 86400, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `pool_creature` WHERE `pool_entry` = 9257502;
DELETE FROM `pool_template` WHERE `entry` = 9257502;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(9257502, 1, 'Zul''Farrak: Dustwraith');
INSERT INTO `pool_creature` (`guid`, `pool_entry`, `chance`, `description`) VALUES
(9257504, 9257502, 20, '10081 Dustwraith');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 10081;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10081, 0, 0, 0, 9, 0, 100, 0, 0, 5, 3600, 7300, 0, 11, 15667, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dustwraith - Within 0-5 Range - Cast Sinister Strike'),
(10081, 0, 1, 0, 0, 0, 100, 0, 5000, 5000, 17000, 18000, 0, 11, 12251, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dustwraith - In Combat - Cast Virulent Poison');
