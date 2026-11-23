-- Drag it Out of Them: the netted Razormane Pillager can be dragged back to Grol'dom Farm. The scripts replace the
-- Pillager's SmartAI.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 34503;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_razormane_pillager' WHERE `entry` IN (34503, 34514);
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_groldom_net', 'spell_snared_in_net');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(65580, 'spell_groldom_net'),
(65581, 'spell_snared_in_net');
DELETE FROM `areatrigger_scripts` WHERE `entry` = 5465;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES (5465, 'AreaTrigger_at_groldom_farm');
