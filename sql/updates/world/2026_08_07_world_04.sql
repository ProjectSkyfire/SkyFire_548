-- Death Comes From On High: wire Eye of Acherus aura and recall scripts.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN
('spell_q12641_the_eye_of_acherus', 'spell_q12641_recall_eye_of_acherus');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(51852, 'spell_q12641_the_eye_of_acherus'),
(52694, 'spell_q12641_recall_eye_of_acherus');
