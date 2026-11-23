-- Do the Right Thing (28189): the Shovel (89089) kills what it is used on, and the quest counts 15 Human Seedlings
-- "saved". The spell had no target condition, so it could be used on any creature in range. Added as in the Cataclysm
-- Preservation Project 4.3.4 and Legends of Azeroth 5.4.8: the shovel can only target a Human Seedling (47872).
-- The click row that Legends of Azeroth 5.4.8 has on the seedling (spell 89552, "Seedling Grows to Sapling") is not
-- added: it only summons a Sapling Protector (48042), an entry this core uses for another quest with different data,
-- and it gives no credit.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 89089;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 89089, 0, 0, 31, 1, 3, 47872, 0, 0, 0, 0, '', 'The Shovel can only be used on a Human Seedling');
