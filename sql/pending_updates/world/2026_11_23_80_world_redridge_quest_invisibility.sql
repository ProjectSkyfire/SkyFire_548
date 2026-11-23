-- Redridge Mountains, Lakeshire: Marshal Marris, the bridge workers, the fight club in the inn basement and the town hall
-- crowd carry an "Apply Quest Invis Zone" aura (Zone 1, 3 and 19) and stay invisible to players that do not carry the
-- matching "Detect: Quest Invis Zone" aura. The spell_area rows that hand out the detect auras were missing, so Marshal
-- Marris could not be seen and his quests (26513 Like a Fart in the Wind, 26570 Render's Army) could not be taken, and the
-- Lakeshire Inn fight club never showed for the quest "John J. Keeshan" (26567).
-- The Zone 1 row is as in Legends of Azeroth 5.4.8. The other three follow the Cataclysm Preservation Project 4.3.4 stages:
-- the fight club shows until "They Drew First Blood" (26607) is taken, Bravo Company waits in the basement until
-- "It's Never Over" (26616) is taken and the town hall crowd is there until the same quest is taken.
-- Bravo Company (Messner 43432, Jorgensen 43433, Krakauer 43434, Danforth 43435) is spawned in the basement as in
-- Legends of Azeroth 5.4.8 with the Zone 9 aura. The 5.4.8 data gives the citizens one aura per entry; the 4.3.4 spawns show
-- that the seven citizens (43222) in the town hall belong to Zone 19 and the three 43247 in the basement to Zone 3; the
-- stand state and flags are those of the entries' template rows.

DELETE FROM `spell_area` WHERE `spell` IN (80695, 80817, 81202, 82100) AND `area` = 69;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(80695, 69, 0, 26520, 0, 0, 2, 1, 64, 11),
(80817, 69, 0, 26607, 0, 0, 2, 1, 64, 1),
(81202, 69, 26607, 26616, 0, 0, 2, 1, 74, 1),
(82100, 69, 0, 26616, 0, 0, 2, 1, 64, 1);

DELETE FROM `creature` WHERE `guid` BETWEEN 9310800 AND 9310803;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9310800, 43432, 0, 1, 0, 0, 0, 0, -9200.94, -2157.94, 57.2693, 2.14675, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310801, 43433, 0, 1, 0, 0, 0, 0, -9201.71, -2153.14, 57.2694, 4.66003, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310802, 43434, 0, 1, 0, 0, 0, 0, -9200.21, -2153.62, 57.269, 4.04916, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9310803, 43435, 0, 1, 0, 0, 0, 0, -9199.89, -2156.77, 57.2691, 2.74017, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_addon` WHERE `guid` IN (9310800, 9310801, 9310802, 9310803, 89639, 89650, 89651, 89694, 89695, 89696, 89698, 89703, 89706, 89754);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(9310800, 0, 0, 0, 1, 0, '81201'),
(9310801, 0, 0, 0, 1, 0, '81201'),
(9310802, 0, 0, 0, 1, 0, '81201'),
(9310803, 0, 0, 0, 1, 0, '81201'),
(89639, 0, 0, 0, 257, 0, '80815'),
(89650, 0, 0, 0, 257, 0, '80815'),
(89651, 0, 0, 0, 257, 0, '80815'),
(89694, 0, 0, 65536, 257, 0, '82099'),
(89695, 0, 0, 65536, 257, 0, '82099'),
(89696, 0, 0, 65536, 257, 0, '82099'),
(89698, 0, 0, 65536, 257, 0, '82099'),
(89703, 0, 0, 65536, 257, 0, '82099'),
(89706, 0, 0, 65536, 257, 0, '82099'),
(89754, 0, 0, 65536, 257, 0, '82099');
