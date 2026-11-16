-- Murmur's Touch explodes with Shockwave when it runs out; the aura had no script for it.

DELETE FROM `spell_script_names` WHERE `spell_id` IN (33711, 38794);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(33711, 'spell_murmur_murmurs_touch'),
(38794, 'spell_murmur_murmurs_touch');
