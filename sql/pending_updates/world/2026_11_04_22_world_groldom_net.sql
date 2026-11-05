-- Drag it Out of Them (13961): the netted Razormane Pillager had no gossip, so it could not be talked into following
-- the player and turned hostile again when the net wore off. The Grol'dom Net could also be thrown at players; it only
-- takes Razormane Pillagers now.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 10519 WHERE `entry` = 34503;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceGroup` = 0 AND `SourceEntry` = 65580;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 65580, 0, 0, 31, 1, 3, 34503, 0, 0, 13, 0, '', 'Grol''dom Net - only on a Razormane Pillager');
