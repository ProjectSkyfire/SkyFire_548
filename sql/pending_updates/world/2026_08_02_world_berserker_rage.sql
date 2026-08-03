-- Berserker Rage (18499): apply Enrage (12880) on cast (rage + physical damage).
DELETE FROM `spell_script_names` WHERE `spell_id` = 18499 OR `ScriptName` = 'spell_warr_berserker_rage';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(18499, 'spell_warr_berserker_rage');
