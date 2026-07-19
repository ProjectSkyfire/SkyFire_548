-- Quest 26787 "Bear In Mind": Coalpelt Bear (44016) must drop Black Bear Brain (60334).

UPDATE `creature_template`
SET `lootid` = 44016,
    `questItem1` = 60334
WHERE `entry` = 44016;

DELETE FROM `creature_loot_template`
WHERE `entry` = 44016 AND `item` = 60334;

INSERT INTO `creature_loot_template`
    (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (44016, 60334, -53.6947, 1, 0, 1, 1);
