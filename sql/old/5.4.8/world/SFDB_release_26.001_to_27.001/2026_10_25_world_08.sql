-- Hunter spell scripts
DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
(53397, 'spell_hun_invigoration'),
(34026, 'spell_hun_kill_command'),
(83381, 'spell_hun_kill_command_damage'),
(3044, 'spell_hun_hunters_mark_trigger'),
(34026, 'spell_hun_hunters_mark_trigger'),
(53209, 'spell_hun_hunters_mark_trigger'),
(53301, 'spell_hun_hunters_mark_trigger'),
(19615, 'spell_hun_frenzy'),
(82692, 'spell_hun_focus_fire'),
(19574, 'spell_hun_bestial_wrath'),
(118455, 'spell_hun_beast_cleave_proc'),
(118459, 'spell_hun_beast_cleave_damage'),
(2643, 'spell_hun_multi_shot'),
(53260, 'spell_hun_cobra_strikes'),
(16827, 'spell_hun_cobra_strikes_stacks'),
(17253, 'spell_hun_cobra_strikes_stacks'),
(49966, 'spell_hun_cobra_strikes_stacks'),
(145625, 'spell_hun_cobra_strikes_stacks'),
(82726, 'spell_hun_fervor'),
(53353, 'spell_hun_chimera_shot_heal'),
(34487, 'spell_hun_master_marksman'),
(53224, 'spell_hun_steady_focus'),
(76659, 'spell_hun_wild_quiver'),
(53302, 'spell_hun_sniper_training'),
(56343, 'spell_hun_lock_and_load'),
(56453, 'spell_hun_lock_and_load_cooldown'),
(13812, 'spell_hun_explosive_trap'),
(19387, 'spell_hun_entrapment'),
(3355, 'spell_hun_crowd_control_dot_hack'),
(19503, 'spell_hun_crowd_control_dot_hack'),
(16827, 'spell_hun_pet_basic_attack'),
(17253, 'spell_hun_pet_basic_attack'),
(49966, 'spell_hun_pet_basic_attack'),
(16827, 'spell_hun_blink_strikes'),
(17253, 'spell_hun_blink_strikes'),
(49966, 'spell_hun_blink_strikes'),
(145625, 'spell_hun_blink_strikes'),
(50285, 'spell_hun_pet_dust_cloud'),
(95725, 'spell_hun_pet_ap_scaling_damage'),
(63900, 'spell_hun_pet_ap_scaling_damage'),
(95714, 'spell_hun_pet_ap_scaling_damage'),
(90361, 'spell_hun_spirit_mend'),
(128997, 'spell_hun_spirit_beast_blessing'),
(120697, 'spell_hun_lynx_rush'),
(120699, 'spell_hun_lynx_rush_dot'),
(118694, 'spell_hun_spirit_bond'),
(781, 'spell_hun_disengage'),
(115928, 'spell_hun_narrow_escape'),
(113073, 'spell_hun_dash'),
(51753, 'spell_hun_camouflage_driver'),
(51755, 'spell_hun_camouflage'),
(80326, 'spell_hun_camouflage_invisibility_driver'),
(125042, 'spell_hun_glyph_of_aspect_of_the_beast'),
(118424, 'spell_hun_tracking'),
(136, 'spell_hun_glyph_of_mend_pet'),
(5118, 'spell_hun_aspect_of_the_cheetah'),
(5118, 'spell_hun_glyph_of_the_aspects'),
(13159, 'spell_hun_glyph_of_the_aspects'),
(13165, 'spell_hun_glyph_of_the_aspects'),
(61648, 'spell_hun_glyph_of_the_aspects'),
(109260, 'spell_hun_glyph_of_the_aspects'),
(19263, 'spell_hun_deterrence'),
(67801, 'spell_hun_deterrence'),
(110617, 'spell_hun_deterrence'),
(110618, 'spell_hun_deterrence'),
(148467, 'spell_hun_deterrence'),
(19263, 'spell_hun_glyph_of_deterrence'),
(148467, 'spell_hun_glyph_of_deterrence'),
(2641, 'spell_hun_dismiss_pet'),
(127933, 'spell_hun_fireworks'),
(57870, 'spell_hun_glyph_of_lesser_proportion'),
(144637, 'spell_hun_t16_2p_bonus'),
(110588, 'spell_hun_misdirection'),
(110591, 'spell_hun_misdirection_proc'),
(144641, 'spell_hun_t16_4p_bonus'),
(56453, 'spell_hun_t16_4p_bonus_survival'),
(34471, 'spell_hun_remove_brutal_kinship'),
(109306, 'spell_hun_thrill_of_the_hunt'),
(109248, 'spell_hun_binding_shot'),
(117405, 'spell_hun_binding_shot_link'),
(63487, 'spell_hun_ice_trap'),
(110601, 'spell_hun_ice_trap'));
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(53397, 'spell_hun_invigoration'),
(34026, 'spell_hun_kill_command'),
(83381, 'spell_hun_kill_command_damage'),
(3044, 'spell_hun_hunters_mark_trigger'),
(34026, 'spell_hun_hunters_mark_trigger'),
(53209, 'spell_hun_hunters_mark_trigger'),
(53301, 'spell_hun_hunters_mark_trigger'),
(19615, 'spell_hun_frenzy'),
(82692, 'spell_hun_focus_fire'),
(19574, 'spell_hun_bestial_wrath'),
(118455, 'spell_hun_beast_cleave_proc'),
(118459, 'spell_hun_beast_cleave_damage'),
(2643, 'spell_hun_multi_shot'),
(53260, 'spell_hun_cobra_strikes'),
(16827, 'spell_hun_cobra_strikes_stacks'),
(17253, 'spell_hun_cobra_strikes_stacks'),
(49966, 'spell_hun_cobra_strikes_stacks'),
(145625, 'spell_hun_cobra_strikes_stacks'),
(82726, 'spell_hun_fervor'),
(53353, 'spell_hun_chimera_shot_heal'),
(34487, 'spell_hun_master_marksman'),
(53224, 'spell_hun_steady_focus'),
(76659, 'spell_hun_wild_quiver'),
(53302, 'spell_hun_sniper_training'),
(56343, 'spell_hun_lock_and_load'),
(56453, 'spell_hun_lock_and_load_cooldown'),
(13812, 'spell_hun_explosive_trap'),
(19387, 'spell_hun_entrapment'),
(3355, 'spell_hun_crowd_control_dot_hack'),
(19503, 'spell_hun_crowd_control_dot_hack'),
(16827, 'spell_hun_pet_basic_attack'),
(17253, 'spell_hun_pet_basic_attack'),
(49966, 'spell_hun_pet_basic_attack'),
(16827, 'spell_hun_blink_strikes'),
(17253, 'spell_hun_blink_strikes'),
(49966, 'spell_hun_blink_strikes'),
(145625, 'spell_hun_blink_strikes'),
(50285, 'spell_hun_pet_dust_cloud'),
(95725, 'spell_hun_pet_ap_scaling_damage'),
(63900, 'spell_hun_pet_ap_scaling_damage'),
(95714, 'spell_hun_pet_ap_scaling_damage'),
(90361, 'spell_hun_spirit_mend'),
(128997, 'spell_hun_spirit_beast_blessing'),
(120697, 'spell_hun_lynx_rush'),
(120699, 'spell_hun_lynx_rush_dot'),
(118694, 'spell_hun_spirit_bond'),
(781, 'spell_hun_disengage'),
(115928, 'spell_hun_narrow_escape'),
(113073, 'spell_hun_dash'),
(51753, 'spell_hun_camouflage_driver'),
(51755, 'spell_hun_camouflage'),
(80326, 'spell_hun_camouflage_invisibility_driver'),
(125042, 'spell_hun_glyph_of_aspect_of_the_beast'),
(118424, 'spell_hun_tracking'),
(136, 'spell_hun_glyph_of_mend_pet'),
(5118, 'spell_hun_aspect_of_the_cheetah'),
(5118, 'spell_hun_glyph_of_the_aspects'),
(13159, 'spell_hun_glyph_of_the_aspects'),
(13165, 'spell_hun_glyph_of_the_aspects'),
(61648, 'spell_hun_glyph_of_the_aspects'),
(109260, 'spell_hun_glyph_of_the_aspects'),
(19263, 'spell_hun_deterrence'),
(67801, 'spell_hun_deterrence'),
(110617, 'spell_hun_deterrence'),
(110618, 'spell_hun_deterrence'),
(148467, 'spell_hun_deterrence'),
(19263, 'spell_hun_glyph_of_deterrence'),
(148467, 'spell_hun_glyph_of_deterrence'),
(2641, 'spell_hun_dismiss_pet'),
(127933, 'spell_hun_fireworks'),
(57870, 'spell_hun_glyph_of_lesser_proportion'),
(144637, 'spell_hun_t16_2p_bonus'),
(110588, 'spell_hun_misdirection'),
(110591, 'spell_hun_misdirection_proc'),
(144641, 'spell_hun_t16_4p_bonus'),
(56453, 'spell_hun_t16_4p_bonus_survival'),
(34471, 'spell_hun_remove_brutal_kinship'),
(109306, 'spell_hun_thrill_of_the_hunt'),
(109248, 'spell_hun_binding_shot'),
(117405, 'spell_hun_binding_shot_link'),
(63487, 'spell_hun_ice_trap'),
(110601, 'spell_hun_ice_trap');

-- Ice Trap, Glyph of Black Ice and Flare area triggers
DELETE FROM `spell_areatrigger_template` WHERE `Entry` IN (347, 510, 621, 628);
INSERT INTO `spell_areatrigger_template` (`Entry`, `Flags`, `CollisionType`, `Radius`, `ScaleX`, `ScaleY`, `ScriptName`) VALUES
(347, 0, 0, 10, 0, 0, 'sat_hun_black_ice'),
(510, 0, 0, 10, 0, 0, 'sat_hun_flare'),
(621, 0, 1, 10, 0, 0, 'sat_hun_ice_trap'),
(628, 0, 0, 10, 0, 0, 'sat_hun_ice_trap');

-- Proc conditions
DELETE FROM `spell_proc_event` WHERE `entry` IN (19184, 19387, 23721, 34487, 35102, 35110, 40485, 53232, 53234, 53238, 56343, 57870, 64860, 70730, 118976, 131564, 138365, 138367, 144637);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(19184, 0, 9, 16, 8192, 0, 0, 0, 0, 0, 0, 0),              -- Entrapment
(19387, 0, 0, 0, 8192, 262144, 0, 0, 0, 0, 0, 0),          -- Entrapment
(23721, 0, 9, 2048, 0, 0, 0, 0, 0, 0, 0, 0),
(34487, 0, 9, 0, 1, 0, 0, 0, 0, 0, 0, 0),                  -- Master Marksman
(35102, 0, 9, 4096, 0, 1, 0, 0, 0, 0, 0, 0),
(35110, 0, 9, 4096, 0, 0, 0, 0, 2, 0, 0, 0),
(40485, 0, 9, 0, 1, 0, 0, 0, 0, 0, 0, 0),
(53232, 0, 9, 32, 0, 0, 0, 0, 0, 0, 0, 0),
(53234, 0, 9, 131072, 1, 1, 0, 0, 2, 0, 0, 0),             -- Piercing Shots
(53238, 0, 9, 131072, 1, 1, 0, 0, 2, 0, 0, 0),             -- Piercing Shots
(56343, 0, 9, 8, 134217728, 262144, 0, 0, 0, 0, 0, 10000), -- Lock and Load
(57870, 0, 9, 8388608, 0, 0, 0, 262144, 0, 0, 0, 0),       -- Glyph of Lesser Proportion
(64860, 0, 9, 0, 1, 0, 0, 0, 0, 0, 0, 0),
(70730, 0, 9, 16384, 4096, 0, 0, 262144, 0, 0, 0, 0),
(118976, 0, 9, 16384, 0, 0, 0, 0, 0, 0, 0, 0),
(131564, 64, 9, 2048, 0, 0, 0, 0, 65536, 0, 0, 0),
(138365, 0, 9, 0, 1, 4194304, 0, 0, 0, 0, 0, 0),
(138367, 0, 9, 137216, 0, 0, 0, 0, 0, 0, 0, 0),
(144637, 0, 9, 137216, 0, 0, 0, 0, 0, 0, 0, 0);            -- Item - Hunter T16 2P Bonus

-- Damage coefficients
DELETE FROM `spell_bonus_data` WHERE `entry` IN (53301, 83381, 118253, 120761, 121414, 131900);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(53301, -1, -1, 0.391, 0.391, 'Hunter - Explosive Shot'),
(83381, -1, -1, 1, -1, 'Hunter - Kill Command'),
(118253, -1, -1, -1, 0.16, 'Hunter - Serpent Sting'),
(120761, -1, -1, 0.2, -1, 'Hunter - Glaive Toss'),
(121414, -1, -1, 0.2, -1, 'Hunter - Glaive Toss'),
(131900, -1, -1, 0.288, -1, 'Hunter - A Murder of Crows');

-- Glyph-taught spells
DELETE FROM `spell_learn_spell` WHERE (`entry`, `SpellID`) IN ((57903, 127933), (79682, 93321));
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES
(57903, 127933, 1),
(79682, 93321, 1);

-- Glyphed Deterrence
DELETE FROM `spell_linked_spell` WHERE (`spell_trigger`, `spell_effect`, `type`) IN ((148467, 110618, 2), (-148467, -110618, 2));
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(148467, 110618, 2, 'Deterrence'),
(-148467, -110618, 2, 'Deterrence');

-- Pet buff and debuff exclusivity
DELETE FROM `spell_group` WHERE (`id`, `spell_id`) IN (
(1109, 90364),
(1127, 93435), (1127, 127830),
(1129, 24844), (1129, 34889),
(1134, 54680), (1134, 82654),
(1141, 24604), (1141, 90309), (1141, 126373));
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1109, 90364),               -- Qiraji Fortitude
(1127, 93435), (1127, 127830), -- Roar of Courage, Spirit Beast Blessing
(1129, 24844), (1129, 34889),  -- Lightning Breath, Fire Breath
(1134, 54680), (1134, 82654),  -- Monstrous Bite, Widow Venom
(1141, 24604), (1141, 90309), (1141, 126373); -- Furious Howl, Terrifying Roar, Fearless Roar

DELETE FROM `spell_group_stack_rules` WHERE `group_id` IN (1127, 1129, 1134, 1141);
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(1127, 1),
(1129, 3),
(1134, 3),
(1141, 1);
