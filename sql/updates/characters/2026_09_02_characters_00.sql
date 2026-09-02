-- mod-prabowow: per-character XP multiplier chosen with ".xp rate <n>".

CREATE TABLE IF NOT EXISTS `character_xp_rate` (
  `guid` int unsigned NOT NULL COMMENT 'characters.guid',
  `rate` tinyint unsigned NOT NULL DEFAULT 1 COMMENT 'XP multiplier, 1 = server rate',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='mod-prabowow: per-character XP rate';
