-- Throne of the Four Winds: the Ravenous Creeper Trigger Anshal summons was a level 1 elite creature with no trigger flag, so
-- it could be targeted and attacked. Level, faction, flags and trigger flag from the 5.4.8 and 4.3.4 data. Safe to run twice.
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 35, `faction_H` = 35, `unit_flags` = 33554688, `flags_extra` = 128 WHERE `entry` = 45813;
