-- Glyph of Blind (https://github.com/ProjectSkyfire/SkyFire_548/issues/946)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_blind'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(2094, 'spell_rog_blind');
