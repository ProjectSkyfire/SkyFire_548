-- Priest spell scripts
DELETE FROM `spell_script_names` WHERE (`spell_id`=527 AND `ScriptName`='spell_pri_dispel_magic') OR (`spell_id`=64129 AND `ScriptName`='spell_pri_body_and_soul');

DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
'spell_pri_shadow_word_death_glyphed','spell_pri_glyph_of_shadow','spell_pri_shadow_orbs','spell_pri_shadowy_apparitions',
'spell_pri_t15_shadow_4p_bonus','spell_pri_shadowy_apparition','spell_pri_t15_shadow_2p_bonus','spell_pri_devouring_plague',
'spell_pri_devouring_plague_shadowy_recall','spell_pri_shadowy_recall','spell_pri_mind_flay_selector','spell_pri_mind_flay',
'spell_pri_mind_flay_insanity','spell_pri_mind_flay_shadowy_recall','spell_pri_mind_spike','spell_pri_mind_sear_check',
'spell_pri_psychic_horror','spell_pri_divine_insight_talent','spell_pri_divine_insight_shadow','spell_pri_divine_insight_holy',
'spell_pri_from_darkness_comes_light','spell_pri_surge_of_light_consumer','spell_pri_surge_of','spell_pri_twist_of_fate',
'spell_pri_shadowfiend','spell_pri_void_tendrils','spell_pri_void_tendrils_grasp','spell_pri_atonement','spell_pri_atonement_heal',
'spell_pri_spirit_shell','spell_pri_archangel','spell_pri_strength_of_soul','spell_pri_power_word_solace',
'spell_pri_solace_and_insanity','spell_pri_train_of_thought','spell_pri_guardian_spirit','spell_pri_angelic_bulwark',
'spell_pri_focused_will','spell_pri_phantasm_proc','spell_pri_echo_of_light','spell_pri_holy_word_sanctuary',
'spell_pri_chakra_serenity','spell_pri_chakra_chastise','spell_pri_binding_heal','spell_pri_prayer_of_mending_proc',
'spell_pri_levitate','spell_pri_glyph_of_heavens','spell_pri_glyph_of_levitate','spell_pri_mass_dispel','spell_pri_fear_ward',
'spell_pri_glyph_of_purify','spell_pri_glyph_of_dispel_magic','spell_pri_glyph_of_smite','spell_pri_glyph_of_inner_sanctum',
'spell_pri_glyph_of_inspired_hymns','spell_pri_item_s12_2p_heal','spell_pri_holy_spark','spell_pri_item_s12_2p_shadow',
'spell_pri_t15_healer_4p_bonus','spell_pri_t16_healer_2p_bonus','spell_pri_t16_healer_4p_bonus','spell_pri_t16_shadow_4p_bonus',
'spell_pri_lightwell_charges','spell_pri_mind_blast','spell_pri_glyph_of_mind_spike','spell_pri_psychic_scream',
'spell_pri_psychic_terror','spell_pri_void_shift','spell_pri_spectral_guise','spell_pri_spectral_guise_remove',
'spell_pri_spectral_guise_stacks','spell_pri_angelic_feather','spell_pri_halo','spell_pri_halo_visual','spell_pri_halo_effect',
'spell_pri_cascade','spell_pri_cascade_missile','spell_pri_cascade_selector','spell_pri_holy_word_sanctuary_heal',
'spell_pri_absolution','spell_pri_divine_star','spell_pri_divine_star_dummy','spell_pri_divine_star_damage_heal');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
-- Shadow
(129176, 'spell_pri_shadow_word_death_glyphed'),
(107906, 'spell_pri_glyph_of_shadow'),
(95740,  'spell_pri_shadow_orbs'),
(78203,  'spell_pri_shadowy_apparitions'),
(138158, 'spell_pri_t15_shadow_4p_bonus'),
(147193, 'spell_pri_shadowy_apparition'),
(148859, 'spell_pri_t15_shadow_2p_bonus'),
(2944,   'spell_pri_devouring_plague'),
(124467, 'spell_pri_devouring_plague_shadowy_recall'),
(77486,  'spell_pri_shadowy_recall'),
(15407,  'spell_pri_mind_flay_selector'),
(15407,  'spell_pri_mind_flay'),
(129197, 'spell_pri_mind_flay'),
(129197, 'spell_pri_mind_flay_insanity'),
(124468, 'spell_pri_mind_flay_shadowy_recall'),
(73510,  'spell_pri_mind_spike'),
(48045,  'spell_pri_mind_sear_check'),
(64044,  'spell_pri_psychic_horror'),
(8092,   'spell_pri_mind_blast'),
(33371,  'spell_pri_glyph_of_mind_spike'),
(8122,   'spell_pri_psychic_scream'),
(113792, 'spell_pri_psychic_terror'),
-- Talents
(109175, 'spell_pri_divine_insight_talent'),
(124430, 'spell_pri_divine_insight_shadow'),
(596,    'spell_pri_divine_insight_holy'),
(2060,   'spell_pri_divine_insight_holy'),
(109186, 'spell_pri_from_darkness_comes_light'),
(2061,   'spell_pri_surge_of_light_consumer'),
(87160,  'spell_pri_surge_of'),
(114255, 'spell_pri_surge_of'),
(109142, 'spell_pri_twist_of_fate'),
(34433,  'spell_pri_shadowfiend'),
(123040, 'spell_pri_shadowfiend'),
(108920, 'spell_pri_void_tendrils'),
(114404, 'spell_pri_void_tendrils_grasp'),
(108968, 'spell_pri_void_shift'),
(142723, 'spell_pri_void_shift'),
(112833, 'spell_pri_spectral_guise'),
(119032, 'spell_pri_spectral_guise_remove'),
(119030, 'spell_pri_spectral_guise_stacks'),
(121536, 'spell_pri_angelic_feather'),
(120517, 'spell_pri_halo'),
(120644, 'spell_pri_halo'),
(120630, 'spell_pri_halo_visual'),
(120643, 'spell_pri_halo_visual'),
(120692, 'spell_pri_halo_effect'),
(120696, 'spell_pri_halo_effect'),
(121135, 'spell_pri_cascade'),
(127632, 'spell_pri_cascade'),
(121146, 'spell_pri_cascade_missile'),
(127627, 'spell_pri_cascade_missile'),
(120785, 'spell_pri_cascade_missile'),
(127628, 'spell_pri_cascade_missile'),
(120786, 'spell_pri_cascade_selector'),
(127630, 'spell_pri_cascade_selector'),
(110744, 'spell_pri_divine_star'),
(122121, 'spell_pri_divine_star'),
(58880,  'spell_pri_divine_star_dummy'),
(122127, 'spell_pri_divine_star_dummy'),
(110745, 'spell_pri_divine_star_damage_heal'),
(122128, 'spell_pri_divine_star_damage_heal'),
-- Discipline
(81749,  'spell_pri_atonement'),
(81751,  'spell_pri_atonement_heal'),
(94472,  'spell_pri_atonement_heal'),
(140815, 'spell_pri_atonement_heal'),
(140816, 'spell_pri_atonement_heal'),
(596,    'spell_pri_spirit_shell'),
(2050,   'spell_pri_spirit_shell'),
(2060,   'spell_pri_spirit_shell'),
(2061,   'spell_pri_spirit_shell'),
(81700,  'spell_pri_archangel'),
(2050,   'spell_pri_strength_of_soul'),
(2060,   'spell_pri_strength_of_soul'),
(2061,   'spell_pri_strength_of_soul'),
(129250, 'spell_pri_power_word_solace'),
(139139, 'spell_pri_solace_and_insanity'),
(92297,  'spell_pri_train_of_thought'),
(47788,  'spell_pri_guardian_spirit'),
(108945, 'spell_pri_angelic_bulwark'),
(45243,  'spell_pri_focused_will'),
(114239, 'spell_pri_phantasm_proc'),
-- Holy
(77485,  'spell_pri_echo_of_light'),
(88685,  'spell_pri_holy_word_sanctuary'),
(88686,  'spell_pri_holy_word_sanctuary_heal'),
(81208,  'spell_pri_chakra_serenity'),
(81209,  'spell_pri_chakra_chastise'),
(32546,  'spell_pri_binding_heal'),
(41635,  'spell_pri_prayer_of_mending_proc'),
(59907,  'spell_pri_lightwell_charges'),
-- Utility and glyphs
(1706,   'spell_pri_levitate'),
(111758, 'spell_pri_glyph_of_heavens'),
(111758, 'spell_pri_glyph_of_levitate'),
(32592,  'spell_pri_mass_dispel'),
(6346,   'spell_pri_fear_ward'),
(527,    'spell_pri_glyph_of_purify'),
(528,    'spell_pri_glyph_of_dispel_magic'),
(585,    'spell_pri_glyph_of_smite'),
(588,    'spell_pri_glyph_of_inner_sanctum'),
(64843,  'spell_pri_glyph_of_inspired_hymns'),
(64901,  'spell_pri_glyph_of_inspired_hymns'),
-- Set bonuses
(33076,  'spell_pri_item_s12_2p_heal'),
(131567, 'spell_pri_holy_spark'),
(47585,  'spell_pri_item_s12_2p_shadow'),
(138301, 'spell_pri_t15_healer_4p_bonus'),
(145306, 'spell_pri_t16_healer_2p_bonus'),
(145334, 'spell_pri_t16_healer_4p_bonus'),
(145336, 'spell_pri_absolution'),
(145179, 'spell_pri_t16_shadow_4p_bonus');

DELETE FROM `spell_proc_event` WHERE `entry` IN (78203,138158,77485,81749,81662,81208,81209,92297,108942,109175,109186,47517,52798,63733,139139,131567,145179,145306,145334,138301,7001,126154,33371,145336);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(78203,  0, 6, 0x00008000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0, 0,   0), -- Shadowy Apparitions (crit SW:P ticks)
(138158, 0, 6, 0x00000000, 0x00000400, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Item - Priest T15 Shadow 4P Bonus
(77485,  0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00004400, 0x00000000, 0, 0,   0), -- Mastery: Echo of Light
(81749,  2, 6, 0x00100080, 0x00008000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Atonement
(81662,  0, 6, 0x00100080, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Evangelism
(81208,  0, 6, 0x00801C00, 0x00000004, 0x00008000, 0x00000008, 0x00000000, 0x00000000, 0, 0,   0), -- Chakra: Serenity
(81209,  0, 6, 0x00000080, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Chakra: Chastise
(92297,  0, 6, 0x00001080, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Train of Thought
(108942, 0, 6, 0x00004000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Phantasm
(109175, 0, 6, 0x00008000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 5,   0), -- Divine Insight (Shadow)
(109186, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000003, 0, 0,   0), -- From Darkness, Comes Light
(47517,  0, 6, 0x00001C00, 0x00010000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Grace
(52798,  0, 6, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Borrowed Time
(63733,  0, 6, 0x00000800, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Serendipity
(139139, 0, 6, 0x00100000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Solace and Insanity
(131567, 0, 6, 0x00801800, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Holy Spark
(145179, 0, 6, 0x00000000, 0x00000000, 0x00000000, 0x00000010, 0x00000000, 0x00000000, 0, 0,   0), -- Item - Priest T16 Shadow 4P Bonus
(145306, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0, 0,   0), -- Item - Priest T16 Healer 2P Bonus
(145334, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000003, 0, 0,   0), -- Item - Priest T16 Healer 4P Bonus
(138301, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 100, 0), -- Item - Priest T15 Healer 4P Bonus
(7001,   0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Lightwell Renew
(126154, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Lightspring Renew
(33371,  0, 6, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0x00000000, 0x00000000, 0, 0,   0), -- Glyph of Mind Spike
(145336, 0, 6, 0x00800000, 0x00000000, 0x00000020, 0x00000000, 0x00014000, 0x00000003, 0, 0,   0); -- Absolution (Holy Word: Serenity, Holy Word: Chastise)

DELETE FROM `spell_linked_spell` WHERE (`spell_trigger`=-14771 AND `spell_effect`=-91724 AND `type`=0) OR (`spell_trigger`=-588 AND `spell_effect`=-91724 AND `type`=0) OR (`spell_trigger`=89485 AND `spell_effect`=96267 AND `type`=0) OR (`spell_trigger`=132157 AND `spell_effect`=23455 AND `type`=0);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-14771, -91724, 0, 'Inner Will removal - remove Spell Warding'),
(-588,   -91724, 0, 'Inner Fire removal - remove Spell Warding'),
(89485,  96267,  0, 'Inner Focus - Strength of Soul immunity'),
(132157, 23455,  0, 'Holy Nova - Holy Nova heal');

DELETE FROM `spell_group` WHERE `id`=1130;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1130, 17),
(1130, 123258);
DELETE FROM `spell_group_stack_rules` WHERE `group_id`=1130;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(1130, 1);

DELETE FROM `spell_bonus_data` WHERE `entry` IN (123258,81751,94472,140815,140816,77489);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(123258, 0.8068, -1, 0, 0, 'Priest - Power Word: Shield'),
(81751,  0,      0,  0, 0, 'Priest - Atonement'),
(94472,  0,      0,  0, 0, 'Priest - Atonement (crit)'),
(140815, 0,      0,  0, 0, 'Priest - Atonement'),
(140816, 0,      0,  0, 0, 'Priest - Atonement (crit)'),
(77489,  0,      0,  0, 0, 'Priest - Echo of Light');

UPDATE `creature_template` SET `ScriptName`='npc_pet_pri_lightspring' WHERE `entry`=64571;
UPDATE `creature_template` SET `ScriptName`='npc_pet_pri_psyfiend' WHERE `entry`=59190;
UPDATE `creature_template` SET `ScriptName`='', `AIName`='NullCreatureAI' WHERE `entry`=59607;

DELETE FROM `spell_areatrigger_template` WHERE `Entry` IN (337,657,658,1489);
INSERT INTO `spell_areatrigger_template` (`Entry`, `Flags`, `CollisionType`, `Radius`, `ScaleX`, `ScaleY`, `ScriptName`) VALUES
(337,  0, 0, 3,   3, 3, 'sat_pri_angelic_feather'),
(657,  0, 0, 35,  0, 0, 'sat_pri_halo'),
(658,  0, 0, 35,  0, 0, 'sat_pri_halo'),
(1489, 0, 1, 6.5, 0, 0, 'sat_pri_power_word_barrier');
