-- Lacerate / Thrash (Bear) / Faerie Fire: chance to reset Mangle (Bear) cooldown.
DELETE FROM `spell_script_names`
WHERE `ScriptName` IN ('spell_dru_thrash_bear', 'spell_dru_faerie_fire')
   OR (`spell_id` = 33745 AND `ScriptName` = 'spell_dru_lacerate');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(33745, 'spell_dru_lacerate'),
(77758, 'spell_dru_thrash_bear'),
(770,   'spell_dru_faerie_fire');
