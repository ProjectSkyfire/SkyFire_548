-- Hour of Twilight (map 940): bind the instance, boss, trash, gameobject and spell scripts, give the summoned creatures their
-- real templates (they were level 1 and friendly placeholders) and open the heroic entrance. Idempotent: absolute UPDATEs,
-- DELETE before INSERT.

UPDATE `instance_template` SET `script`='instance_hour_of_twilight' WHERE `map`=940;
UPDATE `access_requirement` SET `completed_achievement`=0 WHERE `mapId`=940;

UPDATE `creature_template` SET `ScriptName`='npc_frozen_servitor_stalker' WHERE `entry`=53488;
UPDATE `creature_template` SET `ScriptName`='npc_thrall_intro' WHERE `entry`=54548;
UPDATE `creature_template` SET `ScriptName`='npc_frozen_servitor_stalker' WHERE `entry`=54560;
UPDATE `creature_template` SET `ScriptName`='boss_arcurion' WHERE `entry`=54590;
UPDATE `creature_template` SET `ScriptName`='npc_frozen_servitor_stalker' WHERE `entry`=54598;
UPDATE `creature_template` SET `ScriptName`='npc_arcurion_frozen_servitor' WHERE `entry`=54600;
UPDATE `creature_template` SET `ScriptName`='npc_frozen_servitor_stalker' WHERE `entry`=54628;
UPDATE `creature_template` SET `ScriptName`='npc_faceless_minions' WHERE `entry`=54632;
UPDATE `creature_template` SET `ScriptName`='npc_faceless_minions' WHERE `entry`=54633;
UPDATE `creature_template` SET `ScriptName`='npc_thrall_trash_benedictus' WHERE `entry`=54634;
UPDATE `creature_template` SET `ScriptName`='npc_faceless_minions' WHERE `entry`=54646;
UPDATE `creature_template` SET `ScriptName`='npc_faceless_minions' WHERE `entry`=54686;
UPDATE `creature_template` SET `ScriptName`='npc_faceless_minions' WHERE `entry`=54696;
UPDATE `creature_template` SET `ScriptName`='boss_archbishop_benedictus' WHERE `entry`=54938;
UPDATE `creature_template` SET `ScriptName`='boss_asira_dawnslayer' WHERE `entry`=54968;
UPDATE `creature_template` SET `ScriptName`='npc_thrall_benedictus' WHERE `entry`=54971;
UPDATE `creature_template` SET `ScriptName`='npc_thrall_asira' WHERE `entry`=54972;
UPDATE `creature_template` SET `ScriptName`='npc_icy_tomb' WHERE `entry`=54995;
UPDATE `creature_template` SET `ScriptName`='npc_twilights_hammer' WHERE `entry`=55106;
UPDATE `creature_template` SET `ScriptName`='npc_twilights_hammer' WHERE `entry`=55107;
UPDATE `creature_template` SET `ScriptName`='npc_twilights_hammer' WHERE `entry`=55109;
UPDATE `creature_template` SET `ScriptName`='npc_twilights_hammer' WHERE `entry`=55111;
UPDATE `creature_template` SET `ScriptName`='npc_twilights_hammer' WHERE `entry`=55112;
UPDATE `creature_template` SET `ScriptName`='npc_archbishop_benedictus_purifying_light' WHERE `entry`=55377;
UPDATE `creature_template` SET `ScriptName`='npc_frozen_servitor_stalker' WHERE `entry`=55384;
UPDATE `creature_template` SET `ScriptName`='npc_frozen_servitor_stalker' WHERE `entry`=55385;
UPDATE `creature_template` SET `ScriptName`='npc_frozen_servitor_stalker' WHERE `entry`=55389;
UPDATE `creature_template` SET `ScriptName`='npc_hour_of_twilight_life_warden_thrall' WHERE `entry`=55415;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_light_wave' WHERE `entry`=55441;
UPDATE `creature_template` SET `ScriptName`='npc_archbishop_benedictus_purifying_light' WHERE `entry`=55468;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_light_wave' WHERE `entry`=55469;
UPDATE `creature_template` SET `ScriptName`='npc_hour_of_twilight_life_warden' WHERE `entry`=55549;
UPDATE `creature_template` SET `ScriptName`='npc_crystalline_elemental' WHERE `entry`=55559;
UPDATE `creature_template` SET `ScriptName`='npc_hour_of_twilight_twilight_drake' WHERE `entry`=55636;
UPDATE `creature_template` SET `ScriptName`='npc_ice_wall_exit_stalker' WHERE `entry`=55728;
UPDATE `creature_template` SET `ScriptName`='npc_thrall_arcurion' WHERE `entry`=55779;
UPDATE `gameobject_template` SET `ScriptName`='go_hot_time_transit_device' WHERE `entry`=210026;

-- Templates of the summoned creatures and bosses, taken from the 5.4.8 data
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=54522;
UPDATE `creature_template` SET `exp`=3, `maxlevel`=85, `minlevel`=85, `speed_run`=0.992063, `spell1`=104130, `spell2`=103641, `spell3`=102475, `unit_flags2`=2048 WHERE `entry`=54548;
UPDATE `creature_template` SET `exp`=3, `spell1`=105432, `spell2`=105433 WHERE `entry`=54555;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=54560;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=102582, `spell2`=102593, `spell3`=103252, `spell4`=104050, `unit_flags`=32832 WHERE `entry`=54590;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=54598;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=54599;
UPDATE `creature_template` SET `exp`=3, `faction_A`=2102, `faction_H`=2102, `maxlevel`=85, `minlevel`=85, `speed_walk`=1.55556, `spell1`=102198, `spell2`=102480, `unit_class`=2 WHERE `entry`=54600;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128 WHERE `entry`=54628;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=102861, `spell2`=102848, `type`=15, `unit_flags`=64 WHERE `entry`=54632;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=102984, `spell2`=102992, `type`=15, `unit_flags`=0 WHERE `entry`=54633;
UPDATE `creature_template` SET `exp`=3, `spell1`=104088, `unit_flags`=32784, `unit_flags2`=2048 WHERE `entry`=54634;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54644;
UPDATE `creature_template` SET `exp`=3, `faction_A`=16, `faction_H`=16, `maxlevel`=87, `minlevel`=87, `speed_run`=0.714286, `speed_walk`=2, `spell1`=102226, `type`=15, `unit_flags`=16 WHERE `entry`=54646;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54663;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54664;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54665;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54666;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=102995, `type`=15, `unit_flags`=4 WHERE `entry`=54686;
UPDATE `creature_template` SET `exp`=3, `faction_A`=2324, `faction_H`=2324, `maxlevel`=87, `mechanic_immune_mask`=617299967, `minlevel`=87, `speed_run`=1, `speed_walk`=2.8, `VehicleId`=1743 WHERE `entry`=54696;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54735;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=103767, `spell2`=103565, `spell3`=103151, `spell4`=104503 WHERE `entry`=54938;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54955;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=103419, `spell2`=103558, `spell3`=103790, `spell4`=103562, `unit_flags2`=67584 WHERE `entry`=54968;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=2, `spell1`=103637, `spell2`=103550, `spell3`=108442, `spell4`=103688, `unit_flags`=33280, `unit_flags2`=2048 WHERE `entry`=54971;
UPDATE `creature_template` SET `exp`=3, `spell1`=103947, `spell2`=104088, `spell3`=107980, `spell4`=103819 WHERE `entry`=54972;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=85, `minlevel`=85, `speed_run`=1 WHERE `entry`=54995;
UPDATE `creature_template` SET `exp`=3, `spell1`=102924, `spell2`=102967, `spell3`=102925, `spell4`=102926, `unit_flags`=537174080 WHERE `entry`=55106;
UPDATE `creature_template` SET `exp`=3, `spell1`=102975, `spell2`=43415, `spell3`=103023, `spell4`=108443, `unit_flags`=41024 WHERE `entry`=55107;
UPDATE `creature_template` SET `exp`=3, `spell1`=103021, `spell2`=103023, `spell3`=103024, `spell4`=103013, `unit_flags`=4227136 WHERE `entry`=55109;
UPDATE `creature_template` SET `exp`=3, `spell1`=102990, `spell2`=102989, `spell3`=103023, `unit_flags`=41024 WHERE `entry`=55111;
UPDATE `creature_template` SET `exp`=3, `spell1`=103001, `spell2`=103023, `spell3`=103002, `spell4`=103000, `unit_flags`=41024 WHERE `entry`=55112;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `spell1`=103651, `unit_flags`=33554432 WHERE `entry`=55377;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=55384;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=55385;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=55389;
UPDATE `creature_template` SET `exp`=2 WHERE `entry`=55415;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=87, `minlevel`=87, `unit_flags`=33554432 WHERE `entry`=55427;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.42857, `unit_flags`=33554432 WHERE `entry`=55441;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=55445;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=55466;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=87, `minlevel`=87, `spell1`=103777, `unit_flags`=33554432 WHERE `entry`=55467;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `unit_flags`=33554432 WHERE `entry`=55468;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `minlevel`=87, `speed_run`=1.42857, `unit_flags`=33554432 WHERE `entry`=55469;
UPDATE `creature_template` SET `exp`=3, `faction_A`=2141, `faction_H`=2141, `maxlevel`=85, `minlevel`=85, `speed_run`=1, `spell1`=103813, `spell2`=103817, `unit_flags`=131074, `unit_flags2`=524288 WHERE `entry`=55474;
UPDATE `creature_template` SET `exp`=3, `npcflag`=16777217, `unit_flags`=0 WHERE `entry`=55549;
UPDATE `creature_template` SET `exp`=3, `spell1`=104019 WHERE `entry`=55559;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=55563;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `spell1`=104375, `spell2`=104230, `spell3`=104374, `spell4`=104376 WHERE `entry`=55636;
UPDATE `creature_template` SET `exp`=4, `faction_A`=2102, `faction_H`=2102, `mechanic_immune_mask`=617299967, `unit_flags`=131072, `unit_flags2`=0 WHERE `entry`=55728;
UPDATE `creature_template` SET `exp`=4, `speed_run`=0.992063, `spell1`=103834, `spell2`=2645, `spell3`=103921, `spell4`=104540, `unit_flags2`=2048 WHERE `entry`=55779;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `mechanic_immune_mask`=617299967 WHERE `entry`=57197;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `speed_run`=1.14286, `speed_walk`=1, `spell1`=103851, `spell4`=103785 WHERE `entry`=58226;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=58233;
UPDATE `creature_template` SET `exp`=3, `faction_A`=1965, `faction_H`=1965, `maxlevel`=85, `minlevel`=85, `speed_run`=1.42857, `speed_walk`=1.2, `unit_flags2`=33556480 WHERE `entry`=58234;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=108183, `spell2`=108172 WHERE `entry`=58235;

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('boss_archbishop_benedictus','boss_arcurion','boss_asira_dawnslayer','go_hot_time_transit_device','npc_archbishop_benedictus_purifying_light','npc_arcurion_frozen_servitor','npc_crystalline_elemental','npc_faceless_minions','npc_frozen_servitor_stalker','npc_hour_of_twilight_life_warden','npc_hour_of_twilight_life_warden_thrall','npc_hour_of_twilight_twilight_drake','npc_ice_wall_exit_stalker','npc_icy_tomb','npc_thrall_arcurion','npc_thrall_asira','npc_thrall_benedictus','npc_thrall_intro','npc_thrall_trash_benedictus','npc_twilight_light_wave','npc_twilights_hammer','spell_archbishop_benedictus_righteous_snear_aoe','spell_arcurion_icy_boulder','spell_asira_dawnslayer_blade_barrier','spell_asira_dawnslayer_mark_of_silence','spell_asira_dawnslayer_throw_knife','spell_light_wall_cosmetic','spell_hour_of_twilight_molten_fury');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102480, 'spell_arcurion_icy_boulder'),
(102629, 'spell_light_wall_cosmetic'),
(102726, 'spell_asira_dawnslayer_mark_of_silence'),
(103149, 'spell_archbishop_benedictus_righteous_snear_aoe'),
(103362, 'spell_archbishop_benedictus_righteous_snear_aoe'),
(103419, 'spell_asira_dawnslayer_blade_barrier'),
(103562, 'spell_asira_dawnslayer_blade_barrier'),
(103597, 'spell_asira_dawnslayer_throw_knife'),
(103905, 'spell_hour_of_twilight_molten_fury');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=55549;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(55549, 75045, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (102629, 103762);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 102629, 0, 0, 31, 0, 3, 54955, 0, 0, 0, 0, '', 'Benedictus Light wall'),
(13, 1, 103762, 0, 0, 31, 0, 3, 54971, 0, 0, 0, 0, '', 'Archbishop Benedictus - Engulfing Twilight');

DELETE FROM `spell_target_position` WHERE `id` IN (108928, 108929);
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(108928, 0, 940, 4440.64, 454.088, 46.311, 3.25432),
(108929, 0, 940, 3956.93, 284.866, 14.4072, 2.97708);

-- Target Dummy and Cannon are invisible triggers in 5.4.8 (LoA's templates): trigger flag and not selectable.
UPDATE `creature_template` SET `flags_extra`=128, `unit_flags`=33554944 WHERE `entry` IN (56126, 58287);
