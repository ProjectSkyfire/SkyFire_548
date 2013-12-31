-- Add Locale 9 = itIT, Locale 10 = ptBR, Locale 11= ptPT
ALTER TABLE `locales_gossip_menu_option`
	ADD COLUMN `option_text_loc9` TEXT NULL AFTER `option_text_loc8`,
	ADD COLUMN `option_text_loc10` TEXT NULL AFTER `option_text_loc9`,
	ADD COLUMN `option_text_loc11` TEXT NULL AFTER `option_text_loc10`,
	ADD COLUMN `box_text_loc9` TEXT NULL AFTER `box_text_loc8`,
	ADD COLUMN `box_text_loc10` TEXT NULL AFTER `box_text_loc9`,
	ADD COLUMN `box_text_loc11` TEXT NULL AFTER `box_text_loc10`;