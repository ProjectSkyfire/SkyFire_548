-- The Unforgiven channels Unrelenting Anguish (122832) on his target in the 5.4.8 dungeon journal: a five second
-- channeled Shadow damage over time that slows the target by 15 percent. His SmartAI only had Frost Nova and Stealth,
-- as in the older data. The journal gives no timing, so the first cast comes after 8 to 10 seconds and then every
-- 16 to 20 seconds, which leaves room for Frost Nova (every 13 to 17 seconds). Stratholme has no heroic mode, so the
-- row takes no difficulty flag.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 10516 AND `source_type` = 0 AND `id` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10516, 0, 2, 0, 0, 0, 100, 0, 8000, 10000, 16000, 20000, 0, 11, 122832, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'The Unforgiven - In Combat - Cast Unrelenting Anguish');
