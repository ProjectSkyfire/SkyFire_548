-- Vashj'ir, "Swift Action" (25963): the Tamed Seahorse was summoned and boarded by Fiasco Sizzlegrin instead of the player. The
-- summoner of a creature that Smart AI summons is the creature that owns the script, so "On Just Summoned" on the seahorse
-- handed Fiasco to the invoker action and Fiasco took the only seat. Fiasco now stores the player who picked the option and,
-- once the seahorse exists ("On Summoned Unit", the unit is the seahorse), the player casts Ride Vehicle Hardcoded (46598) on it.

UPDATE `creature_template` SET `AIName` = '' WHERE `entry` = 41868;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (40918, 41868);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40918, 0, 0, 1, 62, 0, 100, 0, 11534, 0, 0, 0, 0, 33, 40918, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Quest Credit Fiasco Sizzlegrin'),
(40918, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Close Gossip'),
(40918, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Store the Player'),
(40918, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41868, 1, 900000, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Summon Tamed Seahorse at the Player'),
(40918, 0, 4, 0, 17, 0, 100, 0, 41868, 0, 0, 0, 0, 86, 46598, 2, 12, 1, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Summoned Unit (Tamed Seahorse) - Stored Player Rides the Seahorse');
