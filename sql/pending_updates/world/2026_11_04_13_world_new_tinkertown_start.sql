-- New Tinkertown (Gnome starting zone): data fixes from the Cataclysm Preservation Project 4.3.4 data, with Legends of
-- Azeroth 5.4.8 for the Monk quests (the zone audit findings NT-1 to NT-4). New gameobject spawn guid: 922022 (above the
-- previous maximum 922021). No new creature spawns.

-- ---------------------------------------------------------------------------------------------------------------
-- NT-1: wrong levels on the starting quests. The five "The Future of Gnomeregan" quests were 2 / 2; both references have
-- 1 / 1. The five class "Arts of a ..." quests (Mage 26198, Priest 26200, Warlock 26201, Warrior 26204, Rogue 26207) were
-- 2 / 2; both references have 3 / 3. The Monk quests have no 4.3.4 row, so LoA decides: 31135 is 1 / 1 and 31138 is
-- Level 3 / MinLevel 1.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `Level` = 1, `MinLevel` = 1 WHERE `Id` IN (26197, 26199, 26202, 26203, 26206, 31135);
UPDATE `quest_template` SET `Level` = 3, `MinLevel` = 3 WHERE `Id` IN (26198, 26200, 26201, 26204, 26207);
UPDATE `quest_template` SET `Level` = 3, `MinLevel` = 1 WHERE `Id` = 31138;

-- ---------------------------------------------------------------------------------------------------------------
-- NT-2: the Detonator (object 204042), the objective of "Finishin' the Job" (26318), had no spawn. The row is 4.3.4's (LoA
-- has the same place, 1 yard apart). Phase 0 (phaseId 0, phaseGroup 0): all 259 objects and 704 creatures around the
-- zone are in phase 0 here, and 4.3.4's phase 169 is the default phase.
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `gameobject` WHERE `guid` = 922022;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(922022, 204042, 0, 1, 0, 0, -5523.21, 708.689, 376.925, 6.23083, 0, 0, -0.0261765, 0.999657, 300, 255, 1);

-- ---------------------------------------------------------------------------------------------------------------
-- NT-3: the Monk "Meet the High Tinker" (31137) had no class restriction; LoA has 512 (Monk).
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `RequiredClasses` = 512 WHERE `Id` = 31137;

-- ---------------------------------------------------------------------------------------------------------------
-- NT-4: High Tinker Mekkatorque (42317) was a level 3 template; both references have level 88.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `minlevel` = 88, `maxlevel` = 88 WHERE `entry` = 42317; -- High Tinker Mekkatorque
