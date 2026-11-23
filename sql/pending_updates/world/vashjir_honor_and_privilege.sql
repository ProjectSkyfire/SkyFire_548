-- Vashj'ir, "Honor and Privilege" (25972): swim up to the rescue balloon on the surface (credit 40921), take the flare gun and
-- fire the Rescue Flare (item 56188, spell 77741) at the Horde ships (credit 41572, the Rescue Balloon). Nothing credited either
-- objective. The area trigger 5984 sits on the balloon at the surface (-7311, 4235, -1); the flare spell is a dummy on a nearby
-- unit (target type 38), so a condition names the balloon as its target and the balloon credits the caster when it is hit.

DELETE FROM `areatrigger_scripts` WHERE `entry` = 5984;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(5984, 'SmartTrigger');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 77741;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 77741, 0, 0, 31, 0, 3, 41572, 0, 0, 0, 0, '', 'Rescue Flare - Rescue Balloon');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 41572;
DELETE FROM `smart_scripts` WHERE `source_type` = 2 AND `entryorguid` = 5984;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41572;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5984, 2, 0, 0, 46, 0, 100, 0, 5984, 0, 0, 0, 0, 33, 40921, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger 5984 - On Trigger - Quest Credit Blood Guard Toldrek'),
(41572, 0, 0, 0, 8, 0, 100, 0, 77741, 0, 0, 0, 0, 33, 41572, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Spellhit (Rescue Flare) - Quest Credit Rescue Balloon');
