-- Deeprun Rat Roundup (6661): the Rat Catcher's Flute casts "Melodious Rapture" (21050) on a Deeprun Rat (13016), which
-- turns into an Enthralled Deeprun Rat (13017) that gives the quest credit and follows the player until Monty (12997)
-- bashes it on turn-in. None of the three creatures had a script, so the five rats could not be counted. SmartAI rows,
-- Monty's line and the two spell target conditions are as in Legends of Azeroth 5.4.8 (the same rows exist in
-- TrinityCore 3.3.5). "Monty Bashes Rats" (21052) targets by entry, hence its source 13 condition.

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (12997, 13016, 13017);

DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (12997, 13016, 13017);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12997, 0, 0, 1, 20, 0, 100, 0, 6661, 0, 0, 0, 0, 11, 21052, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Monty - On Quest ''Deeprun Rat Roundup'' Finished - Cast ''Monty Bashes Rats (DND)'''),
(12997, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Monty - On Quest ''Deeprun Rat Roundup'' Finished - Say Line 0'),
(13016, 0, 0, 1, 8, 0, 100, 1, 21050, 0, 0, 0, 0, 12, 13017, 2, 120000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deeprun Rat - On Spellhit ''Melodious Rapture'' - Summon Creature ''Enthralled Deeprun Rat'' (No Repeat)'),
(13016, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 100, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deeprun Rat - On Spellhit ''Melodious Rapture'' - Despawn In 100 ms (No Repeat)'),
(13017, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enthralled Deeprun Rat - On Just Summoned - Set Faction 35'),
(13017, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 13017, 0, 0, 0, 0, 0, 21, 5, 0, 0, 0, 0, 0, 0, 'Enthralled Deeprun Rat - On Just Summoned - Quest Credit ''Deeprun Rat Roundup'''),
(13017, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 21051, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enthralled Deeprun Rat - On Just Summoned - Cast ''Melodious Rapture Visual (DND)'''),
(13017, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 21, 5, 0, 0, 0, 0, 0, 0, 'Enthralled Deeprun Rat - On Just Summoned - Start Follow Closest Player'),
(13017, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enthralled Deeprun Rat - On Just Summoned - Set Event Phase 1'),
(13017, 0, 5, 0, 8, 1, 100, 0, 21052, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enthralled Deeprun Rat - On Spellhit ''Monty Bashes Rats (DND)'' - Despawn Instant');

DELETE FROM `creature_text` WHERE `entry`=12997 AND `groupid`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(12997, 0, 0, 'Into the box me pretties! Thas it. One by one ye go.', 12, 0, 100, 0, 0, 0, 'Monty');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13, 17) AND `SourceEntry` IN (21050, 21052);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 21052, 0, 0, 31, 0, 3, 13017, 0, 0, 0, 0, '', 'Monty Bashes Rats targets the Enthralled Deeprun Rats'),
(17, 0, 21050, 0, 0, 31, 1, 3, 13016, 0, 0, 0, 0, '', 'Melodious Rapture can only be used on a Deeprun Rat');
