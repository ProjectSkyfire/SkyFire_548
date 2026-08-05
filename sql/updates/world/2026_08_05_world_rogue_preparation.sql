-- Preparation (https://github.com/ProjectSkyfire/SkyFire_548/issues/932)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_preparation'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(14185, 'spell_rog_preparation');
