-- Balnazzar stands in the Crusaders' Square fight on his own, without Grand Crusader Dathrohan. The Dathrohan
-- script is left registered but unused. The old spawn row keeps its guid, addon and respawn time.
UPDATE `creature` SET `id` = 10813 WHERE `guid` = 52149 AND `id` IN (10812, 10813);

-- Balnazzar's existing SmartAI rows were limited to one difficulty, which never matches this dungeon, so the
-- spells never fired. Stratholme has a single difficulty, so the rows take no difficulty flag.
UPDATE `smart_scripts` SET `event_flags` = 0 WHERE `entryorguid` = 10813 AND `source_type` = 0 AND `id` BETWEEN 0 AND 4;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 10813 AND `source_type` = 0 AND `id` IN (5, 6);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10813, 0, 5, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Balnazzar - On Aggro - Say 0'),
(10813, 0, 6, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Balnazzar - On Just Died - Say 1');
