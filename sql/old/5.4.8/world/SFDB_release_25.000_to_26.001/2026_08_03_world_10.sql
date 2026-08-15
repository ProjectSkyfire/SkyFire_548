-- Shield Wall / Spell Reflection: cast faction or shield cosmetic auras while active.
-- Also bind Spell Reflection proc filter (not consumed by player pets/guardians).
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_warr_shield_visual', 'spell_warr_spell_reflection');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(871, 'spell_warr_shield_visual'),
(23920, 'spell_warr_shield_visual'),
(23920, 'spell_warr_spell_reflection'),
(114028, 'spell_warr_spell_reflection');
