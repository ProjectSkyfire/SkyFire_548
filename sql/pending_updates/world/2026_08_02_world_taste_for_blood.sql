-- Taste for Blood (56636): Mortal Strike -> 2 stacks of 60503; dodge -> 1 stack.
-- Overpower (7384): consume one stack; reduce Mortal Strike CD by 0.5s.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_warr_taste_for_blood', 'spell_warr_overpower');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(56636, 'spell_warr_taste_for_blood'),
(7384, 'spell_warr_overpower');

-- Proc on melee hit/crit and when the target dodges (auto or spell).
DELETE FROM `spell_proc_event` WHERE `entry` = 56636;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(56636, 0, 0, 0, 0, 0, 0, 0x00000014, 0x00000013, 0, 0, 0);
