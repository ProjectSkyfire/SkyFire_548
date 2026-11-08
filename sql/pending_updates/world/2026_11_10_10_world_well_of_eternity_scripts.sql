-- Well of Eternity (map 939): bind the instance, boss, trash, gameobject and spell scripts, give the summoned creatures their
-- real templates (they were level 1 and friendly placeholders) and open the heroic entrance. Idempotent: absolute UPDATEs,
-- DELETE before INSERT.

UPDATE `instance_template` SET `script`='instance_well_of_eternity' WHERE `map`=939;
UPDATE `access_requirement` SET `completed_achievement`=0 WHERE `mapId`=939;

UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_legion_demon_preevent' WHERE `entry`=54500;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_stalker' WHERE `entry`=54506;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_legion_portal' WHERE `entry`=54513;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_enchanted_highmistress' WHERE `entry`=54589;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_eternal_champion' WHERE `entry`=54612;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_royal_handmaiden' WHERE `entry`=54645;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_eye_of_legion' WHERE `entry`=54747;
UPDATE `creature_template` SET `ScriptName`='boss_queen_azshara' WHERE `entry`=54853;
UPDATE `creature_template` SET `ScriptName`='npc_queen_azshara_hammer_of_divinity' WHERE `entry`=54864;
UPDATE `creature_template` SET `ScriptName`='npc_queen_azshara_enchanted_magus' WHERE `entry`=54882;
UPDATE `creature_template` SET `ScriptName`='npc_queen_azshara_enchanted_magus' WHERE `entry`=54883;
UPDATE `creature_template` SET `ScriptName`='npc_queen_azshara_enchanted_magus' WHERE `entry`=54884;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_guardian_demon' WHERE `entry`=54927;
UPDATE `creature_template` SET `ScriptName`='boss_mannoroth' WHERE `entry`=54969;
UPDATE `creature_template` SET `ScriptName`='boss_perotharn' WHERE `entry`=55085;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_shadowcloak_stalker' WHERE `entry`=55154;
UPDATE `creature_template` SET `ScriptName`='npc_mannoroth_varothen' WHERE `entry`=55419;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_illidan_1' WHERE `entry`=55500;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_legion_demon' WHERE `entry`=55503;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_abyssal_doombringer' WHERE `entry`=55510;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_doomguard_annihilator' WHERE `entry`=55519;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_tyrande' WHERE `entry`=55524;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_illidan_2' WHERE `entry`=55532;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_portal_connector' WHERE `entry`=55541;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_portal_connector' WHERE `entry`=55542;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_portal_connector' WHERE `entry`=55543;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_malfurion' WHERE `entry`=55570;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_nozdormu' WHERE `entry`=55624;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_portal_trash' WHERE `entry`=55654;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_portal_trash' WHERE `entry`=55656;
UPDATE `creature_template` SET `ScriptName`='npc_mannoroth_doomguard_debilitator' WHERE `entry`=55762;
UPDATE `creature_template` SET `ScriptName`='npc_perotharn_eye_of_perotharn' WHERE `entry`=55868;
UPDATE `creature_template` SET `ScriptName`='npc_perotharn_eye_of_perotharn' WHERE `entry`=55879;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_fel_crystal_stalker' WHERE `entry`=55965;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_portal_to_twisting_nether' WHERE `entry`=56087;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_fire_wall_stalker' WHERE `entry`=56096;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_enchanted_highmistress' WHERE `entry`=56579;
UPDATE `gameobject_template` SET `ScriptName`='go_npc_well_of_eternity_portal_energy_focus' WHERE `entry`=209366;
UPDATE `gameobject_template` SET `ScriptName`='go_npc_well_of_eternity_portal_energy_focus' WHERE `entry`=209447;
UPDATE `gameobject_template` SET `ScriptName`='go_npc_well_of_eternity_portal_energy_focus' WHERE `entry`=209448;
UPDATE `gameobject_template` SET `ScriptName`='go_well_of_eternity_teleport' WHERE `entry`=209997;
UPDATE `gameobject_template` SET `ScriptName`='go_well_of_eternity_teleport' WHERE `entry`=209998;
UPDATE `gameobject_template` SET `ScriptName`='go_well_of_eternity_teleport' WHERE `entry`=209999;
UPDATE `gameobject_template` SET `ScriptName`='go_well_of_eternity_teleport' WHERE `entry`=210000;

-- Templates of the summoned creatures and bosses, taken from the 5.4.8 data
UPDATE `creature_template` SET `exp`=3, `flags_extra`=64, `mechanic_immune_mask`=617299967 WHERE `entry`=54500;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54506;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54513;
UPDATE `creature_template` SET `exp`=3, `spell1`=102267, `spell2`=102266 WHERE `entry`=54589;
UPDATE `creature_template` SET `exp`=3, `spell1`=102260, `spell2`=102257, `unit_flags`=41024 WHERE `entry`=54612;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `spell1`=102233, `spell2`=102238, `spell3`=102245 WHERE `entry`=54645;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=54728;
UPDATE `creature_template` SET `exp`=3, `speed_run`=0.992063, `spell1`=102356, `unit_flags`=40960 WHERE `entry`=54747;
UPDATE `creature_template` SET `exp`=3, `spell1`=102915, `spell2`=103241 WHERE `entry`=54853;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=85, `minlevel`=85, `speed_run`=0.142857, `speed_walk`=0.4, `unit_flags`=33587264 WHERE `entry`=54864;
UPDATE `creature_template` SET `exp`=3, `maxlevel`=85, `minlevel`=85, `speed_run`=0.785714, `speed_walk`=2.2, `unit_flags`=33587264 WHERE `entry`=54865;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=1, `mechanic_immune_mask`=655889, `spell1`=102483, `spell2`=102265, `spell3`=102482 WHERE `entry`=54882;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=1, `mechanic_immune_mask`=655889, `spell1`=102467, `spell2`=102465, `spell3`=102466, `spell4`=102478 WHERE `entry`=54883;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=1, `mechanic_immune_mask`=655889, `spell1`=102463 WHERE `entry`=54884;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=102561, `spell2`=102573 WHERE `entry`=54927;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=104961, `spell2`=105093, `spell3`=103888, `spell4`=103966 WHERE `entry`=54969;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=55078;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=104939, `spell2`=104905, `spell3`=105521, `spell4`=105545, `unit_flags`=32768, `unit_flags2`=2048 WHERE `entry`=55085;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=16384 WHERE `entry`=55154;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=55366;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=55390;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=55391;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=55393;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=55394;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=55395;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=88, `mechanic_immune_mask`=617299967, `minlevel`=88, `speed_run`=1, `speed_walk`=1.66667, `unit_flags`=320, `VehicleId`=1946 WHERE `entry`=55400;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=103669 WHERE `entry`=55419;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `unit_flags`=64 WHERE `entry`=55426;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=103763, `unit_flags`=32832 WHERE `entry`=55453;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=55465;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=16384, `npcflag`=1, `spell1`=105543, `spell2`=104738, `spell3`=105547, `spell4`=105635, `unit_flags`=32768 WHERE `entry`=55500;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=85, `minlevel`=85, `speed_run`=0.714286, `speed_walk`=2, `unit_flags`=33554432 WHERE `entry`=55502;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=108479, `spell2`=103913, `spell3`=105243, `unit_flags`=64 WHERE `entry`=55503;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=103992 WHERE `entry`=55510;
UPDATE `creature_template` SET `exp`=3, `unit_flags`=262144 WHERE `entry`=55519;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `speed_run`=0.992063, `spell1`=103917, `spell2`=104313, `spell3`=104214, `spell4`=104688 WHERE `entry`=55524;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `spell1`=104379, `spell2`=104394, `spell3`=104205, `spell4`=105009, `unit_flags`=32848 WHERE `entry`=55532;
UPDATE `creature_template` SET `mechanic_immune_mask`=16384 WHERE `entry`=55541;
UPDATE `creature_template` SET `mechanic_immune_mask`=16384 WHERE `entry`=55542;
UPDATE `creature_template` SET `mechanic_immune_mask`=16384 WHERE `entry`=55543;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=55570;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=55624;
UPDATE `creature_template` SET `exp`=3, `spell1`=107865, `spell2`=107867, `spell3`=107880, `unit_flags`=0 WHERE `entry`=55654;
UPDATE `creature_template` SET `exp`=3, `spell1`=107840, `spell2`=107900, `unit_flags`=0 WHERE `entry`=55656;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=55700;
UPDATE `creature_template` SET `exp`=3, `unit_flags`=16 WHERE `entry`=55739;
UPDATE `creature_template` SET `exp`=3, `spell1`=104678, `unit_flags2`=2048 WHERE `entry`=55762;
UPDATE `creature_template` SET `exp`=3, `maxlevel`=85, `minlevel`=85, `npcflag`=16777216, `speed_run`=0.992063, `unit_flags`=512, `unit_flags2`=67110912 WHERE `entry`=55837;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `speed_run`=0.992063, `unit_flags2`=2048 WHERE `entry`=55838;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128 WHERE `entry`=55839;
UPDATE `creature_template` SET `maxlevel`=82, `minlevel`=82, `speed_walk`=3.2, `spell1`=105538, `unit_flags`=33554432 WHERE `entry`=55868;
UPDATE `creature_template` SET `maxlevel`=82, `minlevel`=82, `speed_walk`=3.2, `spell1`=105538, `unit_flags`=33554432 WHERE `entry`=55879;
UPDATE `creature_template` SET `exp`=3, `faction_A`=90, `faction_H`=90, `maxlevel`=85, `minlevel`=85, `unit_class`=2 WHERE `entry`=56001;
UPDATE `creature_template` SET `exp`=3, `faction_A`=90, `faction_H`=90, `maxlevel`=85, `minlevel`=85, `unit_class`=2 WHERE `entry`=56002;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `maxlevel`=86, `mechanic_immune_mask`=617299967, `minlevel`=86, `unit_flags2`=134219776 WHERE `entry`=56036;
UPDATE `creature_template` SET `exp`=3, `unit_flags`=0 WHERE `entry`=56073;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=56078;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=56087;
UPDATE `creature_template` SET `speed_run`=1, `unit_flags`=33555200 WHERE `entry`=56182;
UPDATE `creature_template` SET `spell1`=105399 WHERE `entry`=56189;
UPDATE `creature_template` SET `exp`=3, `faction_A`=14, `faction_H`=14, `flags_extra`=128, `maxlevel`=87, `mechanic_immune_mask`=16384, `minlevel`=87 WHERE `entry`=56248;
UPDATE `creature_template` SET `exp`=3, `maxlevel`=85, `minlevel`=85, `speed_run`=0.5, `speed_walk`=1.4, `unit_flags`=33555200 WHERE `entry`=56308;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=16384 WHERE `entry`=56389;
UPDATE `creature_template` SET `exp`=3, `spell1`=102265, `spell2`=102263 WHERE `entry`=56579;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967, `npcflag`=16777217 WHERE `entry`=57107;
UPDATE `creature_template` SET `unit_flags`=33554432 WHERE `entry`=57329;
UPDATE `creature_template` SET `exp`=3, `faction_A`=90, `faction_H`=90, `maxlevel`=85, `minlevel`=85, `speed_run`=1.38889 WHERE `entry`=57410;
UPDATE `creature_template` SET `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=57458;
UPDATE `creature_template` SET `exp`=3, `flags_extra`=128, `maxlevel`=85, `minlevel`=85, `unit_flags`=33554432 WHERE `entry`=57631;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=57913;
UPDATE `creature_template` SET `speed_run`=1, `unit_flags`=33554432, `unit_flags2`=33556480 WHERE `entry`=58418;

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('at_well_of_eternity_illidan_outro_resume','at_well_of_eternity_intro','at_well_of_eternity_perotharn_preevent_appear','at_well_of_eternity_skip_illidan_intro','boss_mannoroth','boss_perotharn','boss_queen_azshara','go_npc_well_of_eternity_portal_energy_focus','go_well_of_eternity_teleport','npc_mannoroth_doomguard_debilitator','npc_mannoroth_varothen','npc_perotharn_eye_of_perotharn','npc_queen_azshara_enchanted_magus','npc_queen_azshara_hammer_of_divinity','npc_well_of_eternity_abyssal_doombringer','npc_well_of_eternity_doomguard_annihilator','npc_well_of_eternity_enchanted_highmistress','npc_well_of_eternity_eternal_champion','npc_well_of_eternity_eye_of_legion','npc_well_of_eternity_fel_crystal_stalker','npc_well_of_eternity_fire_wall_stalker','npc_well_of_eternity_guardian_demon','npc_well_of_eternity_illidan_1','npc_well_of_eternity_illidan_2','npc_well_of_eternity_legion_demon','npc_well_of_eternity_legion_demon_preevent','npc_well_of_eternity_legion_portal','npc_well_of_eternity_malfurion','npc_well_of_eternity_nozdormu','npc_well_of_eternity_portal_connector','npc_well_of_eternity_portal_to_twisting_nether','npc_well_of_eternity_portal_trash','npc_well_of_eternity_royal_handmaiden','npc_well_of_eternity_shadowcloak_stalker','npc_well_of_eternity_stalker','npc_well_of_eternity_tyrande','spell_mannoroth_gift_of_sargeras','spell_perotharn_drain_essence','spell_perotharn_drain_essence_dmg','spell_perotharn_punishing_flames_dmg','spell_queen_azshara_arcane_bomb','spell_queen_azshara_coldflame','spell_well_of_eternity_fel_crystal_arc','spell_well_of_eternity_shadow_walk','spell_well_of_eternity_shadowcloak_illidan');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102465, 'spell_queen_azshara_coldflame'),
(102994, 'spell_well_of_eternity_shadow_walk'),
(104905, 'spell_perotharn_drain_essence'),
(104906, 'spell_perotharn_drain_essence_dmg'),
(104998, 'spell_mannoroth_gift_of_sargeras'),
(105018, 'spell_well_of_eternity_fel_crystal_arc'),
(105915, 'spell_well_of_eternity_shadowcloak_illidan'),
(107536, 'spell_perotharn_punishing_flames_dmg');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (55837, 57107);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(55837, 90102, 1, 0),
(57107, 46598, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (103530, 104625, 104678, 104683, 104688, 104961, 105041, 105073, 105074, 105075, 105509, 105543, 105548, 107900, 110121);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 103530, 0, 1, 31, 0, 3, 54927, 0, 0, 0, 0, '', 'Shadowcloak Aggro Check, Stealth targets Guardian Demon'),
(13, 1, 103530, 0, 2, 31, 0, 3, 55656, 0, 0, 0, 0, '', 'Shadowcloak Aggro Check, Stealth targets Dreadlord Defender'),
(13, 1, 103530, 0, 3, 31, 0, 3, 55654, 0, 0, 0, 0, '', 'Shadowcloak Aggro Check, Stealth targets Corrupted Arcanist'),
(13, 1, 103530, 0, 4, 31, 0, 3, 55503, 0, 0, 0, 0, '', 'Shadowcloak Aggro Check, Stealth targets Legion Demon'),
(13, 1, 103530, 0, 5, 31, 0, 3, 55085, 0, 0, 0, 0, '', 'Shadowcloak Aggro Check, Stealth targets Peroth''arn'),
(13, 1, 103530, 0, 0, 31, 0, 3, 54500, 0, 0, 0, 0, '', 'Shadowcloak Aggro Check, Stealth targets Legion Demon'),
(13, 1, 104625, 0, 0, 31, 0, 3, 45979, 0, 0, 0, 0, '', 'Mannoroth - Nether Portal'),
(13, 1, 104678, 0, 0, 31, 0, 3, 55524, 0, 0, 0, 0, '', 'Mannoroth - Debilitating Flay'),
(13, 7, 104683, 0, 0, 31, 0, 3, 55500, 0, 0, 0, 0, '', 'Aggro Illidan targets Illidan'),
(13, 1, 104688, 0, 2, 31, 0, 3, 56001, 0, 0, 0, 0, '', 'Mannoroth - Lunar Shot'),
(13, 1, 104688, 0, 0, 31, 0, 3, 56036, 0, 0, 0, 0, '', 'Mannoroth - Lunar Shot'),
(13, 1, 104688, 0, 3, 31, 0, 3, 57410, 0, 0, 0, 0, '', 'Mannoroth - Lunar Shot'),
(13, 1, 104688, 0, 1, 31, 0, 3, 56002, 0, 0, 0, 0, '', 'Mannoroth - Lunar Shot'),
(13, 1, 104961, 0, 0, 31, 0, 3, 55419, 0, 0, 0, 0, '', 'Mannoroth - Fel Drain'),
(13, 1, 105041, 0, 0, 31, 0, 3, 54020, 0, 0, 0, 0, '', 'Mannoroth - Nether Tear'),
(13, 1, 105073, 0, 0, 31, 0, 3, 56036, 0, 0, 0, 0, '', 'Mannoroth - Wrath of Elune'),
(13, 1, 105073, 0, 1, 31, 0, 3, 56002, 0, 0, 0, 0, '', 'Mannoroth - Wrath of Elune'),
(13, 1, 105073, 0, 2, 31, 0, 3, 56001, 0, 0, 0, 0, '', 'Mannoroth - Wrath of Elune'),
(13, 1, 105073, 0, 3, 31, 0, 3, 57410, 0, 0, 0, 0, '', 'Mannoroth - Wrath of Elune'),
(13, 1, 105074, 0, 0, 31, 0, 3, 55917, 0, 0, 0, 0, '', 'Fel Crystal Arc Meltdown targets Fel Crystal'),
(13, 1, 105075, 0, 0, 31, 0, 3, 56036, 0, 0, 0, 0, '', 'Mannoroth - Wrath of Elune'),
(13, 1, 105075, 0, 1, 31, 0, 3, 56002, 0, 0, 0, 0, '', 'Mannoroth - Wrath of Elune'),
(13, 1, 105075, 0, 2, 31, 0, 3, 56001, 0, 0, 0, 0, '', 'Mannoroth - Wrath of Elune'),
(13, 1, 105075, 0, 3, 31, 0, 3, 57410, 0, 0, 0, 0, '', 'Mannoroth - Wrath of Elune'),
(13, 3, 105509, 0, 0, 31, 0, 3, 55085, 0, 0, 0, 0, '', 'Perotharn - Attack Me Perotharn'),
(13, 3, 105543, 0, 0, 31, 0, 3, 55085, 0, 0, 0, 0, '', 'Illidan (WoE) - Absorb Fel Energy'),
(13, 1, 105548, 0, 0, 31, 0, 3, 55500, 0, 0, 0, 0, '', 'End Exhausted targets Illidan'),
(13, 1, 107900, 0, 0, 31, 0, 3, 55654, 0, 0, 0, 0, '', 'Demonic Warding targets Corrupted Arcanist'),
(13, 1, 110121, 0, 0, 31, 0, 3, 54506, 0, 0, 0, 0, '', 'Distract Demon Missile targets Wall of Eternity Stalker');

DELETE FROM `spell_target_position` WHERE `id` IN (105054, 105058, 105059, 107691, 107934, 107979);
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(105054, 0, 939, 3338.7, -5699.78, 13.01, 3.87),
(105058, 0, 939, 3338.7, -5699.78, 13.01, 3.87),
(105059, 0, 939, 3338.7, -5699.78, 13.01, 3.87),
(107691, 0, 939, 3062.74, -5564.3, 18.13, 0),
(107934, 0, 939, 3219.32, -4995.56, 194.1, 1.87),
(107979, 0, 939, 3490.91, -5005.29, 197.63, 2.56);

DELETE FROM `areatrigger_scripts` WHERE `entry` IN (7029, 7075, 7144, 7308);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7029, 'at_well_of_eternity_intro'),
(7075, 'at_well_of_eternity_perotharn_preevent_appear'),
(7144, 'at_well_of_eternity_skip_illidan_intro'),
(7308, 'at_well_of_eternity_illidan_outro_resume');
UPDATE `gameobject_template` SET `data1`=`entry` WHERE `entry` IN (209541, 210025);
