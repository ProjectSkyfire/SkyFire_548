-- Darkshore quests that are finished by clicking a creature. The creatures had no spell click flag, no click row and no
-- click condition, so the click did nothing. As in Legends of Azeroth 5.4.8.
--
-- Ritual Materials (13566) asks for 3 Moonstalker Whisker, 3 Tuft of Mottled Doe Hair and 3 Thistle Bear Fur, and
-- nothing else in the data gives them. Moonstalker (33127, 36 spawns, spell 62512), Mottled Doe (33313, 44 spawns, spell
-- 62788) and Hungry Thistle Bear (33978, 17 spawns, spell 64116) can be clicked while the quest is taken and fewer than 3
-- of the item are carried. The spells are cast by the player on the animal and so cannot create the item; as in Legends
-- of Azeroth the animal's script adds it. The Moonstalker turns hostile and attacks,
-- the doe and the bear leave. The combat row of the Moonstalker and of the bear keeps id 0, the new rows follow it.
-- The Seeds of Life (13882) asks for the Seed of the Sky: the Darkshore Wisp (34306, 29 spawns) can be clicked while the
-- quest is taken and no seed is carried, spell 65125 creates the item for the player.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (33127, 33313, 33978, 34306);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 33313 AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (33127, 33313, 33978, 34306);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(33127, 62512, 1, 0),
(33313, 62788, 1, 0),
(33978, 64116, 1, 0),
(34306, 65125, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 33127 AND `SourceEntry` = 62512;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 33313 AND `SourceEntry` = 62788;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 33978 AND `SourceEntry` = 64116;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 34306 AND `SourceEntry` = 65125;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 33127, 62512, 0, 0, 2, 0, 44969, 3, 0, 1, 0, 0, '', 'Moonstalker - only without 3 Moonstalker Whisker'),
(18, 33127, 62512, 0, 0, 9, 0, 13566, 0, 0, 0, 0, 0, '', 'Moonstalker - only with Ritual Materials'),
(18, 33313, 62788, 0, 0, 2, 0, 45027, 3, 0, 1, 0, 0, '', 'Mottled Doe - only without 3 Tuft of Mottled Doe Hair'),
(18, 33313, 62788, 0, 0, 9, 0, 13566, 0, 0, 0, 0, 0, '', 'Mottled Doe - only with Ritual Materials'),
(18, 33978, 64116, 0, 0, 2, 0, 45885, 3, 0, 1, 0, 0, '', 'Hungry Thistle Bear - only without 3 Thistle Bear Fur'),
(18, 33978, 64116, 0, 0, 9, 0, 13566, 0, 0, 0, 0, 0, '', 'Hungry Thistle Bear - only with Ritual Materials'),
(18, 34306, 65125, 0, 0, 2, 0, 46355, 1, 0, 1, 0, 0, '', 'Darkshore Wisp - only without a Seed of the Sky'),
(18, 34306, 65125, 0, 0, 9, 0, 13882, 0, 0, 0, 0, 0, '', 'Darkshore Wisp - only with The Seeds of Life');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 33127 AND `id` IN (1, 2, 3, 4);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 33313 AND `id` IN (0, 1, 2);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 33978 AND `id` IN (1, 2, 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(33127, 0, 1, 2, 73, 0, 100, 0, 0, 0, 0, 0, 0, 56, 44969, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker - On Spellclick - Add Item Moonstalker Whisker'),
(33127, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker - Linked To Id 1 - Set Faction 14'),
(33127, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker - Linked To Id 2 - Remove NpcFlag 16777216'),
(33127, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Moonstalker - Linked To Id 3 - Attack Start'),
(33313, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 56, 45027, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mottled Doe - On Spellclick - Add Item Tuft of Mottled Doe Hair'),
(33313, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mottled Doe - Linked To Id 0 - Remove NpcFlag 16777216'),
(33313, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mottled Doe - Linked To Id 1 - Despawn after 2 sec'),
(33978, 0, 1, 2, 73, 0, 100, 0, 0, 0, 0, 0, 0, 56, 45885, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hungry Thistle Bear - On Spellclick - Add Item Thistle Bear Fur'),
(33978, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hungry Thistle Bear - Linked To Id 1 - Remove NpcFlag 16777216'),
(33978, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hungry Thistle Bear - Linked To Id 2 - Despawn after 5 sec');
