-- Utgarde Pinnacle: Skadi's Poisoned Spear applies its poison.
-- Ported from TrinityCore 3.3.5.

DELETE FROM `spell_script_names` WHERE `spell_id` IN (50255, 59331);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(50255, 'spell_skadi_poisoned_spear'),
(59331, 'spell_skadi_poisoned_spear');
