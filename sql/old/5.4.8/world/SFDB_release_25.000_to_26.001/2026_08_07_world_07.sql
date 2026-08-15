-- Ysera's Gift: periodic self/ally heal from talent passive 145108.
DELETE FROM `spell_script_names`
WHERE `ScriptName` IN ('spell_dru_yseras_gift', 'spell_dru_yseras_gift_heal_ally');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(145108, 'spell_dru_yseras_gift'),
(145110, 'spell_dru_yseras_gift_heal_ally');
