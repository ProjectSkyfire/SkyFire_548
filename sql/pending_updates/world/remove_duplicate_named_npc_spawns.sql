-- Remove duplicate spawns of unique named NPCs (issues #1339 and #1349).
--
-- Chef Grual (272), Farrin Daris (2112), Avette Fellwood (228) and Watcher Bukouris (494)
-- are each spawned twice in Stormwind, a few yards apart. The second copy of each sits in
-- the guid block 192400-192600, which is an ambient wildlife import (rats, frogs, roaches,
-- skunks). That import also pulled in these four named NPCs, which already existed at their
-- original guids 5745, 5761, 5763 and 5767. The other named NPCs inside the same block
-- (Egan, Augustus the Touched, Frax Bucketdrop, Fiona) only have a single spawn each and are
-- left untouched.
--
-- Nipsy (13018) in the Deeprun Tram has the same problem: guid 52990 belongs to the coherent
-- Deeprun Rat Roundup set (Deeprun Rat 13016 at guid 52988, Enthralled Deeprun Rat 13017 at
-- guid 52989), while guid 58 landed in a low guid range that was refilled later (its
-- neighbours 59-62 are Sandstone Earthen, a Cataclysm NPC) and carries spawntimesecs 7200
-- instead of the 180 used by the original row.
--
-- None of the removed guids is referenced by creature_addon, smart_scripts, pool_creature,
-- game_event_creature, linked_respawn or creature_formations.

DELETE FROM `creature` WHERE `guid` IN (192456, 192457, 192458, 192463, 58);
