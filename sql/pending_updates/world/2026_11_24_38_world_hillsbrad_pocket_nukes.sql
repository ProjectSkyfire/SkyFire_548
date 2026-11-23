-- Breaking the Hand (28556): a Goblin Pocket-Nuke (spell 91085) is placed at each of the four fingers, Sofera's Naze,
-- Corrahn's Dagger, the Headland and Gavin's Naze. The four credit markers had no script and the spell could be cast
-- anywhere, so no nuke ever counted. Added as in Legends of Azeroth 5.4.8: each marker credits its own entry when the
-- spell hits it, and the spell can only hit those four markers. The nuke the spell summons (48846) gets the vehicle id
-- and unit flags of the source.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (48862, 48864, 48865, 48867);
UPDATE `creature_template` SET `VehicleId` = 1356, `unit_flags2` = 67110912 WHERE `entry` = 48846;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 48862 AND `source_type` = 0 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48864 AND `source_type` = 0 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48865 AND `source_type` = 0 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48867 AND `source_type` = 0 AND `id` IN (0);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48862, 0, 0, 0, 8, 0, 100, 0, 91085, 0, 0, 0, 0, 33, 48862, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spellhit 91085 - kill credit 48862'),
(48864, 0, 0, 0, 8, 0, 100, 0, 91085, 0, 0, 0, 0, 33, 48864, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spellhit 91085 - kill credit 48864'),
(48865, 0, 0, 0, 8, 0, 100, 0, 91085, 0, 0, 0, 0, 33, 48865, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spellhit 91085 - kill credit 48865'),
(48867, 0, 0, 0, 8, 0, 100, 0, 91085, 0, 0, 0, 0, 33, 48867, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On spellhit 91085 - kill credit 48867');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 91085;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 91085, 0, 0, 31, 0, 3, 48862, 0, 0, 0, 0, '', 'Goblin Pocket-Nuke can hit Sofera''s Naze'),
(13, 1, 91085, 0, 1, 31, 0, 3, 48864, 0, 0, 0, 0, '', 'Goblin Pocket-Nuke can hit Corrahn''s Dagger'),
(13, 1, 91085, 0, 2, 31, 0, 3, 48865, 0, 0, 0, 0, '', 'Goblin Pocket-Nuke can hit the Headland'),
(13, 1, 91085, 0, 3, 31, 0, 3, 48867, 0, 0, 0, 0, '', 'Goblin Pocket-Nuke can hit Gavin''s Naze');
