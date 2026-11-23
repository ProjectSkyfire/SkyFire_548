-- Desolace: Early Adoption (14246). The Swoop Egg had no script, so it never summoned its attackers, was never
-- carried off by the Defiant Swoop and gave no credit. Safe to run twice.
UPDATE `creature_template`
SET `minlevel` = 30, `maxlevel` = 30, `faction_A` = 42, `faction_H` = 42, `AIName` = 'SmartAI'
WHERE `entry` = 51139;

-- The Defiant Swoop carries the egg away, so it is a vehicle.
UPDATE `creature_template` SET `VehicleId` = 488 WHERE `entry` = 35752;

-- Placing an egg is not allowed next to another egg.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 94215;
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,
 `ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,
 `ErrorTextId`,`ScriptName`,`Comment`)
VALUES
(17,0,94215,0,0,29,0,51139,10,0,1,0,0,'','Place Swoop Egg');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 51139 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (5113901,5113902,5113903) AND `source_type` = 9;
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,
 `event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,
 `action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,
 `action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,
 `target_z`,`target_o`,`comment`)
VALUES
(51139,0,0,1,11,0,100,0,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Swoop Egg - On Respawn - Set React Passive'),
(51139,0,1,2,61,0,100,0,0,0,0,0,0,2,42,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Swoop Egg - On Respawn - Set Faction 42'),
(51139,0,2,0,61,0,100,0,0,0,0,0,0,102,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Swoop Egg - Linked To Id 0 - Set Health Regen 0'),
(51139,0,3,0,54,0,100,0,0,0,0,0,0,87,5113901,5113902,5113903,0,0,0,1,0,0,0,0,0,0,0,
 'Swoop Egg - On Summoned - Run Random Script'),
(51139,0,4,5,38,0,100,0,0,1,0,0,0,2,35,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Swoop Egg - On Data Set 0 1 - Set Faction 35'),
(51139,0,5,6,61,0,100,0,0,0,0,0,0,11,67912,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Swoop Egg - Linked To Id 4 - Cast Early Adoption Kill Credit'),
(51139,0,6,7,61,0,100,0,0,0,0,0,0,11,46598,0,0,0,0,0,19,35752,3,0,0,0,0,0,
 'Swoop Egg - Linked To Id 5 - Cast Ride Vehicle Hardcoded'),
(51139,0,7,0,61,0,100,0,0,0,0,0,0,41,10000,0,0,0,0,0,1,0,0,0,0,0,0,0,
 'Swoop Egg - Linked To Id 6 - Despawn after 10 sec'),
(5113901,9,0,0,0,0,100,0,8000,8000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113901,9,1,0,0,0,100,0,15000,15000,0,0,0,12,35750,6,10000,1,0,0,1,0,0,0,-15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Snapjaw Basilisk'),
(5113901,9,2,0,0,0,100,0,15000,15000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,0,15,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113901,9,3,0,0,0,100,0,15000,15000,0,0,0,12,35750,6,10000,1,0,0,1,0,0,0,0,-15,0,0,
 'Swoop Egg - On Script - Summon Creature Snapjaw Basilisk'),
(5113901,9,4,0,0,0,100,0,15000,15000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113901,9,5,0,0,0,100,0,15000,15000,0,0,0,12,35752,8,15000,0,0,0,1,0,0,0,-15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Defiant Swoop'),
(5113902,9,0,0,0,0,100,0,8000,8000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,-15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113902,9,1,0,0,0,100,0,15000,15000,0,0,0,12,35750,6,10000,1,0,0,1,0,0,0,15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113902,9,2,0,0,0,100,0,15000,15000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,0,15,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113902,9,3,0,0,0,100,0,15000,15000,0,0,0,12,35750,6,10000,1,0,0,1,0,0,0,0,-15,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113902,9,4,0,0,0,100,0,15000,15000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113902,9,5,0,0,0,100,0,15000,15000,0,0,0,12,35752,8,15000,0,0,0,1,0,0,0,-15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Defiant Swoop'),
(5113903,9,0,0,0,0,100,0,8000,8000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,0,15,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113903,9,1,0,0,0,100,0,15000,15000,0,0,0,12,35750,6,10000,1,0,0,1,0,0,0,0,-15,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113903,9,2,0,0,0,100,0,15000,15000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113903,9,3,0,0,0,100,0,15000,15000,0,0,0,12,35750,6,10000,1,0,0,1,0,0,0,-15,0,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113903,9,4,0,0,0,100,0,15000,15000,0,0,0,12,35811,6,10000,1,0,0,1,0,0,0,0,15,0,0,
 'Swoop Egg - On Script - Summon Creature Famished Bonepaw'),
(5113903,9,5,0,0,0,100,0,15000,15000,0,0,0,12,35752,8,15000,0,0,0,1,0,0,0,0,-15,0,0,
 'Swoop Egg - On Script - Summon Creature Defiant Swoop');
