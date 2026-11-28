-- Touch of the Grave uses a 15-second internal cooldown in the 5.4.8 client data.
-- Keep the client proc flags and 20% chance. With procEx = 0 the core requires
-- an active damaging hit or critical hit, excluding Counterspell and other utility spells.
DELETE FROM `spell_proc_event` WHERE `entry` = 5227;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(5227, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15);
