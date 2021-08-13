-- Fix SQL(p): REPLACE INTO item_instance (itemEntry, owner_guid, creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, reforgeID, transmogrifyId, durability, playedTime, text, guid) VALUES (35, 3, 0, 0, 1, 0, '0 0 0 0 0 ', 0, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ', 0, 4261216296, 0, 40, 69, '#
-- [ERROR]: [1264] Out of range value for column 'reforgeID' at row 1
ALTER TABLE `item_instance`
	CHANGE COLUMN `reforgeID` `reforgeID` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `randomPropertyId`;
