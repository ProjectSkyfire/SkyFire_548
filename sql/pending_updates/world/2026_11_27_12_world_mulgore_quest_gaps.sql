-- Restore Mulgore quest travel and ambient combat.

-- Summon Eagle Spirit (69340) needs spell focus 1640 next to Dyami Windsoar. The focus object (301035) had no template and no spawn, so the Water of Vision could not be used.

DELETE FROM `gameobject_template` WHERE `entry` IN (301035);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `data0`, `data1`) VALUES
(301035, 8, 299, 'you to be next to Dyami Windsoar', 1, 1640, 20);

DELETE FROM `gameobject` WHERE `id` = 301035 AND `guid` BETWEEN 910984 AND 910985;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910984, 301035, 1, 1, 0, 0, -2945.58, 11.4951, 190.589, 1.56396, 0, 0, 0.704686, 0.709519, 300, 255, 1),
(910985, 301035, 1, 1, 0, 0, -2933.04, -42.8545, 189.676, 1.76031, 0, 0, 0.770838, 0.637031, 300, 255, 1);

-- These quest spells need a spell focus object near their target: Place Toxic Fogger (6620, focus 15); Place Lion Carcass (6717, focus 20).
-- The objects had templates and 2 of their spawns were missing, so the casts failed with a requirement error.

DELETE FROM `gameobject` WHERE `id` = 19876 AND `guid` = 911569;
DELETE FROM `gameobject` WHERE `id` = 19594 AND `guid` = 1061264;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911569, 19876, 0, 1, 0, 0, 263.339, -268, 143.757, 1.449, 0, 0, 0.662761, 0.748831, 600, 100, 1),
(1061264, 19594, 1, 1, 0, 0, 1435.41, 0.49, 38.65, 0, 0, 0, 0, 1, 180, 0, 1);

-- Enable SmartAI and select the nearest living opponent, preserving other script rows.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (36942,36943) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 36942 AND `id` = 3;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 36943 AND `id` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36942, 0, 3, 0, 1, 0, 100, 0, 1000, 1000, 5000, 5000, 0, 49, 0, 0, 0, 0, 0, 0, 19, 36943, 10, 0, 0, 0, 0, 0, 'Fledgling Brave - Out of combat - Attack the nearest Bristleback Invader within 10 yards'),
(36943, 0, 2, 0, 1, 0, 100, 0, 1000, 1000, 5000, 5000, 0, 49, 0, 0, 0, 0, 0, 0, 19, 36942, 10, 0, 0, 0, 0, 0, 'Bristleback Invader - Out of combat - Attack the nearest Fledgling Brave within 10 yards');

-- Restore the Eagle Spirit and Wolf Spirit vehicle rides.

UPDATE `creature_template` SET `AIName` = 'SmartAI',`VehicleId` = 550,`InhabitType` = 4,`speed_walk` = 2.5,`speed_run` = 3.4 WHERE `entry` = 36790 AND `ScriptName` = '';

UPDATE `creature_template` SET `AIName` = 'SmartAI',`VehicleId` = 552,`InhabitType` = 3,`speed_walk` = 1.0,`speed_run` = 2.5 WHERE `entry` = 36834 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE (`source_type` = 0 AND `entryorguid` IN (36790,36834)) OR (`source_type` = 9 AND `entryorguid` IN (3679000,3683400));
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(36790,0,0,0,54,0,100,0,0,0,0,0,0,80,3679000,2,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Eagle Spirit - On summoned - Start boarding delay'),
(36790,0,1,2,58,0,100,0,8,36790,0,0,0,28,46598,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Eagle Spirit - On Waypoint Ended - Remove Aura'),
(36790,0,2,0,61,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Eagle Spirit - Linked To Id 1 - Despawn'),
(36834,0,0,0,54,0,100,0,0,0,0,0,0,80,3683400,2,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Wolf Spirit - On Summoned - Run Script'),
(36834,0,1,0,40,0,100,0,3,36834,0,0,0,97,25,25,0,0,0,0,1,0,0,0,-1974.43994140625,-460.3659973144531,-8.141300201416016,0.0,'Wolf Spirit - On Waypoint Reached - Jump To Pos'),
(36834,0,2,3,58,0,100,0,38,36834,0,0,0,28,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Wolf Spirit - On Waypoint Ended - Remove All Auras'),
(36834,0,3,0,61,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Wolf Spirit - Linked To Id 2 - Despawn'),
(3683400,9,0,0,0,0,100,0,2000,2000,0,0,0,53,1,36834,0,20441,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Wolf Spirit - On Script - Waypoint Start'),
(3679000,9,0,0,0,0,100,0,2000,2000,0,0,0,53,1,36790,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Eagle Spirit - After boarding delay - Start flight path');

DELETE FROM `waypoints` WHERE `entry` IN (36790,36834);
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(36790,1,-2927.77001953125,-42.63420104980469,190.26400756835938,'Eagle Spirit'),
(36790,2,-2884.159912109375,-71.08679962158203,242.0679931640625,'Eagle Spirit'),
(36790,3,-2720.590087890625,-111.00399780273438,242.5959930419922,'Eagle Spirit'),
(36790,4,-2683.949951171875,-382.9010009765625,231.1790008544922,'Eagle Spirit'),
(36790,5,-2619.14990234375,-484.9289855957031,231.1790008544922,'Eagle Spirit'),
(36790,6,-2543.8701171875,-525.3330078125,231.1790008544922,'Eagle Spirit'),
(36790,7,-2465.320068359375,-502.489990234375,190.73500061035156,'Eagle Spirit'),
(36790,8,-2343.8701171875,-401.8280029296875,-8.320870399475098,'Eagle Spirit'),
(36834,1,-2165.820068359375,-453.3909912109375,-6.30817985534668,'Wolf Spirit'),
(36834,2,-2123.199951171875,-463.3299865722656,-9.005459785461426,'Wolf Spirit'),
(36834,3,-2095.320068359375,-463.75201416015625,-9.06676959991455,'Wolf Spirit'),
(36834,4,-1974.43994140625,-460.3659973144531,-8.141300201416016,'Wolf Spirit'),
(36834,5,-1926.5699462890625,-467.4389953613281,-6.969580173492432,'Wolf Spirit'),
(36834,6,-1883.68994140625,-469.6549987792969,-7.576990127563477,'Wolf Spirit'),
(36834,7,-1834.27001953125,-474.5849914550781,-3.4593000411987305,'Wolf Spirit'),
(36834,8,-1799.199951171875,-482.1130065917969,-1.4640699625015259,'Wolf Spirit'),
(36834,9,-1763.010009765625,-495.90301513671875,-9.014280319213867,'Wolf Spirit'),
(36834,10,-1724.0699462890625,-513.0120239257812,-12.648099899291992,'Wolf Spirit'),
(36834,11,-1684.800048828125,-527.9910278320312,-18.092599868774414,'Wolf Spirit'),
(36834,12,-1641.8299560546875,-540.2639770507812,-30.85540008544922,'Wolf Spirit'),
(36834,13,-1601.8299560546875,-547.1740112304688,-45.797298431396484,'Wolf Spirit'),
(36834,14,-1570.5699462890625,-553.2689819335938,-49.92900085449219,'Wolf Spirit'),
(36834,15,-1527.800048828125,-559.5120239257812,-56.08190155029297,'Wolf Spirit'),
(36834,16,-1493.8399658203125,-563.0260009765625,-56.167198181152344,'Wolf Spirit'),
(36834,17,-1446.9300537109375,-578.8179931640625,-58.418399810791016,'Wolf Spirit'),
(36834,18,-1412.989990234375,-595.302001953125,-55.19820022583008,'Wolf Spirit'),
(36834,19,-1365.9000244140625,-609.02099609375,-60.01430130004883,'Wolf Spirit'),
(36834,20,-1324.530029296875,-620.5490112304688,-59.165401458740234,'Wolf Spirit'),
(36834,21,-1288.8800048828125,-630.6160278320312,-59.4739990234375,'Wolf Spirit'),
(36834,22,-1256.989990234375,-636.8250122070312,-59.176300048828125,'Wolf Spirit'),
(36834,23,-1217.260009765625,-638.3939819335938,-57.4296989440918,'Wolf Spirit'),
(36834,24,-1171.9300537109375,-638.447998046875,-58.04669952392578,'Wolf Spirit'),
(36834,25,-1134.2900390625,-639.5830078125,-55.997398376464844,'Wolf Spirit'),
(36834,26,-1092.4599609375,-642.072998046875,-53.23360061645508,'Wolf Spirit'),
(36834,27,-1063.8800048828125,-650.4299926757812,-53.23830032348633,'Wolf Spirit'),
(36834,28,-1034.050048828125,-642.3909912109375,-55.690399169921875,'Wolf Spirit'),
(36834,29,-1016.25,-624.2329711914062,-58.9900016784668,'Wolf Spirit'),
(36834,30,-995.9979858398438,-603.2969970703125,-58.60850143432617,'Wolf Spirit'),
(36834,31,-970.7780151367188,-575.8350219726562,-56.6421012878418,'Wolf Spirit'),
(36834,32,-945.0020141601562,-548.4180297851562,-53.867801666259766,'Wolf Spirit'),
(36834,33,-924.6909790039062,-526.7329711914062,-47.61640167236328,'Wolf Spirit'),
(36834,34,-899.3109741210938,-501.1059875488281,-36.09669876098633,'Wolf Spirit'),
(36834,35,-877.1370239257812,-490.0429992675781,-33.425899505615234,'Wolf Spirit'),
(36834,36,-857.1279907226562,-490.04901123046875,-31.26650047302246,'Wolf Spirit'),
(36834,37,-833.06201171875,-487.0679931640625,-29.1205997467041,'Wolf Spirit'),
(36834,38,-804.260009765625,-486.7030029296875,-32.15060043334961,'Wolf Spirit');

-- Tal offers the free Orgrimmar flight only while Walk With The Earth Mother is active.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1 WHERE `entry` = 2995;
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 4326 AND `OptionID` = 1;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 4326 AND `SourceEntry` = 1;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(15,4326,1,0,0,9,0,26397,0,0,0,0,0,'','Tal - Orgrimmar flight requires incomplete quest 26397'),
(15,4326,1,0,1,28,0,26397,0,0,0,0,0,'','Tal - Orgrimmar flight also accepts completed but unrewarded quest 26397');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2995 AND `source_type` = 0 AND `id` IN (3,4);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_chance`,`event_param1`,`event_param2`,`action_type`,`action_param1`,`action_param2`,`target_type`,`comment`) VALUES
(2995,0,3,4,62,100,4326,1,52,387,1,7,'Tal - Select Orgrimmar option - Start free taxi path'),
(2995,0,4,0,61,100,0,0,72,0,0,7,'Tal - Linked - Close gossip');

-- War Dance: restore Orno Grimtotem, his ambushers and Baine's scene.
DELETE FROM `creature` WHERE `guid` = 8332000;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8332000, 36931, 1, 1, 0, 0, 0, 0, -600.932, 189.823, 53.1532, 3.22886, 60, 0, 0, 0, 0, 0, 0, 0, 0); -- Orno Grimtotem

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (36931, 37058, 37062);

DELETE FROM `waypoints` WHERE `entry` = 37058;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(37058, 1, -603.096, 193.539, 53.082, 'Baine Bloodhoof');

DELETE FROM `creature_text` WHERE `entry` IN (36931, 37058);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(36931, 0, 0, 'I''ve had enough of this!', 12, 0, 100, 0, 0, 0, 'Orno Grimtotem'),
(36931, 1, 0, 'Baine! You''ve shown yourself! GET HIM!', 12, 0, 100, 0, 0, 0, 'Orno Grimtotem'),
(36931, 2, 0, 'You will die in shame like your pathetic father.', 12, 0, 100, 0, 0, 0, 'Orno Grimtotem'),
(36931, 3, 0, 'This isn''t over, Baine...', 12, 0, 100, 0, 0, 0, 'Orno Grimtotem'),
(37058, 0, 0, 'So have I, Grimtotem.', 12, 0, 100, 0, 0, 21166, 'Baine Bloodhoof'),
(37058, 1, 0, 'It took a coward''s poison and the fury of Hellscream to bring my father down.', 12, 0, 100, 0, 0, 21161, 'Baine Bloodhoof'),
(37058, 2, 0, 'WHAT CHANCE HAVE YOU?', 12, 0, 100, 0, 0, 21162, 'Baine Bloodhoof'),
(37058, 3, 0, 'It is for you.', 12, 0, 100, 0, 0, 21163, 'Baine Bloodhoof'),
(37058, 4, 0, 'We''re done here. Mulgore will no longer suffer the hatred of the Grimtotem.', 12, 0, 100, 0, 0, 0, 'Baine Bloodhoof'),
(37058, 5, 0, 'I will meet you again in Thunder Bluff.', 12, 0, 100, 0, 0, 0, 'Baine Bloodhoof');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (36931, 37058, 37062);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (3693100, 3705800);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36931, 0, 0, 1, 25, 0, 100, 0, 0, 0, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Reset - Remove UnitFlag 768'),
(36931, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 0 - Set Emote State 0'),
(36931, 0, 2, 3, 4, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Aggro - Store Target List'),
(36931, 0, 3, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 2 - Set Invincibility Hp 1'),
(36931, 0, 4, 5, 2, 0, 100, 1, 0, 20, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Between 0-20% Health - Combat Stop'),
(36931, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 102, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 4 - Set Health Regen 0'),
(36931, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36931, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 5 - Quest Credit'),
(36931, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 6 - Set UnitFlag 768'),
(36931, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 7 - Say Line 0'),
(36931, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -600.9320068359375, 189.822998046875, 53.1531982421875, 0, 'Orno Grimtotem - Move To Pos'),
(36931, 0, 10, 11, 52, 0, 100, 0, 0, 36931, 0, 0, 0, 12, 37058, 8, 0, 0, 0, 0, 8, 0, 0, 0, -626.3090209960938, 207.38400268554688, 53.10940170288086, 0, 'Orno Grimtotem - On Text Over 0 - Summon Creature Baine Bloodhoof'),
(36931, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 10 - Set Orientation'),
(36931, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 375, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 11 - Set Emote State 375'),
(36931, 0, 13, 14, 38, 0, 100, 0, 0, 1, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Data Set 0 1 - Set Orientation'),
(36931, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 13 - Say Line 1'),
(36931, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 37062, 6, 10000, 0, 0, 0, 8, 0, 0, 0, -602.0659790039062, 197.13099670410156, 53.13399887084961, 0, 'Orno Grimtotem - Linked To Id 14 - Summon Creature Grimtotem Ambushe'),
(36931, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 37062, 6, 10000, 0, 0, 0, 8, 0, 0, 0, -606.7930297851562, 196.00399780273438, 53.13399887084961, 0, 'Orno Grimtotem - Linked To Id 15 - Summon Creature Grimtotem Ambushe'),
(36931, 0, 17, 0, 52, 0, 100, 0, 1, 36931, 0, 0, 0, 1, 2, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Text Over 1 - Say Line 2'),
(36931, 0, 18, 0, 52, 0, 100, 0, 2, 36931, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Text Over 2 - Set Data 0 1'),
(36931, 0, 19, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 80, 3693100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Data Set 0 2 - Run Script'),
(36931, 0, 20, 0, 38, 0, 100, 0, 0, 3, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Data Set 0 3 - Despawn'),
(37058, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 75, 69756, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Summoned - Add Aura War Harness'),
(37058, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 37058, 0, 24540, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 0 - Waypoint Start'),
(37058, 0, 2, 3, 58, 0, 100, 0, 1, 37058, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Waypoint Ended - Say Line 0'),
(37058, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 2 - Set Home'),
(37058, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 11, 36931, 10, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 3 - Set Orientation'),
(37058, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 375, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 4 - Set Emote State 375'),
(37058, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 512, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 5 - Remove UnitFlag 512'),
(37058, 0, 7, 0, 52, 0, 100, 0, 0, 37058, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 36931, 10, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Text Over 0 - Set Data 0 1'),
(37058, 0, 8, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 1, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Data Set 0 1 - Say Line 1'),
(37058, 0, 9, 10, 52, 0, 100, 0, 1, 37058, 0, 0, 0, 1, 2, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Text Over 1 - Say Line 2'),
(37058, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 69847, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 9 - Cast War Stomp'),
(37058, 0, 11, 12, 52, 0, 100, 0, 2, 37058, 0, 0, 0, 11, 69879, 0, 0, 0, 0, 0, 11, 36931, 60, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Text Over 2 - Cast Charge'),
(37058, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 36931, 100, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 11 - Set Data 0 2'),
(37058, 0, 13, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 1, 3, 1000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Data 0 2 - Say Line 3'),
(37058, 0, 14, 15, 52, 0, 100, 0, 3, 37058, 0, 0, 0, 5, 36, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Text Over 3 - Play Emote 36'),
(37058, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 11, 36931, 10, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 14 - Kill Unit'),
(37058, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3705800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 15 - Run Script'),
(37062, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grimtotem Ambushe - On Summoned - Set React State Defensive'),
(37062, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Grimtotem Ambushe - Linked To Id 0 - Set Orientation'),
(37062, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Grimtotem Ambushe - Linked To Id 1 - Attack Start'),
(37062, 0, 3, 0, 0, 0, 100, 0, 0, 0, 2000, 2000, 0, 11, 69873, 0, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Grimtotem Ambushe - In Combat - Cast Cosmetic - Combat Attack 1H');

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3693100, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Script - Set Emote State 0'),
(3693100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Script - Say Line 3'),
(3693100, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Script - Set Data 0 2'),
(3705800, 9, 0, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Script - Say Line 4'),
(3705800, 9, 1, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Script - Say Line 5'),
(3705800, 9, 2, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 36931, 10, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Script - Set Data 0 3'),
(3705800, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Script - Despawn');


-- Restore the English completion dialogue for the Mulgore rites and Orgrimmar handoff.
UPDATE `quest_template` SET `OfferRewardText` = 'Be not afraid, $n. You have successfully passed the Rite of Vision.' WHERE `Id` = 20441 AND COALESCE(`OfferRewardText`, '') = '';
UPDATE `quest_template` SET `OfferRewardText` = 'Welcome back, $n.$B$BWhat was done was not for revenge, or pride. It was simply to protect Thunder Bluff, and all our people, from torment and fear. Today, Mulgore is safe once more.$B$BThe Earth Mother thanks you for all your brave deeds, $n, as do I. You have earned your reward, hero. May you be welcome in Thunder Bluff always.' WHERE `Id` = 24540 AND COALESCE(`OfferRewardText`, '') = '';
UPDATE `quest_template` SET `OfferRewardText` = 'The Grimtotem forced from Mulgore... if Baine speaks for this, you are indeed a fierce warrior. From this day, your strength is no longer your own. It belongs to me, and the Horde, as a weapon of war. Hellscream''s eyes are upon you, $n. Victory or death!' WHERE `Id` = 26397 AND COALESCE(`OfferRewardText`, '') = '';
