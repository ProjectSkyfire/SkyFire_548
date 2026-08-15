-- Trapped Miner
DELETE FROM `creature_text` WHERE `entry`=41671;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(41671, 0, 0, 'You\'re the best thing I\'ve seen since the troggs showed up!', 12, 0, 100, 0, 0, 0, 'SAY_0_0'),
(41671, 0, 1, 'I thought I would die down here.', 12, 0, 100, 0, 0, 0, 'SAY_0_1'),
(41671, 0, 2, 'I thought they were going to tear me apart!', 12, 0, 100, 0, 0, 0, 'SAY_0_2'),
(41671, 0, 3, 'I can\'t wait to get home to my family. Thank you, stranger.', 12, 0, 100, 0, 0, 0, 'SAY_0_3');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41671;

DELETE FROM `smart_scripts` WHERE `entryorguid`=41671;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41671, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Miner - On Respawn - Set Emote State'),
(41671, 0, 1, 0, 8, 0, 100, 0, 77819, 0, 0, 0, 0, 80, 4167100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Miner - On Spellhit - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4167100;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4167100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Trapped Miner - On Script - Set Orientation'),
(4167100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Miner - On Script - Say 0'),
(4167100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Miner - On Script - Set Emote State'),
(4167100, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 17, 26, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Miner - On Script - Set Emote State'),
(4167100, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 41671, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Trapped Miner - On Script - Give Credit'),
(4167100, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trapped Miner - On Script - Despawn');

-- The Public Servant
UPDATE `quest_template` SET `OfferRewardText`='What a relief! I\'m glad to see that you were able to save so many miners, $n. I will make sure that their families are notified immediately.$B$BThe Senate will know of your role in winning back the mines.' WHERE `Id`=433;
