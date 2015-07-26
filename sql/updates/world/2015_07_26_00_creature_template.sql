ALTER TABLE `creature_template`
	CHANGE COLUMN `family` `family` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `dynamicflags`;
