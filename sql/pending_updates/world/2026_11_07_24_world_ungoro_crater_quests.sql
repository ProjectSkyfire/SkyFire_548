-- Un'Goro Crater: quests that still could not be completed: the net target, Silithid Leg and coconut loot, the mound
-- loot, the Diemetradon taunt credit and the Steaming Fury objective. Safe to run twice.
-- UG-1: Restrict the net target and require the quest for the item grant.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1
  AND `SourceEntry` = 71349 AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 31 AND `ConditionTarget` = 0;

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1
  AND `SourceEntry` = 38202 AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 9 AND `ConditionTarget` = 0;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`,
 `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`,
 `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`,
 `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 71349, 0, 0, 31, 0, 3, 38202, 0, 0, 0, 0, '',
 'Capture Lasher Seed can hit only Bloodpetal Seed'),
(22, 1, 38202, 0, 0, 9, 0, 24686, 0, 0, 0, 0, 0, '',
 'Bloodpetal Seed gives the seed only with Carried on the Waves taken');

-- UG-2: Quest-only Silithid Leg drops.
DELETE FROM `creature_loot_template`
WHERE `entry` IN (6551, 6552, 6553, 6554, 6555) AND `item` = 50371;

INSERT INTO `creature_loot_template`
(`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(6551, 50371, -53.0028, 1, 0, 1, 1),
(6552, 50371, -53.2248, 1, 0, 1, 1),
(6553, 50371, -59.1732, 1, 0, 1, 1),
(6554, 50371, -68.7488, 1, 0, 1, 1),
(6555, 50371, -70.3946, 1, 0, 1, 1);

-- UG-3: Cracked coconut contents.
DELETE FROM `item_loot_template` WHERE `entry` = 50238 AND `item` = 50236;

INSERT INTO `item_loot_template`
(`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(50238, 50236, 100, 1, 0, 1, 1);

-- UG-4: Connect the mound to the loot supplied by the zone file.
UPDATE `gameobject_template` SET `data1` = 202165
WHERE `entry` = 202165 AND `data1` = 0;

-- UG-8: Preserve the combat howl at ID 0 and add credit and despawn.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 9163 AND `source_type` = 0 AND `id` IN (1, 2);

INSERT INTO `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
 `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`,
 `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`,
 `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`,
 `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`,
 `target_z`, `target_o`, `comment`) VALUES
(9163, 0, 1, 2, 8, 0, 100, 1, 71775, 0, 0, 0, 0,
 33, 38355, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Diemetradon - On SpellHit Scraps of Rotting Meat - Quest Credit Dinosaur Taunt Credit'),
(9163, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0,
 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
 'Diemetradon - Linked To Id 1 - Despawn');

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 2
  AND `SourceEntry` = 9163 AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 9 AND `ConditionTarget` = 0;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`,
 `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`,
 `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`,
 `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 2, 9163, 0, 0, 9, 0, 24697, 0, 0, 0, 0, 0, '',
 'Diemetradon gives the taunt credit only with How to Make Meat Fresh Again taken');

-- UG-10: Count Steaming Fury kills for the quest.
UPDATE `quest_objective` SET `objectId` = 38254
WHERE `questId` = 24706 AND `id` = 266337 AND `objectId` = 38253;
