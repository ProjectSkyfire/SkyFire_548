-- Swipe (Bear): correct AP coefficient to 0.225 (was wrongly 0.063).
-- Swipe (Bear/Cat) and Maul: +20% damage vs bleeding targets.
DELETE FROM `spell_bonus_data` WHERE `entry` = 779;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(779, 0, 0, 0.225, -1, 'Druid - Swipe (Bear)');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_swipe_and_maul_bleed';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(779, 'spell_dru_swipe_and_maul_bleed'),
(6807, 'spell_dru_swipe_and_maul_bleed'),
(62078, 'spell_dru_swipe_and_maul_bleed');
