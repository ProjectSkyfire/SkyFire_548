-- Add Locale 9 = itIT, Locale 10 = ptBR, Locale 11= ptPT
ALTER TABLE `locales_page_text`
	ADD COLUMN `Text_loc9` LONGTEXT NULL AFTER `Text_loc8`,
	ADD COLUMN `Text_loc10` LONGTEXT NULL AFTER `Text_loc9`,
	ADD COLUMN `Text_loc11` LONGTEXT NULL AFTER `Text_loc10`;