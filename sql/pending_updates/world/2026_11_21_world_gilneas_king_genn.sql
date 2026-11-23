-- Gilneas: King Genn Greymane gives and ends Last Chance at Humanity but had no spawn; the hand-off quests follow the last
-- Gilneas quest. Based on Legends of Azeroth 5.4.8.

-- King Genn Greymane 36332 gives and ends Last Chance at Humanity but had no spawn (position and phases as 5.4.8:
--    default world and 181; group 399).
DELETE FROM `creature` WHERE `guid` = 9390000;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9390000, 36332, 654, 1, 0, 399, 0, 0, -1820.89, 2292.93, 42.1128, 0.334925, 300, 0, 0, 0, 0, 0, 0, 0, 0);

-- Prerequisites: Last Chance at Humanity needs the Last Stand, and the hand-off to Darnassus follows the last
--    Gilneas quest (24681 > Endgame > Rut'theran Village).
UPDATE `quest_template` SET `PrevQuestId` = 14222 WHERE `Id` = 14375;
UPDATE `quest_template` SET `PrevQuestId` = 24681 WHERE `Id` = 26706;
UPDATE `quest_template` SET `PrevQuestId` = 26706 WHERE `Id` = 14434;

-- A Rampaging Worgen is not a quest giver (npcflag 0; neither reference has the row).
DELETE FROM `creature_queststarter` WHERE `id` = 34884 AND `quest` = 14093;
