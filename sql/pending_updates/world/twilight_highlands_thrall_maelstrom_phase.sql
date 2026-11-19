-- Twilight Highlands, "Devoured" 27377 to "The Worldbreaker" 27378: Thrall (48051) stands in phase 327 in the Maelstrom, and the
-- area aura "Isorath Nightmare Phase" (89206) that was meant to give the phase never applies there: the spell can only be cast
-- in the area group 2829 of the client data, which holds Iso'rath (5664) alone, so the cast on arrival fails and the area
-- update drops the aura. The phase now comes from the area itself: phase 327 in the Maelstrom (area 5416) for a player who
-- took, completed or handed in "Devoured" and has not handed in "The Worldbreaker". The quest steps refresh area phases
-- (Player::UpdateQuestDependentPhasing), so Thrall appears as soon as the player takes the quest.

DELETE FROM `spell_area` WHERE `spell` = 89206 AND `area` = 5416;

DELETE FROM `phase_area` WHERE `AreaId` = 5416 AND `PhaseId` = 327;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(5416, 327, 'The Maelstrom - Phase 327 - Devoured to The Worldbreaker (Thrall of the dream)');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 327 AND `SourceEntry` = 5416;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(26, 327, 5416, 0, 0, 9, 0, 27377, 0, 0, 0, 0, 0, '', 'Set Phase 327 in area 5416 if q27377 taken'),
(26, 327, 5416, 0, 0, 8, 0, 27378, 0, 0, 1, 0, 0, '', 'Set Phase 327 in area 5416 if q27378 not rewarded'),
(26, 327, 5416, 0, 1, 28, 0, 27377, 0, 0, 0, 0, 0, '', 'Set Phase 327 in area 5416 if q27377 complete'),
(26, 327, 5416, 0, 1, 8, 0, 27378, 0, 0, 1, 0, 0, '', 'Set Phase 327 in area 5416 if q27378 not rewarded'),
(26, 327, 5416, 0, 2, 8, 0, 27377, 0, 0, 0, 0, 0, '', 'Set Phase 327 in area 5416 if q27377 rewarded'),
(26, 327, 5416, 0, 2, 8, 0, 27378, 0, 0, 1, 0, 0, '', 'Set Phase 327 in area 5416 if q27378 not rewarded');
