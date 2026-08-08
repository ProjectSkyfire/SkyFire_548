-- Dirty Tricks: Blind / Gouge do not break from the caster's Poison and Bleed damage
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_dirty_tricks';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(1776, 'spell_rog_dirty_tricks'),
(2094, 'spell_rog_dirty_tricks');
