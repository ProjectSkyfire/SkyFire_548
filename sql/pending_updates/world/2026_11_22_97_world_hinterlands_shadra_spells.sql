-- Shadra (43007), the boss of "Shadra the Venom Queen" (26419 and 26532), has AIName SmartAI but no rows, so she only
-- melees. Add Poison, Slowing Poison and Web, the spells of her script in the Cataclysm Preservation Project 4.3.4.
-- Ported from the Cataclysm Preservation Project 4.3.4.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 43007 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43007, 0, 0, 0, 0, 0, 100, 0, 2000, 4500, 15000, 20000, 0, 11, 13298, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadra - In Combat - Cast Poison'),
(43007, 0, 1, 0, 0, 0, 100, 0, 6000, 9000, 22000, 26000, 0, 11, 7992, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadra - In Combat - Cast Slowing Poison'),
(43007, 0, 2, 0, 9, 0, 100, 0, 0, 10, 9000, 10000, 0, 11, 28991, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadra - Within 0-10 Range - Cast Web');
