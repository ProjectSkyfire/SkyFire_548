-- Restless Blades (#967): reduce AR/KS/Redirect/Shadow Blades/Sprint CD per combo point
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_restless_blades';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(79096, 'spell_rog_restless_blades');

DELETE FROM `spell_proc_event` WHERE `entry` = 79096;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(79096, 0, 8, 1179648, 1, 8192, 0, 3, 0, 0, 0);
