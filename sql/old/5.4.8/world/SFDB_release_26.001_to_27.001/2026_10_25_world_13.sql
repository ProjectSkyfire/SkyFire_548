-- Rogue spell scripts and spell data

-- Spell script bindings
DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
(32645, 'spell_rog_envenom'),
(1943, 'spell_rog_t15_2p_bonus'),
(5171, 'spell_rog_t15_2p_bonus'),
(32645, 'spell_rog_t15_2p_bonus'),
(73651, 'spell_rog_t15_2p_bonus'),
(5171, 'spell_rog_duration_check'),
(73651, 'spell_rog_duration_check'),
(5938, 'spell_rog_shiv'),
(2823, 'spell_rog_poison_proc'),
(3408, 'spell_rog_poison_proc'),
(5761, 'spell_rog_poison_proc'),
(8679, 'spell_rog_poison_proc'),
(108211, 'spell_rog_poison_proc'),
(108215, 'spell_rog_poison_proc'),
(108215, 'spell_rog_paralytic_poison'),
(112961, 'spell_rog_leeching_poison'),
(137619, 'spell_rog_marked_for_death'),
(36563, 'spell_rog_killing_spree_teleport'),
(57840, 'spell_rog_killing_spree_teleport'),
(76806, 'spell_rog_main_gauche'),
(114014, 'spell_rog_shuriken_toss'),
(137586, 'spell_rog_shuriken_toss_aura'),
(14190, 'spell_rog_seal_fate'),
(1329, 'spell_rog_mutilate'),
(121471, 'spell_rog_shadow_blades'),
(26679, 'spell_rog_deadly_throw'),
(74001, 'spell_rog_combat_readiness'),
(8647, 'spell_rog_glyph_of_expose_armor'),
(63254, 'spell_rog_glyph_of_deadly_momentum'),
(921, 'spell_rog_glyph_of_disguise'),
(1766, 'spell_rog_glyph_of_kick'),
(145185, 'spell_rog_t16_2p_bonus'),
(145193, 'spell_rog_silent_blades'),
(145210, 'spell_rog_t16_4p_bonus'),
(57841, 'spell_rog_t16_4p_bonus_killing_spree'),
(57842, 'spell_rog_t16_4p_bonus_killing_spree'),
(79140, 'spell_rog_vendetta'),
(84601, 'spell_rog_assassins_resolve'),
(113613, 'spell_rog_symbiosis_growl'));
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(32645, 'spell_rog_envenom'),
(1943, 'spell_rog_t15_2p_bonus'),
(5171, 'spell_rog_t15_2p_bonus'),
(32645, 'spell_rog_t15_2p_bonus'),
(73651, 'spell_rog_t15_2p_bonus'),
(5171, 'spell_rog_duration_check'),
(73651, 'spell_rog_duration_check'),
(5938, 'spell_rog_shiv'),
(2823, 'spell_rog_poison_proc'),
(3408, 'spell_rog_poison_proc'),
(5761, 'spell_rog_poison_proc'),
(8679, 'spell_rog_poison_proc'),
(108211, 'spell_rog_poison_proc'),
(108215, 'spell_rog_poison_proc'),
(108215, 'spell_rog_paralytic_poison'),
(112961, 'spell_rog_leeching_poison'),
(137619, 'spell_rog_marked_for_death'),
(36563, 'spell_rog_killing_spree_teleport'),
(57840, 'spell_rog_killing_spree_teleport'),
(76806, 'spell_rog_main_gauche'),
(114014, 'spell_rog_shuriken_toss'),
(137586, 'spell_rog_shuriken_toss_aura'),
(14190, 'spell_rog_seal_fate'),
(1329, 'spell_rog_mutilate'),
(121471, 'spell_rog_shadow_blades'),
(26679, 'spell_rog_deadly_throw'),
(74001, 'spell_rog_combat_readiness'),
(8647, 'spell_rog_glyph_of_expose_armor'),
(63254, 'spell_rog_glyph_of_deadly_momentum'),
(921, 'spell_rog_glyph_of_disguise'),
(1766, 'spell_rog_glyph_of_kick'),
(145185, 'spell_rog_t16_2p_bonus'),
(145193, 'spell_rog_silent_blades'),
(145210, 'spell_rog_t16_4p_bonus'),
(57841, 'spell_rog_t16_4p_bonus_killing_spree'),
(57842, 'spell_rog_t16_4p_bonus_killing_spree'),
(79140, 'spell_rog_vendetta'),
(84601, 'spell_rog_assassins_resolve'),
(113613, 'spell_rog_symbiosis_growl');

-- Proc rules
-- Silent Blades and Sleight of Hand use a client proc flag the core does not raise, so their flags are set explicitly
DELETE FROM `spell_proc_event` WHERE `entry` IN (14190, 137586, 145185, 145193, 145211, 28812, 37165, 37168, 37173, 64914, 67209, 70803, 70805);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(14190, 0, 8, 512, 6, 2304, 0, 0, 2, 0, 0, 0),                   -- Seal Fate
(137586, 0, 0, 0, 0, 0, 0, 16, 3, 0, 100, 0),                    -- Shuriken Toss (ranged auto attack)
(145185, 0, 8, 0, 0, 8388608, 0, 0, 3, 0, 0, 0),                 -- Item - Rogue T16 2P Bonus
(145193, 0, 8, 34080518, 270794752, 2304, 0, 69904, 0, 0, 0, 0), -- Silent Blades
(145211, 0, 8, 512, 0, 0, 0, 16, 0, 0, 0, 0),                    -- Sleight of Hand
(28812, 0, 8, 33554438, 0, 0, 0, 0, 2, 0, 0, 0),                 -- Head Rush
(37165, 0, 8, 2098176, 0, 0, 0, 0, 0, 0, 0, 0),                  -- Haste
(37168, 0, 8, 4063232, 9, 0, 0, 0, 0, 0, 0, 0),                  -- Finisher Combo
(37173, 0, 8, 750519704, 262, 0, 0, 0, 0, 0, 0, 30000),          -- Armor Penetration
(64914, 0, 8, 65536, 0, 0, 0, 0, 0, 0, 0, 0),                    -- Item - Rogue T8 2P Bonus
(67209, 1, 8, 1048576, 0, 0, 0, 327680, 0, 0, 0, 0),             -- Item - Rogue T9 2P Bonus (Rupture)
(70803, 0, 8, 4063232, 8, 0, 0, 0, 0, 0, 0, 0),                  -- Item - Rogue T10 4P Bonus
(70805, 0, 8, 0, 131072, 0, 0, 0, 0, 0, 0, 0);                   -- Item - Rogue T10 2P Bonus

-- Spell bonus coefficients
DELETE FROM `spell_bonus_data` WHERE `entry` IN (113780, 114014, 121411, 121733, 26679, 51723, 79136);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(113780, -1, -1, 0.10897, -1, 'Rogue - Deadly Poison (direct damage)'),
(114014, -1, -1, 0.6, -1, 'Rogue - Shuriken Toss'),
(121411, -1, -1, 0.0275, -1, 'Rogue - Crimson Tempest'),
(121733, -1, -1, 0.05, -1, 'Rogue - Throw'),
(26679, -1, -1, 0.12, -1, 'Rogue - Deadly Throw'),
(51723, -1, -1, 0.175, -1, 'Rogue - Fan of Knives'),
(79136, -1, -1, 0.16, -1, 'Rogue - Venomous Wound');

-- Master of Subtlety: unlearning the talent drops the damage bonus
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = -31223 AND `spell_effect` = -31665;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-31223, -31665, 0, 'Master of Subtlety');

-- Glyph of Detection
DELETE FROM `spell_learn_spell` WHERE `entry` = 125044 AND `SpellID` = 56814;
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES
(125044, 56814, 1);

-- Rogue crowd control effects of the same type replace each other
DELETE FROM `spell_group` WHERE `id` = 1144;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1144, 408),
(1144, 1776),
(1144, 1833),
(1144, 2094),
(1144, 6770);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1144;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(1144, 3);
