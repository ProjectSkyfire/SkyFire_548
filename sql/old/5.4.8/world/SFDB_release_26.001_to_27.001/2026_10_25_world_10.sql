-- Monk spell scripts and spell data

DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
    (117959, 'spell_monk_crackling_jade_lightning_knockback'),
    (147489, 'spell_monk_expel_harm'),
    (125195, 'spell_monk_tigereye_brew_stacks'),
    (126046, 'spell_monk_adaptation'),
    (122464, 'spell_monk_dematerialize'),
    (146953, 'spell_monk_glyph_of_fortuitous_spheres'),
    (146951, 'spell_monk_glyph_of_rapid_rolling'),
    (123393, 'spell_monk_glyph_of_breath_of_fire_fix'),
    (116023, 'spell_monk_sparring'),
    (116768, 'spell_monk_t16_windwalker_2p_bonus'),
    (118864, 'spell_monk_t16_windwalker_2p_bonus'),
    (145022, 'spell_monk_t16_windwalker_4p_bonus'),
    (145049, 'spell_monk_t16_brewmaster_2p_bonus'),
    (145055, 'spell_monk_t16_brewmaster_4p_bonus'),
    (145439, 'spell_monk_t16_mistweaver_2p_bonus'),
    (145449, 'spell_monk_t16_mistweaver_4p_bonus'),
    (108562, 'spell_monk_weapon_override_driver'),
    (125660, 'spell_monk_glyph_of_jab'),
    (121128, 'spell_monk_touch_of_death_expel_harm_notification_driver'),
    (125967, 'spell_monk_glyph_of_leer_of_the_ox'),
    (125084, 'spell_monk_charging_ox_wave_visual'),
    (121827, 'spell_monk_roll'),
    (107427, 'spell_monk_roll_aura'),
    (101545, 'spell_monk_restless_pursuit'),
    (124007, 'spell_monk_xuen_tiger_leap'),
    (130793, 'spell_monk_xuen_provoke')
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(117959, 'spell_monk_crackling_jade_lightning_knockback'),
(147489, 'spell_monk_expel_harm'),
(125195, 'spell_monk_tigereye_brew_stacks'),
(126046, 'spell_monk_adaptation'),
(122464, 'spell_monk_dematerialize'),
(146953, 'spell_monk_glyph_of_fortuitous_spheres'),
(146951, 'spell_monk_glyph_of_rapid_rolling'),
(123393, 'spell_monk_glyph_of_breath_of_fire_fix'),
(116023, 'spell_monk_sparring'),
(116768, 'spell_monk_t16_windwalker_2p_bonus'),
(118864, 'spell_monk_t16_windwalker_2p_bonus'),
(145022, 'spell_monk_t16_windwalker_4p_bonus'),
(145049, 'spell_monk_t16_brewmaster_2p_bonus'),
(145055, 'spell_monk_t16_brewmaster_4p_bonus'),
(145439, 'spell_monk_t16_mistweaver_2p_bonus'),
(145449, 'spell_monk_t16_mistweaver_4p_bonus'),
(108562, 'spell_monk_weapon_override_driver'),
(125660, 'spell_monk_glyph_of_jab'),
(121128, 'spell_monk_touch_of_death_expel_harm_notification_driver'),
(125967, 'spell_monk_glyph_of_leer_of_the_ox'),
(125084, 'spell_monk_charging_ox_wave_visual'),
(121827, 'spell_monk_roll'),
(107427, 'spell_monk_roll_aura'),
(101545, 'spell_monk_restless_pursuit'),
(124007, 'spell_monk_xuen_tiger_leap'),
(130793, 'spell_monk_xuen_provoke');

-- Xuen: Tiger Leap autocast
UPDATE `creature_template` SET `spell1`=124007 WHERE `entry`=63508;

-- Glyph of Flying Serpent Kick: area trigger that ends the kick on the first enemy flown into
DELETE FROM `spell_areatrigger_template` WHERE `Entry`=388;
INSERT INTO `spell_areatrigger_template` (`Entry`, `Flags`, `CollisionType`, `Radius`, `ScaleX`, `ScaleY`, `ScriptName`) VALUES
(388, 0, 0, 8, 0, 0, 'sat_monk_glyph_of_flying_serpent_kick');

-- Swift Reflexes: counterattack on parry, at most once per second
DELETE FROM `spell_proc_event` WHERE `entry`=124334;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(124334, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 1);

-- Attack power coefficients; -1 rows keep weapon-damage spells from picking up a default coefficient
DELETE FROM `spell_bonus_data` WHERE `entry` IN (100780, 100787, 107270, 108557, 115181, 115464, 115687, 115693, 115695, 115698, 117640, 117952, 117993, 118022, 122278, 123586, 123725, 123996, 124040, 124041, 124081, 124098, 124101, 124335, 124507, 125033, 130654, 132463, 132467, 135914, 135920, 148135, 148187, 149112);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(100780, -1, -1, -1, -1, 'Monk - Jab'),
(100787, -1, -1, -1, -1, 'Monk - Tiger Palm'),
(107270, -1, -1, -1, -1, 'Monk - Spinning Crane Kick'),
(108557, -1, -1, -1, -1, 'Monk - Jab (Staff)'),
(115181, -1, -1, 0.36, -1, 'Monk - Breath of Fire'),
(115464, -1, -1, 0.75, -1, 'Monk - Healing Sphere'),
(115687, -1, -1, -1, -1, 'Monk - Jab (Axes)'),
(115693, -1, -1, -1, -1, 'Monk - Jab (Maces)'),
(115695, -1, -1, -1, -1, 'Monk - Jab (Swords)'),
(115698, -1, -1, -1, -1, 'Monk - Jab (Polearm)'),
(117640, -1, -1, 0.048, -1, 'Monk - Spinning Crane Kick (heal)'),
(117952, -1, -1, -1, 0.386, 'Monk - Crackling Jade Lightning'),
(117993, -1, -1, 0.556, -1, 'Monk - Chi Torpedo (damage)'),
(118022, -1, -1, 1.1, -1, 'Monk - Dizzying Haze'),
(122278, -1, -1, -1, -1, 'Monk - Dampen Harm'),
(123586, -1, -1, 0.45, -1, 'Monk - Flying Serpent Kick'),
(123725, -1, -1, -1, 0.162, 'Monk - Breath of Fire (periodic)'),
(123996, -1, -1, 0.505, -1, 'Monk - Crackling Tiger Lightning'),
(124040, -1, -1, 0.385, -1, 'Monk - Chi Torpedo (heal)'),
(124041, -1, -1, 0.352, -1, 'Monk - Gift of the Serpent'),
(124081, -1, -1, -1, 0.09, 'Monk - Zen Sphere (heal)'),
(124098, -1, -1, 0.09, -1, 'Monk - Zen Sphere (damage)'),
(124101, -1, -1, 0.234, -1, 'Monk - Zen Sphere: Detonate (heal)'),
(124335, -1, -1, -1, -1, 'Monk - Swift Reflexes'),
(124507, -1, -1, 0.5025, -1, 'Monk - Gift of the Ox'),
(125033, -1, -1, 0.368, -1, 'Monk - Zen Sphere: Detonate (damage)'),
(130654, -1, -1, 1, -1, 'Monk - Chi Burst (heal)'),
(132463, -1, -1, 0.45, -1, 'Monk - Chi Wave (heal)'),
(132467, -1, -1, 0.45, -1, 'Monk - Chi Wave (damage)'),
(135914, -1, -1, 0.375, -1, 'Monk - Healing Sphere (expired)'),
(135920, -1, -1, 0.176, -1, 'Monk - Gift of the Serpent (expired)'),
(148135, -1, -1, 1.21, -1, 'Monk - Chi Burst (damage)'),
(148187, -1, -1, -1, -1, 'Monk - Rushing Jade Wind'),
(149112, -1, -1, 0.45, -1, 'Monk - Mist Wave');

-- Stagger levels are mutually exclusive
DELETE FROM `spell_group` WHERE `id`=1132 AND `spell_id` IN (124273, 124274, 124275);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1132, 124273),
(1132, 124274),
(1132, 124275);
DELETE FROM `spell_group_stack_rules` WHERE `group_id`=1132;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(1132, 1);

-- Legacy of the White Tiger doesn't stack with other critical strike raid buffs
DELETE FROM `spell_group` WHERE `id`=1141 AND `spell_id`=116781;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1141, 116781);
