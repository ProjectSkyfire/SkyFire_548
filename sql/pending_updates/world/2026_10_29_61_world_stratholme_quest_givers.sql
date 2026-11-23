-- Stratholme: Cataclysm quest givers and the Eyes of Naxxramas with their patrols. Quest givers at their 4.3.4 positions (the camps
-- at both entrances); Eyes and their paths from Legends of Azeroth 5.4.8 for the MoP layout. Safe to run twice.

DELETE FROM `creature_addon` WHERE `guid` BETWEEN 8304110 AND 8304129;
DELETE FROM `creature` WHERE `guid` BETWEEN 8304110 AND 8304129;
-- creature rows removed here: the upstream data already has these spawns.

-- creature_addon rows removed here: the upstream data already has these spawns.


DELETE FROM `waypoint_data` WHERE `id` IN (83041200, 83041210, 83041220);
-- waypoint_data rows removed here: the upstream data already has these spawns.
