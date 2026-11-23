-- Making Stumps (13815): the Splintertree Axe (64605) picks a nearby creature by entry (implicit target 38), which needs a
-- condition naming the Ashenvale Oak (34167). Without it the axe hit nothing, so the oak's SmartAI never gave the credit.
-- Based on Legends of Azeroth 5.4.8. Safe to run twice.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 64605;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 64605, 0, 0, 31, 0, 3, 34167, 0, 0, 0, 0, '', 'Splintertree Axe targets the Ashenvale Oak');
