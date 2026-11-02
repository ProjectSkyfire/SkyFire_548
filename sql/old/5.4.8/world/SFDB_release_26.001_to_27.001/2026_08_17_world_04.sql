-- Per-creature aggro detection range.
--
-- Sections:
--   1. New `creature_template`.`detection_range` column
--   2. Starting-zone detection ranges


-- ---------------------------------------------------------------------------
-- 1. New `creature_template`.`detection_range` column
-- ---------------------------------------------------------------------------
-- Radius, in yards, at which a creature notices a target of its own level.
-- The core adjusts it by one yard per level of difference and keeps the result
-- inside the 5 to 45 yard range, so 20 reproduces the previous behavior.

ALTER TABLE `creature_template`
    ADD COLUMN `detection_range` FLOAT NOT NULL DEFAULT 20 AFTER `ModLevel`;


-- ---------------------------------------------------------------------------
-- 2. Starting-zone detection ranges
-- ---------------------------------------------------------------------------
-- Creatures in the starting-zone level bracket let players come far closer
-- before engaging than the rest of the world does, which keeps a single pull
-- from dragging in the surrounding camp.

UPDATE `creature_template` SET `detection_range`=10 WHERE `maxlevel` <= 6;
