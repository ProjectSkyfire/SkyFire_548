-- Deepholm: previous-quest links of the 5.4.8 data for quests that are all offered at once today (for example the
-- gunship quests, "Shaken and Stirred", "Hard Falls" and "Fragile Values", the Stonecore entry quests). Only links where the
-- previous quest is open to the same factions, has a giver and an ender that a player sees without a stage phase and does not
-- close a loop. The links that depend on a quest of a stage phase, that differ from the 4.3.4 order or that belong to holiday
-- and scenario quests are not in this file. "The Maelstrom" (27203) required the Horde quest 27722, so no Alliance
-- player could take it; the 5.4.8 data has no previous quest.
-- Based on Legends of Azeroth 5.4.8.

UPDATE `quest_template` SET `PrevQuestId` = 26247 WHERE `Id` = 26248; -- All Our Friends Are Dead
UPDATE `quest_template` SET `PrevQuestId` = 26245 WHERE `Id` = 26249; -- The Admiral's Cabin
UPDATE `quest_template` SET `PrevQuestId` = 26248 WHERE `Id` = 26250; -- On Second Thought, Take One Prisoner
UPDATE `quest_template` SET `PrevQuestId` = 26409 WHERE `Id` = 26410; -- Explosive Bonding Compound
UPDATE `quest_template` SET `PrevQuestId` = 27935 WHERE `Id` = 26500; -- We're Surrounded
UPDATE `quest_template` SET `PrevQuestId` = 26575 WHERE `Id` = 26576; -- Steady Hand
UPDATE `quest_template` SET `PrevQuestId` = 26507 WHERE `Id` = 26580; -- Familiar Intruders
UPDATE `quest_template` SET `PrevQuestId` = 26582 WHERE `Id` = 26584; -- Shaken and Stirred
UPDATE `quest_template` SET `PrevQuestId` = 26501 WHERE `Id` = 26591; -- Battlefront Triage
UPDATE `quest_template` SET `PrevQuestId` = 26656 WHERE `Id` = 26657; -- Hard Falls
UPDATE `quest_template` SET `PrevQuestId` = 26656 WHERE `Id` = 26658; -- Fragile Values
UPDATE `quest_template` SET `PrevQuestId` = 27061 WHERE `Id` = 26766; -- Big Game, Big Bait
UPDATE `quest_template` SET `PrevQuestId` = 27061 WHERE `Id` = 26768; -- To Catch a Dragon
UPDATE `quest_template` SET `PrevQuestId` = 26834 WHERE `Id` = 26791; -- Sprout No More
UPDATE `quest_template` SET `PrevQuestId` = 26834 WHERE `Id` = 26792; -- Fungal Monstrosities
UPDATE `quest_template` SET `PrevQuestId` = 26625 WHERE `Id` = 27126; -- Rush Delivery
UPDATE `quest_template` SET `PrevQuestId` = 26409 WHERE `Id` = 27135; -- Something that Burns
UPDATE `quest_template` SET `PrevQuestId` = 0 WHERE `Id` = 27203; -- The Maelstrom
UPDATE `quest_template` SET `PrevQuestId` = 27931 WHERE `Id` = 27932; -- The Axe of Earthly Sundering
UPDATE `quest_template` SET `PrevQuestId` = 27931 WHERE `Id` = 27933; -- Elemental Ore
UPDATE `quest_template` SET `PrevQuestId` = 26871 WHERE `Id` = 28824; -- Wayward Child
UPDATE `quest_template` SET `PrevQuestId` = 27061 WHERE `Id` = 28866; -- Into the Stonecore
UPDATE `quest_template` SET `PrevQuestId` = 26439 WHERE `Id` = 28869; -- Pebble
