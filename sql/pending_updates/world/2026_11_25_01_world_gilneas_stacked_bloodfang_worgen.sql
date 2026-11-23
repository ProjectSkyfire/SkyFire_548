-- Gilneas City: three Bloodfang Worgen (35118) stood on the exact spot of another one, so they looked like one
-- worgen with a doubled health bar. Each removed spawn has the same entry and phase (170, no phase group)
-- within 1 yard of the kept one, is not pooled, not in a game event, has no addon, formation, linked respawn or script
-- row, and the entry is never summoned (no summon spell, summon group or SmartAI/C++ summon uses 35118).
-- Kept: 236530 and 236656. Both references show fewer worgen at these spots:
--   236625 (-1733.18, 1418.10, 21.79), 0.55 yd from 236530 (-1733.30, 1417.55, 21.75): Legends of Azeroth 5.4.8 has
--     one worgen within 6 yd of the spot (guid 221204) and the Cataclysm Preservation Project 4.3.4 sniff (build
--     15595) has one at the same spot as 236530 (guid 376564).
--   236660 (-1726.74, 1392.28, 21.53) and 236667 (-1726.12, 1391.94, 21.53), 0.4 to 0.7 yd from 236656
--     (-1726.37, 1392.09, 21.53): LoA has none within 6 yd of the spot and TCPP two (matching the existing spawns 236604
--     and 236596); within 10 yd the references have 3 (LoA) and 3 (TCPP) against 6 here, and 4 remain after this file.
-- 35118 total on the map: 43 before, 40 after (LoA 38, TCPP 30). Not removed: the pair 236537 and 236627 (TCPP has five
-- at one spot 3 yd away, LoA one), the pair 236684 and 236691 (the base data has fewer there than LoA).
-- Based on Legends of Azeroth 5.4.8 and the Cataclysm Preservation Project 4.3.4.

DELETE FROM `creature` WHERE `guid` IN (236625, 236660, 236667) AND `id` = 35118 AND `map` = 654;
