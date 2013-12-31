-- Add Locale 9 = itIT, Locale 10 = ptBR, Locale 11= ptPT
ALTER TABLE `locales_achievement_reward`
	ADD COLUMN `subject_loc9` VARCHAR(100) NOT NULL DEFAULT '' AFTER `subject_loc8`,
	ADD COLUMN `subject_loc10` VARCHAR(100) NOT NULL DEFAULT '' AFTER `subject_loc9`,
	ADD COLUMN `subject_loc11` VARCHAR(100) NOT NULL DEFAULT '' AFTER `subject_loc10`,
	ADD COLUMN `text_loc9` TEXT NULL AFTER `text_loc8`,
	ADD COLUMN `text_loc10` TEXT NULL AFTER `text_loc9`,
	ADD COLUMN `text_loc11` TEXT NULL AFTER `text_loc10`;