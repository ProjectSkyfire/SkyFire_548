-- Restore Craggle Wobbletop's shop and Trade District canal patrol.
-- https://www.wowhead.com/mop-classic/npc=52358/craggle-wobbletop
-- https://www.wowhead.com/mop-classic/quest=29117/lets-go-fly-a-kite
-- Menu 12759 is shared with Blax Bottlerocket: retain one working shop option.
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 12759 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0;
UPDATE `gossip_menu_option`
SET `OptionType` = 3, `OptionNpcflag` = 128
WHERE `MenuID` = 12759 AND `OptionID` = 3;

INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`)
VALUES
(52358, 0, 68890, 0, 0, 0, 1),
(52358, 0, 69057, 0, 0, 0, 1),
(52358, 0, 69895, 0, 0, 0, 1),
(52358, 0, 69896, 0, 0, 0, 1),
(52358, 0, 95481, 0, 0, 0, 1),
(52358, 0, 95482, 0, 0, 0, 1),
(52358, 0, 95621, 0, 0, 0, 1),
(52358, 0, 104323, 0, 0, 0, 1),
(52358, 0, 104324, 0, 0, 0, 1)
ON DUPLICATE KEY UPDATE `item` = VALUES(`item`);

-- These quest-only toys must not be offered without their corresponding quests.
-- https://www.wowhead.com/mop-classic/item=69057/foam-sword-rack
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 52358 AND `SourceEntry` IN (68890, 69057);
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(23, 52358, 68890, 0, 0, 9, 0, 29117, 0, 0, 0, 0, 0, '', 'Craggle Wobbletop - Dragon Kite 2-Pack requires Let us Go Fly a Kite'),
(23, 52358, 69057, 0, 0, 9, 0, 171, 0, 0, 0, 0, 0, '', 'Craggle Wobbletop - Foam Sword Rack requires A Warden of the Alliance');

SET @GUID := 30858;
SET @PATH := @GUID * 10;
UPDATE `creature`
SET `position_x` = -8742.11, `position_y` = 565.326, `position_z` = 97.4465,
    `orientation` = 0.7496, `MovementType` = 2, `spawndist` = 0, `currentwaypoint` = 0
WHERE `guid` = @GUID AND `id` = 52358 AND `map` = 0;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
SELECT `c`.`guid`, @PATH, `a`.`mount`, `a`.`bytes1`, `a`.`bytes2`, 379, `a`.`auras`
FROM `creature` AS `c`
JOIN `creature_template_addon` AS `a` ON `a`.`entry` = `c`.`id`
WHERE `c`.`guid` = @GUID AND `c`.`id` = 52358 AND `c`.`map` = 0
ON DUPLICATE KEY UPDATE `path_id` = @PATH, `emote` = 379;
DELETE FROM `waypoint_data` WHERE `id` = @PATH;
INSERT INTO `waypoint_data`
(`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`)
VALUES
(@PATH, 1, -8742.11, 565.326, 97.4465, 0.0, 0, 0, 0, 100, 0),
(@PATH, 2, -8724.28, 581.938, 98.0096, 0.0, 0, 0, 0, 100, 0),
(@PATH, 3, -8706.55, 599.998, 99.2008, 0.0, 0, 0, 0, 100, 0),
(@PATH, 4, -8701.12, 615.582, 99.9045, 0.0, 0, 0, 0, 100, 0),
(@PATH, 5, -8702.06, 636.189, 100.263, 0.0, 0, 0, 0, 100, 0),
(@PATH, 6, -8706.83, 653.502, 99.8812, 0.0, 0, 0, 0, 100, 0),
(@PATH, 7, -8716.93, 673.677, 98.9862, 0.0, 0, 0, 0, 100, 0),
(@PATH, 8, -8731.68, 694.602, 99.026, 0.0, 0, 0, 0, 100, 0),
(@PATH, 9, -8744.11, 710.651, 98.6385, 0.0, 0, 0, 0, 100, 0),
(@PATH, 10, -8756.38, 726.195, 98.4693, 0.0, 0, 0, 0, 100, 0),
(@PATH, 11, -8772.68, 740.035, 99.5238, 0.0, 0, 0, 0, 100, 0),
(@PATH, 12, -8784.97, 744.403, 99.008, 0.0, 0, 0, 0, 100, 0),
(@PATH, 13, -8806.59, 741.773, 97.7635, 0.0, 0, 0, 0, 100, 0),
(@PATH, 14, -8831.72, 728.413, 98.3643, 0.0, 0, 0, 0, 100, 0),
(@PATH, 15, -8850.83, 719.234, 97.5135, 0.0, 0, 0, 0, 100, 0),
(@PATH, 16, -8874.03, 709.74, 97.9275, 0.0, 0, 0, 0, 100, 0),
(@PATH, 17, -8888.76, 701.622, 98.5337, 0.0, 0, 0, 0, 100, 0),
(@PATH, 18, -8897.93, 696.422, 98.9529, 0.0, 0, 0, 0, 100, 0),
(@PATH, 19, -8888.76, 701.622, 98.5337, 0.0, 0, 0, 0, 100, 0),
(@PATH, 20, -8874.03, 709.74, 97.9275, 0.0, 0, 0, 0, 100, 0),
(@PATH, 21, -8850.83, 719.234, 97.5135, 0.0, 0, 0, 0, 100, 0),
(@PATH, 22, -8831.72, 728.413, 98.3643, 0.0, 0, 0, 0, 100, 0),
(@PATH, 23, -8806.59, 741.773, 97.7635, 0.0, 0, 0, 0, 100, 0),
(@PATH, 24, -8784.97, 744.403, 99.008, 0.0, 0, 0, 0, 100, 0),
(@PATH, 25, -8772.68, 740.035, 99.5238, 0.0, 0, 0, 0, 100, 0),
(@PATH, 26, -8756.38, 726.195, 98.4693, 0.0, 0, 0, 0, 100, 0),
(@PATH, 27, -8744.11, 710.651, 98.6385, 0.0, 0, 0, 0, 100, 0),
(@PATH, 28, -8731.68, 694.602, 99.026, 0.0, 0, 0, 0, 100, 0),
(@PATH, 29, -8716.93, 673.677, 98.9862, 0.0, 0, 0, 0, 100, 0),
(@PATH, 30, -8706.83, 653.502, 99.8812, 0.0, 0, 0, 0, 100, 0),
(@PATH, 31, -8702.06, 636.189, 100.263, 0.0, 0, 0, 0, 100, 0),
(@PATH, 32, -8701.12, 615.582, 99.9045, 0.0, 0, 0, 0, 100, 0),
(@PATH, 33, -8706.55, 599.998, 99.2008, 0.0, 0, 0, 0, 100, 0),
(@PATH, 34, -8724.08, 582.176, 97.8885, 0.0, 0, 0, 0, 100, 0);
