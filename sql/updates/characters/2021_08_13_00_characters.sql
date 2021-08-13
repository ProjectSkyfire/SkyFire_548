-- Fix SQL(p): REPLACE INTO item_instance (itemEntry, owner_guid, creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, reforgeID, transmogrifyId, durability, playedTime, text, guid) VALUES (35, 3, 0, 0, 1, 0, '0 0 0 0 0 ', 0, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ', 0, 4261216296, 0, 40, 69, '#
 [ERROR]: [1264] Out of range value for column 'reforgeID' at row 1
ALTER TABLE `item_instance`
	CHANGE COLUMN `guid` `guid` INT(11) UNSIGNED NOT NULL DEFAULT '0' FIRST,
	CHANGE COLUMN `owner_guid` `owner_guid` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `itemEntry`,
	CHANGE COLUMN `creatorGuid` `creatorGuid` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `owner_guid`,
	CHANGE COLUMN `giftCreatorGuid` `giftCreatorGuid` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `creatorGuid`,
	CHANGE COLUMN `count` `count` INT(11) UNSIGNED NOT NULL DEFAULT '1' AFTER `giftCreatorGuid`,
	CHANGE COLUMN `duration` `duration` INT(11) NOT NULL DEFAULT '0' AFTER `count`,
	CHANGE COLUMN `reforgeID` `reforgeID` INT(11) NOT NULL DEFAULT '0' AFTER `randomPropertyId`,
	CHANGE COLUMN `transmogrifyId` `transmogrifyId` INT(11) NOT NULL DEFAULT '0' AFTER `reforgeID`,
	CHANGE COLUMN `playedTime` `playedTime` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `durability`;
