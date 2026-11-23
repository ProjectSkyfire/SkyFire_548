-- Vashj'ir, "Ain't Too Proud to Beg" (25598): find Mack and Samir. The objective is the creature 39669 (Captain Samir) and
-- nothing credited it: no spell does (the client data has none) and he had no script. His gossip menu 11444 has the
-- option "Make for that cave to the west. It's safe and dry."; picking it gives the credit.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 39669;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 39669;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(39669, 0, 0, 1, 62, 0, 100, 0, 11444, 0, 0, 0, 0, 33, 39669, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Quest Credit Captain Samir'),
(39669, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Close Gossip');
