-- Thousand Needles quest fixes.
-- Sources: 5.4.8 data for everything inserted below (spawns, SmartAI rows, gossip option, conditions, creature text,
-- event script, loot row, spellclick rows). The 4.3.4 data has none of these, or only the combat rows the database
-- already holds. Where 5.4.8 SmartAI uses action 203 (combat stop) it is written as 27 in this core.
-- Safe to run twice: every block deletes its own keys first or sets absolute values.

-- The Mudsprocket flight master has two quest gossip options and no script: "To the Summit" (Horde) and "To New
-- Thalanaar" (Alliance) need the option to give the quest credit, close the gossip and start the quest flight
-- (taxi paths 718 and 1974). The second option did not exist. Options are shown only while the quest is taken.
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 11372 AND `OptionID` = 2;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(11372, 2, 0, 'Dyslix, I need to get to New Thalanaar!', 40364, 1, 1, 0, 0, 0, 0, NULL, 0, 15050);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 11372 AND `SourceEntry` IN (1, 2);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11372, 1, 0, 0, 9, 0, 25478, 0, 0, 0, 0, 0, '', 'Dyslix Silvergrub gossip option 1 needs quest 25478 taken'),
(15, 11372, 2, 0, 0, 9, 0, 25479, 0, 0, 0, 0, 0, '', 'Dyslix Silvergrub gossip option 2 needs quest 25479 taken');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 40358;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 40358 AND `id` BETWEEN 0 AND 5;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40358, 0, 0, 1, 62, 0, 100, 0, 11372, 1, 0, 0, 0, 33, 39897, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dyslix Silvergrub - On Gossip Select - Quest Credit'),
(40358, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dyslix Silvergrub - Linked To Id 0 - Close Gossip'),
(40358, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 52, 718, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dyslix Silvergrub - Linked To Id 1 - Activate Taxi'),
(40358, 0, 3, 4, 62, 0, 100, 0, 11372, 2, 0, 0, 0, 33, 39897, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dyslix Silvergrub - On Gossip Select - Quest Credit'),
(40358, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dyslix Silvergrub - Linked To Id 0 - Close Gossip'),
(40358, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 52, 1974, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dyslix Silvergrub - Linked To Id 1 - Activate Taxi');

-- Quest NPCs that were never spawned, 5.4.8 positions: Rugfizzle and Drag Master Miglen (give and end "A Little
-- Payback"), the Spirit of Tony Two-Tusk (gives "Haunted"), Magatha Grimtotem (ends "To the Withering", gives the
-- Withering quests), the Captured Freewind Brave and Captured Feralas Sentinel (objectives of "The Brave and the
-- Bold" and "Save the Sentinel"), Caryssia Moonhunter and Rendow (Feralas, give and end the Alliance entry quests
-- "The Grimtotem are Coming" and "Two If By Boat"), Telaron Windflight (Feralas, gives the Alliance "Looming Threat")
-- and Koristrasza (ends both "Looming Threat" quests, inside the Razorfen Downs entrance; map 129 with the spawn mask
-- the other spawns of that map use). 4.3.4 and 5.4.8 have the same positions for Caryssia, Rendow and Telaron.
DELETE FROM `creature` WHERE `guid` BETWEEN 8331400 AND 8331409;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8331400, 40595, 1, 1, 0, 0, 0, 0, -6126.88, -3875.53, 6.24781, 0.925025, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Rugfizzle
(8331401, 40591, 1, 1, 0, 0, 0, 0, -6110.82, -3868.58, 6.24822, 3.36848, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Drag Master Miglen
(8331402, 41058, 1, 1, 0, 0, 0, 0, -6281.72, -4471.08, 87.6216, 2.04421, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Spirit of Tony Two-Tusk
(8331403, 47745, 1, 1, 0, 0, 0, 0, -5802.11, -2639.95, 89.0876, 4.83456, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Magatha Grimtotem
(8331404, 41460, 1, 1, 0, 0, 0, 0, -5438.34, -2423.82, 90.2828, 1.37881, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Captured Freewind Brave
(8331405, 41463, 1, 1, 0, 0, 0, 0, -5438.34, -2423.82, 90.2828, 1.37881, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Captured Feralas Sentinel
(8331406, 39946, 1, 1, 0, 0, 0, 0, -4464.86, -540.342, 5.65433, 0.568209, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Caryssia Moonhunter
(8331407, 39992, 1, 1, 0, 0, 0, 0, -4425.64, -706.717, 1.02763, 2.43919, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Rendow
(8331408, 44837, 129, 2, 0, 0, 0, 0, 2570.52, 1106.28, 47.7058, 4.36332, 7200, 0, 0, 0, 0, 0, 0, 0, 0), -- Koristrasza
(8331409, 40032, 1, 1, 0, 0, 0, 0, -3161.69, 1918.94, 45.774, 4.17134, 300, 0, 0, 0, 0, 0, 0, 0, 0); -- Telaron Windflight

-- The captured Freewind Brave and Feralas Sentinel are freed by clicking: the click gives the objective credit
-- (spell effect) and summons the freed NPC. Both need the spellclick flag and a click row; the Brave also despawns on
-- the click (5.4.8 SmartAI rows 0 and 2; row 1, a cosmetic set data on a bunny, is left out).
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216, `unit_flags` = 33536 WHERE `entry` IN (41460, 41463);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 41460;
DELETE FROM `npc_spellclick_spells` WHERE (`npc_entry`, `spell_id`) IN ((41460, 77584), (41463, 77583));
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(41460, 77584, 1, 0),
(41463, 77583, 1, 0);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41460 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41460, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 11, 77579, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Freewind Brave - On Respawn - Cast Captured Freewind Brave''s Anim & Invis'),
(41460, 0, 1, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Freewind Brave - On Spellclick - Despawn');

-- "Get Zherin!" and "Get Koalbeard!": the proposal comes from the rival leader. At 1 percent health he stops
-- fighting, turns friendly, becomes talkable and a Khan cheers; talking to him gives the proposal item (55808 Horde,
-- 55807 Alliance) and he despawns 10 seconds later. The 5.4.8 rows are added after the existing combat rows (ids from
-- 10). The creature guids in the data set rows are this database's guids of the two Khans and of the leaders.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (41204, 41205);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` IN (41196, 41199) AND `id` BETWEEN 10 AND 21) OR (`entryorguid` IN (41204, 41205) AND `id` BETWEEN 0 AND 1));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41196, 0, 10, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - On Aggro - Set Invincibility Hp 1'),
(41196, 0, 11, 12, 2, 0, 100, 1, 0, 1, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Between 0-1% Health - Combat Stop'),
(41196, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 102, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Linked To Id 1 - Set Regen Health 0'),
(41196, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 190, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Linked To Id 2 - Set Faction 190'),
(41196, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 81, 16777217, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Linked To Id 3 - Set NpcFlag 16777217'),
(41196, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Linked To Id 4 - Set Orientation'),
(41196, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 10, 212904, 41205, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Linked To Id 5 - Set Data 0 1'),
(41196, 0, 17, 18, 38, 0, 100, 0, 0, 1, 0, 0, 0, 11, 46957, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - On Data Set 0 1 - Cast Cosmetic - Stun'),
(41196, 0, 18, 19, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Linked To Id 7 - Say Line 0'),
(41196, 0, 19, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Linked To Id 8 - Despawn after 10 sec'),
(41196, 0, 20, 21, 64, 0, 100, 0, 0, 0, 0, 0, 0, 56, 55807, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - On Gossip Hello - Add Item Alliance''s Proposal'),
(41196, 0, 21, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kravel Koalbeard - Linked To Id 10 - Close Gossip'),
(41199, 0, 10, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - On Aggro - Set Invincibility Hp 1'),
(41199, 0, 11, 12, 2, 0, 100, 1, 0, 1, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Between 0-1% Health - Combat Stop'),
(41199, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 102, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Linked To Id 1 - Set Regen Health 0'),
(41199, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 190, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Linked To Id 2 - Set Faction 190'),
(41199, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 81, 16777217, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Linked To Id 3 - Set NpcFlag 16777217'),
(41199, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Linked To Id 4 - Set Orientation'),
(41199, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 10, 213153, 41204, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Linked To Id 5 - Set Data 0 1'),
(41199, 0, 17, 18, 38, 0, 100, 0, 0, 1, 0, 0, 0, 11, 46957, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - On Data Set 0 1 - Cast Cosmetic - Stun'),
(41199, 0, 18, 19, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Linked To Id 7 - Say Line 0'),
(41199, 0, 19, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Linked To Id 8 - Despawn after 10 sec'),
(41199, 0, 20, 21, 64, 0, 100, 0, 0, 0, 0, 0, 0, 56, 55808, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - On Gossip Hello - Add Item Horde''s Proposal'),
(41199, 0, 21, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Trackmaster Zherin - Linked To Id 10 - Close Gossip'),
(41204, 0, 0, 1, 38, 0, 100, 0, 0, 1, 0, 0, 0, 5, 37, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Khan Ablinh - On Data Set 0 1 - Play Emote 37'),
(41204, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 10, 213035, 41199, 0, 0, 0, 0, 0, 'Khan Ablinh - Linked To Id 0 - Set Data 0 1'),
(41205, 0, 0, 1, 38, 0, 100, 0, 0, 1, 0, 0, 0, 5, 37, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Khan Blizh - On Data Set 0 1 - Play Emote 37'),
(41205, 0, 1, 0, 61, 0, 100, 0, 0, 1, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 10, 212907, 41196, 0, 0, 0, 0, 0, 'Khan Blizh - Linked To Id 0 - Set Data 0 1');

DELETE FROM `creature_text` WHERE `entry` IN (41196, 41199) AND `groupid` = 0 AND `id` = 0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(41196, 0, 0, 'Kravel Koalbeard is stunned. Steal the Alliance''s Proposal now!', 41, 0, 100, 0, 0, 0, 'Kravel Koalbeard'),
(41199, 0, 0, 'Trackmaster Zherin is stunned. Steal the Horde''s Proposal now!', 41, 0, 100, 0, 0, 0, 'Trackmaster Zherin');

-- "Go Blow that Horn": blowing the Horn Mouthpiece summons Whrrrl (60 seconds), the second objective. The state change
-- event of this core follows the loot state, so 2 (activated) is the used state.
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 203248;
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 203248 AND `id` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(203248, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 12, 41334, 1, 60000, 0, 0, 0, 8, 0, 0, 0, -5324.01, -4373.12, 124.612, 0, 'Horn Mouthpiece - On GO State Change - Summon Whrrrl');

-- "The Mad Magus": the Soul Stick (spell 76873) hit gives the credit and adds the soul aura. Added after the four
-- combat rows (ids 4 and 5).
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41131 AND `id` IN (4, 5);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41131, 0, 4, 5, 8, 0, 100, 0, 76873, 0, 0, 0, 0, 33, 41130, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mad Magus Tirth - On SpellHit Soul Stick - Quest Credit'),
(41131, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 75, 77014, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mad Magus Tirth - Linked To Id 0 - Add Aura Mad Magus Tirth''s Soul');

-- "The Ancient Brazier": the Splithoof Brand sends event 24689, which summons the Aquarian and the brazier bunny and
-- gives the brazier credit. The Aquarian was a level 1 friendly placeholder with no drop: set the 4.3.4 level and
-- faction, loot (The Sacred Flame, quest item, always dropped) and the 5.4.8 summon rows and shout.
DELETE FROM `event_scripts` WHERE `id` = 24689;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(24689, 0, 10, 41236, 120000, 0, -4934.36, -2288.16, -72.2623, 0.122173),
(24689, 0, 10, 41242, 120000, 0, -4889.69, -2281.89, -67.5898, 2.72271),
(24689, 0, 8, 41242, 0, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `minlevel` = 42, `maxlevel` = 42, `faction_A` = 14, `faction_H` = 14, `lootid` = 41236, `AIName` = 'SmartAI' WHERE `entry` = 41236;
DELETE FROM `creature_loot_template` WHERE `entry` = 41236 AND `item` = 55979;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(41236, 55979, -100, 1, 0, 1, 1);
DELETE FROM `creature_text` WHERE `entry` = 41236 AND `groupid` = 0 AND `id` = 0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(41236, 0, 0, 'I know who you are, $n! I''ll not surrender the Sacred Flame to a filthy meatbag such as yourself!', 14, 0, 100, 0, 0, 0, 'Aquarian');
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 41236 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41236, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 21, 40, 0, 0, 0, 0, 0, 0, 'Aquarian - On Summoned - Say Line 0'),
(41236, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 40, 0, 0, 0, 0, 0, 0, 'Aquarian - Linked To Id 0 - Attack Start');

-- "We All Scream for Ice Cream... and then Die!": the Dead Employees are clicked for the credit (5.4.8: spellclick
-- 88478, credit, despawn after 2 seconds).
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216, `unit_flags` = 33536, `AIName` = 'SmartAI' WHERE `entry` = 47446;
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 47446 AND `spell_id` = 88478;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(47446, 88478, 1, 0);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 47446 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47446, 0, 0, 1, 73, 0, 100, 1, 0, 0, 0, 0, 0, 33, 47446, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dead Employee - On Spellclick - Give Kill Credit'),
(47446, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dead Employee');

-- The Freewind Brave and the Feralas Sentinel who start the Darkcloud Pinnacle quests ("No Weapons For You!" to "The
-- Captive Bride") are guardians summoned by an area aura: in the Darkcloud Pinnacle area after "The Brave and the
-- Bold" / "Save the Sentinel" is rewarded, until "Invoking the Serpent" is rewarded. Home had no spell area rows for
-- them, so the quest givers never existed. Rows are the same in 4.3.4 and 5.4.8.
DELETE FROM `spell_area` WHERE `spell` IN (84657, 84658) AND `area` = 2097;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(84657, 2097, 25871, 27329, 0, 0, 2, 1, 64, 11),
(84658, 2097, 25872, 27330, 0, 0, 2, 1, 64, 11);

-- Quest gates: the proposals, the two captives and the Dead Employees only answer a player who has the quest; the
-- 5.4.8 rows give the item or the credit to anyone and despawn the captive for everyone. Aquarian attacks the closest
-- player within 40 yards (the 5.4.8 row picked players between 35 and 40 yards away).
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 21 AND `SourceEntry` IN (41196, 41199) AND `SourceId` = 0)
    OR (`SourceTypeOrReferenceId` = 18 AND (`SourceGroup`, `SourceEntry`) IN ((41460, 77584), (41463, 77583), (47446, 88478)));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 21, 41196, 0, 0, 9, 0, 25757, 0, 0, 0, 0, 0, '', 'Kravel Koalbeard - proposal only while Get Koalbeard! is active'),
(22, 21, 41199, 0, 0, 9, 0, 25756, 0, 0, 0, 0, 0, '', 'Trackmaster Zherin - proposal only while Get Zherin! is active'),
(18, 41460, 77584, 0, 0, 9, 0, 25872, 0, 0, 0, 0, 0, '', 'Captured Freewind Brave - click only with the quest'),
(18, 41463, 77583, 0, 0, 9, 0, 25871, 0, 0, 0, 0, 0, '', 'Captured Feralas Sentinel - click only with the quest'),
(18, 47446, 88478, 0, 0, 9, 0, 28051, 0, 0, 0, 0, 0, '', 'Dead Employee - click only with the quest');
