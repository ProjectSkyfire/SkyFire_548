-- Hacking the Wibson (27778): the Wibson Mainframe (46715) had no click to hack it. Using it while the quest is
-- active plays the hacking animation on the player and credits the hack.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 46715;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 46715;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(46715, 87033, 3, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 46715 AND `SourceEntry` = 87033;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 46715, 87033, 0, 0, 9, 0, 27778, 0, 0, 0, 0, 0, '', 'Spellclick allowed only with Hacking the Wibson taken');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 46715;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(46715, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 47028, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wibson Mainframe - On Spellclick - Quest Credit Wibson Hacked Credit');

-- Unlimited Potential (28351): the Reinforced Cage spell (93753) only has a dummy effect. It can be used on a
-- Cavorting Pygmy at 30% health or less, which is credited and removed. Conditions Based on Legends of Azeroth 5.4.8.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 93753;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 93753, 0, 0, 31, 1, 3, 51217, 0, 0, 0, 0, '', 'Sullah''s Pygmy Pen can target only Cavorting Pygmies'),
(17, 0, 93753, 0, 0, 38, 1, 30, 4, 0, 0, 0, 0, '', 'Sullah''s Pygmy Pen can target only a pygmy at 30% health or less');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 51217 AND `id` IN (2, 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(51217, 0, 2, 3, 8, 0, 100, 0, 93753, 0, 0, 0, 0, 33, 51188, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cavorting Pygmy - On Spellhit Sullah''s Pygmy Pen - Quest Credit Captured Pygmy Kill Credit'),
(51217, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cavorting Pygmy - Linked - Despawn');

-- Salhet's Gambit (28486): accepting the quest from Salhet summons the Ramkahen Ranger Captain (spell 91877 needs
-- Salhet as its summon position), who follows the player. At the top of the mountain he reaches the Ranger Captain
-- Kill Credit and the quest is credited. The group of rangers is not summoned.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (49253, 49345);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 91877;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 3, 91877, 0, 0, 31, 0, 3, 49345, 0, 0, 0, 0, '', 'Summon Ranger Captain uses Salhet as its target');

DELETE FROM `creature` WHERE `guid` = 9256800;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9256800, 49250, 1, 1, 0, 0, 0, 0, -10969.3, -907.321, 106.724, 0, 300, 0, 0, 0, 0, 0, 0, 33555200, 0);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (49253, 49345);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49253, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 85232, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ramkahen Ranger Captain - In Combat - Cast Shoot'),
(49253, 0, 1, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Ramkahen Ranger Captain - Between 0-15% Health - Flee For Assist (No Repeat)'),
(49253, 0, 2, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 29, 3, 0, 49250, 49250, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ramkahen Ranger Captain - On Just Summoned - Follow Summoner, Credit on Arrival at Ranger Captain Kill Credit'),
(49345, 0, 0, 0, 19, 0, 100, 0, 28486, 0, 0, 0, 0, 85, 91877, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Salhet - On Quest ''Salhet''s Gambit'' Accepted - Invoker Cast Summon Ranger Captain');
