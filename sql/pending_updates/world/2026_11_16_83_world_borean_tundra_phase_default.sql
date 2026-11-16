-- Borean Tundra: the aura 54119 (phase 170) is cast on every player in Riplash Strand, Riplash Ruins, Pal'ea, Garrosh's
-- Landing and the Shrine of Scales. A phase aura replaces the default phase, so all unphased spawns of these areas
-- vanished. Giving the default phase 169 there keeps them visible.
DELETE FROM `phase_area` WHERE `PhaseId` = 169 AND `AreaId` IN (4028, 4029, 4031, 4106, 4269);
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(4028, 169, 'Borean Tundra - Riplash Strand - Phase 169 - Default world stays visible'),
(4029, 169, 'Borean Tundra - Riplash Ruins - Phase 169 - Default world stays visible'),
(4031, 169, 'Borean Tundra - Pal''ea - Phase 169 - Default world stays visible'),
(4106, 169, 'Borean Tundra - Garrosh''s Landing - Phase 169 - Default world stays visible'),
(4269, 169, 'Borean Tundra - Shrine of Scales - Phase 169 - Default world stays visible');
