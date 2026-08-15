-- Persist loader-side quest/objective cleanup reported in DBErrors.

-- Quests with MinLevel -1 are intended to stay disabled.
INSERT IGNORE INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`)
SELECT 1, `Id`, 0, '', '', CONCAT('DBErrors: disabled quest with MinLevel ', `MinLevel`)
FROM `quest_template`
WHERE `MinLevel` = -1;

-- Daily and weekly flags cannot coexist; daily/weekly/monthly quests must be repeatable.
UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`Flags` = qt.`Flags` & 0xFFFFEFFF
WHERE dq.`entry` IS NULL
  AND (qt.`Flags` & 0x1000) <> 0
  AND (qt.`Flags` & 0x8000) <> 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`SpecialFlags` = qt.`SpecialFlags` | 0x001
WHERE dq.`entry` IS NULL
  AND ((qt.`Flags` & 0x1000) <> 0 OR (qt.`Flags` & 0x8000) <> 0 OR (qt.`SpecialFlags` & 0x010) <> 0)
  AND (qt.`SpecialFlags` & 0x001) = 0;

-- Clear source item references that no longer exist.
UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`SourceItemId`
SET qt.`SourceItemId` = 0
WHERE dq.`entry` IS NULL
  AND qt.`SourceItemId` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RequiredSourceItemId1`
SET qt.`RequiredSourceItemId1` = 0,
    qt.`RequiredSourceItemCount1` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RequiredSourceItemId1` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RequiredSourceItemId2`
SET qt.`RequiredSourceItemId2` = 0,
    qt.`RequiredSourceItemCount2` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RequiredSourceItemId2` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RequiredSourceItemId3`
SET qt.`RequiredSourceItemId3` = 0,
    qt.`RequiredSourceItemCount3` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RequiredSourceItemId3` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RequiredSourceItemId4`
SET qt.`RequiredSourceItemId4` = 0,
    qt.`RequiredSourceItemCount4` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RequiredSourceItemId4` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RequiredSourceItemCount1` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RequiredSourceItemId1` = 0
  AND qt.`RequiredSourceItemCount1` <> 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RequiredSourceItemCount2` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RequiredSourceItemId2` = 0
  AND qt.`RequiredSourceItemCount2` <> 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RequiredSourceItemCount3` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RequiredSourceItemId3` = 0
  AND qt.`RequiredSourceItemCount3` <> 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RequiredSourceItemCount4` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RequiredSourceItemId4` = 0
  AND qt.`RequiredSourceItemCount4` <> 0;

-- Remove fixed and choice reward item references that are absent from item_template.
UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardItemId1`
SET qt.`RewardItemId1` = 0,
    qt.`RewardItemCount1` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardItemId1` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardItemId2`
SET qt.`RewardItemId2` = 0,
    qt.`RewardItemCount2` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardItemId2` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardItemId3`
SET qt.`RewardItemId3` = 0,
    qt.`RewardItemCount3` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardItemId3` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardItemId4`
SET qt.`RewardItemId4` = 0,
    qt.`RewardItemCount4` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardItemId4` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardChoiceItemId1`
SET qt.`RewardChoiceItemId1` = 0,
    qt.`RewardChoiceItemCount1` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardChoiceItemId1` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardChoiceItemId2`
SET qt.`RewardChoiceItemId2` = 0,
    qt.`RewardChoiceItemCount2` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardChoiceItemId2` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardChoiceItemId3`
SET qt.`RewardChoiceItemId3` = 0,
    qt.`RewardChoiceItemCount3` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardChoiceItemId3` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardChoiceItemId4`
SET qt.`RewardChoiceItemId4` = 0,
    qt.`RewardChoiceItemCount4` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardChoiceItemId4` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardChoiceItemId5`
SET qt.`RewardChoiceItemId5` = 0,
    qt.`RewardChoiceItemCount5` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardChoiceItemId5` <> 0
  AND item_ref.`entry` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `item_template` item_ref ON item_ref.`entry` = qt.`RewardChoiceItemId6`
SET qt.`RewardChoiceItemId6` = 0,
    qt.`RewardChoiceItemCount6` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardChoiceItemId6` <> 0
  AND item_ref.`entry` IS NULL;

-- Tracking-event quests can only auto-reward their first choice item.
UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RewardChoiceItemId2` = 0,
    qt.`RewardChoiceItemCount2` = 0,
    qt.`RewardChoiceItemId3` = 0,
    qt.`RewardChoiceItemCount3` = 0,
    qt.`RewardChoiceItemId4` = 0,
    qt.`RewardChoiceItemCount4` = 0,
    qt.`RewardChoiceItemId5` = 0,
    qt.`RewardChoiceItemCount5` = 0,
    qt.`RewardChoiceItemId6` = 0,
    qt.`RewardChoiceItemCount6` = 0
WHERE dq.`entry` IS NULL
  AND (qt.`Flags` & 0x400) <> 0
  AND (
      qt.`RewardChoiceItemId2` <> 0 OR qt.`RewardChoiceItemCount2` <> 0
      OR qt.`RewardChoiceItemId3` <> 0 OR qt.`RewardChoiceItemCount3` <> 0
      OR qt.`RewardChoiceItemId4` <> 0 OR qt.`RewardChoiceItemCount4` <> 0
      OR qt.`RewardChoiceItemId5` <> 0 OR qt.`RewardChoiceItemCount5` <> 0
      OR qt.`RewardChoiceItemId6` <> 0 OR qt.`RewardChoiceItemCount6` <> 0
  );

-- Clear quest chain and reward metadata that the loader cannot resolve.
UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `quest_template` next_q ON next_q.`Id` = qt.`NextQuestIdChain`
SET qt.`NextQuestIdChain` = 0
WHERE dq.`entry` IS NULL
  AND qt.`NextQuestIdChain` <> 0
  AND next_q.`Id` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `quest_template` next_q ON next_q.`Id` = ABS(qt.`NextQuestId`)
SET qt.`NextQuestId` = 0
WHERE dq.`entry` IS NULL
  AND qt.`NextQuestId` <> 0
  AND next_q.`Id` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
LEFT JOIN `quest_template` prev_q ON prev_q.`Id` = ABS(qt.`PrevQuestId`)
SET qt.`PrevQuestId` = 0
WHERE dq.`entry` IS NULL
  AND qt.`PrevQuestId` <> 0
  AND prev_q.`Id` IS NULL;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RewardCurrencyCount1` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardCurrencyId1` = 0
  AND qt.`RewardCurrencyCount1` <> 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RewardCurrencyCount2` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardCurrencyId2` = 0
  AND qt.`RewardCurrencyCount2` <> 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RewardCurrencyCount3` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardCurrencyId3` = 0
  AND qt.`RewardCurrencyCount3` <> 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RewardCurrencyCount4` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardCurrencyId4` = 0
  AND qt.`RewardCurrencyCount4` <> 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RewardSkillId` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardSkillId` <> 0
  AND qt.`RewardSkillPoints` = 0;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`RewardTitleId` = 0
WHERE dq.`entry` IS NULL
  AND qt.`RewardTitleId` = 70;

UPDATE `quest_template` qt
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qt.`Id`
SET qt.`ZoneOrSort` = 0
WHERE dq.`entry` IS NULL
  AND qt.`ZoneOrSort` = 3535;

-- Remove objective visual effects that point at objectives the loader does not register.
DELETE qoe
FROM `quest_objective_effects` qoe
JOIN `quest_objective` qo ON qo.`id` = qoe.`objectiveId`
LEFT JOIN `quest_template` qt ON qt.`Id` = qo.`questId`
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qo.`questId`
LEFT JOIN `creature_template` ct ON qo.`type` IN (0, 3, 11) AND ct.`entry` = qo.`objectId`
LEFT JOIN `item_template` item_ref ON qo.`type` = 1 AND item_ref.`entry` = qo.`objectId`
LEFT JOIN `gameobject_template` gt ON qo.`type` = 2 AND gt.`entry` = qo.`objectId`
WHERE dq.`entry` IS NOT NULL
   OR qt.`Id` IS NULL
   OR qo.`type` >= 14
   OR (qo.`type` IN (0, 3, 11) AND (ct.`entry` IS NULL OR qo.`amount` <= 0))
   OR (qo.`type` = 1 AND (item_ref.`entry` IS NULL OR qo.`amount` <= 0))
   OR (qo.`type` = 2 AND (gt.`entry` IS NULL OR qo.`amount` <= 0))
   OR (qo.`type` = 5 AND qo.`amount` <= 0);

DELETE qoe
FROM `quest_objective_effects` qoe
LEFT JOIN `quest_objective` qo ON qo.`id` = qoe.`objectiveId`
WHERE qo.`id` IS NULL;

-- Remove objectives that the loader skips because their owning quest or target data is missing.
DELETE qo
FROM `quest_objective` qo
LEFT JOIN `quest_template` qt ON qt.`Id` = qo.`questId`
LEFT JOIN `disables` dq ON dq.`sourceType` = 1 AND dq.`entry` = qo.`questId`
LEFT JOIN `creature_template` ct ON qo.`type` IN (0, 3, 11) AND ct.`entry` = qo.`objectId`
LEFT JOIN `item_template` item_ref ON qo.`type` = 1 AND item_ref.`entry` = qo.`objectId`
LEFT JOIN `gameobject_template` gt ON qo.`type` = 2 AND gt.`entry` = qo.`objectId`
WHERE dq.`entry` IS NULL
  AND (
      qt.`Id` IS NULL
      OR qo.`type` >= 14
      OR (qo.`type` IN (0, 3, 11) AND (ct.`entry` IS NULL OR qo.`amount` <= 0))
      OR (qo.`type` = 1 AND (item_ref.`entry` IS NULL OR qo.`amount` <= 0))
      OR (qo.`type` = 2 AND (gt.`entry` IS NULL OR qo.`amount` <= 0))
      OR (qo.`type` = 5 AND qo.`amount` <= 0)
  );

-- Remove graveyard links the loader reports as duplicate child/root mappings.
DELETE FROM `game_graveyard_zone`
WHERE (`id` = 34 AND `ghost_zone` = 6452)
   OR (`id` = 93 AND `ghost_zone` = 6450)
   OR (`id` = 94 AND `ghost_zone` = 6454)
   OR (`id` = 100 AND `ghost_zone` = 6176)
   OR (`id` = 105 AND `ghost_zone` = 6170)
   OR (`id` = 709 AND `ghost_zone` = 6451)
   OR (`id` = 912 AND `ghost_zone` = 6455)
   OR (`id` = 923 AND `ghost_zone` = 6456)
   OR (`id` = 1471 AND `ghost_zone` = 6457)
   OR (`id` = 1692 AND `ghost_zone` = 4755)
   OR (`id` = 1693 AND `ghost_zone` = 4755)
   OR (`id` = 1694 AND `ghost_zone` = 4755)
   OR (`id` = 1695 AND `ghost_zone` = 4755)
   OR (`id` = 1700 AND `ghost_zone` = 6453)
   OR (`id` = 4276 AND `ghost_zone` = 6141)
   OR (`id` = 4276 AND `ghost_zone` = 6553)
   OR (`id` = 4277 AND `ghost_zone` = 6141)
   OR (`id` = 4277 AND `ghost_zone` = 6553)
   OR (`id` = 4278 AND `ghost_zone` = 6074)
   OR (`id` = 4476 AND `ghost_zone` = 6576)
   OR (`id` = 4476 AND `ghost_zone` = 6578)
   OR (`id` = 4476 AND `ghost_zone` = 6579)
   OR (`id` = 4476 AND `ghost_zone` = 6724)
   OR (`id` = 4476 AND `ghost_zone` = 6725)
   OR (`id` = 4509 AND `ghost_zone` = 6576)
   OR (`id` = 4509 AND `ghost_zone` = 6578)
   OR (`id` = 4509 AND `ghost_zone` = 6579)
   OR (`id` = 4509 AND `ghost_zone` = 6724)
   OR (`id` = 4509 AND `ghost_zone` = 6725)
   OR (`id` = 4623 AND `ghost_zone` = 6823);
