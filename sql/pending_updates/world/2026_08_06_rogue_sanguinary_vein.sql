-- Sanguinary Vein (#942): apply 124271 while Rupture/Garrote/Crimson Tempest (or glyph'd Hemorrhage) is up
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_sanguinary_vein';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(1943, 'spell_rog_sanguinary_vein'),
(703, 'spell_rog_sanguinary_vein'),
(122233, 'spell_rog_sanguinary_vein'),
(89775, 'spell_rog_sanguinary_vein');
