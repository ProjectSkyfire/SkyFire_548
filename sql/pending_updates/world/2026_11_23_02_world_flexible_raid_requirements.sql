-- Flexible Siege of Orgrimmar has no minimum item level. Preserve Raid Finder requirements.
UPDATE `access_requirement` SET `iLvl` = 0 WHERE `mapId` = 1136 AND `difficulty` = 14;
UPDATE `lfg_dungeon_template` SET `requiredItemLevel` = 0
WHERE `dungeonId` IN (726, 728, 729, 730, 766, 771, 772, 773, 774);
