-- Ulduar, Thorim: pulling the lever opens the Dark Iron Portcullis (guid 34155), as in TrinityCore 3.3.5. The instance
-- script only closes the lever and the portcullis again after a wipe; nothing opened the portcullis.
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 194264;
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 194264;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(194264, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 14, 34155, 0, 0, 0, 0, 0, 0, 'Lever - On Gameobject State Changed - Activate Gameobject');
