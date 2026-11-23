-- Buried Blades (27467), Duskwing, Oh How I Hate Thee... (27523) and Scourged Mass (27528): the quest item spells
-- (Light-Touched Blades 85252, The Corpseburner's Flare 85431) could be used on any target,
-- so the blades could be buried in anything, the flare was wasted on any creature and the torch burned anything.
-- Each spell is now limited to its target (Slain Scourge Trooper, Duskwing, Overstuffed Golem). Duskwing also runs to
-- the player when the flare hits him, as in the source; his existing Strike row is kept.
-- Rows and conditions are as in Legends of Azeroth 5.4.8.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 11897 AND `source_type` = 0 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11897, 0, 1, 2, 8, 0, 100, 0, 85431, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Duskwing - On Spellhit Corpseburner''s Flare - Set Run'),
(11897, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Duskwing - Linked To Id 1 - Move To Invoker');

DELETE FROM `conditions` WHERE `SourceEntry` IN (85252, 85431) AND `SourceTypeOrReferenceId` IN (13, 17);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 85252, 0, 0, 31, 1, 3, 45695, 0, 0, 0, 0, '', 'Light-Touched Blades can hit Slain Scourge Troopers'),
(13, 1, 85431, 0, 0, 31, 0, 3, 11897, 0, 0, 0, 0, '', 'Corpseburner''s Flare can hit Duskwing');
