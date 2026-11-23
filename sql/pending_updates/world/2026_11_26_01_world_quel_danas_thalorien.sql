-- Isle of Quel'Danas: "Journey To The Sunwell" (24522, 24562) and "Thalorien Dawnseeker" (24535, 24563) could not be
-- turned in or done. Halduron Brightwing, who ends the first pair and gives and ends the second, and Thalorien
-- Dawnseeker's Remains were not spawned, and the remains had no script, so examining them gave no credit.
-- Based on Legends of Azeroth 5.4.8 (spawns, script and gossip conditions); the Cataclysm Preservation Project 4.3.4
-- has the same two spawns. Spell 70265 gives the kill credit of Thalorien Dawnseeker Credit (37601).
DELETE FROM `creature` WHERE `guid` IN (9311600, 9311601);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9311600, 37527, 530, 1, 0, 0, 0, 1, 12559.3, -6781.66, 14.9905, 0.888647, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9311601, 37552, 530, 1, 0, 0, 0, 0, 11781.6, -7068.71, 24.9312, 3.32152, 120, 0, 0, 0, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37552;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=37552;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37552, 0, 0, 1, 62, 0, 100, 0, 10908, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thalorien Dawnseeker''s Remains - On gossip option 0 select - Close gossip'),
(37552, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 85, 70265, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Thalorien Dawnseeker''s Remains - On gossip option 0 select - Player cast Thalorien Dawnseeker Credit on self');

-- The option only shows while one of the two quests (Alliance 24535, Horde 24563) is taken.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=10908 AND `SourceEntry`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 10908, 0, 0, 0, 9, 0, 24535, 0, 0, 0, 0, 0, '', 'Thalorien Dawnseeker''s Remains - Option requires quest 24535 taken'),
(15, 10908, 0, 0, 1, 9, 0, 24563, 0, 0, 0, 0, 0, '', 'Thalorien Dawnseeker''s Remains - Option requires quest 24563 taken');
