-- Add Locale 9 = itIT, Locale 10 = ptBR, Locale 11= ptPT
ALTER TABLE `trinity_string`
	ADD COLUMN `content_loc9` TEXT NULL AFTER `content_loc8`,
	ADD COLUMN `content_loc10` TEXT NULL AFTER `content_loc9`,
	ADD COLUMN `content_loc11` TEXT NULL AFTER `content_loc10`;
