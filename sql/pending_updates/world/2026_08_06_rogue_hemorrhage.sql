-- Hemorrhage: apply lingering bleed DoT (89775) from direct strike damage
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_hemorrhage';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(16511, 'spell_rog_hemorrhage');
