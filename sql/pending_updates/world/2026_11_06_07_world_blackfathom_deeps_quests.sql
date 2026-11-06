-- Blackfathom Deeps: the two quest enders inside the dungeon were never spawned, and the Fathom Stone never summoned
-- Baron Aquanis. Spawns and scripts from the 5.4.8 data. Safe to run twice.

-- Je'neu Sancrea (ends Je'neu of the Earthen Ring and the Horde dungeon quest) and Sentinel Aluwyn (ends the Alliance
-- dungeon quests) stand at the entrance of the dungeon.
DELETE FROM `creature` WHERE `guid` IN (8330800, 8330801);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8330800, 12736, 48, 2, 0, 0, 0, 0, -157.078, 74.129, -45.5308, 1.5708, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8330801, 33260, 48, 2, 0, 0, 0, 0, -158.536, 74.219, -45.7543, 0.890118, 7200, 0, 0, 0, 0, 0, 0, 0, 0);

-- Opening the Fathom Stone summons Baron Aquanis (Baron Aquanis quest); he casts Frostbolt and Frost Nova.
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 177964 AND `ScriptName` = '';
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 12876 AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE (`entryorguid` = 177964 AND `source_type` = 1) OR (`entryorguid` = 12876 AND `source_type` = 0);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12876, 0, 0, 0, 0, 0, 100, 0, 0, 0, 4000, 6000, 0, 11, 15043, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Baron Aquanis - In Combat - Cast Frostbolt'),
(12876, 0, 1, 0, 0, 0, 100, 0, 7000, 12000, 14000, 19000, 0, 11, 14907, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baron Aquanis - In Combat - Cast Frost Nova'),
(177964, 1, 0, 0, 70, 0, 100, 0, 1, 0, 0, 0, 0, 12, 12876, 6, 300000, 1, 0, 0, 8, 0, 0, 0, -780.44, -79.3731, -44.0216, 0.680678, 'Fathom Stone - On State Changed - Summon Baron Aquanis');
