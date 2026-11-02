-- Paladin spell scripts
DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
(85256, 'spell_pal_templar_s_verdict'),
(53385, 'spell_pal_divine_storm'));

DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
(65148, 'spell_pal_sacred_shield_absorb'),
(85673, 'spell_pal_word_of_glory'),
(136494, 'spell_pal_word_of_glory'),
(130551, 'spell_pal_word_of_glory_heal'),
(130552, 'spell_pal_harsh_word'),
(114163, 'spell_pal_eternal_flame'),
(85222, 'spell_pal_light_of_dawn'),
(84963, 'spell_pal_inquisition'),
(53385, 'spell_pal_divine_purpose'),
(53600, 'spell_pal_divine_purpose'),
(84963, 'spell_pal_divine_purpose'),
(85222, 'spell_pal_divine_purpose'),
(85256, 'spell_pal_divine_purpose'),
(85673, 'spell_pal_divine_purpose'),
(114163, 'spell_pal_divine_purpose'),
(136494, 'spell_pal_divine_purpose'),
(138165, 'spell_pal_divine_purpose'),
(53385, 'spell_pal_glyph_of_divine_storm'),
(59578, 'spell_pal_art_of_war'),
(879, 'spell_pal_exorcism'),
(122032, 'spell_pal_exorcism_glyphed'),
(35395, 'spell_pal_crusader_strike'),
(35395, 'spell_pal_t15_retribution_4p_bonus'),
(24275, 'spell_pal_sword_of_light_damage_bonus_holy'),
(42463, 'spell_pal_seal_of_truth_hotfix'),
(31803, 'spell_pal_censure_hotfix'),
(20164, 'spell_pal_seal_of_justice'),
(63225, 'spell_pal_glyph_of_inquisition'),
(119072, 'spell_pal_holy_wrath'),
(53600, 'spell_pal_shield_of_the_righteous'),
(498, 'spell_pal_divine_protection'),
(138250, 'spell_pal_t15_protection_4p_bonus'),
(149150, 'spell_pal_t16_protection_2p_bonus'),
(76669, 'spell_pal_mastery_illuminated_healing'),
(88819, 'spell_pal_daybreak'),
(121129, 'spell_pal_daybreak_heal'),
(19750, 'spell_pal_tower_of_radiance'),
(82326, 'spell_pal_tower_of_radiance'),
(19750, 'spell_pal_pvp_p14_holy'),
(93466, 'spell_pal_glyph_of_protector_of_the_innocent'),
(114165, 'spell_pal_holy_prism'),
(114852, 'spell_pal_holy_prism_effect'),
(114871, 'spell_pal_holy_prism_effect'),
(114157, 'spell_pal_execution_sentence'),
(114916, 'spell_pal_execution_sentence_effect'),
(114917, 'spell_pal_execution_sentence_effect'),
(633, 'spell_pal_lay_on_hands'),
(642, 'spell_pal_divine_shield'),
(1022, 'spell_pal_hand_of_protection'),
(114039, 'spell_pal_hand_of_purity'),
(121783, 'spell_pal_emancipate'),
(115750, 'spell_pal_blinding_light'),
(31821, 'spell_pal_glyph_of_devotion_aura'),
(64364, 'spell_pal_glyph_of_devotion_aura_trigger'),
(54943, 'spell_pal_glyph_of_blessed_life'),
(20066, 'spell_pal_censure_control_fix'),
(105421, 'spell_pal_censure_control_fix'),
(125043, 'spell_pal_glyph_of_contemplation'),
(54428, 'spell_pal_divine_plea'),
(26573, 'spell_pal_consecration'),
(116467, 'spell_pal_consecration'),
(88263, 'spell_pal_hammer_of_the_righteous'),
(86674, 'spell_pal_ancient_healer'),
(86698, 'spell_pal_ancient_crusader'),
(86703, 'spell_pal_ancient_crusader'),
(86698, 'spell_pal_guardian_of_ancient_kings'),
(54922, 'spell_pal_glyph_of_double_jeopardy'),
(31884, 'spell_pal_avenging_wrath'),
(114232, 'spell_pal_sanctified_wrath'),
(88821, 'spell_pal_daybreak_passive'),
(26023, 'spell_pal_pursuit_of_justice'),
(114918, 'spell_pal_lights_hammer'),
(119952, 'spell_pal_arcing_light_heal'),
(20165, 'spell_pal_seal_of_insight'),
(20167, 'spell_pal_seal_of_insight_heal'),
(13819, 'spell_pal_glyph_of_mounted_king'),
(23214, 'spell_pal_glyph_of_mounted_king'),
(34767, 'spell_pal_glyph_of_mounted_king'),
(34769, 'spell_pal_glyph_of_mounted_king'),
(66906, 'spell_pal_glyph_of_mounted_king'),
(69820, 'spell_pal_glyph_of_mounted_king'),
(69826, 'spell_pal_glyph_of_mounted_king'),
(73629, 'spell_pal_glyph_of_mounted_king'),
(73630, 'spell_pal_glyph_of_mounted_king'));
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(65148, 'spell_pal_sacred_shield_absorb'),
(85673, 'spell_pal_word_of_glory'),
(136494, 'spell_pal_word_of_glory'),
(130551, 'spell_pal_word_of_glory_heal'),
(130552, 'spell_pal_harsh_word'),
(114163, 'spell_pal_eternal_flame'),
(85222, 'spell_pal_light_of_dawn'),
(84963, 'spell_pal_inquisition'),
(53385, 'spell_pal_divine_purpose'),
(53600, 'spell_pal_divine_purpose'),
(84963, 'spell_pal_divine_purpose'),
(85222, 'spell_pal_divine_purpose'),
(85256, 'spell_pal_divine_purpose'),
(85673, 'spell_pal_divine_purpose'),
(114163, 'spell_pal_divine_purpose'),
(136494, 'spell_pal_divine_purpose'),
(138165, 'spell_pal_divine_purpose'),
(53385, 'spell_pal_glyph_of_divine_storm'),
(59578, 'spell_pal_art_of_war'),
(879, 'spell_pal_exorcism'),
(122032, 'spell_pal_exorcism_glyphed'),
(35395, 'spell_pal_crusader_strike'),
(35395, 'spell_pal_t15_retribution_4p_bonus'),
(24275, 'spell_pal_sword_of_light_damage_bonus_holy'),
(42463, 'spell_pal_seal_of_truth_hotfix'),
(31803, 'spell_pal_censure_hotfix'),
(20164, 'spell_pal_seal_of_justice'),
(63225, 'spell_pal_glyph_of_inquisition'),
(119072, 'spell_pal_holy_wrath'),
(53600, 'spell_pal_shield_of_the_righteous'),
(498, 'spell_pal_divine_protection'),
(138250, 'spell_pal_t15_protection_4p_bonus'),
(149150, 'spell_pal_t16_protection_2p_bonus'),
(76669, 'spell_pal_mastery_illuminated_healing'),
(88819, 'spell_pal_daybreak'),
(121129, 'spell_pal_daybreak_heal'),
(19750, 'spell_pal_tower_of_radiance'),
(82326, 'spell_pal_tower_of_radiance'),
(19750, 'spell_pal_pvp_p14_holy'),
(93466, 'spell_pal_glyph_of_protector_of_the_innocent'),
(114165, 'spell_pal_holy_prism'),
(114852, 'spell_pal_holy_prism_effect'),
(114871, 'spell_pal_holy_prism_effect'),
(114157, 'spell_pal_execution_sentence'),
(114916, 'spell_pal_execution_sentence_effect'),
(114917, 'spell_pal_execution_sentence_effect'),
(633, 'spell_pal_lay_on_hands'),
(642, 'spell_pal_divine_shield'),
(1022, 'spell_pal_hand_of_protection'),
(114039, 'spell_pal_hand_of_purity'),
(121783, 'spell_pal_emancipate'),
(115750, 'spell_pal_blinding_light'),
(31821, 'spell_pal_glyph_of_devotion_aura'),
(64364, 'spell_pal_glyph_of_devotion_aura_trigger'),
(54943, 'spell_pal_glyph_of_blessed_life'),
(20066, 'spell_pal_censure_control_fix'),
(105421, 'spell_pal_censure_control_fix'),
(125043, 'spell_pal_glyph_of_contemplation'),
(54428, 'spell_pal_divine_plea'),
(26573, 'spell_pal_consecration'),
(116467, 'spell_pal_consecration'),
(88263, 'spell_pal_hammer_of_the_righteous'),
(86674, 'spell_pal_ancient_healer'),
(86698, 'spell_pal_ancient_crusader'),
(86703, 'spell_pal_ancient_crusader'),
(86698, 'spell_pal_guardian_of_ancient_kings'),
(54922, 'spell_pal_glyph_of_double_jeopardy'),
(31884, 'spell_pal_avenging_wrath'),
(114232, 'spell_pal_sanctified_wrath'),
(88821, 'spell_pal_daybreak_passive'),
(26023, 'spell_pal_pursuit_of_justice'),
(114918, 'spell_pal_lights_hammer'),
(119952, 'spell_pal_arcing_light_heal'),
(20165, 'spell_pal_seal_of_insight'),
(20167, 'spell_pal_seal_of_insight_heal'),
(13819, 'spell_pal_glyph_of_mounted_king'),
(23214, 'spell_pal_glyph_of_mounted_king'),
(34767, 'spell_pal_glyph_of_mounted_king'),
(34769, 'spell_pal_glyph_of_mounted_king'),
(66906, 'spell_pal_glyph_of_mounted_king'),
(69820, 'spell_pal_glyph_of_mounted_king'),
(69826, 'spell_pal_glyph_of_mounted_king'),
(73629, 'spell_pal_glyph_of_mounted_king'),
(73630, 'spell_pal_glyph_of_mounted_king');

-- Proc conditions
DELETE FROM `spell_proc_event` WHERE `entry` IN (26135, 28789, 37189, 37195, 40470, 53576, 53651, 54922, 54926, 54927, 54937, 54943, 54957, 56414, 56420, 57954, 57955, 63222, 64882, 64890, 70756, 70761, 76669, 85043, 85416, 85804, 86674, 86698, 88819, 88821, 93466, 128863, 131633, 138169, 144586, 144613, 144625);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(26135, 0, 10, 8388608, 0, 0, 0, 0, 65536, 0, 0, 0),              -- Battlegear of Eternal Justice
(28789, 0, 10, 3221225472, 0, 0, 0, 0, 0, 0, 0, 0),               -- Holy Power
(37189, 0, 10, 3221225472, 0, 0, 0, 0, 2, 0, 0, 60000),           -- Reduced Holy Light Cast Time
(37195, 0, 10, 8388608, 0, 0, 0, 0, 262144, 0, 0, 0),             -- Judgment Group Heal
(40470, 0, 10, 3229614080, 0, 0, 0, 0, 262144, 0, 0, 0),          -- Paladin Tier 6 Trinket
(53576, 0, 10, 2097152, 65536, 0, 0, 0, 2, 0, 0, 0),              -- Infusion of Light
(53651, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0),                         -- Beacon of Light
(54922, 0, 10, 8388608, 0, 0, 0, 0, 0, 0, 0, 0),                  -- Glyph of Double Jeopardy
(54926, 0, 10, 0, 2, 8192, 0, 0, 0, 0, 0, 0),                     -- Glyph of Templar's Verdict
(54927, 0, 10, 0, 8192, 0, 0, 2048, 1, 0, 0, 0),                  -- Glyph of Avenging Wrath
(54937, 0, 10, 2097152, 65536, 0, 0, 0, 2, 0, 0, 0),              -- Glyph of Illumination
(54943, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0),                         -- Glyph of Blessed Life
(54957, 0, 10, 3221258240, 65536, 100943360, 50, 0, 0, 0, 0, 0),  -- Glyph of Flash of Light
(56414, 0, 10, 16384, 0, 0, 0, 0, 0, 0, 0, 0),                    -- Glyph of Dazing Shield
(56420, 0, 10, 2097152, 65536, 0, 0, 0, 0, 0, 0, 0),              -- Glyph of Denounce
(57954, 0, 10, 8388608, 128, 0, 0, 0, 2, 0, 0, 0),                -- Glyph of Fire From the Heavens
(57955, 0, 10, 1073741824, 0, 0, 0, 0, 0, 0, 0, 0),               -- Glyph of Flash of Light
(63222, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0),                        -- Glyph of the Alabaster Shield
(64882, 0, 10, 0, 1048576, 0, 0, 0, 0, 0, 0, 0),                  -- Item - Paladin T8 Protection 4P Bonus
(64890, 0, 10, 0, 65536, 0, 0, 0, 2, 0, 0, 0),                    -- Item - Paladin T8 Holy 2P Bonus
(70756, 0, 10, 2097152, 65536, 0, 0, 0, 0, 0, 0, 0),              -- Item - Paladin T10 Holy 4P Bonus
(70761, 0, 10, 0, 2147500032, 1, 0, 1024, 0, 0, 0, 0),            -- Item - Paladin T10 Protection 4P Bonus
(76669, 0, 0, 0, 0, 0, 0, 17408, 0, 0, 0, 0),                     -- Mastery: Illuminated Healing
(85043, 0, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0),                        -- Grand Crusader
(85416, 0, 10, 16384, 0, 0, 0, 0, 0, 0, 0, 0),                    -- Grand Crusader (reset)
(85804, 0, 0, 8388608, 0, 0, 0, 0, 0, 0, 0, 0),                   -- Selfless Healer
(86674, 0, 0, 0, 0, 0, 0, 17408, 0, 0, 0, 0),                     -- Ancient Healer
(86698, 0, 0, 0, 0, 0, 0, 69972, 3, 0, 100, 0),                   -- Guardian of Ancient Kings (Retribution)
(88819, 0, 10, 0, 65536, 0, 0, 0, 0, 0, 0, 0),                    -- Daybreak
(88821, 0, 10, 0, 0, 512, 0, 0, 0, 0, 0, 0),                      -- Daybreak
(93466, 0, 10, 0, 0, 33570816, 0, 0, 0, 0, 0, 0),                 -- Glyph of Protector of the Innocent
(128863, 0, 10, 1073741824, 0, 1536, 0, 0, 0, 0, 0, 0),           -- Selfless Healer
(131633, 0, 10, 0, 32768, 0, 0, 0, 0, 0, 0, 0),                   -- Item - Paladin PvP Set Retribution 2P Bonus
(138169, 0, 10, 0, 0, 8192, 0, 0, 3, 0, 0, 0),                    -- Item - Paladin T15 Retribution 4P Bonus
(144586, 0, 10, 0, 0, 2, 0, 0, 1, 0, 0, 0),                       -- Item - Paladin T16 Retribution 2P Bonus
(144613, 0, 10, 0, 256, 0, 0, 0, 1, 0, 0, 0),                     -- Item - Paladin T16 Holy 4P Bonus
(144625, 0, 10, 2097152, 65536, 0, 0, 0, 2, 0, 0, 0);             -- Item - Paladin T16 Holy 2P Bonus

-- Linked spells
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 53563 AND `spell_effect` = 53651 AND `type` = 2;
DELETE FROM `spell_linked_spell` WHERE (`spell_trigger`, `spell_effect`, `type`) IN (
(-114250, -128863, 0),
(-76669, -53651, 0),
(86669, 86674, 2),
(-86669, -86674, 0),
(122032, 147715, 0),
(24275, 141459, 0),
(-498, -138250, 0),
(-498, -149150, 0));
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-114250, -128863, 0, 'Selfless Healer - remove checker'),
(-76669, -53651, 0, 'Mastery: Illuminated Healing - remove Beacon of Light'),
(86669, 86674, 2, 'Guardian of Ancient Kings - Ancient Healer'),
(-86669, -86674, 0, 'Guardian of Ancient Kings - remove Ancient Healer'),
(122032, 147715, 0, 'Exorcism - secondary Exorcism'),
(24275, 141459, 0, 'Hammer of Wrath - secondary Hammer of Wrath'),
(-498, -138250, 0, 'Divine Protection - remove T15 Protection 4P'),
(-498, -149150, 0, 'Divine Protection - remove T16 Protection 2P');

-- Spell power coefficients
DELETE FROM `spell_bonus_data` WHERE `entry` IN (119072, 122032, 20271, 81297, 114916, 114917, 146585, 146586, 144581, 53652, 115536, 121129, 86678);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(119072, 0.91, -1, -1, -1, 'Paladin - Holy Wrath'),
(122032, -1, -1, 0.677, -1, 'Paladin - Exorcism (Glyph of Mass Exorcism)'),
(20271, 0, 0, 0.328, 0, 'Paladin - Judgment'),
(81297, -1, -1, 0.08, -1, 'Paladin - Consecration'),
(114916, 0, 0, 0, 0, 'Paladin - Execution Sentence'),
(114917, 0, 0, 0, 0, 'Paladin - Stay of Execution'),
(146585, 0, 0, 0, 0, 'Paladin - Execution Sentence (dispel)'),
(146586, 0, 0, 0, 0, 'Paladin - Stay of Execution (dispel)'),
(144581, 0, 0, 0, 0, 'Paladin - T16 Protection 2P heal'),
(53652, 0, 0, 0, 0, 'Paladin - Beacon of Light heal'),
(115536, 0, 0, 0, 0, 'Paladin - Glyph of Protector of the Innocent'),
(121129, 0, 0, 0, 0, 'Paladin - Daybreak'),
(86678, 0, 0, 0, 0, 'Paladin - Light of the Ancient Kings');

-- Blessing of Might shares the mastery buff group
DELETE FROM `spell_group` WHERE `id` = 1127 AND `spell_id` = 19740;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1127, 19740);

-- Class mounts teach riding
DELETE FROM `spell_learn_spell` WHERE (`entry`, `SpellID`) IN ((13819, 33388), (23214, 33391), (34767, 33391), (34769, 33388));
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES
(13819, 33388, 1),
(23214, 33391, 1),
(34767, 33391, 1),
(34769, 33388, 1);

-- Seal of Truth replaces Seal of Command
DELETE FROM `spell_ranks` WHERE `first_spell_id` = 105361;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `spell_rank`) VALUES
(105361, 105361, 1),
(105361, 31801, 2);

-- Guardian of Ancient Kings (Retribution)
DELETE FROM `creature_template_addon` WHERE `entry` = 46506;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(46506, 0, 0, 0, 0, 0, '86703');
UPDATE `creature_template` SET `ScriptName` = 'npc_pal_guardian_of_ancient_kings' WHERE `entry` = 46506;

-- Light's Hammer: periodic Arcing Light and cosmetic
UPDATE `creature_template` SET `spell1` = 114918, `spell2` = 122257 WHERE `entry` = 59738;
