-- Monty (12997), the Deeprun Tram rat quest giver, is spawned twice on the same spot.
--
-- guid 52957: duplicate of guid 40 (kept). Same entry, 0.09 yards apart (-19.63, 40.08, -4.21 against -4.30), same
-- orientation 4.8, same phase 0 and spawn mask 1. Not pooled, not in a game event, no creature_addon, formation,
-- linked respawn, waypoint, condition or SmartAI row on the guid, and the entry is never summoned (no summon action or
-- summon group names it). Legends of Azeroth 5.4.8 has one Monty at that spot (its rows are unverified, VerifiedBuild
-- 0, so this is weaker evidence); the Cataclysm Preservation Project 4.3.4 has the same two as here. One Monty remains.

DELETE FROM `creature` WHERE `guid` IN (52957);
