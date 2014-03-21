-- Add Locale 9 = itIT, Locale 10 = ptBR, Locale 11= ptPT
ALTER TABLE `locales_creature`
	ADD COLUMN `name_loc9` VARCHAR(100) NOT NULL DEFAULT '' AFTER `name_loc8`,
	ADD COLUMN `name_loc10` VARCHAR(100) NOT NULL DEFAULT '' AFTER `name_loc9`,
	ADD COLUMN `name_loc11` VARCHAR(100) NOT NULL DEFAULT '' AFTER `name_loc10`,
	ADD COLUMN `subname_loc9` VARCHAR(100) NULL DEFAULT NULL AFTER `subname_loc8`,
	ADD COLUMN `subname_loc10` VARCHAR(100) NULL DEFAULT NULL AFTER `subname_loc9`,
	ADD COLUMN `subname_loc11` VARCHAR(100) NULL DEFAULT NULL AFTER `subname_loc10`;