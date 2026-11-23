-- Latent Disease (26934): the objective is 20 Plague Tangles (item 60679) that grow where the Diseased Black Bears
-- die. Nothing summoned the tangles (object 205060, loot 31797 already exists), so the quest could not be finished.
-- A dying bear casts Latent Disease: Summon Plague Tangle (83105, castable while dead) five times, as in the source,
-- and the existing Infected Wound row is kept.
-- Rows are as in Legends of Azeroth 5.4.8.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 1815 AND `source_type` = 0 AND `id` BETWEEN 1 AND 5;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1815, 0, 1, 2, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 83105, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Diseased Black Bear - On Death - Cast Summon Plague Tangle'),
(1815, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 83105, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Diseased Black Bear - Linked To Id 1 - Cast Summon Plague Tangle'),
(1815, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 83105, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Diseased Black Bear - Linked To Id 2 - Cast Summon Plague Tangle'),
(1815, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 83105, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Diseased Black Bear - Linked To Id 3 - Cast Summon Plague Tangle'),
(1815, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 83105, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Diseased Black Bear - Linked To Id 4 - Cast Summon Plague Tangle');
