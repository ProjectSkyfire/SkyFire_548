-- Warlock: bind the Drain Life handler to spell 689.

DELETE FROM `spell_script_names` WHERE `spell_id` = 689 AND `ScriptName` = 'spell_warl_drain_life';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(689, 'spell_warl_drain_life');
