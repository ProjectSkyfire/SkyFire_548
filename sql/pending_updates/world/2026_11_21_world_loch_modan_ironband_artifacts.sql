-- Loch Modan: "Keep Your Hands Off The Goods!" (13650). The three artifact markers at the Ironband's Excavation Site have no
-- script, so approaching them gave no objective credit. Each marker credits players within 10 yards once a second. Based on
-- Legends of Azeroth 5.4.8.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (33485, 33486, 33487);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (33485, 33486, 33487);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(33485, 0, 0, 0, 1, 0, 100, 0, 0, 0, 1000, 1000, 0, 33, 33485, 0, 0, 0, 0, 0, 17, 0, 10, 0, 0, 0, 0, 0, 'Ironband Sandal Artifact Bunny - Out Of Combat - Give Kill Credit To Players In Range'),
(33486, 0, 0, 0, 1, 0, 100, 0, 0, 0, 1000, 1000, 0, 33, 33486, 0, 0, 0, 0, 0, 17, 0, 10, 0, 0, 0, 0, 0, 'Ironband Liberty Artifact Bunny - Out Of Combat - Give Kill Credit To Players In Range'),
(33487, 0, 0, 0, 1, 0, 100, 0, 0, 0, 1000, 1000, 0, 33, 33487, 0, 0, 0, 0, 0, 17, 0, 10, 0, 0, 0, 0, 0, 'Ironband Tablet Artifact Bunny - Out Of Combat - Give Kill Credit To Players In Range');
