-- Druid spell scripts, procs and linked spells

-- Proc rules
DELETE FROM `spell_proc_event` WHERE `entry` IN (16961, 46832, 48484, 48500, 54825, 54832, 77495, 93399, 113043, 114107, 124974, 138216, 144841, 144864, 144869, 144871, 146874);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(16961, 0, 7, 233472, 1088, 0, 0, 0, 2, 0, 0, 0),             -- Primal Fury: crits with Rake, Shred, Ravage, Mangle
(46832, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 6000),                   -- Sudden Eclipse: crits only
(48484, 0, 7, 100352, 1088, 0, 0, 0, 0, 0, 0, 0),              -- Infected Wounds: Shred, Ravage, Mangle
(48500, 0, 7, 96, 33554434, 0, 0, 0, 2, 0, 0, 0),              -- Living Seed: crits with Healing Touch, Regrowth, Swiftmend, Nourish
(54825, 0, 7, 32, 0, 0, 0, 0, 0, 0, 0, 0),                     -- Glyph of Healing Touch: Healing Touch
(54832, 0, 7, 0, 4096, 0, 0, 16384, 0, 0, 100, 0),             -- Glyph of Innervate: Innervate
(77495, 0, 7, 96, 33554434, 0, 0, 16384, 0, 0, 100, 0),        -- Mastery: Harmony: direct heals
(93399, 0, 7, 2, 0, 0, 0, 0, 2, 0, 100, 0),                    -- Shooting Stars: chance rolled by the script
(113043, 0, 7, 0, 16, 0, 0, 0, 0, 0, 0, 0),                    -- Omen of Clarity (Restoration): Lifebloom
(114107, 0, 7, 5, 0, 33554432, 0, 0, 0, 0, 0, 0),              -- Soul of the Forest: Wrath, Starfire, Starsurge
(124974, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0),                     -- Nature's Vigil
(138216, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0),                    -- Item - Druid T15 Guardian 2P Bonus: dodge
(144841, 0, 7, 0, 0, 2048, 0, 0, 1, 0, 0, 0),                  -- Item - Druid T16 Feral 4P Bonus
(144864, 0, 7, 0, 0, 0, 262144, 0, 0, 0, 0, 0),                -- Item - Druid T16 Feral 2P Bonus
(144869, 0, 7, 16, 0, 0, 0, 0, 0, 0, 0, 0),                    -- Item - Druid T16 Restoration 2P Bonus: Rejuvenation
(144871, 0, 7, 32, 0, 0, 0, 16384, 0, 0, 0, 0),                -- Sage Mender: consumed by Healing Touch
(146874, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);                     -- Feral Rage

-- Linked spells
DELETE FROM `spell_linked_spell` WHERE (`spell_trigger`, `spell_effect`) IN ((-108373, -145151), (48517, -145151), (48518, -145151), (112071, -145151), (110617, 110618), (-110617, -110618), (-5487, -106922));
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-108373, -145151, 0, 'Dream of Cenarius - remove buff on unlearn'),
(48517, -145151, 0, 'Eclipse (Solar) - remove Dream of Cenarius'),
(48518, -145151, 0, 'Eclipse (Lunar) - remove Dream of Cenarius'),
(112071, -145151, 0, 'Celestial Alignment - remove Dream of Cenarius'),
(110617, 110618, 0, 'Deterrence (Symbiosis)'),
(-110617, -110618, 0, 'Deterrence (Symbiosis)'),
(-5487, -106922, 0, 'Bear Form - remove Might of Ursoc');

-- Spell power coefficients
DELETE FROM `spell_bonus_data` WHERE `entry` = 150017;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(150017, -1, -1, 0.08333, 0.08333, 'Force of Nature (Feral) - Rake');

-- Shattering Blow (Symbiosis) gets its own script
DELETE FROM `spell_script_names` WHERE `spell_id` = 112997 AND `ScriptName` = 'spell_warr_shattering_throw';

-- Script bindings
DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
(79577, 'spell_dru_eclipse_remove'),
(112071, 'spell_dru_celestial_alignment'),
(8921, 'spell_dru_moonfire'),
(99, 'spell_dru_glyph_of_omens'), (339, 'spell_dru_glyph_of_omens'), (770, 'spell_dru_glyph_of_omens'), (5211, 'spell_dru_glyph_of_omens'),
(33786, 'spell_dru_glyph_of_omens'), (102355, 'spell_dru_glyph_of_omens'), (102359, 'spell_dru_glyph_of_omens'), (102793, 'spell_dru_glyph_of_omens'),
(132469, 'spell_dru_glyph_of_omens'),
(46832, 'spell_dru_sudden_eclipse'),
(16914, 'spell_dru_glyph_of_hurricane'), (106996, 'spell_dru_glyph_of_hurricane'),
(144767, 'spell_dru_t16_balance_2p_bonus'),
(127538, 'spell_dru_savage_roar'),
(52610, 'spell_dru_savage_roar_duration_check'), (127538, 'spell_dru_savage_roar_duration_check'),
(127538, 'spell_dru_savage_roar_glyphed'),
(5221, 'spell_dru_rip_duration'), (6785, 'spell_dru_rip_duration'), (33876, 'spell_dru_rip_duration'), (102545, 'spell_dru_rip_duration'),
(5221, 'spell_dru_shred'), (114236, 'spell_dru_shred'),
(5217, 'spell_dru_tigers_fury'),
(5217, 'spell_dru_glyph_of_shred'), (106951, 'spell_dru_glyph_of_shred'),
(16961, 'spell_dru_primal_fury'),
(62078, 'spell_dru_swipe'),
(131537, 'spell_dru_item_pvp_feral_4p'),
(81022, 'spell_dru_stampede_effect'),
(131538, 'spell_dru_stampede_timer'),
(135700, 'spell_dru_t16_feral_2p_bonus'),
(144841, 'spell_dru_t16_feral_4p_bonus'),
(146874, 'spell_dru_feral_rage'),
(5185, 'spell_dru_healing_touch_dream_of_cenarius'),
(5176, 'spell_dru_wrath_dream_of_cenarius'),
(108373, 'spell_dru_dream_of_cenarius'),
(135597, 'spell_dru_tooth_and_claw_absorb'),
(6807, 'spell_dru_tooth_and_claw_trigger'),
(50334, 'spell_dru_berserk_bear'),
(5487, 'spell_dru_bear_form'),
(1178, 'spell_dru_bear_form_passive'),
(114237, 'spell_dru_glyph_of_fae_silence_cooldown'),
(138222, 'spell_dru_t15_guardian_4p_bonus'),
(770, 'spell_dru_glyph_of_fae_silence'), (102355, 'spell_dru_glyph_of_fae_silence'),
(6795, 'spell_dru_growl'),
(81262, 'spell_dru_efflorescence'), (142423, 'spell_dru_efflorescence'),
(81269, 'spell_dru_efflorescence_heal'), (142424, 'spell_dru_efflorescence_heal'),
(48438, 'spell_dru_wild_growth'),
(77495, 'spell_dru_harmony'),
(8936, 'spell_dru_glyph_of_regrowth'),
(54825, 'spell_dru_glyph_of_healing_touch'),
(144871, 'spell_dru_sage_mender'),
(774, 'spell_dru_t15_restoration_4p_bonus'),
(108291, 'spell_dru_heart_of_the_wild'), (108292, 'spell_dru_heart_of_the_wild'), (108293, 'spell_dru_heart_of_the_wild'), (108294, 'spell_dru_heart_of_the_wild'),
(768, 'spell_dru_heart_of_the_wild_bonus'), (5487, 'spell_dru_heart_of_the_wild_bonus'),
(108300, 'spell_dru_killer_instinct'),
(768, 'spell_dru_killer_instinct_form'), (5487, 'spell_dru_killer_instinct_form'),
(124974, 'spell_dru_natures_vigil'),
(124991, 'spell_dru_natures_vigil_damage'),
(124988, 'spell_dru_natures_vigil_heal'),
(110309, 'spell_dru_symbiosis_caster_aura'),
(110309, 'spell_dru_symbiosis'),
(110478, 'spell_dru_symbiosis_target_check'), (110479, 'spell_dru_symbiosis_target_check'), (110482, 'spell_dru_symbiosis_target_check'),
(110483, 'spell_dru_symbiosis_target_check'), (110484, 'spell_dru_symbiosis_target_check'), (110485, 'spell_dru_symbiosis_target_check'),
(110486, 'spell_dru_symbiosis_target_check'), (110488, 'spell_dru_symbiosis_target_check'), (110490, 'spell_dru_symbiosis_target_check'),
(110491, 'spell_dru_symbiosis_target_check'),
(126456, 'spell_dru_symbiosis_fortifying_brew'),
(110597, 'spell_dru_play_death'),
(122290, 'spell_dru_life_tap'),
(112970, 'spell_dru_demonic_circle_teleport'),
(112997, 'spell_dru_shattering_blow'),
(113004, 'spell_dru_intimidating_roar'),
(125047, 'spell_dru_glyph_of_the_treant'),
(5215, 'spell_dru_prowl'),
(106898, 'spell_dru_stampeding_roar'), (77761, 'spell_dru_stampeding_roar'), (77764, 'spell_dru_stampeding_roar'), (97993, 'spell_dru_stampeding_roar'),
(18960, 'spell_dru_teleport_moonglade'),
(5215, 'spell_dru_activate_cat_form'), (102547, 'spell_dru_activate_cat_form'), (102280, 'spell_dru_activate_cat_form'),
(145518, 'spell_dru_genesis'),
(102383, 'spell_dru_wild_charge_moonkin'),
(8921, 'spell_dru_shooting_stars_trigger'), (93402, 'spell_dru_shooting_stars_trigger'),
(93399, 'spell_dru_shooting_stars_proc'),
(150017, 'spell_dru_force_of_nature_rake'),
(22812, 'spell_dru_t16_guardian_2p_bonus'),
(774, 'spell_dru_glyph_of_rejuvenation'),
(110810, 'spell_dru_soul_swap'), (110810, 'spell_dru_activate_cat_form'),
(783, 'spell_dru_travel_form'),
(5225, 'spell_dru_glyph_of_the_predator'), (114280, 'spell_dru_glyph_of_the_predator'),
(107059, 'spell_dru_glyph_of_the_chameleon_fix'),
(768, 'spell_dru_glyph_of_the_chameleon'), (5487, 'spell_dru_glyph_of_the_chameleon'));
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
-- Balance
(79577, 'spell_dru_eclipse_remove'),
(112071, 'spell_dru_celestial_alignment'),
(8921, 'spell_dru_moonfire'),
(99, 'spell_dru_glyph_of_omens'), (339, 'spell_dru_glyph_of_omens'), (770, 'spell_dru_glyph_of_omens'), (5211, 'spell_dru_glyph_of_omens'),
(33786, 'spell_dru_glyph_of_omens'), (102355, 'spell_dru_glyph_of_omens'), (102359, 'spell_dru_glyph_of_omens'), (102793, 'spell_dru_glyph_of_omens'),
(132469, 'spell_dru_glyph_of_omens'),
(46832, 'spell_dru_sudden_eclipse'),
(16914, 'spell_dru_glyph_of_hurricane'), (106996, 'spell_dru_glyph_of_hurricane'),
(144767, 'spell_dru_t16_balance_2p_bonus'),
-- Feral
(127538, 'spell_dru_savage_roar'),
(52610, 'spell_dru_savage_roar_duration_check'), (127538, 'spell_dru_savage_roar_duration_check'),
(127538, 'spell_dru_savage_roar_glyphed'),
(5221, 'spell_dru_rip_duration'), (6785, 'spell_dru_rip_duration'), (33876, 'spell_dru_rip_duration'), (102545, 'spell_dru_rip_duration'),
(5221, 'spell_dru_shred'), (114236, 'spell_dru_shred'),
(5217, 'spell_dru_tigers_fury'),
(5217, 'spell_dru_glyph_of_shred'), (106951, 'spell_dru_glyph_of_shred'),
(16961, 'spell_dru_primal_fury'),
(62078, 'spell_dru_swipe'),
(131537, 'spell_dru_item_pvp_feral_4p'),
(81022, 'spell_dru_stampede_effect'),
(131538, 'spell_dru_stampede_timer'),
(135700, 'spell_dru_t16_feral_2p_bonus'),
(144841, 'spell_dru_t16_feral_4p_bonus'),
(146874, 'spell_dru_feral_rage'),
(5185, 'spell_dru_healing_touch_dream_of_cenarius'),
(5176, 'spell_dru_wrath_dream_of_cenarius'),
(108373, 'spell_dru_dream_of_cenarius'),
-- Guardian
(135597, 'spell_dru_tooth_and_claw_absorb'),
(6807, 'spell_dru_tooth_and_claw_trigger'),
(50334, 'spell_dru_berserk_bear'),
(5487, 'spell_dru_bear_form'),
(1178, 'spell_dru_bear_form_passive'),
(114237, 'spell_dru_glyph_of_fae_silence_cooldown'),
(138222, 'spell_dru_t15_guardian_4p_bonus'),
(770, 'spell_dru_glyph_of_fae_silence'), (102355, 'spell_dru_glyph_of_fae_silence'),
(6795, 'spell_dru_growl'),
-- Restoration
(81262, 'spell_dru_efflorescence'), (142423, 'spell_dru_efflorescence'),
(81269, 'spell_dru_efflorescence_heal'), (142424, 'spell_dru_efflorescence_heal'),
(48438, 'spell_dru_wild_growth'),
(77495, 'spell_dru_harmony'),
(8936, 'spell_dru_glyph_of_regrowth'),
(54825, 'spell_dru_glyph_of_healing_touch'),
(144871, 'spell_dru_sage_mender'),
(774, 'spell_dru_t15_restoration_4p_bonus'),
-- Talents
(108291, 'spell_dru_heart_of_the_wild'), (108292, 'spell_dru_heart_of_the_wild'), (108293, 'spell_dru_heart_of_the_wild'), (108294, 'spell_dru_heart_of_the_wild'),
(768, 'spell_dru_heart_of_the_wild_bonus'), (5487, 'spell_dru_heart_of_the_wild_bonus'),
(108300, 'spell_dru_killer_instinct'),
(768, 'spell_dru_killer_instinct_form'), (5487, 'spell_dru_killer_instinct_form'),
(124974, 'spell_dru_natures_vigil'),
(124991, 'spell_dru_natures_vigil_damage'),
(124988, 'spell_dru_natures_vigil_heal'),
-- Symbiosis
(110309, 'spell_dru_symbiosis_caster_aura'),
(110309, 'spell_dru_symbiosis'),
(110478, 'spell_dru_symbiosis_target_check'), (110479, 'spell_dru_symbiosis_target_check'), (110482, 'spell_dru_symbiosis_target_check'),
(110483, 'spell_dru_symbiosis_target_check'), (110484, 'spell_dru_symbiosis_target_check'), (110485, 'spell_dru_symbiosis_target_check'),
(110486, 'spell_dru_symbiosis_target_check'), (110488, 'spell_dru_symbiosis_target_check'), (110490, 'spell_dru_symbiosis_target_check'),
(110491, 'spell_dru_symbiosis_target_check'),
(126456, 'spell_dru_symbiosis_fortifying_brew'),
(110597, 'spell_dru_play_death'),
(122290, 'spell_dru_life_tap'),
(112970, 'spell_dru_demonic_circle_teleport'),
(112997, 'spell_dru_shattering_blow'),
(113004, 'spell_dru_intimidating_roar'),
-- Glyphs and utility
(125047, 'spell_dru_glyph_of_the_treant'),
(5215, 'spell_dru_prowl'),
(106898, 'spell_dru_stampeding_roar'), (77761, 'spell_dru_stampeding_roar'), (77764, 'spell_dru_stampeding_roar'), (97993, 'spell_dru_stampeding_roar'),
(18960, 'spell_dru_teleport_moonglade'),
(5215, 'spell_dru_activate_cat_form'), (102547, 'spell_dru_activate_cat_form'), (102280, 'spell_dru_activate_cat_form'),
-- Balance
(102383, 'spell_dru_wild_charge_moonkin'),
(8921, 'spell_dru_shooting_stars_trigger'), (93402, 'spell_dru_shooting_stars_trigger'),
(93399, 'spell_dru_shooting_stars_proc'),
-- Feral / Guardian
(150017, 'spell_dru_force_of_nature_rake'),
(22812, 'spell_dru_t16_guardian_2p_bonus'),
-- Restoration
(145518, 'spell_dru_genesis'),
(774, 'spell_dru_glyph_of_rejuvenation'),
-- Symbiosis
(110810, 'spell_dru_soul_swap'), (110810, 'spell_dru_activate_cat_form'),
-- Glyphs and forms
(783, 'spell_dru_travel_form'),
(5225, 'spell_dru_glyph_of_the_predator'), (114280, 'spell_dru_glyph_of_the_predator'),
(107059, 'spell_dru_glyph_of_the_chameleon_fix'),
(768, 'spell_dru_glyph_of_the_chameleon'), (5487, 'spell_dru_glyph_of_the_chameleon');
