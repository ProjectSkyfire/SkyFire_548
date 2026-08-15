-- Makeshift Cage
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE `entry`=204019;

DELETE FROM `smart_scripts` WHERE `entryorguid`=204019;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(204019, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 80, 20401900, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Makeshift Cage - Activated - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=20401900;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(20401900, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 42645, 5, 0, 0, 0, 0, 0, 'Makeshift Cage - On Script - Set Data to Captured Demolitionist'),
(20401900, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 42645, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Makeshift Cage - On Script - Give Credit'),
(20401900, 9, 2, 0, 0, 0, 100, 0, 60000, 60000, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Makeshift Cage - On Script - Reset Gameobject');

-- Captured Demolitionist
DELETE FROM `creature_text` WHERE `entry`=42645;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42645, 0, 0, 'I\'m free! I\'m really free!', 12, 0, 100, 0, 0, 0, 'Captured Demolitionist - SAY_0'),
(42645, 0, 1, 'Thank you for breaking me out of here!', 12, 0, 100, 0, 0, 0, 'Captured Demolitionist - SAY_0'),
(42645, 0, 2, 'Thanks. Now, let\'s blow up that cave!', 12, 0, 100, 0, 0, 0, 'Captured Demolitionist - SAY_0'),
(42645, 0, 3, 'I don\'t ever want to smell unwashed trogg again!', 12, 0, 100, 0, 0, 0, 'Captured Demolitionist - SAY_0'),
(42645, 0, 4, 'You have no idea how happy I am to see you!', 12, 0, 100, 0, 0, 0, 'Captured Demolitionist - SAY_0');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42645;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42645;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42645, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Demolitionist - On Spawn - Set Data 1 0'),
(42645, 0, 1, 0, 38, 0, 100, 0, 1, 1, 1000, 1000, 0, 80, 4264500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Demolitionist - Data Set - Say 1');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4264500;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4264500, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Demolitionist - On Script - SAY_0'),
(4264500, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 46, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Demolitionist - On Script - Move Forward'),
(4264500, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Demolitionist - On Script - Despawn');

-- Missing in Action
UPDATE `quest_template` SET `OfferRewardText`='You have me own thanks and th\' thanks of th\' men you rescued. I\'ve already had th\' boys start setting up the gear down inside the cave. It should be ready to go soon.' WHERE `Id`=26284;
