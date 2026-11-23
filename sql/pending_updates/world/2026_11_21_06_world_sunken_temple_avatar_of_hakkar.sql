-- The Avatar of Hakkar was never spawned and had no AI. He waits hidden and pacified at the altar until a player
-- uses the Shrine of the Soulflayer, then rises and fights with the three abilities the dungeon journal lists.
DELETE FROM `creature` WHERE `guid` = 9305000;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`) VALUES
(9305000, 8443, 109, 2, -467.198, 272.862, -90.3651, 3.08923, 43200, 0, 0, 9800, 0, 0);

UPDATE `creature_template` SET `AIName` = 'SmartAI', `mechanic_immune_mask` = 1 WHERE `entry` = 8443 AND `AIName` = '' AND `ScriptName` = '';
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 208321 AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE (`entryorguid` = 8443 AND `source_type` = 0) OR (`entryorguid` = 208321 AND `source_type` = 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8443, 0, 0, 1, 63, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Avatar of Hakkar - On Just Created - Hide'),
(8443, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 131072, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Avatar of Hakkar - On Just Created - Set Pacified'),
(8443, 0, 2, 3, 38, 0, 100, 0, 1, 1, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Avatar of Hakkar - On Data Set 1 - Show'),
(8443, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 131072, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Avatar of Hakkar - On Data Set 1 - Remove Pacified'),
(8443, 0, 4, 0, 0, 0, 100, 0, 8000, 8000, 8000, 8000, 0, 11, 6607, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Avatar of Hakkar - In Combat - Cast Lash'),
(8443, 0, 5, 0, 0, 0, 100, 0, 6000, 12000, 14000, 14500, 0, 11, 12889, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Avatar of Hakkar - In Combat - Cast Curse of Tongues'),
(8443, 0, 6, 0, 0, 0, 100, 0, 20000, 20000, 20000, 20000, 0, 11, 12888, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Avatar of Hakkar - In Combat - Cast Cause Insanity'),
(208321, 1, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 9, 8443, 0, 100, 0, 0, 0, 0, 'Shrine of the Soulflayer - On Gossip Hello - Set Data 1 1 on Avatar of Hakkar');
