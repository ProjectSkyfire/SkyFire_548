-- Vashj'ir, "Waking the Beast" (25996): the riding Swiftfin Seahorse was summoned and boarded by the clicked Swiftfin Seahorse
-- instead of the player, for the same reason as the Tamed Seahorse of "Swift Action": the summoner of a Smart AI summon is the
-- creature that owns the script. The clicked seahorse stores the player and, once the riding seahorse exists ("On Summoned
-- Unit", the unit is the riding seahorse), the player casts Ride Vehicle Hardcoded (46598) on it.

UPDATE `creature_template` SET `AIName` = '' WHERE `entry` = 41778;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (41776, 41778);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41776, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 85, 77927, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Spellclick - Invoker Casts Summon Escape Seahorse (Master)'),
(41776, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Store the Player'),
(41776, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41778, 1, 900000, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Summon Swiftfin Seahorse at the Player'),
(41776, 0, 3, 0, 17, 0, 100, 0, 41778, 0, 0, 0, 0, 86, 46598, 2, 12, 1, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Summoned Unit (Swiftfin Seahorse) - Stored Player Rides the Seahorse');
