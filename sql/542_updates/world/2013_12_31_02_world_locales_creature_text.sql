-- Add Locale 9 = itIT, Locale 10 = ptBR, Locale 11= ptPT
ALTER TABLE `locales_creature_text`
	ADD COLUMN `text_loc9` TEXT NULL AFTER `text_loc8`,
	ADD COLUMN `text_loc10` TEXT NULL AFTER `text_loc9`,
	ADD COLUMN `text_loc11` TEXT NULL AFTER `text_loc10`;