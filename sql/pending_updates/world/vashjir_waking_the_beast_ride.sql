-- Vashj'ir, "Waking the Beast" (25996): speak with Earthmender Duarn inside Nespirah (credit 41531), then escape on Erunak's
-- Swiftfin Seahorse (credit 41776). Neither step had a script. The option "Whenever you're ready, Duarn." of menu 11525 gives
-- the first credit. The seahorse (41776) has the click flag; in the client data "Summon Escape Seahorse (Master)" (77927) credits
-- 41776 to the player and summons the riding seahorse (41778 for the Horde, vehicle 840 in the 5.4.8 data, none here). Clicking
-- the seahorse now casts 77927 on the player, summons 41778 and puts the player on it. The lightning storm and the hook of the
-- escape (77916 to 77919) are not scripted.

UPDATE `creature_template` SET `VehicleId` = 840 WHERE `entry` IN (41778, 41785);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (41531, 41776, 41778);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (41531, 41776, 41778);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41531, 0, 0, 1, 62, 0, 100, 0, 11525, 0, 0, 0, 0, 33, 41531, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Quest Credit Earthmender Duarn'),
(41531, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Close Gossip'),
(41776, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 85, 77927, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Spellclick - Invoker Casts Summon Escape Seahorse (Master)'),
(41776, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 41778, 1, 900000, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Summon Swiftfin Seahorse at the Player'),
(41778, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 85, 46598, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Just Summoned - Summoner Rides This Vehicle');
