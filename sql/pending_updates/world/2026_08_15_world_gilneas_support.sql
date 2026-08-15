-- Gilneas zone script and database support.

DELETE FROM `smart_scripts`
WHERE (`source_type` = 0 AND `entryorguid` IN (34884, 35077, 35378, 35463, 35550, 35631, 35905, 35907, 36459, 36540, 36555, 43336, 44928, 50260))
   OR (`source_type` = 9 AND `entryorguid` IN (3488400, 3507700, 3537800, 3546300, 3555000, 3563100, 3590500, 3590700, 3645900, 3654000, 3655500, 4333600, 4492800, 5026000));

UPDATE `creature_template` SET `npcflag` = 3, `unit_flags` = 33280, `unit_flags2` = 2048, `flags_extra` = 2, `AIName` = '', `ScriptName` = 'npc_prince_liam_greymane' WHERE `entry` = 34850;
UPDATE `creature_template` SET `npcflag` = 0, `unit_flags` = 32768, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_gilneas_city_guard_gate' WHERE `entry` = 34864;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 8599, `unit_flags` = 32768, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_rampaging_worgen' WHERE `entry` = 34884;
UPDATE `creature_template` SET `npcflag` = 2, `AIName` = '', `ScriptName` = 'npc_lord_darius_crowley_35077' WHERE `entry` = 35077;
UPDATE `creature_template` SET `npcflag` = 2, `unit_flags` = 33280, `unit_flags2` = 2048, `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 35112;
UPDATE `creature_template` SET `npcflag` = 2, `unit_flags` = 33280, `unit_flags2` = 2048, `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 35115;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 8599, `unit_flags` = 32768, `unit_flags2` = 2048, `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 35118;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 35124;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 8599, `unit_flags` = 32768, `unit_flags2` = 2048, `flags_extra` = 64, `AIName` = '', `ScriptName` = 'npc_worgen_alpha_35167' WHERE `entry` = 35167;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 8599, `unit_flags` = 32768, `unit_flags2` = 2048, `flags_extra` = 64, `AIName` = '', `ScriptName` = 'npc_worgen_alpha_35170' WHERE `entry` = 35170;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 8599, `unit_flags` = 32768, `unit_flags2` = 2048, `flags_extra` = 64, `AIName` = '', `ScriptName` = 'npc_worgen_runt_35188' WHERE `entry` = 35188;
UPDATE `creature_template` SET `npcflag` = 2, `unit_flags` = 33280, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_josiah_avery' WHERE `entry` = 35369;
UPDATE `creature_template` SET `npcflag` = 0, `unit_flags` = 33024, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_josiah_avery_worgen_form' WHERE `entry` = 35370;
UPDATE `creature_template` SET `npcflag` = 2, `unit_flags` = 33280, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_lorna_crowley_basement' WHERE `entry` = 35378;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 8599, `unit_flags` = 32768, `unit_flags2` = 2048, `flags_extra` = 64, `AIName` = '', `ScriptName` = 'npc_worgen_runt' WHERE `entry` = 35456;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 8599, `unit_flags` = 32768, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_bloodfang_lurker' WHERE `entry` = 35463;
UPDATE `creature_template` SET `npcflag` = 3, `unit_flags` = 33280, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_king_genn_greymane' WHERE `entry` = 35550;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 67805, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_gilnean_mastiff' WHERE `entry` = 35631;
UPDATE `creature_template` SET `npcflag` = 0, `InhabitType` = 4, `AIName` = '', `ScriptName` = '' WHERE `entry` = 35753;
UPDATE `creature_template` SET `npcflag` = 16777216, `spell1` = 68219, `VehicleId` = 494, `unit_flags` = 33536, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_vehicle_genn_horse' WHERE `entry` = 35905;
UPDATE `creature_template` SET `npcflag` = 0, `unit_flags` = 33280, `unit_flags2` = 2080, `AIName` = '', `ScriptName` = 'npc_saved_aranas' WHERE `entry` = 35907;
UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = '', `ScriptName` = 'npc_cynthia' WHERE `entry` = 36287;
UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = '', `ScriptName` = 'npc_ashley' WHERE `entry` = 36288;
UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = '', `ScriptName` = 'npc_james' WHERE `entry` = 36289;
UPDATE `creature_template` SET `npcflag` = 2, `unit_flags` = 33280, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_wahl' WHERE `entry` = 36458;
UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = '', `ScriptName` = 'npc_chance_the_cat' WHERE `entry` = 36459;
UPDATE `creature_template` SET `npcflag` = 0, `spell1` = 41440, `unit_flags` = 33280, `unit_flags2` = 2048, `AIName` = '', `ScriptName` = 'npc_lucius_the_cruel' WHERE `entry` = 36461;
UPDATE `creature_template` SET `npcflag` = 1, `spell1` = 68903, `VehicleId` = 527, `AIName` = '', `ScriptName` = 'npc_mountain_horse' WHERE `entry` = 36540;
UPDATE `creature_template` SET `speed_walk` = 1, `speed_run` = 1.71429, `AIName` = '', `ScriptName` = 'npc_mountain_horse_summoned' WHERE `entry` = 36555;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` IN (36452, 36606, 38765);
UPDATE `creature_template` SET `VehicleId` = 958, `unit_flags2` = 268437504, `AIName` = '', `ScriptName` = 'npc_stagecoach_harness' WHERE `entry` = 43336;
UPDATE `creature_template` SET `npcflag` = 1, `VehicleId` = 959, `AIName` = '', `ScriptName` = 'npc_stagecoach_carriage_exodus' WHERE `entry` = 44928;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_gilneas_crow' WHERE `entry` = 50260;

DELETE FROM `smart_scripts`
WHERE (`source_type` = 0 AND `entryorguid` IN (35112, 35118, 36452, 36606, 38765))
   OR (`source_type` = 9 AND `entryorguid` IN (3511200));

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35112, 0, 0, 0, 19, 0, 100, 0, 14157, 0, 0, 0, 0, 80, 3511200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'King Genn Greymane - On Quest Accept - Start Script'),
(35118, 0, 0, 1, 2, 0, 100, 0, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - At 30% HP - Cast Enrage'),
(35118, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - At 30% HP - Say Text Line 0'),
(35118, 0, 2, 0, 8, 0, 100, 0, 100, 0, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Charge - Give Quest Credit'),
(35118, 0, 3, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Steady Shot - Give Quest Credit'),
(35118, 0, 4, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Eviscerate - Give Quest Credit'),
(35118, 0, 5, 0, 8, 0, 100, 0, 122, 0, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Frost Nova - Give Quest Credit'),
(35118, 0, 6, 0, 8, 0, 100, 0, 172, 0, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Corruption - Give Quest Credit'),
(35118, 0, 7, 0, 8, 0, 100, 0, 589, 0, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(35118, 0, 8, 0, 8, 0, 100, 0, 8921, 0, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Moonfire - Give Quest Credit'),
(35118, 0, 9, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 19, 35232, 35, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Reset - Attack Gilneas City Guard'),
(35118, 0, 10, 0, 1, 0, 100, 0, 10000, 10000, 10000, 10000, 0, 49, 0, 0, 0, 0, 0, 0, 19, 35232, 35, 0, 0, 0, 0, 0, 'Bloodfang Worgen - Out of Combat - Attack Gilneas City Guard'),
(36452, 0, 0, 0, 19, 0, 100, 0, 14465, 0, 0, 0, 0, 85, 72772, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gwen Armstead - On Quest Accept - Summon Horse'),
(36606, 0, 0, 0, 20, 0, 100, 0, 14465, 0, 0, 0, 0, 85, 68709, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Queen Mia Greymane - On Quest Rewarded - Cataclysm'),
(38765, 0, 0, 0, 27, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 38765, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Passenger Boarded - Start Waypoint'),
(38765, 0, 1, 0, 40, 0, 100, 0, 15, 0, 0, 0, 0, 28, 46598, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 15 - Remove Passenger'),
(38765, 0, 2, 0, 40, 0, 100, 0, 3, 0, 0, 0, 0, 54, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 3 - Pause'),
(38765, 0, 3, 0, 40, 0, 100, 0, 3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 14, 166783, 196863, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 3 - Open Gate'),
(38765, 0, 5, 0, 40, 0, 100, 0, 13, 0, 0, 0, 0, 54, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 13 - Pause'),
(38765, 0, 6, 0, 40, 0, 100, 0, 13, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 14, 166784, 196864, 0, 0, 0, 0, 0, 'Stout Mountain Horse - On Waypoint 13 - Open Gate'),
(3511200, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 81, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'King Genn Greymane - Remove Questgiver Flag'),
(3511200, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 35115, 30, 0, 0, 0, 0, 0, 'King Genn Greymane - Lord Godfrey Says Text Line 0'),
(3511200, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'King Genn Greymane - Say Text Line 0'),
(3511200, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 81, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'King Genn Greymane - Restore Questgiver Flag');

UPDATE `quest_template` SET `Flags` = 0 WHERE `Id` = 14204;
UPDATE `quest_template` SET `Flags` = 8 WHERE `Id` = 14465;

DELETE FROM `spell_script_names`
WHERE `spell_id` IN (66853, 66914, 67357, 67805, 69257)
   OR `ScriptName` IN ('spell_gen_gilneas_prison_periodic_dummy', 'spell_gilneas_prison_periodic_dummy', 'spell_gilneas_pull_to', 'spell_attack_lurker', 'spell_gilneas_test_telescope');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(66853, 'spell_gen_gilneas_prison_periodic_dummy'),
(66914, 'spell_gilneas_prison_periodic_dummy'),
(67357, 'spell_gilneas_pull_to'),
(67805, 'spell_attack_lurker'),
(69257, 'spell_gilneas_test_telescope');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (35905, 36459, 36540, 43336, 44928);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(35905, 63151, 1, 0),
(36459, 68743, 1, 0),
(43336, 46598, 0, 0),
(44928, 46598, 0, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 93275;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 93275, 0, 0, 31, 0, 3, 50260, 0, 0, 0, 0, '', 'Ping Gilnean Crow - Target Gilnean Crow');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 36459 AND `SourceEntry` = 68743;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 36459, 68743, 0, 0, 9, 0, 14401, 0, 0, 0, 0, 0, '', 'Requires Grandma''s Cat to enable spellclick');

DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (43336, 44928);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(43336, 43338, 0, 1, 'Stagecoach Harness - Horse', 8, 0),
(43336, 43338, 1, 1, 'Stagecoach Harness - Horse', 8, 0),
(43336, 44928, 2, 1, 'Stagecoach Harness - Caravan', 8, 0),
(44928, 38853, 0, 1, 'Stagecoach Carriage', 8, 0),
(44928, 44460, 2, 1, 'Stagecoach Carriage', 8, 0),
(44928, 36138, 3, 1, 'Stagecoach Carriage', 8, 0),
(44928, 43907, 4, 1, 'Stagecoach Carriage', 8, 0),
(44928, 43907, 5, 1, 'Stagecoach Carriage', 8, 0),
(44928, 51409, 6, 1, 'Stagecoach Carriage', 8, 0);

DELETE FROM `script_waypoint` WHERE `entry` = 43336;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES
(43336, 1, -1681.45, 2508.27, 97.84, 0, 'Stagecoach Harness'),
(43336, 2, -1695.46, 2486.82, 92.64, 0, 'Stagecoach Harness'),
(43336, 3, -1704.99, 2468.26, 84.84, 0, 'Stagecoach Harness'),
(43336, 4, -1698.38, 2447.65, 80.77, 0, 'Stagecoach Harness'),
(43336, 5, -1698.42, 2432.73, 76.52, 0, 'Stagecoach Harness'),
(43336, 6, -1725.88, 2390.50, 60.80, 0, 'Stagecoach Harness'),
(43336, 7, -1735.64, 2362.27, 63.15, 0, 'Stagecoach Harness'),
(43336, 8, -1745.18, 2343.69, 67.34, 0, 'Stagecoach Harness'),
(43336, 9, -1746.53, 2329.92, 69.60, 0, 'Stagecoach Harness'),
(43336, 10, -1757.21, 2300.38, 75.77, 0, 'Stagecoach Harness'),
(43336, 11, -1776.38, 2271.80, 82.11, 0, 'Stagecoach Harness'),
(43336, 12, -1799.38, 2251.48, 87.64, 0, 'Stagecoach Harness'),
(43336, 13, -1826.19, 2238.05, 89.31, 0, 'Stagecoach Harness'),
(43336, 14, -1868.50, 2174.60, 89.31, 0, 'Stagecoach Harness'),
(43336, 15, -1872.45, 2135.71, 89.31, 0, 'Stagecoach Harness'),
(43336, 16, -1872.96, 2075.18, 89.31, 0, 'Stagecoach Harness'),
(43336, 17, -1881.14, 2046.58, 89.31, 0, 'Stagecoach Harness'),
(43336, 18, -1885.72, 2019.56, 89.31, 0, 'Stagecoach Harness'),
(43336, 19, -1876.85, 1970.16, 89.17, 0, 'Stagecoach Harness'),
(43336, 20, -1878.35, 1921.33, 89.13, 0, 'Stagecoach Harness'),
(43336, 21, -1890.52, 1904.26, 89.15, 0, 'Stagecoach Harness'),
(43336, 22, -1990.23, 1901.42, 89.28, 0, 'Stagecoach Harness'),
(43336, 23, -2036.81, 1914.56, 83.23, 0, 'Stagecoach Harness'),
(43336, 24, -2061.07, 1905.41, 73.95, 0, 'Stagecoach Harness'),
(43336, 25, -2093.23, 1881.99, 53.77, 0, 'Stagecoach Harness'),
(43336, 26, -2103.06, 1870.42, 46.52, 0, 'Stagecoach Harness'),
(43336, 27, -2122.61, 1831.95, 29.18, 0, 'Stagecoach Harness'),
(43336, 28, -2146.32, 1814.97, 19.03, 0, 'Stagecoach Harness'),
(43336, 29, -2186.33, 1808.11, 12.11, 0, 'Stagecoach Harness'),
(43336, 30, -2217.77, 1809.60, 11.78, 0, 'Stagecoach Harness'),
(43336, 31, -2239.38, 1805.10, 11.94, 0, 'Stagecoach Harness'),
(43336, 32, -2310.30, 1774.33, 11.05, 0, 'Stagecoach Harness'),
(43336, 33, -2376.50, 1704.52, 11.15, 0, 'Stagecoach Harness');

DELETE FROM `waypoints` WHERE `entry` = 38765;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(38765, 1, -1870.36, 2282.73, 42.32, 'Stout Mountain Horse'),
(38765, 2, -1852.05, 2302.51, 41.83, 'Stout Mountain Horse'),
(38765, 3, -1829.29, 2323.74, 37.55, 'Stout Mountain Horse'),
(38765, 4, -1818.32, 2332.80, 36.37, 'Stout Mountain Horse'),
(38765, 5, -1801.58, 2344.71, 35.77, 'Stout Mountain Horse'),
(38765, 6, -1792.48, 2352.44, 36.52, 'Stout Mountain Horse'),
(38765, 7, -1787.74, 2359.60, 38.00, 'Stout Mountain Horse'),
(38765, 8, -1781.81, 2383.18, 43.27, 'Stout Mountain Horse'),
(38765, 9, -1777.55, 2423.56, 55.68, 'Stout Mountain Horse'),
(38765, 10, -1770.01, 2447.47, 62.25, 'Stout Mountain Horse'),
(38765, 11, -1754.47, 2464.13, 69.36, 'Stout Mountain Horse'),
(38765, 12, -1710.82, 2466.15, 82.81, 'Stout Mountain Horse'),
(38765, 13, -1691.46, 2493.66, 95.20, 'Stout Mountain Horse'),
(38765, 14, -1682.62, 2507.22, 97.79, 'Stout Mountain Horse'),
(38765, 15, -1669.61, 2519.56, 97.89, 'Stout Mountain Horse');

DELETE FROM `creature_template_addon` WHERE `entry` IN (35753, 50260);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(35753, 0, 0, 0, 1, 0, '78037'),
(50260, 0, 0, 50331649, 1, 0, NULL);

DELETE FROM `creature_text` WHERE `entry` IN (34850, 34864, 34884, 35112, 35115, 35118, 35369, 35378, 35550, 35753, 35905, 35907, 36287, 36288, 36289, 36458, 36461, 51409);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34850, 0, 0, 'Stand ready, guards!  We don''t know how many intruders we''re dealing with, but the Headlands are overrun and we''re cut off from the harbor towns.  Expect to be outnumbered.', 12, 0, 100, 1, 0, 19614, 'Prince Liam Greymane - Say 0'),
(34850, 1, 0, 'I want the perimeter secured and the gates manned by two guards at all times.  No one gets in, no one gets out.', 12, 0, 100, 1, 0, 19615, 'Prince Liam Greymane - Say 1'),
(34850, 2, 0, 'We protected Gilneas from the Scourge.  We protected Gilneas during the Northgate rebellion.  We will protect Gilneas from whatever this new threat may be.', 12, 0, 100, 25, 0, 19616, 'Prince Liam Greymane - Say 2'),
(34864, 0, 0, 'No one''s allowed to leave. Prince Liam''s orders.', 12, 0, 100, 1, 0, 0, 'Gilneas City Guard'),
(34864, 0, 1, 'It isn''t safe to leave, there''s something lurking out there in the woods.', 12, 0, 100, 1, 0, 0, 'Gilneas City Guard'),
(34864, 0, 2, 'Stay calm. We have everything under control.', 12, 0, 100, 25, 0, 0, 'Gilneas City Guard'),
(34884, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Rampaging Worgen'),
(35112, 0, 0, 'Enough!  I''ve made up my mind.  We need Crowley on our side.', 12, 0, 100, 0, 0, 0, 'King Genn Greymane to Player'),
(35115, 0, 0, 'We ought to leave Crowley in prison with those beasts!', 12, 0, 100, 0, 0, 0, 'Lord Godfrey to Player'),
(35118, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Bloodfang Worgen'),
(35369, 0, 0, 'What''s wrong with me??!', 14, 0, 100, 0, 0, 0, 'Josiah Avery'),
(35369, 1, 0, 'My face!  What''s wrong with my face!', 14, 0, 100, 0, 0, 0, 'Josiah Avery'),
(35369, 2, 0, 'My hands... don''t look at my hands!', 14, 0, 100, 0, 0, 0, 'Josiah Avery'),
(35369, 3, 0, 'The pain is unbearable!', 14, 0, 100, 0, 0, 0, 'Josiah Avery'),
(35369, 4, 0, 'Make it stop!', 14, 0, 100, 0, 0, 0, 'Josiah Avery'),
(35369, 5, 0, 'I can''t fight it!', 14, 0, 100, 0, 0, 0, 'Josiah Avery'),
(35378, 0, 0, 'This mastiff will help you find the hidden worgen.', 12, 0, 100, 0, 0, 19696, 'Lorna Crowley to Player'),
(35550, 0, 0, 'Keep them back!', 14, 0, 100, 0, 0, 19712, 'King Genn Greymane'),
(35550, 0, 1, 'Fire!', 14, 0, 100, 0, 0, 19710, 'King Genn Greymane'),
(35550, 0, 2, 'Hold the barricades!  Do not give them an inch!', 14, 0, 100, 0, 0, 19709, 'King Genn Greymane'),
(35753, 0, 0, 'Help!  Up here!', 14, 0, 100, 0, 0, 0, 'Krennan Aranas to Player'),
(35905, 0, 0, 'Rescue Krennan Aranas by using your vehicle''s ability.$B|TInterface\\Icons\\inv_misc_groupneedmore.blp:64|t', 42, 0, 100, 0, 0, 0, 'King Greymane''s Horse to Player'),
(35907, 0, 0, 'Thank you!  I owe you my life.', 12, 0, 100, 0, 0, 20922, 'Krennan Aranas to King Greymane''s Horse'),
(36287, 0, 0, 'You are scary!  I just want my mommy!', 12, 0, 100, 0, 0, 0, 'Cynthia to Player'),
(36288, 0, 0, 'Are you one of the good worgen, $g mister:ma''am;?  Did you see Cynthia hiding in the sheds outside?', 12, 0, 100, 0, 0, 0, 'Ashley to Player'),
(36289, 0, 0, 'Don''t hurt me!  I was just looking for my sisters!  I think Ashley''s inside that house!', 12, 0, 100, 0, 0, 0, 'James to Player'),
(36458, 0, 0, 'You do not mess with my kitty you son of a mongrel!', 14, 0, 100, 0, 0, 0, 'Grandma Wahl to Lucius the Cruel'),
(36461, 0, 0, 'I''ll be taking this cat.  It seems to work as the perfect bait.  Prepare to die now, fool!', 12, 0, 100, 16, 0, 0, 'Lucius the Cruel to Player'),
(51409, 0, 0, 'Look out! We''re under attack!', 14, 0, 100, 0, 0, 0, 'Lorna Crowley to Stagecoach Carriage');
