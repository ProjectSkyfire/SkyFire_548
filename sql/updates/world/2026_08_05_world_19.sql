-- Nerve Strike (https://github.com/ProjectSkyfire/SkyFire_548/issues/923)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_nerve_strike',
    'spell_rog_nerve_strike_effect'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(408, 'spell_rog_nerve_strike'),
(1833, 'spell_rog_nerve_strike'),
(112947, 'spell_rog_nerve_strike_effect');
