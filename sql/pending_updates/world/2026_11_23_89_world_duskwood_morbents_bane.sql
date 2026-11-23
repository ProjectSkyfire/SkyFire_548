-- Morbent's Bane (26754): the player uses the item Morbent's Bane (60212, spell 82130 Sacred Cleansing) on
-- Morbent Fel (43761) to strip his Unholy Shield, which turns him into the Weakened Morbent Fel (43762) that attacks the
-- player and counts as the kill. The spell could be used on anything and Morbent Fel had no row that reacts to it, so the
-- shield stayed up and the quest could not be finished. Spell target condition and the three rows are as in Legends of
-- Azeroth 5.4.8; the weakened template (a level 1 friendly placeholder) takes level and faction of the source. The
-- existing Touch of Death, Presence of Death and Unholy Shield rows are kept (renumbered). The source also kills the
-- ghouls around Morbent when he dies; that is not taken over.

UPDATE `creature_template` SET `minlevel` = 25, `maxlevel` = 25, `faction_A` = 21, `faction_H` = 21 WHERE `entry` = 43762 AND `ScriptName` = '';

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 82130;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 82130, 0, 0, 31, 0, 3, 43761, 0, 0, 0, 0, '', 'Sacred Cleansing - Target is Morbent Fel');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43761;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43761, 0, 0, 1, 8, 0, 100, 0, 82130, 0, 0, 0, 0, 28, 8909, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Morbent Fel - On SpellHit Sacred Cleansing - Remove Aura Unholy Shield'),
(43761, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 36, 43762, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Morbent Fel - Linked To Id 0 - Update Template On Weakened Morbent Fel'),
(43761, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Morbent Fel - Linked To Id 1 - Attack Start'),
(43761, 0, 3, 0, 2, 0, 100, 0, 0, 55, 22000, 26000, 0, 11, 3108, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Morbent Fel - Between 0-55% Health - Cast ''Touch of Death'''),
(43761, 0, 4, 0, 0, 0, 100, 0, 6000, 9000, 18000, 20000, 0, 11, 3109, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Morbent Fel - In Combat - Cast ''Presence of Death'''),
(43761, 0, 5, 0, 11, 0, 100, 1, 0, 0, 0, 0, 0, 11, 8909, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Morbent Fel - On Respawn - Cast ''Unholy Shield'' (No Repeat)');
