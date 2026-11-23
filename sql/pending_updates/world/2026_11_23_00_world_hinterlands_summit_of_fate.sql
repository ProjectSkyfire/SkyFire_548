-- Summit of Fate (26363 and 26517) asks for 8 Spider Idols, which are taken from the Sacrificed Vilebranch (42857) by
-- clicking them: the click is only the trigger, the SmartAI gives the item and removes the body. The Sacrificed
-- Vilebranch had no spell click data, no spell click flag and no SmartAI, so it could not be clicked. Click data, flag
-- and script are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = 'SmartAI' WHERE `entry` = 42857 AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 42857;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(42857, 79929, 2, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 42857 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42857, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 56, 58228, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sacrificed Vilebranch - On Spellclick - Add Item Spider Idol'),
(42857, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sacrificed Vilebranch - Linked To Id 0 - Despawn');
