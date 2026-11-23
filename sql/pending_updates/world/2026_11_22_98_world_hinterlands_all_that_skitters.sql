-- All That Skitters (26310 and 26523) asks for 8 Skittering Spiderlings, which are collected by clicking the
-- Skittering Spiderling (42689): the click casts spell 79591, which creates the item, and the spiderling goes away.
-- The spiderling had no spell click data and no spell click flag, so it could not be clicked, and no script to
-- despawn it. Click data, flag and script are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = 'SmartAI' WHERE `entry` = 42689 AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 42689;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(42689, 79591, 2, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 42689 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42689, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skittering Spiderling - On Spellclick - Despawn');
