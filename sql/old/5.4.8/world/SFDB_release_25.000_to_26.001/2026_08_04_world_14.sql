-- Honor Among Thieves: party/raid crits grant a combo point (2s ICD while in combat)
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_honor_among_thieves';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(51701, 'spell_rog_honor_among_thieves');

-- Crit-only procs; ICD is enforced on the rogue in script (actor cooldown is per-party-member)
DELETE FROM `spell_proc_event` WHERE `entry` = 51701;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(51701, 0, 0, 0, 0, 0, 0, 2, 0, 100, 0);
