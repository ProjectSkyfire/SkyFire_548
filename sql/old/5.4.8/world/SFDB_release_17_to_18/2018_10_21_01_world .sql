ALTER TABLE access_requirement
	ADD COLUMN iLvl MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER quest_failed_text;
	