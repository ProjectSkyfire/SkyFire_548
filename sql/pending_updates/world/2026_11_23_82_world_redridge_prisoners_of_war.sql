-- Prisoners of War (26646): the Blackrock Key Pouch in the southern cave holds the Blackrock Holding Pen Key (item 59261),
-- the key opens the three Blackrock Holding Pens in Render's Valley and each opened pen gives the credit "Prisoners of War
-- Freed" (43574). The pouch had no loot table and the pens had no script, so the key could not be found and opening a pen
-- gave nothing. Rows and loot are as in Legends of Azeroth 5.4.8.

UPDATE `gameobject_template` SET `data1` = 204437 WHERE `entry` = 204437 AND `ScriptName` = '';
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (204435, 204441, 204442) AND `ScriptName` = '';

DELETE FROM `gameobject_loot_template` WHERE `entry` = 204437;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(204437, 59261, -100, 1, 0, 1, 1);

DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` IN (204435, 204441, 204442);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(204435, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 43574, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Holding Pen - On Active alternative - Killcredit'),
(204441, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 43574, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Holding Pen - On Active alternative - Killcredit'),
(204442, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 43574, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Holding Pen - On Active alternative - Killcredit');
