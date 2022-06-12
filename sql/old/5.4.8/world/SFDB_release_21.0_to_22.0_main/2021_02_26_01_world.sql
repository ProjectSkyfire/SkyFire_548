DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=115546 AND `spell_effect`=118635 AND `type`=0;

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_monk_provoke';
DELETE FROM `spell_script_names` WHERE `spell_id`=115546;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(115546, 'spell_monk_provoke');

