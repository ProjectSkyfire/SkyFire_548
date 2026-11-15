-- Frozen Power (63374): Frost Shock roots the target for 5 seconds. The talent has no proc data, so the Frost Shock cast
-- applies the root. Based on Legends of Azeroth 5.4.8. Safe to run twice.
DELETE FROM `spell_script_names` WHERE `spell_id` = 8056 AND `ScriptName` = 'spell_sha_frozen_power';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(8056, 'spell_sha_frozen_power');
