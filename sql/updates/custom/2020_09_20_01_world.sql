DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=170 AND `SourceEntry`=4755;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=170 AND `SourceEntry`=4757;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=26 AND `SourceGroup`=170 AND `SourceEntry` IN (4714, 4762);

DELETE FROM `phase_area` WHERE `AreaId`=4755 AND `PhaseId`=170;
DELETE FROM `phase_area` WHERE `AreaId`=4757 AND `PhaseId`=170;
DELETE FROM `phase_area` WHERE `AreaId` IN (4714, 4762) AND `PhaseId`=170;

DELETE FROM `spell_area` WHERE `spell`= 59073 AND `area` IN (4756, 4755, 4757, 4714);
INSERT INTO `spell_area` (`spell`, `area`, `gender`, `quest_start`, `quest_start_status`, `flags`, `quest_end`, `quest_end_status`) VALUES
(59073, 4756, 2, 14078, 64, 1, 14159, 1|2|8|32),
(59073, 4755, 2, 14099, 2|64, 1, 14159, 1|2|8|32),
(59073, 4757, 2, 14099, 2|64, 1, 14159, 1|2|8|32),
(59073, 4714, 2, 14099, 2|64, 1, 14159, 1|2|8|32);

DELETE FROM `spell_area` WHERE `spell`= 49416 AND `area`= 4756;
INSERT INTO `spell_area` (`spell`, `area`, `gender`, `quest_start`, `quest_start_status`, `flags`, `quest_end`, `quest_end_status`) VALUES
(49416, 4756, 2, 14078, 64, 1, 14099, 1|8|32);

-- Generic NPC phasing fixups
UPDATE `creature` SET `phaseId`= 0, `phaseGroup`= 371 WHERE `id` IN (39095, 34863);

-- Generic trigger bunny (multiphase)
UPDATE `creature` SET `phaseId`= 0, `phaseGroup`= 372 WHERE `id` IN (35374, 35830, 35010, 35011) AND `map`= 654;

-- Rampaging Worgen fixups
UPDATE `creature_template` SET `flags_extra` = `flags_extra` | 2 WHERE `entry`= 34884;

DELETE FROM `spell_area` WHERE `spell` IN (72870, 49416, 72872, 76642) AND `area` IN (4755, 4757, 4758, 4759, 4761);
INSERT INTO `spell_area` (`spell`, `area`, `gender`, `quest_start`, `quest_start_status`, `flags`, `quest_end`, `quest_end_status`) VALUES
(72870, 4757, 2, 14159, 64, 1, 14293, 1|2|8|32),
(72870, 4758, 2, 14159, 64, 1, 14293, 1|2|8|32),
(49416, 4757, 2, 14159, 64, 1, 14221, 1|2|8|32),
(72872, 4757, 2, 14293, 64, 1, 14221, 1|2|8|32),
(72872, 4758, 2, 14293, 64, 1, 14221, 1|2|8|32),
(72872, 4759, 2, 14293, 64, 1, 14221, 1|2|8|32),
(72872, 4761, 2, 14293, 64, 1, 14221, 1|2|8|32),
(72872, 4755, 2, 14293, 64, 1, 14221, 1|2|8|32),
-- Last Stand
(76642, 4757, 2, 14221, 64, 1, 14222, 1|2|8|32),
(76642, 4758, 2, 14221, 64, 1, 14222, 1|2|8|32),
(76642, 4759, 2, 14221, 64, 1, 14222, 1|2|8|32),
(76642, 4761, 2, 14221, 64, 1, 14222, 1|2|8|32);

UPDATE `creature` SET `phaseId`= 0, `phaseGroup`= 367 WHERE `id` IN (35906);
UPDATE `creature` SET `phaseId`= 0, `phaseGroup`= 460 WHERE `id` IN (35229, 35317, 35566);
UPDATE `creature` SET `phaseId`= 172, `phaseGroup`= 0 WHERE `id` IN (35618, 36057);
