-- Zul'Farrak: spawn the rare Sandarr Dunereaver and Zerillis. Each sits alone in a pool and appears in 12 of 100 runs.
-- Both templates had spells listed but no AI, so they get their SmartAI rows.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (10080, 10082) AND `ScriptName` = '';

DELETE FROM `creature` WHERE `guid` IN (9257502, 9257503);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9257502, 10080, 209, 2, 0, 0, 0, 0, 1546.25, 1017.14, 8.87683, 0.0109968, 28800, 0, 0, 0, 0, 0, 0, 0, 0),
(9257503, 10082, 209, 2, 0, 0, 0, 0, 1628.94, 952.042, 8.87684, 5.43152, 28800, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `pool_creature` WHERE `pool_entry` IN (9257500, 9257501);
DELETE FROM `pool_template` WHERE `entry` IN (9257500, 9257501);
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(9257500, 1, 'Zul''Farrak: Sandarr Dunereaver'),
(9257501, 1, 'Zul''Farrak: Zerillis');
INSERT INTO `pool_creature` (`guid`, `pool_entry`, `chance`, `description`) VALUES
(9257502, 9257500, 12, '10080 Sandarr Dunereaver'),
(9257503, 9257501, 12, '10082 Zerillis');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (10080, 10082);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10080, 0, 0, 0, 9, 0, 100, 0, 0, 5, 16000, 26000, 0, 11, 14516, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sandarr Dunereaver - Within 0-5 Range - Cast Strike'),
(10080, 0, 1, 0, 0, 0, 100, 0, 6000, 12000, 25000, 34000, 0, 11, 13730, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sandarr Dunereaver - In Combat - Cast Demoralizing Shout'),
(10080, 0, 2, 0, 0, 0, 100, 0, 10000, 20000, 15000, 24000, 0, 11, 15615, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sandarr Dunereaver - In Combat - Cast Pummel'),
(10082, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 15547, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Zerillis - In Combat - Cast Shoot'),
(10082, 0, 1, 0, 9, 0, 100, 0, 0, 20, 12000, 25000, 0, 11, 6533, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Zerillis - Within 0-20 Range - Cast Net'),
(10082, 0, 2, 0, 0, 0, 100, 0, 6000, 11000, 5800, 16000, 0, 11, 12551, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Zerillis - In Combat - Cast Frost Shot');
