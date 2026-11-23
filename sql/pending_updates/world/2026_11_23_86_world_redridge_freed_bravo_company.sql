-- Redridge Mountains, Bravo Company rescue chain: the freed Messner (43300), Jorgensen (43305), Krakauer (43303) and Danforth
-- (43302) are summoned next to the player and give the quests 26560 to 26563. Once the player has accepted the next quest
-- the soldier who gave it leaves (is despawned), as in Legends of Azeroth 5.4.8. They stayed standing next to the player.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (43300, 43302, 43303, 43305) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (43300, 43302, 43303, 43305);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43300, 0, 0, 0, 19, 0, 100, 0, 26560, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Messner - On Accepted Quest - Despawn'),
(43302, 0, 0, 0, 19, 0, 100, 0, 26563, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Danforth - On Accepted Quest - Despawn'),
(43303, 0, 0, 0, 19, 0, 100, 0, 26562, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Krakauer - On Accepted Quest - Despawn'),
(43305, 0, 0, 0, 19, 0, 100, 0, 26561, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jorgensen - On Accepted Quest - Despawn');
