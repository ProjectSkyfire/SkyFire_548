-- Fix Dungeon Finder teleports whose lfg_entrances rows still use classic Scarlet Monastery (map 189) coordinates.
-- Dungeon ids 163, 164, and 285 now point at Scarlet Halls (1001) and Scarlet Monastery (1004).
-- Other lfg_entrances rows (Maraudon, Dire Maul, Stratholme, Ahune, Coren, Crown Chemical) already match their maps.

DELETE FROM `lfg_entrances` WHERE `dungeonId` IN (163, 164, 285);
INSERT INTO `lfg_entrances` (`dungeonId`, `name`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(163, 'Scarlet Halls', 820.743, 607.812, 13.6389, 0),
(164, 'Scarlet Monastery', 1124.64, 512.467, 0.989549, 1.5708),
(285, 'The Headless Horseman', 1124.64, 512.467, 0.989549, 1.5708);

DELETE FROM `lfg_dungeon_template` WHERE `dungeonId` IN (163, 164, 285);
INSERT INTO `lfg_dungeon_template` (`dungeonId`, `position_x`, `position_y`, `position_z`, `orientation`, `requiredItemLevel`) VALUES
(163, 820.743, 607.812, 13.6389, 0, 0),
(164, 1124.64, 512.467, 0.989549, 1.5708, 0),
(285, 1124.64, 512.467, 0.989549, 1.5708, 0);
