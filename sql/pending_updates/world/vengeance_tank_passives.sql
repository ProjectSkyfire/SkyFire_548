-- Vengeance: the tank passives of Paladin, Druid, Warrior, Death Knight and Monk grant attack power from the damage taken.
-- Proc on hits, fully absorbed hits, dodges and parries (1 normal hit, 2 critical hit, 16 dodge, 32 parry, 1024 absorb), so misses never reach the script.
DELETE FROM `spell_proc_event` WHERE `entry` IN (84839,84840,93098,93099,120267);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(84839, 0, 0, 0, 0, 0, 0, 0, 1075, 0, 0, 0),
(84840, 0, 0, 0, 0, 0, 0, 0, 1075, 0, 0, 0),
(93098, 0, 0, 0, 0, 0, 0, 0, 1075, 0, 0, 0),
(93099, 0, 0, 0, 0, 0, 0, 0, 1075, 0, 0, 0),
(120267, 0, 0, 0, 0, 0, 0, 0, 1075, 0, 0, 0);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (84839,84840,93098,93099,120267) AND `ScriptName` = 'spell_gen_vengeance';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(84839, 'spell_gen_vengeance'),
(84840, 'spell_gen_vengeance'),
(93098, 'spell_gen_vengeance'),
(93099, 'spell_gen_vengeance'),
(120267, 'spell_gen_vengeance');

DELETE FROM `spell_script_names` WHERE `spell_id` = 768 AND `ScriptName` = 'spell_dru_cat_form_vengeance';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(768, 'spell_dru_cat_form_vengeance');
