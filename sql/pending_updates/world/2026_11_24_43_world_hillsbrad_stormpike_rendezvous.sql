-- Stormpike Rendezvous (28348): accepting the quest from Kingslayer Orkus (48218) starts a short scene in which Orkus's
-- double, Vanndar Stormpike, Captain Balinda Stonehearth and Kasha appear, talk and fight, and the player gets the
-- credit at the end. Orkus had no script and the four scene actors were placeholder templates (level 1, friendly, no
-- AI), so the quest could never be completed. Added as in Legends of Azeroth 5.4.8: Orkus's gossip menu, flags and
-- AI (on accepting the quest he stores the player and runs the scene, hiding his quest flag until it ends), the levels,
-- factions and AI of the four actors, their texts, and the timed script. The actors are summoned for a minute at most.

UPDATE `creature_template` SET `gossip_menu_id` = 12430, `unit_flags` = 768, `AIName` = 'SmartAI' WHERE `entry` = 48218;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 29, `faction_H` = 29, `unit_flags` = 32768, `AIName` = 'SmartAI' WHERE `entry` IN (48396, 48397);
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `faction_A` = 1216, `faction_H` = 1216, `AIName` = 'SmartAI' WHERE `entry` = 48403;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `faction_A` = 1216, `faction_H` = 1216, `unit_flags` = 32768, `AIName` = 'SmartAI' WHERE `entry` = 48407;

DELETE FROM `creature_text` WHERE `entry` IN (48397, 48403, 48407);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(48397, 0, 0, 'Ugh, the lok''tar must have been cooking up a turd - which would explain the smell.', 12, 0, 100, 0, 0, 0, ''),
(48397, 1, 0, 'By Hellscream''s axe!', 12, 0, 100, 5, 0, 0, ''),
(48397, 2, 0, 'You face Kingslayer Orkus now, dwarf!', 12, 0, 100, 5, 0, 0, ''),
(48397, 3, 0, 'KASHA! Your master needs you!', 14, 0, 100, 22, 0, 0, ''),
(48403, 0, 0, 'Oy, ''Linda, where are we meetin'' up with this Ivar Bloodfang fella? An'' what in Bronzebeard''s name is that terrible smell?', 12, 0, 100, 1, 0, 0, ''),
(48403, 1, 0, 'Look there, ''Linda! What in the...', 12, 0, 100, 0, 0, 0, ''),
(48407, 0, 0, 'They are massing at Purgation Isle, just west of us here, off the coast. We are to meet them there, general.', 12, 0, 100, 1, 0, 0, ''),
(48407, 1, 0, 'As for the smell... *Balinda sniffs at the air* I think it''s coming from over there...', 12, 0, 100, 1, 0, 0, '');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 48218 AND `source_type` = 0 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48218, 0, 0, 1, 19, 0, 100, 0, 28348, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kingslayer Orkus - On Accepted Quest - Store Target'),
(48218, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4821800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kingslayer Orkus - On Link - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 48396 AND `source_type` = 0 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48397 AND `source_type` = 0 AND `id` IN (0, 1, 2, 3);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48403 AND `source_type` = 0 AND `id` IN (1, 2);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48407 AND `source_type` = 0 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48396, 0, 0, 0, 38, 0, 100, 0, 6, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 19, 48407, 100, 0, 0, 0, 0, 0, 'Kasha - On Data Set 6 - Aggro'),
(48397, 0, 0, 0, 38, 0, 100, 0, 1, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -890.572, -574.041, 7.29059, 0, 'Kingslayer Orkus - On Data Set 1 - Move to Pos'),
(48397, 0, 1, 0, 38, 0, 100, 0, 2, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 2.25001, 'Kingslayer Orkus - On Data Set 2 - Set Orientation'),
(48397, 0, 2, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 97, 10, 10, 0, 0, 0, 0, 1, 0, 0, 0, -891.764, -571.821, 8.60625, 0, 'Kingslayer Orkus - On Data Set 3 - Jump to Pos'),
(48397, 0, 3, 0, 38, 0, 100, 0, 4, 0, 0, 0, 0, 97, 15, 15, 0, 0, 0, 0, 1, 0, 0, 0, -904.302, -560.513, 6.87974, 0, 'Kingslayer Orkus - On Data Set 4 - Jump to Pos'),
(48403, 0, 1, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Vanndar Stormpike - On Data Set 3 - Set Orientation'),
(48403, 0, 2, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Vanndar Stormpike - On Data Set 5 - Aggro'),
(48407, 0, 1, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Captain Balinda Stonehearth - On Data Set 3 - Set Orientation'),
(48407, 0, 2, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Captain Balinda Stonehearth - On Data Set 5 - Aggro');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 4821800 AND `source_type` = 9 AND `id` IN (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4821800, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 48, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Active ON'),
(4821800, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 83, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Remove Quest Flag'),
(4821800, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 48397, 3, 60000, 0, 0, 0, 8, 0, 0, 0, -875.485, -573.102, 11.4638, 2.25001, 'Kingslayer Orkus - Script - Summon Kingslayer Orkus'),
(4821800, 9, 3, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 48403, 3, 60000, 0, 0, 0, 8, 0, 0, 0, -904.302, -560.513, 6.87974, 2.25001, 'Kingslayer Orkus - Script - Summon Vanndar Stormpike'),
(4821800, 9, 4, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 48407, 3, 60000, 0, 0, 0, 8, 0, 0, 0, -901.909, -557.962, 6.87974, 2.25001, 'Kingslayer Orkus - Script - Summon Captain Balinda Stonehearth'),
(4821800, 9, 5, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 18, 33536, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Immine Flags'),
(4821800, 9, 6, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 18, 33536, 0, 0, 0, 0, 0, 19, 48403, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Immine Flags'),
(4821800, 9, 7, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 18, 33536, 0, 0, 0, 0, 0, 19, 48407, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Immine Flags'),
(4821800, 9, 8, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Say Lines'),
(4821800, 9, 9, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Say Lines'),
(4821800, 9, 10, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 11, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 12, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 48403, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Say Lines'),
(4821800, 9, 13, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 48407, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Say Lines'),
(4821800, 9, 14, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 48407, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Say Lines'),
(4821800, 9, 15, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 48403, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Say Lines'),
(4821800, 9, 16, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 19, 48403, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 17, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 19, 48407, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 18, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 19, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Say Lines'),
(4821800, 9, 20, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 45, 4, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 21, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 19, 512, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Remove Immine Flags'),
(4821800, 9, 22, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 19, 512, 0, 0, 0, 0, 0, 19, 48403, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Remove Immine Flags'),
(4821800, 9, 23, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 19, 512, 0, 0, 0, 0, 0, 19, 48407, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Remove Immine Flags'),
(4821800, 9, 24, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 19, 48403, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 25, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 19, 48407, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 26, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 19, 48397, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Say Lines'),
(4821800, 9, 27, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 48396, 3, 7000, 0, 0, 0, 8, 0, 0, 0, -897.797, -573.781, 6.87964, 2.25001, 'Kingslayer Orkus - Script - Summon Kasha'),
(4821800, 9, 28, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 18, 33024, 0, 0, 0, 0, 0, 19, 48396, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Immine Flags'),
(4821800, 9, 29, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 6, 0, 0, 0, 0, 0, 19, 48396, 100, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Data'),
(4821800, 9, 30, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 15, 28348, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Questcredit'),
(4821800, 9, 31, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 82, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Reset Quest Flag'),
(4821800, 9, 32, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 48, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kingslayer Orkus - Script - Set Active OFF');
