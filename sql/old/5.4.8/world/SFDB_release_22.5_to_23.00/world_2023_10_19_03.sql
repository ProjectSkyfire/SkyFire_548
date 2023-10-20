-- Fixed required race for rogue version of 'Fear no Evil' where human race mask was missing.
UPDATE `quest_template` SET `RequiredRaces`=2097153 WHERE `Id`=28811;
