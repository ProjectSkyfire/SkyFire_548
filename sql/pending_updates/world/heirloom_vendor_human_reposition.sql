-- mod-prabowow: move the Heirloom Vendor at the Human start point off the NPC it overlaps.
--
-- Problem
--   2026_09_02_world_03.sql spawns one vendor (entry 900001) per distinct row in
--   `playercreateinfo`, 4 yards straight in front of the start position and facing
--   back at it. At the Human start in Northshire that spot is already occupied, so
--   the vendor stands inside another NPC.
--
--   No spawn in that file has literal coordinates -- position and guid are both
--   derived at apply time (`8300000 + ROW_NUMBER() OVER (ORDER BY map, position_x,
--   position_y)`). The Human vendor's guid therefore exists only in the live DB and
--   cannot be hardcoded here. This update derives it the same way the spawn was
--   derived, so it stays correct whatever the dump numbered it.
--
--   2026_09_02_world_03.sql itself must not be edited: it is already recorded in
--   `skyfire_db_updates`, and production runs `WorldDatabase.AllowUpdateHashMismatch = 0`,
--   so a changed hash makes worldserver refuse to start.
--
-- Approach
--   Rather than guess a free spot, search for one. Twelve candidate points sit on a
--   ring around the Human start position; only the seven at 90 degrees or more away
--   from the facing direction are considered, because the crowded spot is straight
--   ahead. The first candidate with no other creature within CLEAR_RADIUS wins,
--   preferring straight sideways and falling back towards directly behind.
--
--   If no candidate is clear the UPDATE matches nothing and the vendor stays put --
--   it never writes NULL over a good position.
--
-- Tuning
--   Only the four SET values below need to change to move the vendor further out,
--   loosen the clearance test, or widen the vertical tolerance.

SET @ENTRY        := 900001;
SET @RING         := 8;    -- yards from the start point to place the vendor
SET @CLEAR_RADIUS := 5;    -- a candidate is rejected if another creature is this close
SET @Z_TOLERANCE  := 6;    -- creatures further than this above/below are a different floor
SET @MATCH_RADIUS := 5;    -- how close a spawn must be to be recognised as the Human vendor

-- Human start position (race 1). Every Human class shares one spot.
SELECT `position_x`, `position_y`, `position_z`, `orientation`
  INTO @HX, @HY, @HZ, @HO
FROM `playercreateinfo`
WHERE `race` = 1 AND `map` = 0
ORDER BY `class`
LIMIT 1;

-- Where 2026_09_02_world_03.sql put the vendor: 4 yards along the start orientation.
SET @VX := @HX + 4 * COS(@HO);
SET @VY := @HY + 4 * SIN(@HO);

-- The MATCH_RADIUS guard keeps this off the Pandaren-Alliance vendor, the only other
-- entry 900001 spawn on map 0 (Stormwind Trade District, ~700 yards away).
SET @VGUID := (
    SELECT `guid`
    FROM `creature`
    WHERE `id` = @ENTRY
      AND `map` = 0
      AND POW(`position_x` - @VX, 2) + POW(`position_y` - @VY, 2) < POW(@MATCH_RADIUS, 2)
    ORDER BY POW(`position_x` - @VX, 2) + POW(`position_y` - @VY, 2)
    LIMIT 1
);

SET @NEWX := NULL;
SET @NEWY := NULL;

-- k counts 30-degree steps from the facing direction. Only k = 3..9 is offered, i.e.
-- 90 to 270 degrees away from straight ahead. LEAST(k, 12 - k) is the step distance
-- from ahead, so ordering by it ascending tries 90 degrees first (k = 3 or 9) and
-- directly behind last (k = 6); the trailing k breaks the left/right tie.
SELECT `x`, `y`
  INTO @NEWX, @NEWY
FROM (
    SELECT `k`,
           @HX + @RING * COS(@HO + `k` * PI() / 6) AS `x`,
           @HY + @RING * SIN(@HO + `k` * PI() / 6) AS `y`
    FROM (
        SELECT 3 AS `k` UNION ALL SELECT 4 UNION ALL SELECT 5 UNION ALL
        SELECT 6        UNION ALL SELECT 7 UNION ALL SELECT 8 UNION ALL
        SELECT 9
    ) AS `angles`
) AS `cand`
WHERE NOT EXISTS (
    SELECT 1
    FROM `creature` `other`
    WHERE `other`.`map` = 0
      AND `other`.`id` <> @ENTRY
      AND ABS(`other`.`position_z` - @HZ) < @Z_TOLERANCE
      AND POW(`other`.`position_x` - `cand`.`x`, 2)
        + POW(`other`.`position_y` - `cand`.`y`, 2) < POW(@CLEAR_RADIUS, 2)
)
ORDER BY LEAST(`k`, 12 - `k`), `k`
LIMIT 1;

UPDATE `creature`
SET `position_x`  = @NEWX,
    `position_y`  = @NEWY,
    `position_z`  = @HZ,
    `orientation` = MOD(ATAN2(@HY - @NEWY, @HX - @NEWX) + 2 * PI(), 2 * PI())
WHERE `guid` = @VGUID
  AND @NEWX IS NOT NULL;
