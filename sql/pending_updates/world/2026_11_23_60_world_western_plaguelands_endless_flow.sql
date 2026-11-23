-- The Endless Flow (26922, 27161): the hand grenades (spell 83108) hit any creature in the blast and nothing marked
-- the Scourge Bone Animus as destroyed, so the first objective (credit 44360) could never be completed. The grenade
-- damage is limited to the Stickbone Berserkers (condition on effect 0) and its dummy effect reaches the animus
-- bunny, which now gives the credit and goes away. Both animus bunnies are spawned.
-- Rows and condition are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 44360 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` = 44360 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44360, 0, 0, 1, 8, 0, 100, 1, 83108, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scourge Bone Animus Bunny - On Spellhit Hand Grenade - Despawn'),
(44360, 0, 1, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 44360, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Scourge Bone Animus Bunny - Linked To Id 0 - Quest Credit');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 83108;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 83108, 0, 0, 31, 0, 3, 44329, 0, 0, 0, 0, '', 'Hand Grenade damage can hit only Stickbone Berserkers');
