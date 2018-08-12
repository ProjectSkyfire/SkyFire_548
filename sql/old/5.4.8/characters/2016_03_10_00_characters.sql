ALTER TABLE `characters`
	ADD COLUMN `lfgbonusfaction` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `watchedFaction`;
