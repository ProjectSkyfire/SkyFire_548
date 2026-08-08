-- Cloak and Dagger (138106): Ambush, Garrote, and Cheap Shot teleport the rogue behind
-- the target (132987) when used from range. Range extension is DBC-driven (+35 yards).
DELETE FROM `spell_script_names`
WHERE `ScriptName` = 'spell_rog_cloak_and_dagger';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(703, 'spell_rog_cloak_and_dagger'),
(1833, 'spell_rog_cloak_and_dagger'),
(8676, 'spell_rog_cloak_and_dagger');
