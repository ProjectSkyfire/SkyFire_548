-- Add optional server-side tavern rest footprints for taverns whose DBC trigger
-- is only a boundary marker instead of a reliable inn interior.

CREATE TABLE IF NOT EXISTS `areatrigger_tavern_rest_area` (
  `id` int unsigned NOT NULL COMMENT 'AreaTrigger.dbc id from areatrigger_tavern',
  `map` smallint unsigned NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `radius` float NOT NULL DEFAULT '0',
  `box_x` float NOT NULL DEFAULT '0',
  `box_y` float NOT NULL DEFAULT '0',
  `box_z` float NOT NULL DEFAULT '0',
  `box_orientation` float NOT NULL DEFAULT '0',
  `name` varchar(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Server-side tavern rest footprints';

INSERT IGNORE INTO `areatrigger_tavern` (`id`, `name`) VALUES
    (5830, 'Elwynn Forest - Goldshire - Lion''s Pride Inn');

INSERT INTO `areatrigger_tavern_rest_area`
    (`id`, `map`, `position_x`, `position_y`, `position_z`, `radius`, `box_x`, `box_y`, `box_z`, `box_orientation`, `name`)
VALUES
    (5830, 0, -9467.07, 23.8406, 56.34, 0, 60, 48, 20, 0, 'Elwynn Forest - Goldshire - Lion''s Pride Inn')
ON DUPLICATE KEY UPDATE
    `map` = VALUES(`map`),
    `position_x` = VALUES(`position_x`),
    `position_y` = VALUES(`position_y`),
    `position_z` = VALUES(`position_z`),
    `radius` = VALUES(`radius`),
    `box_x` = VALUES(`box_x`),
    `box_y` = VALUES(`box_y`),
    `box_z` = VALUES(`box_z`),
    `box_orientation` = VALUES(`box_orientation`),
    `name` = VALUES(`name`);
