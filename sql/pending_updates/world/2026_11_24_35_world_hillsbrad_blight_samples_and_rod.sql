-- Studies in Lethality (28324) and Helcular's Rod Giveth... (28331): the Lethality Analyzer (spell 89752) takes a sample
-- from a Blight Slime and Helcular's Rod (89821) empowers a Dark Ranger. Neither spell was limited to its target, so the
-- analyzer worked on anything and the rod on any creature, and the Dark Rangers never gave the quest credit. Added as in
-- Legends of Azeroth 5.4.8: the target conditions (the analyzer on a Blight Slime, 48136; the rod on a Dark Ranger,
-- 48269, that does not have Helcular's Ire, 89824, yet) and the Dark Rangers' credit rows (the rod stores the caster, the
-- Ire gives the credit once a minute and the ranger cheers). Their combat rows keep ids 0 to 2, the new rows follow.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 48269 AND `source_type` = 0 AND `id` IN (3, 4, 5);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48269, 0, 3, 0, 8, 0, 100, 0, 89821, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dark Ranger - On SpellHit Helcular''s Rod - Store Target List'),
(48269, 0, 4, 5, 8, 0, 100, 0, 89824, 0, 60000, 60000, 0, 33, 48269, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Dark Ranger - On SpellHit Helcular''s Ire - Quest Credit'),
(48269, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Ranger - Linked To Id 1 - Say Line Random');

DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 89752) OR (`SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 89821);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 89752, 0, 0, 31, 1, 3, 48136, 0, 0, 0, 0, '', 'Lethality Analyzer can only be used on a Blight Slime'),
(13, 1, 89821, 0, 0, 1, 0, 89824, 0, 0, 1, 0, 0, '', 'Helculars Rod cannot hit a Dark Ranger that already has Helculars Ire'),
(13, 1, 89821, 0, 0, 31, 0, 3, 48269, 0, 0, 0, 0, '', 'Helculars Rod can only hit Dark Rangers');

DELETE FROM `creature_text` WHERE `entry` IN (48269);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(48269, 0, 0, 'I am renewed!', 12, 0, 100, 5, 0, 0, ''),
(48269, 0, 1, 'Lordaeron is ours!', 12, 0, 100, 5, 0, 0, ''),
(48269, 0, 2, 'Our enemies will perish!', 12, 0, 100, 5, 0, 0, ''),
(48269, 0, 3, 'Such glorious, beautiful power!', 12, 0, 100, 5, 0, 0, ''),
(48269, 0, 4, 'Victory to the Forsaken!', 12, 0, 100, 5, 0, 0, '');
