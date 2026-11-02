CREATE TABLE IF NOT EXISTS `character_spell_charges` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `category` int unsigned NOT NULL DEFAULT '0' COMMENT 'Spell charge category',
  `consumedCharges` tinyint unsigned NOT NULL DEFAULT '0',
  `resetTime` int unsigned NOT NULL DEFAULT '0',
  `baseRegenTime` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`category`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
