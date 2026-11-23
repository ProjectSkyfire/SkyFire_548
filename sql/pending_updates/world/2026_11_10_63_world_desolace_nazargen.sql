-- Desolace: Putting Their Heads Together (14227, 14364). Using a Bleached Skull sends event 22225, which had no
-- script, so Nazargen was never summoned at the Burning Blade Summoning Circle. Safe to run twice.
DELETE FROM `event_scripts` WHERE `id` = 22225;
INSERT INTO `event_scripts` (`id`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`)
VALUES (22225,2,10,35647,180000,0,-348.068,1763.64,138.371,2.33778);
