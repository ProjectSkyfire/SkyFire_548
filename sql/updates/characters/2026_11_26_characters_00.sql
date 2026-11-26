-- Required with transport passenger persistence/recovery core changes.
CREATE TABLE IF NOT EXISTS `character_transport_recovery` (
  `guid` int unsigned NOT NULL,
  `entry` int unsigned NOT NULL,
  `map` int unsigned NOT NULL COMMENT '4294967295 when no safe arrival is configured',
  `position_x` float NOT NULL DEFAULT 0,
  `position_y` float NOT NULL DEFAULT 0,
  `position_z` float NOT NULL DEFAULT 0,
  `orientation` float NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
