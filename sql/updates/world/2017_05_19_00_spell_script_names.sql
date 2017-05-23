DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_warr_bloodthirst';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_warr_bloodthirst_heal';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_warr_deep_wounds';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(23881, 'spell_warr_bloodthirst'),
(6343, 'spell_warr_deep_wounds'),
(12294, 'spell_warr_deep_wounds'),
(23881, 'spell_warr_deep_wounds'),
(20243, 'spell_warr_deep_wounds');
