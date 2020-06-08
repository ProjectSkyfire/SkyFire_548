DELETE FROM `quest_objective` WHERE `questId`=31539;
INSERT INTO `quest_objective` (`questId`, `id`, `index`, `type`, `objectId`, `amount`, `flags`, `description`) VALUES
(31539, 268700, 0, 1, 87811, 1, 0, '');


UPDATE `quest_template` SET `RequiredSkillId`=773, `RequiredSkillPoints`=1 WHERE `Id`=31539;
