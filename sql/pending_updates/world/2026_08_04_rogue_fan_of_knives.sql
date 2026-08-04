-- Fan of Knives (#955): award combo points to current combo/selected target only
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_fan_of_knives';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(51723, 'spell_rog_fan_of_knives');
