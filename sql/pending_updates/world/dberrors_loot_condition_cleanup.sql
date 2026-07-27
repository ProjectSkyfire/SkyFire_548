-- Clean loader-rejected loot, condition, and SmartAI rows reported by DBErrors.

DELETE l
FROM `creature_loot_template` l
LEFT JOIN `item_template` i ON i.`entry` = l.`item`
WHERE l.`mincountOrRef` > 0
  AND i.`entry` IS NULL;

DELETE l
FROM `gameobject_loot_template` l
LEFT JOIN `item_template` i ON i.`entry` = l.`item`
WHERE l.`mincountOrRef` > 0
  AND i.`entry` IS NULL;

DELETE l
FROM `creature_loot_template` l
LEFT JOIN `creature_template` owner ON owner.`lootid` = l.`entry`
LEFT JOIN `creature_template` same_entry ON same_entry.`entry` = l.`entry`
WHERE l.`entry` IN (68026, 68027)
  AND owner.`entry` IS NULL
  AND same_entry.`entry` IS NULL;

DELETE l
FROM `gameobject_loot_template` l
LEFT JOIN `gameobject_template` owner ON owner.`type` IN (3, 25) AND owner.`data1` = l.`entry`
WHERE l.`entry` IN (938, 1689, 10101, 10102, 15000, 17378, 22906, 24124, 27826, 28682, 210016)
  AND owner.`entry` IS NULL;

DELETE FROM `reference_loot_template`
WHERE `entry` IN (24122, 24123, 44012);

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 1
  AND `SourceGroup` = 69;

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 13
  AND
  (
    (`SourceEntry` IN (80208, 93072) AND `SourceGroup` = 0)
    OR (`SourceEntry` IN (33831, 74758) AND `SourceGroup` = 1)
    OR (`SourceEntry` IN (44885, 46350) AND `SourceGroup` = 2)
    OR (`SourceEntry` IN (45201, 45203, 51395) AND `SourceGroup` = 3)
  );

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 18
  AND `SourceGroup` = 57301
  AND `SourceEntry` = 111543
  AND `ConditionTypeOrReference` = 2
  AND `ConditionValue1` = 18;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 43950
  AND `source_type` = 0
  AND `id` = 2
  AND `action_type` = 17
  AND `action_param1` = 490;
