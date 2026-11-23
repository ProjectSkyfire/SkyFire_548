-- Tol Barad Peninsula, "Bombs Away!" (28275, 28696): the quest uses the Wellson Cannons on the docks (48283, 8 spawns) to
-- sink the supply boats. The cannon has no click flag and no spell click, so a player cannot get on it. The spell click of the
-- 5.4.8 data is "Ride Vehicle - Wellson Boat Quest" (94354).

UPDATE `creature_template` SET `npcflag` = 16777216 WHERE `entry` = 48283;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 48283;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(48283, 94354, 1, 0);
