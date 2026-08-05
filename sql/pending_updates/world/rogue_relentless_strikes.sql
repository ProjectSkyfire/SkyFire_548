-- Relentless Strikes (https://github.com/ProjectSkyfire/SkyFire_548/issues/943)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_relentless_strikes'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(14181, 'spell_rog_relentless_strikes');
