-- Balance Druid: run Eclipse entry effects for Solar and Lunar Eclipse.
DELETE FROM `spell_script_names`
WHERE `ScriptName` IN ('spell_dru_eclipse_solar', 'spell_dru_eclipse_lunar');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(48517, 'spell_dru_eclipse_solar'),
(48518, 'spell_dru_eclipse_lunar');
