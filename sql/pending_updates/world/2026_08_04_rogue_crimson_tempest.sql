-- Crimson Tempest (#953): apply bleed DoT 122233 from direct damage
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_crimson_tempest';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(121411, 'spell_rog_crimson_tempest');
