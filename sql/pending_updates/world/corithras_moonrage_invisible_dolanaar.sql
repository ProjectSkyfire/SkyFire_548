-- Corithras Moonrage (entry 3515) is invisible in Dolanaar.
--
-- His Dolanaar spawn (guid 138764) carries aura 85813 in `creature_addon`. It is the only row
-- in the world database referencing that spell. The aura hides the questgiver from players
-- completely: the creature is spawned server side, but is never sent to the client, so it also
-- never reaches the client's creaturecache.wdb. `.npc add 3515` produces a visible copy, since
-- a fresh spawn has no `creature_addon` row.
--
-- Clear the aura only and leave the rest of the addon row untouched.
UPDATE `creature_addon` SET `auras`=NULL WHERE `guid`=138764;
