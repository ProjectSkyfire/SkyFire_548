-- Feralas: quests that still could not be completed: Gordunni Orb drops, the Glowing Soil loot, the spell targets of
-- Ysondre's Tear and the giant vessel, the two flight masters and Seyala's option, and the OOX-22/FE escort. Safe to run
-- twice.
-- FE-1: Gordunni Orb drops.
DELETE FROM `creature_loot_template`
WHERE (`entry`, `item`) IN ((5236, 52832), (5240, 52832));
INSERT INTO `creature_loot_template`
    (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (5236, 52832, -16.6629, 1, 0, 1, 1),
    (5240, 52832, -17.2689, 1, 0, 1, 1);

-- FE-2: Use the maintainer's existing Glowing Soil loot row.
UPDATE `gameobject_template`
SET `data1` = 202793
WHERE `entry` = 202793 AND `data1` = 0;

-- FE-4: Target the Dream Portal Bunny with Ysondre's Tear.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1
  AND `SourceEntry` = 74409 AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 31 AND `ConditionTarget` = 0;
INSERT INTO `conditions`
    (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`,
     `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`,
     `ConditionValue1`, `ConditionValue2`, `ConditionValue3`,
     `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
    (13, 1, 74409, 0, 0, 31, 0, 3, 39834, 0, 0, 0, 0, '',
     'Seal Dream Portal - target Dream Portal Bunny');

-- FE-5: Accept a dead Mountain Giant as another vessel target.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 17 AND `SourceGroup` = 0
  AND `SourceEntry` = 11889 AND `SourceId` = 0 AND `ElseGroup` = 4
  AND `ConditionTarget` = 1 AND `ConditionTypeOrReference` IN (31, 36);
INSERT INTO `conditions`
    (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`,
     `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`,
     `ConditionValue1`, `ConditionValue2`, `ConditionValue3`,
     `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
    (17, 0, 11889, 0, 4, 36, 1, 0, 0, 0, 1, 0, 0, '',
     'Capture Mountain Giant can be cast on dead targets'),
    (17, 0, 11889, 0, 4, 31, 1, 3, 40026, 0, 0, 0, 0, '',
     'Capture Mountain Giant target Mountain Giant');

-- FE-9: Enable the flight-map options and limit Seyala's quest option.
UPDATE `gossip_menu_option`
SET `OptionType` = 4, `OptionNpcflag` = 8192
WHERE `MenuID` IN (11304, 11373) AND `OptionID` = 0
  AND `OptionType` = 1 AND `OptionNpcflag` = 1;

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 11373
  AND `SourceEntry` = 1 AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 9 AND `ConditionTarget` = 0;
INSERT INTO `conditions`
    (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`,
     `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`,
     `ConditionValue1`, `ConditionValue2`, `ConditionValue3`,
     `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
    (15, 11373, 1, 0, 0, 9, 0, 25481, 0, 0, 0, 0, 0, '',
     'Show gossip option only with quest 25481 taken');

-- FE-10: Start the existing escort chain and despawn after the final line.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 7807 AND `source_type` = 0 AND `id` IN (1, 9);
INSERT INTO `smart_scripts`
    (`entryorguid`, `source_type`, `id`, `link`, `event_type`,
     `event_phase_mask`, `event_chance`, `event_flags`,
     `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
     `action_type`, `action_param1`, `action_param2`, `action_param3`,
     `action_param4`, `action_param5`, `action_param6`,
     `target_type`, `target_param1`, `target_param2`, `target_param3`,
     `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
    (7807, 0, 1, 2, 19, 0, 100, 0, 25476, 0, 0, 0, 0,
     8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
     'Homing Robot OOX-22/FE - On Accepted Quest - Set React State Aggressive'),
    (7807, 0, 9, 0, 52, 0, 100, 0, 3, 7807, 0, 0, 0,
     41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
     'Homing Robot OOX-22/FE - On Text Over 3 - Despawn');
