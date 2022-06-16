ALTER TABLE `character_currency`
	CHANGE COLUMN `total_count` `quantity` INT(10) UNSIGNED NOT NULL AFTER `currency`,
	CHANGE COLUMN `week_count` `weekly_quantity` INT(10) UNSIGNED NOT NULL AFTER `quantity`,
	CHANGE COLUMN `season_count` `tracked_quantity` INT(10) UNSIGNED NOT NULL AFTER `weekly_quantity`;
