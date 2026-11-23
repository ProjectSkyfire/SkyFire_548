-- Learn to Ride in Elwynn Forest (14079) had no quest ender, so the quest could not be turned in. Randal Hunter (4732,
-- the Elwynn riding trainer in Eastvale Logging Camp) ends it in Legends of Azeroth 5.4.8, like the other race riding
-- quests (14081 to 14089) have their trainer as ender.
DELETE FROM `creature_questender` WHERE `id` = 4732 AND `quest` = 14079;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(4732, 14079);
