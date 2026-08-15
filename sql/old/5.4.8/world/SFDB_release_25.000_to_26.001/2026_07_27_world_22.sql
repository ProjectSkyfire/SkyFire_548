-- Clean stale loader-skipped world data reported by DBErrors.

DELETE cqs
FROM `creature_queststarter` cqs
LEFT JOIN `creature_template` ct ON ct.`entry` = cqs.`id`
LEFT JOIN `quest_template` qt ON qt.`Id` = cqs.`quest`
WHERE ct.`entry` IS NULL OR qt.`Id` IS NULL;

DELETE cqe
FROM `creature_questender` cqe
LEFT JOIN `creature_template` ct ON ct.`entry` = cqe.`id`
LEFT JOIN `quest_template` qt ON qt.`Id` = cqe.`quest`
WHERE ct.`entry` IS NULL OR qt.`Id` IS NULL;

DELETE gqs
FROM `gameobject_queststarter` gqs
LEFT JOIN `gameobject_template` gt ON gt.`entry` = gqs.`id`
LEFT JOIN `quest_template` qt ON qt.`Id` = gqs.`quest`
WHERE gt.`entry` IS NULL OR qt.`Id` IS NULL;

DELETE gqe
FROM `gameobject_questender` gqe
LEFT JOIN `gameobject_template` gt ON gt.`entry` = gqe.`id`
LEFT JOIN `quest_template` qt ON qt.`Id` = gqe.`quest`
WHERE gt.`entry` IS NULL OR qt.`Id` IS NULL;

UPDATE `creature_template` ct
LEFT JOIN `npc_spellclick_spells` sc ON sc.`npc_entry` = ct.`entry`
SET ct.`npcflag` = ct.`npcflag` - 16777216
WHERE (ct.`npcflag` & 16777216) <> 0
  AND sc.`npc_entry` IS NULL;

DELETE FROM `instance_encounters`
WHERE `creditType` = 1
  AND (`entry`, `creditEntry`) IN
  (
    (296, 58630),
    (300, 58630),
    (334, 68572),
    (336, 68572),
    (338, 68574),
    (339, 68574),
    (567, 59046),
    (568, 59046),
    (1086, 68184),
    (1121, 59450),
    (1133, 65074),
    (1135, 64899),
    (1141, 64985)
  );

DELETE b
FROM `battle_pet_item_to_species` b
LEFT JOIN `item_template` i ON i.`entry` = b.`itemId`
WHERE i.`entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 81, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 81
WHERE src.`level` = 80
  AND src.`creature_entry` IN (1)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 82, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 82
WHERE src.`level` = 81
  AND src.`creature_entry` IN (1)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 83, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 83
WHERE src.`level` = 82
  AND src.`creature_entry` IN (1)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 84, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 84
WHERE src.`level` = 83
  AND src.`creature_entry` IN (1)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 85, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 85
WHERE src.`level` = 84
  AND src.`creature_entry` IN (1)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 86, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 86
WHERE src.`level` = 85
  AND src.`creature_entry` IN (1, 329, 416, 417, 510, 575, 1860, 1863, 3450, 3939, 5058, 5766, 6250, 8477, 8996, 10928, 10979, 12922, 14385, 15214, 15352, 15438, 17252, 19668, 22362, 24476, 24656, 24815, 25553, 25566, 26101, 26125)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 87, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 87
WHERE src.`level` = 86
  AND src.`creature_entry` IN (1, 329, 416, 417, 510, 575, 1860, 1863, 3450, 3939, 5058, 5766, 6250, 8477, 8996, 10928, 10979, 12922, 14385, 15214, 15352, 15438, 17252, 19668, 22362, 24476, 24656, 24815, 25553, 25566, 26101, 26125)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 88, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 88
WHERE src.`level` = 87
  AND src.`creature_entry` IN (1, 329, 416, 417, 510, 575, 1860, 1863, 3450, 3939, 5058, 5766, 6250, 8477, 8996, 10928, 10979, 12922, 14385, 15214, 15352, 15438, 17252, 19668, 22362, 24476, 24656, 24815, 25553, 25566, 26101, 26125)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 89, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 89
WHERE src.`level` = 88
  AND src.`creature_entry` IN (1, 329, 416, 417, 510, 575, 1860, 1863, 3450, 3939, 5058, 5766, 6250, 8477, 8996, 10928, 10979, 12922, 14385, 15214, 15352, 15438, 17252, 19668, 22362, 24476, 24656, 24815, 25553, 25566, 26101, 26125)
  AND dst.`creature_entry` IS NULL;

INSERT INTO `pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT src.`creature_entry`, 90, src.`hp`, src.`mana`, src.`armor`, src.`str`, src.`agi`, src.`sta`, src.`inte`, src.`spi`
FROM `pet_levelstats` src
LEFT JOIN `pet_levelstats` dst ON dst.`creature_entry` = src.`creature_entry` AND dst.`level` = 90
WHERE src.`level` = 89
  AND src.`creature_entry` IN (1, 329, 416, 417, 510, 575, 1860, 1863, 3450, 3939, 5058, 5766, 6250, 8477, 8996, 10928, 10979, 12922, 14385, 15214, 15352, 15438, 17252, 19668, 22362, 24476, 24656, 24815, 25553, 25566, 26101, 26125)
  AND dst.`creature_entry` IS NULL;
