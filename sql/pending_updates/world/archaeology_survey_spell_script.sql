-- Bind Survey (80451) to the archaeology survey script and remove any obsolete binds.
DELETE FROM `spell_script_names` WHERE `spell_id` = 80451;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(80451, 'spell_archaeology_survey');
