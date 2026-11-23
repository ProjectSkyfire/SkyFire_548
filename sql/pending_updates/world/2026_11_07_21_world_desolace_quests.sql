-- Desolace: quests that still could not be completed: quest drops, the prisoner manifest, the fertile mound bunny, Korrah's
-- ride and charm and the Blood-filled Leech. Safe to run twice.
-- DE-2: quest drops absent from the maintainer's zone file.
DELETE FROM `creature_loot_template`
WHERE (`entry`, `item`) IN
((4663,49008),(4664,49008),(4665,49008),(4666,49008),(4667,49008),
 (4692,49032),(4696,49026),(4701,49195),(4711,49056),(4712,49058),
 (4713,49060),(4728,49030),(11562,49222),(11563,49222));

INSERT INTO `creature_loot_template`
(`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`)
VALUES
(4663,49008,-85.9633,1,0,1,1),
(4664,49008,-85.9858,1,0,1,1),
(4665,49008,-79.2634,1,0,1,1),
(4666,49008,-79.4856,1,0,1,1),
(4667,49008,-58.4492,1,0,1,1),
(4692,49032,-86.3294,1,0,1,6),
(4696,49026,-85.47,1,0,1,1),
(4701,49195,-38.7744,1,0,1,1),
(4711,49056,-100,1,0,1,1),
(4712,49058,-65.1288,1,0,3,9),
(4713,49060,-43.4721,1,0,1,1),
(4728,49030,-83.4169,1,0,1,1),
(11562,49222,-45.3182,1,0,1,1),
(11563,49222,-49.6642,1,0,1,1);

-- DE-3: Maraudine Prisoner Manifest.
DELETE FROM `creature_loot_template` WHERE `entry` = 4656 AND `item` = 38567;
INSERT INTO `creature_loot_template`
(`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`)
VALUES (4656,38567,21.8198,1,0,1,1);

-- DE-5: retain the existing credit action and link it to free ID 1.
UPDATE `smart_scripts` SET `link` = 1
WHERE `entryorguid` = 36123 AND `source_type` = 0 AND `id` = 0;
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 36123 AND `source_type` = 0 AND `id` = 1;
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,
 `event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,
 `event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,
 `action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,
 `target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,
 `target_o`,`comment`)
VALUES
(36123,0,1,0,61,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Fertile Mound Bunny - Linked - Despawn');

-- DE-6: mount settings and corrections to the maintainer's gossip actions.
UPDATE `creature_template`
SET `VehicleId` = 490, `speed_run` = 2.5, `InhabitType` = 7
WHERE `entry` = 35779;
UPDATE `smart_scripts` SET `action_param2` = 0
WHERE `entryorguid` = 35773 AND `source_type` = 0 AND `id` = 1;
UPDATE `smart_scripts` SET `action_param2` = 0
WHERE `entryorguid` = 35773 AND `source_type` = 0 AND `id` = 2;
UPDATE `smart_scripts` SET `target_type` = 7
WHERE `entryorguid` = 35773 AND `source_type` = 0 AND `id` = 3;

-- DE-6: restrict the two Korrah gossip options.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 10680
  AND `SourceEntry` = 0 AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 9 AND `ConditionTarget` = 0
  AND `ConditionValue1` = 14256 AND `ConditionValue2` = 0
  AND `ConditionValue3` = 0;
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 10680
  AND `SourceEntry` = 1 AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 8 AND `ConditionTarget` = 0
  AND `ConditionValue1` IN (14257,14282) AND `ConditionValue2` = 0
  AND `ConditionValue3` = 0;
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,
 `ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,
 `ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`)
VALUES
(15,10680,0,0,0,9,0,14256,0,0,0,0,0,'',
 'Korrah - ride option requires The Emerging Threat taken'),
(15,10680,1,0,0,8,0,14257,0,0,0,0,0,'',
 'Korrah - spare charm requires Slitherblade Slaughter rewarded'),
(15,10680,1,0,0,8,0,14282,0,0,1,0,0,'',
 'Korrah - spare charm requires Mystery Solved not rewarded');

-- DE-8: Blood-filled Leech click flag and behavior.
UPDATE `creature_template`
SET `AIName` = 'SmartAI', `npcflag` = `npcflag` | 16777216
WHERE `entry` = 36059;
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 36059 AND `source_type` = 0 AND `id` IN (0,1);
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,
 `event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,
 `event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,
 `action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,
 `target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,
 `target_o`,`comment`)
VALUES
(36059,0,0,0,54,0,100,0,0,0,0,0,0,89,10,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Blood-filled Leech - On Just Summoned - Start Random Movement'),
(36059,0,1,0,8,0,100,0,68293,0,0,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Blood-filled Leech - On Spellhit - Despawn');
