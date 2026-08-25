-- Hunter Stampede (121818) and Glaive Toss missile/damage scripts
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_hun_stampede',
    'spell_hun_glaive_toss_missile',
    'spell_hun_glaive_toss_damage'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(121818, 'spell_hun_stampede'),
(120755, 'spell_hun_glaive_toss_missile'),
(120756, 'spell_hun_glaive_toss_missile'),
(120761, 'spell_hun_glaive_toss_damage'),
(121414, 'spell_hun_glaive_toss_damage');
