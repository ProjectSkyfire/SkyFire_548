-- Bandit's Guile (#964): Sinister Strike / Revealing Strike insight stacking
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_bandits_guile';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(84654, 'spell_rog_bandits_guile');
