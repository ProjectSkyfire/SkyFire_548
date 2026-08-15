-- Thrill of the Hunt (109306): MoP proc aura triggers 34720 at 30% for focus-spending ranged attacks and Kill Command.
-- DBC supplies the triggered aura, its 3 charges, and the -20 focus modifier for Arcane Shot / Multi-Shot.
DELETE FROM `spell_proc_event` WHERE `entry` = 109306;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(109306, 0, 9, 0x00025880, 0x88000800, 0x01000101, 0x0000208D, 0x00011510, 0x00000000, 0, 30, 0);
