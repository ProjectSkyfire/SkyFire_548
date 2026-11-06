-- Winterspring quest fixes.
-- Sources: 5.4.8 data for the spawns, the loot rows, the quest ender, the prerequisite of "A Yeti of Your Own", the spellclick row and
-- the SmartAI rows; 4.3.4 and 5.4.8 for the removal of the stale drop conditions (the 4.3.4 data still has them, the
-- 5.4.8 data does not). Where 5.4.8 SmartAI casts an item-creating spell from a dying creature it is written as an
-- item grant here (a dead creature cannot cast), and the stored target list is replaced by the invoker.
-- Safe to run twice: every block deletes its own keys first or sets absolute values.

-- The Crudely-Written Log (starts "The Final Piece", needed for "Words of the High Chief" and so for the whole
-- Kel'Theril chain) drops from High Chief Winterfall only if the player has been rewarded the old quest 5128, which is
-- deprecated and cannot be taken. The Winterfall Spirit Beads ("Winterfall Activity", "More Beads for Salfa") drop only
-- if the old quest 8464 was rewarded, also deprecated. 5.4.8 has neither condition.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 1 AND `SourceGroup` = 10738 AND `SourceEntry` = 12842 AND `ConditionTypeOrReference` = 8 AND `ConditionValue1` = 5128;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 1 AND `SourceGroup` IN (7438, 7439, 7440, 7441, 7442, 10738, 10916) AND `SourceEntry` = 21383 AND `ConditionTypeOrReference` = 8 AND `ConditionValue1` = 8464;

-- The new Empty Firewater Flask (64353) starts "Winterfall Firewater" and has no source: the chain 28462 to 28470 cannot
-- be started. Drop rows of the 6 Winterfall creatures, 5.4.8 chances.
DELETE FROM `creature_loot_template` WHERE `entry` IN (7438, 7439, 7440, 7441, 7442, 10916) AND `item` = 64353;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(7438, 64353, 0.4676, 1, 0, 1, 1),
(7439, 64353, 0.4407, 1, 0, 1, 1),
(7440, 64353, 1.2777, 1, 0, 1, 1),
(7441, 64353, 1.9727, 1, 0, 1, 1),
(7442, 64353, 1.7143, 1, 0, 1, 1),
(10916, 64353, 0.9272, 1, 0, 1, 1);

-- "Winterfall Firewater" is started by the Empty Firewater Flask and has no quest ender, so it cannot be handed in and
-- the rest of the Donova chain (28464 and on) never opens. 5.4.8 ends it at Donova Snowden; 4.3.4 has no ender.
DELETE FROM `creature_questender` WHERE `id` = 9298 AND `quest` = 28462;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(9298, 28462);

-- Quest NPCs that were never spawned, 5.4.8 positions: Nafien (Timbermaw Hold, starts "Speak to Salfa" and
-- "Winterspring!"), Gorn One Eye (Timbermaw Hold, starts and ends "The Root of All Evil") and the Kaldorei, Quel'dorei
-- and Sin'dorei Spirits at Lake Kel'Theril (give and end "Pride of the Highborne" and the rest of the Kel'Theril
-- chain). 4.3.4 has the same positions for Nafien and Gorn One Eye and no spawn for the spirits.
DELETE FROM `creature` WHERE `guid` BETWEEN 8331800 AND 8331804;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8331800, 15395, 1, 1, 0, 0, 0, 0, 6821.85, -2082.51, 623.27, 3.15905, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Nafien
(8331801, 11555, 1, 1, 0, 0, 0, 0, 7030.72, -2106.32, 589.056, 4.34587, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Gorn One Eye
(8331802, 48658, 1, 1, 0, 0, 0, 0, 6543.91, -4110.53, 663.446, 0.366519, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Kaldorei Spirit
(8331803, 48659, 1, 1, 0, 0, 0, 0, 6538.57, -4115.86, 664.659, 4.50295, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Quel'dorei Spirit
(8331804, 48660, 1, 1, 0, 0, 0, 0, 6535.74, -4109.19, 662.943, 2.67035, 300, 0, 0, 0, 0, 0, 0, 0, 0); -- Sin'dorei Spirit

-- "A Yeti of Your Own" requires the deprecated quest 5163, which has no starter, so it can never be taken. 5.4.8
-- requires "Yetiphobia" (28722), the last quest of the current Umi Rumplesnicker chain; 4.3.4 still has 5163.
UPDATE `quest_template` SET `PrevQuestId` = 28722 WHERE `Id` = 8798;

-- "Fresh From The Hills": the 7 Pure Glacier Ice come from killing Solid Ice (13 spawns), which has no script. 5.4.8
-- casts the item creating spell 91850 on the killer; here the killer gets the item directly, and only while the quest is
-- taken (condition on the SmartAI event, source type 22, group = event id + 1).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 49233;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 49233 AND `id` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49233, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 56, 64662, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Solid Ice - On Death - Add Item Pure Glacier Ice');

-- "Turning the Earth": Tanrir's Overcharged Totem (spell 91647) hit on a Winterfall Earth Totem gives the credit. 4.3.4
-- has no script; the 5.4.8 row is used (its second row, an area explosion that casts spell 91633 on itself with a cast
-- flag this core does not have, is left out).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 49177;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 49177 AND `id` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49177, 0, 0, 0, 8, 0, 100, 0, 91647, 0, 0, 0, 0, 33, 49177, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Winterfall Earth Totem - On SpellHit Overcharged Totem - Quest Credit');

-- "The Arcane Storm Within": the Empowered Dreamcatcher (spell 93392) closes an Arcane Tesseract and gives the credit
-- (5 tesseracts). 4.3.4 has no script. 5.4.8 rows, with the stored target list replaced by the invoker and the
-- credit moved first in the chain.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 50291;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 50291 AND `id` BETWEEN 0 AND 4;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(50291, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 75, 93361, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arcane Tesseract - On Respawn - Add Aura Open Tesseract Effect'),
(50291, 0, 1, 2, 8, 0, 100, 0, 93392, 0, 60000, 60000, 0, 33, 50291, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Arcane Tesseract - On SpellHit Close Tesseract - Quest Credit'),
(50291, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 93361, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arcane Tesseract - Linked To Id 1 - Remove Aura Open Tesseract Effect'),
(50291, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 58538, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arcane Tesseract - Linked To Id 2 - Cast Arcane Explosion Visual'),
(50291, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Arcane Tesseract - Linked To Id 3 - Despawn after 10 sec');

-- "Get Them While They're Young": the 8 Frostsaber Cubs are collected by clicking the wild cubs (50 spawns): the click
-- spell 97178 creates the Frostsaber Cub item and the cub despawns. The cub had no spellclick flag or row. 5.4.8 data;
-- the click is allowed only while the quest is taken.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216, `AIName` = 'SmartAI' WHERE `entry` = 51681;
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 51681 AND `spell_id` = 97178;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(51681, 97178, 3, 0);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 51681 AND `id` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(51681, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Frostsaber Cub - On Spellclick - Despawn');

-- Conditions: the glacier ice only while "Fresh From The Hills" is taken, the cub click only while "Get Them While
-- They're Young" is taken.
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 49233 AND `SourceId` = 0)
    OR (`SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 51681 AND `SourceEntry` = 97178);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 49233, 0, 0, 9, 0, 28632, 0, 0, 0, 0, 0, '', 'Solid Ice - Pure Glacier Ice only while Fresh From The Hills is taken'),
(18, 51681, 97178, 0, 0, 9, 0, 29032, 0, 0, 0, 0, 0, '', 'Frostsaber Cub - click only while Get Them While They''re Young is taken');
