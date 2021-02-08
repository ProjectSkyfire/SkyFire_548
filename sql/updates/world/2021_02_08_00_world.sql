DELETE FROM `spell_script_names` WHERE  `spell_id`=92833;
DELETE FROM `spell_script_names` WHERE  `spell_id`=73325;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('73325', 'spell_pri_leap_of_faith');

DELETE FROM `spell_linked_spell` WHERE  `spell_trigger`=73325 AND `spell_effect`=92833 AND `type`=0;