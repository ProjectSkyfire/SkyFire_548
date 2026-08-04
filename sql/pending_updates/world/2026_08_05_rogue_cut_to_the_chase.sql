-- Cut to the Chase (#957): refresh Slice and Dice on Envenom
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_cut_to_the_chase';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(51667, 'spell_rog_cut_to_the_chase');

DELETE FROM `spell_proc_event` WHERE `entry` = 51667;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(51667, 0, 8, 0, 0x8, 0, 0, 0, 0, 0, 0);
