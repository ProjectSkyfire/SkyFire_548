DELETE FROM `spell_script_names` WHERE `spell_id` = 57731 AND `ScriptName` = 'spell_hadronox_web_grab';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (57731, 'spell_hadronox_web_grab');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (54314, 59354) AND `ScriptName` = 'spell_azjol_drain_power';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(54314, 'spell_azjol_drain_power'),
(59354, 'spell_azjol_drain_power');
