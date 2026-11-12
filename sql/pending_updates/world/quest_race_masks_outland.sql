-- Quest race masks, the Outland map (Outland, Eversong Woods, Ghostlands, Azuremyst Isle, Bloodmyst Isle): quests that allow every race but are limited to one faction in the 5.4.8 data.
-- Alliance is 18875469 and Horde is 33555378. Only rows still at 0 are changed.
-- Based on Legends of Azeroth 5.4.8 (AllowableRaces).

-- Azuremyst Isle
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9278, 9306, 9307, 9308, 9409, 9507, 9546, 10304, 14082
);

-- Blade's Edge Mountains
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    10716, 11022
);

-- Bloodmyst Isle
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9568, 9695, 9749, 28559
);

-- Eversong Woods
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9357, 14081
);

-- Ghostlands
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    28560
);

-- Hellfire Peninsula
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9382, 9384, 10558, 28708
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9344, 9377, 10046, 10053, 10059, 10060, 10156
);

-- Nagrand
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9956, 10076, 10077
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9867, 10074, 10075
);

-- Netherstorm
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    10187
);

-- Shadowmoon Valley
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    10871, 10872, 11081, 11082
);

-- Shattrath City
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    10020, 10021, 11451, 11534
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    11517
);

-- Terokkar Forest
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9930, 9941, 9943, 9949, 9952, 9958, 9980, 10014, 10048
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9942, 9950, 9959, 9981, 10015, 10049
);

-- Zangarmarsh
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9776, 9777, 9781, 9782, 9783, 9786, 9787, 9790, 9793, 9801, 9803, 9830, 9833, 9896, 9902, 9905, 10104, 10115,
    10355
);
UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `RequiredRaces` = 0 AND `Id` IN (
    9769, 9770, 9771, 9772, 9774, 9775, 9814, 9816, 9820, 9822, 9823, 9841, 9842, 9846, 9847, 9898, 9899, 10105, 10118
);
