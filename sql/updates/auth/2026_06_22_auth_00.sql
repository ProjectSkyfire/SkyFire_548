CREATE TABLE IF NOT EXISTS `gm_notes` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `realm_id` int NOT NULL DEFAULT '0',
  `account_id` int unsigned NOT NULL,
  `player_id` int unsigned NOT NULL,
  `note_date` date NOT NULL,
  `note_time` time NOT NULL,
  `map_id` smallint unsigned NOT NULL DEFAULT '0',
  `zone_id` smallint unsigned NOT NULL DEFAULT '0',
  `area_id` smallint unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `location` varchar(128) NOT NULL DEFAULT '',
  `note` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_gm_notes_realm_date` (`realm_id`,`note_date`),
  KEY `idx_gm_notes_account_id` (`account_id`),
  KEY `idx_gm_notes_player_id` (`player_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='In-game GM notes';

DELETE FROM `rbac_permissions` WHERE `id`=808;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (808, 'Command: gm note');

DELETE FROM `rbac_linked_permissions` WHERE `id`=198 AND `linkedId`=808;
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (198, 808);
