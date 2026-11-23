-- Mount Hyjal: the spawns in the upstream Mount Hyjal data carry no phase. In this core an unphased spawn is seen only
-- by a player with no phase or in phase 169, and the areas below put the player in phase 194 or 195, so the spawns
-- stayed invisible there. Phase 169 in the same areas keeps them visible.
DELETE FROM `phase_area` WHERE `AreaId` IN (4861, 4979, 4989, 4990, 5013, 5014, 5016, 5018, 5019, 5020, 5022) AND `PhaseId` = 169;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(4861, 169, 'Mount Hyjal - default phase'),
(4979, 169, 'Mount Hyjal - default phase'),
(4989, 169, 'Mount Hyjal - default phase'),
(4990, 169, 'Mount Hyjal - default phase'),
(5013, 169, 'Mount Hyjal - default phase'),
(5014, 169, 'Mount Hyjal - default phase'),
(5016, 169, 'Mount Hyjal - default phase'),
(5018, 169, 'Mount Hyjal - default phase'),
(5019, 169, 'Mount Hyjal - default phase'),
(5020, 169, 'Mount Hyjal - default phase'),
(5022, 169, 'Mount Hyjal - default phase');
