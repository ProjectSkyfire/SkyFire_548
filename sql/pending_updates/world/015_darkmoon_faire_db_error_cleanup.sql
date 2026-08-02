-- Darkmoon Faire DB error cleanup.

START TRANSACTION;

-- Normalize Darkmoon Faire collectible gameobject loot IDs to the loot IDs
-- referenced by gameobject_template.data1.
DELETE FROM `gameobject_loot_template`
WHERE `entry` IN (209273, 209283, 209284, 209287, 40164, 40247, 40249, 40250);

INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(40164, 71968, -100, 1, 0, 1, 1),
(40247, 72018, -100, 1, 0, 1, 1),
(40249, 72046, -100, 1, 0, 1, 1),
(40250, 72052, -100, 1, 0, 1, 1);

UPDATE `gameobject_template`
SET `data1` = CASE `entry`
    WHEN 209273 THEN 40164
    WHEN 209274 THEN 40164
    WHEN 209275 THEN 40164
    WHEN 209283 THEN 40247
    WHEN 209284 THEN 40249
    WHEN 209287 THEN 40250
    ELSE `data1`
END
WHERE `entry` IN (209273, 209274, 209275, 209283, 209284, 209287);

-- Bind the Darkmoon Deathmatch area transition spell script.
DELETE FROM `spell_script_names`
WHERE `ScriptName` = 'spell_darkmoon_deathmatch';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(108941, 'spell_darkmoon_deathmatch');

-- Use existing kill-credit spells instead of direct SmartAI kill credit.
UPDATE `smart_scripts`
SET `action_type` = 11,
    `action_param1` = 79227,
    `action_param2` = 2,
    `action_param3` = 0,
    `action_param4` = 0,
    `action_param5` = 0,
    `action_param6` = 0,
    `comment` = 'High Tinker Mekkatorque - On Script - Cast Credit'
WHERE `entryorguid` = 4231700
  AND `source_type` = 9
  AND `id` = 12
  AND `action_type` = 33
  AND `action_param1` = 42463;

UPDATE `smart_scripts`
SET `action_type` = 11,
    `action_param1` = 79606,
    `action_param2` = 2,
    `action_param3` = 0,
    `action_param4` = 0,
    `action_param5` = 0,
    `action_param6` = 0,
    `comment` = 'Makeshift Cage - On Script - Cast Credit'
WHERE `entryorguid` = 20401900
  AND `source_type` = 9
  AND `id` = 1
  AND `action_type` = 33
  AND `action_param1` = 42645;

COMMIT;
