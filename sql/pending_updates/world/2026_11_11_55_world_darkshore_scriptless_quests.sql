-- In Defense of Darkshore (13885): its credits (34323, 34324, 34325) had no source. The animals that Darkshore
-- Protection (spell 65188) already protects for An Ounce of Prevention now also credit these three. The Lifebringer
-- Sapling is given on accept so the protection can be cast from the ground; the hippogryph flight is not implemented.
UPDATE `quest_template` SET `SourceItemId` = 46363 WHERE `Id` = 13885;

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (207100, 216500, 223700, 3431800) AND `id` = 5;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(207100, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34325, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Moonstalker Matriarch - On Script - Quest Credit Moonstalker Sire Quest Credit'),
(216500, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34323, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Grizzled Thistle Bear - On Script - Quest Credit Grizzled Thistle Bear Quest Credit'),
(223700, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34325, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Moonstalker Sire - On Script - Quest Credit Moonstalker Sire Quest Credit'),
(3431800, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34324, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Whitetail Stag - On Script - Quest Credit Whitetail Stag Quest Credit');
