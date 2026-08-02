-- Darkmoon Faire cannon scorer position and scoring area.

SET @DARKMOON_CANNON_TARGET_GUID := 8200300;

START TRANSACTION;

UPDATE `creature`
SET `position_x` = -4479.484863,
    `position_y` = 6222.407227,
    `position_z` = -1.569462,
    `orientation` = 0
WHERE `guid` = @DARKMOON_CANNON_TARGET_GUID
  AND `id` = 54224;

COMMIT;
