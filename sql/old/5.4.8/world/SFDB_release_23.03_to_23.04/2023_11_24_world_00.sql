UPDATE `quest_template` SET `PrevQuestId`=29417 WHERE `Id`=29418;
UPDATE `quest_template` SET `PrevQuestId`=29417 WHERE `Id`=29523;

DELETE FROM `creature_addon` WHERE `guid`=224775;
INSERT INTO `creature_addon` (`guid`, `auras`) VALUES
(224775, '108900 84886 126160');

DELETE FROM `spell_area` WHERE `spell`=102398;
INSERT INTO `spell_area` (`spell`, `area`, `quest_end`, `autocast`, `quest_end_status`) VALUES
(102398, 5835, 29523, 1, 66);
