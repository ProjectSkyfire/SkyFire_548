-- Azuremyst Isle, Bloodmyst Isle and Exodar quest fixes.
-- Sources: 5.4.8 SmartAI for the Blacksilt Scout and for Matis the Cruel (4.3.4 has the Blacksilt Scout row, with the
-- 512 event flag cleared and the cast target written as invoker; 4.3.4 has no Matis script that completes the quest on
-- its own). The Bloodcursed Voyager spawns are 4.3.4 data. Geezle's
-- scene is the core's own script, now bound.
-- Where 5.4.8 SmartAI uses action 203 (combat stop) it is written as 27 in this core. No cast flags are used.
-- Safe to run twice: every block deletes its own keys first or sets absolute values.

-- "Catch and Release" (9629): the six Blacksilt Scouts are tagged with the Murloc Tagger (spell 30877). The credit
-- creature "Tagged Blacksilt Scout" (17654) is not spawned and the scout has no AI, so the spell hit gave nothing.
-- The scout gives the credit to the player who hit it, goes passive and despawns after 5 seconds.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 17326;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 17326;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17326, 0, 0, 1, 8, 0, 100, 0, 30877, 0, 30000, 30000, 0, 33, 17654, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blacksilt Scout - On Spellhit Tag Murloc - Quest Credit'),
(17326, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blacksilt Scout - Linked To Id 0 - Set React State Passive'),
(17326, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 256, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blacksilt Scout - Linked To Id 1 - Set UnitFlags 256'),
(17326, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blacksilt Scout - Linked To Id 2 - Despawn after 5 sec');

-- "The Hopeless Ones..." (9682) needs 4 Bloodcursed Souls, which only the Bloodcursed Voyager (17714) drops (80 percent
-- quest drop). Home has 4 of the 18 spawns of 4.3.4, so the souls (and the Irradiated Crystal Shards of 9641 and 9642,
-- which the Voyager also drops) are contested. These are the 14 missing 4.3.4 spawns (guids 75068 to 75081 there). No
-- Voyager stands within 3 yards of any of them.
DELETE FROM `creature` WHERE `guid` BETWEEN 8333100 AND 8333113;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8333100, 17714, 530, 1, 0, 0, 0, 0, -981.083, -12847.7, -78.3502, 2.45272, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333101, 17714, 530, 1, 0, 0, 0, 0, -981.314, -12813.6, -51.9604, 0.810614, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333102, 17714, 530, 1, 0, 0, 0, 0, -1135.07, -12931.6, -98.4871, 2.43533, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333103, 17714, 530, 1, 0, 0, 0, 0, -1119.17, -12949.7, -99.689, 2.53529, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333104, 17714, 530, 1, 0, 0, 0, 0, -1151.62, -12952.3, -101.069, 2.9852, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333105, 17714, 530, 1, 0, 0, 0, 0, -1150.57, -12917.9, -101.545, 3.95037, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333106, 17714, 530, 1, 0, 0, 0, 0, -1201.32, -12933.4, -102.708, 1.19337, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333107, 17714, 530, 1, 0, 0, 0, 0, -1183.29, -12912.6, -102.463, 3.73105, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333108, 17714, 530, 1, 0, 0, 0, 0, -1221.54, -12954.4, -104.158, 4.28428, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333109, 17714, 530, 1, 0, 0, 0, 0, -1252.93, -12952.8, -104.744, 3.63331, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333110, 17714, 530, 1, 0, 0, 0, 0, -1134.56, -12897.9, -102.611, 2.9341, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333111, 17714, 530, 1, 0, 0, 0, 0, -1147.65, -12879.5, -102.42, 4.14375, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333112, 17714, 530, 1, 0, 0, 0, 0, -1411.56, -12765.2, -16.1739, 1.5953, 300, 5, 0, 356, 0, 1, 0, 0, 0),
(8333113, 17714, 530, 1, 0, 0, 0, 0, -1415.73, -12795.1, -15.7665, 1.83091, 300, 5, 0, 356, 0, 1, 0, 0, 0);

-- "Matis the Cruel" (9711): the quest has no objective and is flagged as an event quest (special flag 2), so it only
-- completes when the quest-complete spell "Capturing Matis" (31339) hits the player. Nothing casts it in home: Matis
-- has no AI. The 5.4.8 script makes him give up at half health: he stops fighting, casts the spell on the closest
-- player within 5 yards, speaks and leaves after 10 seconds. Three 5.4.8 rows are left out because they only move the
-- Tracker of the Hand (17853, a text-over trigger and a set-data row in the list) or set a cosmetic mount model (2346).
-- Two 5.4.8 comments were swapped (the emote row and the set-data row of the list) and are corrected here.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 17664;

DELETE FROM `creature_text` WHERE `entry` = 17664;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(17664, 0, 0, 'You will never take me alive!', 12, 0, 100, 0, 0, 0, 'Matis the Cruel'),
(17664, 1, 0, '%s falls unconscious.', 16, 0, 100, 0, 0, 0, 'Matis the Cruel');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 17664;
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 1766400;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17664, 0, 0, 7, 11, 0, 100, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - On Respawn - Set Invincibility Hp 1'),
(17664, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 1000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - On Aggro - Say Line 0'),
(17664, 0, 3, 4, 2, 0, 100, 1, 0, 50, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - On Health Pct 0-50% - Combat Stop'),
(17664, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33555200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - Linked To Id 3 - Set UnitFlag 33555200'),
(17664, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 31339, 0, 0, 0, 0, 0, 25, 5, 1, 0, 0, 0, 0, 0, 'Matis the Cruel - Linked To Id 4 - Cast Capturing Matis'),
(17664, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 1766400, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - Linked To Id 5 - Run Script'),
(17664, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 33555200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - Linked To Id 0 - Remove UnitFlag 33555200 on Reset'),
(1766400, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 445, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - On Script - Play Emote 445'),
(1766400, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - On Script - Say Line 1'),
(1766400, 9, 2, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Matis the Cruel - On Script - Despawn after 10 sec');

-- "Tree's Company" (9531): the Tree Disguise Kit summons Geezle, whose scene (npc_geezle) gives the credit for
-- "The Traitor Uncovered"; the script was never bound to him (it is registered again in the same change).
UPDATE `creature_template` SET `ScriptName` = 'npc_geezle' WHERE `entry` = 17318;
