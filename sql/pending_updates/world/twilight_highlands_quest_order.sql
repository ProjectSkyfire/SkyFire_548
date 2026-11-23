-- Twilight Highlands: the previous quests. In the 5.4.8 data 35 quests of the zone have a previous quest that the world lacks, so the
-- quests that follow each other are all offered at once: the Highbank chain ("Patchwork Command" 27576 and its followers), the
-- Southern Flank chain (27607, 27610, 27611, 27622, 28584, 28586), the Wildhammer chains of the Alliance (28860 to 28864), the
-- Crucible of Carnage quests, "Crushblow" 28249, "Ready the Air Force" 26358 after "Ready the Ground Troops" and others. One link
-- fixes a defect: "The Maw of Madness" (27374, Alliance) required the Horde quest 27945, so no Alliance character could take it; the
-- 5.4.8 data has no previous quest. Quests where the world has another previous quest than the 5.4.8 data and the 4.3.4 data agrees
-- with the world are not touched. Absolute values, safe to run twice. Based on Legends of Azeroth 5.4.8.

UPDATE `quest_template` SET `PrevQuestId` = 26374 WHERE `Id` = 26358; -- Ready the Air Force
UPDATE `quest_template` SET `PrevQuestId` = 26608 WHERE `Id` = 26619; -- You Say You Want a Revolution
UPDATE `quest_template` SET `PrevQuestId` = 0 WHERE `Id` = 27374; -- The Maw of Madness
UPDATE `quest_template` SET `PrevQuestId` = 27380 WHERE `Id` = 27486; -- Warm Welcome
UPDATE `quest_template` SET `PrevQuestId` = 27564 WHERE `Id` = 27507; -- Encroaching Twilight
UPDATE `quest_template` SET `PrevQuestId` = 27564 WHERE `Id` = 27508; -- Far from the Nest
UPDATE `quest_template` SET `PrevQuestId` = 27564 WHERE `Id` = 27509; -- Breach in the Defenses
UPDATE `quest_template` SET `PrevQuestId` = 27509 WHERE `Id` = 27576; -- Patchwork Command
UPDATE `quest_template` SET `PrevQuestId` = 26840 WHERE `Id` = 27607; -- The Southern Flank
UPDATE `quest_template` SET `PrevQuestId` = 27607 WHERE `Id` = 27610; -- Scouting the Shore
UPDATE `quest_template` SET `PrevQuestId` = 27607 WHERE `Id` = 27611; -- Blood on the Sand
UPDATE `quest_template` SET `PrevQuestId` = 27610 WHERE `Id` = 27622; -- Mo' Better Shredder
UPDATE `quest_template` SET `PrevQuestId` = 27638 WHERE `Id` = 27653; -- Dark Assassins
UPDATE `quest_template` SET `PrevQuestId` = 27638 WHERE `Id` = 27655; -- Bring the Hammer Down
UPDATE `quest_template` SET `PrevQuestId` = 27638 WHERE `Id` = 27658; -- Help from the Earthcaller
UPDATE `quest_template` SET `PrevQuestId` = 27863 WHERE `Id` = 27864; -- The Crucible of Carnage: The Deadly Dragonmaw!
UPDATE `quest_template` SET `PrevQuestId` = 27863 WHERE `Id` = 27865; -- The Crucible of Carnage: The Wayward Wildhammer!
UPDATE `quest_template` SET `PrevQuestId` = 27576 WHERE `Id` = 28090; -- Precious Goods
UPDATE `quest_template` SET `PrevQuestId` = 27576 WHERE `Id` = 28091; -- Easy Pickings
UPDATE `quest_template` SET `PrevQuestId` = 28171 WHERE `Id` = 28173; -- Blackout
UPDATE `quest_template` SET `PrevQuestId` = 28191 WHERE `Id` = 28175; -- Shining Through the Dark
UPDATE `quest_template` SET `PrevQuestId` = 28247 WHERE `Id` = 28249; -- Crushblow
UPDATE `quest_template` SET `PrevQuestId` = 27610 WHERE `Id` = 28584; -- Quality Construction
UPDATE `quest_template` SET `PrevQuestId` = 27611 WHERE `Id` = 28586; -- Pool Pony Rescue
UPDATE `quest_template` SET `PrevQuestId` = 28589 WHERE `Id` = 28590; -- Reprisal
UPDATE `quest_template` SET `PrevQuestId` = 28591 WHERE `Id` = 28593; -- Of Utmost Importance
UPDATE `quest_template` SET `PrevQuestId` = 28591 WHERE `Id` = 28594; -- Highbank, Crybank
UPDATE `quest_template` SET `PrevQuestId` = 28712 WHERE `Id` = 28758; -- Battle of Life and Death
UPDATE `quest_template` SET `PrevQuestId` = 27754 WHERE `Id` = 28860; -- Keeping the Dragonmaw at Bay
UPDATE `quest_template` SET `PrevQuestId` = 27752 WHERE `Id` = 28861; -- Fight Like a Wildhammer
UPDATE `quest_template` SET `PrevQuestId` = 27753 WHERE `Id` = 28862; -- Never Leave a Dinner Behind
UPDATE `quest_template` SET `PrevQuestId` = 28409 WHERE `Id` = 28864; -- Beer Run
UPDATE `quest_template` SET `PrevQuestId` = 27751 WHERE `Id` = 28871; -- Crushing the Wildhammer
UPDATE `quest_template` SET `PrevQuestId` = 27747 WHERE `Id` = 28872; -- Total War
UPDATE `quest_template` SET `PrevQuestId` = 27750 WHERE `Id` = 28873; -- Another Maw to Feed
UPDATE `quest_template` SET `PrevQuestId` = 27929 WHERE `Id` = 28874; -- Hook 'em High
