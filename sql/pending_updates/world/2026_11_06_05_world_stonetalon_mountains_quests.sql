-- Stonetalon Mountains quest fixes.
-- Sources: 4.3.4 for the quest and creature data the database already has. Everything inserted below comes from the
-- 5.4.8 data (spawns, SmartAI, creature text, loot rows, spellclick, vehicle and item rows), because the 4.3.4 data has
-- none of it or its mechanism is not in the database. Safe to run twice: every block deletes its own keys first or
-- sets absolute values.

-- Quest NPCs that were never spawned, so their quests could not be finished or taken: Furien (ends "Nothing Left for
-- You Here"), Nura Pathfinder (ends "Nura Pathfinder"), the Overlord Krom'gar copy at Cliffwalker Post (ends "Might
-- Makes Right", gives "To Be Horde...") together with Garrosh Hellscream (the "To Be Horde..." objective), and Officer
-- Jankie (ends the Alliance quest "Help for Desolace"). Positions are the 5.4.8 spawns.
DELETE FROM `creature` WHERE `guid` BETWEEN 8330400 AND 8330404;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8330400, 35286, 1, 1, 0, 0, 0, 0, 189.557, 1770.96, 91.2064, 1.79769, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Furien
(8330401, 37135, 1, 1, 0, 0, 0, 0, -454.924, -972.305, 96.1874, 0.523599, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Nura Pathfinder
(8330402, 42106, 1, 1, 0, 0, 0, 0, 2067.93, 1213.52, 450.781, 0.359037, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Overlord Krom'gar (Cliffwalker Post)
(8330403, 42124, 1, 1, 0, 0, 0, 0, 2079.78, 1218.43, 451.325, 3.63029, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Garrosh Hellscream
(8330404, 36410, 1, 1, 0, 0, 0, 0, 175.338, 1782.81, 89.6453, 5.6715, 300, 0, 0, 0, 0, 0, 0, 0, 0); -- Officer Jankie

-- "Lessons from the Lost Isles" asks for 12 Darnassian Scouts but only 3 were spawned. These are the other 11 of the
-- 14 5.4.8 spawns (three 5.4.8 spawns sit on the existing ones). Walking spawns without a path stay in place.
DELETE FROM `creature` WHERE `guid` BETWEEN 8330405 AND 8330415;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8330405, 41865, 1, 1, 0, 0, 0, 0, 1257.91, -592.002, 21.385, 0.727785, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330406, 41865, 1, 1, 0, 0, 0, 0, 1209.21, -510.116, 11.7313, 1.76663, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330407, 41865, 1, 1, 0, 0, 0, 0, 1283.69, -385.881, 14.304, 5.44785, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330408, 41865, 1, 1, 0, 0, 0, 0, 1184.38, -382.813, 10.5248, 3.14159, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330409, 41865, 1, 1, 0, 0, 0, 0, 1123.48, -473.939, 9.89102, 1.96171, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330410, 41865, 1, 1, 0, 0, 0, 0, 1147.19, -451.429, 16.6546, 2.90319, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330411, 41865, 1, 1, 0, 0, 0, 0, 1181.61, -414.279, 16.1608, 4.4251, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330412, 41865, 1, 1, 0, 0, 0, 0, 1152.75, -509.9, 6.66794, 0.199547, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330413, 41865, 1, 1, 0, 0, 0, 0, 1252.44, -509.387, 15.2487, 5.87431, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330414, 41865, 1, 1, 0, 0, 0, 0, 1230.11, -557.24, 10.9571, 3.53605, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330415, 41865, 1, 1, 0, 0, 0, 0, 1192.6, -560.633, 8.73039, 2.40517, 60, 0, 0, 0, 0, 0, 0, 0, 0);

-- Alliance "They Put the Assass in... Never Mind" asks for 12 Krom'gar Assassins (stealthed, revealed by the Infrared
-- Heat Detection Device) but only 1 was spawned. These are 24 more of the 25 5.4.8 spawns (one sits on the existing one).
DELETE FROM `creature` WHERE `guid` BETWEEN 8330416 AND 8330439;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8330416, 41291, 1, 1, 0, 0, 0, 0, 732.292, -238.439, 7.49518, 4.78632, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330417, 41291, 1, 1, 0, 0, 0, 0, 539.276, -167.093, 25.1345, 4.041, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330418, 41291, 1, 1, 0, 0, 0, 0, 599.356, -132.836, 19.8559, 3.44731, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330419, 41291, 1, 1, 0, 0, 0, 0, 674.094, -157.045, 34.0083, 3.48151, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330420, 41291, 1, 1, 0, 0, 0, 0, 478.522, -170.984, 32.2564, 5.14513, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330421, 41291, 1, 1, 0, 0, 0, 0, 502.661, -155.255, 28.0512, 5.56768, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330422, 41291, 1, 1, 0, 0, 0, 0, 462.73, -185.694, 33.116, 2.46455, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330423, 41291, 1, 1, 0, 0, 0, 0, 478.495, -198.956, 34.6513, 5.40114, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330424, 41291, 1, 1, 0, 0, 0, 0, 572.353, -176.997, 21.2444, 3.30104, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330425, 41291, 1, 1, 0, 0, 0, 0, 608.419, -154.399, 32.8404, 3.74227, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330426, 41291, 1, 1, 0, 0, 0, 0, 570.022, -121.154, 44.2318, 0.5211, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330427, 41291, 1, 1, 0, 0, 0, 0, 648.45, -194.5, 19.668, 4.77124, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330428, 41291, 1, 1, 0, 0, 0, 0, 744.393, -193.229, 3.35675, 2.59598, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330429, 41291, 1, 1, 0, 0, 0, 0, 689.72, -200.169, 11.525, 4.1675, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330430, 41291, 1, 1, 0, 0, 0, 0, 440.267, -235.524, 57.1133, 3.42395, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8330431, 41291, 1, 1, 0, 0, 0, 0, 516.687, -168.025, 45.5862, 3.30501, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330432, 41291, 1, 1, 0, 0, 0, 0, 527.967, -133.972, 27.9662, 6.00224, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330433, 41291, 1, 1, 0, 0, 0, 0, 556.719, -134.969, 24.3293, 4.33074, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330434, 41291, 1, 1, 0, 0, 0, 0, 404.624, -268.418, 66.2258, 1.97472, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330435, 41291, 1, 1, 0, 0, 0, 0, 612.95, -188.413, 20.3384, 5.24198, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330436, 41291, 1, 1, 0, 0, 0, 0, 604.117, -232.817, 22.1456, 6.07547, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330437, 41291, 1, 1, 0, 0, 0, 0, 601.274, -167.563, 21.3198, 1.28446, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330438, 41291, 1, 1, 0, 0, 0, 0, 557.087, -201.794, 26.1357, 5.18268, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8330439, 41291, 1, 1, 0, 0, 0, 0, 456.582, -175.137, 57.8649, 0.640937, 60, 0, 0, 0, 0, 0, 0, 0, 0);

-- "In Defense of Krom'gar Fortress" needs 25 Gnome Wing Commanders. They sit on the Gnomish Bombers as the passenger
-- (5.4.8 vehicle accessory); the bombers were spawned without it.
DELETE FROM `vehicle_template_accessory` WHERE `entry` = 42015 AND `accessory_entry` = 42017;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(42015, 42017, 0, 0, 'Gnomish Bomber - Gnome Wing Commander', 6, 30000);

-- Templates that need the scripts below: Frightened Peon (spellclick flag), Krom'gar Marauder, Grimtotem Bodyguard,
-- Warsong Stockpile and the Sniper Cannon (spellclick flag). The Northwatch Tent object gets SmartAI.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = `npcflag` | 16777216 WHERE `entry` = 41989;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (41360, 41351, 40957);
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` = 32254;
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 203431;

-- The Krom'gar Flame Thrower ("Ashes to Ashes") had no use spell. 5.4.8 gives it spell 78141 and no cooldown.
UPDATE `item_template` SET `spellid_1` = 78141, `spellcooldown_1` = 0 WHERE `entry` = 56473;

-- "BEWARE OF CRAGJAW!" (Alliance 25730, Horde 26043) are both started from the same sign, and neither had a race
-- limit, so each faction was offered the other's quest. Masks are the 5.4.8 ones.
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `Id` = 25730;
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `Id` = 26043;

-- Frightened Peon ("And That's Why They Call Them Peons...") and the Sniper Cannon ("The Only Way Down is in a Body
-- Bag") are used through spellclick. The click rows and their quest conditions are 5.4.8 data.
DELETE FROM `npc_spellclick_spells` WHERE (`npc_entry`, `spell_id`) IN ((41989, 77412), (32254, 63852));
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(41989, 77412, 1, 0),
(32254, 63852, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND (`SourceGroup`, `SourceEntry`) IN ((41989, 77412), (32254, 63852));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 41989, 77412, 0, 0, 9, 0, 26047, 0, 0, 0, 0, 0, '', 'Frightened Peon requires quest 26047 taken for spellclick 77412'),
(18, 32254, 63852, 0, 0, 9, 0, 25614, 0, 0, 0, 0, 0, '', 'Sniper Cannon requires quest 25614 taken for spellclick 63852');

-- Creature text for the scripts below (Frightened Peon, and the Boulderslide Rock Keeper, Geomancer and Stonepounder
-- that the Subjugator's whip turns). 5.4.8 text.
DELETE FROM `creature_text` WHERE `entry` IN (41989, 11915, 11917, 11918);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(11915, 0, 0, 'NO! ME NO HELP HORDE!', 12, 0, 100, 0, 0, 0, ''),
(11915, 0, 1, 'Whip make me angry!', 12, 0, 100, 0, 0, 0, ''),
(11915, 0, 2, 'Me shove whip down face!', 12, 0, 100, 0, 0, 0, ''),
(11915, 0, 3, 'Me make you eat whip!', 12, 0, 100, 0, 0, 0, ''),
(11915, 0, 4, 'You is BAD $r!', 12, 0, 100, 0, 0, 0, ''),
(11915, 0, 5, 'ME SO ANGRY!', 12, 0, 100, 0, 0, 0, ''),
(11915, 1, 0, 'OK! OK! Me do what $r wants!', 12, 0, 100, 0, 0, 0, ''),
(11915, 1, 1, 'Please no whip me! Me help Horde!', 12, 0, 100, 0, 0, 0, ''),
(11915, 1, 2, 'Hurts! Stop!', 12, 0, 100, 0, 0, 0, ''),
(11915, 1, 3, 'No more hurt! Me join you!', 12, 0, 100, 0, 0, 0, ''),
(11917, 0, 0, 'NO! ME NO HELP HORDE!', 12, 0, 100, 0, 0, 0, ''),
(11917, 0, 1, 'Whip make me angry!', 12, 0, 100, 0, 0, 0, ''),
(11917, 0, 2, 'Me shove whip down face!', 12, 0, 100, 0, 0, 0, ''),
(11917, 0, 3, 'Me make you eat whip!', 12, 0, 100, 0, 0, 0, ''),
(11917, 0, 4, 'You is BAD $r!', 12, 0, 100, 0, 0, 0, ''),
(11917, 0, 5, 'ME SO ANGRY!', 12, 0, 100, 0, 0, 0, ''),
(11917, 1, 0, 'OK! OK! Me do what $r wants!', 12, 0, 100, 0, 0, 0, ''),
(11917, 1, 1, 'Please no whip me! Me help Horde!', 12, 0, 100, 0, 0, 0, ''),
(11917, 1, 2, 'Hurts! Stop!', 12, 0, 100, 0, 0, 0, ''),
(11917, 1, 3, 'No more hurt! Me join you!', 12, 0, 100, 0, 0, 0, ''),
(11918, 0, 0, 'NO! ME NO HELP HORDE!', 12, 0, 100, 0, 0, 0, ''),
(11918, 0, 1, 'Whip make me angry!', 12, 0, 100, 0, 0, 0, ''),
(11918, 0, 2, 'Me shove whip down face!', 12, 0, 100, 0, 0, 0, ''),
(11918, 0, 3, 'Me make you eat whip!', 12, 0, 100, 0, 0, 0, ''),
(11918, 0, 4, 'You is BAD $r!', 12, 0, 100, 0, 0, 0, ''),
(11918, 0, 5, 'ME SO ANGRY!', 12, 0, 100, 0, 0, 0, ''),
(11918, 1, 0, 'OK! OK! Me do what $r wants!', 12, 0, 100, 0, 0, 0, ''),
(11918, 1, 1, 'Please no whip me! Me help Horde!', 12, 0, 100, 0, 0, 0, ''),
(11918, 1, 2, 'Hurts! Stop!', 12, 0, 100, 0, 0, 0, ''),
(11918, 1, 3, 'No more hurt! Me join you!', 12, 0, 100, 0, 0, 0, ''),
(41989, 0, 0, 'Oof! You touch naughty spot!', 12, 0, 100, 0, 0, 0, ''),
(41989, 0, 1, 'Which way to go???', 12, 0, 100, 0, 0, 0, ''),
(41989, 0, 2, 'Tanks!', 12, 0, 100, 0, 0, 0, ''),
(41989, 0, 3, 'You is friend. Tanks!', 12, 0, 100, 0, 0, 0, ''),
(41989, 0, 4, 'Why dis happen???', 12, 0, 100, 0, 0, 0, '');

-- SmartAI, 5.4.8 rows:
--  41989 Frightened Peon: spellclick gives the credit, removes the click flag, shouts and runs off ("And That's Why
--        They Call Them Peons...").
--  41360 Krom'gar Marauder: Jin'Zil's Voodoo Stick spell hit gives the credit ("Jin'Zil's Blessing").
--  41351 Grimtotem Bodyguard: Alliance Weapon Crate spell hit gives the credit ("Grimtotem Supremacy").
--  40957 Warsong Stockpile: flamethrower hit gives the credit after 4 seconds ("Burn, Baby, Burn!").
--  203431 Northwatch Tent (object): state change gives the credit and respawns it ("Ashes to Ashes").
--  11915, 11917, 11918 Boulderslide kobolds: Subjugator Devo's whip turns half of them hostile and half into a
--        followed, credited Subdued Kobold ("Reinforcements..."). Added as rule 10 beside the existing combat rows;
--        action 203 of the 5.4.8 numbering is 27 (combat stop) in this core.
DELETE FROM `smart_scripts` WHERE (`source_type` = 0 AND `entryorguid` IN (41360, 41351, 40957, 41989)) OR (`source_type` = 1 AND `entryorguid` = 203431) OR (`source_type` = 0 AND `entryorguid` IN (11915, 11917, 11918) AND `id` = 10) OR (`source_type` = 9 AND `entryorguid` IN (1191501, 1191502, 1191701, 1191702, 1191801, 1191802));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40957, 0, 0, 1, 8, 0, 100, 0, 65308, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Warsong Stockpile - On SpellHit Flamethrower - Store Target List'),
(40957, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 40957, 4000, 4000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warsong Stockpile - Linked To Id 0 - Create Timed Event'),
(40957, 0, 2, 3, 59, 0, 100, 0, 40957, 0, 0, 0, 0, 33, 40957, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Warsong Stockpile - On Timed Event Triggered - Quest Credit'),
(40957, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Warsong Stockpile - Linked To Id 2 - Despawn'),
(41351, 0, 0, 0, 8, 0, 100, 0, 77526, 0, 0, 0, 0, 33, 41351, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grimtotem Bodyguard - On SpellHit Alliance Weapon Crate - Quest Credit'),
(41360, 0, 0, 0, 8, 0, 100, 0, 78337, 0, 60000, 60000, 0, 33, 41360, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Krom''gar Marauder - On SpellHit Voodoo Power - Quest Credit'),
(41989, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 11, 78269, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Frightened Peon - On Spellclick - Cast Quest Credit: That''s Why They Call Them Peons'),
(41989, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frightened Peon - Linked To Id 0 - Remove NpcFlag 16777216'),
(41989, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 2000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frightened Peon - Linked To Id 1 - Say Line Random'),
(41989, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frightened Peon - Linked To Id 2 - Set Run 1'),
(41989, 0, 4, 0, 52, 0, 100, 0, 0, 41989, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frightened Peon - On Text Over 0 - Despawn'),
(203431, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 41936, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Northwatch Tent - On GO State Change - Quest Credit'),
(203431, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 70, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Northwatch Tent - Linked To Id 0 - Respawn Object'),
(11915, 0, 10, 0, 8, 0, 100, 0, 78315, 0, 5000, 5000, 0, 87, 1191501, 1191502, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On SpellHit Whip ''Em - Run Random Script'),
(11917, 0, 10, 0, 8, 0, 100, 0, 78315, 0, 5000, 5000, 0, 87, 1191701, 1191702, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On SpellHit Whip ''Em - Run Random Script'),
(11918, 0, 10, 0, 8, 0, 100, 0, 78315, 0, 5000, 5000, 0, 87, 1191801, 1191802, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On SpellHit Whip ''Em - Run Random Script'),
(1191501, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 75, 78317, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Add Aura Whipped!'),
(1191501, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Say Line 0'),
(1191501, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Attack Start'),
(1191502, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 42024, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Quest Credit'),
(1191502, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Combat Stop'),
(1191502, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Set React State Passive'),
(1191502, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Set Faction 35'),
(1191502, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 520, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Set UnitFlag 520'),
(1191502, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Set Phase 1'),
(1191502, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Say Line 1'),
(1191502, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 42023, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Rock Keeper - On Script - Follow Target'),
(1191701, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 75, 78317, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Add Aura Whipped!'),
(1191701, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Say Line 0'),
(1191701, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Attack Start'),
(1191702, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 42024, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Quest Credit'),
(1191702, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Combat Stop'),
(1191702, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Set React State Passive'),
(1191702, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Set Faction 35'),
(1191702, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 520, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Set UnitFlag 520'),
(1191702, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Set Phase 1'),
(1191702, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Say Line 1'),
(1191702, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 42023, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Geomancer - On Script - Follow Target'),
(1191801, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 75, 78317, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Add Aura Whipped!'),
(1191801, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Say Line 0'),
(1191801, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Attack Start'),
(1191802, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 42024, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Quest Credit'),
(1191802, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Combat Stop'),
(1191802, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Set React State Passive'),
(1191802, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Set Faction 35'),
(1191802, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 520, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Set UnitFlag 520'),
(1191802, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Set Phase 1'),
(1191802, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Say Line 1'),
(1191802, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 42023, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Boulderslide Stonepounder - On Script - Follow Target');

-- Quest items nothing dropped: Deepmoss Venom Sac (Preparations for the Future; only Queen Silith dropped it, one per
-- kill), Stormer Heart (Da Voodoo: Stormer Heart) and Boulderslide Cheese (A Proper Peace Offerin'). Chances are the
-- 5.4.8 quest drop chances.
DELETE FROM `creature_loot_template` WHERE (`entry`, `item`) IN ((4005, 55280), (4006, 55280), (4007, 55280), (4008, 56796), (11915, 56042), (11917, 56042), (11918, 56042));
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(4005, 55280, -41.8346, 1, 0, 1, 1), -- Deepmoss Venom Sac
(4006, 55280, -16.124, 1, 0, 1, 1), -- Deepmoss Venom Sac
(4007, 55280, -39.4902, 1, 0, 1, 1), -- Deepmoss Venom Sac
(11915, 56042, -38.2311, 1, 0, 1, 1), -- Boulderslide Cheese
(11917, 56042, -38.5308, 1, 0, 1, 1), -- Boulderslide Cheese
(11918, 56042, -45.2334, 1, 0, 1, 1), -- Boulderslide Cheese
(4008, 56796, -46.0275, 1, 0, 1, 1); -- Stormer Heart

-- Gameobjects that were never spawned: Azure Iron Ore ("Don't Look Them in the Eyes" wants 10; 9 of the 14 5.4.8
-- spawns are used because only ten object guids were reserved, with a 60 second respawn) and the Horde General's Chest
-- ("Downfall").
DELETE FROM `gameobject` WHERE `guid` BETWEEN 930020 AND 930029;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(930020, 203132, 1, 1, 0, 0, 923.74, -324.353, 18.6014, 1.99057, 0, 0, 0.838915, 0.544263, 60, 100, 1), -- Azure Iron Ore
(930021, 203132, 1, 1, 0, 0, 985.022, -324.999, 0.390552, 1.45779, 0, 0, 0.666045, 0.745911, 60, 100, 1), -- Azure Iron Ore
(930022, 203132, 1, 1, 0, 0, 986.43, -332.018, -0.406657, 0.464648, 0, 0, 0.23024, 0.973134, 60, 100, 1), -- Azure Iron Ore
(930023, 203132, 1, 1, 0, 0, 980.414, -336.829, -1.54567, 3.3525, 0, 0, -0.994445, 0.105259, 60, 100, 1), -- Azure Iron Ore
(930024, 203132, 1, 1, 0, 0, 930.212, -310.478, 17.5236, 4.1444, 0, 0, -0.876908, 0.480658, 60, 100, 1), -- Azure Iron Ore
(930025, 203132, 1, 1, 0, 0, 916.028, -320.581, 19.6463, 3.36024, 0, 0, -0.99403, 0.109107, 60, 100, 1), -- Azure Iron Ore
(930026, 203132, 1, 1, 0, 0, 900.574, -332.512, 18.1057, 2.70398, 0, 0, 0.976157, 0.217065, 60, 100, 1), -- Azure Iron Ore
(930027, 203132, 1, 1, 0, 0, 886.738, -314.734, 13.2916, 4.7835, 0, 0, -0.681524, 0.731795, 60, 100, 1), -- Azure Iron Ore
(930028, 203132, 1, 1, 0, 0, 893.391, -265.636, 6.02565, 2.43892, 0, 0, 0.938914, 0.344151, 60, 100, 1), -- Azure Iron Ore
(930029, 203281, 1, 1, 0, 0, -50.588, -488.721, -31.3919, 5.05685, 0, 0, -0.57546, 0.81783, 300, 100, 1); -- Horde General's Chest
