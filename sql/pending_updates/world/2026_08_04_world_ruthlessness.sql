-- Ruthlessness (#969): delay combo-point grant until after finisher clears CPs
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_combo_point_delayed';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(139569, 'spell_rog_combo_point_delayed');
