-- Blindside (#956): proc only from Mutilate hits; buff consumed only by Dispatch
DELETE FROM `spell_proc_event` WHERE `entry` IN (121152, 121153);
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
-- Mutilate MH (0x2) + OH (0x4) damage spells
(121152, 0, 8, 0, 0x6, 0, 0, 3, 0, 0, 0),
-- Dispatch SpellFamilyFlags[2] = 0x100
(121153, 0, 8, 0, 0, 0x100, 0, 0, 0, 0, 0);
