-- Zul'Drak: the Altar of Quetz'lun gives every player who finished 12667 and not yet 12675 the phase aura 51671
-- (phase 170). A phase aura replaces the default phase, so all unphased spawns of the area vanished for those players.
-- Giving the default phase 169 in the area keeps them visible.
DELETE FROM `phase_area` WHERE `AreaId` = 4325 AND `PhaseId` = 169;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(4325, 169, 'Zul''Drak - Altar of Quetz''lun - Phase 169 - Default world stays visible');
