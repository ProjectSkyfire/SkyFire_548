-- Fill in Mists of Pandaria LFG requirement metadata.
-- Existing coordinates are preserved; this update only corrects queue requirement values.

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
VALUES
    -- Random Mists queues
    (462, 0, 0, 0, 0, 435),
    (463, 0, 0, 0, 0, 325),
    (493, 0, 0, 0, 0, 425),
    (641, 0, 0, 0, 0, 480),

    -- Mists normal dungeons
    (464, 0, 0, 0, 0, 325),
    (465, 0, 0, 0, 0, 325),
    (466, 0, 0, 0, 0, 366),
    (467, 0, 0, 0, 0, 366),
    (630, 0, 0, 0, 0, 393),
    (631, 0, 0, 0, 0, 393),

    -- Mists heroic dungeons
    (468, 0, 0, 0, 0, 435),
    (469, 0, 0, 0, 0, 435),
    (470, 0, 0, 0, 0, 435),
    (471, 0, 0, 0, 0, 435),
    (472, 0, 0, 0, 0, 435),
    (473, 0, 0, 0, 0, 435),
    (474, 0, 0, 0, 0, 435),
    (519, 0, 0, 0, 0, 435),
    (554, 0, 0, 0, 0, 435),

    -- Mists normal scenarios
    (492, 0, 0, 0, 0, 425),
    (499, 0, 0, 0, 0, 425),
    (504, 0, 0, 0, 0, 425),
    (511, 0, 0, 0, 0, 425),
    (517, 0, 0, 0, 0, 425),
    (537, 0, 0, 0, 0, 425),
    (539, 0, 0, 0, 0, 425),
    (566, 0, 0, 0, 0, 425),
    (567, 0, 0, 0, 0, 425),
    (586, 0, 0, 0, 0, 425),
    (589, 0, 0, 0, 0, 425),
    (590, 0, 0, 0, 0, 425),
    (593, 0, 0, 0, 0, 425),
    (595, 0, 0, 0, 0, 425),
    (616, 0, 0, 0, 0, 425),
    (619, 0, 0, 0, 0, 425),
    (646, 0, 0, 0, 0, 425),
    (647, 0, 0, 0, 0, 425),
    (649, 0, 0, 0, 0, 425),
    (654, 0, 0, 0, 0, 425),
    (655, 0, 0, 0, 0, 425),

    -- Mists heroic scenarios
    (588, 0, 0, 0, 0, 480),
    (624, 0, 0, 0, 0, 480),
    (625, 0, 0, 0, 0, 480),
    (637, 0, 0, 0, 0, 480),
    (639, 0, 0, 0, 0, 480),
    (648, 0, 0, 0, 0, 480),
    (652, 0, 0, 0, 0, 480),

    -- Mists Raid Finder and flexible raid wings
    (527, 0, 0, 0, 0, 460),
    (528, 0, 0, 0, 0, 460),
    (526, 0, 0, 0, 0, 470),
    (529, 0, 0, 0, 0, 470),
    (530, 0, 0, 0, 0, 470),
    (610, 0, 0, 0, 0, 480),
    (611, 0, 0, 0, 0, 480),
    (612, 0, 0, 0, 0, 480),
    (613, 0, 0, 0, 0, 480),
    (716, 0, 0, 0, 0, 496),
    (717, 0, 0, 0, 0, 496),
    (724, 0, 0, 0, 0, 496),
    (725, 0, 0, 0, 0, 496),
    (726, 0, 0, 0, 0, 496),
    (728, 0, 0, 0, 0, 496),
    (729, 0, 0, 0, 0, 496),
    (730, 0, 0, 0, 0, 496),
    (771, 0, 0, 0, 0, 496),
    (772, 0, 0, 0, 0, 496),
    (773, 0, 0, 0, 0, 496),
    (774, 0, 0, 0, 0, 496)
ON DUPLICATE KEY UPDATE
    `requiredItemLevel` = VALUES(`requiredItemLevel`);

INSERT INTO `access_requirement`
    (`mapId`, `difficulty`, `level_min`, `level_max`, `item`, `item2`, `quest_done_A`, `quest_done_H`, `completed_achievement`, `quest_failed_text`, `iLvl`, `comment`)
VALUES
    -- Mists normal dungeons
    (960, 1, 85, 0, 0, 0, 0, 0, 0, '', 325, 'Temple of the Jade Serpent'),
    (961, 1, 85, 0, 0, 0, 0, 0, 0, '', 325, 'Stormstout Brewery'),
    (959, 1, 87, 0, 0, 0, 0, 0, 0, '', 366, 'Shado-Pan Monastery'),
    (994, 1, 87, 0, 0, 0, 0, 0, 0, '', 366, 'Mogu''shan Palace'),
    (962, 1, 88, 0, 0, 0, 0, 0, 0, '', 393, 'Gate of the Setting Sun'),
    (1011, 1, 88, 0, 0, 0, 0, 0, 0, '', 393, 'Siege of Niuzao Temple'),

    -- Mists heroic dungeons
    (960, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Temple of the Jade Serpent Heroic'),
    (961, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Stormstout Brewery Heroic'),
    (959, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Shado-Pan Monastery Heroic'),
    (962, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Gate of the Setting Sun Heroic'),
    (1007, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Scholomance Heroic'),
    (1001, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Scarlet Halls Heroic'),
    (1004, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Scarlet Monastery Heroic'),
    (994, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Mogu''shan Palace Heroic'),
    (1011, 2, 90, 0, 0, 0, 0, 0, 0, '', 435, 'Siege of Niuzao Temple Heroic'),

    -- Mists normal scenarios. Theramore map rows are intentionally omitted because level 85 queues share those maps.
    (1024, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Greenstone Village Scenario'),
    (1048, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Unga Ingoo Scenario'),
    (1030, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Crypt of Forgotten Kings Scenario'),
    (1031, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Arena of Annihilation Scenario'),
    (1005, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'A Brewing Storm Scenario'),
    (1050, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Assault on Zan''vess Scenario'),
    (1051, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Brewmoon Festival Scenario'),
    (1095, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Dagger in the Dark Scenario'),
    (1104, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'A Little Patience Scenario'),
    (1103, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Lion''s Landing Scenario'),
    (1102, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Domination Point Scenario'),
    (1130, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Blood in the Snow Scenario'),
    (1144, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Dark Heart of Pandaria Scenario'),
    (1131, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Secrets of Ragefire Scenario'),
    (1099, 12, 90, 0, 0, 0, 0, 0, 0, '', 425, 'Battle on the High Seas Scenario'),

    -- Mists heroic scenarios
    (1099, 11, 90, 0, 0, 0, 0, 0, 0, '', 480, 'Battle on the High Seas Heroic Scenario'),
    (1144, 11, 90, 0, 0, 0, 0, 0, 0, '', 480, 'Dark Heart of Pandaria Heroic Scenario'),
    (1131, 11, 90, 0, 0, 0, 0, 0, 0, '', 480, 'Secrets of Ragefire Heroic Scenario'),
    (1130, 11, 90, 0, 0, 0, 0, 0, 0, '', 480, 'Blood in the Snow Heroic Scenario'),
    (1005, 11, 90, 0, 0, 0, 0, 0, 0, '', 480, 'A Brewing Storm Heroic Scenario'),
    (1030, 11, 90, 0, 0, 0, 0, 0, 0, '', 480, 'Crypt of Forgotten Kings Heroic Scenario'),

    -- Mists Raid Finder and flexible raids
    (1008, 7, 90, 0, 0, 0, 0, 0, 0, '', 460, 'Mogu''shan Vaults Raid Finder'),
    (1009, 7, 90, 0, 0, 0, 0, 0, 0, '', 470, 'Heart of Fear Raid Finder'),
    (996, 7, 90, 0, 0, 0, 0, 0, 0, '', 470, 'Terrace of Endless Spring Raid Finder'),
    (1098, 7, 90, 0, 0, 0, 0, 0, 0, '', 480, 'Throne of Thunder Raid Finder'),
    (1136, 7, 90, 0, 0, 0, 0, 0, 0, '', 496, 'Siege of Orgrimmar Raid Finder'),
    (1136, 14, 90, 0, 0, 0, 0, 0, 0, '', 496, 'Siege of Orgrimmar Flexible')
ON DUPLICATE KEY UPDATE
    `level_min` = VALUES(`level_min`),
    `level_max` = VALUES(`level_max`),
    `iLvl` = VALUES(`iLvl`),
    `comment` = VALUES(`comment`);
