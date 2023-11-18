DELETE FROM `quest_poi` WHERE `questId`=29522;
INSERT INTO `quest_poi` (`questId`, `id`, `objIndex`, `mapid`, `WorldMapAreaId`, `FloorId`, `unk3`, `unk4`) VALUES
(29522, 0, -1, 860, 808, 0, 0, 1);

DELETE FROM `quest_poi_points` WHERE `questId`=29522;
INSERT INTO `quest_poi_points` (`questId`, `id`, `idx`, `x`, `y`) VALUES
(29522, 0, 0, 1407, 3637);
