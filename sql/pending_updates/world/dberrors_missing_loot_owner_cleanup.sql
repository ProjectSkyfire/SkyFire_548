-- Clear missing loot references and questgiver marker mismatches reported by DBErrors.

UPDATE `creature_template` ct
LEFT JOIN `creature_loot_template` loot ON loot.`entry` = ct.`lootid`
SET ct.`lootid` = 0
WHERE ct.`lootid` <> 0
  AND loot.`entry` IS NULL;

UPDATE `gameobject_template` gt
LEFT JOIN `gameobject_loot_template` loot ON loot.`entry` = gt.`data1`
SET gt.`data1` = 0
WHERE gt.`type` IN (3, 25)
  AND gt.`data1` > 0
  AND loot.`entry` IS NULL;

DELETE loot
FROM `gameobject_loot_template` loot
LEFT JOIN `reference_loot_template` ref ON ref.`entry` = ABS(loot.`mincountOrRef`)
WHERE loot.`mincountOrRef` < 0
  AND ref.`entry` IS NULL;

UPDATE `creature_template` ct
JOIN `creature_queststarter` starter ON starter.`id` = ct.`entry`
SET ct.`npcflag` = ct.`npcflag` | 2
WHERE (ct.`npcflag` & 2) = 0;
