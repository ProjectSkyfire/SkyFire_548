-- Activate Death Knight scripts that were present in source but not bound in the world database.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN
('spell_dk_gorefiends_grasp', 'spell_dk_runic_empowerment', 'spell_dk_runic_corruption', 'spell_dk_raise_dead');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(108199, 'spell_dk_gorefiends_grasp'),
(47632, 'spell_dk_runic_empowerment'),
(49143, 'spell_dk_runic_empowerment'),
(56815, 'spell_dk_runic_empowerment'),
(47632, 'spell_dk_runic_corruption'),
(49143, 'spell_dk_runic_corruption'),
(56815, 'spell_dk_runic_corruption'),
(46584, 'spell_dk_raise_dead');
