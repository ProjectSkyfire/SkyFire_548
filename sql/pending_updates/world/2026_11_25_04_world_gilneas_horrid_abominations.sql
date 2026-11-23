-- Gilneas, You Can't Take 'Em Alone (14348): the credit objective (Horrid Abomination Credit, 36233) had no source, so
-- the quest could not be completed. Throwing a Black Gunpowder Keg (item spell 69094, trigger 68555) at an Horrid
-- Abomination makes it stop, say a line, and four seconds later explode: the throwing player gets the credit, the
-- abomination bursts into gibs and despawns. The keg spells only work on an abomination that does not carry the keg
-- aura yet.
-- Based on Legends of Azeroth 5.4.8 (smart scripts, texts and spell conditions).

-- 1. Target conditions: the keg spell needs an Horrid Abomination as target, and so does the aura it triggers, which
--    also must not be on the target already.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` IN (68555, 69094);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 68555, 0, 0, 1, 1, 68555, 0, 0, 1, 0, 0, '', 'Keg Placed - Target has no keg yet'),
(17, 0, 68555, 0, 0, 31, 1, 3, 36231, 0, 0, 0, 0, '', 'Keg Placed - Target Horrid Abomination'),
(17, 0, 69094, 0, 0, 31, 1, 3, 36231, 0, 0, 0, 0, '', 'Black Gunpowder Keg - Target Horrid Abomination');

-- 2. The abomination script.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 36231;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 36231;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36231, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - On Respawn - Remove Unit Flag 33554432'),
(36231, 0, 1, 2, 8, 0, 100, 0, 68555, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - On Spell Hit (Keg Placed) - Store Target List'),
(36231, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Stop Auto Attack'),
(36231, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Disable Combat Movement'),
(36231, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 4000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Say Text Line 0'),
(36231, 0, 5, 6, 52, 0, 100, 0, 0, 36231, 0, 0, 0, 33, 36233, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - On Text Over - Give Quest Credit'),
(36231, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 68560, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Horrid Abomination Explosion'),
(36231, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Set Unit Flag 33554432'),
(36231, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42266, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Poison'),
(36231, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42274, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Bone 2'),
(36231, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42266, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Poison'),
(36231, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42267, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Bone'),
(36231, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42266, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Poison'),
(36231, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42274, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Bone 2'),
(36231, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42266, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Poison'),
(36231, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42274, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Bone 2'),
(36231, 0, 16, 17, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42266, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Poison'),
(36231, 0, 17, 18, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42267, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Bone'),
(36231, 0, 18, 19, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42266, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Cast Random Circumference Point Poison'),
(36231, 0, 19, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horrid Abomination - Linked - Despawn');

DELETE FROM `creature_text` WHERE `entry` = 36231;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(36231, 0, 0, 'Barrel smell like gunpowder...', 12, 0, 100, 0, 0, 20909, 'Horrid Abomination - Keg Placed'),
(36231, 0, 1, 'GAH!!!!  I CAN''T SEE IN HERE!!!!', 12, 0, 100, 0, 0, 20908, 'Horrid Abomination - Keg Placed'),
(36231, 0, 2, 'Uh-oh... this gonna hurts me...', 12, 0, 100, 0, 0, 20911, 'Horrid Abomination - Keg Placed'),
(36231, 0, 3, 'This not be good...', 12, 0, 100, 0, 0, 20911, 'Horrid Abomination - Keg Placed'),
(36231, 0, 4, 'I gots bad feeling...', 12, 0, 100, 0, 0, 20911, 'Horrid Abomination - Keg Placed'),
(36231, 0, 5, 'Uh-oh... this gonna hurts me...', 12, 0, 100, 0, 0, 20911, 'Horrid Abomination - Keg Placed');
