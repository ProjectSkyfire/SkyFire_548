-- Rake (1822): MoP AP coefficients (0.3 initial / 0.3 per tick).
-- DoT AP is baked in spell_dru_rake so the $w2 bleed tooltip matches tick damage.
DELETE FROM `spell_bonus_data` WHERE `entry` = 1822;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(1822, -1, -1, 0.3, 0, 'Rake (MoP: 0.3 AP initial; DoT AP via script)');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_rake';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(1822, 'spell_dru_rake');
