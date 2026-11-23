-- End Time (map 938): bind the instance, boss, trash, gameobject and spell scripts and give the summoned creatures their
-- real templates (they were level 1 and friendly placeholders). Idempotent: absolute UPDATEs, DELETE before INSERT.

UPDATE `instance_template` SET `script`='instance_end_time' WHERE `map`=938;

UPDATE `creature_template` SET `ScriptName`='boss_echo_of_sylvanas' WHERE `entry`=54123;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_sylvanas_risen_ghoul' WHERE `entry`=54191;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_sylvanas_ghoul' WHERE `entry`=54197;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_sylvanas_blighted_arrows' WHERE `entry`=54403;
UPDATE `creature_template` SET `ScriptName`='boss_echo_of_baine' WHERE `entry`=54431;
UPDATE `creature_template` SET `ScriptName`='boss_murozond' WHERE `entry`=54432;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_baine_baines_totem' WHERE `entry`=54434;
UPDATE `creature_template` SET `ScriptName`='npc_murozond_mirror_image' WHERE `entry`=54435;
UPDATE `creature_template` SET `ScriptName`='boss_echo_of_jaina' WHERE `entry`=54445;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_jaina_flarecore' WHERE `entry`=54446;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_pool_of_moonlight' WHERE `entry`=54508;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_jaina_blink_target' WHERE `entry`=54542;
UPDATE `creature_template` SET `ScriptName`='boss_echo_of_tyrande' WHERE `entry`=54544;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_moonlance' WHERE `entry`=54574;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_moonlance' WHERE `entry`=54580;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_moonlance' WHERE `entry`=54581;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_moonlance' WHERE `entry`=54582;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_eye_of_elune' WHERE `entry`=54594;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_eye_of_elune' WHERE `entry`=54597;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_time_twisted_footman' WHERE `entry`=54687;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_time_twisted_priest' WHERE `entry`=54690;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_time_twisted_sorceress' WHERE `entry`=54691;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_time_twisted_rifleman' WHERE `entry`=54693;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_nozdormu' WHERE `entry`=54751;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_image_of_nozdormu' WHERE `entry`=54867;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_infinite_suppressor' WHERE `entry`=54920;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_infinite_warden' WHERE `entry`=54923;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_eye_of_elune' WHERE `entry`=54939;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_eye_of_elune' WHERE `entry`=54940;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_eye_of_elune' WHERE `entry`=54941;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_eye_of_elune' WHERE `entry`=54942;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (54507, 54511, 54543, 54552, 54553) AND `ScriptName`='';

UPDATE `gameobject_template` SET `ScriptName`='go_murozond_hourglass_of_time' WHERE `entry`=209249;
UPDATE `gameobject_template` SET `ScriptName`='go_echo_of_jaina_jaina_staff_fragment' WHERE `entry`=209318;
UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry`=209437;
UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry`=209438;
UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry`=209439;
UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry`=209440;
UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry`=209441;
UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry`=209442;
UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry`=209443;
UPDATE `gameobject_template` SET `data1`=209547 WHERE `entry`=209547;

-- Templates of the summoned creatures and bosses, taken from the 5.4.8 data
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967, `spell1`=101404, `spell2`=101401, `spell3`=100686, `spell4`=101348 WHERE `entry`=54123;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=85, `minlevel`=85, `speed_run`=0.5, `speed_walk`=0.5 WHERE `entry`=54191;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=85, `minlevel`=85, `speed_run`=0.214286, `speed_walk`=0.6, `spell1`=101318, `unit_flags`=33554432 WHERE `entry`=54197;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=87, `minlevel`=87, `speed_run`=0.785714, `speed_walk`=2.2, `unit_flags`=33587264, `unit_flags2`=34816, `VehicleId`=1730 WHERE `entry`=54385;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=85, `minlevel`=85, `speed_run`=0.785714, `speed_walk`=2.2, `unit_flags`=33554496 WHERE `entry`=54403;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967, `spell1`=101840, `spell2`=101627, `spell3`=101625 WHERE `entry`=54431;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967, `spell1`=101984, `spell2`=107550, `spell3`=102569, `spell4`=108589 WHERE `entry`=54432;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967, `type`=10 WHERE `entry`=54433;
UPDATE `creature_template` SET `exp`=3, `maxlevel`=87, `mechanic_immune_mask`=617299967, `minlevel`=87, `npcflag`=16777216, `speed_run`=1.42857, `speed_walk`=2.4, `type`=10, `unit_flags`=32832, `VehicleId`=1733 WHERE `entry`=54434;
UPDATE `creature_template` SET `exp`=3, `faction_A`=31, `faction_H`=31, `maxlevel`=85, `minlevel`=85, `spell1`=57723, `spell2`=82926, `spell3`=95809, `spell4`=24907, `unit_class`=8, `unit_flags`=256, `unit_flags2`=2064 WHERE `entry`=54435;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967, `spell1`=101812, `spell2`=101927, `spell3`=101339, `spell4`=101810, `unit_flags`=32832 WHERE `entry`=54445;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `mechanic_immune_mask`=617299967, `minlevel`=87, `unit_flags`=33554432, `unit_flags2`=34816 WHERE `entry`=54446;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=54476;
UPDATE `creature_template` SET `exp`=0 WHERE `entry`=54486;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `mechanic_immune_mask`=617299967, `minlevel`=87, `spell1`=101337, `unit_flags`=33554432 WHERE `entry`=54494;
UPDATE `creature_template` SET `flags_extra`=0, `mechanic_immune_mask`=617299967 WHERE `entry`=54496;
UPDATE `creature_template` SET `spell1`=101888, `spell2`=102063, `spell3`=101891, `unit_flags`=32768 WHERE `entry`=54507;
UPDATE `creature_template` SET `flags_extra`=128, `spell1`=101842 WHERE `entry`=54508;
UPDATE `creature_template` SET `speed_run`=0.992063, `spell1`=109952, `spell2`=109944, `spell3`=109945, `spell4`=102066 WHERE `entry`=54511;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=85, `minlevel`=85, `unit_flags`=33554432, `unit_flags2`=34816, `VehicleId`=1739 WHERE `entry`=54521;
UPDATE `creature_template` SET `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=54542;
UPDATE `creature_template` SET `spell1`=102134, `spell2`=102135 WHERE `entry`=54543;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967, `speed_run`=0.992063, `spell1`=102181, `spell2`=102606, `spell3`=102605, `spell4`=102472, `unit_flags`=32832 WHERE `entry`=54544;
UPDATE `creature_template` SET `spell1`=102057, `spell2`=105238, `unit_flags`=33555206 WHERE `entry`=54550;
UPDATE `creature_template` SET `spell1`=102132, `spell2`=102124 WHERE `entry`=54552;
UPDATE `creature_template` SET `spell1`=102156, `spell2`=102158 WHERE `entry`=54553;
UPDATE `creature_template` SET `exp`=3, `faction_A`=103, `faction_H`=103, `maxlevel`=85, `minlevel`=85, `speed_run`=0.285714, `speed_walk`=0.8, `unit_flags`=33587200 WHERE `entry`=54566;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102149, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54574;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102149, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54580;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102149, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54581;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102149, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54582;
UPDATE `creature_template` SET `exp`=3, `faction_A`=2035, `faction_H`=2035, `maxlevel`=85, `minlevel`=85, `speed_run`=0.285714, `speed_walk`=0.8, `unit_flags`=33587200 WHERE `entry`=54585;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102183, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54594;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102183, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54597;
UPDATE `creature_template` SET `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=54639;
UPDATE `creature_template` SET `exp`=3, `faction_A`=16, `faction_H`=16, `maxlevel`=87, `mechanic_immune_mask`=617299967, `minlevel`=87, `speed_run`=2.14286, `unit_class`=2, `unit_flags`=33587968 WHERE `entry`=54641;
UPDATE `creature_template` SET `spell1`=101817, `spell2`=101811, `spell3`=101820 WHERE `entry`=54687;
UPDATE `creature_template` SET `spell1`=102405, `spell2`=102409 WHERE `entry`=54690;
UPDATE `creature_template` SET `spell1`=102442, `spell2`=101816, `spell3`=101812 WHERE `entry`=54691;
UPDATE `creature_template` SET `spell1`=102411, `spell2`=102410 WHERE `entry`=54693;
UPDATE `creature_template` SET `unit_flags`=40960 WHERE `entry`=54699;
UPDATE `creature_template` SET `unit_flags`=40960 WHERE `entry`=54700;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=54701;
UPDATE `creature_template` SET `flags_extra`=0, `mechanic_immune_mask`=617299967 WHERE `entry`=54751;
UPDATE `creature_template` SET `exp`=3, `faction_A`=16, `faction_H`=16, `maxlevel`=85, `minlevel`=85, `speed_run`=0.992063, `unit_flags`=524288, `unit_flags2`=32 WHERE `entry`=54795;
UPDATE `creature_template` SET `flags_extra`=0, `mechanic_immune_mask`=617299967, `unit_flags`=33024 WHERE `entry`=54867;
UPDATE `creature_template` SET `spell1`=102601, `spell2`=102600 WHERE `entry`=54920;
UPDATE `creature_template` SET `spell1`=102599, `spell2`=102598 WHERE `entry`=54923;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102183, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54939;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102183, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54940;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102183, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54941;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.71429, `speed_walk`=2, `spell1`=102183, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=54942;
UPDATE `creature_template` SET `flags_extra`=0, `mechanic_immune_mask`=617299967 WHERE `entry`=54945;
UPDATE `creature_template` SET `exp`=3, `faction_A`=16, `faction_H`=16, `maxlevel`=85, `minlevel`=85, `speed_run`=1, `speed_walk`=1.2, `unit_flags`=32768 WHERE `entry`=54952;

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('boss_echo_of_baine','boss_echo_of_jaina','boss_echo_of_sylvanas','boss_echo_of_tyrande','boss_murozond','go_echo_of_jaina_jaina_staff_fragment','go_end_time_teleport','go_murozond_hourglass_of_time','npc_echo_of_baine_baines_totem','npc_echo_of_jaina_blink_target','npc_echo_of_jaina_flarecore','npc_echo_of_sylvanas_blighted_arrows','npc_echo_of_sylvanas_ghoul','npc_echo_of_sylvanas_risen_ghoul','npc_echo_of_tyrande_eye_of_elune','npc_echo_of_tyrande_moonlance','npc_echo_of_tyrande_pool_of_moonlight','npc_end_time_image_of_nozdormu','npc_end_time_infinite_suppressor','npc_end_time_infinite_warden','npc_end_time_nozdormu','npc_end_time_time_twisted_footman','npc_end_time_time_twisted_priest','npc_end_time_time_twisted_rifleman','npc_end_time_time_twisted_sorceress','npc_murozond_mirror_image','spell_echo_of_baine_pulverize_aoe','spell_echo_of_jaina_frost_blades','spell_echo_of_sylvanas_death_grip_aoe','spell_echo_of_sylvanas_seeping_shadows','spell_echo_of_sylvanas_wracking_pain_dmg','spell_echo_of_tyrande_eyes_of_the_goddess','spell_echo_of_tyrande_tears_of_elune_script','spell_end_time_blink');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101257, 'spell_echo_of_sylvanas_wracking_pain_dmg'),
(101337, 'spell_echo_of_jaina_frost_blades'),
(101397, 'spell_echo_of_sylvanas_death_grip_aoe'),
(101625, 'spell_echo_of_baine_pulverize_aoe'),
(101812, 'spell_end_time_blink'),
(102242, 'spell_echo_of_tyrande_tears_of_elune_script'),
(102608, 'spell_echo_of_tyrande_eyes_of_the_goddess'),
(103175, 'spell_echo_of_sylvanas_seeping_shadows');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=54434;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(54434, 107837, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (101401, 102183);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 101401, 0, 0, 31, 0, 3, 54403, 0, 0, 0, 0, '', 'Blighted Arrows targets Blighted Arrows'),
(13, 3, 102183, 0, 0, 31, 0, 4, 0, 0, 0, 0, 0, '', 'Piercing Gaze of Elune targets players');

DELETE FROM `areatrigger_scripts` WHERE `entry`=6955;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(6955, 'at_et_tyrande');

DELETE FROM `spell_target_position` WHERE `id` IN (102564, 102579, 104761, 102126, 103868, 104764);
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(102126, 0, 938, 2997.37, 570.222, 25.3074, 5.54309),
(102564, 0, 938, 3711.59, -375.389, 113.064, 2.23589),
(102579, 0, 938, 3833.33, 1101.35, 83.413, 3.78613),
(103868, 0, 938, 4343.33, 1294.59, 147.504, 0.765022),
(104761, 0, 938, 2959.33, 64.8018, 11.46, 2.95),
(104764, 0, 938, 4042.71, -351.774, 122.216, 4.32825);
