-- Persistent Death Gate departure point (issue #1520).
CREATE TABLE IF NOT EXISTS `character_death_gate_return` (
  `guid` int unsigned NOT NULL,
  `map` int unsigned NOT NULL,
  `posX` float NOT NULL,
  `posY` float NOT NULL,
  `posZ` float NOT NULL,
  `o` float NOT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
