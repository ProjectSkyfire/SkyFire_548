-- Updated the Chain of Storyline Quests in Teldrassil  http://wow.gamepedia.com/Teldrassil_storyline
-- Quest = The Balance of Nature (28713)
-- http://wow.gamepedia.com/Quest:The_Balance_of_Nature
-- http://www.wowhead.com/quest=28713
-- http://www.wowdb.com/quests/28713

UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28714 WHERE `Id` = 28713;
UPDATE `quest_template` SET `PrevQuestId` = 28713, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 28714;
UPDATE `quest_template` SET `PrevQuestId` = 28713, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28715 WHERE `Id` = 28734;
UPDATE `quest_template` SET `PrevQuestId` = 28734, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 28715;
UPDATE `quest_template` SET `PrevQuestId` = 28714, `NextQuestId` = 26948, `ExclusiveGroup` = 3120, `NextQuestIdChain` = 26948 WHERE `Id` = 3120;
UPDATE `quest_template` SET `PrevQuestId` = 28714, `NextQuestId` = 26947, `ExclusiveGroup` = 3120, `NextQuestIdChain` = 26947 WHERE `Id` = 3112;
UPDATE `quest_template` SET `PrevQuestId` = 28714, `NextQuestId` = 26940, `ExclusiveGroup` = 3120, `NextQuestIdChain` = 26940 WHERE `Id` = 26841;
UPDATE `quest_template` SET `PrevQuestId` = 28714, `NextQuestId` = 31169, `ExclusiveGroup` = 3120, `NextQuestIdChain` = 31169 WHERE `Id` = 31168;
UPDATE `quest_template` SET `PrevQuestId` = 28714, `NextQuestId` = 26949, `ExclusiveGroup` = 3120, `NextQuestIdChain` = 26949 WHERE `Id` = 3119;
UPDATE `quest_template` SET `PrevQuestId` = 28714, `NextQuestId` = 26946, `ExclusiveGroup` = 3120, `NextQuestIdChain` = 26946 WHERE `Id` = 3118;
UPDATE `quest_template` SET `PrevQuestId` = 28714, `NextQuestId` = 26945, `ExclusiveGroup` = 3120, `NextQuestIdChain` = 26945 WHERE `Id` = 3116;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28723, `ExclusiveGroup` = 26948, `NextQuestIdChain` = 0 WHERE `Id` = 26948;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28723, `ExclusiveGroup` = 26948, `NextQuestIdChain` = 0 WHERE `Id` = 26947;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28723, `ExclusiveGroup` = 26948, `NextQuestIdChain` = 0 WHERE `Id` = 26940;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28723, `ExclusiveGroup` = 26948, `NextQuestIdChain` = 0 WHERE `Id` = 31169;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28723, `ExclusiveGroup` = 26948, `NextQuestIdChain` = 0 WHERE `Id` = 26949;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28723, `ExclusiveGroup` = 26948, `NextQuestIdChain` = 0 WHERE `Id` = 26946;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28723, `ExclusiveGroup` = 26948, `NextQuestIdChain` = 0 WHERE `Id` = 26945;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28724 WHERE `Id` = 28723;
UPDATE `quest_template` SET `PrevQuestId` = 28723, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28725 WHERE `Id` = 28724;
UPDATE `quest_template` SET `PrevQuestId` = 28724, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28726 WHERE `Id` = 28725;
UPDATE `quest_template` SET `PrevQuestId` = 28725, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28727 WHERE `Id` = 28726;
UPDATE `quest_template` SET `PrevQuestId` = 28726, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28728 WHERE `Id` = 28727;
UPDATE `quest_template` SET `PrevQuestId` = 28727, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28729 WHERE `Id` = 28728;
UPDATE `quest_template` SET `PrevQuestId` = 28728, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28730 WHERE `Id` = 28729;
UPDATE `quest_template` SET `PrevQuestId` = 28729, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 28731 WHERE `Id` = 28730;
UPDATE `quest_template` SET `PrevQuestId` = 28730, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 28731;
UPDATE `quest_template` SET `PrevQuestId` = 28730, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 2159;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 488;
UPDATE `quest_template` SET `PrevQuestId` = 488, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 489;
UPDATE `quest_template` SET `PrevQuestId` = 489, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 13949;
UPDATE `quest_template` SET `PrevQuestId` = 489, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 932;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 6341 WHERE `Id` = 6344;
UPDATE `quest_template` SET `PrevQuestId` = 6344, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 6342 WHERE `Id` = 6341;
UPDATE `quest_template` SET `PrevQuestId` = 6341, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 6343 WHERE `Id` = 6342;
UPDATE `quest_template` SET `PrevQuestId` = 6342, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 6343;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 2459 WHERE `Id` = 2438;
UPDATE `quest_template` SET `PrevQuestId` = 2438, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 2459;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 476 WHERE `Id` = 475;
UPDATE `quest_template` SET `PrevQuestId` = 475, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 483 WHERE `Id` = 476;
UPDATE `quest_template` SET `PrevQuestId` = 476, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 486 WHERE `Id` = 483;
UPDATE `quest_template` SET `PrevQuestId` = 476, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 13945;
UPDATE `quest_template` SET `PrevQuestId` = 476, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 2561 WHERE `Id` = 2541;
UPDATE `quest_template` SET `PrevQuestId` = 2541, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 2561;
UPDATE `quest_template` SET `PrevQuestId` = 483, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 486;
UPDATE `quest_template` SET `PrevQuestId` = 483, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 487;
UPDATE `quest_template` SET `PrevQuestId` = 486, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 918 WHERE `Id` = 997;
UPDATE `quest_template` SET `PrevQuestId` = 997, `NextQuestId` = 922, `ExclusiveGroup` = -918, `NextQuestIdChain` = 0 WHERE `Id` = 918;
UPDATE `quest_template` SET `PrevQuestId` = 997, `NextQuestId` = 922, `ExclusiveGroup` = -918, `NextQuestIdChain` = 0 WHERE `Id` = 919;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 923 WHERE `Id` = 922;
UPDATE `quest_template` SET `PrevQuestId` = 918, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 930;
UPDATE `quest_template` SET `PrevQuestId` = 918, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 931;
UPDATE `quest_template` SET `PrevQuestId` = 931, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 2399;
UPDATE `quest_template` SET `PrevQuestId` = 918, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 941 WHERE `Id` = 927;
UPDATE `quest_template` SET `PrevQuestId` = 927, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 941;
UPDATE `quest_template` SET `PrevQuestId` = 922, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 923;
UPDATE `quest_template` SET `PrevQuestId` = 923, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 2499;
UPDATE `quest_template` SET `PrevQuestId` = 918, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 929;
UPDATE `quest_template` SET `PrevQuestId` = 918, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 937;
UPDATE `quest_template` SET `PrevQuestId` = 918, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 2518;
UPDATE `quest_template` SET `PrevQuestId` = 929, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 933;
UPDATE `quest_template` SET `PrevQuestId` = 933, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 14005;
UPDATE `quest_template` SET `PrevQuestId` = 14005, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 935;
UPDATE `quest_template` SET `PrevQuestId` = 935, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 14039;
UPDATE `quest_template` SET `PrevQuestId` = 14039, `NextQuestId` = 0, `ExclusiveGroup` = 26385, `NextQuestIdChain` = 0 WHERE `Id` = 26385;
UPDATE `quest_template` SET `PrevQuestId` = 14039, `NextQuestId` = 0, `ExclusiveGroup` = 26385, `NextQuestIdChain` = 0 WHERE `Id` = 26383;

SET @CGUID:=2573;
DELETE FROM `creature` WHERE `id` = 2079;
INSERT into `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(@CGUID+0, 2079, 1, 1, 1, 0, 1, 10312, 831.302, 1326.5, 5.39306, 120, 0, 0, 1, 0, 0, 0, 0, 0);

UPDATE `creature_addon` SET `auras` = '' WHERE `guid` in (2573, 138335, 138523, 138521, 138522);
UPDATE `creature_template_addon` SET `auras` = '' WHERE `entry` in (49478, 49477);
UPDATE `quest_template` SET `Flags` = 0 WHERE `Id` = 28713;
UPDATE `quest_poi` SET `WorldMapAreaId` = 41, `unk4` = 1 WHERE `questId` = 28713;
UPDATE `creature_template` SET `npcflag` = 0, `mindmg` = 26, `maxdmg` = 26, `Health_Mod` = 2 WHERE `entry` = 2031;
UPDATE `creature` SET `spawndist` = 10, `MovementType` = 1 WHERE `id` = 2031;
UPDATE `creature_classlevelstats` SET `OldContentBaseHP` = 42, `CurrentContentBaseHP` = 42 WHERE `level` = 1 AND `class` = 1;
