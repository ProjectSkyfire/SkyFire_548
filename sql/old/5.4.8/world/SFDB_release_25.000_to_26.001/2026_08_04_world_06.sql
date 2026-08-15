-- Vanish (#935): apply Vanish aura 11327 (or Subterfuge Vanish 115193); leave combat; expire into Stealth
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_vanish',
    'spell_rog_vanish_initial'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(11327,  'spell_rog_vanish'),
(115193, 'spell_rog_vanish'),
(1856,   'spell_rog_vanish_initial');

DELETE FROM `spell_proc_event` WHERE `entry` IN (115193);
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(115193, 0, 0, 0, 0, 0, 70996, 3, 0, 0, 0);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = -1784 AND `spell_effect` = -11327;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-1784, -11327, 0, 'Stealth removed -> remove Vanish');
