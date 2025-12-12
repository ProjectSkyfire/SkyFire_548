-- Fix Fear No Evil injured infantry health randomizer script binding
DELETE FROM `spell_script_names` WHERE `spell_id` = 53034;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(53034, 'spell_q28813_set_health_random');

