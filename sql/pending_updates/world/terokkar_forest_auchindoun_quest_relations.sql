-- Terokkar Forest: the missing turn-ins and starter of the Auchindoun item quests and Tokens of the Descendants,
-- and the Auchenai Coffin target Reanimated Exarch.

-- Host of the Hidden City (Auchindoun item quests): the enders were missing; Tokens of the Descendants: the starter
DELETE FROM `creature_queststarter` WHERE (`id` = 23306 AND `quest` = 11074);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(23306, 11074);
DELETE FROM `creature_questender` WHERE (`id` = 18252 AND `quest` = 9984) OR (`id` = 18383 AND `quest` = 9985);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(18252, 9984),
(18383, 9985);

-- Reanimated Exarch (The Fallen Exarch), who stands at the Auchenai Coffin
DELETE FROM `creature` WHERE `guid` IN (8317006);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8317006, 22452, 530, 1, 0, 0, 0, 0, -3360.92000, 5158.46000, -8.99613, 1.291540, 60, 0, 0, 0, 0, 0, 0, 0, 0);
