UPDATE `creature_template` SET `ScriptName`='' WHERE  `entry`=3052;

DELETE FROM `conditions` WHERE `SourceGroup` IN (24) AND `SourceTypeOrReferenceId` = 15;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 24, 0, 0, 0, 8, 0, 770, 0, 0, 1, 0, 0, '', "Skorn Whitecloud - Gossip Menu Option - Quest Not Rewarded");
