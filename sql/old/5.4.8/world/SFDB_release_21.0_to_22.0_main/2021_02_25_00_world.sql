DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_monk_roll';
DELETE FROM `spell_script_names` WHERE `spell_id`=109132;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(109132, 'spell_monk_roll');
