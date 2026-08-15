-- Glyph of Thunder Strike: trigger its lightning visual only from Thunder Clap.
DELETE FROM `spell_proc_event` WHERE `entry` = 68164;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(68164, 0, 4, 128, 0, 0, 0, 0, 0, 0, 0, 0);
