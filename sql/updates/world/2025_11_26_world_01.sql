-- Register Seal of Command spell script
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_pal_seal_of_command';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(105361, 'spell_pal_seal_of_command');

