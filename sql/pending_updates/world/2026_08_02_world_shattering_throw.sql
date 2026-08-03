-- Shattering Throw (#996): strip Ice Block/Divine Shield then deal damage
-- Damage casts: 64382 (player), 65940 (ToC), 112997 (Symbiosis Shattering Blow)
-- Linked missiles: 64380, 65941, 113000
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_warr_shattering_throw';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(64380, 'spell_warr_shattering_throw'),
(64382, 'spell_warr_shattering_throw'),
(65940, 'spell_warr_shattering_throw'),
(65941, 'spell_warr_shattering_throw'),
(112997, 'spell_warr_shattering_throw'),
(113000, 'spell_warr_shattering_throw');
