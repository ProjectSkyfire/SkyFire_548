-- Ultimatum: proc only from critical Shield Slam hits.
DELETE FROM `spell_proc_event` WHERE `entry` = 122509;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(122509, 0, 4, 0, 512, 0, 0, 0, 2, 0, 0, 0);
