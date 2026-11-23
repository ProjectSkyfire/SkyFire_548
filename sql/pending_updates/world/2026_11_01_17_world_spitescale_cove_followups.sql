-- Spitescale Cove, Echo Isles: the Darkspear Tribesmen had no SmartAI, so they stood idle next to the naga while the
-- shamans fought; they now attack the nearest Wavethrasher. The flag bunnies show the Territorial Fetish for two
-- minutes after it is placed.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (38324, 38560);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (38324, 38560);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38324, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 5000, 5000, 0, 49, 0, 0, 0, 0, 0, 0, 11, 38300, 30, 0, 0, 0, 0, 0, 'Darkspear Tribesman - OOC - Start Attack Creature ID: 38300, 30 Yards'),
(38560, 0, 0, 1, 8, 0, 100, 0, 72070, 0, 120000, 120000, 0, 67, 38560, 120000, 120000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spitescale Flag Bunny - On SpellHit Place Territorial Fetish - Create Timed Event'),
(38560, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 72072, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spitescale Flag Bunny - Linked To Id 0 - Cast Territorial Fetish'),
(38560, 0, 2, 0, 59, 0, 100, 0, 38560, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spitescale Flag Bunny - On Timed Event Triggered - Remove Aura Territorial Fetish');
