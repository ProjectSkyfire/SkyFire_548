-- Mount Hyjal: four leftovers of the quest creature scripts

-- Thol'embaar (40107) runs the timed action list 4010700 when he appears (Ring of Fire, his opening line, the end of his
-- immunity), but the list was missing, so his intro never played
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4010700;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4010700, 9, 0, 0, 0, 0, 100, 0, 0, 1000, 0, 0, 0, 11, 74788, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar - Script - Cast Ring of Fire'),
(4010700, 9, 1, 0, 0, 0, 100, 0, 2000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar - Script - Say line 0'),
(4010700, 9, 2, 0, 0, 0, 100, 0, 0, 1000, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar - Script - Remove immune flag');

-- Marion Wormwing (41112) is fought, subdued by Thisalee Crow and questioned through her SmartAI, as quest 25731 describes
-- it; the old script that credits the quest at half health took the AI away from the SmartAI, so it is no longer bound.
-- Thisalee Crow (41114) is the vehicle Marion rides after she is subdued (vehicle 809, as in the 5.4.8 data).
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 41112 AND `ScriptName` = 'npc_marion_wormswing';
UPDATE `creature_template` SET `VehicleId` = 809, `unit_flags` = 32768 WHERE `entry` = 41114 AND `VehicleId` = 0;
-- the 5.4.8 move-offset action is not in this core: her despawn follows the data event directly
UPDATE `smart_scripts` SET `event_type` = 38, `event_param1` = 0, `event_param2` = 2, `comment` = 'Marion Wormwing - On Data Set 0 2 - Despawn after 5 sec' WHERE `entryorguid` = 41112 AND `source_type` = 0 AND `id` = 15 AND `event_type` = 61;

-- Twilight Juggernaut (41031) loses one layer of Armor Plating for each counter value; the counter events (77) and the set
-- counter action (116, the 5.4.8 data uses 63 for it) come with the counter support of the core
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41031 AND `id` IN (6, 10, 15, 19, 22, 24);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41031, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 116, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Juggernaut - Linked To Id 5 - Set Counter 1 1'),
(41031, 0, 10, 11, 77, 0, 100, 0, 1, 1, 0, 0, 0, 28, 77224, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Juggernaut - On Counter 1 1 - Remove Aura Armor Plating'),
(41031, 0, 15, 16, 77, 0, 100, 0, 1, 2, 0, 0, 0, 28, 77224, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Juggernaut - On Counter 1 2 - Remove Aura Armor Plating'),
(41031, 0, 19, 20, 77, 0, 100, 0, 1, 3, 0, 0, 0, 28, 77224, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Juggernaut - On Counter 1 3 - Remove Aura Armor Plating'),
(41031, 0, 22, 23, 77, 0, 100, 0, 1, 4, 0, 0, 0, 28, 77224, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Juggernaut - On Counter 1 4 - Remove Aura Armor Plating'),
(41031, 0, 24, 0, 77, 0, 100, 0, 1, 5, 0, 0, 0, 28, 77224, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Juggernaut - On Counter 1 5 - Remove Aura Armor Plating');

-- quest 25332 (Get Me Outta Here!) follows Gar'gol's Gotta Go only; the Lycanthoth quests do not lead to it
UPDATE `quest_template` SET `NextQuestId` = 0 WHERE `Id` IN (25272, 25273) AND `NextQuestId` = 25332;
