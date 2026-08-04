-- Shadowstep: reject self-target (and SotA Antipersonnel Cannon)
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_shadowstep';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(36554, 'spell_rog_shadowstep');
