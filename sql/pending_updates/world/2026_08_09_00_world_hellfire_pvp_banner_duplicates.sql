-- Remove static Hellfire Peninsula outdoor PvP objects owned by OutdoorPvPHP.
-- The script spawns and updates these objects dynamically as capture state changes.
DELETE FROM `gameobject`
WHERE `map` = 530
  AND (
    (`id` = 182173 AND ABS(`position_x` + 290.016) < 0.05 AND ABS(`position_y` - 3702.42) < 0.05 AND ABS(`position_z` - 56.6729) < 0.05) OR
    (`id` = 182174 AND ABS(`position_x` + 184.889) < 0.05 AND ABS(`position_y` - 3476.93) < 0.05 AND ABS(`position_z` - 38.205) < 0.05) OR
    (`id` = 182175 AND ABS(`position_x` + 471.462) < 0.05 AND ABS(`position_y` - 3451.09) < 0.05 AND ABS(`position_z` - 34.6432) < 0.05) OR
    (`id` = 182525 AND ABS(`position_x` + 187.887) < 0.05 AND ABS(`position_y` - 3459.38) < 0.05 AND ABS(`position_z` - 60.0403) < 0.05) OR
    (`id` = 183514 AND ABS(`position_x` + 467.078) < 0.05 AND ABS(`position_y` - 3528.17) < 0.05 AND ABS(`position_z` - 64.7121) < 0.05) OR
    (`id` = 183515 AND ABS(`position_x` + 289.610) < 0.05 AND ABS(`position_y` - 3696.83) < 0.05 AND ABS(`position_z` - 75.9447) < 0.05)
  );
