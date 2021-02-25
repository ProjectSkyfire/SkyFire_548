DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_monk_disable';
DELETE FROM `spell_script_names` WHERE `spell_id`=116095;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(116095, 'spell_monk_disable');


