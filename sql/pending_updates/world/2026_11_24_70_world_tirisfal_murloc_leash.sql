-- The Vile Fin capture quest (24974, Sedrick Calston): the Murloc Leash (73108) summons a Captured Vile Fin
-- Puddlejumper (38923) or Minor Oracle (39078) for the player, but neither had a script, so the capture gave no credit and bringing the murloc to Sedrick
-- Calston gave none either. The scripts are those of the Cataclysm Preservation Project 4.3.4 (the summoned murloc gives
-- the capture credit, turns passive and immune to creatures, gets the rope beam from its owner; within 2 yards of
-- Sedrick it gives the delivery credit and leaves). The action ids differ in this core: the invoker cast is 85 here.
-- Both references limit the Leash to the two murloc entries; as in the 4.3.4 data it also needs the murloc at under 30
-- percent health, as the quest says "weaken it first". The immune flag is 768 (players and creatures), as the 5.4.8
-- script sets it.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (38923, 39078) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE (`entryorguid` IN (38923, 39078) AND `source_type` = 0) OR (`entryorguid` IN (3892300, 3892301, 3907800, 3907801) AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38923, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3892300, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'PuddlejumperC - On Spawn - Actionlist'),
(38923, 0, 1, 0, 75, 0, 100, 1, 0, 38925, 2, 0, 0, 80, 3892301, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'PuddlejumperC - Near Sedrick Calston - Actionlist'),
(39078, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3907800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'OracleC - On Spawn - Actionlist'),
(39078, 0, 1, 0, 75, 0, 100, 1, 0, 38925, 2, 0, 0, 80, 3907801, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'OracleC - Near Sedrick Calston - Actionlist'),
(3892300, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'PuddlejumperC - On Script - Set React Passive'),
(3892300, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'PuddlejumperC - On Script - Set Immune To Players And Creatures'),
(3892300, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38923, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'PuddlejumperC - Actionlist - Credit quest'),
(3892300, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 85, 82922, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'PuddlejumperC - On spawn - Cast cosmetic chains'),
(3892301, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38887, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'PuddlejumperC - Actionlist - Credit quest'),
(3892301, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'PuddlejumperC - Actionlist - Despawn'),
(3907800, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'OracleC - On Script - Set React Passive'),
(3907800, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'OracleC - On Script - Set Immune To Players And Creatures'),
(3907800, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38923, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'OracleC - Actionlist - Credit quest'),
(3907800, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 85, 82922, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'OracleC - On spawn - Cast cosmetic chains'),
(3907801, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38887, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'OracleC - Actionlist - Credit quest'),
(3907801, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'OracleC - Actionlist - Despawn');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceGroup` = 0 AND `SourceEntry` = 73108;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 73108, 0, 0, 31, 1, 3, 1544, 0, 0, 0, 0, '', 'The Murloc Leash can target only a Vile Fin Minor Oracle'),
(17, 0, 73108, 0, 0, 38, 1, 30, 2, 0, 0, 0, 0, '', 'The Murloc Leash needs the target below 30 percent health'),
(17, 0, 73108, 0, 1, 31, 1, 3, 1543, 0, 0, 0, 0, '', 'The Murloc Leash can target only a Vile Fin Puddlejumper'),
(17, 0, 73108, 0, 1, 38, 1, 30, 2, 0, 0, 0, 0, '', 'The Murloc Leash needs the target below 30 percent health');
