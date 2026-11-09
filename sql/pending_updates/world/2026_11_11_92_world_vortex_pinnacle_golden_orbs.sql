-- The Vortex Pinnacle (map 657) heroic: the nine Golden Orbs. Using one counts it for the instance, and the fifth makes
-- the Achievement Check spell (94756) run for everyone inside. The orbs the instance import placed (8 spawns, both
-- difficulties, other positions) are replaced by these nine heroic spawns.
-- Safe to run twice (every block deletes its own rows first).

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 51157;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 51157;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(51157, 0, 0, 1, 8, 0, 100, 1, 68742, 0, 0, 0, 0, 34, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Golden Orb - On spell hit Interact Dummy - Set instance data 4'),
(51157, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 94316, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Golden Orb - On spell hit (Link) - Remove Golden Orb Visual'),
(51157, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1400, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Golden Orb - On spell hit (Link) - Despawn');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 51157;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(51157, 68742, 1, 0);

DELETE FROM `creature_addon` WHERE `guid` IN (9220016, 9220017, 9220018, 9220019, 9220020, 9220021, 9220022, 9220126) OR `guid` BETWEEN 9220200 AND 9220208;
DELETE FROM `creature` WHERE (`guid` IN (9220016, 9220017, 9220018, 9220019, 9220020, 9220021, 9220022, 9220126) AND `id` = 51157) OR `guid` BETWEEN 9220200 AND 9220208;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9220200, 51157, 657, 4, 0, 0, 0, 0, -430.007, -111.033, 626.035, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0),
(9220201, 51157, 657, 4, 0, 0, 0, 0, -754.844, -61.9653, 635.946, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0),
(9220202, 51157, 657, 4, 0, 0, 0, 0, -1032.68, -38.3854, 705.917, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0),
(9220203, 51157, 657, 4, 0, 0, 0, 0, -1088.38, -56.2604, 705.906, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0),
(9220204, 51157, 657, 4, 0, 0, 0, 0, -1190.59, 73.6944, 754.223, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0),
(9220205, 51157, 657, 4, 0, 0, 0, 0, -1226.01, 88.9306, 754.223, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0),
(9220206, 51157, 657, 4, 0, 0, 0, 0, -1046.92, 419.465, 698.126, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0),
(9220207, 51157, 657, 4, 0, 0, 0, 0, -1004.28, 430.224, 657.974, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0),
(9220208, 51157, 657, 4, 0, 0, 0, 0, -763.62, 411.309, 682.691, 0, 604800, 0, 0, 0, 0, 0, 0, 0, 0);

INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(9220200, 0, 0, 0, 1, 0, '94316'),
(9220201, 0, 0, 0, 1, 0, '94316'),
(9220202, 0, 0, 0, 1, 0, '94316'),
(9220203, 0, 0, 0, 1, 0, '94316'),
(9220204, 0, 0, 0, 1, 0, '94316'),
(9220205, 0, 0, 0, 1, 0, '94316'),
(9220206, 0, 0, 0, 1, 0, '94316'),
(9220207, 0, 0, 0, 1, 0, '94316'),
(9220208, 0, 0, 0, 1, 0, '94316');
