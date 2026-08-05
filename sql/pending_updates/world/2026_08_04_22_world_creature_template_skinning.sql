-- Correct creature skinning data for issue #1220.

UPDATE `creature_template`
SET `skinloot` = 43704
WHERE `entry` = 43704;

UPDATE `creature_template`
SET `skinloot` = 42357
WHERE `entry` = 42357;

UPDATE `creature_template`
SET `skinloot` = 0
WHERE `entry` IN (217, 422);

DELETE FROM `skinning_loot_template`
WHERE `entry` IN (217, 422, 42357, 43704);

INSERT INTO `skinning_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(42357, 783, 4.7793, 1, 0, 1, 1),
(42357, 2318, 40, 1, 0, 1, 1),
(42357, 2934, 30, 1, 0, 1, 1),
(43704, 783, 3.9915, 1, 0, 1, 1),
(43704, 2318, 40, 1, 0, 1, 1),
(43704, 2319, 34.5165, 1, 0, 1, 1),
(43704, 4232, 5, 1, 0, 1, 1);
