-- The Ruby Sanctum: SmartAI for the Charscale trash, and spawns in every normal size
-- Ported from TrinityCore 3.3.5. TrinityCore's 10 and 25 player difficulty bits are moved to the 10 and 25 player normal bits. Scorch and Shockwave use the ids the 5.4.8 client data has (75412 and 75417).

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (40417,40419,40421,40423) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (40417,40419,40421,40423);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40417, 0, 0, 0, 0, 0, 100, 0, 6000, 6000, 8000, 8000, 0, 11, 75412, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Charscale Invoker - In Combat - Cast ''Scorch'''),
(40417, 0, 1, 0, 0, 0, 100, 0, 10000, 10000, 20000, 20000, 0, 11, 75413, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Charscale Invoker - In Combat - Cast ''Flame Wave'''),
(40419, 0, 0, 0, 0, 0, 100, 0, 6000, 6000, 7000, 7000, 0, 11, 15284, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Charscale Assaulter - In Combat - Cast ''Cleave'''),
(40419, 0, 1, 0, 0, 0, 100, 0, 14000, 14000, 14000, 14000, 0, 11, 75417, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Charscale Assaulter - In Combat - Cast ''Shockwave'''),
(40421, 0, 0, 0, 0, 0, 100, 0, 12000, 12000, 11000, 11000, 0, 11, 15621, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Charscale Elite - In Combat - Cast ''Skull Crack'''),
(40423, 0, 0, 1, 0, 0, 100, 0, 8000, 8000, 40000, 40000, 0, 11, 75414, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Charscale Commander - In Combat - Cast ''Rallying Shout'''),
(40423, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Charscale Commander - In Combat - Say Line 0'),
(40423, 0, 2, 0, 0, 0, 100, 0, 15000, 15000, 14000, 14000, 0, 11, 13737, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Charscale Commander - In Combat - Cast ''Mortal Strike''');

INSERT IGNORE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(40423, 0, 0, '%s rallies the other combatants with a battle roar!', 41, 0, 100, 0, 0, 0, 'Charscale Commander');

-- Every spawn here carried the 10 player normal bit only, so the 25 player normal run had no creatures or objects.
-- TrinityCore has them in all four modes. The four exit portals are one per mode in TrinityCore, not four in one.
UPDATE `creature` SET `spawnMask` = 24 WHERE `map` = 724 AND `spawnMask` = 8;
UPDATE `gameobject` SET `spawnMask` = 24 WHERE `map` = 724 AND `spawnMask` = 8 AND `id` NOT IN (204051, 204052, 204053, 204054);
UPDATE `gameobject` SET `spawnMask` = 8 WHERE `map` = 724 AND `id` = 204051;
UPDATE `gameobject` SET `spawnMask` = 16 WHERE `map` = 724 AND `id` = 204053;
UPDATE `gameobject` SET `spawnMask` = 32 WHERE `map` = 724 AND `id` = 204052;
UPDATE `gameobject` SET `spawnMask` = 64 WHERE `map` = 724 AND `id` = 204054;
