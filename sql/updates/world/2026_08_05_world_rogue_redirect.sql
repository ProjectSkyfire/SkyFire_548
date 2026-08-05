-- Redirect (https://github.com/ProjectSkyfire/SkyFire_548/issues/933)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_redirect'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(73981, 'spell_rog_redirect'),
(110730, 'spell_rog_redirect');
