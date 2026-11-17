-- Heroic fixes for Ahn'kahet. The two Ahn'kahar Watchers of the heroic mode and the Brazier KC Bunny were missing, and
-- the Watcher and the bunny had no AI. Spawns and AI are the TrinityCore ones; positions are identical in both
-- references.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (31104, 31105) AND `AIName` = '';

DELETE FROM `creature` WHERE `guid` IN (8359900, 8359901, 8359902);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8359900, 31104, 619, 4, 0, 0, 0, 0, 454.153, -1024.99, 30.4352, 5.21853, 7200, 5, 0, 1, 0, 1, 0, 0, 0),
(8359901, 31104, 619, 4, 0, 0, 0, 0, 460.032, -1034.93, 30.3338, 1.97222, 7200, 5, 0, 1, 0, 1, 0, 0, 0),
(8359902, 31105, 619, 4, 0, 0, 0, 0, 520.706, -352.902, 47.9224, 2.23402, 7200, 0, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (31104, 31105);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(31104, 0, 0, 0, 9, 0, 100, 0, 0, 5, 14000, 17000, 0, 11, 42746, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ahn kahar Watcher - In Combat - Cast Cleave'),
(31104, 0, 1, 0, 0, 0, 100, 0, 9000, 12000, 15000, 18000, 0, 11, 56643, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ahn kahar Watcher - In Combat - Cast Triple Slash'),
(31104, 0, 2, 0, 2, 0, 100, 0, 0, 30, 8000, 10000, 0, 11, 56646, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ahn kahar Watcher - Between 0-30% Health - Cast Enrage'),
(31105, 0, 0, 1, 8, 0, 100, 8, 58515, 0, 0, 0, 0, 11, 58518, 2, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 'Ahn kahet Brazier KC Bunny - On Spell Hit (Burn Corpse) - Give Quest Credit (Heroic Only)'),
(31105, 0, 1, 0, 61, 0, 100, 8, 0, 0, 0, 0, 0, 11, 58522, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ahn kahet Brazier KC Bunny - Linked with Previous Event - Cast Ahn kahet Brazier Effect');
