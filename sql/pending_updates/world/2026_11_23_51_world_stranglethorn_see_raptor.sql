-- See Raptor (26359, 26773): the objective "Mind Vision complete" (credit 42840) is given by the first option of Priestess
-- Hu'rala's and Priestess Thaalia's menus, "I am ready for the Mind Vision ritual". Neither priestess had a script, so
-- the option did nothing and the quest could not be finished. The option is shown only while the quest is taken.
-- Legends of Azeroth 5.4.8 does this in C++ (npc_stranglethorn_priestess_hurala: the same credit and the same quest
-- check, with a note that the ritual itself, a teleport into Zul'Gurub, is not done); the rows here do the same.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (42812, 44017) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (42812, 44017);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42812, 0, 0, 1, 62, 0, 100, 0, 11661, 0, 0, 0, 0, 33, 42840, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Priestess Hu''rala - On Gossip Select - Quest Credit'),
(42812, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Priestess Hu''rala - On Gossip Select - Close Gossip'),
(44017, 0, 0, 1, 62, 0, 100, 0, 11785, 0, 0, 0, 0, 33, 42840, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Priestess Thaalia - On Gossip Select - Quest Credit'),
(44017, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Priestess Thaalia - On Gossip Select - Close Gossip');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (11661, 11785) AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11661, 0, 0, 0, 9, 0, 26359, 0, 0, 0, 0, 0, '', 'Priestess Hu''rala - Mind Vision only with See Raptor'),
(15, 11785, 0, 0, 0, 9, 0, 26773, 0, 0, 0, 0, 0, '', 'Priestess Thaalia - Mind Vision only with See Raptor');
