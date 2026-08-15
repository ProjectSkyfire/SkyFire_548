-- Slam + Sweeping Strikes: 146361 cleave script filters the primary Slam target
DELETE FROM `spell_script_names` WHERE `spell_id` = 146361 AND `ScriptName` = 'spell_warr_slam_cleave';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(146361, 'spell_warr_slam_cleave');
