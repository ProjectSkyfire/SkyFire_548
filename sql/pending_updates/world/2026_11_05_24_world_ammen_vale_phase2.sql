-- Ammen Vale (Draenei starting zone): quest givers of the old class training quests (zone audit finding quests with no giver). No new spawns, so no new guids.

-- ---------------------------------------------------------------------------------------------------------------
-- the six class training quests "Paladin / Hunter / Warrior / Mage / Priest / Shaman Training" (9287, 9288, 9289, 9290,
-- 9291, 9421) had no giver in home, so nobody could take them. The class trainers that start the new class quests (26966,
-- 26963, 26958, 26968, 26970, 26969) are also their givers and turn-in NPCs in 4.3.4 (9290, 9291, 9421) and in 5.4.8 (all six);
-- home had the turn-ins of 9290, 9291 and 9421 only. Both references make each training quest the prerequisite of the new
-- class quest and offer the new one on turn-in (home already has the automatic offer), so the prerequisites are restored too;
-- without them both quests of a class would be offered at once.
-- Starters: Aurelon 16501 (Paladin), Keilnei 16499 (Hunter), Kore 16503 (Warrior), Valaatu 16500 (Mage), Zalduun 16502
-- (Priest), Firmanvaar 17089 (Shaman). Turn-ins added for the first three; 9290, 9291 and 9421 already have theirs.
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((16501, 9287), (16499, 9288), (16503, 9289), (16500, 9290), (16502, 9291), (17089, 9421));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(16501, 9287),
(16499, 9288),
(16503, 9289),
(16500, 9290),
(16502, 9291),
(17089, 9421);

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((16501, 9287), (16499, 9288), (16503, 9289));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(16501, 9287),
(16499, 9288),
(16503, 9289);

UPDATE `quest_template` SET `PrevQuestId` = 9289 WHERE `Id` = 26958; -- Your First Lesson after Warrior Training
UPDATE `quest_template` SET `PrevQuestId` = 9288 WHERE `Id` = 26963; -- Steadying Your Shot after Hunter Training
UPDATE `quest_template` SET `PrevQuestId` = 9287 WHERE `Id` = 26966; -- The Light's Power after Paladin Training
UPDATE `quest_template` SET `PrevQuestId` = 9290 WHERE `Id` = 26968; -- Frost Nova after Mage Training
UPDATE `quest_template` SET `PrevQuestId` = 9421 WHERE `Id` = 26969; -- Primal Strike after Shaman Training
UPDATE `quest_template` SET `PrevQuestId` = 9291 WHERE `Id` = 26970; -- Learning the Word after Priest Training
