-- Divine Aegis SpellScriptName
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_divine_aegis';
DELETE FROM `spell_script_names` WHERE `spell_id`=47515;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(47515, 'spell_pri_divine_aegis');
