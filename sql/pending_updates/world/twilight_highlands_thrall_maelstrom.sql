-- Twilight Highlands, "Devoured" 27377 to "The Worldbreaker" 27378: Thrall (48051) has no spawn in the world, so the quest
-- cannot be handed in. In the original game the player is carried to the Maelstrom while Iso'rath digests them (the teleport
-- spell 89471, "Maelstrom Nightmare Teleport", lands at 847.2, 1040.0, -5.7 on map 730) and Thrall stands there; the client data
-- has the spell "Summon Thrall" (89607) for him. The teleport has no target position here, and the Maelstrom already holds the
-- Deepholm intro Thrall (45042) at the same place, so the dream Thrall stands in phase 327, which "Isorath Nightmare Phase"
-- (89206) gives and which the area aura below applies while the player is in the Maelstrom with the two quests. Map 730 belongs to
-- the Deepholm range of the zone import, so the spawn uses its guid. Based on Legends of Azeroth 5.4.8 and the Cataclysm Preservation Project 4.3.4.

DELETE FROM `spell_target_position` WHERE `id` = 89471;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(89471, 0, 730, 847.207, 1040.03, -5.66212, 5.17286); -- Maelstrom Nightmare Teleport

DELETE FROM `creature` WHERE `guid` = 9277900;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9277900, 48051, 730, 1, 327, 0, 36056, 0, 852.306, 1038.92, -5.3136, 2.93, 300, 0, 0, 42946000, 0, 0, 0, 0, 0); -- Thrall, the dream of the Maelstrom, beside the arrival point

-- Phase 327 while the player is in the Maelstrom (area 5416) with "Devoured" taken or done and "The Worldbreaker" not done
DELETE FROM `spell_area` WHERE `spell` = 89206 AND `area` = 5416;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(89206, 5416, 27377, 27378, 0, 0, 2, 1, 74, 9); -- Isorath Nightmare Phase
