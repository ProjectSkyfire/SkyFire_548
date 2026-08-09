-- Killing Spree teleport: correct destination against world geometry
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_killing_spree_teleport';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(57840, 'spell_rog_killing_spree_teleport');
