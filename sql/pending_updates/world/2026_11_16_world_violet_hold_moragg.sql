-- The Violet Hold: Moragg's Ray of Pain, Ray of Suffering and Optic Link scripts.
-- Ported from TrinityCore 3.3.5.

DELETE FROM `spell_script_names` WHERE `spell_id` IN (54396, 54438, 54442, 59523, 59524);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(54396, 'spell_moragg_optic_link'),
(54438, 'spell_moragg_ray'),
(54442, 'spell_moragg_ray'),
(59523, 'spell_moragg_ray'),
(59524, 'spell_moragg_ray');
