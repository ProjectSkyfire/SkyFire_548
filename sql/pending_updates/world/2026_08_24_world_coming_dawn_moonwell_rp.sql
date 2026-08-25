-- Quest 933 Teldrassil: The Coming Dawn
-- Shade of the Kaldorei RP at Pools of Arlithrien never played on approach.
-- SMART_EVENT_OOC_LOS InitTimer applies event cooldown as the initial delay;
-- 119000 ms meant players filled the phial and left before bunny guid 140059
-- could cast 65679. Match the Shadowglen moonwell (quest 28729) cooldown.

DELETE FROM `smart_scripts` WHERE `entryorguid`=-140059 AND `source_type`=0 AND `id`=2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-140059, 0, 2, 0, 10, 0, 100, 0, 1, 10, 1000, 1000, 0, 11, 65679, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Moonwell Bunny - On OOC Los - Cast Forcecast Summon Shade of the Kaldorei');
