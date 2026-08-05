-- Shroud of Concealment (https://github.com/ProjectSkyfire/SkyFire_548/issues/939)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_shroud_of_concealment'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(115834, 'spell_rog_shroud_of_concealment');
