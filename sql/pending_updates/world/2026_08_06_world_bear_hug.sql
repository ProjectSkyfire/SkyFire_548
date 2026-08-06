-- Bear Hug: periodic damage is % of caster max health.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_bear_hug';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102795, 'spell_dru_bear_hug');
