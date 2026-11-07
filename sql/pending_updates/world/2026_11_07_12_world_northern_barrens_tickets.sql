-- Northern Barrens, in-game reports 76, 80 and 81. Reference data: 5.4.8 for the scripts, 4.3.4 for the gossip
-- link. Safe to run twice: every block deletes its own keys first or sets absolute values.

-- Waptor Twapping (14068): the trap summons Waptor Twap (34831), which had no script. In the 5.4.8 data a Sunscale
-- raptor that sees the trap runs to it and sets its data when it arrives, the raptor dies, and the trap fills itself
-- (spell 66286 spawns the Woaded Waptor Twap chest with Wittle Waptor). Added next to the combat rows the raptors
-- already have (the 5.4.8 rows replace them, so only the trap rows are taken). Sunscale Consort (44165) had no AI.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (34831, 44165);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (3255, 3256, 44164, 44165) AND `id` BETWEEN 10 AND 13;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 34831 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34831, 0, 0, 1, 38, 0, 100, 1, 1, 1, 0, 0, 0, 11, 66286, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waptor Twap - On Data Set 1 1 - Cast Fill the Waptor Twap'),
(34831, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waptor Twap - Linked To Id 0 - Despawn'),
(3255, 0, 10, 0, 10, 0, 100, 0, 1, 30, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 11, 34831, 30, 0, 0, 0, 0, 0, 'Sunscale Screecher - Out Of Combat Los - Move To Waptor Twap'),
(3255, 0, 11, 12, 34, 0, 100, 0, 0, 1, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 34831, 2, 0, 0, 0, 0, 0, 'Sunscale Screecher - On Reached Point 1 - Set Data 1 1 On Waptor Twap'),
(3255, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Screecher - Linked To Id 11 - Die'),
(3255, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Screecher - Linked To Id 12 - Despawn'),
(3256, 0, 10, 0, 10, 0, 100, 0, 1, 30, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 11, 34831, 30, 0, 0, 0, 0, 0, 'Sunscale Scytheclaw - Out Of Combat Los - Move To Waptor Twap'),
(3256, 0, 11, 12, 34, 0, 100, 0, 0, 1, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 34831, 2, 0, 0, 0, 0, 0, 'Sunscale Scytheclaw - On Reached Point 1 - Set Data 1 1 On Waptor Twap'),
(3256, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Scytheclaw - Linked To Id 11 - Die'),
(3256, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Scytheclaw - Linked To Id 12 - Despawn'),
(44164, 0, 10, 0, 10, 0, 100, 0, 1, 30, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 11, 34831, 30, 0, 0, 0, 0, 0, 'Sunscale Ravager - Out Of Combat Los - Move To Waptor Twap'),
(44164, 0, 11, 12, 34, 0, 100, 0, 0, 1, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 34831, 2, 0, 0, 0, 0, 0, 'Sunscale Ravager - On Reached Point 1 - Set Data 1 1 On Waptor Twap'),
(44164, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Ravager - Linked To Id 11 - Die'),
(44164, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Ravager - Linked To Id 12 - Despawn'),
(44165, 0, 10, 0, 10, 0, 100, 0, 1, 30, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 11, 34831, 30, 0, 0, 0, 0, 0, 'Sunscale Consort - Out Of Combat Los - Move To Waptor Twap'),
(44165, 0, 11, 12, 34, 0, 100, 0, 0, 1, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 34831, 2, 0, 0, 0, 0, 0, 'Sunscale Consort - On Reached Point 1 - Set Data 1 1 On Waptor Twap'),
(44165, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Consort - Linked To Id 11 - Die'),
(44165, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Consort - Linked To Id 12 - Despawn');

-- The Baron's Demands (14046): Baron Longshore had the script and the gossip flag but no gossip menu, so the
-- "Break Longshore's manacles" option (menu 10567, option 1) was never shown and the credit never given. Both
-- 4.3.4 and 5.4.8 link him to menu 10567.
UPDATE `creature_template` SET `gossip_menu_id` = 10567 WHERE `entry` = 3467;

-- Second Baron Longshore on the mountain above Ratchet. The 4.3.4 data has a copy there; the 5.4.8 data has only
-- the one in the Alliance camp, where the quests are given and finished, so the mountain copy is removed.
DELETE FROM `creature` WHERE `guid` = 149394 AND `id` = 3467;
