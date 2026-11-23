-- Gilneas (map 654): phase windows and the multi-phase spawns the quest chain needs.
-- Phases replace the default world in this core, so a window only works when every NPC, object and mob the player
-- needs sits in that window's phase. Based on Legends of Azeroth 5.4.8 (window quests, multi-phase NPC sets) and
-- ported from the Cataclysm Preservation Project 4.3.4 (windows for phases 181 to 183, phase groups of the Duskhaven
-- spawns). Phase groups are the client's PhaseXPhaseGroup ids. Safe to run twice: DELETE then INSERT on the same keys,
-- absolute UPDATEs.

-- Phase auras. 59073 (phase 170) is already applied for 14078 to 14159. Each window starts when the previous gate
--    quest is rewarded and ends when its own end quest is rewarded (status 11 = not rewarded).
DELETE FROM `spell_area` WHERE `spell` IN (72870, 72872, 76642, 68481, 68482, 68483, 69484, 69485, 70696, 69486, 70695, 74093) AND `area` IN (4714, 4755);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(72870, 4714, 14159, 14293, 0, 0, 2, 1, 64, 11),
(72870, 4755, 14159, 14293, 0, 0, 2, 1, 64, 11),
(72872, 4714, 14293, 14221, 0, 0, 2, 1, 64, 11),
(72872, 4755, 14293, 14221, 0, 0, 2, 1, 64, 11),
(76642, 4714, 14221, 14222, 0, 0, 2, 1, 64, 11),
(76642, 4755, 14221, 14222, 0, 0, 2, 1, 64, 11),
(68481, 4714, 14375, 14321, 0, 0, 2, 1, 64, 11),
(68481, 4755, 14375, 14321, 0, 0, 2, 1, 64, 11),
(68482, 4714, 14321, 14386, 0, 0, 2, 1, 64, 11),
(68482, 4755, 14321, 14386, 0, 0, 2, 1, 64, 11),
(68483, 4714, 14386, 14466, 0, 0, 2, 1, 64, 11),
(68483, 4755, 14386, 14466, 0, 0, 2, 1, 64, 11),
(69484, 4714, 14466, 24676, 0, 0, 2, 1, 64, 11),
(69484, 4755, 14466, 24676, 0, 0, 2, 1, 64, 11),
(69485, 4714, 24676, 24904, 0, 0, 2, 1, 64, 11),
(69485, 4755, 24676, 24904, 0, 0, 2, 1, 64, 11),
(70696, 4714, 24904, 24902, 0, 0, 2, 1, 64, 11),
(70696, 4755, 24904, 24902, 0, 0, 2, 1, 64, 11),
(69486, 4714, 24902, 24679, 0, 0, 2, 1, 64, 11),
(69486, 4755, 24902, 24679, 0, 0, 2, 1, 64, 11),
(70695, 4714, 24679, 24681, 0, 0, 2, 1, 64, 11),
(70695, 4755, 24679, 24681, 0, 0, 2, 1, 64, 11),
(74093, 4714, 24681, 0, 0, 0, 2, 1, 64, 11),
(74093, 4755, 24681, 0, 0, 0, 2, 1, 64, 11);

-- The old area rule gave phase 170 for good once 14094 was done, so it stacked on every later window.
--    59073 above covers the whole map and the city areas (their zone is 4755).
DELETE FROM `phase_area` WHERE `AreaId` IN (4714, 4755, 4756, 4757, 4762) AND `PhaseId` = 170;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 170 AND `SourceEntry` IN (4714, 4755, 4756, 4757, 4762);

-- NPCs the chain needs across a window change get the phases the references show (groups: 367 = 171+172,
--    460 = 172+179, 429 = 181+182, 431 = 181 to 183, 432 = 183 to 186, 440 = 186+187, 467 = 188+189,
--    479 = 187+188+190, 480 = 188+190, 504 = 189+191, 412 = 169 to 190).
-- Lord Godfrey 35906: starts Time to Regroup after 14293 is rewarded.
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 367 WHERE `map` = 654 AND `id` = 35906;
-- Lord Darius Crowley 35566 and the Rebel Cannons: Never Surrender ends with him and Last Stand runs in 179.
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 460 WHERE `map` = 654 AND `id` IN (35566, 35317);
-- Gwen Armstead 34571: ends Invasion (181) and starts Kill or Be Killed, ends Holding Steady (182).
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 429 WHERE `map` = 654 AND `id` = 34571;
-- Duskhaven residents of 181 that stay for 182 and 183 (class trainers, Krennan, Slain Watchman, captives, crows).
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 431 WHERE `map` = 654 AND `guid` IN (
    235585, 235722, 236821, 236833, 236834, 236885, 236886, 236887,
    236888, 236889, 236890, 236891, 236892, 236893, 236896, 236898,
    236900, 236901, 236902, 236903, 236904, 236912, 236914, 236915,
    236916, 236917, 236918, 236920, 236922, 236923, 236924, 236925,
    236927, 236928, 236929, 236930, 236931, 236932, 236933, 236934,
    236937, 236938, 236939, 236940, 236943, 236944, 236945, 236946,
    236947, 236948, 236949, 236950, 236952, 236953, 236955, 236956,
    236957, 236958, 236959, 236960, 236961, 236962, 236963, 236964,
    236965, 236966, 236967, 236968, 236969, 236971, 236973, 236974,
    236975, 236976, 236977, 236978, 236979, 236980, 236981, 236982,
    236983, 237129);
-- Greymane Manor (Queen Mia) is needed from 183 (To Greymane Manor ends there) and in 186; King Genn 36743 ends
-- The King's Observatory in 183 and gives Alas, Gilneas! and Exodus in 186. Nothing grants phase 184 itself.
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 432 WHERE `map` = 654 AND (`phaseId` = 184 OR `id` = 36743);
UPDATE `gameobject` SET `phaseId` = 183 WHERE `map` = 654 AND `id` IN (196863, 196864);
UPDATE `gameobject` SET `phaseId` = 0, `phaseGroup` = 432 WHERE `map` = 654 AND `phaseId` = 184;
-- Chapter two NPCs that carry the chain over a window change.
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 440 WHERE `map` = 654 AND `id` IN (37783, 37803, 38143);
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 479 WHERE `map` = 654 AND `id` = 38611;
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 480 WHERE `map` = 654 AND `id` = 38539;
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 467 WHERE `map` = 654 AND `id` = 38144;
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 504 WHERE `map` = 654 AND `id` IN (38149, 36616, 43727);
-- Spirit Healers stood in phase 0, which no window shows.
UPDATE `creature` SET `phaseId` = 0, `phaseGroup` = 412 WHERE `map` = 654 AND `id` = 39660;

-- The Swamp Crocolisks and Dark Scouts (guids 8402223 to 8402238) were left in phase 0; Stranded at the Marsh and
--    Losing Your Tail run in 186. Grandma's Good Clothes (I Can't Wear This) stood in 181 but the quest is 183.
UPDATE `creature` SET `phaseId` = 186 WHERE `guid` BETWEEN 8402223 AND 8402238 AND `id` IN (37078, 37953);
UPDATE `gameobject` SET `phaseId` = 183 WHERE `map` = 654 AND `id` = 196472;
