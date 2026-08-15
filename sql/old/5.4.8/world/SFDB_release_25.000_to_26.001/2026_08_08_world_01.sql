-- Incarnation (talent 106731 / per-spec forms): script bindings and Tree of Life passives.
-- Spec action-bar remap is DBC-driven via Balance/Feral/Guardian/Restoration Overrides Passive.
DELETE FROM `spell_script_names`
WHERE `ScriptName` IN (
    'spell_dru_incarnation_chosen_of_elune',
    'spell_dru_incarnation_skins',
    'spell_dru_tree_of_life',
    'spell_dru_incarnation_tree_aura'
);

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102560, 'spell_dru_incarnation_chosen_of_elune'),
(102560, 'spell_dru_incarnation_skins'),
(102543, 'spell_dru_incarnation_skins'),
(102558, 'spell_dru_incarnation_skins'),
(33891, 'spell_dru_tree_of_life'),
(117679, 'spell_dru_incarnation_tree_aura');

-- Tree of Life: healing/armor passive, instant Roots/Regrowth, unlimited Lifebloom.
DELETE FROM `spell_linked_spell`
WHERE `spell_trigger` = 33891
  AND `spell_effect` IN (5420, 81097, 81098);

INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(33891, 5420, 0, 'Incarnation Tree of Life: healing and armor'),
(33891, 81097, 0, 'Incarnation Tree of Life: instant Roots and Regrowth'),
(33891, 81098, 0, 'Incarnation Tree of Life: unlimited Lifebloom');
