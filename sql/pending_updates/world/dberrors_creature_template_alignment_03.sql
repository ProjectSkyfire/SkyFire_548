-- Continue top-down DBErrors cleanup for creature template validation.

UPDATE `creature_template` diff
JOIN (
    SELECT 3625 AS base_entry, 37373 AS diff_entry UNION ALL
    SELECT 4255, 37242 UNION ALL
    SELECT 5134, 37344 UNION ALL
    SELECT 5139, 37348 UNION ALL
    SELECT 10364, 37485 UNION ALL
    SELECT 10367, 37399 UNION ALL
    SELECT 11948, 37444 UNION ALL
    SELECT 12096, 37420 UNION ALL
    SELECT 12097, 37305 UNION ALL
    SELECT 13097, 37269 UNION ALL
    SELECT 13219, 37342 UNION ALL
    SELECT 13257, 37369 UNION ALL
    SELECT 13442, 37239 UNION ALL
    SELECT 13443, 37284 UNION ALL
    SELECT 13447, 37278 UNION ALL
    SELECT 13537, 37381 UNION ALL
    SELECT 13577, 37422 UNION ALL
    SELECT 13617, 37426 UNION ALL
    SELECT 15106, 37298 UNION ALL
    SELECT 36948, 38640 UNION ALL
    SELECT 41570, 51103
) pairs ON pairs.diff_entry = diff.`entry`
JOIN `creature_template` base ON base.`entry` = pairs.base_entry
SET diff.`npcflag` = base.`npcflag`
WHERE base.`difficulty_entry_3` = diff.`entry`
  AND diff.`npcflag` <> base.`npcflag`;

INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES
(51479, 0, 0, 2, 0),
(51481, 0, 0, 2, 0),
(51482, 0, 0, 2, 0),
(51483, 0, 0, 2, 0),
(55896, 0, 0, 2, 0),
(55907, 0.306, 1.5, 0, 0)
ON DUPLICATE KEY UPDATE
    `bounding_radius` = VALUES(`bounding_radius`),
    `combat_reach` = VALUES(`combat_reach`),
    `gender` = VALUES(`gender`),
    `modelid_other_gender` = VALUES(`modelid_other_gender`);
