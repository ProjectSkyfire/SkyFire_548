-- Shooting Stars: proc from critical Moonfire or Sunfire periodic damage and reset Starsurge.
DELETE FROM `spell_proc_event` WHERE `entry` = 93399;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(93399, 0, 7, 2, 0, 0, 0, 0, 2, 0, 0, 0);

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_shooting_stars';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(93400, 'spell_dru_shooting_stars');
