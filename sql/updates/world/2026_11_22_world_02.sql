-- Restrict class services to the classes taught by each menu's trainers.
-- Shared menu 0 also serves profession trainers: its filtering belongs in core.
-- Ordinary gossip, quests, vendor services and pet trainers are not changed.
CREATE TEMPORARY TABLE `class_trainer_gossip_masks` AS
SELECT options.`MenuID`, options.`OptionID`, BIT_OR(1 << (trainer.`trainer_class` - 1)) AS `ClassMask`
FROM `creature_template` AS trainer
JOIN `gossip_menu_option` AS options ON options.`MenuID` = trainer.`gossip_menu_id`
WHERE trainer.`trainer_type` = 0 AND trainer.`trainer_class` BETWEEN 1 AND 11
  AND (trainer.`npcflag` & 16) <> 0 AND options.`MenuID` <> 0
  AND (options.`OptionType` IN (5, 16, 18, 20)
       OR (options.`OptionType` = 1 AND options.`ActionMenuID` IN (10371, 9791)))
GROUP BY options.`MenuID`, options.`OptionID`;

-- Preserve every existing OR branch and its level/quest/other requirements.
CREATE TEMPORARY TABLE `class_trainer_gossip_branches` AS
SELECT DISTINCT masks.`MenuID`, masks.`OptionID`, masks.`ClassMask`,
       COALESCE(existing.`ElseGroup`, 0) AS `ElseGroup`
FROM `class_trainer_gossip_masks` AS masks
LEFT JOIN `conditions` AS existing ON existing.`SourceTypeOrReferenceId` = 15
  AND existing.`SourceGroup` = masks.`MenuID` AND existing.`SourceEntry` = masks.`OptionID`
  AND existing.`SourceId` = 0;

-- Replace incorrect player-class masks (e.g. menu 4463 was paladin-only).
-- For menus shared across classes, core also checks the actual NPC's class.
DELETE existing FROM `conditions` AS existing
JOIN `class_trainer_gossip_masks` AS masks
  ON existing.`SourceGroup` = masks.`MenuID` AND existing.`SourceEntry` = masks.`OptionID`
WHERE existing.`SourceTypeOrReferenceId` = 15 AND existing.`SourceId` = 0
  AND existing.`ConditionTypeOrReference` = 15 AND existing.`ConditionTarget` = 0;

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
SELECT 15, `MenuID`, `OptionID`, 0, `ElseGroup`, 15, 0, `ClassMask`, 0, 0, 0, 0, 0, '',
       'Class trainer service: require a class taught by this menu'
FROM `class_trainer_gossip_branches`;

DROP TEMPORARY TABLE `class_trainer_gossip_branches`;
DROP TEMPORARY TABLE `class_trainer_gossip_masks`;
