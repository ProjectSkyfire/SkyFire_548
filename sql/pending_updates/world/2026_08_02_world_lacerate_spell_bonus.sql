-- Lacerate (33745): MoP AP scaling
-- Direct hit: AP * 0.616 (via spell_bonus_data)
-- DoT tick: AP * 0.0512 baked into aura amount by spell_dru_lacerate (client $w1 + ticks)
DELETE FROM `spell_bonus_data` WHERE `entry` = 33745;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(33745, 0, 0, 0.616, 0, 'Druid - Lacerate (MoP: AP*0.616 direct; DoT AP via script)');

DELETE FROM `spell_script_names` WHERE `spell_id` = 33745 AND `ScriptName` = 'spell_dru_lacerate';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(33745, 'spell_dru_lacerate');
