ALTER TABLE `achievement_reward`
	ADD COLUMN `spell` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `item`;
