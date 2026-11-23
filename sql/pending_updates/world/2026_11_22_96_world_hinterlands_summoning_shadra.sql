-- Summoning Shadra (26531 and 26558) cannot be finished. The player uses the Idol of Shadra (58784, spell 80469) at the
-- Northwestern, Southwestern and Eastern Temples of Shadra'Alor, and each altar bunny (43067, 43068, 43069) gives its
-- credit. The bunnies are spawned but had no SmartAI, so the idol never gave a credit, and the spell had no limit to
-- its target. It now works only on the three bunnies. Script and conditions are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (43067, 43068, 43069) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (43067, 43068, 43069) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43067, 0, 0, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 33, 43067, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadra''Alor Northwestern Altar Bunny - On SpellHit Idol of Shadra - Quest Credit'),
(43068, 0, 0, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 33, 43068, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadra''Alor Southwestern Altar Bunny - On SpellHit Idol of Shadra - Quest Credit'),
(43069, 0, 0, 0, 8, 0, 100, 0, 80469, 0, 0, 0, 0, 33, 43069, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shadra''Alor Eastern Altar Bunny - On SpellHit Idol of Shadra - Quest Credit');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 80469;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 80469, 0, 0, 31, 0, 3, 43067, 0, 0, 0, 0, '', 'Idol of Shadra - Target is the Northwestern Temple bunny'),
(13, 1, 80469, 0, 1, 31, 0, 3, 43069, 0, 0, 0, 0, '', 'Idol of Shadra - Target is the Eastern Temple bunny'),
(13, 1, 80469, 0, 2, 31, 0, 3, 43068, 0, 0, 0, 0, '', 'Idol of Shadra - Target is the Southwestern Temple bunny');
