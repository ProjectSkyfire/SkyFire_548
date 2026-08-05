-- Master Poisoner (https://github.com/ProjectSkyfire/SkyFire_548/issues/952)
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_master_poisoner'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(2818, 'spell_rog_master_poisoner'),
(3409, 'spell_rog_master_poisoner'),
(5760, 'spell_rog_master_poisoner'),
(8680, 'spell_rog_master_poisoner'),
(112961, 'spell_rog_master_poisoner'),
(113780, 'spell_rog_master_poisoner'),
(113952, 'spell_rog_master_poisoner');
