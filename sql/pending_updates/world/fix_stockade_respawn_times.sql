-- The Stockade: every creature respawned after 120 seconds, so cleared trash came back while a group was still inside
-- and the walk back through the hub met a full set of mobs again. LoA 5.4.8 and the Cataclysm 4.3.4 data give the
-- dungeon spawns 7200 seconds (a few bosses longer); use 7200 for every spawn that still has the 120 second value.
UPDATE `creature` SET `spawntimesecs` = 7200 WHERE `map` = 34 AND `spawntimesecs` = 120;
