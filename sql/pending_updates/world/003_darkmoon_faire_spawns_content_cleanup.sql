-- Darkmoon Faire loader cleanup for quest, loot, gossip, text, SmartAI, and condition support.

START TRANSACTION;

-- Add Tonk Scrap object templates referenced by the Darkmoon Faire spawns.
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES
(209274, 3, 7975, 'Tonk Scrap', '', '', '', 0, 0, 0.5, 71968, 0, 0, 0, 0, 0, 1691, 209273, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
(209275, 3, 8938, 'Tonk Scrap', '', '', '', 0, 0, 0.5, 71968, 0, 0, 0, 0, 0, 1691, 209273, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595)
ON DUPLICATE KEY UPDATE
    `type` = VALUES(`type`),
    `displayId` = VALUES(`displayId`),
    `name` = VALUES(`name`),
    `IconName` = VALUES(`IconName`),
    `castBarCaption` = VALUES(`castBarCaption`),
    `unk1` = VALUES(`unk1`),
    `faction` = VALUES(`faction`),
    `flags` = VALUES(`flags`),
    `size` = VALUES(`size`),
    `questItem1` = VALUES(`questItem1`),
    `data0` = VALUES(`data0`),
    `data1` = VALUES(`data1`),
    `data2` = VALUES(`data2`),
    `data3` = VALUES(`data3`),
    `AIName` = VALUES(`AIName`),
    `ScriptName` = VALUES(`ScriptName`),
    `WDBVerified` = VALUES(`WDBVerified`);

-- Hook Darkmoon Faire loot templates to the owning creature and object templates.
UPDATE `creature_template`
SET `lootid` = `entry`
WHERE `entry` IN (55475, 58336, 71992);

UPDATE `gameobject_template`
SET `data1` = CASE `entry`
    WHEN 209273 THEN 209273
    WHEN 209274 THEN 209273
    WHEN 209275 THEN 209273
    WHEN 209283 THEN 209283
    WHEN 209284 THEN 209284
    WHEN 209287 THEN 209287
    WHEN 209620 THEN 209620
    WHEN 210216 THEN 210216
    ELSE `data1`
END
WHERE `entry` IN (209273, 209274, 209275, 209283, 209284, 209287, 209620, 210216);

-- Existing Orchard Tool spawns use instant respawn and must not be marked consumable.
UPDATE `gameobject_template`
SET `data3` = 0
WHERE `entry` IN (209344, 209345)
  AND `type` = 3;

-- Darkmoon Striders are clickable ride creatures.
UPDATE `creature_template`
SET `npcflag` = `npcflag` | 16777216
WHERE `entry` = 68491;

DELETE FROM `npc_spellclick_spells`
WHERE `npc_entry` = 68491;

INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(68491, 134931, 3, 0);

-- Remove stale Sayge aura gating that references a spell missing from the 5.4.8 client data.
UPDATE `smart_scripts`
SET `link` = 0
WHERE `entryorguid` = 14822
  AND `source_type` = 0
  AND `link` = 14;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 14822
  AND `source_type` = 0
  AND `id` = 14
  AND `event_type` = 61
  AND `action_type` = 11
  AND `action_param1` = 23770;

DELETE FROM `conditions`
WHERE `ConditionTypeOrReference` = 1
  AND `ConditionValue1` = 23770
  AND (
      (`SourceTypeOrReferenceId` = 14 AND `SourceGroup` = 6186 AND `SourceEntry` IN (7339, 7393))
      OR (`SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 6186 AND `SourceEntry` = 0)
  );

COMMIT;
