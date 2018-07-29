DROP TABLE IF EXISTS `playercreateinfo_spell_cast`;
CREATE TABLE IF NOT EXISTS `playercreateinfo_spell_cast` (
  `racemask` int(10) unsigned NOT NULL DEFAULT '0',
  `classmask` int(10) unsigned NOT NULL DEFAULT '0',
  `Spell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Note` varchar(255) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;