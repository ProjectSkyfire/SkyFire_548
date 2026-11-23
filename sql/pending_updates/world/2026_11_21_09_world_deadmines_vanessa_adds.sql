-- The three heroic adds of Vanessa VanCleef (Defias Enforcer, Shadowguard and Blood Wizard) had no AI. The rows take
-- no difficulty flag: they only appear on heroic. The Enforcer's opening charge is the 5.4.8 Charge, since the spell
-- the 4.3.4 row names is not in the client.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (49850, 49852, 49854) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (49850, 49852, 49854) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49850, 0, 0, 0, 0, 0, 100, 1, 1000, 1000, 0, 0, 0, 11, 90928, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Defias Enforcer - In Combat - Cast Charge'),
(49850, 0, 1, 0, 0, 0, 100, 0, 6500, 7000, 30000, 30000, 0, 11, 90925, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Defias Enforcer - In Combat - Cast Bloodbath'),
(49850, 0, 2, 0, 2, 0, 100, 0, 0, 20, 29000, 30000, 0, 11, 90929, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Defias Enforcer - Between 0-20% Health - Cast Recklessness'),
(49852, 0, 0, 0, 0, 0, 100, 1, 1000, 1000, 0, 0, 0, 11, 90954, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Defias Shadowguard - In Combat - Cast Camouflage'),
(49852, 0, 1, 0, 0, 0, 100, 0, 5500, 6000, 10000, 11000, 0, 11, 90951, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Defias Shadowguard - In Combat - Cast Sinister Strike'),
(49852, 0, 2, 0, 0, 0, 100, 0, 10000, 10000, 20000, 20000, 0, 11, 90956, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Defias Shadowguard - In Combat - Cast Shadowstep'),
(49852, 0, 3, 0, 0, 0, 100, 0, 13000, 13000, 16000, 16000, 0, 11, 90960, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Defias Shadowguard - In Combat - Cast Whirling Blades'),
(49852, 0, 4, 0, 2, 0, 100, 0, 0, 20, 29000, 30000, 0, 11, 90958, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Defias Shadowguard - Between 0-20% Health - Cast Evasion'),
(49854, 0, 0, 0, 0, 0, 100, 0, 0, 0, 3000, 3000, 0, 11, 90938, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Defias Blood Wizard - In Combat - Cast Bloodbolt'),
(49854, 0, 1, 0, 0, 0, 100, 0, 7000, 7000, 60000, 60000, 0, 11, 90932, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Defias Blood Wizard - In Combat - Cast Ragezone');
