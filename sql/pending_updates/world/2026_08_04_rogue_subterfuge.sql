-- Subterfuge (#922): Stealth form 115191 + Subterfuge buff 115192 on break / FoK / Crimson Tempest
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_stealth_subterfuge',
    'spell_rog_subterfuge_cast_trigger'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(115191, 'spell_rog_stealth_subterfuge'),
(51723,  'spell_rog_subterfuge_cast_trigger'),
(121411, 'spell_rog_subterfuge_cast_trigger');

DELETE FROM `spell_proc_event` WHERE `entry` IN (115191);
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(115191, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (-108208, -115192) AND `spell_effect` IN (-115191, -115193);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-108208, -115191, 0, 'Subterfuge talent removed -> remove Subterfuge Stealth'),
(-115192, -115191, 0, 'Subterfuge buff removed -> remove Subterfuge Stealth'),
(-115192, -115193, 0, 'Subterfuge buff removed -> remove Subterfuge Vanish');
