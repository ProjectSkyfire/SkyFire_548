DROP TABLE `creature_classlevelstats`;
CREATE TABLE `creature_classlevelstats` (
	`level` TINYINT(4) NOT NULL,
	`class` TINYINT(4) NOT NULL,
	`OldContentBaseHP` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '1',
	`CurrentContentBaseHP` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '1',
	`basemana` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '1',
	`basearmor` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '1',
	PRIMARY KEY (`level`, `class`)
)
COLLATE='utf8_general_ci'
ENGINE=MyISAM;
