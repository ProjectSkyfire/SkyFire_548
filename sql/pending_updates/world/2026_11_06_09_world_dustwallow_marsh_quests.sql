-- Dustwallow Marsh quest fixes. Reference data: 4.3.4 for the event scripts, the Raptor Capture credit rows and the
-- gryphon survey gossip script; 5.4.8 for the spawns, the Ogre Remains and Captured Totem rows, the Daggermaw credit
-- row and the quest ids of the Agitator gossip condition and the Mok'Morokk rows. The other quest id corrections are
-- the current quest ids of scripts that home and 4.3.4 still key to the old quests. Safe to run twice: every block
-- deletes its own keys first or sets absolute values.

-- Renn McGill (starts "Secondhand Diving Gear" and "Jaina Must Know", ends "Renn McGill") had no spawn, so the
-- Alliance chain from "Defias in Dustwallow?" stopped. Lurking Shark ("Nat's Bargain", kill target) and Gavis
-- Greyshield ("The End of the Deserters", capture target) were not spawned either. Positions are the 5.4.8 spawns.
DELETE FROM `creature` WHERE `guid` BETWEEN 8331200 AND 8331202;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8331200, 23569, 1, 1, 0, 0, 0, 0, -2629.72, -4321.5, -1.99871, 2.3911, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8331201, 23928, 1, 1, 0, 0, 0, 0, -4234.04, -3883.99, -1.95866, 4.62589, 60, 4, 0, 0, 0, 1, 0, 0, 0),
(8331202, 23941, 1, 1, 0, 0, 0, 0, -4030.78, -4979.28, 7.86344, 5.14872, 60, 3, 0, 0, 0, 1, 0, 0, 0);

-- "Secondhand Diving Gear" needs the Damaged Diving Gear and the Tool Kit: both chest objects (186273 and 186272)
-- have a template and loot but no spawn in home or 4.3.4. 5.4.8 spawns.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 930060 AND 930061;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(930060, 186273, 1, 1, 0, 0, -2671.49, -4263.42, 3.59574, 1.46608, 0, 0, 0.66913, 0.743145, 300, 100, 1),
(930061, 186272, 1, 1, 0, 0, -2691.45, -4279.76, 3.60655, 0.907571, 0, 0, 0.438371, 0.898794, 300, 100, 1);

-- Event scripts that are missing. Event 15363 (Raptor Bait, "Unleash the Raptors") summons nine Captured Raptors
-- and gives the credit; event 15385 (Wreath, "Peace at Last") summons Lynn and Jimmy Hyal. The 4.3.4 rows.
DELETE FROM `event_scripts` WHERE `id` IN (15363, 15385);
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(15363, 0, 8, 23727, 1, 0, 0, 0, 0, 0),
(15363, 0, 10, 23741, 600000, 0, -2617.89, -3277.13, 31.9344, 0.541052),
(15363, 0, 10, 23741, 600000, 0, -2621.78, -3275.27, 30.6254, 0.296706),
(15363, 0, 10, 23741, 600000, 0, -2620.45, -3279.92, 31.8134, 0.296706),
(15363, 0, 10, 23741, 600000, 0, -2482.96, -3240.59, 40.9678, 2.49582),
(15363, 0, 10, 23741, 600000, 0, -2481.63, -3245.02, 43.0934, 2.58309),
(15363, 0, 10, 23741, 600000, 0, -2479.41, -3239, 40.0641, 2.53073),
(15363, 0, 10, 23741, 600000, 0, -2524.42, -3128.14, 29.7597, 0.331613),
(15363, 0, 10, 23741, 600000, 0, -2527.19, -3125.76, 29.5518, 0.383972),
(15363, 0, 10, 23741, 600000, 0, -2526.19, -3131.12, 29.0088, 0.279253),
(15385, 3, 10, 23768, 180000, 0, -3532.56, -4316.76, 7.04, 2.98),
(15385, 3, 10, 23769, 180000, 0, -3533.17, -4318.34, 7.05, 2.51);

-- "Raptor Captor": the Bloodfen Raptor and Screecher have SmartAI but no rows, so the Sturdy Rope (spell 42325)
-- never gives the Raptors Captured credit (spell 42337). The 4.3.4 rows: credit on the spell hit and the emote at
-- low health.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (4351, 4352);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4351, 0, 0, 0, 8, 0, 100, 0, 42325, 0, 0, 0, 0, 11, 42337, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfen Raptor - On Spellhit Capture Raptor - Cast Raptor Capture Credit'),
(4351, 0, 1, 0, 2, 0, 100, 1, 0, 20, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodfen Raptor - Between 0-20% Health - Say Line 0'),
(4352, 0, 0, 0, 8, 0, 100, 0, 42325, 0, 0, 0, 0, 11, 42337, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfen Screecher - On Spellhit Capture Raptor - Cast Raptor Capture Credit'),
(4352, 0, 1, 0, 2, 0, 100, 1, 0, 20, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodfen Screecher - Between 0-20% Health - Say Line 0');

-- Cassa Crimsonwing ("Survey Alcaz Island"): the C++ script bound to her shows the gryphon option only for the old
-- quest 11142, so the ride never starts for the current quest 27239. Use the 4.3.4 SmartAI (cast the survey spell on
-- option 0 and close the gossip) with the database gossip option, and show the option only while quest 27239 is
-- taken (4.3.4 uses the old id 11142 for that condition). The two rows are the 4.3.4 rows with the unused event
-- flag 512 cleared.
UPDATE `creature_template` SET `ScriptName` = '', `AIName` = 'SmartAI' WHERE `entry` = 23704;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 23704;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(23704, 0, 0, 1, 62, 0, 100, 0, 8782, 0, 0, 0, 0, 11, 42295, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cassa Crimsonwing - On Gossip Option 0 Selected - Cast Spell 42295'),
(23704, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cassa Crimsonwing - On Linked Actions - Close Gossip');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 8782 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 8782, 0, 0, 0, 9, 0, 27239, 0, 0, 0, 0, 0, '', 'Show gossip option only if player has taken quest 27239');

-- "Traitors Among Us": the gossip option that exposes a Deserter Agitator needs the quest taken, but its condition
-- names the old quest 11126 (4.3.4), so the option never shows. 5.4.8 uses 27210.
UPDATE `conditions` SET `ConditionValue1` = 27210 WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 8762 AND `SourceEntry` = 0 AND `ConditionTypeOrReference` = 9 AND `ConditionValue1` IN (11126, 27210);

-- "Challenge Overlord Mok'Morokk": Mok'Morokk's quest-accepted event and the quest credit in his action list name
-- the old quest 1173, so accepting 27418 does not start the fight and the quest cannot be credited. 5.4.8 uses 27418.
UPDATE `smart_scripts` SET `event_param1` = 27418 WHERE `entryorguid` = 4500 AND `source_type` = 0 AND `id` = 1 AND `event_type` = 19 AND `event_param1` IN (1173, 27418);
UPDATE `smart_scripts` SET `action_param1` = 27418 WHERE `entryorguid` = 450001 AND `source_type` = 9 AND `id` = 4 AND `action_type` = 15 AND `action_param1` IN (1173, 27418);

-- "Questioning Reethe": Ogron's fail-on-death and quest-accepted rows and Paval Reethe's quest credit name the old
-- quest 1273, so the escort never starts and the quest (completed by that credit) cannot be finished. 4.3.4 and
-- 5.4.8 both keep 1273, the current quest is 27261.
UPDATE `smart_scripts` SET `action_param1` = 27261 WHERE `entryorguid` = 4983 AND `source_type` = 0 AND `id` = 0 AND `action_type` = 6 AND `action_param1` IN (1273, 27261);
UPDATE `smart_scripts` SET `event_param1` = 27261 WHERE `entryorguid` = 4983 AND `source_type` = 0 AND `id` = 1 AND `event_type` = 19 AND `event_param1` IN (1273, 27261);
UPDATE `smart_scripts` SET `action_param1` = 27261 WHERE `entryorguid` = 498001 AND `source_type` = 9 AND `id` = 0 AND `action_type` = 15 AND `action_param1` IN (1273, 27261);

-- "Spirits of Stonemaul Hold": using the Energized Totem on the Ogre Remains (27 objects, goober state change)
-- should call up a Stonemaul Spirit (the kill target, no spawn). 5.4.8 SmartGameObjectAI row that summons it
-- for 60 seconds.
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 186332;
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 186332;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(186332, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 12, 23786, 3, 60000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogre Remains - On State Active - Summon Creature Stonemaul Spirit');

-- "The Grimtotem Weapon": the Captured Totem (summoned by the quest item, spell 42452) is the objective creature and
-- nothing gives the credit. 5.4.8: a Drywallow Daggermaw death gives the killer the credit, and the totem has its
-- periodic aura and react state rows (the 5.4.8 row that disables evade uses an action this core does not have and
-- is left out).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 23811;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 23811;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(23811, 0, 0, 1, 11, 0, 100, 1, 0, 0, 0, 0, 0, 75, 42464, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Totem - On Spawn - Add Aura Captured Totem Periodic'),
(23811, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Totem - Linked To Id 0 - Set React State Aggressive'),
(23811, 0, 2, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Totem - On Aggro - Disable Combat Movement');
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 4345 AND `id` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4345, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 33, 23811, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Drywallow Daggermaw - On Death - Quest Credit');

-- "Mudrock Soup and Bugs" (Morgan Stern) requires quest 1260, which is a deprecated quest nobody offers, so the quest
-- and its follow-up "... and Bugs" can never be taken. 4.3.4 has no prerequisite.
UPDATE `quest_template` SET `PrevQuestId` = 0 WHERE `Id` = 1204 AND `PrevQuestId` IN (1260, 0);
