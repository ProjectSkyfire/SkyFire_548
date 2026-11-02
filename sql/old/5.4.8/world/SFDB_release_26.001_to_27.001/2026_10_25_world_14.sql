-- Shaman spell scripts and spell data

-- Bindings without a script
DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
(6474, 'spell_sha_earthbind_totem'),
(8050, 'spell_sha_flame_shock'),
(16191, 'spell_sha_mana_tide'));

-- Spell script bindings
DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
(138305, 'spell_sha_ancestral_awakening'),
(108281, 'spell_sha_ancestral_guidance'),
(114911, 'spell_sha_ancestral_guidance_heal'),
(108285, 'spell_sha_call_of_the_elements'),
(108282, 'spell_sha_conductivity'),
(64695, 'spell_sha_earthgrab'),
(61882, 'spell_sha_earthquake'),
(77478, 'spell_sha_earthquake_tick'),
(117014, 'spell_sha_elemental_blast'),
(144964, 'spell_sha_flow_of_the_elements'),
(8042, 'spell_sha_fulmination'),
(51886, 'spell_sha_glyph_of_cleansing_waters'),
(77130, 'spell_sha_glyph_of_cleansing_waters'),
(55447, 'spell_sha_glyph_of_flame_shock'),
(2645, 'spell_sha_glyph_of_lakestrider'),
(370, 'spell_sha_glyph_of_purging'),
(30823, 'spell_sha_glyph_of_shamanistic_rage'),
(73920, 'spell_sha_healing_rain'),
(52042, 'spell_sha_healing_stream'),
(51514, 'spell_sha_hex'),
(138146, 'spell_sha_lightning_strike'),
(51505, 'spell_sha_lava_burst'),
(77451, 'spell_sha_lava_burst'),
(105792, 'spell_sha_lava_lash_spread_flame_shock'),
(51505, 'spell_sha_lava_surge_cooldown_fix'),
(53817, 'spell_sha_maelstrom_weapon'),
(113289, 'spell_sha_prowl'),
(16213, 'spell_sha_purification'),
(114083, 'spell_sha_restorative_mists'),
(16196, 'spell_sha_resurgence'),
(98020, 'spell_sha_spirit_link'),
(79206, 'spell_sha_spiritwalkers_grace'),
(114893, 'spell_sha_stone_bulwark'),
(114889, 'spell_sha_stone_bulwark_totem_passive'),
(17364, 'spell_sha_stormstrike'),
(51505, 'spell_sha_t15_elemental_4p_bonus'),
(17364, 'spell_sha_t15_enhancement_2p_bonus'),
(115356, 'spell_sha_t15_enhancement_2p_bonus'),
(144998, 'spell_sha_t16_elemental_2p_bonus'),
(144966, 'spell_sha_t16_enhancement_4p_bonus'),
(379, 'spell_sha_t16_restoration_2p_bonus'),
(108287, 'spell_sha_totemic_projection'),
(73680, 'spell_sha_unleash_elements'),
(73682, 'spell_sha_unleash_frost'),
(331, 'spell_sha_unleashed_fury_earthliving'),
(8004, 'spell_sha_unleashed_fury_earthliving'),
(61295, 'spell_sha_unleashed_fury_earthliving'),
(77472, 'spell_sha_unleashed_fury_earthliving'),
(126371, 'spell_sha_unleashed_fury_earthliving'),
(61295, 'spell_sha_unleashed_fury_earthliving_riptide'),
(118475, 'spell_sha_unleashed_fury_rockbiter'),
(114052, 'spell_sha_water_ascendant'),
(16246, 'spell_sha_clearcasting'),
(108283, 'spell_sha_echo_of_the_elements'),
(52007, 'spell_sha_earthliving_weapon'),
(34967, 'spell_sha_elemental_totem'),
(2062, 'spell_sha_elemental_totem_summon'),
(2894, 'spell_sha_elemental_totem_summon'),
(77223, 'spell_sha_enhanced_elements'),
(147781, 'spell_sha_glyph_of_eternal_earth'),
(58057, 'spell_sha_glyph_of_totemic_encirclement'),
(8178, 'spell_sha_grounding_totem_effect'),
(89523, 'spell_sha_grounding_totem_effect'),
(108270, 'spell_sha_hp_based_totem'),
(108280, 'spell_sha_hp_based_totem'),
(117013, 'spell_sha_primal_elementalist'),
(51527, 'spell_sha_static_shock'),
(120676, 'spell_sha_stormlash_totem'),
(33757, 'spell_sha_windfury_weapon'));
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(138305, 'spell_sha_ancestral_awakening'),
(108281, 'spell_sha_ancestral_guidance'),
(114911, 'spell_sha_ancestral_guidance_heal'),
(108285, 'spell_sha_call_of_the_elements'),
(108282, 'spell_sha_conductivity'),
(64695, 'spell_sha_earthgrab'),
(61882, 'spell_sha_earthquake'),
(77478, 'spell_sha_earthquake_tick'),
(117014, 'spell_sha_elemental_blast'),
(144964, 'spell_sha_flow_of_the_elements'),
(8042, 'spell_sha_fulmination'),
(51886, 'spell_sha_glyph_of_cleansing_waters'),
(77130, 'spell_sha_glyph_of_cleansing_waters'),
(55447, 'spell_sha_glyph_of_flame_shock'),
(2645, 'spell_sha_glyph_of_lakestrider'),
(370, 'spell_sha_glyph_of_purging'),
(30823, 'spell_sha_glyph_of_shamanistic_rage'),
(73920, 'spell_sha_healing_rain'),
(52042, 'spell_sha_healing_stream'),
(51514, 'spell_sha_hex'),
(138146, 'spell_sha_lightning_strike'),
(51505, 'spell_sha_lava_burst'),
(77451, 'spell_sha_lava_burst'),
(105792, 'spell_sha_lava_lash_spread_flame_shock'),
(51505, 'spell_sha_lava_surge_cooldown_fix'),
(53817, 'spell_sha_maelstrom_weapon'),
(113289, 'spell_sha_prowl'),
(16213, 'spell_sha_purification'),
(114083, 'spell_sha_restorative_mists'),
(16196, 'spell_sha_resurgence'),
(98020, 'spell_sha_spirit_link'),
(79206, 'spell_sha_spiritwalkers_grace'),
(114893, 'spell_sha_stone_bulwark'),
(114889, 'spell_sha_stone_bulwark_totem_passive'),
(17364, 'spell_sha_stormstrike'),
(51505, 'spell_sha_t15_elemental_4p_bonus'),
(17364, 'spell_sha_t15_enhancement_2p_bonus'),
(115356, 'spell_sha_t15_enhancement_2p_bonus'),
(144998, 'spell_sha_t16_elemental_2p_bonus'),
(144966, 'spell_sha_t16_enhancement_4p_bonus'),
(379, 'spell_sha_t16_restoration_2p_bonus'),
(108287, 'spell_sha_totemic_projection'),
(73680, 'spell_sha_unleash_elements'),
(73682, 'spell_sha_unleash_frost'),
(331, 'spell_sha_unleashed_fury_earthliving'),
(8004, 'spell_sha_unleashed_fury_earthliving'),
(61295, 'spell_sha_unleashed_fury_earthliving'),
(77472, 'spell_sha_unleashed_fury_earthliving'),
(126371, 'spell_sha_unleashed_fury_earthliving'),
(61295, 'spell_sha_unleashed_fury_earthliving_riptide'),
(118475, 'spell_sha_unleashed_fury_rockbiter'),
(114052, 'spell_sha_water_ascendant'),
(16246, 'spell_sha_clearcasting'),
(108283, 'spell_sha_echo_of_the_elements'),
(52007, 'spell_sha_earthliving_weapon'),
(34967, 'spell_sha_elemental_totem'),
(2062, 'spell_sha_elemental_totem_summon'),
(2894, 'spell_sha_elemental_totem_summon'),
(77223, 'spell_sha_enhanced_elements'),
(147781, 'spell_sha_glyph_of_eternal_earth'),
(58057, 'spell_sha_glyph_of_totemic_encirclement'),
(8178, 'spell_sha_grounding_totem_effect'),
(89523, 'spell_sha_grounding_totem_effect'),
(108270, 'spell_sha_hp_based_totem'),
(108280, 'spell_sha_hp_based_totem'),
(117013, 'spell_sha_primal_elementalist'),
(51527, 'spell_sha_static_shock'),
(120676, 'spell_sha_stormlash_totem'),
(33757, 'spell_sha_windfury_weapon');

-- Proc conditions (Cooldown in seconds)
DELETE FROM `spell_proc_event` WHERE `entry` IN (16164, 16196, 16213, 16246, 23551, 23572, 28823, 37227, 37237, 40463, 51527, 51558, 51564, 52007, 53390, 55440, 55447, 55453, 58057, 64928, 70807, 70811, 70817, 70830, 77756, 88764, 105764, 108282, 108283, 138145, 144962, 144966, 147781);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(16164, 28, 11, 2551190551, 2147758084, 67117060, 0, 65536, 2, 0, 100, 0),  -- Elemental Focus
(16196, 0, 11, 448, 0, 327696, 0, 17408, 2, 0, 0, 0),                     -- Resurgence
(16213, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),                                  -- Purification
(16246, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),                                  -- Clearcasting
(23551, 0, 11, 192, 0, 0, 0, 0, 0, 0, 0, 0),                               -- Lightning Shield (item)
(23572, 0, 11, 192, 0, 0, 0, 0, 0, 0, 0, 0),                               -- Mana Surge (item)
(28823, 0, 11, 192, 0, 0, 0, 0, 0, 0, 0, 0),                               -- Totemic Power
(37227, 0, 11, 448, 0, 0, 0, 0, 2, 0, 0, 60),                              -- Improved Healing Wave
(37237, 0, 11, 1, 0, 0, 0, 0, 2, 0, 0, 0),                                 -- Lightning Bolt Discount
(40463, 0, 11, 129, 16, 0, 0, 0, 0, 0, 0, 0),                              -- Shaman Tier 6 Trinket
(51527, 0, 11, 0, 0, 4100, 0, 0, 0, 0, 0, 0),                              -- Static Shock
(51558, 0, 11, 0, 0, 0, 0, 0, 2, 0, 0, 0),                                 -- Ancestral Awakening
(51564, 0, 0, 256, 0, 16, 0, 0, 0, 0, 0, 0),                               -- Tidal Waves
(52007, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),                                  -- Earthliving Weapon (Passive)
(53390, 11, 0, 192, 0, 65536, 0, 0, 0, 0, 0, 0),                           -- Tidal Waves (buff)
(55440, 0, 11, 64, 0, 0, 0, 0, 0, 0, 0, 0),                                -- Glyph of Healing Wave
(55447, 0, 11, 268435456, 0, 0, 0, 0, 0, 0, 0, 0),                         -- Glyph of Flame Shock
(55453, 8, 11, 1, 0, 0, 1, 0, 0, 0, 0, 0),                                 -- Glyph of Telluric Currents
(58057, 0, 11, 537432080, 0, 0, 0, 0, 0, 0, 0, 0),                         -- Glyph of Totemic Encirclement
(64928, 0, 11, 1, 0, 0, 0, 0, 2, 0, 0, 0),                                 -- Item - Shaman T8 Elemental 4P Bonus
(70807, 0, 11, 0, 0, 16, 0, 0, 0, 0, 100, 0),                              -- Item - Shaman T10 Restoration 2P Bonus
(70811, 0, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0),                                 -- Item - Shaman T10 Elemental 2P Bonus
(70817, 0, 11, 0, 4096, 0, 0, 65536, 0, 0, 0, 0),                          -- Item - Shaman T10 Elemental 4P Bonus
(70830, 0, 11, 0, 131072, 0, 0, 0, 0, 0, 0, 0),                            -- Item - Shaman T10 Enhancement 2P Bonus
(77756, 0, 11, 268435456, 0, 0, 0, 0, 0, 0, 0, 0),                         -- Lava Surge
(88764, 8, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0),                                 -- Rolling Thunder
(105764, 0, 11, 536870912, 0, 0, 0, 0, 0, 0, 0, 0),                        -- Item - Shaman T13 Restoration 2P Bonus
(108282, 0, 11, 1049027, 0, 69632, 4, 0, 0, 0, 0, 0),                      -- Conductivity
(108283, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),                                 -- Echo of the Elements
(138145, 0, 11, 3, 4, 0, 0, 0, 0, 0, 0, 0),                                -- Item - Shaman T15 Elemental 2P Bonus
(144962, 0, 11, 0, 0, 1024, 0, 1024, 3, 0, 0, 0),                          -- Item - Shaman T16 Enhancement 2P Bonus
(144966, 0, 11, 268435456, 0, 0, 0, 0, 0, 0, 0, 0),                        -- Item - Shaman T16 Enhancement 4P Bonus
(147781, 0, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0);                                -- Glyph of Eternal Earth

-- Elemental shields are exclusive per caster
DELETE FROM `spell_group` WHERE `id` = 1146 AND `spell_id` IN (324, 974, 52127);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1146, 324),
(1146, 974),
(1146, 52127);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1146;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(1146, 2);

-- Ancestral Swiftness passive haste, Glyph of Rain of Frogs
DELETE FROM `spell_learn_spell` WHERE (`entry`, `SpellID`) IN ((16188, 121617), (147707, 147709));
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES
(16188, 121617, 0),
(147707, 147709, 1);

-- Fulmination damage already includes the orb spell power bonus
DELETE FROM `spell_bonus_data` WHERE `entry` IN (58859, 88767);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(58859, -1, -1, 0.3, -1, 'Shaman - Spirit Wolf - Spirit Bite'),
(88767, 0, 0, 0, 0, 'Shaman - Fulmination');

-- Stormlash damage is calculated by the totem aura
DELETE FROM `spell_bonus_data` WHERE `entry` = 120687;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(120687, 0, 0, 0, 0, 'Shaman - Stormlash');

-- Elemental totems: the summon spell picks the normal or primal elemental
UPDATE `creature_template` SET `spell1` = 0 WHERE `entry` IN (15430, 15439);

-- Elementals inherit Elemental Fury and Mastery: Enhanced Elements
DELETE FROM `creature_template_addon` WHERE `entry` IN (15352, 15438, 61029, 61056);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(15352, 0, 0, 0, 0, 0, '34967'),
(15438, 0, 0, 0, 0, 0, '34967'),
(61029, 0, 0, 0, 0, 0, '34967'),
(61056, 0, 0, 0, 0, 0, '34967');

-- Capacitor Totem
UPDATE `creature_template` SET `ScriptName` = 'npc_capacitor_totem' WHERE `entry` = 61245;
