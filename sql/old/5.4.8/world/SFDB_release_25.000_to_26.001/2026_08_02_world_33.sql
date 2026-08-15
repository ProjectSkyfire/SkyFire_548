-- Rejuvenation: bind script that suppresses EFFECT_2 periodic heal (double-tick).
DELETE FROM `spell_script_names` WHERE `spell_id` = 774 AND `ScriptName` = 'spell_dru_rejuvenation';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(774, 'spell_dru_rejuvenation');
