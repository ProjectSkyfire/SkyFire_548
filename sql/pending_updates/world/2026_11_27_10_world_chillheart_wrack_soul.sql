-- Wrack Soul jumps to one party member when its damage aura expires.
DELETE FROM `spell_script_names` WHERE `spell_id` = 111631 AND `ScriptName` = 'spell_gen_chillheart_wrack_soul';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(111631, 'spell_gen_chillheart_wrack_soul');
