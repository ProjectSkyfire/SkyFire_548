-- The Storm Peaks: Creteus, the two Brann Bronzebeards and Velog Icebellow (The Brothers Bronzebeard line for the
-- Alliance, 12879, 12880, 12973) stand in phases 170 and 171, which the auras 55782 and 55783 give around Valkyrion;
-- Thorim (13010, 13057) is in the default phase. Positions and auras from 5.4.8.
DELETE FROM `creature` WHERE `guid` BETWEEN 8320300 AND 8320304;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8320300, 30082, 571, 1, 170, 0, 0, 0, 7263.44, -873.846, 925.126, 5.43711, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(8320301, 30382, 571, 1, 170, 0, 0, 0, 7524.93, -972.161, 478.667, 5.40885, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(8320302, 30107, 571, 1, 171, 0, 0, 0, 7528.35, -980.098, 467.43, 5.08716, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8320303, 30401, 571, 1, 171, 0, 0, 0, 6653.72, -310.77, 989.38, 1.88619, 600, 0, 0, 0, 0, 0, 0, 0, 0),
(8320304, 30390, 571, 1, 0, 0, 0, 0, 7882.51, -3236.7, 849.411, 4.42864, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `spell_area` WHERE (`spell` = 55782 AND `area` IN (4431, 4432) AND `quest_start` = 12879) OR (`spell` = 55783 AND `area` IN (4428, 4431, 4432) AND `quest_start` = 12880);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(55782, 4431, 12879, 12880, 0, 18875469, 2, 1, 74, 11),
(55782, 4432, 12879, 12880, 0, 18875469, 2, 1, 74, 11),
(55783, 4428, 12880, 0, 0, 18875469, 2, 1, 64, 11),
(55783, 4431, 12880, 0, 0, 18875469, 2, 1, 64, 11),
(55783, 4432, 12880, 0, 0, 18875469, 2, 1, 64, 11);

-- A phase aura replaces the default phase for the player, so every unphased spawn of an area that gives one vanishes
-- unless the area also gives the default phase 169. This covers the areas that already had a phase aura here (55858
-- after Forging an Alliance, 56526 after 13007) and the three added above.
DELETE FROM `phase_area` WHERE `PhaseId` = 169 AND `AreaId` IN (4428, 4431, 4432, 4435, 4436, 4437, 4438, 4439, 4440, 4455, 4495);
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(4428, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4431, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4432, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4435, 169, 'The Storm Peaks - Narvir''s Cradle - Phase 169 - Default world stays visible'),
(4436, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4437, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4438, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4439, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4440, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4455, 169, 'The Storm Peaks - Phase 169 - Default world stays visible'),
(4495, 169, 'The Storm Peaks - Phase 169 - Default world stays visible');
