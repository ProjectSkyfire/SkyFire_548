-- Throne of the Four Winds: the Fall Catcher that lifts a falling player back up had no vehicle seat, so the player
-- could not ride it. Level, flags and vehicle from 5.4.8 data. Safe to run twice.
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3, `unit_flags` = 512, `flags_extra` = 128, `VehicleId` = 1134 WHERE `entry` = 45710;
