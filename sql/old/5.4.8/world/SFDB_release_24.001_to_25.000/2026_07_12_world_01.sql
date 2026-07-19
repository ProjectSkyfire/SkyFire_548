-- Balance Blackrock Battle Worg loot.
-- Player-facing reference: https://www.wowhead.com/npc=43340/blackrock-battle-worg
-- The active loot-bearing Blackrock Battle Worg template in the world data is 49871.

UPDATE `creature_template`
SET `lootid` = 49871
WHERE `entry` = 49871;

DELETE FROM `creature_loot_template`
WHERE `entry` = 49871;

INSERT INTO `creature_loot_template`
    (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (49871,   805,  0.1000, 1, 0, 1, 1),
    (49871,   828,  0.1000, 1, 0, 1, 1),
    (49871,  1468,  0.0020, 1, 0, 1, 1),
    (49871,  2672,  0.0010, 1, 0, 1, 1),
    (49871,  3300, 23.9292, 1, 0, 1, 1),
    (49871,  4496,  0.1099, 1, 0, 1, 1),
    (49871,  5571,  0.1119, 1, 0, 1, 1),
    (49871,  5572,  0.1000, 1, 0, 1, 1),
    (49871, 20482,  0.0010, 1, 0, 1, 1),
    (49871, 20845,  0.0010, 1, 0, 1, 1),
    (49871, 22934,  0.0010, 1, 0, 1, 1),
    (49871, 58361, -0.0051, 1, 0, 1, 1),
    (49871, 62328, 76.0708, 1, 0, 1, 1);
