-- Fix Second Wind spell scripts registration
DELETE FROM `spell_script_names` WHERE `spell_id` IN (29834, 29838, 125667);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(29834, 'spell_warr_second_wind_proc'),
(29838, 'spell_warr_second_wind_proc'),
(125667, 'spell_warr_second_wind_heal');

-- Ensure Second Wind procs on damage taken
-- Spell 29838 is the Second Wind proc spell (rank 2)
DELETE FROM `spell_proc_event` WHERE `entry` IN (29834, 29838);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(29838, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00100000, 0x00000000, 0.000000, 0.000000, 0),
(29834, 0, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00100000, 0x00000000, 0.000000, 0.000000, 0);
-- Note: procFlags 0x00100000 = PROC_FLAG_TAKEN_DAMAGE (proc on any damage taken)

