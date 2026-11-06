-- Gilneas: seven Rampaging Worgen (34884) stood on top of another one (same spot within 1.5 yards), so the packs in
-- the streets were doubled and up to five worgen rose from one point. The upper copies are removed; every spot keeps
-- one spawn (57 to 50; 5.4.8 has 45). None of them has an addon, script, pool, formation or event row.
DELETE FROM `creature` WHERE `id` = 34884 AND `guid` IN (236769, 236779, 236787, 236797, 236799, 236801, 237122);
