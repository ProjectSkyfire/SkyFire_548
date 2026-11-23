-- Drakos' Magic Pull is a dummy effect that needs its script to pull the players.

DELETE FROM `spell_script_names` WHERE `spell_id` = 51336;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(51336, 'spell_drakos_magic_pull');
