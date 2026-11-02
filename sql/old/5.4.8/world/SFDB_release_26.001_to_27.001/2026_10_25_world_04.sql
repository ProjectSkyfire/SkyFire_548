-- Monk Stagger
DELETE FROM `spell_script_names` WHERE `spell_id` IN (115069, 124255, 124273, 124274, 124275);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(115069, 'spell_monk_stance_of_the_sturdy_ox'),
(124255, 'spell_monk_stagger'),
(124273, 'spell_monk_stagger_dummy'),
(124274, 'spell_monk_stagger_dummy'),
(124275, 'spell_monk_stagger_dummy');
