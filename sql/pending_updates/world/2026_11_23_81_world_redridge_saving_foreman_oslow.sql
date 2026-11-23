-- Saving Foreman Oslow (26520): the player uses the Control Ettin item (spell 80704) on a Canyon Ettin, which is replaced by
-- a Subdued Canyon Ettin under the player's control (spell 80702), and then on the Huge Boulder at the Lakeshire bridge:
-- the ettin lifts the boulder (spell 80739), the bridge workers cheer, the ettin throws the boulder into the lake and the
-- player gets the credit for Foreman Oslow (341). Nothing of this existed: the spell could be used on anything, the
-- boulder, the ettin, Oslow and the workers had no script and the ettin was a level 1 humanoid without a vehicle.
-- Rows, texts, spell scripts and spell target conditions are as in Legends of Azeroth 5.4.8 (the Cataclysm Preservation
-- Project 4.3.4 has the same two spell target conditions). The source gives the credit to the player stored when the
-- ettin is summoned, which it does here too.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (341, 648, 649, 650, 651, 652, 653, 43196) AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 18, `maxlevel` = 18, `unit_flags` = 64, `type` = 5, `VehicleId` = 938, `AIName` = 'SmartAI' WHERE `entry` = 43197 AND `ScriptName` = '';

DELETE FROM `spell_scripts` WHERE `id` IN (80704, 80739);
INSERT INTO `spell_scripts` (`id`, `effIndex`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(80704, 1, 0, 15, 80702, 0, 0, 0, 0, 0, 0),
(80739, 0, 0, 15, 82566, 0, 0, 0, 0, 0, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (80704, 80739);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 80739, 0, 0, 31, 0, 3, 43197, 0, 0, 0, 0, '', 'Lift Huge Boulder - Target is Subdued Canyon Ettin'),
(13, 2, 80704, 0, 0, 31, 0, 3, 43094, 0, 0, 0, 0, '', 'Control Ettin - Target is Canyon Ettin'),
(13, 2, 80704, 0, 1, 31, 0, 3, 43196, 0, 0, 0, 0, '', 'Control Ettin - Target is Huge Boulder');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (341, 648, 649, 650, 651, 652, 653, 43196, 43197);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(341, 0, 0, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Foreman Oslow - On Data Set - Despawn'),
(648, 0, 0, 1, 38, 0, 100, 0, 2, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - State Emote Stop'),
(648, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Run'),
(648, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -9281.44, -2285.27, 67.5123, 0, 'Bridge Worker - On Data Set - Move to Pos'),
(648, 0, 3, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 341, 20, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Orientation'),
(648, 0, 4, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Despawn'),
(649, 0, 0, 1, 38, 0, 100, 0, 2, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - State Emote Stop'),
(649, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Run'),
(649, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -9282.8, -2293.28, 67.5089, 0, 'Bridge Worker - On Data Set - Move to Pos'),
(649, 0, 3, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 341, 20, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Orientation'),
(649, 0, 4, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Despawn'),
(650, 0, 0, 1, 38, 0, 100, 0, 2, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - State Emote Stop'),
(650, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Run'),
(650, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -9282.27, -2290.95, 67.5319, 0, 'Bridge Worker - On Data Set - Move to Pos'),
(650, 0, 3, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 341, 20, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Orientation'),
(650, 0, 4, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Despawn'),
(651, 0, 0, 1, 38, 0, 100, 0, 2, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - State Emote Stop'),
(651, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Run'),
(651, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -9281.77, -2287.55, 67.5869, 0, 'Bridge Worker - On Data Set - Move to Pos'),
(651, 0, 3, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 341, 20, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Orientation'),
(651, 0, 4, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Despawn'),
(652, 0, 0, 1, 38, 0, 100, 0, 2, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - State Emote Stop'),
(652, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Run'),
(652, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -9280.71, -2283.21, 67.5747, 0, 'Bridge Worker - On Data Set - Move to Pos'),
(652, 0, 3, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 341, 20, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Orientation'),
(652, 0, 4, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Despawn'),
(653, 0, 0, 1, 38, 0, 100, 0, 2, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - State Emote Stop'),
(653, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Run'),
(653, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -9279.86, -2281.42, 67.5854, 0, 'Bridge Worker - On Data Set - Move to Pos'),
(653, 0, 3, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 341, 20, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Set Orientation'),
(653, 0, 4, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bridge Worker - On Data Set - Despawn'),
(43196, 0, 0, 0, 8, 0, 100, 0, 80704, 0, 240000, 240000, 0, 80, 4319600, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huge Boulder - On Spellhit - Run Script'),
(43197, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Subdued Canyon Ettin - On Respawn - Store Target'),
(43197, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Subdued Canyon Ettin - On Respawn - Set Passive'),
(43197, 0, 2, 0, 38, 0, 100, 0, 1, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 19, 43196, 20, 0, 0, 0, 0, 0, 'Subdued Canyon Ettin - On Data Set - Move to Pos'),
(43197, 0, 3, 0, 38, 0, 100, 0, 4, 0, 0, 0, 0, 33, 341, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Subdued Canyon Ettin - On Data Set - Killcredit'),
(43197, 0, 4, 5, 38, 0, 100, 0, 5, 0, 0, 0, 0, 28, 82566, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Subdued Canyon Ettin - On Data Set - Remove Aura Lift Boulder Ride'),
(43197, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Subdued Canyon Ettin - On Data Set - Despawn');

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (4319600);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4319600, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 19, 43197, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 1, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 9, 648, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 9, 649, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 3, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 9, 650, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 4, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 9, 651, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 5, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 9, 652, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 6, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 9, 653, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 7, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 11, 80739, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Cast Lift Huge Boulder'),
(4319600, 9, 8, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 9, 648, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 9, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 9, 649, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 10, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 9, 650, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 11, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 9, 651, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 12, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 9, 652, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 13, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 9, 653, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 14, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 43197, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Say Line 0'),
(4319600, 9, 15, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 43197, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Say Line 1'),
(4319600, 9, 16, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 649, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Say Line 0'),
(4319600, 9, 17, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 653, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Say Line 0'),
(4319600, 9, 18, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 43197, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Say Line 2'),
(4319600, 9, 19, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 4, 0, 0, 0, 0, 0, 19, 43197, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 20, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 19, 43197, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 21, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 9, 648, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 22, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 9, 649, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 23, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 9, 650, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 24, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 9, 651, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 25, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 9, 652, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 26, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 9, 653, 0, 20, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 27, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 19, 341, 20, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Set Data'),
(4319600, 9, 28, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Huge Boulder - Script - Despawn');

DELETE FROM `creature_text` WHERE `entry` IN (649, 653, 43197);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(649, 0, 0, 'HO!', 12, 0, 100, 0, 0, 0, 'Bridge Worker Dmitri'),
(653, 0, 0, 'PUT YOUR BACKS INTO IT, BOYS!', 12, 0, 100, 0, 0, 0, 'Bridge Worker Alex'),
(43197, 0, 0, 'ROCK NOT SO HEAVY! PUNY HUMIES!', 12, 0, 100, 0, 0, 0, 'Subdued Canyon Ettin'),
(43197, 1, 0, 'Where trow? TROW ON BRIDGE??', 12, 0, 100, 0, 0, 0, 'Subdued Canyon Ettin'),
(43197, 2, 0, 'OK! Me trow in water!', 12, 0, 100, 0, 0, 0, 'Subdued Canyon Ettin');
