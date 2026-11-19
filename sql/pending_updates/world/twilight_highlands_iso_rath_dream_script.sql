-- Twilight Highlands, "Devoured" 27377 and "The Worldbreaker" 27378: nothing in the world casts the spells that credit the two
-- quests and carry the player to the Maelstrom and back, so neither can be completed. The client data has the chain: "Iso'rath
-- Fake Death Aura" (89458) triggers the kill credit of "Iso'rath Nightmare Credit" 48108 and, six seconds later, the teleport to
-- the Maelstrom (89471); "Maelstrom Fake Death Aura" (89656) credits "Maelstrom Nightmare Credit" 48161 and, six seconds later,
-- the teleport back to Earthcaller Yevaa (89659). Yevaa starts the first quest and Thrall the second, so each casts the aura
-- on the player a while after the quest is taken. The waits are a stand-in for the fights of the original game.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (47991, 48051);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (47991, 48051);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (4799100, 4805100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47991, 0, 0, 0, 19, 0, 100, 0, 27377, 0, 0, 0, 0, 80, 4799100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Earthcaller Yevaa - On Quest Accept (Devoured) - Run Script'),
(4799100, 9, 0, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, 11, 89458, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Earthcaller Yevaa - Script - Cast Iso''rath Fake Death Aura on Invoker'),
(48051, 0, 0, 0, 19, 0, 100, 0, 27378, 0, 0, 0, 0, 80, 4805100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thrall - On Quest Accept (The Worldbreaker) - Run Script'),
(4805100, 9, 0, 0, 0, 0, 100, 0, 20000, 20000, 0, 0, 0, 11, 89656, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thrall - Script - Cast Maelstrom Fake Death Aura on Invoker');
