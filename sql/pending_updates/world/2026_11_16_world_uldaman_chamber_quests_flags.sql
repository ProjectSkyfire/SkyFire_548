-- The Chamber of Khaz'mul and Archaedas, The Ancient Stone Watcher carried the auto-complete flag (0x10000) from the 4.3.4
-- data, so High Examiner Tae'thelan Bloodwatcher opened a turn-in window instead of offering them. The 5.4.8 flags are 8 and 786432.
-- Based on Legends of Azeroth 5.4.8.
UPDATE `quest_template` SET `Flags` = 8 WHERE `Id` = 27679;
UPDATE `quest_template` SET `Flags` = 786432 WHERE `Id` = 27680;
