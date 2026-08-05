-- Smoke Bomb (https://github.com/ProjectSkyfire/SkyFire_548/issues/938)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_smoke_bomb'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(76577, 'spell_rog_smoke_bomb'),
(128829, 'spell_rog_smoke_bomb');
