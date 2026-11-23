-- New Tinkertown (Gnome starting zone): quest scripting that the 4.3.4 and 5.4.8 data have and the base data lacked (zone audit
-- quests that stopped or broke). Every row below is written for the
-- action and event numbers of this core. No new spawns, so no new guids.
-- Sources: the 5.4.8 data (SmartAI, waypoints, texts) for the Razlo Crushcog fight, the Multi-Bot and the Toxic Pools; 4.3.4
-- for the Multi-Bot summon on the engineer and for the gossip condition.


-- ---------------------------------------------------------------------------------------------------------------
-- Quest 26208 "The Fight Continues": the objective (credit 42463) was credited only by spell 79227 at the very end
-- of High Tinker Mekkatorque's speech (list 4231700, about 90 s after accepting). The 5.4.8 data gives the credit by an
-- explicit kill-credit action; the same action is added after the spell cast (a second credit does nothing), so the
-- objective no longer depends on how that spell picks its target.
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4231700 AND `source_type` = 9 AND `id` = 14;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4231700, 9, 14, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 42463, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Quest Credit (Operation: Gnomeregan Recap Credit)');


-- ---------------------------------------------------------------------------------------------------------------
-- Quest 26364 "Down with Crushcog!": the objective is the credit 42860 and nothing here gave it. The fight of
-- High Tinker Mekkatorque, Mountaineer Stonegrind and the four Crushcog's Guardians against Razlo Crushcog had no scripting
-- at all (the 4.3.4 data has the texts but no AI), so the gossip option "I'm ready to start the assault." did nothing.
-- The rows are the 5.4.8 ones, translated to this core: the counter of Razlo (four guardians must die before he can be
-- attacked) uses event phases because the counter event and action do not exist here, and the combat stop is action 27.
-- Differences from the 5.4.8 rows, all needed to make the fight run:
--   * Razlo wakes the guardians: his "Set Data 0 1" after the second line targets the guardians (42294) instead of the
--     technicians (43230), because only the guardians run a script on that data.
--   * Mekkatorque casts his four attacks in combat on a timer (5.4.8 only runs them on a data value nothing sends).
--   * The guardian scripts are keyed by spawn guid, as in 5.4.8; the guids below are the existing spawns at the same four places
--     (146243, 146244, 146245, 146258 for 168763, 168766, 168767, 168765).
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (42294, 42839, 42849, 42852) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (42839, 42849, 42852) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (-146243, -146244, -146245, -146258) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (4229400, 4229401, 4229402, 4229403, 4283900, 4283901, 4284900, 4284901, 4284902, 4284903, 4284904) AND `source_type` = 9;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 43230 AND `source_type` = 0 AND `id` IN (1, 2);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42849, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Respawn - Set React State Aggressive'),
(42849, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3000, 3000, 0, 49, 0, 0, 0, 0, 0, 0, 19, 42294, 5, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - In Combat - Attack Start'),
(42849, 0, 2, 3, 62, 0, 100, 0, 11662, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Gossip Select - Store Target List'),
(42849, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - Linked To Id 2 - Close Gossip'),
(42849, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4284900, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - Linked To Id 3 - Run Script'),
(42849, 0, 5, 6, 58, 0, 100, 0, 3, 42849, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 42839, 15, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Waypoint Ended - Set Data 0 1'),
(42849, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - Linked To Id 5 - Set Home'),
(42849, 0, 7, 8, 38, 0, 100, 0, 0, 1, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 42839, 10, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Data Set 0 1 - Attack Start'),
(42849, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - Linked To Id 7 - Set Phase 1'),
(42849, 0, 9, 0, 38, 1, 100, 0, 0, 3, 0, 0, 0, 87, 4284901, 4284902, 4284903, 4284904, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Data Set 0 3 - Run Random Script'),
(42849, 0, 10, 11, 38, 0, 100, 0, 0, 2, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Data Set 0 2 - Combat Stop'),
(42849, 0, 11, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 3, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - Linked To Id 10 - Say Line 3'),
(42849, 0, 12, 13, 52, 0, 100, 0, 3, 42849, 0, 0, 0, 1, 4, 25000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Text Over 3 - Say Line 4'),
(42849, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 42860, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - Linked To Id 12 - Quest Credit'),
(42849, 0, 14, 0, 52, 0, 100, 0, 4, 42849, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Text Over 4 - Despawn'),
(42849, 0, 15, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - Linked To Id 10 - Reset Phase'),
(42849, 0, 16, 0, 0, 1, 100, 0, 5000, 8000, 12000, 16000, 0, 87, 4284901, 4284902, 4284903, 4284904, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - In Combat - Run Random Script (attacks)'),
(42839, 0, 0, 15, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Respawn - Set React State Defensive'),
(42839, 0, 1, 0, 0, 0, 100, 0, 5000, 5000, 15000, 15000, 0, 87, 4283900, 4283901, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - In Combat - Run Random Script'),
(42839, 0, 2, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 0, 8000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Data Set 0 1 - Say Line 0'),
(42839, 0, 3, 0, 52, 0, 100, 0, 0, 42839, 0, 0, 0, 1, 1, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Text Over 0 - Say Line 1'),
(42839, 0, 4, 0, 52, 0, 100, 0, 1, 42839, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 42294, 40, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Text Over 1 - Set Data 0 1 (Crushcog''s Guardians)'),
(42839, 0, 5, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Data Set 0 2 - Say Line 2'),
(42839, 0, 6, 7, 38, 8, 100, 0, 0, 3, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Data Set 0 3 (fourth guardian dead) - Next Phase'),
(42839, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - Linked To Id 6 - Remove UnitFlag 768'),
(42839, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 42849, 10, 0, 0, 0, 0, 0, 'Razlo Crushcog - Linked To Id 7 - Set Data 0 1 (High Tinker Mekkatorque)'),
(42839, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 42852, 10, 0, 0, 0, 0, 0, 'Razlo Crushcog - Linked To Id 8 - Set Data 0 2 (Mountaineer Stonegrind)'),
(42839, 0, 10, 0, 38, 7, 100, 0, 0, 3, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Data Set 0 3 (guardian dead) - Next Phase'),
(42839, 0, 11, 12, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 42849, 10, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Death - Set Data 0 2 (High Tinker Mekkatorque)'),
(42839, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 42852, 10, 0, 0, 0, 0, 0, 'Razlo Crushcog - Linked To Id 11 - Set Data 0 3 (Mountaineer Stonegrind)'),
(42839, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - Linked To Id 12 - Despawn'),
(42839, 0, 14, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - On Reset - Set Phase 1'),
(42839, 0, 15, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Razlo Crushcog - Linked To Id 0 - Set Phase 1'),
(42852, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - On Respawn - Set React State Aggressive'),
(42852, 0, 1, 0, 0, 0, 100, 0, 0, 0, 3000, 3000, 0, 49, 0, 0, 0, 0, 0, 0, 19, 42294, 5, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - In Combat - Attack Start'),
(42852, 0, 2, 3, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - On Data Set 0 1 - Say Line 0'),
(42852, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 0, 42852, 0, 26364, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - Linked To Id 2 - Waypoint Start'),
(42852, 0, 4, 0, 58, 0, 100, 0, 2, 42852, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - On Waypoint Ended - Set Home'),
(42852, 0, 5, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 42839, 10, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - On Data Set 0 2 - Attack Start'),
(42852, 0, 6, 7, 38, 0, 100, 0, 0, 3, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - On Data Set 0 3 - Combat Stop'),
(42852, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 30000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - Linked To Id 6 - Say Line 1'),
(42852, 0, 8, 0, 52, 0, 100, 0, 1, 42852, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mountaineer Stonegrind - On Text Over 1 - Despawn'),
(-146243, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 80, 4229400, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Data Set 0 1 - Run Script (spawn 146243)'),
(-146243, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 42839, 15, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Death - Set Data 0 3 (spawn 146243)'),
(-146244, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 80, 4229401, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Data Set 0 1 - Run Script (spawn 146244)'),
(-146244, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 42839, 15, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Death - Set Data 0 3 (spawn 146244)'),
(-146245, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 80, 4229403, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Data Set 0 1 - Run Script (spawn 146245)'),
(-146245, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 42839, 15, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Death - Set Data 0 3 (spawn 146245)'),
(-146258, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 80, 4229402, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Data Set 0 1 - Run Script (spawn 146258)'),
(-146258, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 42839, 15, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Death - Set Data 0 3 (spawn 146258)'),
(43230, 0, 1, 2, 54, 0, 100, 0, 0, 0, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog Technician - On Summoned - Remove UnitFlag 768'),
(43230, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 30, 0, 0, 0, 0, 0, 0, 'Crushcog Technician - Linked To Id 1 - Attack Start'),
(4229400, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set Run 1'),
(4229400, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 28, 16245, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove Aura Freeze Anim'),
(4229400, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5252.98, 128.316, 394.184, 0, 'Crushcog''s Guardian - On Script - Move To Pos'),
(4229400, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove UnitFlag 33554432'),
(4229400, 9, 4, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove UnitFlag 768'),
(4229400, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 42839, 10, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set Data 0 2'),
(4229400, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set React State Aggressive'),
(4229400, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 42852, 10, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Attack Start'),
(4229401, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set Run 1'),
(4229401, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 28, 16245, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove Aura Freeze Anim'),
(4229401, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5252.07, 123.959, 393.777, 0, 'Crushcog''s Guardian - On Script - Move To Pos'),
(4229401, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove UnitFlag 33554432'),
(4229401, 9, 4, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove UnitFlag 768'),
(4229401, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set React State Aggressive'),
(4229401, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 42852, 10, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Attack Start'),
(4229402, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set Run 1'),
(4229402, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 28, 16245, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove Aura Freeze Anim'),
(4229402, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5253.38, 115.766, 393.962, 0, 'Crushcog''s Guardian - On Script - Move To Pos'),
(4229402, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove UnitFlag 33554432'),
(4229402, 9, 4, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove UnitFlag 768'),
(4229402, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set React State Aggressive'),
(4229402, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 42849, 10, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Attack Start'),
(4229403, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set Run 1'),
(4229403, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 28, 16245, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove Aura Freeze Anim'),
(4229403, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -5255.27, 111.534, 393.167, 0, 'Crushcog''s Guardian - On Script - Move To Pos'),
(4229403, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove UnitFlag 33554432'),
(4229403, 9, 4, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Remove UnitFlag 768'),
(4229403, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Set React State Aggressive'),
(4229403, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 42849, 10, 0, 0, 0, 0, 0, 'Crushcog''s Guardian - On Script - Attack Start'),
(4283900, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 43230, 6, 5000, 1, 0, 0, 8, 0, 0, 0, -5267.87, 105.625, 392.128, 0, 'Razlo Crushcog - On Script - Summon Creature Crushcog Technician'),
(4283901, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 43230, 6, 5000, 1, 0, 0, 8, 0, 0, 0, -5270.25, 129.416, 392.886, 0, 'Razlo Crushcog - On Script - Summon Creature Crushcog Technician'),
(4284900, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Remove NpcFlag 1'),
(4284900, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Say Line 0'),
(4284900, 9, 2, 0, 0, 0, 100, 0, 13000, 13000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Say Line 1'),
(4284900, 9, 3, 0, 0, 0, 100, 0, 11000, 11000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Say Line 2'),
(4284900, 9, 4, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 42852, 5, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Set Data 0 1'),
(4284900, 9, 5, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 53, 0, 42849, 0, 26364, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Waypoint Start'),
(4284901, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Say Line 5'),
(4284901, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 80110, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Cast Mekkatorque-Missiles'),
(4284902, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Say Line 6'),
(4284902, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 22742, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Cast Super Shrink Ray'),
(4284903, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Say Line 7'),
(4284903, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 80148, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Cast Mekkatorque-Ray'),
(4284904, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Say Line 8'),
(4284904, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 80098, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Cast Mekkatorque-Cannon');


DELETE FROM `waypoints` WHERE `entry` IN (42849, 42852);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(42849, 1, -5292.57, 132.09, 386.318, 'High Tinker Mekkatorque'),
(42849, 2, -5280.62, 124.816, 390.736, 'High Tinker Mekkatorque'),
(42849, 3, -5261.72, 119.5, 393.865, 'High Tinker Mekkatorque'),
(42852, 1, -5278.12, 129.457, 391.236, 'Mountaineer Stonegrind'),
(42852, 2, -5260.35, 124.175, 393.962, 'Mountaineer Stonegrind');


DELETE FROM `creature_text` WHERE `entry` IN (42839, 42849, 42852);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42839, 0, 0, 'You! How did you escape detection by my sentry-bots?', 12, 0, 100, 0, 0, 21243, 'Razlo Crushcog'),
(42839, 1, 0, 'No matter! My guardians and I will make short work of you. To arms, men!', 12, 0, 100, 0, 0, 21244, 'Razlo Crushcog'),
(42839, 2, 0, 'You will never defeat the true sons of Gnomeregan!', 12, 0, 100, 0, 0, 21245, 'Razlo Crushcog'),
(42849, 0, 0, 'Mekgineer Thermaplugg refuses to acknowledge that his defeat is imminent! He has sent Razlo Crushcog to prevent us from rebuilding our beloved Gnomeregan!', 12, 0, 100, 5, 0, 20890, 'High Tinker Mekkatorque'),
(42849, 1, 0, 'But $n has thwarted his plans at every turn, and the dwarves of Ironforge stand with us!', 12, 0, 100, 25, 0, 20891, 'High Tinker Mekkatorque'),
(42849, 2, 0, 'Let''s send him crawling back to his master in defeat!', 12, 0, 100, 5, 0, 20892, 'High Tinker Mekkatorque'),
(42849, 3, 0, 'We''ve done it! We''re victorious!', 12, 0, 100, 5, 0, 20893, 'High Tinker Mekkatorque'),
(42849, 4, 0, 'With Crushcog defeated, Thermaplugg is sure to be quaking in his mechano-tank, and rightly so. You''re next Thermaplugg. You''re next!', 12, 0, 100, 5, 0, 20894, 'High Tinker Mekkatorque'),
(42849, 5, 0, 'Mekkatorque-Cannon!', 14, 0, 100, 0, 0, 0, 'High Tinker Mekkatorque'),
(42849, 6, 0, 'Mekkatorque-Ray!', 14, 0, 100, 0, 0, 0, 'High Tinker Mekkatorque'),
(42849, 7, 0, 'Mekkatorque-Missiles!', 14, 0, 100, 0, 0, 0, 'High Tinker Mekkatorque'),
(42849, 8, 0, 'Mekkatorque-Beam!', 14, 0, 100, 0, 0, 0, 'High Tinker Mekkatorque'),
(42852, 0, 0, 'Aye, let''s teach this addle-brained gnome a lesson!', 12, 0, 100, 25, 0, 0, 'Mountaineer Stonegrind'),
(42852, 1, 0, 'That''ll teach you to mess with the might of Ironforge and Gnomeregan!', 12, 0, 100, 25, 0, 0, 'Mountaineer Stonegrind');


-- The gossip option of Mekkatorque (menu 11662) is only offered while "Down with Crushcog!" is taken, as in 5.4.8.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 11662 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11662, 0, 0, 0, 9, 0, 26364, 0, 0, 0, 0, 0, '', 'Mekkatorque gossip option is offered while Down with Crushcog! is taken');


-- ---------------------------------------------------------------------------------------------------------------
-- Quest 26205 "A Job for the Multi-Bot": the quest needs the GS-9x Multi-Bot (a follower that cleans the toxic pools
-- and gives the credit 42563), but nobody summoned it: Engineer Grindspark and the Multi-Bot had no scripting and the
-- Toxic Pools no AI. Accepting the quest (or the gossip option "I need a new GS-9x Multibot.") now makes the player cast
-- the summon spell 79419 (4.3.4 rows of Grindspark; the gossip option is only shown while the quest is taken, as in 4.3.4).
-- The Multi-Bot and Toxic Pool rows are the 5.4.8 ones; the Multi-Bot reacts to each pool by spawn guid (5.4.8 guids
-- 167559 ... 168543, the 16 existing pools at the same places are listed in the comments). The two Grindspark cutscene rows
-- of 4.3.4 (reward of 26222) are cosmetic and not added.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (42553, 42563, 42598) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (42553, 42563, 42598) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4259800 AND `source_type` = 9;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42553, 0, 0, 0, 19, 0, 100, 0, 26205, 0, 0, 0, 0, 85, 79419, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Engineer Grindspark - On Quest Accept - Invoker Cast ''Force Cast Summon Multi-Bot'''),
(42553, 0, 1, 2, 62, 0, 100, 0, 12634, 0, 0, 0, 0, 85, 79419, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Engineer Grindspark - On Gossip Option Select - Invoker Cast Force Cast Summon Multi-Bot'),
(42553, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Engineer Grindspark - Linked To Id 1 - Close Gossip'),
(42563, 0, 0, 0, 1, 0, 100, 0, 10000, 10000, 10000, 10000, 0, 11, 79391, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Toxic Pool - Out of Combat - Cast Toxic Eruption'),
(42563, 0, 1, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Toxic Pool - On Data Set 0 1 - Despawn'),
(42598, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Summoned - Store Target List'),
(42598, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 0 - Set Run 1'),
(42598, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 1 - Follow Target'),
(42598, 0, 3, 4, 75, 0, 100, 1, 146348, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146348)'),
(42598, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 3 - Run Script'),
(42598, 0, 5, 6, 75, 0, 100, 1, 146387, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146387)'),
(42598, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 5 - Run Script'),
(42598, 0, 7, 8, 75, 0, 100, 1, 146349, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146349)'),
(42598, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 7 - Run Script'),
(42598, 0, 9, 10, 75, 0, 100, 1, 146385, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146385)'),
(42598, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 9 - Run Script'),
(42598, 0, 11, 12, 75, 0, 100, 1, 146433, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146433)'),
(42598, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 11 - Run Script'),
(42598, 0, 13, 14, 75, 0, 100, 1, 146513, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146513)'),
(42598, 0, 14, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 13 - Run Script'),
(42598, 0, 15, 16, 75, 0, 100, 1, 146432, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146432)'),
(42598, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 15 - Run Script'),
(42598, 0, 17, 18, 75, 0, 100, 1, 146420, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146420)'),
(42598, 0, 18, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 17 - Run Script'),
(42598, 0, 19, 20, 75, 0, 100, 1, 146417, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146417)'),
(42598, 0, 20, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 19 - Run Script'),
(42598, 0, 21, 22, 75, 0, 100, 1, 146419, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146419)'),
(42598, 0, 22, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 21 - Run Script'),
(42598, 0, 23, 24, 75, 0, 100, 1, 146363, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146363)'),
(42598, 0, 24, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 23 - Run Script'),
(42598, 0, 25, 26, 75, 0, 100, 1, 146524, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146524)'),
(42598, 0, 26, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 25 - Run Script'),
(42598, 0, 27, 28, 75, 0, 100, 1, 146525, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146525)'),
(42598, 0, 28, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 27 - Run Script'),
(42598, 0, 29, 30, 75, 0, 100, 1, 146431, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146431)'),
(42598, 0, 30, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 29 - Run Script'),
(42598, 0, 31, 32, 75, 0, 100, 1, 146362, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146362)'),
(42598, 0, 32, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 31 - Run Script'),
(42598, 0, 33, 34, 75, 0, 100, 1, 146337, 0, 3, 8000, 0, 29, 0, 0, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Distance Creature - Follow Target (pool spawn 146337)'),
(42598, 0, 34, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4259800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - Linked To Id 33 - Run Script'),
(4259800, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Script - Say Line 0'),
(4259800, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79424, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Script - Cast Clean Up Toxic Pool'),
(4259800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 79422, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Script - Cast Toxic Pool Credit to Master'),
(4259800, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 94513, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Script - Cast Green Fuelbot Transform'),
(4259800, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79424, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Script - Cast Clean Up Toxic Pool'),
(4259800, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 42563, 3, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Script - Set Data 0 1'),
(4259800, 9, 6, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'GS-9x Multi-Bot - On Script - Follow Target');


DELETE FROM `creature_text` WHERE `entry` = 42598;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42598, 0, 0, 'Initiating cleanup ... ...', 12, 0, 100, 0, 0, 0, 'GS-9x Multi-Bot');


DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 12634 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 12634, 0, 0, 0, 9, 0, 26205, 0, 0, 0, 0, 0, '', 'Grindspark gossip option is offered while A Job for the Multi-Bot is taken');
