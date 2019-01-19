-- Add Locale 9 = itIT, Locale 10 = ptBR, Locale 11= ptPT
ALTER TABLE `locales_points_of_interest`
	ADD COLUMN `icon_name_loc9` TEXT NULL AFTER `icon_name_loc8`,
	ADD COLUMN `icon_name_loc10` TEXT NULL AFTER `icon_name_loc9`,
	ADD COLUMN `icon_name_loc11` TEXT NULL AFTER `icon_name_loc10`;