-- Heartfelt Appreciation requires all six Explorers' League documents to be returned.
-- Keep Method = 0: the final quest is accepted and completed immediately at Torren.
-- https://github.com/ProjectSkyfire/SkyFire_548/issues/1514
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` = 0 AND `SourceEntry` = 13661 AND `SourceId` = 0;

-- Shared ElseGroup makes the six QUEST_REWARDED conditions an AND requirement.
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`,
 `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`,
 `ConditionValue1`, `ConditionValue2`, `ConditionValue3`,
 `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19, 0, 13661, 0, 0, 8, 0, 13655, 0, 0, 0, 0, 0, '', 'Heartfelt Appreciation requires Explorers'' League Document (2 of 6) rewarded'),
(19, 0, 13661, 0, 0, 8, 0, 13656, 0, 0, 0, 0, 0, '', 'Heartfelt Appreciation requires Explorers'' League Document (1 of 6) rewarded'),
(19, 0, 13661, 0, 0, 8, 0, 13657, 0, 0, 0, 0, 0, '', 'Heartfelt Appreciation requires Explorers'' League Document (3 of 6) rewarded'),
(19, 0, 13661, 0, 0, 8, 0, 13658, 0, 0, 0, 0, 0, '', 'Heartfelt Appreciation requires Explorers'' League Document (4 of 6) rewarded'),
(19, 0, 13661, 0, 0, 8, 0, 13659, 0, 0, 0, 0, 0, '', 'Heartfelt Appreciation requires Explorers'' League Document (6 of 6) rewarded'),
(19, 0, 13661, 0, 0, 8, 0, 13660, 0, 0, 0, 0, 0, '', 'Heartfelt Appreciation requires Explorers'' League Document (5 of 6) rewarded');

DELETE FROM `creature_queststarter` WHERE `id` = 1153 AND `quest` = 13661;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (1153, 13661);
