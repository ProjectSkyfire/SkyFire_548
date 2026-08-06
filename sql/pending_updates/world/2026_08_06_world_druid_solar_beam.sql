-- Solar Beam: apply its silence while enemies remain inside the beam.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_solar_beam';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(78675, 'spell_dru_solar_beam'),
(113286, 'spell_dru_solar_beam');
