-- Darkmoon Faire carousel WHEE trigger binding

DELETE FROM `spell_script_names`
WHERE `ScriptName` = 'spell_whee_trigger';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(134815, 'spell_whee_trigger');
