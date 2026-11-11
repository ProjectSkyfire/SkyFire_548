-- Quest race masks, Northrend: quests that allow every race but are limited to one faction in the 5.4.8 data.
-- Alliance is 18875469 and Horde is 33555378. Only rows still at 0 are changed.
-- Based on Legends of Azeroth 5.4.8 (AllowableRaces).

-- Borean Tundra
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    28709, 29307
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    28711, 29308
);

-- Dalaran
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    14203
);

-- Wintergrasp
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    236, 13538
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    13539
);
