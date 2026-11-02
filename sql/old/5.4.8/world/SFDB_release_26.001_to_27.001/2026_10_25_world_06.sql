-- Death Knight spell scripts, procs, links, coefficients and buff groups

-- spell_script_names
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
'spell_dk_blood_charges','spell_dk_blood_tap','spell_dk_rune_converter','spell_dk_bone_shield_check','spell_dk_bone_shield',
'spell_dk_dancing_rune_weapon','spell_dk_rune_weapon_scaling','spell_dk_pillar_of_frost','spell_dk_howling_blast','spell_dk_sudden_doom',
'spell_dk_shadow_infusion_proc','spell_dk_shadow_infusion','spell_dk_dark_transformation','spell_dk_ghoul_ability','spell_dk_festering_strike',
'spell_dk_plague_strike','spell_dk_chilblains','spell_dk_unholy_blight','spell_dk_pestilence','spell_dk_blood_boil','spell_dk_death_siphon',
'spell_dk_remorseless_winter_targets','spell_dk_remorseless_winter','spell_dk_desecrated_ground','spell_dk_plague_leech','spell_dk_purgatory',
'spell_dk_purgatory_absorb','spell_dk_deaths_advance','spell_dk_presence','spell_dk_glyph_of_horn_of_winter','spell_dk_glyph_of_the_loud_horn',
'spell_dk_glyph_of_death_and_decay','spell_dk_glyph_of_unholy_command','spell_dk_glyph_of_strangulate','spell_dk_glyph_of_the_skeleton',
'spell_dk_glyph_of_the_geist','spell_dk_will_of_the_necropolis','spell_dk_will_of_the_necropolis_rune_tap','spell_dk_crimson_scourge',
'spell_dk_relentless_grip','spell_dk_gargoyle_strike','spell_dk_riposte','spell_dk_might_of_ursoc','spell_dk_wild_mushroom_plague_periodic',
'spell_dk_wild_mushroom_plague_trigger','spell_dk_frozen_power','spell_dk_t15_dps_4p_bonus','spell_dk_t15_blood_4p_bonus',
'spell_dk_death_shroud_duration','spell_dk_death_shroud','spell_dk_t16_dps_4p_bonus','spell_dk_t16_blood_2p_bonus','spell_dk_t16_blood_4p_bonus',
'spell_dk_soul_reaper','spell_dk_soul_reaper_haste','spell_dk_unholy_frenzy','spell_dk_dark_simulacrum','spell_dk_asphyxiate');
DELETE FROM `spell_script_names` WHERE `spell_id` = 47541 AND `ScriptName` = 'spell_dk_death_coil';
DELETE FROM `spell_script_names` WHERE `spell_id` = 110570 AND `ScriptName` = 'spell_dk_anti_magic_shell_self';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(47541, 'spell_dk_death_coil'),
(47541, 'spell_dk_blood_charges'),
(49143, 'spell_dk_blood_charges'),
(56815, 'spell_dk_blood_charges'),
(45529, 'spell_dk_blood_tap'),
(50034, 'spell_dk_rune_converter'),
(56835, 'spell_dk_rune_converter'),
(49222, 'spell_dk_bone_shield_check'),
(49222, 'spell_dk_bone_shield'),
(49028, 'spell_dk_dancing_rune_weapon'),
(51906, 'spell_dk_rune_weapon_scaling'),
(51271, 'spell_dk_pillar_of_frost'),
(49184, 'spell_dk_howling_blast'),
(49530, 'spell_dk_sudden_doom'),
(49572, 'spell_dk_shadow_infusion_proc'),
(91342, 'spell_dk_shadow_infusion'),
(63560, 'spell_dk_dark_transformation'),
(47468, 'spell_dk_ghoul_ability'),
(47481, 'spell_dk_ghoul_ability'),
(47482, 'spell_dk_ghoul_ability'),
(47484, 'spell_dk_ghoul_ability'),
(85948, 'spell_dk_festering_strike'),
(45462, 'spell_dk_plague_strike'),
(45524, 'spell_dk_chilblains'),
(115994, 'spell_dk_unholy_blight'),
(50842, 'spell_dk_pestilence'),
(116617, 'spell_dk_pestilence'),
(48721, 'spell_dk_blood_boil'),
(108196, 'spell_dk_death_siphon'),
(108200, 'spell_dk_remorseless_winter_targets'),
(115000, 'spell_dk_remorseless_winter'),
(118009, 'spell_dk_desecrated_ground'),
(123693, 'spell_dk_plague_leech'),
(116888, 'spell_dk_purgatory'),
(114556, 'spell_dk_purgatory_absorb'),
(96268, 'spell_dk_deaths_advance'),
(48263, 'spell_dk_presence'),
(48265, 'spell_dk_presence'),
(48266, 'spell_dk_presence'),
(57330, 'spell_dk_glyph_of_horn_of_winter'),
(57330, 'spell_dk_glyph_of_the_loud_horn'),
(43265, 'spell_dk_glyph_of_death_and_decay'),
(59327, 'spell_dk_glyph_of_unholy_command'),
(47476, 'spell_dk_glyph_of_strangulate'),
(146652, 'spell_dk_glyph_of_the_skeleton'),
(58640, 'spell_dk_glyph_of_the_geist'),
(81164, 'spell_dk_will_of_the_necropolis'),
(96171, 'spell_dk_will_of_the_necropolis_rune_tap'),
(81136, 'spell_dk_crimson_scourge'),
(131542, 'spell_dk_relentless_grip'),
(51963, 'spell_dk_gargoyle_strike'),
(145677, 'spell_dk_riposte'),
(113072, 'spell_dk_might_of_ursoc'),
(113517, 'spell_dk_wild_mushroom_plague_periodic'),
(113518, 'spell_dk_wild_mushroom_plague_trigger'),
(147620, 'spell_dk_frozen_power'),
(51124, 'spell_dk_t15_dps_4p_bonus'),
(138197, 'spell_dk_t15_blood_4p_bonus'),
(144901, 'spell_dk_death_shroud_duration'),
(144901, 'spell_dk_death_shroud'),
(144907, 'spell_dk_t16_dps_4p_bonus'),
(144934, 'spell_dk_t16_blood_2p_bonus'),
(144950, 'spell_dk_t16_blood_4p_bonus'),
(110570, 'spell_dk_anti_magic_shell_self'),
(114866, 'spell_dk_soul_reaper'),
(130735, 'spell_dk_soul_reaper'),
(130736, 'spell_dk_soul_reaper'),
(114868, 'spell_dk_soul_reaper_haste'),
(49016, 'spell_dk_unholy_frenzy'),
(77606, 'spell_dk_dark_simulacrum'),
(108194, 'spell_dk_asphyxiate');

-- spell_proc_event
DELETE FROM `spell_proc_event` WHERE `entry` IN (49530,49572,50034,56817,56835,58677,59057,59327,60132,62459,66192,67115,70652,77606,131542,131547,144899,144907,144934,144950);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(49530, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0),                              -- Sudden Doom
(49572, 32, 15, 2147483648, 0, 1, 0, 0, 0, 0, 0, 0),                   -- Shadow Infusion
(50034, 0, 15, 16, 0, 0, 0, 0, 0, 0, 0, 0),                            -- Blood Rites
(56817, 0, 15, 0, 536870912, 0, 0, 0, 0, 0, 0, 0),                     -- Rune Strike
(56835, 0, 15, 4456450, 65536, 512, 0, 0, 0, 0, 0, 0),                 -- Reaping
(58677, 0, 15, 8192, 0, 0, 0, 0, 262144, 0, 0, 0),                     -- Glyph of Death's Embrace
(59057, 1, 15, 0, 131072, 0, 0, 0, 0, 0, 0, 0),                        -- Rime
(59327, 0, 15, 134217728, 0, 0, 0, 0, 0, 0, 0, 0),                     -- Glyph of Unholy Command
(60132, 0, 15, 16, 134348800, 0, 0, 0, 0, 0, 0, 0),                    -- Oblit/Scourge Strike Runic Power Up
(62459, 0, 15, 4, 0, 0, 0, 0, 0, 0, 0, 0),                             -- Chains of Ice Runic Power
(66192, 0, 15, 0, 0, 0, 0, 16, 3, 0, 0, 0),                            -- Threat of Thassarian
(67115, 0, 15, 20971520, 0, 0, 0, 0, 0, 0, 0, 45000),                  -- Item - Death Knight T9 Melee 2P Bonus
(70652, 0, 15, 8, 0, 0, 0, 0, 0, 0, 0, 0),                             -- Item - Death Knight T10 Tank 4P Bonus
(77606, 0, 0, 0, 0, 0, 0, 0, 65536, 0, 0, 0),                          -- Dark Simulacrum
(131542, 0, 15, 33554432, 0, 0, 0, 0, 65537, 0, 0, 0),                 -- Relentless Grip
(131547, 0, 15, 33554432, 0, 0, 0, 0, 65537, 0, 0, 0),                 -- Relentless Grip
(144899, 0, 15, 0, 0, 131072, 16, 0, 1, 0, 0, 0),                      -- Item - Death Knight T16 DPS 2P Bonus
(144907, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0),                             -- Item - Death Knight T16 DPS 4P Bonus
(144934, 0, 15, 17047552, 536870912, 67108864, 0, 69648, 1, 0, 101, 0), -- Item - Death Knight T16 Blood 2P Bonus
(144950, 0, 15, 1073741824, 0, 0, 0, 0, 1, 0, 0, 0);                   -- Item - Death Knight T16 Blood 4P Bonus

-- spell_linked_spell
DELETE FROM `spell_linked_spell` WHERE (`spell_trigger`, `spell_effect`) IN ((-131547,136410),(-51271,-144909),(-51271,-90259),(48778,50772),(73313,50772),(81162,96171));
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-131547, 136410, 0, 'Relentless Grip'),
(-51271, -144909, 0, 'Pillar of Frost - remove Death Knight T16 DPS 4P Bonus (Hidden)'),
(-51271, -90259, 0, 'Pillar of Frost - remove Glyph of Pillar of Frost'),
(48778, 50772, 0, 'Acherus Deathcharger - Summon Unholy Mount Visual'),
(73313, 50772, 0, 'Crimson Deathcharger - Summon Unholy Mount Visual'),
(81162, 96171, 0, 'Will of the Necropolis');

-- spell_bonus_data
DELETE FROM `spell_bonus_data` WHERE `entry` IN (45524,108196,114867,147620);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(45524, -1, -1, 0.08, -1, 'Death Knight - Chains of Ice (Glyphed)'),
(108196, -1, -1, 0.374, -1, 'Death Knight - Death Siphon'),
(114867, -1, -1, 1.2, -1, 'Death Knight - Soul Reaper'),
(147620, -1, -1, 0.08, -1, 'Death Knight - Frozen Power');

-- spell_learn_spell
DELETE FROM `spell_learn_spell` WHERE (`entry`, `SpellID`) IN ((56815,56816),(96268,124285));
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES
(56815, 56816, 0),
(96268, 124285, 1);

-- spell_group: attack power and attack speed raid buffs are exclusive
DELETE FROM `spell_group` WHERE `id` IN (1123,1128);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1123, 6673),   -- Battle Shout
(1123, 57330),  -- Horn of Winter
(1128, 30809),  -- Unleashed Rage
(1128, 55610),  -- Unholy Aura
(1128, 113742); -- Swiftblade's Cunning

DELETE FROM `spell_group_stack_rules` WHERE `group_id` IN (1123,1128);
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(1123, 1),
(1128, 1);
