-- Combat Potency (#965): bind script that rolls OH-speed-scaled proc chance
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_combat_potency';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(35551, 'spell_rog_combat_potency');
