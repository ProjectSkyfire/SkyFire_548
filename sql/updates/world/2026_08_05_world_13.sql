DELETE FROM `spell_script_names` WHERE `spell_id` = 120679 OR `ScriptName` = 'spell_hun_dire_beast';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(120679, 'spell_hun_dire_beast');

UPDATE `creature_template` SET `ScriptName` = 'npc_dire_beast' WHERE `entry` = 62005;
