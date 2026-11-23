-- Dragonmaw Sky Stalker (23030) circled over the courtyard unselectable: the template carried the not selectable flag,
-- and it was removed only once the rider was in combat, which a rider flying high above may never be. 5.4.8 has the
-- riders selectable at all times.
UPDATE `creature_template` SET `unit_flags` = `unit_flags` & ~33554432 WHERE `entry` = 23030;
