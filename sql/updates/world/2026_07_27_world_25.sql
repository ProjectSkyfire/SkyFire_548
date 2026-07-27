-- Non-gossip DBErrors cleanup for loader-verified rows.

UPDATE `creature_template` diff
JOIN (
    SELECT 13097 AS `base_entry`, 37269 AS `diff_entry` UNION ALL
    SELECT 13442, 37239 UNION ALL
    SELECT 13443, 37284 UNION ALL
    SELECT 13537, 37381 UNION ALL
    SELECT 14026, 37436 UNION ALL
    SELECT 14027, 37439 UNION ALL
    SELECT 14029, 37437 UNION ALL
    SELECT 14030, 37440 UNION ALL
    SELECT 14031, 37441 UNION ALL
    SELECT 34461, 35745 UNION ALL
    SELECT 42166, 49058 UNION ALL
    SELECT 42178, 49055 UNION ALL
    SELECT 42179, 49049 UNION ALL
    SELECT 42180, 49052 UNION ALL
    SELECT 52571, 53858 UNION ALL
    SELECT 54015, 54018 UNION ALL
    SELECT 55265, 57772 UNION ALL
    SELECT 55689, 57956 UNION ALL
    SELECT 56700, 58251
) pairs ON pairs.`diff_entry` = diff.`entry`
JOIN `creature_template` base ON base.`entry` = pairs.`base_entry`
SET diff.`unit_class` = base.`unit_class`
WHERE diff.`unit_class` <> base.`unit_class`;

UPDATE `creature_template`
SET `minlevel` = 90,
    `maxlevel` = 90
WHERE `entry` IN (59899, 68981, 73811, 73818)
  AND `minlevel` = 100
  AND `maxlevel` = 100;

UPDATE `creature_template`
SET `flags_extra` = `flags_extra` - 1
WHERE `entry` = 68869
  AND (`flags_extra` & 1) <> 0;

DELETE FROM `creature`
WHERE `guid` IN (733, 734, 735, 737, 1016, 1032, 1293, 1294, 232782, 232783, 232784, 237138)
  AND `id` IN (67382, 67383, 67384, 68561, 74019, 73422, 74010, 74012, 68993)
  AND NOT EXISTS (
      SELECT 1
      FROM `creature_template`
      WHERE `creature_template`.`entry` = `creature`.`id`
  );

DELETE FROM `gameobject`
WHERE `guid` IN (
      101194, 101195, 101196, 101197, 101198, 101199, 101200, 101201, 101202, 101203,
      101204, 101205, 101206, 101207, 101208, 101209, 101210, 101211, 200008, 200011
  )
  AND `id` IN (215413, 213074, 212922)
  AND NOT EXISTS (
      SELECT 1
      FROM `gameobject_template`
      WHERE `gameobject_template`.`entry` = `gameobject`.`id`
  );
