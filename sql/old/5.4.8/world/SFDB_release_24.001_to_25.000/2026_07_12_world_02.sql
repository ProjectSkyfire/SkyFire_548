-- Balance Blackrock Spy loot.
-- The active loot-bearing Blackrock Spy template in the world data is 49874.

UPDATE `creature_template`
SET `lootid` = 49874
WHERE `entry` = 49874;

DELETE FROM `creature_loot_template`
WHERE `entry` = 49874;

INSERT INTO `creature_loot_template`
    (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (49874,   117, 10.0700, 1, 0, 1, 1),
    (49874,   159,  4.8500, 1, 0, 1, 1),
    (49874,   805,  1.2200, 1, 0, 1, 1),
    (49874,   828,  1.4200, 1, 0, 1, 1),
    (49874,  2589,  0.2700, 1, 0, 1, 2),
    (49874,  3300,  0.0300, 1, 0, 1, 1),
    (49874,  4496,  1.1000, 1, 0, 1, 1),
    (49874,  5465,  0.0500, 1, 0, 1, 1),
    (49874,  5523,  0.0500, 1, 0, 1, 1),
    (49874,  5571,  1.0900, 1, 0, 1, 1),
    (49874,  5572,  0.9500, 1, 0, 1, 1),
    (49874, 55973,  0.0800, 1, 0, 1, 1),
    (49874, 55983,  0.0300, 1, 0, 1, 1),
    (49874, 56147,  0.0300, 1, 0, 1, 1),
    (49874, 62328,  0.0600, 1, 0, 1, 1),
    (49874, 62771,  0.0300, 1, 0, 1, 1),
    (49874, 62772,  0.1800, 1, 0, 1, 1);
