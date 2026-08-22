ALTER TABLE `quest_template`
	CHANGE COLUMN `RequiredRaces` `RequiredRaces` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `RequiredClasses`;
