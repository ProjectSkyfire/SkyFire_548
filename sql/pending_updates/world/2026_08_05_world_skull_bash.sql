-- Skull Bash: teach MoP spell 106839 (Cat + Bear) and bind charge/interrupt scripts.
-- Replaces legacy form-split IDs 80964 (bear) / 80965 (cat) which used the wrong icons.
DELETE FROM `npc_trainer` WHERE `entry` = 200011 AND `spell` IN (80964, 80965, 106839);
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(200011, 106839, 2100, 0, 0, 22);

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_skull_bash';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(106839, 'spell_dru_skull_bash'),
(80964, 'spell_dru_skull_bash'),
(80965, 'spell_dru_skull_bash');
