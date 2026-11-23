-- Soothing Spirits (26777): the Soothing Incense Cloud (spell 82194, the Holy Censer) calms the five Forlorn Spirits. The
-- cloud credits the player as soon as it is summoned, and nothing limited the spell to the spirits, so the five credits
-- could be had anywhere. The spell can now only be cast within 5 yards of a Forlorn Spirit (43923), as in Legends of
-- Azeroth 5.4.8.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 82194;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 82194, 0, 0, 29, 0, 43923, 5, 0, 0, 0, 0, '', 'Soothing Incense Cloud - Caster is near a Forlorn Spirit');
