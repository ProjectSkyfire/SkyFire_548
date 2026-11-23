-- The flamethrower, corpse piles and centurions had a minimal SmartAI; the scripts replace it (rows kept).
UPDATE `creature_template` SET `ScriptName` = 'npc_scm_hooded_crusader' WHERE `entry` = 64838 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_scarlet_flamethower' WHERE `entry` = 59705 AND `ScriptName` IN ('', 'npc_scarlet_flamethower') AND `AIName` IN ('', 'SmartAI');
UPDATE `creature_template` SET `ScriptName` = 'npc_scm_fuel_tank' WHERE `entry` = 59706 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_frenzied_spirit' WHERE `entry` = 60033 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_fanatic' WHERE `entry` = 58555 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_judicator' WHERE `entry` = 58605 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_pile_corpses' WHERE `entry` = 59722 AND `ScriptName` IN ('', 'npc_pile_corpses') AND `AIName` IN ('', 'SmartAI');
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_scarlet_centurion' WHERE `entry` = 59746 AND `ScriptName` IN ('', 'npc_scarlet_centurion') AND `AIName` IN ('', 'SmartAI');
UPDATE `creature_template` SET `ScriptName` = 'npc_zombified_corpse' WHERE `entry` = 59771 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_zealot' WHERE `entry` = 58590 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_sc_spirit_of_redemption' WHERE `entry` = 58592 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_purifier' WHERE `entry` = 58569 AND `ScriptName` = '' AND `AIName` = '';

DELETE FROM `creature_text` WHERE `entry` = 64838 AND `groupid` = 1;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(64838, 1, 0, 'Destroy the Piles of Corpses to stop the Zombified Corpses!', 41, 0, 100, 0, 0, 0, 'Hooded Crusader entrance');

DELETE FROM `spell_script_names` WHERE (`spell_id` = 115519 AND `ScriptName` = 'spell_sc_cleave') OR (`spell_id` = 111670 AND `ScriptName` = 'spell_sc_ressurection') OR (`spell_id` = 114952 AND `ScriptName` = 'spell_scm_barrel_explosion');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(115519, 'spell_sc_cleave'),
(111670, 'spell_sc_ressurection'),
(114952, 'spell_scm_barrel_explosion');

DELETE FROM `areatrigger_scripts` WHERE `entry` = 8364;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES (8364, 'AreaTrigger_at_scarlet_monastery');
