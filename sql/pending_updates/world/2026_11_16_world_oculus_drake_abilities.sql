-- The Oculus: Amber Drake Shock Lance detonates Shock Charges, Ruby Drake Evasive Maneuvers burns a charge per dodge.
-- Ported from TrinityCore 3.3.5.

DELETE FROM `spell_script_names` WHERE `spell_id` IN (49840, 50240);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(49840, 'spell_oculus_shock_lance'),
(50240, 'spell_oculus_evasive_maneuvers');
