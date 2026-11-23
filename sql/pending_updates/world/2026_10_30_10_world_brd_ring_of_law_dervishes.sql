-- Blackrock Depths: Gorosh the Dervish, Grizzle and Anub'shiah (Ring of Law) use the 4.3.4 SmartAI instead of
-- SkyFire's old C++ scripts, which had fewer and older spells.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` IN (9027, 9028, 9031);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (9027, 9028, 9031);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9027, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 100, 0, 0, 0, 0, 0, 0, 'Gorosh the Dervish - Just summoned - Start attack'),
(9027, 0, 1, 0, 0, 0, 100, 0, 2000, 4000, 6000, 8000, 0, 11, 13736, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Gorosh the Dervish - In Combat - Cast Whirlwind'),
(9027, 0, 2, 0, 0, 0, 100, 0, 4000, 6000, 5000, 6000, 0, 11, 15708, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Gorosh the Dervish - In Combat - Cast Mortal Strike'),
(9027, 0, 3, 0, 0, 0, 100, 1, 10000, 15000, 10000, 15000, 0, 11, 21049, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gorosh the Dervish - In Combat - Cast Bloodlust'),
(9028, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 100, 0, 0, 0, 0, 0, 0, 'Grizzle - Just summoned - Start attack'),
(9028, 0, 1, 0, 0, 0, 100, 0, 2000, 3000, 4000, 5000, 0, 11, 40504, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grizzle the Dervish - In Combat - Cast Cleave'),
(9028, 0, 2, 0, 0, 0, 100, 0, 4000, 5000, 8000, 9000, 0, 11, 6524, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grizzle the Dervish - In Combat - Cast Ground Tremor'),
(9028, 0, 3, 0, 0, 0, 100, 1, 10000, 15000, 10000, 15000, 0, 11, 8269, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grizzle the Dervish - In Combat - Cast Frenzy'),
(9028, 0, 4, 0, 0, 0, 100, 0, 6000, 7000, 6000, 9000, 0, 11, 24458, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Grizzle the Dervish - In Combat - Cast Shadow Shock'),
(9031, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 100, 0, 0, 0, 0, 0, 0, 'Anub''shiah - Just summoned - Start attack'),
(9031, 0, 1, 0, 0, 0, 100, 0, 5000, 7000, 10000, 11000, 0, 11, 8994, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anubshiah - In Combat - Cast Banish'),
(9031, 0, 2, 0, 0, 0, 100, 0, 2000, 3000, 8000, 9000, 0, 11, 15470, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anubshiah - In Combat - Cast Curse of Tongues'),
(9031, 0, 3, 0, 0, 0, 100, 0, 4000, 5000, 13000, 15000, 0, 11, 12493, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anubshiah - In Combat - Cast Curse of Weakness'),
(9031, 0, 4, 0, 0, 0, 100, 0, 1000, 1000, 300000, 300000, 0, 11, 13787, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anubshiah - In Combat - Cast Demon Armor'),
(9031, 0, 5, 0, 0, 0, 100, 0, 7000, 8000, 10000, 13000, 0, 11, 15471, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anubshiah - In Combat - Cast Enveloping Web'),
(9031, 0, 6, 0, 0, 0, 100, 0, 3000, 4000, 4000, 5000, 0, 11, 15472, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Anubshiah - In Combat - Cast Shadow Bolt');
