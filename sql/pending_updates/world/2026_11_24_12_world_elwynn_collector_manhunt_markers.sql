-- The Collector (123) and Manhunt (147) are turned in to Marshal McCree at the Eastvale Logging Camp, but the map markers of
-- the turn-in pointed to Marshal Dughan's spot in Goldshire (upstream issue 1473). The two points are moved to McCree's
-- position, as in Legends of Azeroth 5.4.8.

UPDATE `quest_poi_points` SET `x` = -9468, `y` = -1306 WHERE `questId` = 123 AND `id` = 0 AND `idx` = 0;
UPDATE `quest_poi_points` SET `x` = -9475, `y` = -1249 WHERE `questId` = 147 AND `id` = 1 AND `idx` = 0;
