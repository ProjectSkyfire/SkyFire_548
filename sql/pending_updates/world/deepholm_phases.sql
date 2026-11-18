-- Deepholm: the quest stages of the sniffed spawns. The base data has spawns in phases 237, 251, 252, 253, 254 and 257 (the
-- phase names in the client data: Therazane Siege, Rescue the Stonefather... and Flint, The Stone March, Temple of Earth Finale
-- Stages 1 and 2 and Complete) and nothing that gives a player any of them, so the quest givers, enders and kill targets of
-- the main chain that stand only in those phases were invisible: Bouldergut of "Rescue the Stonefather... and Flint",
-- Therazane of "Our Part of the Bargain", Maruut Stonebinder of "Word In Stone", Earthcaller Torunscar of "The Binding" and
-- "The Stone Throne", Earthcaller Torunscar of "The Very Earth Beneath Our Feet". A phase replaces the default one here, so
-- each stage is an area phase that follows the state of the quest chain: the NPCs of one stage stand only inside its areas
-- (the areas of the sniffed spawns) and only while its quests are in the states below. The stages follow from which quest
-- giver, ender or target stands in which phase; the 4.3.4 data has no phase rows for Deepholm. Not verified against the
-- original game yet.

DELETE FROM `phase_area` WHERE `AreaId` = 5291 AND `PhaseId` = 237;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 237 AND `SourceEntry` = 5291;
DELETE FROM `phase_area` WHERE `AreaId` = 5418 AND `PhaseId` = 237;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 237 AND `SourceEntry` = 5418;
DELETE FROM `phase_area` WHERE `AreaId` = 5350 AND `PhaseId` = 251;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 251 AND `SourceEntry` = 5350;
DELETE FROM `phase_area` WHERE `AreaId` = 5297 AND `PhaseId` = 252;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 252 AND `SourceEntry` = 5297;
DELETE FROM `phase_area` WHERE `AreaId` = 5298 AND `PhaseId` = 252;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 252 AND `SourceEntry` = 5298;
DELETE FROM `phase_area` WHERE `AreaId` = 5299 AND `PhaseId` = 252;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 252 AND `SourceEntry` = 5299;
DELETE FROM `phase_area` WHERE `AreaId` = 5303 AND `PhaseId` = 253;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 253 AND `SourceEntry` = 5303;
DELETE FROM `phase_area` WHERE `AreaId` = 5395 AND `PhaseId` = 253;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 253 AND `SourceEntry` = 5395;
DELETE FROM `phase_area` WHERE `AreaId` = 5303 AND `PhaseId` = 254;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 254 AND `SourceEntry` = 5303;
DELETE FROM `phase_area` WHERE `AreaId` = 5395 AND `PhaseId` = 254;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 254 AND `SourceEntry` = 5395;
DELETE FROM `phase_area` WHERE `AreaId` = 5303 AND `PhaseId` = 257;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 257 AND `SourceEntry` = 5303;

INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(5291, 237, 'Deepholm Therazane Siege, phase 237'),
(5418, 237, 'Deepholm Therazane Siege, phase 237'),
(5350, 251, 'Deepholm Rescue the Stonefather... and Flint, phase 251'),
(5297, 252, 'Deepholm The Stone March, phase 252'),
(5298, 252, 'Deepholm The Stone March, phase 252'),
(5299, 252, 'Deepholm The Stone March, phase 252'),
(5303, 253, 'Deepholm Temple of Earth Finale, Stage 1, phase 253'),
(5395, 253, 'Deepholm Temple of Earth Finale, Stage 1, phase 253'),
(5303, 254, 'Deepholm Temple of Earth Finale, Stage 2, phase 254'),
(5395, 254, 'Deepholm Temple of Earth Finale, Stage 2, phase 254'),
(5303, 257, 'Deepholm Temple of Earth Finale Complete, phase 257');

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(26, 237, 5291, 0, 0, 47, 0, 26326, 74, 0, 0, 0, 0, '', 'Phase 237 in area 5291: 26326 taken'),
(26, 237, 5291, 0, 0, 47, 0, 26328, 1, 0, 0, 0, 0, '', 'Phase 237 in area 5291: 26328 not taken'),
(26, 237, 5291, 0, 0, 47, 0, 26328, 64, 0, 1, 0, 0, '', 'Phase 237 in area 5291: 26328 not rewarded'),
(26, 237, 5418, 0, 0, 47, 0, 26326, 74, 0, 0, 0, 0, '', 'Phase 237 in area 5418: 26326 taken'),
(26, 237, 5418, 0, 0, 47, 0, 26328, 1, 0, 0, 0, 0, '', 'Phase 237 in area 5418: 26328 not taken'),
(26, 237, 5418, 0, 0, 47, 0, 26328, 64, 0, 1, 0, 0, '', 'Phase 237 in area 5418: 26328 not rewarded'),
(26, 251, 5350, 0, 0, 47, 0, 26836, 8, 0, 0, 0, 0, '', 'Phase 251 in area 5350: 26836 in progress'),
(26, 252, 5297, 0, 0, 47, 0, 26828, 74, 0, 0, 0, 0, '', 'Phase 252 in area 5297: 26828 taken'),
(26, 252, 5297, 0, 0, 47, 0, 26833, 66, 0, 1, 0, 0, '', 'Phase 252 in area 5297: 26833 not complete'),
(26, 252, 5298, 0, 0, 47, 0, 26828, 74, 0, 0, 0, 0, '', 'Phase 252 in area 5298: 26828 taken'),
(26, 252, 5298, 0, 0, 47, 0, 26833, 66, 0, 1, 0, 0, '', 'Phase 252 in area 5298: 26833 not complete'),
(26, 252, 5299, 0, 0, 47, 0, 26828, 74, 0, 0, 0, 0, '', 'Phase 252 in area 5299: 26828 taken'),
(26, 252, 5299, 0, 0, 47, 0, 26833, 66, 0, 1, 0, 0, '', 'Phase 252 in area 5299: 26833 not complete'),
(26, 253, 5303, 0, 0, 47, 0, 26833, 66, 0, 0, 0, 0, '', 'Phase 253 in area 5303: 26833 complete'),
(26, 253, 5303, 0, 0, 47, 0, 26971, 74, 0, 1, 0, 0, '', 'Phase 253 in area 5303: 26971 not taken'),
(26, 253, 5395, 0, 0, 47, 0, 26833, 66, 0, 0, 0, 0, '', 'Phase 253 in area 5395: 26833 complete'),
(26, 253, 5395, 0, 0, 47, 0, 26971, 74, 0, 1, 0, 0, '', 'Phase 253 in area 5395: 26971 not taken'),
(26, 254, 5303, 0, 0, 47, 0, 26971, 8, 0, 0, 0, 0, '', 'Phase 254 in area 5303: 26971 in progress'),
(26, 254, 5395, 0, 0, 47, 0, 26971, 8, 0, 0, 0, 0, '', 'Phase 254 in area 5395: 26971 in progress'),
(26, 257, 5303, 0, 0, 47, 0, 26971, 66, 0, 0, 0, 0, '', 'Phase 257 in area 5303: 26971 complete');
