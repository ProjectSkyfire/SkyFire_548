-- Restore script bindings that match the current 5.4.8 world data.

-- Creature and gameobject scripts.
UPDATE `creature_template` SET `ScriptName`='npc_pet_shaman_earth_elemental' WHERE `entry`=15352;
UPDATE `creature_template` SET `ScriptName`='npc_pet_shaman_fire_elemental' WHERE `entry`=15438;
UPDATE `creature_template` SET `ScriptName`='boss_lord_valthalak' WHERE `entry`=16042;
UPDATE `creature_template` SET `ScriptName`='npc_pet_hunter_snake_trap' WHERE `entry` IN (19833,19921);
UPDATE `creature_template` SET `ScriptName`='npc_yor' WHERE `entry`=22930;
UPDATE `creature_template` SET `ScriptName`='boss_anzu' WHERE `entry`=23035;
UPDATE `creature_template` SET `ScriptName`='npc_pet_gen_mojo' WHERE `entry`=24480;
UPDATE `creature_template` SET `ScriptName`='npc_drakkari_invader' WHERE `entry` IN (27709,27753);
UPDATE `creature_template` SET `ScriptName`='npc_pet_dk_ebon_gargoyle' WHERE `entry`=27829;
UPDATE `creature_template` SET `ScriptName`='npc_storm_cloud' WHERE `entry`=29939;
UPDATE `creature_template` SET `ScriptName`='npc_morbidus' WHERE `entry`=30698;
UPDATE `creature_template` SET `ScriptName`='npc_violet_hold_arcane_sphere' WHERE `entry`=30837;
UPDATE `creature_template` SET `ScriptName`='npc_margrave_dhakar' WHERE `entry`=31306;
UPDATE `creature_template` SET `ScriptName`='npc_pet_pri_lightwell' WHERE `entry` IN (31883,31893,31894,31895,31896,31897);
UPDATE `creature_template` SET `ScriptName`='npc_spring_rabbit' WHERE `entry`=32791;
UPDATE `creature_template` SET `ScriptName`='npc_defiant_troll' WHERE `entry`=34830;
UPDATE `creature_template` SET `ScriptName`='npc_raging_ghoul' WHERE `entry`=36940;
UPDATE `creature_template` SET `ScriptName`='npc_risen_witch_doctor' WHERE `entry`=36941;
UPDATE `creature_template` SET `ScriptName`='npc_jaina_or_sylvanas_escape_hor' WHERE `entry` IN (36955,37554);
UPDATE `creature_template` SET `ScriptName`='npc_lumbering_abomination' WHERE `entry`=37069;
UPDATE `creature_template` SET `ScriptName`='npc_blackrock_invader' WHERE `entry`=42937;
UPDATE `creature_template` SET `ScriptName`='boss_pit_lord_argaloth' WHERE `entry`=47120;
UPDATE `creature_template` SET `ScriptName`='npc_blackrock_spy' WHERE `entry`=49874;
UPDATE `creature_template` SET `ScriptName`='npc_goblin_assassin' WHERE `entry`=50039;
UPDATE `creature_template` SET `ScriptName`='boss_occuthar' WHERE `entry`=52363;
UPDATE `creature_template` SET `ScriptName`='npc_eyestalk' WHERE `entry`=52369;
UPDATE `gameobject_template` SET `ScriptName`='go_blackhoof_cage' WHERE `entry`=186287;
UPDATE `gameobject_template` SET `ScriptName`='go_activation_crystal' WHERE `entry`=193611;

DELETE FROM `areatrigger_scripts`
WHERE `entry`=5605 OR `ScriptName`='at_shadow_throne';
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(5605,'at_shadow_throne');

-- Achievement scripts with valid Criteria.dbc entries.
DELETE FROM `achievement_criteria_data`
WHERE `ScriptName` IN ('achievement_he_feeds_on_your_tears','achievement_killed_exp_or_honor_target')
   OR (`criteria_id`=4946 AND `type`=11);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(4946,11,0,0,'achievement_killed_exp_or_honor_target');

-- Spell scripts. Delete stale rows first so missing or retargeted spell ids do not survive.
DELETE FROM `spell_script_names`
WHERE `ScriptName` IN (
'spell_devourer_of_souls_mirrored_soul',
'spell_devourer_of_souls_mirrored_soul_proc',
'spell_devourer_of_souls_mirrored_soul_target_selector',
'spell_dk_death_strike_enabler',
'spell_dk_icebound_fortitude',
'spell_dk_rune_tap_party',
'spell_dk_scent_of_blood',
'spell_dru_glyph_of_starfire',
'spell_dru_glyph_of_starfire_proc',
'spell_dru_predatory_strikes',
'spell_dru_stampede',
'spell_dru_starfall_dummy',
'spell_gen_ds_flush_knockback',
'spell_gen_override_display_power',
'spell_general_vezax_mark_of_the_faceless',
'spell_general_vezax_mark_of_the_faceless_leech',
'spell_hun_fire',
'spell_hun_improved_serpent_sting',
'spell_hun_invigoration',
'spell_hun_ready_set_aim',
'spell_hun_thrill_of_the_hunt',
'spell_mastery_blood_shield',
'spell_mastery_combo_breaker',
'spell_mastery_elemental_overload',
'spell_mastery_hand_of_light',
'spell_mastery_ignite',
'spell_mastery_shield_discipline',
'spell_mastery_unshackled_fury',
'spell_monk_chi_burst',
'spell_monk_crackling_jade_lightning',
'spell_monk_glyph_of_mana_tea',
'spell_monk_glyph_of_zen_flight',
'spell_monk_mana_tea_stacks',
'spell_monk_serpents_zeal',
'spell_monk_spinning_fire_blossom_damage',
'spell_monk_teachings_of_the_monastery',
'spell_monk_zen_sphere_hot',
'spell_occuthar_eyes_of_occuthar',
'spell_occuthar_eyes_of_occuthar_vehicle',
'spell_occuthar_focused_fire',
'spell_occuthar_occuthars_destruction',
'spell_pal_aura_mastery',
'spell_pal_aura_mastery_immune',
'spell_pal_item_healing_discount',
'spell_pri_item_greater_heal_refund',
'spell_pri_vampiric_embrace_target',
'spell_q28813_set_health_random',
'spell_rog_crippling_poison',
'spell_sha_healing_stream_totem',
'spell_sha_item_lightning_shield',
'spell_sha_item_lightning_shield_trigger',
'spell_sha_item_mana_surge',
'spell_sha_mana_tide_totem',
'spell_sha_nature_guardian',
'spell_sha_telluric_currents',
'spell_sha_thunderstorm',
'spell_tharon_ja_clear_gift_of_tharon_ja',
'spell_trollgore_consume',
'spell_trollgore_corpse_explode',
'spell_trollgore_invader_taunt',
'spell_warl_fel_synergy',
'spell_warl_healthstone_heal',
'spell_warl_soul_swap_dot_marker',
'spell_warl_soul_swap_override',
'spell_warr_concussion_blow',
'spell_warr_improved_spell_reflection',
'spell_warr_rend',
'spell_warr_retaliation',
'spell_warr_victorious',
'spell_xt002_heart_overload_periodic'
);

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(69051,'spell_devourer_of_souls_mirrored_soul'),
(69023,'spell_devourer_of_souls_mirrored_soul_proc'),
(69048,'spell_devourer_of_souls_mirrored_soul_target_selector'),
(89832,'spell_dk_death_strike_enabler'),
(48792,'spell_dk_icebound_fortitude'),
(59754,'spell_dk_rune_tap_party'),
(49509,'spell_dk_scent_of_blood'),
(16974,'spell_dru_predatory_strikes'),
(50286,'spell_dru_starfall_dummy'),
(96538,'spell_gen_ds_flush_knockback'),
(123933,'spell_gen_override_display_power'),
(145627,'spell_gen_override_display_power'),
(145628,'spell_gen_override_display_power'),
(145044,'spell_gen_override_display_power'),
(63276,'spell_general_vezax_mark_of_the_faceless'),
(63278,'spell_general_vezax_mark_of_the_faceless_leech'),
(82926,'spell_hun_fire'),
(82834,'spell_hun_improved_serpent_sting'),
(82925,'spell_hun_ready_set_aim'),
(12880,'spell_mastery_unshackled_fury'),
(17,'spell_mastery_shield_discipline'),
(123258,'spell_mastery_shield_discipline'),
(114908,'spell_mastery_shield_discipline'),
(114214,'spell_mastery_shield_discipline'),
(47753,'spell_mastery_shield_discipline'),
(100780,'spell_mastery_combo_breaker'),
(108557,'spell_mastery_combo_breaker'),
(115698,'spell_mastery_combo_breaker'),
(115687,'spell_mastery_combo_breaker'),
(115693,'spell_mastery_combo_breaker'),
(115695,'spell_mastery_combo_breaker'),
(45470,'spell_mastery_blood_shield'),
(133,'spell_mastery_ignite'),
(44614,'spell_mastery_ignite'),
(108853,'spell_mastery_ignite'),
(2948,'spell_mastery_ignite'),
(11366,'spell_mastery_ignite'),
(35395,'spell_mastery_hand_of_light'),
(53595,'spell_mastery_hand_of_light'),
(24275,'spell_mastery_hand_of_light'),
(85256,'spell_mastery_hand_of_light'),
(53385,'spell_mastery_hand_of_light'),
(403,'spell_mastery_elemental_overload'),
(421,'spell_mastery_elemental_overload'),
(51505,'spell_mastery_elemental_overload'),
(117014,'spell_mastery_elemental_overload'),
(123986,'spell_monk_chi_burst'),
(117952,'spell_monk_crackling_jade_lightning'),
(125893,'spell_monk_glyph_of_zen_flight'),
(123766,'spell_monk_mana_tea_stacks'),
(127722,'spell_monk_serpents_zeal'),
(123408,'spell_monk_spinning_fire_blossom_damage'),
(107270,'spell_monk_teachings_of_the_monastery'),
(124081,'spell_monk_zen_sphere_hot'),
(96872,'spell_occuthar_focused_fire'),
(96931,'spell_occuthar_eyes_of_occuthar'),
(96932,'spell_occuthar_eyes_of_occuthar_vehicle'),
(96942,'spell_occuthar_occuthars_destruction'),
(37705,'spell_pal_item_healing_discount'),
(37594,'spell_pri_item_greater_heal_refund'),
(15290,'spell_pri_vampiric_embrace_target'),
(53034,'spell_q28813_set_health_random'),
(51626,'spell_rog_crippling_poison'),
(23551,'spell_sha_item_lightning_shield'),
(23552,'spell_sha_item_lightning_shield_trigger'),
(23572,'spell_sha_item_mana_surge'),
(16191,'spell_sha_mana_tide_totem'),
(30884,'spell_sha_nature_guardian'),
(55453,'spell_sha_telluric_currents'),
(51490,'spell_sha_thunderstorm'),
(53242,'spell_tharon_ja_clear_gift_of_tharon_ja'),
(49380,'spell_trollgore_consume'),
(59803,'spell_trollgore_consume'),
(49555,'spell_trollgore_corpse_explode'),
(59807,'spell_trollgore_corpse_explode'),
(49405,'spell_trollgore_invader_taunt'),
(47230,'spell_warl_fel_synergy'),
(6262,'spell_warl_healthstone_heal'),
(92795,'spell_warl_soul_swap_dot_marker'),
(86211,'spell_warl_soul_swap_override'),
(52719,'spell_warr_concussion_blow'),
(114028,'spell_warr_improved_spell_reflection'),
(22857,'spell_warr_retaliation'),
(32215,'spell_warr_victorious'),
(62789,'spell_xt002_heart_overload_periodic');
