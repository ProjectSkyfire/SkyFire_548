-- Body and Soul SpellScriptName
DELETE FROM `spell_script_names` WHERE ScriptName='spell_pri_body_and_soul';
DELETE FROM `spell_script_names` WHERE spell_id=64129;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(64129, 'spell_pri_body_and_soul');
