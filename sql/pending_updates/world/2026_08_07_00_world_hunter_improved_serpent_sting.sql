DELETE FROM `spell_script_names`
WHERE (`spell_id` IN (19464, 82834) AND `ScriptName` = 'spell_hun_improved_serpent_sting')
   OR (`spell_id` = 118253 AND `ScriptName` = 'spell_hun_serpent_sting');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(82834, 'spell_hun_improved_serpent_sting'),
(118253, 'spell_hun_serpent_sting');
