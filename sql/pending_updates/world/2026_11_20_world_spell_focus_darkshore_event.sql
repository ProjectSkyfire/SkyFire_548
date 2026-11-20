-- The Lifebringer Sapling (spell 65207) sends event 21674 at the Consumed Watering Hole, but the event had no script, so The Devourer of Darkshore never
-- got its credit creature (34331). Add the event row that summons it.
-- Based on Legends of Azeroth 5.4.8.
DELETE FROM `event_scripts` WHERE `id` = 21674;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(21674, 2, 10, 34331, 180000, 0, 4809.26, 105.603, 45.5833, 5.54021);
