-- Wound Poison (https://github.com/ProjectSkyfire/SkyFire_548/issues/949)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_wound_poison'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(8680, 'spell_rog_wound_poison');
