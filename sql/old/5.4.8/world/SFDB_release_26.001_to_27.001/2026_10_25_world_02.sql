-- Monk Brewmaster, Windwalker and Mistweaver spell scripts

DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_monk_teachings_of_the_monastery',
    'spell_monk_mana_tea_stacks',
    'spell_monk_tigereye_brew_stacks',
    'spell_monk_healing_elixirs',
    'spell_monk_avert_harm',
    'spell_monk_leer_of_the_ox',
    'spell_monk_nimble_brew',
    'spell_monk_chi_wave',
    'spell_monk_chi_wave_bolt',
    'spell_monk_chi_wave_target_selector',
    'spell_monk_chi_brew',
    'spell_monk_brewing_tigereye_brew',
    'spell_monk_brewing_mana_tea',
    'spell_monk_healing_elixirs_active',
    'spell_monk_healing_elixirs_heal',
    'spell_monk_desperate_measures',
    'spell_monk_charging_ox_wave',
    'spell_monk_spinning_crane_kick',
    'spell_monk_spinning_crane_kick_energize',
    'spell_monk_spinning_crane_kick_heal',
    'spell_monk_stance_of_the_wise_serpent',
    'spell_monk_eminence_driver',
    'spell_monk_thunder_focus_tea',
    'spell_monk_zen_meditation',
    'spell_monk_mana_tea_glyphed',
    'spell_monk_mana_tea_dispel',
    'spell_monk_surging_mist_glyphed',
    'spell_monk_soothing_mist_trigger'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(115213, 'spell_monk_avert_harm'),
(115543, 'spell_monk_leer_of_the_ox'),
(137562, 'spell_monk_nimble_brew'),
(115098, 'spell_monk_chi_wave'),
(132464, 'spell_monk_chi_wave_bolt'),
(132467, 'spell_monk_chi_wave_bolt'),
(132466, 'spell_monk_chi_wave_target_selector'),
(145640, 'spell_monk_chi_brew'),
(123980, 'spell_monk_brewing_tigereye_brew'),
(115869, 'spell_monk_brewing_mana_tea'),
(122280, 'spell_monk_healing_elixirs'),
(134563, 'spell_monk_healing_elixirs_active'),
(122281, 'spell_monk_healing_elixirs_heal'),
(126060, 'spell_monk_desperate_measures'),
(119392, 'spell_monk_charging_ox_wave'),
(101546, 'spell_monk_spinning_crane_kick'),
(116847, 'spell_monk_spinning_crane_kick'),
(107270, 'spell_monk_spinning_crane_kick_energize'),
(117640, 'spell_monk_spinning_crane_kick_energize'),
(148187, 'spell_monk_spinning_crane_kick_energize'),
(117640, 'spell_monk_spinning_crane_kick_heal'),
(115070, 'spell_monk_stance_of_the_wise_serpent'),
(115070, 'spell_monk_eminence_driver'),
(130324, 'spell_monk_eminence_driver'),
(116680, 'spell_monk_thunder_focus_tea'),
(131523, 'spell_monk_zen_meditation'),
(123761, 'spell_monk_mana_tea_glyphed'),
(119611, 'spell_monk_mana_tea_dispel'),
(123273, 'spell_monk_surging_mist_glyphed'),
(116346, 'spell_monk_soothing_mist_trigger');

-- Healing Elixirs: its only effect is a periodic trigger, so it needs a row to proc at low health
DELETE FROM `spell_proc_event` WHERE `entry`=122280;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(122280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

-- Zen Meditation: the channel and its melee-break aura end together
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (-131523, -115176) AND `spell_effect` IN (-131523, -115176);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-131523, -115176, 0, 'Zen Meditation'),
(-115176, -131523, 0, 'Zen Meditation');
