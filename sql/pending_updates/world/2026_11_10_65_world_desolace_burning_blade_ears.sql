-- Desolace: Ears Are Burning (14232, 14362). The two quests are started by a Burning Blade Ear that drops from the
-- Burning Blade casters, and no loot row gave it. Safe to run twice.
DELETE FROM `creature_loot_template` WHERE `entry` IN (4663,4664,4665,4666,4667) AND `item` IN (49010,49203);
INSERT INTO `creature_loot_template`
(`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`)
VALUES
(4663,49010,1.9256,1,0,1,1),
(4663,49203,1.2887,1,0,1,1),
(4664,49010,1.8425,1,0,1,1),
(4664,49203,1.1102,1,0,1,1),
(4665,49010,1.1403,1,0,1,1),
(4665,49203,0.9466,1,0,1,1),
(4666,49010,1.7178,1,0,1,1),
(4666,49203,1.5129,1,0,1,1),
(4667,49010,9.25,1,0,1,1),
(4667,49203,6.6888,1,0,1,1);
