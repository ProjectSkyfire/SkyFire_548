DELETE FROM `spell_script_names` WHERE `spell_id`=768 AND `ScriptName`='spell_dru_cat_form';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (768, 'spell_dru_cat_form');

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=1850 AND `spell_effect`=768 AND `type`=0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (1850, 768, 0, 'Dash into catform');

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=5212 AND `spell_effect`=768 AND `type`=0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (5215, 768, 0, 'Prowl into catform');

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=102280 AND `spell_effect`=768 AND `type`=0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (102280, 768, 0, 'Displacer Beast into catform');

DELETE FROM `spell_script_names` WHERE `spell_id`=1850 AND `ScriptName`='spell_dru_dash';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (1850, 'spell_dru_dash');
