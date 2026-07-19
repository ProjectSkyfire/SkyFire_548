-- Bellygrub (345): remove 3 stacked duplicate spawns in Redridge Mountains (map 0).
-- SFDB baseline had 4 spawns within ~20yd; trinitycore/retail has 1 at guid 89612 coords.
-- Not from LOA MoP port (world_sfdb already contained all 4).
DELETE FROM `creature` WHERE `guid` IN (89480, 89585, 89611) AND `id` = 345;
