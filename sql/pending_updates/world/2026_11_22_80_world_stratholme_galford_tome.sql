-- The Tome of Knowledge (13385) drops from Instructor Galford, not from Hearthsinger Forresten. The dungeon journal
-- of the 5.4.8 client lists it under Galford, and TrinityCore 3.3.5, AzerothCore and the Cataclysm Preservation
-- Project 4.3.4 all have it in Galford's table at 12 percent and not in Forresten's. The table here copied the
-- Legends of Azeroth rows, which put it in Forresten's group with no chance.
DELETE FROM `creature_loot_template` WHERE `entry` IN (10558, 10811) AND `item` = 13385;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(10811, 13385, 12, 1, 1, 1, 1);
