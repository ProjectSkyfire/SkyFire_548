-- Add optional Dungeon Finder template data for entrance and lock metadata.

CREATE TABLE IF NOT EXISTS `lfg_dungeon_template` (
    `dungeonId` INT UNSIGNED NOT NULL,
    `position_x` FLOAT NOT NULL DEFAULT 0,
    `position_y` FLOAT NOT NULL DEFAULT 0,
    `position_z` FLOAT NOT NULL DEFAULT 0,
    `orientation` FLOAT NOT NULL DEFAULT 0,
    `requiredItemLevel` INT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`dungeonId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `lfg_dungeon_template`
    (`dungeonId`, `position_x`, `position_y`, `position_z`, `orientation`, `requiredItemLevel`)
SELECT
    entrance.`dungeonId`,
    entrance.`position_x`,
    entrance.`position_y`,
    entrance.`position_z`,
    entrance.`orientation`,
    0
FROM `lfg_entrances` entrance
LEFT JOIN `lfg_dungeon_template` existing_template
    ON existing_template.`dungeonId` = entrance.`dungeonId`
WHERE existing_template.`dungeonId` IS NULL;
