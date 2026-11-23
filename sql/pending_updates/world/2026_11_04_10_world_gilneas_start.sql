-- Gilneas (Worgen starting zone): data fixes from the Legends of Azeroth 5.4.8 data (the zone audit findings GL-2, GL-3,
-- GL-4 and GL-10). New creature spawn guids: 8402223 to 8402238 (above the Kezan file, which ends on 8402222; the previous
-- maximum in the world table was 8402098).

-- ---------------------------------------------------------------------------------------------------------------
-- GL-2: twenty prerequisites that LoA and 4.3.4 both have and the base data lacked, so the quests could be taken out of order.
-- "Something's Amiss" (14091) comes before 14093, 14094 and 14098; "Royal Orders" (14099) before the seven class intro
-- quests; then the chains 14293 -> 14294 -> 14212, 14320 -> 14321, 14336 -> 14347 and 14348, 14367 -> 14368, 14369 and
-- 14382, 14386 -> 14396, 26129 -> 14159. The successors all had PrevQuestId 0.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `PrevQuestId` = 14091 WHERE `Id` IN (14093, 14094, 14098);
UPDATE `quest_template` SET `PrevQuestId` = 14099 WHERE `Id` IN (14265, 14269, 14273, 14275, 14277, 14278, 14280);
UPDATE `quest_template` SET `PrevQuestId` = 14293 WHERE `Id` = 14294;
UPDATE `quest_template` SET `PrevQuestId` = 14294 WHERE `Id` = 14212;
UPDATE `quest_template` SET `PrevQuestId` = 14320 WHERE `Id` = 14321;
UPDATE `quest_template` SET `PrevQuestId` = 14336 WHERE `Id` IN (14347, 14348);
UPDATE `quest_template` SET `PrevQuestId` = 14367 WHERE `Id` IN (14368, 14369, 14382);
UPDATE `quest_template` SET `PrevQuestId` = 14386 WHERE `Id` = 14396;
UPDATE `quest_template` SET `PrevQuestId` = 26129 WHERE `Id` = 14159;

-- ---------------------------------------------------------------------------------------------------------------
-- GL-3: NextQuestIdChain (the quest offered automatically on turn-in) of the class intro and class training quests set
-- to the value LoA and 4.3.4 agree on, so the intro quest offers the class quest (for example 14265 offers 14266, not
-- 14286) and the class quest offers the follow-up.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `NextQuestIdChain` = 14266 WHERE `Id` = 14265;
UPDATE `quest_template` SET `NextQuestIdChain` = 14272 WHERE `Id` = 14269;
UPDATE `quest_template` SET `NextQuestIdChain` = 14285 WHERE `Id` = 14272;
UPDATE `quest_template` SET `NextQuestIdChain` = 14274 WHERE `Id` = 14273;
UPDATE `quest_template` SET `NextQuestIdChain` = 14276 WHERE `Id` = 14275;
UPDATE `quest_template` SET `NextQuestIdChain` = 14290 WHERE `Id` = 14276;
UPDATE `quest_template` SET `NextQuestIdChain` = 14281 WHERE `Id` = 14277;
UPDATE `quest_template` SET `NextQuestIdChain` = 14279 WHERE `Id` = 14278;
UPDATE `quest_template` SET `NextQuestIdChain` = 14283 WHERE `Id` = 14280;
UPDATE `quest_template` SET `NextQuestIdChain` = 14291 WHERE `Id` = 14283;

-- ---------------------------------------------------------------------------------------------------------------
-- GL-4: kill targets with no spawn here. Swamp Crocolisk (37078, 15 spawns, x5 for "Stranded at the Marsh" 24468) and
-- Dark Scout (37953, 1 spawn, x1 for "Losing Your Tail" 24616) are LoA spawns (positions, respawn time, wander radius).
-- Phase 0 (phaseId 0, phaseGroup 0); LoA uses phase mask 131072 for both. The model is left to the template. The Dark
-- Scout template here is still a placeholder (level 1, faction 35); see the notes, it is not changed here.
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `creature` WHERE `guid` BETWEEN 8402223 AND 8402238;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8402223, 37078, 654, 1, 0, 0, 0, 0, -1961.23, 1852.48, 7.29027, 0.471239, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402224, 37078, 654, 1, 0, 0, 0, 0, -2035.11, 1822.58, 6.39407, 5.55015, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402225, 37078, 654, 1, 0, 0, 0, 0, -2090.77, 1769.82, 5.60029, 3.12414, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402226, 37078, 654, 1, 0, 0, 0, 0, -2084.58, 1742.1, 5.75564, 4.67748, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402227, 37078, 654, 1, 0, 0, 0, 0, -2062.98, 1811.81, 6.76907, 4.67748, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402228, 37078, 654, 1, 0, 0, 0, 0, -2078.92, 1802.41, 5.99517, 4.67748, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402229, 37078, 654, 1, 0, 0, 0, 0, -2041.48, 1753.8, 7.13064, 4.36332, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402230, 37078, 654, 1, 0, 0, 0, 0, -1974.44, 1754.43, 6.22959, 4.67748, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402231, 37078, 654, 1, 0, 0, 0, 0, -2045.41, 1781.03, 5.95963, 4.67748, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402232, 37078, 654, 1, 0, 0, 0, 0, -1994.76, 1820.89, 6.14443, 6.24828, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402233, 37078, 654, 1, 0, 0, 0, 0, -1972.23, 1796.59, 6.9872, 0.174533, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402234, 37078, 654, 1, 0, 0, 0, 0, -2078.83, 1805.24, 6.24517, 4.67748, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402235, 37078, 654, 1, 0, 0, 0, 0, -2087.94, 1769.77, 5.60029, 3.12414, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402236, 37078, 654, 1, 0, 0, 0, 0, -2062.89, 1814.64, 6.64407, 4.67748, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402237, 37078, 654, 1, 0, 0, 0, 0, -2037.21, 1824.47, 6.64407, 5.55015, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8402238, 37953, 654, 1, 0, 0, 0, 0, -2226.2, 1444.82, -25.4718, 4.31105, 300, 3, 0, 0, 0, 1, 0, 0, 0);

-- ---------------------------------------------------------------------------------------------------------------
-- GL-10: Forsaken Invader (36809) had maxlevel 5; both references have 5 to 6. Toad (1420) and Deer (883) are already
-- level 1 from the Shadowglen and Northshire files, so they are not repeated.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `maxlevel` = 6 WHERE `entry` = 36809; -- Forsaken Invader

-- GL-4 (template): the Dark Scout (37953), the target of "Losing Your Tail" (24616), was a friendly level 1
-- placeholder that could not be attacked; Legends of Azeroth has it as a level 9 hostile mob.
UPDATE `creature_template` SET `minlevel` = 9, `maxlevel` = 9, `exp` = 0, `faction_A` = 118, `faction_H` = 118, `npcflag` = 0, `unit_class` = 8, `unit_flags` = 0, `unit_flags2` = 2048, `Health_mod` = 1, `baseattacktime` = 2000 WHERE `entry` = 37953;
