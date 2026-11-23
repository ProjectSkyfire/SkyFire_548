-- Deathknell (Undead starting zone): quest flow fixes from the Legends of Azeroth 5.4.8 data (the zone audit
-- findings DK-1 to DK-4, DK-6, DK-7 and DK-12). Where LoA drives a script in C++ (the three Wakening NPCs), the data
-- route from the 4.3.4 SmartAI is used instead, with LoA's template values.
-- New creature spawn guids: 8402093 to 8402098 (above the previous maximum 8402092).

-- ---------------------------------------------------------------------------------------------------------------
-- DK-1: Agatha (49044), giver of "Fresh out of the Grave" (24959), had no spawn and no AI. Her template is already
-- complete (level 10, faction 68, quest giver flag); only AIName is missing. Spawn and the two SmartAI rows (on quest
-- accept cast Val'kyr Resurrection 73524 on the player, then remove Rigor Mortis 73523) are the same in LoA and 4.3.4.
-- The addon (lying down, aura 49414) is the same in both references.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 49044 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` = 49044 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49044, 0, 0, 1, 19, 0, 100, 0, 24959, 0, 0, 0, 0, 11, 73524, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Agatha - On Quest Accept (24959) - Cast Spell ''Val''kyr Resurrection'''),
(49044, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 73523, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Agatha - On Quest Accept (24959) (Link) - Remove Aura ''Rigor Mortis''');

DELETE FROM `creature_template_addon` WHERE `entry` = 49044;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(49044, 0, 0, 50397184, 1, 0, '49414');

DELETE FROM `creature` WHERE `guid` = 8402093;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8402093, 49044, 0, 1, 0, 0, 0, 0, 1700.2, 1703.36, 137.198, 1.67552, 300, 0, 0, 0, 0, 0, 0, 0, 0); -- Agatha

-- ---------------------------------------------------------------------------------------------------------------
-- DK-2: Darnell (49425), turn-in of "No Better Than the Zombies" (24970) and giver of "Assault on the Rotbrain
-- Encampment" (24971), had no spawn. Template and quest links already match LoA; neither reference has AI for him.
-- Addon: always stand (LoA creature_template_addon VisFlags 1, 4.3.4 bytes1 65536). The 4.3.4-only aura 49415 is
-- left out because LoA does not have it.
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `creature_template_addon` WHERE `entry` = 49425;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(49425, 0, 0, 65536, 1, 0, NULL);

DELETE FROM `creature` WHERE `guid` = 8402094;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8402094, 49425, 0, 1, 0, 0, 0, 0, 1962.83, 1417.6, 67.4629, 3.56047, 300, 0, 0, 0, 0, 0, 0, 0, 0); -- Darnell

-- ---------------------------------------------------------------------------------------------------------------
-- DK-3: "The Wakening" (24960) asks the player to speak to Lilian Voss (38895), Marshal Redpath (49230) and Valdred
-- Moray (49231). Their templates were placeholders (level 1, faction 35, no gossip menu, no AI) and none was spawned.
-- Template values are LoA's (faction 68, npcflag gossip, unit_flags 33536, class 2, gossip menus 12483 / 12485 /
-- 12487, which the base data already has with their options and texts). LoA scripts the conversation in C++
-- (npc_lilian_voss, npc_marshal_redpath, npc_valdred_moray); here the 4.3.4 SmartAI is used so it does not depend on
-- those scripts: the last gossip option runs an action list that closes the gossip, credits the quest objective
-- (action 33, same creature id as the objective in quest_objective), talks, walks away and despawns.
-- Changes against the 4.3.4 rows: the idle poses (Lilian emote state 431, Valdred kneeling aura 68442) run on
-- reset (event 25) like the existing Lilian Voss 38910 script, because the 4.3.4 rows used event 54 (just summoned),
-- which never fires for a spawned creature; "Set state none" uses emote 0 where 4.3.4 had 30.
-- The gossip menus show only to a player on the quest (condition rows, same in LoA and 4.3.4).
-- Spawns are LoA's, respawn 60 seconds.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `faction_A` = 68, `faction_H` = 68, `npcflag` = 1, `unit_flags` = 33536, `unit_class` = 2, `baseattacktime` = 2000, `rangeattacktime` = 2000, `gossip_menu_id` = 12483, `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 38895; -- Lilian Voss
UPDATE `creature_template` SET `faction_A` = 68, `faction_H` = 68, `npcflag` = 1, `unit_flags` = 33536, `unit_class` = 2, `baseattacktime` = 2000, `rangeattacktime` = 2000, `gossip_menu_id` = 12485, `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 49230; -- Marshal Redpath
UPDATE `creature_template` SET `faction_A` = 68, `faction_H` = 68, `npcflag` = 1, `unit_flags` = 33536, `unit_class` = 2, `baseattacktime` = 2000, `rangeattacktime` = 2000, `gossip_menu_id` = 12487, `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 49231; -- Valdred Moray

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (38895, 49230, 49231) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (3889500, 4923000, 4923100) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38895, 0, 0, 0, 62, 0, 100, 0, 12484, 0, 0, 0, 0, 80, 3889500, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Gossip Select - Run Action List'),
(38895, 0, 1, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - On Reset - Set Emote State 431'),
(3889500, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - Action List - Close Gossip'),
(3889500, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38895, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - Action List - Quest Credit'),
(3889500, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - Action List - Say Line 0'),
(3889500, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - Action List - Set Emote State 0'),
(3889500, 9, 4, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1727.87, 1629.11, 118.862, 5.49784, 'Lilian Voss - Action List - Move To Position'),
(3889500, 9, 6, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lilian Voss - Action List - Despawn'),
(49230, 0, 0, 0, 62, 0, 100, 0, 12486, 0, 0, 0, 0, 80, 4923000, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Redpath - On Gossip Select - Run Action List'),
(4923000, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Marshal Redpath - Action List - Close Gossip'),
(4923000, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 49230, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Marshal Redpath - Action List - Quest Credit'),
(4923000, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Redpath - Action List - Say Line 0'),
(4923000, 9, 4, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1753.04, 1613.1, 113.051, 2.0146, 'Marshal Redpath - Action List - Move To Position'),
(4923000, 9, 6, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Redpath - Action List - Despawn'),
(49231, 0, 0, 0, 62, 0, 100, 0, 12489, 0, 0, 0, 0, 80, 4923100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - On Gossip Select - Run Action List'),
(49231, 0, 1, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 75, 68442, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - On Reset - Add Aura Kneel'),
(4923100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - Action List - Close Gossip'),
(4923100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 49231, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - Action List - Quest Credit'),
(4923100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - Action List - Say Line 0'),
(4923100, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 68442, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - Action List - Remove Aura Kneel'),
(4923100, 9, 4, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1689.71, 1674.79, 135.675, 0.349066, 'Valdred Moray - Action List - Move To Position'),
(4923100, 9, 5, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - Action List - Say Line 1'),
(4923100, 9, 6, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - Action List - Despawn');

-- Their lines (4.3.4 creature_text; LoA has the same texts plus a second variant for each).
DELETE FROM `creature_text` WHERE `entry` IN (38895, 49230, 49231);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38895, 0, 0, 'No. You''re lying! My father will protect me!', 12, 0, 100, 0, 0, 0, 'Lilian'),
(49230, 0, 0, 'Who are you calling a monster? You''re the monster! I''m just a man who died.', 12, 0, 100, 1, 0, 0, 'Marshal'),
(49231, 0, 0, 'I see. Well then, let''s get to work, $n! The Dark Lady needs us, right?', 12, 0, 100, 1, 0, 0, 'Valdred'),
(49231, 1, 0, 'Valdred Moray, reporting for duty, sir!', 14, 0, 100, 1, 0, 0, 'Valdred');

-- The gossip options show only to a player who has the quest (condition type 9 = quest taken).
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (12483, 12485, 12487) AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 12483, 0, 0, 0, 9, 0, 24960, 0, 0, 0, 0, 0, '', 'Lilian Voss - Show gossip option if the player has the quest The Wakening'),
(15, 12485, 0, 0, 0, 9, 0, 24960, 0, 0, 0, 0, 0, '', 'Marshal Redpath - Show gossip option if the player has the quest The Wakening'),
(15, 12487, 0, 0, 0, 9, 0, 24960, 0, 0, 0, 0, 0, '', 'Valdred Moray - Show gossip option if the player has the quest The Wakening');

DELETE FROM `creature` WHERE `guid` IN (8402095, 8402096, 8402097);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8402095, 38895, 0, 1, 0, 0, 0, 0, 1757.35, 1665.95, 121.21, 5.157, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Lilian Voss
(8402096, 49230, 0, 1, 0, 0, 0, 0, 1756.63, 1641.85, 117.204, 5.8555, 60, 0, 0, 0, 0, 0, 0, 0, 0), -- Marshal Redpath
(8402097, 49231, 0, 1, 0, 0, 0, 0, 1740.28, 1704.4, 128.785, 1.50333, 60, 0, 0, 0, 0, 0, 0, 0, 0); -- Valdred Moray

-- ---------------------------------------------------------------------------------------------------------------
-- DK-4: the Monk chain. "Scribbled Scroll" (31146) gets its starter (Shadow Priest Sarvis, LoA) and the Monk class
-- mask 512 (the base data had 0); "Tiger Palm" (31147) is restricted to Monk, 512 (the base data had 10 = Paladin and Rogue); Ting,
-- Strong of Stomach (63272), giver and turn-in of 31147 and turn-in of 31146, gets the LoA spawn. Ting's template is
-- already here and is not changed.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `RequiredClasses` = 512 WHERE `Id` IN (31146, 31147);

DELETE FROM `creature_queststarter` WHERE `id` = 1569 AND `quest` = 31146;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(1569, 31146);

DELETE FROM `creature` WHERE `guid` = 8402098;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8402098, 63272, 0, 1, 0, 0, 0, 0, 1875.44, 1570.03, 94.3131, 3.05108, 300, 0, 0, 0, 0, 0, 0, 0, 0); -- Ting, Strong of Stomach

-- ---------------------------------------------------------------------------------------------------------------
-- DK-6: prerequisites. "The Wakening" (24960) requires "Caretaker Caice" (28652), not "The Shadow Grave" (28608);
-- "Vital Intelligence" (24972) requires "Assault on the Rotbrain Encampment" (24971). Same in LoA and 4.3.4.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `PrevQuestId` = 28652 WHERE `Id` = 24960;
UPDATE `quest_template` SET `PrevQuestId` = 24971 WHERE `Id` = 24972;

-- ---------------------------------------------------------------------------------------------------------------
-- DK-7: "Welcome!" (5847, Brill Gift Voucher) had no turn-in; LoA has Claire Willower (11945), who is spawned.
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `creature_questender` WHERE `id` = 11945 AND `quest` = 5847;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(11945, 5847);

-- ---------------------------------------------------------------------------------------------------------------
-- DK-12: automatic next offer (NextQuestIdChain) where LoA and 4.3.4 agree: the class scrolls offer their class quest,
-- Trail-Worn Scroll offers The Thrill of the Hunt, every class quest offers "Novice Elreth" (28651), and "Scourge on
-- our Perimeter" (26801) offers nothing (the base data had 28651).
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `NextQuestIdChain` = 24969 WHERE `Id` = 3095;
UPDATE `quest_template` SET `NextQuestIdChain` = 24967 WHERE `Id` = 3096;
UPDATE `quest_template` SET `NextQuestIdChain` = 24966 WHERE `Id` = 3097;
UPDATE `quest_template` SET `NextQuestIdChain` = 24968 WHERE `Id` = 3099;
UPDATE `quest_template` SET `NextQuestIdChain` = 24964 WHERE `Id` = 24962;
UPDATE `quest_template` SET `NextQuestIdChain` = 28651 WHERE `Id` IN (24964, 24965, 24966, 24967, 24968, 24969);
UPDATE `quest_template` SET `NextQuestIdChain` = 0 WHERE `Id` = 26801;
