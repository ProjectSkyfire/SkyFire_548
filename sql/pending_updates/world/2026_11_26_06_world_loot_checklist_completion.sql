-- Complete remaining creature loot, skinning and quest eligibility corrections.
-- Use the spawned Thunderhead and Highland Razormaw entries rather than retired versions.
-- Keep existing ordinary loot; quest items require their associated active quest.
-- Skinning chooses one material from a 100% exclusive group.
-- Back up before applying: world tables may use MyISAM.

-- Thunderhead: clear the static living-skinnable flag; death handling sets it normally.
UPDATE `creature_template` SET `lootid` = 37208, `skinloot` = 37208, `unit_flags` = `unit_flags` & ~67108864 WHERE `entry` = 37208;
UPDATE `creature` SET `unit_flags` = `unit_flags` & ~67108864 WHERE `id` = 37208;
UPDATE `creature_template` SET `lootid` = 41400, `skinloot` = 41400 WHERE `entry` = 41400;
-- Highborne Relics and Twilight Plans were configured below 0.2% even during their quests.
UPDATE `creature_loot_template` SET `ChanceOrQuestChance` = -50 WHERE (`entry` = 32861 AND `item` = 44830) OR (`entry` = 32888 AND `item` = 44968);
-- Remove unrelated quest items imported into Darkshore creature tables.
DELETE FROM `creature_loot_template` WHERE (`entry` = 33009 AND `item` IN (772,773,780,1357,3297,3411,8050,23685,23789)) OR (`entry` = 32861 AND `item` = 780);

INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(37208,5128,25,1,2,1,1),
(37208,5133,55,1,2,1,1),
(37208,6663,0.05,1,6,1,1),
(37208,15232,0.4,1,1,1,1),
(37208,15250,0.3,1,1,1,1),
(37208,15349,0.5,1,1,1,1),
(37208,15544,0.05,1,1,1,1),
(37208,52305,-25,1,0,1,1),
(39004,805,0.03,1,5,1,1),
(39004,828,0.03,1,5,1,1),
(39004,4496,0.1,1,5,1,1),
(39004,4873,10,1,2,1,1),
(39004,5571,0.03,1,5,1,1),
(39004,62514,75,1,2,1,1),
(41400,2991,0.3,1,1,1,1),
(41400,3180,25,1,2,1,1),
(41400,3198,0.25,1,1,1,1),
(41400,3685,40,1,0,1,1),
(41400,6382,0.2,1,1,1,1),
(41400,6565,0.15,1,1,1,1),
(41400,6577,0.2,1,1,1,1),
(41400,15230,0.15,1,1,1,1),
(41400,54832,55,1,2,1,1)
ON DUPLICATE KEY UPDATE `ChanceOrQuestChance` = VALUES(`ChanceOrQuestChance`), `lootmode` = VALUES(`lootmode`), `groupid` = VALUES(`groupid`), `mincountOrRef` = VALUES(`mincountOrRef`), `maxcount` = VALUES(`maxcount`);

INSERT INTO `skinning_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(32997,783,4.134656,1,1,1,1),
(32997,2318,58.948622,1,1,1,1),
(32997,2319,36.916722,1,1,1,1),
(33009,783,3.377375,1,1,1,1),
(33009,2318,55.212929,1,1,1,1),
(33009,2934,41.409696,1,1,1,1),
(37208,2319,37,1,1,1,1),
(37208,4232,7,1,1,1,1),
(37208,4234,53,1,1,1,1),
(37208,4235,3,1,1,1,1),
(41400,783,3,1,1,1,1),
(41400,2318,38,1,1,1,1),
(41400,2319,56,1,1,1,1),
(41400,4232,3,1,1,1,1)
ON DUPLICATE KEY UPDATE `ChanceOrQuestChance` = VALUES(`ChanceOrQuestChance`), `lootmode` = VALUES(`lootmode`), `groupid` = VALUES(`groupid`), `mincountOrRef` = VALUES(`mincountOrRef`), `maxcount` = VALUES(`maxcount`);

INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(1,7234,8049,0,0,9,0,2459,0,0,0,0,0,'','Creature 7234 - item 8049 requires active quest 2459'),
(1,11917,16581,0,0,9,0,6421,0,0,0,0,0,'','Creature 11917 - item 16581 requires active quest 6421'),
(1,11918,16581,0,0,9,0,6421,0,0,0,0,0,'','Creature 11918 - item 16581 requires active quest 6421'),
(1,32860,44942,0,0,9,0,13513,0,0,0,0,0,'','Creature 32860 - item 44942 requires active quest 13513'),
(1,32861,44830,0,0,9,0,13505,0,0,0,0,0,'','Creature 32861 - item 44830 requires active quest 13505'),
(1,32863,44979,0,0,8,0,13506,0,0,1,0,0,'','Overseers Orders - Reason to Worry not rewarded'),
(1,32863,44979,0,0,14,0,13506,0,0,0,0,0,'','Overseers Orders - Reason to Worry not started'),
(1,32863,44979,0,0,27,0,10,3,0,0,0,0,'','Overseers Orders - minimum quest level 10'),
(1,32888,44968,0,0,9,0,13596,0,0,0,0,0,'','Creature 32888 - item 44968 requires active quest 13596'),
(1,32985,44868,0,0,9,0,13542,0,0,0,0,0,'','Creature 32985 - item 44868 requires active quest 13542'),
(1,32997,44886,0,0,9,0,13544,0,0,0,0,0,'','Creature 32997 - item 44886 requires active quest 13544'),
(1,33009,44913,0,0,9,0,13528,0,0,0,0,0,'','Creature 33009 - item 44913 requires active quest 13528'),
(1,34385,46692,0,0,9,0,13582,0,0,0,0,0,'','Creature 34385 - item 46692 requires active quest 13582'),
(1,37208,52305,0,0,9,0,25080,0,0,0,0,0,'','Creature 37208 - item 52305 requires active quest 25080'),
(1,39004,4888,0,0,9,0,25170,0,0,0,0,0,'','Creature 39004 - item 4888 requires active quest 25170'),
(1,39004,52080,0,0,9,0,25037,0,0,0,0,0,'','Creature 39004 - item 52080 requires active quest 25037'),
(1,43185,58936,0,0,9,0,26544,0,0,0,0,0,'','Creature 43185 - item 58936 requires active quest 26544')
ON DUPLICATE KEY UPDATE `NegativeCondition` = VALUES(`NegativeCondition`), `ErrorType` = VALUES(`ErrorType`), `ErrorTextId` = VALUES(`ErrorTextId`), `ScriptName` = VALUES(`ScriptName`), `Comment` = VALUES(`Comment`);
