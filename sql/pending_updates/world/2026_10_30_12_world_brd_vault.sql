-- Blackrock Depths: the Vault. Opening the twelve Relic Coffer Doors makes each Relic Coffer lootable; the
-- instance script counts the doors (SkyFire's SmartAI has no counters) and at twelve wakes the Warbringer
-- Constructs and summons Watchman Doomgrip, whose death opens the Secret Door and so the Secret Safe.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (8905, 9476) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (8905, 9476);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 890500;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8905, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 80, 890500, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warbringer Construct - On data set - Wake up'),
(8905, 0, 1, 2, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warbringer Construct - On respawn - Set passive'),
(8905, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warbringer Construct - Linked - Set not selectable'),
(8905, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warbringer Construct - Linked - Set event phase 0'),
(8905, 0, 4, 0, 0, 0, 100, 0, 4000, 4000, 8000, 8000, 0, 11, 15621, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Warbringer Construct - In combat - Cast Skull Crack'),
(890500, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warbringer Construct - Action list - Remove unit flag'),
(890500, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warbringer Construct - Action list - Set aggressive'),
(890500, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warbringer Construct - Action list - SET PHASE EVENT 0'),
(9476, 0, 0, 0, 0, 0, 100, 0, 4000, 4000, 8000, 8000, 0, 11, 11971, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Watchman Doomgrip - In combat - Cast Sunder Armor'),
(9476, 0, 1, 0, 0, 0, 100, 0, 10000, 10000, 14000, 14000, 0, 11, 15504, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Watchman Doomgrip - In combat - Cast Drink Healing Potion'),
(9476, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 20, 174553, 70, 0, 0, 0, 0, 0, 'Watchman Doomgrip - On death - Open the Secret Door'),
(9476, 0, 3, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Watchman Doomgrip - On aggro - Say text 0');

DELETE FROM `creature_text` WHERE `entry` = 9476;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(9476, 0, 0, 'What are you doing?  Intruders!!', 14, 0, 100, 0, 0, 0, 'Watchman Doomgrip');

UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (174553, 174554, 174555, 174556, 174557, 174558, 174559, 174560, 174561, 174562, 174563, 174564, 174566) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` IN (174553, 174554, 174555, 174556, 174557, 174558, 174559, 174560, 174561, 174562, 174563, 174564, 174566);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(174554, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174554, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174555, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174555, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174556, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174556, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174557, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174557, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174558, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174558, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174559, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174559, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174560, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174560, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174561, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174561, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174562, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174562, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174563, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174563, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174564, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174564, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174566, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 35, 28, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Relic Coffer Door - On opened - Tell the instance'),
(174566, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 160836, 10, 0, 0, 0, 0, 0, 'Relic Coffer Door - Linked - Make the Relic Coffer lootable'),
(174553, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 20, 161495, 10, 0, 0, 0, 0, 0, 'Secret Door - On opened - Make the Secret Safe usable');
