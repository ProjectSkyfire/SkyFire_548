-- Add Locale 9 = itIT, Locale 10 = ptBR, Locale 11= ptPT
ALTER TABLE `locales_item`
	ADD COLUMN `name_loc9` VARCHAR(100) NOT NULL DEFAULT '' AFTER `name_loc8`,
	ADD COLUMN `name_loc10` VARCHAR(100) NOT NULL DEFAULT '' AFTER `name_loc9`,
	ADD COLUMN `name_loc11` VARCHAR(100) NOT NULL DEFAULT '' AFTER `name_loc10`,
	ADD COLUMN `description_loc9` VARCHAR(255) NULL DEFAULT NULL AFTER `description_loc8`,
	ADD COLUMN `description_loc10` VARCHAR(255) NULL DEFAULT NULL AFTER `description_loc9`,
	ADD COLUMN `description_loc11` VARCHAR(255) NULL DEFAULT NULL AFTER `description_loc10`;