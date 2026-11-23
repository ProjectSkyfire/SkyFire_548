-- Blasted Lands: "Loramus Thalipedes Awaits" (25700) and "The Final Ritual" (26170) ask the player to take part in
-- the amulet ritual, but they had no objective, so they were complete as soon as they were taken. The ritual option
-- of Bloodmage Lynnore and Kasim Sharim only shows while the quest is incomplete, so it never showed. The quests now
-- wait for the ritual, which credits the player who picks the option.
UPDATE `quest_template` SET `SpecialFlags` = `SpecialFlags` | 2 WHERE `Id` IN (25700, 26170);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (7506, 42298) AND `source_type` = 0 AND `id` IN (1, 4);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7506, 0, 1, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodmage Lynnore - On Gossip Option Select - Close Gossip'),
(7506, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 15, 25700, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodmage Lynnore - On Gossip Option Select - Quest Credit Loramus Thalipedes Awaits'),
(42298, 0, 1, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kasim Sharim - On Gossip Option Select - Close Gossip'),
(42298, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 15, 26170, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kasim Sharim - On Gossip Option Select - Quest Credit The Final Ritual');
