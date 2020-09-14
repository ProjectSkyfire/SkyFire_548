ALTER TABLE `smart_scripts`
	CHANGE COLUMN `event_flags` `event_flags` SMALLINT UNSIGNED NOT NULL DEFAULT 0 AFTER `event_chance`;
