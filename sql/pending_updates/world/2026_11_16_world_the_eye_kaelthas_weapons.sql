-- The Eye: SmartAI for Kael'thas' seven legendary weapons, which had no AI
-- Ported from TrinityCore 3.3.5. The corpse delay step of TrinityCore's script is left out, this core has no such action.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` BETWEEN 21268 AND 21274 AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` BETWEEN 21268 AND 21274;
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 2126800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(21268, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 80, 2126800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Netherstrand Longbow - On AI Initialize - Run Script'),
(21268, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3600, 3600, 0, 11, 36980, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Netherstrand Longbow - In Combat CMC - Cast ''Shoot'''),
(21268, 0, 2, 0, 0, 0, 100, 0, 5000, 15000, 15000, 30000, 0, 11, 36979, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Netherstrand Longbow - In Combat - Cast ''Multi-Shot'''),
(21268, 0, 3, 0, 0, 0, 100, 0, 30000, 50000, 30000, 50000, 0, 11, 36994, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Netherstrand Longbow - In Combat - Cast ''Blink'''),
(21269, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 80, 2126800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Devastation - On AI Initialize - Run Script'),
(21269, 0, 1, 0, 0, 0, 100, 0, 10000, 20000, 20000, 30000, 0, 11, 36981, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Devastation - In Combat - Cast ''Whirlwind'''),
(21270, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 80, 2126800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cosmic Infuser - On AI Initialize - Run Script'),
(21270, 0, 1, 0, 0, 0, 100, 0, 5000, 10000, 10000, 10000, 0, 11, 36983, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cosmic Infuser - In Combat - Cast ''Heal'''),
(21270, 0, 2, 0, 0, 0, 100, 0, 0, 5000, 5000, 15000, 0, 11, 36985, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cosmic Infuser - In Combat - Cast ''Holy Nova'''),
(21271, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 80, 2126800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infinity Blades - On AI Initialize - Run Script'),
(21271, 0, 1, 0, 0, 0, 100, 0, 5000, 15000, 10000, 15000, 0, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Infinity Blades - In Combat - Cast ''Thrash'''),
(21272, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 80, 2126800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warp Slicer - On AI Initialize - Run Script'),
(21272, 0, 1, 0, 0, 0, 100, 0, 6000, 6000, 6000, 6000, 0, 11, 36991, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Warp Slicer - In Combat - Cast ''Rend'''),
(21273, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 80, 2126800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Phaseshift Bulwark - On AI Initialize - Run Script'),
(21273, 0, 1, 0, 0, 0, 100, 0, 5000, 10000, 5000, 15000, 0, 11, 36988, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Phaseshift Bulwark - In Combat - Cast ''Shield Bash'''),
(21274, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 80, 2126800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Staff of Disintegration - On AI Initialize - Run Script'),
(21274, 0, 1, 0, 0, 0, 100, 0, 0, 0, 2600, 3600, 0, 11, 36990, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Staff of Disintegration - In Combat CMC - Cast ''Frostbolt'''),
(21274, 0, 2, 0, 0, 0, 100, 0, 5000, 15000, 15000, 25000, 0, 11, 36989, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Staff of Disintegration - In Combat - Cast ''Frost Nova'''),
(2126800, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kael''thas Legendary Weapon - On Script - Set Reactstate Passive'),
(2126800, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kael''thas Legendary Weapon - On Script - Set Reactstate Aggressive'),
(2126800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 38, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kael''thas Legendary Weapon - On Script - Set In Combat With Zone');
