-- Uldum, Schnottz's Landing (area 5602) and Schnottz's Hostel (area 5700): the four chapters of the Landing as phases.
--
-- Stages (Phase.dbc "Schnottz's Landing - Chapter 1 to 4"; windows from the 4.3.4 phase_area and conditions rows, with the
-- end of 324 and the start of 325 added):
--   313 Chapter 1: no aura 88111 (Phase Player) and quest 27950 (Gobbles!) not rewarded
--   315 Chapter 2: 27950 rewarded and 28002 (Crisis Management) not rewarded
--   324 Chapter 3: 28002 rewarded and 27990 (Battlezone) not complete
--   325 Chapter 4: 27990 complete or rewarded (stays)
-- 324 and 325 are exact complements (27990 state mask 66 = complete or rewarded), so a player has exactly one chapter.
-- Phase groups 528 {313, 315} and 532 {313, 315, 324} come from PhaseXPhaseGroup.dbc and need no row here.
-- Every area that gives a chapter also gives the default phase 169, or the unphased spawns there would vanish.
-- The Gobbles ritual phase 312 stays out (scene, not built yet), and so does its 4.3.4 phase_area row.
-- Quest state masks (condition type 47): 1 none, 2 complete, 8 incomplete, 32 failed, 64 rewarded.
-- Area ids are the 5.4.8 ones (AreaTable.dbc: 5602 and 5700, parent 5034 Uldum).
--
-- Replaces: nothing. The live world has no phase_area, spell_area or type 26 condition row for 5602 or 5700 and no
-- phase id 312 to 325 anywhere. The spell_area rows 88144 and 88847 of the detection file stay as they are.
-- The spawns of the chapters come with the Uldum spawn update (2026_11_07_16); the UPDATEs at the end give the
-- 122 creatures and 1 gameobject already in the live world their chapter, so the old and the new chapters do not show
-- together. Each UPDATE names the guid and the entry, so a changed row is left alone.

DELETE FROM `phase_area` WHERE `AreaId` IN (5602, 5700) AND `PhaseId` IN (169, 313, 315, 324, 325);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` IN (313, 315, 324, 325) AND `SourceEntry` IN (5602, 5700);

INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(5602, 169, 'Uldum - Schnottz''s Landing - Phase 169 - Default world stays visible'),
(5700, 169, 'Uldum - Schnottz''s Hostel - Phase 169 - Default world stays visible'),
(5602, 313, 'Uldum - Schnottz''s Landing - Phase 313 - Chapter 1, until quest 27950 rewarded'),
(5700, 313, 'Uldum - Schnottz''s Hostel - Phase 313 - Chapter 1, until quest 27950 rewarded'),
(5602, 315, 'Uldum - Schnottz''s Landing - Phase 315 - Chapter 2, quest 27950 rewarded until 28002 rewarded'),
(5700, 315, 'Uldum - Schnottz''s Hostel - Phase 315 - Chapter 2, quest 27950 rewarded until 28002 rewarded'),
(5602, 324, 'Uldum - Schnottz''s Landing - Phase 324 - Chapter 3, quest 28002 rewarded until 27990 complete'),
(5700, 324, 'Uldum - Schnottz''s Hostel - Phase 324 - Chapter 3, quest 28002 rewarded until 27990 complete'),
(5602, 325, 'Uldum - Schnottz''s Landing - Phase 325 - Chapter 4, quest 27990 complete'),
(5700, 325, 'Uldum - Schnottz''s Hostel - Phase 325 - Chapter 4, quest 27990 complete');

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(26, 313, 5602, 0, 0, 1, 0, 88111, 0, 0, 1, 0, 0, '', 'Set Phase 313 in area 5602 if the player has no aura Phase Player'),
(26, 313, 5602, 0, 0, 8, 0, 27950, 0, 0, 1, 0, 0, '', 'and q27950 is not rewarded'),
(26, 313, 5700, 0, 0, 1, 0, 88111, 0, 0, 1, 0, 0, '', 'Set Phase 313 in area 5700 if the player has no aura Phase Player'),
(26, 313, 5700, 0, 0, 8, 0, 27950, 0, 0, 1, 0, 0, '', 'and q27950 is not rewarded'),
(26, 315, 5602, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'Set Phase 315 in area 5602 if q27950 is rewarded'),
(26, 315, 5602, 0, 0, 8, 0, 28002, 0, 0, 1, 0, 0, '', 'and q28002 is not rewarded'),
(26, 315, 5700, 0, 0, 8, 0, 27950, 0, 0, 0, 0, 0, '', 'Set Phase 315 in area 5700 if q27950 is rewarded'),
(26, 315, 5700, 0, 0, 8, 0, 28002, 0, 0, 1, 0, 0, '', 'and q28002 is not rewarded'),
(26, 324, 5602, 0, 0, 8, 0, 28002, 0, 0, 0, 0, 0, '', 'Set Phase 324 in area 5602 if q28002 is rewarded'),
(26, 324, 5602, 0, 0, 47, 0, 27990, 66, 0, 1, 0, 0, '', 'and q27990 is not complete or rewarded'),
(26, 324, 5700, 0, 0, 8, 0, 28002, 0, 0, 0, 0, 0, '', 'Set Phase 324 in area 5700 if q28002 is rewarded'),
(26, 324, 5700, 0, 0, 47, 0, 27990, 66, 0, 1, 0, 0, '', 'and q27990 is not complete or rewarded'),
(26, 325, 5602, 0, 0, 47, 0, 27990, 66, 0, 0, 0, 0, '', 'Set Phase 325 in area 5602 if q27990 is complete or rewarded'),
(26, 325, 5700, 0, 0, 47, 0, 27990, 66, 0, 0, 0, 0, '', 'Set Phase 325 in area 5700 if q27990 is complete or rewarded');

-- Live spawns: their chapter. Guid and entry from the live world, chapter from the 4.3.4 data (live_creature, live_gameobject).
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 528 WHERE (`guid`, `id`) IN (
    (181196, 47546), (181199, 47546), (181200, 47546), (181201, 47546), (181204, 47546), (181207, 47546), (181210, 47546), (181226, 47546),
    (181274, 47546));
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 532 WHERE (`guid`, `id`) IN (
    (181202, 47772), (181203, 47452), (181205, 47772), (181206, 47452), (181208, 47455), (181209, 47732), (181219, 47193), (181220, 47193),
    (181221, 47193), (181227, 47193), (181229, 49528), (181232, 48273), (181234, 47193), (181235, 47193), (181237, 47193), (181240, 49525),
    (181245, 48959), (181252, 47193), (181260, 47193), (181262, 47193), (181263, 47982), (181264, 47982), (181267, 47982), (181268, 47982),
    (181269, 47191), (181270, 47193), (181271, 47193), (181272, 47670), (181273, 47193), (181277, 47982), (181278, 47982), (181279, 47982),
    (181281, 47982), (181282, 47982), (181283, 47982), (181284, 47982), (181285, 47982), (181286, 47982), (181287, 47193), (181288, 47193));
UPDATE `creature` SET `phaseId` = 313, `phaseGroup` = 0 WHERE (`guid`, `id`) IN (
    (181193, 47291), (181194, 47291), (181195, 47291), (181197, 47291), (181198, 47291), (181222, 47291), (181223, 47291), (181224, 47291),
    (181225, 47185), (181228, 47185), (181230, 47514), (181231, 47461), (181233, 47472), (181236, 47187), (181238, 47472), (181239, 47189),
    (181241, 47700), (181242, 47187), (181243, 47185), (181244, 47185), (181246, 47514), (181247, 47187), (181248, 47189), (181249, 47461),
    (181250, 47461), (181251, 47472), (181253, 47461), (181254, 47189), (181255, 47472), (181256, 47187), (181257, 47461), (181258, 47514),
    (181259, 47185), (181261, 47176), (181265, 47698), (181275, 47291), (181276, 47291), (181289, 47291), (181290, 47291), (181291, 47291),
    (181292, 47291), (181293, 47291), (181490, 47291), (181491, 47291), (181492, 47291), (181493, 47291), (181494, 47291), (181495, 47291),
    (181496, 47291), (181497, 47291), (181498, 47291), (181499, 47291), (181500, 47291), (181501, 47291), (181502, 47291), (181503, 47291),
    (181504, 47291), (181505, 47291), (181506, 47291), (181507, 47291), (181508, 47291), (181509, 47291), (181510, 47291), (181517, 47291),
    (181518, 47291), (181519, 47291), (181520, 47291), (181521, 47291), (181522, 47291), (181523, 47291), (181524, 47291), (181525, 47291));
UPDATE `creature` SET `phaseId` = 315, `phaseGroup` = 0 WHERE (`guid`, `id`) IN (
    (10, 47707));
UPDATE `gameobject` SET `phaseId` = 0, `phaseGroup` = 532 WHERE (`guid`, `id`) IN (
    (77419, 205385));
