-- Restore Kirthaven residents, services, and quest-stage visibility.
-- Reserved spawn GUIDs: 1108100..1108133. Existing rare and event pools are unchanged.
-- The Beast Unleashed and Uninvited Tentacle belong to the scripted ceremony; do not place them in the normal village.

INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseId`,`phaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(1108100,49577,0,1,0,0,36976,0,-2676.5,-5365.060059,181.578003,3.12414,300,0.0,0,85239,0,0,0,0,0),
(1108101,49578,0,1,0,0,36975,0,-2779.399902,-5306.580078,173.906006,3.92699,300,0.0,0,85239,0,0,0,0,0),
(1108102,49574,0,1,0,0,36546,0,-2788.929932,-5316.009766,173.936005,5.42797,300,0.0,0,85239,0,0,0,0,0),
(1108103,49579,0,1,0,0,36974,0,-2806.52002,-5341.540039,176.970993,5.48033,300,0.0,0,85239,0,0,0,0,0),
(1108104,49581,0,1,0,0,36973,0,-2758.919922,-5356.459961,175.867004,0.314159,300,0.0,0,85239,0,0,0,0,0),
(1108105,48364,0,1,243,0,35269,0,-2744.860107,-5304.450195,174.171997,1.41372,300,0.0,0,619920,0,0,0,0,0),
(1108106,48367,0,1,243,0,36125,0,-2744.629883,-5301.100098,174.164993,4.04916,300,0.0,0,85239,8908,0,0,0,0),
(1108107,48366,0,1,243,0,36124,0,-2772.719971,-5315.029785,173.998993,4.5204,300,0.0,0,85239,0,0,0,0,0),
(1108108,49575,0,1,0,0,36977,0,-2804.080078,-5347.109863,177.186005,0.750492,300,0.0,0,85239,0,0,0,0,0),
(1108109,48915,0,1,0,0,30412,0,-2698.439941,-5431.25,190.246002,5.87505,300,3.0,0,42,0,1,0,0,0),
(1108110,48915,0,1,0,0,36336,0,-2695.580078,-5363.919922,181.350006,4.57111,300,3.0,0,42,0,1,0,0,0),
(1108111,48915,0,1,0,0,30412,0,-2684.550049,-5419.25,190.246002,3.37736,300,3.0,0,42,0,1,0,0,0),
(1108112,48915,0,1,0,0,30412,0,-2701.280029,-5428.459961,190.225006,5.18446,300,3.0,0,42,0,1,0,0,0),
(1108113,48915,0,1,0,0,36332,0,-2650.959961,-5381.040039,171.132996,1.12596,300,3.0,0,42,0,1,0,0,0),
(1108114,48915,0,1,0,0,36332,0,-2710.699951,-5429.790039,191.526001,4.4818,300,3.0,0,42,0,1,0,0,0),
(1108115,48915,0,1,0,0,36336,0,-2670.889893,-5401.990234,171.098007,3.31024,300,3.0,0,42,0,1,0,0,0),
(1108116,48915,0,1,0,0,30412,0,-2648.629883,-5385.02002,171.158005,2.72023,300,3.0,0,42,0,1,0,0,0),
(1108117,48915,0,1,0,0,36332,0,-2683.76001,-5421.890137,190.246002,3.91713,300,3.0,0,42,0,1,0,0,0),
(1108118,48915,0,1,0,0,30412,0,-2706.47998,-5427.600098,190.225006,3.14159,300,3.0,0,42,0,1,0,0,0),
(1108119,48915,0,1,0,0,36336,0,-2670.530029,-5381.830078,181.947006,4.60613,300,3.0,0,42,0,1,0,0,0),
(1108120,48915,0,1,0,0,30412,0,-2649.419922,-5381.569824,171.113007,6.2632,300,3.0,0,42,0,1,0,0,0),
(1108121,48915,0,1,0,0,36336,0,-2673.659912,-5382.459961,181.931,4.08711,300,3.0,0,42,0,1,0,0,0),
(1108122,48915,0,1,0,0,30412,0,-2644.699951,-5387.040039,171.104996,0.479895,300,3.0,0,42,0,1,0,0,0),
(1108123,47155,0,1,0,0,35433,0,-2687.199951,-5430.140137,190.289993,2.21657,300,0.0,0,85239,0,0,0,0,0),
(1108124,48368,0,1,243,0,36126,0,-2744.939941,-5297.779785,174.184998,4.41568,300,0.0,0,85239,8908,0,0,0,0),
(1108125,48365,0,1,243,0,34288,0,-2760.399902,-5329.509766,173.839005,2.46845,300,0.0,0,426195,0,0,0,0,0),
(1108126,48530,0,1,243,0,35949,0,-2787.23999,-5322.870117,173.725998,5.95157,300,0.0,0,70946,0,0,0,0,0),
(1108127,49335,0,1,245,0,0,0,-2778.689941,-5354.330078,174.279007,4.06829,120,0.0,0,85239,0,0,0,0,0),
(1108128,49314,0,1,245,0,0,0,-2776.389893,-5333.799805,174.332001,3.28048,120,0.0,0,85239,0,0,0,0,0),
(1108129,49360,0,1,244,0,0,0,-2808.860107,-5280.609863,162.757996,6.04739,120,0.0,0,85239,0,0,0,0,0),
(1108130,49358,0,1,244,0,0,0,-2776.01001,-5328.049805,173.785995,4.66672,600,0.0,0,426195,0,0,0,0,0),
(1108131,49374,0,1,244,0,0,0,-2767.52002,-5346.740234,173.720001,3.46355,300,0.0,0,85239,8908,0,0,0,0),
(1108132,46153,0,1,0,0,0,0,-2566.139893,-5401.549805,166.809998,4.27868,300,3.0,0,70946,0,1,0,0,0),
(1108133,46153,0,1,0,0,0,0,-2617.310059,-5410.060059,177.248001,2.94743,300,3.0,0,70946,0,1,0,0,0)
ON DUPLICATE KEY UPDATE `id`=VALUES(`id`), `map`=VALUES(`map`), `spawnMask`=VALUES(`spawnMask`), `phaseId`=VALUES(`phaseId`), `phaseGroup`=VALUES(`phaseGroup`), `modelid`=VALUES(`modelid`), `equipment_id`=VALUES(`equipment_id`), `position_x`=VALUES(`position_x`), `position_y`=VALUES(`position_y`), `position_z`=VALUES(`position_z`), `orientation`=VALUES(`orientation`), `spawntimesecs`=VALUES(`spawntimesecs`), `spawndist`=VALUES(`spawndist`), `currentwaypoint`=VALUES(`currentwaypoint`), `curhealth`=VALUES(`curhealth`), `curmana`=VALUES(`curmana`), `MovementType`=VALUES(`MovementType`), `npcflag`=VALUES(`npcflag`), `unit_flags`=VALUES(`unit_flags`), `dynamicflags`=VALUES(`dynamicflags`);

INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(1108100,0,0,0,1,0,NULL),
(1108101,0,0,0,1,0,NULL),
(1108102,0,0,0,1,0,NULL),
(1108103,0,0,0,1,0,NULL),
(1108104,0,0,0,1,0,NULL),
(1108105,0,0,0,1,0,NULL),
(1108106,0,0,0,1,0,NULL),
(1108107,0,0,0,1,0,'90353'),
(1108108,0,0,0,1,0,NULL),
(1108109,0,0,0,1,0,NULL),
(1108110,0,0,0,1,0,NULL),
(1108111,0,0,0,1,0,NULL),
(1108112,0,0,0,1,0,NULL),
(1108113,0,0,0,1,0,NULL),
(1108114,0,0,0,1,0,NULL),
(1108115,0,0,0,1,0,NULL),
(1108116,0,0,0,1,0,NULL),
(1108117,0,0,0,1,0,NULL),
(1108118,0,0,0,1,0,NULL),
(1108119,0,0,0,1,0,NULL),
(1108120,0,0,0,1,0,NULL),
(1108121,0,0,0,1,0,NULL),
(1108122,0,0,0,1,0,NULL),
(1108123,0,0,0,1,0,NULL),
(1108124,0,0,65536,1,0,NULL),
(1108125,0,0,65536,1,0,NULL),
(1108126,0,0,65536,1,0,NULL),
(1108127,0,0,0,1,0,NULL),
(1108128,0,0,0,1,0,NULL),
(1108129,0,0,0,1,0,NULL),
(1108130,0,0,0,1,0,NULL),
(1108131,0,0,0,1,0,NULL),
(1108132,0,0,0,1,0,NULL),
(1108133,0,0,0,1,0,NULL)
ON DUPLICATE KEY UPDATE `path_id`=VALUES(`path_id`), `mount`=VALUES(`mount`), `bytes1`=VALUES(`bytes1`), `bytes2`=VALUES(`bytes2`), `emote`=VALUES(`emote`), `auras`=VALUES(`auras`);

INSERT INTO `npc_vendor` (`entry`,`slot`,`item`,`maxcount`,`incrtime`,`ExtendedCost`,`type`) VALUES
(49575,0,2880,0,0,0,1),
(49575,0,2901,0,0,0,1),
(49575,0,3466,0,0,0,1),
(49575,0,3857,0,0,0,1),
(49575,0,5956,0,0,0,1),
(49575,0,18567,0,0,0,1),
(49577,0,37460,0,0,0,1),
(49578,0,63388,0,0,0,1),
(49578,0,64670,0,0,0,1),
(49578,0,79249,0,0,0,1),
(49579,0,3371,0,0,0,1),
(49579,0,6532,0,0,0,1),
(49579,0,14341,0,0,0,1),
(49579,0,38426,0,0,0,1),
(49579,0,39354,0,0,0,1),
(49581,0,58256,0,0,0,1),
(49581,0,58257,0,0,0,1),
(49581,0,58260,0,0,0,1),
(49581,0,58261,0,0,0,1),
(49581,0,58274,0,0,0,1),
(49581,0,58275,0,0,0,1)
ON DUPLICATE KEY UPDATE `slot`=VALUES(`slot`), `item`=VALUES(`item`), `maxcount`=VALUES(`maxcount`), `incrtime`=VALUES(`incrtime`), `ExtendedCost`=VALUES(`ExtendedCost`), `type`=VALUES(`type`);

-- Restore the actual stable service in addition to Baird's vendor inventory.
UPDATE `creature_template` SET `npcflag`=(`npcflag` | 4194304) & ~1 WHERE `entry`=49577;

INSERT INTO `phase_area` (`AreaId`,`PhaseId`,`Comment`) VALUES
(5143,169,'Kirthaven - retain ordinary residents and services'),
(5143,243,'Kirthaven - before wedding completion'),
(5143,244,'Kirthaven - after wedding completion'),
(5143,245,'Kirthaven - wedding guests')
ON DUPLICATE KEY UPDATE `PhaseId`=VALUES(`PhaseId`), `Comment`=VALUES(`Comment`);

INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(26,243,5143,0,0,28,0,28655,0,0,1,0,0,'','Kirthaven - wedding not complete'),
(26,243,5143,0,0,8,0,28655,0,0,1,0,0,'','Kirthaven - wedding not rewarded'),
(26,244,5143,0,0,28,0,28655,0,0,0,0,0,'','Kirthaven - completed wedding awaiting turn-in'),
(26,244,5143,0,1,8,0,28655,0,0,0,0,0,'','Kirthaven - wedding rewarded'),
(26,245,5143,0,0,9,0,28655,0,0,0,0,0,'','Kirthaven - guests during active wedding'),
(19,0,28655,0,0,8,0,28408,0,0,0,0,0,'','Wildhammer Wedding - preparation quest rewarded'),
(19,0,28655,0,0,8,0,28409,0,0,0,0,0,'','Wildhammer Wedding - preparation quest rewarded'),
(19,0,28655,0,0,8,0,28410,0,0,0,0,0,'','Wildhammer Wedding - preparation quest rewarded'),
(19,0,28655,0,0,8,0,28411,0,0,0,0,0,'','Wildhammer Wedding - preparation quest rewarded'),
(19,0,28655,0,0,8,0,28413,0,0,0,0,0,'','Wildhammer Wedding - preparation quest rewarded')
ON DUPLICATE KEY UPDATE `SourceGroup`=VALUES(`SourceGroup`), `SourceEntry`=VALUES(`SourceEntry`), `SourceId`=VALUES(`SourceId`), `ElseGroup`=VALUES(`ElseGroup`), `ConditionTypeOrReference`=VALUES(`ConditionTypeOrReference`), `ConditionTarget`=VALUES(`ConditionTarget`), `ConditionValue1`=VALUES(`ConditionValue1`), `ConditionValue2`=VALUES(`ConditionValue2`), `ConditionValue3`=VALUES(`ConditionValue3`), `NegativeCondition`=VALUES(`NegativeCondition`), `ErrorType`=VALUES(`ErrorType`), `ErrorTextId`=VALUES(`ErrorTextId`), `ScriptName`=VALUES(`ScriptName`), `Comment`=VALUES(`Comment`);

-- Follow the local quest stages; the post-wedding introduction must not require a Horde quest.
UPDATE `quest_template` SET `PrevQuestId`=28294 WHERE `Id`=28346;
UPDATE `quest_template` SET `PrevQuestId`=28346 WHERE `Id`=28377;
UPDATE `quest_template` SET `PrevQuestId`=28407 WHERE `Id` IN (28408,28409,28410,28411,28413);
UPDATE `quest_template` SET `PrevQuestId`=28655 WHERE `Id`=27374;
UPDATE `quest_template` SET `RequiredRaces`=18875469 WHERE `Id` IN (27374,28294,28346,28377,28407,28408,28409,28410,28411,28413,28655);
