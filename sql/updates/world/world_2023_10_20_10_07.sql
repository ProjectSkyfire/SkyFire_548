ALTER TABLE `achievement_reward`
	CHANGE COLUMN `spell` `spell` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `item`;
