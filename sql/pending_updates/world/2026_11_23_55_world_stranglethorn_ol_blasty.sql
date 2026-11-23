-- Ol' Blasty (26647): the quest asks the player to hit Smilin' Timmy Sticks five times with a cannonball from the
-- cannon Ol' Blasty (43562). The cannon was a vehicle (VehicleId 972) but had no spell click and no spell click flag, no
-- spells on its bar and no script, so it could not be boarded and Timmy gave no credit. Fire! (81506) triggers Fire!
-- Effect (81507), which hits Timmy and gives the credit, and spell 81507 may only hit him.
-- The spell click, the bar spells (Aim, Fire!, Ready), the script (the cannon stays rooted while ridden) and Timmy's credit
-- row are as in Legends of Azeroth 5.4.8. The source names the cannon's AI SmartVehicleAI, which this core does not have:
-- SmartAI handles the passenger events. The source also sets creature flag 16384 on both templates, which is not taken.

UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = 'SmartAI', `spell1` = 81514, `spell2` = 81506, `spell3` = 81513 WHERE `entry` = 43562 AND `ScriptName` = '';
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 43560 AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 43562;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(43562, 46598, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 81507;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 81507, 0, 0, 31, 0, 3, 43560, 0, 0, 0, 0, '', 'Fire! Effect - targets Smilin'' Timmy Sticks');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43562 AND `id` IN (0, 1, 2);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43560 AND `id` IN (0);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43562, 0, 0, 0, 27, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ol'' Blasty - On Passenger Boarded - Set Root'),
(43562, 0, 1, 2, 28, 0, 100, 0, 0, 0, 0, 0, 0, 103, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ol'' Blasty - On Passenger Removed - Reset Root'),
(43562, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ol'' Blasty - On Passenger Removed - Evade'),
(43560, 0, 0, 0, 8, 0, 100, 0, 81507, 0, 0, 0, 0, 33, 43560, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Smilin'' Timmy Sticks - On Spellhit - Killcredit');
