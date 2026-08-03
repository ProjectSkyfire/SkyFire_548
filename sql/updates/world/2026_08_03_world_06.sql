-- Unbridled Wrath (143268): on stun/immobilize, cast 29842 (15 Rage over 10 sec).
DELETE FROM `spell_script_names` WHERE `spell_id` = 143268 AND `ScriptName` = 'spell_warr_unbridled_wrath';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(143268, 'spell_warr_unbridled_wrath');

-- 29842 is Unbridled Wrath regen in MoP, not Second Wind.
DELETE FROM `spell_script_names` WHERE `spell_id` = 29842 AND `ScriptName` = 'spell_warr_second_wind_trigger';

-- Taken harmful spells; TRIGGER_ALWAYS so zero-damage CC (Cheap Shot) still procs.
-- CustomChance 100: DBC ProcChance is not usable for this passive (would roll 0%).
DELETE FROM `spell_proc_event` WHERE `entry` = 143268;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(143268, 0, 0, 0, 0, 0, 0, 0x00022220, 0x00010000, 0, 100, 0);
-- procFlags: TAKEN_SPELL_MELEE | TAKEN_SPELL_RANGED | TAKEN_SPELL_NONE_NEG | TAKEN_SPELL_MAGIC_NEG
-- procEx: PROC_EX_EX_TRIGGER_ALWAYS (0x10000) - required when damage == 0
