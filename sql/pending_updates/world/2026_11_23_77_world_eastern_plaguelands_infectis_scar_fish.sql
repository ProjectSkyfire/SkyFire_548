-- Frederick's Fish Fancy (27450) asks for 8 Plague Puffers, 8 Infectis Inchers and 8 Infectis Scuttlers (items
-- 61292 to 61294) from the Infectis Scar, but the fish could not be picked up: they were not clickable and had no
-- script. Clicking one now gives its item and removes it (the Peck Pufferfish spell click and the quest item rows of
-- the source).
-- Rows are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (45650, 45655, 45657) AND `ScriptName` = '';
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (45650, 45655, 45657);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (45650, 45655, 45657);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(45650, 86328, 1, 0),
(45655, 86328, 1, 0),
(45657, 86328, 1, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (45650, 45655, 45657) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45650, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 56, 61292, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Plague Puffer - On Spellclick - Add Quest Item'),
(45650, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Puffer - Linked To Id 0 - Despawn'),
(45655, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 56, 61293, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Infectis Incher - On Spellclick - Add Quest Item'),
(45655, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infectis Incher - Linked To Id 0 - Despawn'),
(45657, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 56, 61294, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Infectis Scuttler - On Spellclick - Add Quest Item'),
(45657, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infectis Scuttler - Linked To Id 0 - Despawn');
