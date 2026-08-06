-- Ferocious Bite (22568): restore AP coefficient, energy dump damage, Rip refresh, glyph heal.
DELETE FROM `spell_bonus_data` WHERE `entry` = 22568;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(22568, -1, -1, 0.196, -1, 'Ferocious Bite');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_ferocious_bite';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(22568, 'spell_dru_ferocious_bite');
