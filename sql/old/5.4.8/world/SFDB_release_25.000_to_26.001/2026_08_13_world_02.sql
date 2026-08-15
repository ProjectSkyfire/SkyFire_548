-- Frozen Mountaineer
DELETE FROM `creature_text` WHERE `entry`=41763;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(41763, 0, 0, '%s gasps for air and shivers in the cold.', 16, 0, 100, 20, 0, 0, 'Frozen Mountaineer');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41763;

DELETE FROM `smart_scripts` WHERE `entryorguid`=41763;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41763, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 11, 77906, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Respawn - Summon'),
(41763, 0, 1, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 80, 4176300, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Data Set - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4176300;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4176300, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 77910, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Script - Remove Aura'),
(4176300, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Script - Say 0'),
(4176300, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frozen Mountaineer - On Script - Despawn');

-- Icy Tomb
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41768;

DELETE FROM `smart_scripts` WHERE `entryorguid`=41768;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41768, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 41763, 5, 0, 0, 0, 0, 0, 'Icy Tomb - On Death - Set Data'),
(41768, 0, 1, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4176800, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Icy Tomb - On Summoned - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4176800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4176800, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 11, 41763, 5, 0, 0, 0, 0, 0, 'Icy Tomb - On Script - Set Data'),
(4176800, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Icy Tomb - On Script - Set Rooted'),
(4176800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Icy Tomb - On Script - Set React State');

-- Entombed in Ice
UPDATE `quest_template` SET `OfferRewardText`='You\'ve done it! The mountaineers are returning to camp and our medics will have their hands full. We are all in your debt, $n.' WHERE `Id`=25978;

