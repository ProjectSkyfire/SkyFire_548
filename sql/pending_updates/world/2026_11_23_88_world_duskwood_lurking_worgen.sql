-- A Curse We Cannot Lift (26720): the player weakens the Lurking Worgen (43814) and uses Harris's Ampule (spell 82058) on
-- it; the worgen is captured (it disappears) and the player gets the credit "Lurking Worgen captured" (43860). The
-- ampule could be used on anything at any health and the worgen had no script, so no credit was given.
-- Spell target conditions (the worgen, at most 20% health) and rows are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 43814 AND `ScriptName` = '';

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 82058;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 82058, 0, 0, 31, 0, 3, 43814, 0, 0, 0, 0, '', 'Harris''s Ampule - Target is Lurking Worgen'),
(13, 1, 82058, 0, 0, 38, 0, 20, 4, 0, 0, 0, 0, '', 'Harris''s Ampule - Target has 20 percent health or less');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (43814);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43814, 0, 0, 1, 8, 0, 100, 1, 82058, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lurking Worgen - On Spellhit Harris''s Ampule - Despawn'),
(43814, 0, 1, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 33, 43860, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lurking Worgen - Linked - Quest Credit');
